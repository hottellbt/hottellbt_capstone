#ifndef INCLUDED_SUBPROCESS_HPP
#define INCLUDED_SUBPROCESS_HPP

#include <stdexcept>

namespace Subprocesses {

	class subprocess_error : public std::runtime_error {
		public:
			subprocess_error(const std::string &msg) : runtime_error(msg) {}
			subprocess_error(const char* msg) : runtime_error(msg) {}
	};

	void run(const char *path, char* const* argv);

	void open_editor();

}

#endif
