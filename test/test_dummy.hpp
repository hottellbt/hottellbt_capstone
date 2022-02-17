#include <cxxtest/TestSuite.h>

class DummyTestSuite : public CxxTest::TestSuite {
	public:
		void test_dummy(void) {
			TS_ASSERT(true);
		}
};

