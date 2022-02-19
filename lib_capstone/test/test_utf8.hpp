#include <cxxtest/TestSuite.h>

#include "unicode.hpp"
#include "utf8.hpp"

class UTF8TestSuite : public CxxTest::TestSuite {
	public:
		bool check_decode_encode(
				const std::string &input) {

			const auto decoded = UTF8::decode(input);
			const auto actual = UTF8::encode(decoded);

			if (input != actual) {
				TS_FAIL("encoded != decoded, input=" + input + " actual=" + actual);
				return false;
			}

			return true;
		}

		void test_encoding(void) {
			check_decode_encode("");

			check_decode_encode("Lorem ipsum dolor sit amet,");

			check_decode_encode("0");
			check_decode_encode("A");
			check_decode_encode("$");
			check_decode_encode("$A0$$abc019");

			// examples taken from https://en.wikipedia.org/wiki/UTF-8
			check_decode_encode("£");
			check_decode_encode("ह");
			check_decode_encode("€");
			check_decode_encode("한");
			check_decode_encode("𐍈");
			check_decode_encode("$£€ह한𐍈");
			check_decode_encode("0A$£€0ह한1𐍈b");

			check_decode_encode("◍◍");

			// emojis
			check_decode_encode("🤠");
			check_decode_encode("🍭");
			check_decode_encode("🪶");
			check_decode_encode("🤠🪶🍭");

			check_decode_encode("🍭A$🪶£€0ह한1𐍈b🤠");
		}
};

