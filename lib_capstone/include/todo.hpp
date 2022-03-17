#ifndef INCLUDED_TODO_HPP
#define INCLUDED_TODO_HPP

#include <string>
#include <cstdint>

#include "encoding.hpp"

namespace todo {

	static int counter = 0;

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
		NONE,
		IN_PROGRESS,
		DONE,
		CANCELED,

		DEFAULT = NONE,
	};

	class Item {
		public:
			Priority priority = Priority::DEFAULT;
			Status status = Status::DEFAULT;
			Unicode::string_t title = encoding::decode_literal((std::string) "New Item " + std::to_string(counter++));

			Item() {}
	};

	const char* to_string(const Priority &p);
	const char* to_string(const Status &p);

};

#endif
