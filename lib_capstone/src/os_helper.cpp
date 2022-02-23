#include "os_helper.hpp"

#include <string>
#include <optional>
#include <cstdlib>
#include <unistd.h>

const char* OS::getenv(const char* var) {
	return std::getenv(var);
}

const char* find_in_path(const std::string &) {
	const char* PATH = OS::getenv("PATH");

	if (PATH == nullptr) {
		return std::nullopt;
	}

	const size_t
	char* path_dir;
	do {

		path_dir = strtok(PATH, ":");

	} while (path_dir != nullptr);
}

bool OS::Path::exists(const char *path) {
	return access(path, F_OK) == 0;
}

bool OS::Path::access(
		const char *path,
		const bool read,
		const bool write,
		const bool execute) {

	int amode;
	if (read)    flag |= R_OK;
	if (write)   flag |= W_OK;
	if (execute) flag |= X_OK;
	return access(path) == 0;
}

