#include <string>

#include <cstdlib>
#include <cstdint>
#include <cassert>

#include "unicode.hpp"
#include "encoding.hpp"

struct utf16_state {
	bool check_bom;
	bool little_endian;
	uint_fast8_t num_bytes; // 0..2
	uint8_t bytes[2];
	bool had_prior_surrogate;
	uint_fast16_t prior_surrogate;
};

encoding::ErrorCode encoding::utf16_decode_start(void** state_ptr, bool little_endian) noexcept {

	assert(state_ptr != NULL);

	*state_ptr = realloc(*state_ptr, sizeof(utf16_state));
	if (*state_ptr == NULL) return encoding::E_MEMORY;

	utf16_state* state = (utf16_state*) *state_ptr;

	state->check_bom = true;
	state->little_endian = little_endian;
	state->num_bytes = 0;
	state->had_prior_surrogate = false;

	return encoding::E_OK;
}

encoding::ErrorCode encoding::utf16_decode_part(
		void* state_ptr,
		const char *input,
		const size_t input_len,
		Unicode::string_t* ustr) noexcept {

	assert(state_ptr != NULL);
	assert(input != NULL);
	assert(ustr != NULL);

	utf16_state* state = (utf16_state*) state_ptr;

	for (size_t input_pos = 0; input_pos < input_len; input_pos++) {

		state->bytes[state->num_bytes++] = input[input_pos];

		if (state->num_bytes == 2) {
			state->num_bytes = 0;

			uint8_t b1, b2;
			uint16_t cp;

			b1 = state->bytes[0];
			b2 = state->bytes[1];

			if (state->little_endian) {
				cp = (b1 << 8) | b2;
			} else {
				cp = (b2 << 8) | b1;
			}

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

			if (cp >= 0xD800 && cp <= 0xDFFF) {
				if (state->had_prior_surrogate) {
					state->had_prior_surrogate = false;
					const uint32_t cp_full = (state->prior_surrogate << 16) | cp;
					ustr->push_back(cp_full);
				} else {
					state->had_prior_surrogate = true;
					state->prior_surrogate = cp;
				}
				continue;
			} else if (state->had_prior_surrogate) {
				return E_GROUP;
			}

			ustr->push_back(cp);
		}

	}

	return encoding::E_OK;
}

encoding::ErrorCode encoding::utf16_decode_end(void* state_ptr) noexcept {
	utf16_state* state = (utf16_state*) state_ptr;
	if (state->num_bytes != 0) return encoding::E_BADEND;
	if (state->had_prior_surrogate) return encoding::E_GROUP;
	return encoding::E_OK;
}

encoding::ErrorCode encoding::utf16_encode(const Unicode::string_t* ustr, std::vector<char>* estr, bool little_endian) noexcept {

	for (size_t i = 0; i < ustr->size(); i++) {
		Unicode::codepoint_t cp = (*ustr)[i];

		if (cp > 0xFFFF) {
			return encoding::E_CANTREP;
		}

		if (little_endian) {
			estr->push_back((cp >> 8) & 0xFF);
			estr->push_back((cp     ) & 0xFF);
		} else {
			estr->push_back((cp     ) & 0xFF);
			estr->push_back((cp >> 8) & 0xFF);
		}
	}

	return encoding::E_OK;
}
