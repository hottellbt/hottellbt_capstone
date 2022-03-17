#ifndef INCLUDED_TWIG_OS_HPP
#define INCLUDED_TWIG_OS_HPP

#include <optional>
#include <string>
#include <filesystem>
#include <stdexcept>

namespace twig::os {

	char* getenv(const char* var);

	std::optional<std::filesystem::path> find_executable(const std::filesystem::path name);

	namespace subprocess {

		class subprocess_error : public std::runtime_error {
			public:
				subprocess_error(const std::string &msg) : runtime_error(msg) {}
				subprocess_error(const char* msg) : runtime_error(msg) {}
		};

		void run(const char *path, char* const* argv);

		void open_editor(std::filesystem::path file);

		std::string open_editor_line(
			const char* line,
			const char* suffix = ".txt",
			const char* prefix = "tmpedit");

		inline std::string open_editor_line(const std::string &line) {
			return open_editor_line(line.c_str());
		}

	}

};

#endif
