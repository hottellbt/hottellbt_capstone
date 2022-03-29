#include <string>

#include <cstdlib>
#include <cstdint>

#include "unicode.hpp"
#include "encoding.hpp"

struct ucs2_state {
	bool little_endian;
	uint_fast8_t num_bytes;
	uint8_t bytes[2];
};

encoding::ErrorCode encoding::ucs2_decode_start(void** state_ptr, bool little_endian) noexcept {

	ucs2_state* state = (ucs2_state*) realloc(*state_ptr, sizeof(ucs2_state));
	*state_ptr = state;
	if (state == NULL) return encoding::E_MEMORY;

	state->little_endian = little_endian;
	state->num_bytes = 0;
	// saved_bytes are left undefined

	return encoding::E_OK;
}

encoding::ErrorCode encoding::ucs2_decode_part(
		void* state_ptr,
		const char *input,
		const size_t input_len,
		Unicode::string_t* ustr) noexcept {

	ucs2_state* state = (ucs2_state*) state_ptr;

	for (size_t input_pos = 0; input_pos < input_len; input_pos++) {

		state->bytes[state->num_bytes++] = input[input_pos];

		if (state->num_bytes == 2) {

			const uint8_t b1 = state->bytes[0];
			const uint8_t b2 = state->bytes[1];
			Unicode::codepoint_t cp = (b1 << 8) | b2;
			ustr->push_back(cp);

			state->num_bytes = 0;
		}

	}

	return encoding::E_OK;
}

encoding::ErrorCode ucs2_decode_end(void* state_ptr) noexcept {
	ucs2_state* state = (ucs2_state*) state_ptr;
	if (state->num_bytes != 0) return encoding::E_BADEND;
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
