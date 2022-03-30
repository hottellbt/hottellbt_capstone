#include <string>

#include <cstdlib>
#include <cstdint>

#include "unicode.hpp"
#include "encoding.hpp"

struct utf32_state {
	bool little_endian;
	uint_fast8_t num_bytes;
	uint8_t bytes[4];
};

encoding::ErrorCode encoding::utf32_decode_start(void** state_ptr, bool little_endian) noexcept {

	utf32_state* state = (utf32_state*) realloc(*state_ptr, sizeof(utf32_state));
	*state_ptr = state;
	if (state == NULL) return encoding::E_MEMORY;

	state->little_endian = little_endian;
	state->num_bytes = 0;
	// saved_bytes are left undefined

	return encoding::E_OK;
}

encoding::ErrorCode encoding::utf32_decode_part(
		void* state_ptr,
		const char *input,
		const size_t input_len,
		Unicode::string_t* ustr) noexcept {

	assert(state_ptr != NULL);
	assert(input != NULL);
	assert(ustr != NULL);

	utf32_state* state = (utf32_state*) state_ptr;

	for (size_t input_pos = 0; input_pos < input_len; input_pos++) {

		state->bytes[state->num_bytes++] = input[input_pos];

		if (state->num_bytes == 4) {

			uint8_t b1, b2, b3, b4;

			b1 = state->bytes[0];
			b2 = state->bytes[1];
			b3 = state->bytes[2];
			b4 = state->bytes[3];

			Unicode::codepoint_t cp = (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
			ustr->push_back(cp);

			state->num_bytes = 0;
		}

	}

	return encoding::E_OK;
}

encoding::ErrorCode encoding::utf32_decode_end(void* state_ptr) noexcept {
	utf32_state* state = (utf32_state*) state_ptr;
	if (state->num_bytes != 0) return encoding::E_BADEND;
	return encoding::E_OK;
}

encoding::ErrorCode encoding::utf32_encode(const Unicode::string_t* ustr, std::string* estr) noexcept {

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
