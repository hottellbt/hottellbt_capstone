#include <cxxtest/TestSuite.h>

#include <string>
#include <vector>
#include <cstdint>

#include "unicode.hpp"
#include "encoding.hpp"

class UTFTestSuite : public CxxTest::TestSuite {
	public:

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
				ret += " ";
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

		char* create_char_ptr(const std::vector<unsigned char>& v) {
			char* ret = (char*) malloc(sizeof(char) * v.size());
			if (ret == NULL) throw std::runtime_error("malloc failed!");
			for (size_t i = 0; i < v.size(); i++) {
				ret[i] = v[i];
			}
			return ret;
		}

		void utfs_testv(
				const std::vector<unsigned char>& utf8v,
				const std::vector<unsigned char>& utf16lev,
				const std::vector<unsigned char>& utf16bev,
				const std::vector<unsigned char>& utf32lev,
				const std::vector<unsigned char>& utf32bev,
				const Unicode::string_t& result) {

			char* utf8 = create_char_ptr(utf8v);
			char* utf16le = create_char_ptr(utf16lev);
			char* utf16be = create_char_ptr(utf16bev);
			char* utf32le = create_char_ptr(utf32lev);
			char* utf32be = create_char_ptr(utf32bev);

			size_t utf8_len = utf8v.size();
			size_t utf16_len = utf16bev.size();
			size_t utf32_len = utf32bev.size();

			utfs_test(utf8, utf8_len, utf16le, utf16be, utf16_len, utf32le, utf32be, utf32_len, result);

			free(utf8);
			free(utf16le);
			free(utf16be);
			free(utf32le);
			free(utf32be);
		}

		void test_utfs() {
			utfs_test("", 0, "", "", 0, "", "", 0, {});
			utfs_test("a", 1, "a\0", "\0a", 2, "a\0\0\0", "\0\0\0a", 4, {'a'});
			utfs_test("b", 1, "b\0", "\0b", 2, "b\0\0\0", "\0\0\0b", 4, {'b'});

			utfs_test(
					"ab", 2,
					"a\0b\0", "\0a\0b", 4,
					"a\0\0\0b\0\0\0", "\0\0\0a\0\0\0b", 8,
					{'a', 'b'});

			/*
			utfs_test(
					"🪶", 3,
					"\ud83e\udeb6", 
					{0x1FAB6});
					*/

			// emojis
			// 🤠
			utfs_testv(
					{0xF0, 0x9F, 0xA4, 0xA0},
					{0x3e, 0xd8, 0x20, 0xdd},
					{0xd8, 0x3e, 0xdd, 0x20},
					{0x20, 0xf9, 0x01, 0x00},
					{0x00, 0x01, 0xf9, 0x20},
					{ 0x1F920 });

			// examples taken from https://en.wikipedia.org/wiki/UTF-8
			utfs_testv(
					{ '$' },
					{0x24, 0x00},
					{0x00, 0x24},
					{0x24, 0x00, 0x00, 0x00},
					{0x00, 0x00, 0x00, 0x24},
					{ '$' });

			// £
			utfs_testv(
					{0xC2, 0xA3},
					{0xA3, 0x00},
					{0x00, 0xA3},
					{0xA3, 0x00, 0x00, 0x00},
					{0x00, 0x00, 0x00, 0xA3},
					{ 0x00A3 });

			// ह
			utfs_testv(
					{0xE0, 0xA4, 0xB9},
					{0x39, 0x09},
					{0x09, 0x39},
					{0x39, 0x09, 0x00, 0x00},
					{0x00, 0x00, 0x09, 0x39},
					{ 0x0939 });
		}

		void do_bom_test(
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
		}

		void do_bom_tests(
				const std::vector<unsigned char>& utf16lev,
				const std::vector<unsigned char>& utf16bev,
				const std::vector<unsigned char>& utf32lev,
				const std::vector<unsigned char>& utf32bev,
				const Unicode::string_t& expected) {

			char* utf16le = create_char_ptr(utf16lev);
			char* utf16be = create_char_ptr(utf16bev);
			char* utf32le = create_char_ptr(utf32lev);
			char* utf32be = create_char_ptr(utf32bev);

			do_bom_test(encoding::Encoding::UTF16LE, utf16le, utf16lev.size(), expected);
			do_bom_test(encoding::Encoding::UTF16BE, utf16be, utf16bev.size(), expected);
			do_bom_test(encoding::Encoding::UTF32LE, utf32le, utf32lev.size(), expected);
			do_bom_test(encoding::Encoding::UTF32BE, utf32be, utf32bev.size(), expected);

			free(utf16le);
			free(utf16be);
			free(utf32le);
			free(utf32be);
		}

		void test_bom() {
			// ignore BOM if it aligns with what we expect
			do_bom_tests(
					{0xFF, 0xFE, 0x24, 0x00},
					{0xFE, 0xFF, 0x00, 0x24},
					{0xFF, 0xFE, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00},
					{0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0x24},
					{ '$' });

			// adapt to BOM if it does not align with what we expect
			do_bom_tests(
					{0xFE, 0xFF, 0x00, 0x24},
					{0xFF, 0xFE, 0x24, 0x00},
					{0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0x24},
					{0xFF, 0xFE, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00},
					{ '$' });
		}

		/*
			please re-add these tests

			smoke_test(e, "◍◍");

			// examples taken from https://en.wikipedia.org/wiki/UTF-8
			smoke_test(e, "€");
			smoke_test(e, "한");
			smoke_test(e, "𐍈");
			smoke_test(e, "$£€ह한𐍈");
			smoke_test(e, "0A$£€0ह한1𐍈b");

			// emojis
			smoke_test(e, "🍭");
			smoke_test(e, "🪶");
			smoke_test(e, "🤠🪶🍭");

			smoke_test(e, "🍭A$🪶£€0ह한1𐍈b🤠");
		*/
};

