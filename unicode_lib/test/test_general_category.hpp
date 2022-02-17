#include <cxxtest/TestSuite.h>

#include <vector>
#include "unicode.hpp"

class GeneralCategoryTestSuite : public CxxTest::TestSuite {
	public:
		bool check_codepoint(Unicode::codepoint_t cp, Unicode::GeneralCategory gc) {
			Unicode::GeneralCategory actual = Unicode::get_general_category(cp);

			if (actual != gc) {
				TS_FAIL("Incorrect general category for " 
						+ Unicode::to_string(cp)
						+ " (" + std::to_string(cp) + ")"
						", expected "
						+ Unicode::to_string(gc)
						+ ", got " + Unicode::to_string(actual));
				return false;
			}

			return true;
		}

		bool check_range(
				Unicode::codepoint_t cp_min,
				Unicode::codepoint_t cp_max,
				Unicode::GeneralCategory gc) {

			for (Unicode::codepoint_t i = cp_min; i <= cp_max; i++) {
				if (!check_codepoint(i, gc)) {
					return false;
				}
			}

			return true;
		}

		void check_vec(
				const std::vector<Unicode::codepoint_t> &codepoints,
				Unicode::GeneralCategory c) {
			for (Unicode::codepoint_t cp : codepoints) {
				check_codepoint(cp, c);
			}
		}

		void test_control(void) {
			check_range(0x00, 0x1F, Unicode::GeneralCategory::Cc);
			check_range(0x7F, 0x9F, Unicode::GeneralCategory::Cc);
		}

		void test_format(void) {
			auto c = Unicode::GeneralCategory::Cf;
			 // soft hyphen
			check_codepoint(0x00AD, c);
			// arabic
			check_range(0x0600, 0x605, c);
			check_vec({ 0x061C, 0x06DD }, c);

			// tags
			check_codepoint(0xE0001, c);
			check_range(0xE0020, 0xE007F, c);
		}

		void test_private_use(void) {
			auto c = Unicode::GeneralCategory::Co;
			check_range(0xE000,   0xF8FF,   c);
			check_range(0xF0000,  0xFFFFD,  c);
			check_range(0x100000, 0x10FFFD, c);
		}

		void test_lowercase_letter(void) {
			check_range('a', 'z', Unicode::GeneralCategory::Ll);
		}

		void test_uppercase_letter(void) {
			check_range('A', 'Z', Unicode::GeneralCategory::Lu);
		}

		void test_decimal_number(void) {
			check_range('0', '9', Unicode::GeneralCategory::Nd);
			// arabic-indic
			check_range(0x0660, 0x0669, Unicode::GeneralCategory::Nd);
			check_range(0x06F0, 0x06F9, Unicode::GeneralCategory::Nd);
			// nko
			check_range(0x07C0, 0x07C9, Unicode::GeneralCategory::Nd);
			// devanagari
			check_range(0x0966, 0x096F, Unicode::GeneralCategory::Nd);
			// bengali
			check_range(0x09E6, 0x09EF, Unicode::GeneralCategory::Nd);

			// mathematical digits
			check_range(0x1D7CE, 0x1D7FF, Unicode::GeneralCategory::Nd);

			// segmented digits
			check_range(0x1FBF0, 0x1FBF9, Unicode::GeneralCategory::Nd);
		}

		void test_line_separator(void) {
			check_codepoint(0x2028, Unicode::GeneralCategory::Zl);
		}

		void test_paragraph_separator(void) {
			check_codepoint(0x2029, Unicode::GeneralCategory::Zp);
		}

		void test_nonspacing_mark(void) {
			const auto c = Unicode::GeneralCategory::Mn;
			// variation selectors
			check_range(0xE0100, 0xE01EF, c);
		}

		void test_space_separator(void) {
			const auto c = Unicode::GeneralCategory::Zs;
			check_vec({0x0020, 0x00A0, 0x1680}, c);
			check_range(0x2001, 0x200A, c);
			check_vec({0x202F, 0x205F, 0x3000}, c);
		}

		void test_above_max(void) {
			check_codepoint(Unicode::MAX_CODEPOINT + 1, Unicode::GeneralCategory::Cn);
		}

};

