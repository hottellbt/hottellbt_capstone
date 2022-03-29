#include <string>

#include <cstdlib>

#include "unicode.hpp"
#include "encoding.hpp"

struct ucs2_state {
	bool little_endian;
	bool has_saved_byte;
	char saved_byte;
};

encoding::ErrorCode encoding::ucs2_decode_start(void** state_ptr, bool little_endian) noexcept {

	ucs2_state* state = (ucs2_state*) realloc(*state_ptr, sizeof(ucs2_state));
	*state_ptr = state;
	if (state == NULL) return encoding::E_MEMORY;

	state->little_endian = little_endian;
	state->has_saved_byte = false;
	// saved_byte is left undefined

	return encoding::E_OK;
}

encoding::ErrorCode encoding::ucs2_decode_part(
		void* state_ptr,
		const char *input,
		const size_t input_len,
		Unicode::string_t* ustr) noexcept {
	ucs2_state* state = (ucs2_state*) state_ptr;

	size_t bytes_pos = 0;
	char bytes[2];

	if (state->has_saved_byte) {
		bytes[0] = state->saved_byte;
		bytes_pos++;
		state->has_saved_byte = false;
	}

	for (size_t input_pos = 0; input_pos < input_len; input_pos++) {
		bytes[bytes_pos++] = input[input_pos];

		if (bytes_pos == 2) {
			uint8_t b1 = bytes[0];
			uint8_t b2 = bytes[1];
			Unicode::codepoint_t cp = (b1 << 8) | b2;

			ustr->push_back(cp);
			bytes_pos = 0;
		}
	}

	if (bytes_pos == 1) {
		state->saved_byte = bytes[0];
		state->has_saved_byte = true;
	}

	return encoding::E_OK;
}

encoding::ErrorCode ucs2_decode_end(void* state) noexcept {
	return encoding::E_OK;
}

encoding::ErrorCode encoding::ucs2_encode(const Unicode::string_t* ustr, std::string* estr) noexcept {

	for (size_t i = 0; i < ustr->size(); i++) {
		Unicode::codepoint_t cp = (*ustr)[i];

		if (cp > 0xFFFF) {
			return encoding::E_CANTREP;
		}

		estr->push_back((cp >> 8));
		estr->push_back((cp & 0xFF));
	}

	return encoding::E_OK;
}
