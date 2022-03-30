#include <cxxtest/TestSuite.h>

#include <string>
#include <vector>
#include <cstdint>

#include "unicode.hpp"
#include "encoding.hpp"

class UTFTestSuite : public CxxTest::TestSuite {
	public:

		const std::vector<encoding::Encoding> utfs {
			encoding::Encoding::UTF8,
			encoding::Encoding::UTF16LE,
			encoding::Encoding::UTF16BE,
			encoding::Encoding::UTF32LE,
			encoding::Encoding::UTF32BE,
		};

		/*
		   Tests that...
		   (1) The given bytes are decoded to a 32 bit string without error
		   (2) The 32 bit string matches what is expected
		   (2) The 32 bit string is then re-encoded into bytes without error
		   (3) The re-encoded bytes are equal to the given bytes
		*/
		void consistency_test(
				encoding::Encoding e,
				const char* input,
				const size_t input_size,
				const Unicode::string_t& expected) {

			const Unicode::string_t decoded = encoding::auto_decode_or_throw(e, input, input_size);

			if (decoded != expected) {
				TS_FAIL((std::string) "decoded data did not match expectations for "
						+ encoding::to_string(e));
			}

			const std::string actual = encoding::auto_encode_or_throw(e, &decoded);

			if (input != actual) {
				TS_FAIL((std::string) "re-encoded data did not match initial data for" 
						+ encoding::to_string(e));
			}
		}

		void utfs_test(
				const char* utf8,
				const size_t utf8_len,
				const char* utf16le,
				const char* utf16be,
				const size_t utf16_len,
				const char* utf32le,
				const char* utf32be,
				const size_t utf32_len,
				const Unicode::string_t& result) {

			consistency_test(encoding::Encoding::UTF8,    utf8,    utf8_len,  result);
			consistency_test(encoding::Encoding::UTF16LE, utf16le, utf16_len, result);
			consistency_test(encoding::Encoding::UTF16BE, utf16be, utf16_len, result);
			consistency_test(encoding::Encoding::UTF32LE, utf32le, utf32_len, result);
			consistency_test(encoding::Encoding::UTF32BE, utf32be, utf32_len, result);
		}

		void test_utfs() {
			utfs_test("", 0, "", "", 0, "", "", 0, {});
		}

		/*
		void disabled_test_ascii(const encoding::Encoding e) {
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

		void disabled_test_utf8(void) {
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
		*/
};

