#include <string>

#include <cstdlib>
#include <cstdint>

#include "unicode.hpp"
#include "encoding.hpp"

struct utf32_state {
	bool check_bom;
	bool little_endian;
	uint_fast8_t num_bytes;
	uint8_t bytes[4];
};

encoding::ErrorCode encoding::utf32_decode_start(void** state_ptr, bool little_endian) noexcept {

	utf32_state* state = (utf32_state*) realloc(*state_ptr, sizeof(utf32_state));
	*state_ptr = state;
	if (state == NULL) return encoding::E_MEMORY;

	state->check_bom = true;
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
			state->num_bytes = 0;

			uint8_t b1, b2, b3, b4;

			if (state->little_endian) {
				b1 = state->bytes[0];
				b2 = state->bytes[1];
				b3 = state->bytes[2];
				b4 = state->bytes[3];
			} else {
				b1 = state->bytes[3];
				b2 = state->bytes[2];
				b3 = state->bytes[1];
				b4 = state->bytes[0];
			}

			Unicode::codepoint_t cp = (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;

			if (state->check_bom) {
				state->check_bom = false;
				if (cp == 0xFFFE) {
					state->little_endian = !state->little_endian;
					continue;
				}
				if (cp == 0xFEFF) {
					// current endianness is correct
					continue;
				}
				// otherwise: there was no BOM, so let's hope we were correct anyways :)
			}

			ustr->push_back(cp);
		}

	}

	return encoding::E_OK;
}

encoding::ErrorCode encoding::utf32_decode_end(void* state_ptr) noexcept {
	utf32_state* state = (utf32_state*) state_ptr;
	if (state->num_bytes != 0) return encoding::E_BADEND;
	return encoding::E_OK;
}

encoding::ErrorCode encoding::utf32_encode(const Unicode::string_t* ustr, std::vector<char>* estr, bool little_endian) noexcept {

	for (size_t i = 0; i < ustr->size(); i++) {
		Unicode::codepoint_t cp = (*ustr)[i];

		if (little_endian) {
			estr->push_back((cp      ) & 0xFF);
			estr->push_back((cp >> 8 ) & 0xFF);
			estr->push_back((cp >> 16) & 0xFF);
			estr->push_back((cp >> 24) & 0xFF);
		} else {
			estr->push_back((cp >> 24) & 0xFF);
			estr->push_back((cp >> 16) & 0xFF);
			estr->push_back((cp >> 8 ) & 0xFF);
			estr->push_back((cp      ) & 0xFF);
		}
	}

	return encoding::E_OK;
}
