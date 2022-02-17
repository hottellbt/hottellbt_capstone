#include <cxxtest/TestSuite.h>

#include "unicode.hpp"

class CodepointToStringTestSuite : public CxxTest::TestSuite {
	public:
		void check(Unicode::codepoint_t cp, const std::string &expected) {
			std::string actual = Unicode::to_string(cp);
			if (actual != expected) {
				TS_FAIL("Failed for codepoint: "
						+ std::to_string(cp) + " got "
						+ actual + " instead of " + expected);
			}
		}

		void test_to_string(void) {
			check(0, "U+0000");
			check('0', "U+0030");
			check(0x007E, "U+007E");
			check(0x00A0, "U+00A0");
			check(0x02FF, "U+02FF");
			check(0x04FF, "U+04FF");
			check(0x111FB, "U+111FB");
			check(0x11A40, "U+11A40");
			check(0x10FFFD, "U+10FFFD");
			check(Unicode::MAX_CODEPOINT, "U+10FFFF");
			check(0xFFFFFFFF, "U+FFFFFFFF");
		}
};

