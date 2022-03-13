#include <cxxtest/TestSuite.h>

#include <string>
#include <vector>
#include <cstdint>

#include "unicode.hpp"
#include "encoding.hpp"

static const std::vector<encoding::Encoding> all_encodings {
		encoding::Encoding::UTF8,
		encoding::Encoding::UCS2,
	};

class UTF8TestSuite : public CxxTest::TestSuite {
	public:

		void smoke_test(
				const encoding::Encoding encoding,
				const char* input,
				const size_t input_size) {

			const Unicode::string_t decoded = encoding::decode(encoding, input, input_size);

			const std::string actual = encoding::encode(encoding, decoded);

			if (input != actual) {
				TS_FAIL((std::string) "encoded != decoded, input=" 
						+ input + " actual=" + actual + " for encoding "
						+ encoding::to_string(encoding));
			}
		}

		void smoke_test(
				const encoding::Encoding encoding,
				const std::string& input) {
			smoke_test(encoding, input.c_str(), input.size());
		}

		void check_decode(
				const encoding::Encoding encoding,
				const char* input,
				const size_t input_size,
				const Unicode::string_t &result) {

			const auto decoded = encoding::decode(encoding, input, input_size);

			if (decoded != result) {
				TS_FAIL((std::string) "decoding failed, input=" 
						+ input + " for encoding "
						+ encoding::to_string(encoding));
			}
		}

		void check_decode(
				const encoding::Encoding encoding,
				const char* input,
				const size_t input_size,
				const Unicode::codepoint_t result) {
			Unicode::string_t s;
			s.push_back(result);
			check_decode(encoding, input, input_size, s);
		}

		void test_empty_strings(void) {
			for (const auto e : all_encodings) {
				smoke_test(e, "");
			}
		}

		void test_ascii(const encoding::Encoding e) {
			for (uint8_t i = 1; i < 128; i++) {
				char s[2];
				s[0] = i;
				s[1] = 0;
				smoke_test(e, s, 1);
			}

			smoke_test(e, "Lorem ipsum dolor sit amet,");

			smoke_test(e, "0");
			smoke_test(e, "A");
			smoke_test(e, "$");
			smoke_test(e, "$A0$$abc019");
		}

		void test_utf8(void) {
			const auto e = encoding::Encoding::UTF8;

			test_ascii(e);

			smoke_test(e, "◍◍");

			// examples taken from https://en.wikipedia.org/wiki/UTF-8
			smoke_test(e, "£");
			smoke_test(e, "ह");
			smoke_test(e, "€");
			smoke_test(e, "한");
			smoke_test(e, "𐍈");
			smoke_test(e, "$£€ह한𐍈");
			smoke_test(e, "0A$£€0ह한1𐍈b");

			// emojis
			smoke_test(e, "🤠");
			smoke_test(e, "🍭");
			smoke_test(e, "🪶");
			smoke_test(e, "🤠🪶🍭");

			smoke_test(e, "🍭A$🪶£€0ह한1𐍈b🤠");
		}

		void test_ucs2(void) {
			const auto e = encoding::Encoding::UCS2;

			for (uint8_t i = 1; i < 128; i++) {
				char s[2];
				s[0] = 0;
				s[1] = i;
				check_decode(e, s, 2, (Unicode::codepoint_t) i);
			}

			char s[2];
			s[0] = 0xFF;
			s[1] = 0xFF;
			check_decode(e, s, 2, (Unicode::codepoint_t) 0xFFFF);
			smoke_test(e, s, 2);
		}
};

