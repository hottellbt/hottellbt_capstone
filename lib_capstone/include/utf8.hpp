#ifndef INCLUDED_UTF8_HPP
#define INCLUDED_UTF8_HPP

#ifndef INCLUDED_UNICODE_HPP
#include "unicode.hpp"
#endif

#include <stdexcept>

namespace UTF8 {

	class encoding_error : public std::runtime_error {
		public:
			encoding_error(const std::string &msg) : runtime_error(msg) {}
	};

	class decoding_error : public std::runtime_error {
		public:
			decoding_error(const std::string &msg) : runtime_error(msg) {}
	};

	class BufferedDecoder {
		public:
			Unicode::string_t decode(const char *bytes, const size_t bytes_len);

			Unicode::string_t decode(char byte) { return decode(&byte, 1); }

			bool is_empty() { return codepoint_pos == 0; }

		private:
			Unicode::codepoint_t codepoint;
			uint_fast8_t codepoint_pos = 0;
			uint_fast8_t codepoint_len = 0;
	};

	inline Unicode::string_t decode(const std::string &s) {
		BufferedDecoder bd;
		auto ret = bd.decode(s.c_str(), s.size());
		if (!bd.is_empty()) {
			throw decoding_error("UTF-8: Incomplete codepoint");
		}	
		return ret;
	}

	std::string encode(const Unicode::string_t &s);

};

#endif
