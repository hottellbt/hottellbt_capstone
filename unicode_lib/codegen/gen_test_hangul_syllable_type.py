import ucd

SPECIAL_FUNCTIONS = """
bool check_codepoint(Unicode::codepoint_t cp, Unicode::HangulSyllableType expected) {
    Unicode::HangulSyllableType actual = Unicode::get_hangul_syllable_type(cp);

    if (actual != expected) {
        TS_FAIL("incorrect hangul syllable type for " 
                + Unicode::to_string(cp)
                + " (" + std::to_string(cp) + ")"
                ", expected "
                + Unicode::to_string(expected)
                + ", got " + Unicode::to_string(actual));
        return false;
    }

    return true;
}

void check_range(
        Unicode::codepoint_t cp_min,
        Unicode::codepoint_t cp_max,
        Unicode::HangulSyllableType x) {

    for (Unicode::codepoint_t i = cp_min; i <= cp_max; i++) {
        if (!check_codepoint(i, x)) return;
    }
}
"""

def main():
    ucd.print_codegen_header()
    ucd.start_test_suite("HangulSyllableType")

    print(SPECIAL_FUNCTIONS)

    print("void test_everything(void) {")
    for parts in ucd.preprocess_parts('HangulSyllableType.txt'):
        codepoint_min, codepoint_max = ucd.parse_codepoint_range(parts[0])
        value = parts[1].strip()
        print(f"check_range({codepoint_min}, {codepoint_max}, Unicode::HangulSyllableType::{value});")
    print("}")

    ucd.end_test_suite()


if __name__ == '__main__':
    main()

