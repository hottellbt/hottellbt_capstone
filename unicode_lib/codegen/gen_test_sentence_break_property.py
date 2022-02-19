import ucd
import test_helper

def main():
    ucd.print_codegen_header()
    ucd.start_test_suite("SentenceBreak")

    test_helper.print_codepoint_checker_funcs(
            table_type="Unicode::SentenceBreak",
            table_lookup_func="Unicode::get_sentence_break")

    print("void test_everything(void) {")
    for parts in ucd.preprocess_parts('auxiliary/SentenceBreakProperty.txt'):
        codepoint_min, codepoint_max = ucd.parse_codepoint_range(parts[0])
        value = parts[1].strip()
        print(f"check_range({codepoint_min}, {codepoint_max}, Unicode::SentenceBreak::{value});")
    print("}")

    ucd.end_test_suite()


if __name__ == '__main__':
    main()

