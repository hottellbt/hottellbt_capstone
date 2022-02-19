#ifndef INCLUDED_UTF8_HPP
#define INCLUDED_UTF8_HPP

#ifndef INCLUDED_UNICODE_HPP
#include "unicode.hpp"
#endif

#include <stdexcept>

namespace UTF8 {

	Unicode::string_t decode(const std::string &s);

	std::string encode(const Unicode::string_t &s);

	class encoding_error : public std::runtime_error {
		public:
			encoding_error(const std::string &msg) : runtime_error(msg) {}
	};

	class decoding_error : public std::runtime_error {
		public:
			decoding_error(const std::string &msg) : runtime_error(msg) {}
	};

};

#endif
