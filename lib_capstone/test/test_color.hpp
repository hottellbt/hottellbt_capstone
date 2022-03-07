#include <cxxtest/TestSuite.h>

#include "terminal.hpp"

class ColorTestSuite : public CxxTest::TestSuite {
	public:
		void test_color_eq(void) {
			using Terminal::Color;

			Color c16_blue = Terminal::Color16::BLUE;
			Color c16_red = Terminal::Color16::RED;

			TS_ASSERT(c16_blue == c16_blue);
			TS_ASSERT(c16_red == c16_red);

			TS_ASSERT(c16_blue != c16_red);
			TS_ASSERT(c16_red != c16_blue);

			Terminal::Color c256_100 = 100;
			Terminal::Color c256_102 = 102;

			TS_ASSERT(c256_100 == c256_100);
			TS_ASSERT(c256_102 == c256_102);
			TS_ASSERT(c256_100 != c256_102);
			TS_ASSERT(c256_102 != c256_100);

			TS_ASSERT(c16_blue != c256_100);
			TS_ASSERT(c16_blue != c256_102);

			Terminal::Color cFFFFFF (0xFF, 0xFF, 0xFF);
			Terminal::Color cAAAAAA (0xAA, 0xAA, 0xAA);

			TS_ASSERT(cFFFFFF == cFFFFFF);
			TS_ASSERT(cAAAAAA == cAAAAAA);
			TS_ASSERT(cAAAAAA != cFFFFFF);
			TS_ASSERT(cFFFFFF != cAAAAAA);
		}

		void test_color_rgb(void) {
			Terminal::Color c (0x01, 0x02, 0x03);
			TS_ASSERT(c.color_rgb_r == 0x01);
			TS_ASSERT(c.color_rgb_g == 0x02);
			TS_ASSERT(c.color_rgb_b == 0x03);
		}
};

