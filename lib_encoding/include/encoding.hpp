#ifndef INCLUDED_ENCODING_HPP
#define INCLUDED_ENCODING_HPP

#include "unicode.hpp"

#include <cstdint>
#include <cstring>
#include <cassert>
#include <cstdlib>

#include <stdexcept>
#include <string>

namespace encoding {

	typedef int ErrorCode;

	constexpr ErrorCode E_OK = 0;       // there is no error
	constexpr ErrorCode E_NEVER = 1;    // programming error, shouldn't ever happen
	constexpr ErrorCode E_MEMORY = 2;   // malloc/realloc/calloc failed
	constexpr ErrorCode E_BADEND = 3;   // ended too soon, expected more bytes, etc.
	constexpr ErrorCode E_CANTREP = 4;  // can't represent an otherwise legal codepoint in this encoding
	constexpr ErrorCode E_GROUP = 5;    // malformed group of bytes (or surrogate pair)
	constexpr ErrorCode E_CPOOB = 6;    // codepoint out of bounds (i.e. above the maximum codepoint value)

	ErrorCode utf8_decode_start  (void** state)                     noexcept;
	ErrorCode utf16_decode_start (void** state, bool little_endian) noexcept;
	ErrorCode utf32_decode_start (void** state, bool little_endian) noexcept;

	ErrorCode utf8_decode_part  (void* state, const char* data, size_t data_len, Unicode::string_t* ustr) noexcept;
	ErrorCode utf16_decode_part (void* state, const char* data, size_t data_len, Unicode::string_t* ustr) noexcept;
	ErrorCode utf32_decode_part (void* state, const char* data, size_t data_len, Unicode::string_t* ustr) noexcept;

	// the end functions do not call free on the void*, which might still be the caller's responsibility
	ErrorCode utf8_decode_end  (void* state) noexcept;
	ErrorCode utf16_decode_end (void* state) noexcept;
	ErrorCode utf32_decode_end (void* state) noexcept;

	ErrorCode utf8_encode  (const Unicode::string_t* ustr, std::string* estr) noexcept;
	ErrorCode utf16_encode (const Unicode::string_t* ustr, std::string* estr) noexcept;
	ErrorCode utf32_encode (const Unicode::string_t* ustr, std::string* estr) noexcept;

	enum class Encoding {
		UTF8,
		UTF16LE,
		UTF16BE,
		UTF32LE,
		UTF32BE,
	};

	enum class Endian {
		BIG,
		LITTLE,
		CHECK_BOM,
	};

	const char* to_string(const Encoding e);

	inline ErrorCode auto_decode_start(const Encoding e, void** state) noexcept {
		using X = encoding::Encoding;
		switch (e) {
			case X::UTF8:    return utf8_decode_start (state);
			case X::UTF16LE: return utf16_decode_start(state, true);
			case X::UTF16BE: return utf16_decode_start(state, false);
			case X::UTF32LE: return utf32_decode_start(state, true);
			case X::UTF32BE: return utf32_decode_start(state, false);
		}
		assert(false);
		return E_NEVER;
	}

	inline ErrorCode auto_decode_part(
			const Encoding e,
			void* state,
			const char* data,
			size_t data_len,
			Unicode::string_t* ustr) noexcept {

		assert(state != NULL);
		using X = encoding::Encoding;

		switch (e) {
			case X::UTF8:
				return utf8_decode_part (state, data, data_len, ustr);

			case X::UTF16LE:
			case X::UTF16BE:
				return utf16_decode_part(state, data, data_len, ustr);

			case X::UTF32LE:
			case X::UTF32BE:
				return utf32_decode_part(state, data, data_len, ustr);
		}

		assert(false);
		return E_NEVER;
	}

	inline ErrorCode auto_decode_end(const Encoding e, void* state) noexcept {

		assert(state != NULL);
		using X = encoding::Encoding;

		switch (e) {
			case X::UTF8:
				return utf8_decode_end (state);

			case X::UTF16LE:
			case X::UTF16BE:
				return utf16_decode_end(state);

			case X::UTF32LE:
			case X::UTF32BE:
				return utf32_decode_end(state);
		}

		assert(false);
		return E_NEVER;
	}

	/*
		Goes through the entire decoding process, handling error codes along the way.
	*/
	inline ErrorCode auto_decode(
			const Encoding e,
			const char* data,
			const size_t data_len,
			Unicode::string_t* ustr) noexcept {

		int status = E_OK;
		void* state = NULL;

		if (status == E_OK) status = auto_decode_start(e, &state);
		if (status == E_OK) status = auto_decode_part(e, state, data, data_len, ustr);
		if (status == E_OK) status = auto_decode_end(e, state);

		free(state);

		return status;
	}

	inline ErrorCode auto_encode(
			const Encoding e,
			const Unicode::string_t* ustr,
			std::string* estr) noexcept {
		using X = encoding::Encoding;
		switch (e) {
			case X::UTF8:    return utf8_encode (ustr, estr);
			case X::UTF16LE: return utf16_encode(ustr, estr);
			case X::UTF16BE: return utf16_encode(ustr, estr);
			case X::UTF32LE: return utf32_encode(ustr, estr);
			case X::UTF32BE: return utf32_encode(ustr, estr);
		}
		assert(false);
		return E_NEVER;
	}

	class EncodingError : public std::runtime_error {
		public:
			EncodingError(const std::string &msg) : runtime_error(msg) {}
	};

	inline Unicode::string_t auto_decode_or_throw(
			const Encoding e,
			const char* data,
			const size_t data_len) {

		Unicode::string_t ret;
		int status = auto_decode(e, data, data_len, &ret);
		if (status != E_OK) {
			throw EncodingError((std::string) "Failed to decode " + to_string(e) + ", error code " + std::to_string(status));
		}
		return ret;
	}

	inline std::string auto_encode_or_throw(
			const Encoding e,
			const Unicode::string_t* ustr) {

		std::string ret;
		int status = auto_encode(e, ustr, &ret);
		if (status != E_OK) {
			throw EncodingError((std::string) "Failed to encode " + to_string(e) + ", error code " + std::to_string(status));
		}
		return ret;
	}

	inline Unicode::string_t decode_literal(const char* data, size_t len) {
		return auto_decode_or_throw(Encoding::UTF8, data, len);
	}

	inline Unicode::string_t decode_literal(const char* data) {
		return decode_literal(data, strlen(data));
	}

	inline Unicode::string_t decode_literal(const std::string& str) {
		return decode_literal(str.c_str(), str.size());
	}
}

#endif
