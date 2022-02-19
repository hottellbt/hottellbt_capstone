import ucd

SPECIAL_FUNCTIONS = """
void check_range(
        Unicode::codepoint_t cp_min,
        Unicode::codepoint_t cp_max,
        std::function<bool(Unicode::codepoint_t)> func,
        const std::string &property_name) {

    for (Unicode::codepoint_t i = cp_min; i <= cp_max; i++) {
        if (!func(i)) {
            TS_FAIL("Codepoint " + Unicode::to_string(i)
                + " missing property " + property_name);
        }
    }
}
"""

def main():
    ucd.print_codegen_header()
    print("#include <functional>")
    ucd.start_test_suite("SimpleProperty")

    print(SPECIAL_FUNCTIONS)

    print("void test_everything(void) {")

    def eat_file(file_name):
        for parts in ucd.preprocess_parts(file_name):
            codepoint_min, codepoint_max = ucd.parse_codepoint_range(parts[0])
            value = parts[1].strip()

            func = f"Unicode::is_{value.lower()}"

            print(f"check_range({codepoint_min}, {codepoint_max}, {func}, \"{value}\");")

    eat_file('PropList.txt')
    eat_file('emoji/emoji-data.txt')

    print("}")

    ucd.end_test_suite()


if __name__ == '__main__':
    main()

