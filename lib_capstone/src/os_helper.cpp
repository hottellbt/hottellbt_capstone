#include "os_helper.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <string>
#include <optional>

#include <cstdio>
#include <cstdlib>
#include <cstring>

char* OS::getenv(const char* var) {
	return std::getenv(var);
}

std::optional<std::filesystem::path> OS::find_executable(const std::filesystem::path exe_name) {

	if (std::filesystem::exists(exe_name)) {
		return exe_name;
	}

	const char* PATH_ENV = OS::getenv("PATH");

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

using OS::Subprocess::subprocess_error;

void OS::Subprocess::run(
		const char* path,
		char* const* argv) {

	pid_t pid;

	if ((pid = fork()) == -1) {
		throw subprocess_error("fork");
	} else if (pid != 0) {
		int status;
		int options = 0;
		waitpid(pid, &status, options);
		return;
	}

	// we are in child thread

	// the exec replaces the current process image
	// i.e. we will never return on success. this is effectively exit()

	execv(path, argv);
	throw subprocess_error("execv");
}

void OS::Subprocess::open_editor(std::filesystem::path file) {

	std::filesystem::path editor;

	const char* EDITOR_ENV = OS::getenv("EDITOR");

	if (EDITOR_ENV != nullptr) {
		editor = EDITOR_ENV;
	} else {
		// vi is probably installed, right?
		editor = "vi"; 
	}

	std::optional<std::filesystem::path> editor_exe = OS::find_executable(editor);

	if (editor_exe) {

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

		return;

	}

	throw subprocess_error("editor not found: " + editor.string());
}
