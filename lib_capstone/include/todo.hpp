#ifndef INCLUDED_TODO_HPP
#define INCLUDED_TODO_HPP

#include <string>
#include <cstdint>

namespace todo {

	enum class Priority : int8_t {
		LOWEST  = -3,
		LOWER   = -2,
		LOW     = -1,
		NORMAL  = 0,
		HIGH    = 1,
		HIGHER  = 2,
		HIGHEST = 3,

		DEFAULT = NORMAL,
	};

	enum Status : uint8_t {
		DOING,
		NO_STATUS,
		DONE,
		CANCELED,

		DEFAULT = NO_STATUS,
	};

	class Item {
		public:
			Priority priority = Priority::DEFAULT;
			Status status = Status::DEFAULT;
			std::string title_utf8 = "New Item";

			Item() {}
	};

	const char* to_string(const Priority &p);
	const char* to_string(const Status &p);

};

#endif
