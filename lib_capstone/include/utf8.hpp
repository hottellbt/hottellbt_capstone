#ifndef INCLUDED_UTF8_HPP
#define INCLUDED_UTF8_HPP

#ifndef INCLUDED_UNICODE_HPP
#include "unicode.hpp"
#endif

namespace UTF8 {

	inline bool is_appendee(char c) {
		return (c & 0xC0) == 0x80;
	}

	Unicode::string_t decode(const std::string &s);

	std::string encode(const Unicode::string_t &s);

};

#endif
