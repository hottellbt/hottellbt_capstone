#include "twig_os.hpp"
#include "twig_app.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <string>
#include <optional>
#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

inline std::string comment_with_errno(const std::string &comment, int errorno) {
	std::string ret = comment;

	if (errno != 0) {
		ret += ", errno=";
		ret += std::to_string(errno);
		ret += " ";
		ret += std::strerror(errno);
	}

	return ret;
}

inline std::string comment_with_auto_errno(const std::string &comment) {
	int my_errno = errno;
	errno = 0;
	return comment_with_errno(comment, my_errno);
}

char* twig::os::getenv(const char* var) {
	return std::getenv(var);
}

std::optional<std::filesystem::path> twig::os::find_executable(const std::filesystem::path exe_name) {

	if (std::filesystem::exists(exe_name)) {
		return exe_name;
	}

	const char* PATH_ENV = twig::os::getenv("PATH");

	if (PATH_ENV == nullptr) {
		return std::nullopt;
	}

	const size_t PATH_LEN = std::strlen(PATH_ENV);
	char path[PATH_LEN + 1];
	std::strcpy(path, PATH_ENV);
	path[PATH_LEN] = 0;

	char* path_dir = strtok(path, ":");

	while (path_dir != nullptr) {

		auto guess = path_dir / exe_name;

		if (std::filesystem::exists(guess)) {
			return guess;
		}

		path_dir = strtok(nullptr, ":");
	}

	return std::nullopt;
}

using twig::os::subprocess::subprocess_error;

void twig::os::subprocess::run(
		const char* path,
		char* const* argv) {

	pid_t pid;

	twig::curses::exit();

	if ((pid = fork()) == -1) {
		throw subprocess_error("fork");
	} else if (pid != 0) {
		int status;
		int options = 0;

		int wait_ret;
		do {
			wait_ret = waitpid(pid, &status, options);
		} while (wait_ret == -1 && errno == EINTR);

		if (wait_ret == -1) {
			throw new subprocess_error("waitpid");
		}

		twig::curses::init();

		return;
	}

	// we are in child thread

	// the exec replaces the current process image
	// i.e. we will never return on success. this is effectively exit()

	execv(path, argv);
	throw subprocess_error("execv");
}

void twig::os::subprocess::open_editor(std::filesystem::path file) {

	std::filesystem::path editor;

	const char* EDITOR_ENV = twig::os::getenv("EDITOR");

	if (EDITOR_ENV != nullptr) {
		editor = EDITOR_ENV;
	} else {
		// vi is probably installed, right?
		editor = "vi"; 
	}

	std::optional<std::filesystem::path> editor_exe = twig::os::find_executable(editor);

	if (!editor_exe) {
		throw subprocess_error("editor not found: " + editor.string());
	}

	// reusable
	size_t size;

	// get the exe's path into a char* as the first arg to the process
	const std::string exe_path_string = (*editor_exe).string();
	size = exe_path_string.size();
	char arg0[size + 1];
	std::strcpy(arg0, exe_path_string.c_str());
	arg0[size] = 0;

	// get the path as the second arg to the process
	const std::string file_path_string = file.string();
	size = file_path_string.size();
	char arg1[size + 1];
	std::strcpy(arg1, file_path_string.c_str());
	arg1[size] = 0;

	char *argv[3] { arg0, arg1, (char*) nullptr }; 
	run(arg0, argv);

}

std::string twig::os::subprocess::open_editor_line(
		const char* line,
		const char* suffix,
		const char* prefix) {

	// we will put what we read into this string
	std::string ret;

	// cobble together the c-string for mkstemps
	const int prefix_len = std::strlen(prefix);
	const int suffix_len = std::strlen(suffix);
	const int tmp_buffer_len = prefix_len + suffix_len + 6;

	char tmp_buffer[tmp_buffer_len + 1];
	char* tmp_buffer_idx = tmp_buffer;

	strncpy(tmp_buffer_idx, prefix, prefix_len);
	tmp_buffer_idx += prefix_len;
	strncpy(tmp_buffer_idx, "XXXXXX", 6);
	tmp_buffer_idx += 6;
	strncpy(tmp_buffer_idx, suffix, suffix_len);
	tmp_buffer[tmp_buffer_len] = 0;

	// invoke mkstemps
	int fd = mkstemps(tmp_buffer, suffix_len);
	if (fd == -1) {
		std::string msg = "mkstemps ";
		msg += tmp_buffer;
		throw std::runtime_error(comment_with_auto_errno(msg));
	}

	// error handling
	bool errored = false;
	std::string error_msg;

	try {

		// write to the file that mkstemps gave us
		// we can use the file descriptor to make life easy
		if (write(fd, line, strlen(line)) == -1) {
			throw std::runtime_error(comment_with_auto_errno("write"));
		}

		// open the user's editor
		twig::os::subprocess::open_editor((std::string) tmp_buffer);

		// now read what they wrote to the file
		// i've found we have to use the path rather than the file descriptor
		FILE* fp = fopen(tmp_buffer, "r");

		if (fp == nullptr) {
			throw std::runtime_error(comment_with_auto_errno((std::string) "fopen " + tmp_buffer));
		}

		static const int read_buffer_len = 128;
		char read_buffer[read_buffer_len+1];

		while (fgets(read_buffer, read_buffer_len, fp) != nullptr) {
			ret += read_buffer;
		}

	} catch (const std::runtime_error &e) {

		errored = true;
		error_msg = e.what();

	}

	// cleanup, even if we errored earlier

	if (close(fd) == -1) {
		if (errored) {
			error_msg += "\n";
		}
		error_msg += comment_with_auto_errno("Failed to close file descriptor: " + std::to_string(fd));
		errored = true;
	}

	if (unlink(tmp_buffer) == -1) {
		if (errored) {
			error_msg += "\n";
		}
		error_msg += comment_with_auto_errno((std::string) "Failed to unlink: " + tmp_buffer);
		errored = true;
	}

	// ok, if there was an error, lets throw it now
	if (errored) {
		throw std::runtime_error(error_msg);
	}

	// we did it :)
	return ret;
}
