#include <string>

#include "unicode.hpp"
#include "encoding.hpp"

using Unicode::codepoint_t;
using Encoding::encoding_error;
using Encoding::decoding_error;

inline bool is_appendee(char c) {
	return (c & 0xC0) == 0x80;
}

Unicode::string_t Encoding::UTF8::UTF8BufferedDecoder::decode(const char *bytes, const size_t bytes_len) {

	Unicode::string_t ret;

	for (int i = 0; i < bytes_len; i++) {
		char b = bytes[i];

		if (codepoint_pos == 0) {
			if ((b & 0x80) == 0x00) {
				ret.push_back((codepoint_t) b);
				continue;

			} else if ((b & 0xE0) == 0xC0) {
				codepoint = b & 0x1F;
				codepoint_len = 2;

			} else if ((b & 0xF0) == 0xE0) {
				codepoint = b & 0x0F;
				codepoint_len = 3;

			} else if ((b & 0xF8) == 0xF0) {
				codepoint = b & 0x07;
				codepoint_len = 4;

			} else {
				throw decoding_error("UTF-8: Disallowed first byte: " + std::to_string(b));
			}

			codepoint_pos = 1;
			continue;
		}

		if (!is_appendee(b)) {
			throw decoding_error(
					"UTF-8: Expected byte in range 0x80 to 0xBF, got "
					+ std::to_string(b));
		}

		codepoint <<= 6;
		codepoint |= (b & 0x3f);

		++codepoint_pos;
		if (codepoint_pos == codepoint_len) {
			codepoint_pos = 0;
			ret.push_back(codepoint);
		}
	}

	return ret;
}

std::string Encoding::UTF8::encode(const Unicode::string_t &s) {
	const auto s_size = s.size();

	std::string ret;
	ret.reserve(s_size);

	for (std::string::size_type i = 0; i < s_size; i++) {

		Unicode::codepoint_t cp = s[i];

		if (cp <= 0x00007F) {
			ret += (char) cp;

		} else if (cp <= 0x0007FF) {
			ret += 0xC0 | (0x1F & ((char) (cp >> 6)));
			ret += 0x80 | (0x3F & ((char) cp));

		} else if (cp <= 0x00FFFF) {
			ret += 0xE0 | (0x0F & ((char) (cp >> 12)));
			ret += 0x80 | (0x3F & ((char) (cp >> 6)));
			ret += 0x80 | (0x3F & ((char) cp));

		} else if (cp <= 0x10FFFF) {
			ret += 0xF0 | (0x07 & ((char) (cp >> 18)));
			ret += 0x80 | (0x3F & ((char) (cp >> 12)));
			ret += 0x80 | (0x3F & ((char) (cp >> 6)));
			ret += 0x80 | (0x3F & ((char) cp));

		} else {
			throw encoding_error(
					"UTF-8: codepoint exceeds 0x10FFFF: " + std::to_string(cp));
		}
	}

	return ret;
}

