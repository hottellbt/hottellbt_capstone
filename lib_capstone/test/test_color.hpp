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
		}
};

