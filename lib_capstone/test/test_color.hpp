#include <cxxtest/TestSuite.h>

#include "terminal.hpp"

class ColorTestSuite : public CxxTest::TestSuite {
	public:
		void test_color_eq(void) {
			Color c16_blue = Color16::BLUE;
			Color c16_red = Color16::RED;

			TS_ASSERT(c16_blue == c16_blue);
			TS_ASSERT(c16_red == c16_red);

			TS_ASSERT(c16_blue != c16_red);
			TS_ASSERT(c16_red != c16_blue);
		}
};

