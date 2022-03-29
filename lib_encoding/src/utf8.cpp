#include <string>

#include <cstdint>

#include "unicode.hpp"
#include "encoding.hpp"

inline bool is_appendee(char c) {
	return (c & 0xC0) == 0x80;
}

struct utf8_state {
	Unicode::codepoint_t codepoint;
	uint_fast8_t codepoint_pos;
	uint_fast8_t codepoint_len;
};

encoding::ErrorCode encoding::utf8_decode_start(void** state_ptr) noexcept {

	utf8_state* state = (utf8_state*) realloc(*state_ptr, sizeof(utf8_state));
	*state_ptr = state;
	if (state == NULL) return encoding::E_MEMORY;

	// codepoint is left undefined
	state->codepoint_pos = 0;
	state->codepoint_len = 0;

	return encoding::E_OK;
}

encoding::ErrorCode encoding::utf8_decode_part(void* state_ptr, const char *bytes, const size_t bytes_len, Unicode::string_t* ustr) noexcept {
	utf8_state* state = (utf8_state*) state_ptr;

	for (uint i = 0; i < bytes_len; i++) {
		char b = bytes[i];

		if (state->codepoint_pos == 0) {
			if ((b & 0x80) == 0x00) {
				ustr->push_back((Unicode::codepoint_t) b);
				continue;

			} else if ((b & 0xE0) == 0xC0) {
				state->codepoint = b & 0x1F;
				state->codepoint_len = 2;

			} else if ((b & 0xF0) == 0xE0) {
				state->codepoint = b & 0x0F;
				state->codepoint_len = 3;

			} else if ((b & 0xF8) == 0xF0) {
				state->codepoint = b & 0x07;
				state->codepoint_len = 4;

			} else {
				// illegal first byte in multibyte sequence
				return encoding::E_GROUP; 
			}

			state->codepoint_pos = 1;
			continue;
		}

		if (!is_appendee(b)) {
			// expected byte in range 0x80 to 0xBF
			return encoding::E_GROUP;
		}

		state->codepoint <<= 6;
		state->codepoint |= (b & 0x3f);

		state->codepoint_pos += 1;

		if (state->codepoint_pos == state->codepoint_len) {
			state->codepoint_pos = 0;
			ustr->push_back(state->codepoint);
		}
	}

	return encoding::E_OK;
}

encoding::ErrorCode encoding::utf8_encode(const Unicode::string_t* ustr, std::string* estr) noexcept {

	for (size_t i = 0; i < ustr->size(); i++) {

		Unicode::codepoint_t cp = (*ustr)[i];

		if (cp <= 0x00007F) {
			estr += (char) cp;

		} else if (cp <= 0x0007FF) {
			*estr += 0xC0 | (0x1F & ((char) (cp >> 6)));
			*estr += 0x80 | (0x3F & ((char) cp));

		} else if (cp <= 0x00FFFF) {
			*estr += 0xE0 | (0x0F & ((char) (cp >> 12)));
			*estr += 0x80 | (0x3F & ((char) (cp >> 6)));
			*estr += 0x80 | (0x3F & ((char) cp));

		} else if (cp <= 0x10FFFF) {
			*estr += 0xF0 | (0x07 & ((char) (cp >> 18)));
			*estr += 0x80 | (0x3F & ((char) (cp >> 12)));
			*estr += 0x80 | (0x3F & ((char) (cp >> 6)));
			*estr += 0x80 | (0x3F & ((char) cp));

		} else {
			return encoding::E_CPOOB;
		}
	}

	return encoding::E_OK;
}

