#ifndef INCLUDED_TWIG_STRING_HPP
#define INCLUDED_TWIG_STRING_HPP

#include "unicode.hpp"

namespace twig::string {

	inline unsigned short get_width(Unicode::codepoint_t cp) {
		using X = Unicode::EastAsianWidth;
		const auto width_prop = Unicode::get_east_asian_width(cp);
		switch(width_prop) {
			case X::F:
			case X::W:
				return 2;
			default:
				return 1;
		}
	}

	template <typename T>
	inline unsigned short get_width(T s, size_t start, size_t end) {
		unsigned short sum = 0;
		for (size_t i = start; i < end; i++) {
			sum += get_width(s[i]);
		}
		return sum;
	}

	inline unsigned short get_width(Unicode::string_t s) {
		return get_width(s, 0, s.size());
	}

}

#endif
