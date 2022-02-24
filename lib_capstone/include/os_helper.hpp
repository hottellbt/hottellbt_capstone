#ifndef INCLUDED_OS_HELPER_HPP
#define INCLUDED_OS_HELPER_HPP

#include <optional>
#include <string>
#include <filesystem>

namespace OS {

	char* getenv(const char* var);

	std::optional<std::filesystem::path> find_executable(const std::filesystem::path name);

};

#endif
