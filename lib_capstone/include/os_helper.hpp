#ifndef INCLUDED_OS_HELPER_HPP
#define INCLUDED_OS_HELPER_HPP

#include <optional>
#include <string>

namespace OS {

	const char* getenv(const char* var);

	const char* getenv_or(
			const char* var,
			const char* fallback) {
		const char* ret = getenv(var);
		if (ret == nullptr) return fallback;
		return ret;
	}

	std::optional<std::string> find_in_path(const char* file);

	namespace Path {

		bool exists(const char* path);

		bool access(
				const char *path,
				const bool read,
				const bool write,
				const bool execute);

	}

};

#endif
