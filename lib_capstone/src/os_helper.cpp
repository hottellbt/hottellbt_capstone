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

void OS::Subprocess::open_editor() {

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

		// get the exe's path into a char* as the first arg to the process
		const std::string editor_exe_string = (*editor_exe).string();
		const size_t size = editor_exe_string.size();
		char editor_exe_c_str[size + 1];
		std::strcpy(editor_exe_c_str, editor_exe_string.c_str());
		editor_exe_c_str[size] = 0;

		char *argv[2] { editor_exe_c_str, (char*) nullptr }; 
		run(editor_exe_c_str, argv);

		return;

	}

	throw subprocess_error("editor not found: " + editor.string());
}
