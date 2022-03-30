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

		std::string bytes2str(const char* s, const size_t s_len) {
			std::string ret;
			for (size_t i = 0; i < s_len; i++) {
				ret += std::to_string((int) (s[i]));
				ret += " ";
			}
			return ret;
		}

		std::string unistr(const Unicode::string_t& s) {
			std::string ret;
			for (auto c : s) {
				ret += Unicode::to_string(c);
			}
			return ret;
		}

		bool str_eq(
				const char* s1,
				const size_t s1_len,
				const char* s2,
				const size_t s2_len) {

			if (s1_len != s2_len) return false;
			for (size_t i = 0; i < s1_len; i++) {
				if (s1[i] != s2[i]) return false;
			}
			return true;
		}

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
						+ encoding::to_string(e)
						+ ". expected = "
						+ unistr(expected)
						+ " , decoded = "
						+ unistr(decoded));
			}

			const std::vector<char> actual = encoding::auto_encode_or_throw(e, &decoded);

			size_t actual_str_len = actual.size();
			char* actual_str = (char*) malloc(sizeof(char) * actual_str_len);

			for (size_t i = 0; i < actual_str_len; i++) {
				actual_str[i] = actual[i];
			}

			bool eq = str_eq(input, input_size, actual_str, actual_str_len);

			free(actual_str);

			if (!eq) {
				TS_FAIL((std::string) "re-encoded data did not match initial data for " 
						+ encoding::to_string(e)
						+ ". input = "
						+ bytes2str(input, input_size)
						+ " , encoded = "
						+ bytes2str(actual_str, actual_str_len));
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
			utfs_test("a", 1, "\0a", "a\0", 2, "\0\0\0a", "a\0\0\0", 4, {'a'});
			utfs_test("b", 1, "\0b", "b\0", 2, "\0\0\0b", "b\0\0\0", 4, {'b'});

			utfs_test(
					"ab", 2,
					"\0a\0b", "a\0b\0", 4,
					"\0\0\0a\0\0\0b", "a\0\0\0b\0\0\0", 8,
					{'a', 'b'});
		}

		/*
			please re-add these tests

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
		*/
};

