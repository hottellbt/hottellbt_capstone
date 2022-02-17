#include <string>

#include "coding_error.hpp"
#include "unicode.hpp"
#include "utf8.hpp"

using Unicode::codepoint_t;

Unicode::string_t UTF8::decode(const std::string &s) {
	const std::string::size_type s_size = s.size();
	std::string::size_type s_index = 0;
	std::string::size_type cp_size;

	Unicode::codepoint_t cp;

	Unicode::string_t ret;
	ret.reserve(s_size);

	while(s_index < s_size) {

		const char s_0 = s[s_index];

		if ((s_0 & 0x80) == 0x00) {
			cp = s_0;
			cp_size = 1;

		} else if ((s_0 & 0xE0) == 0xC0) {
			cp = s_0 & 0x1F;
			cp_size = 2;

		} else if ((s_0 & 0xF0) == 0xE0) {
			cp = s_0 & 0x0F;
			cp_size = 3;

		} else if ((s_0 & 0xF8) == 0xF0) {
			cp = s_0 & 0x07;
			cp_size = 4;

		} else {
			throw decoding_error("UTF-8: Disallowed first byte: " + std::to_string(s_0));
		}

		if (cp_size + s_index > s_size) {
			throw decoding_error("UTF-8: Incomplete codepoint");
		}

		for (int i = 1; i < cp_size; i++) {
			char appendee_raw = s[s_index + i];

			if (!is_appendee(appendee_raw)) {
				throw decoding_error(
						"UTF-8: Expected byte in range 0x80 to 0xBF, got "
						+ std::to_string(appendee_raw));
			}

			char appendee = (appendee_raw & 0x3f);

			cp <<= 6;
			cp |= appendee;
		}

		s_index += cp_size;
		ret += cp;
	}

	return ret;
}

std::string UTF8::encode(const Unicode::string_t &s) {
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

