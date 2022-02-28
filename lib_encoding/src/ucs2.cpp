#include <string>

#include "unicode.hpp"
#include "encoding.hpp"


Unicode::string_t Encoding::UCS2::UCS2BufferedDecoder::decode(
		const char *input,
		const size_t input_len) {

	Unicode::string_t ret;

	size_t bytes_pos = 0;
	char bytes[2];

	if (has_saved_byte) {
		bytes[0] = saved_byte;
		bytes_pos++;
		has_saved_byte = false;
	}

	for (size_t input_pos = 0; input_pos < input_len; input_pos++) {
		bytes[bytes_pos++] = input[input_pos];

		if (bytes_pos == 2) {
			uint8_t b1 = bytes[0];
			uint8_t b2 = bytes[1];
			Unicode::codepoint_t cp = (b1 << 8) | b2;

			ret.push_back(cp);
			bytes_pos = 0;
		}
	}

	if (bytes_pos == 1) {
		saved_byte = bytes[0];
		has_saved_byte = true;
	}

	return ret;
}

std::string Encoding::UCS2::encode(const Unicode::string_t &s) {
	std::string ret;
	ret.reserve(s.size() * 2);

	for (size_t i = 0; i < s.size(); i++) {
		Unicode::codepoint_t cp = s[i];

		if (cp > 0xFFFF) {
			throw encoding_error("UCS-2: Not supported: " + Unicode::to_string(cp));
		}

		ret.push_back((cp >> 8));
		ret.push_back((cp & 0xFF));
	}

	return ret;
}
