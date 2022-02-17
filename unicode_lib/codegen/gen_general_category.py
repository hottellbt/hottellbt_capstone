#!/usr/bin/python3

from megatable import MegaTable
import ucd

def prepend_x(value):
    if not isinstance(value, str):
        raise TypeError("expected string, got " + str(type(value)) + " (" + str(value) + ")")
    return "X::" + value


def get_general_categories():
    DEFAULT_CATEGORY = 'Cn'
    fill_category = DEFAULT_CATEGORY

    special_function = """Unicode::GeneralCategory Unicode::get_general_category(Unicode::codepoint_t codepoint) {
	return GeneralCategoryData::lookup(codepoint);
}"""

    ret = MegaTable(
            DEFAULT_CATEGORY,
            "X",
            "GeneralCategoryData",
            namespace_preamble = "using X = Unicode::GeneralCategory;",
            global_postamble = special_function,
            out_of_bounds_value = "X::Cn",
            value_print_converter = prepend_x,
            sizeof_chunk_elem = 1)

    prior_codepoint = 0

    for parts in ucd.preprocess_parts('UnicodeData.txt'):

        codepoint = int(parts[0], 16)
        name = parts[1]
        category = parts[2]

        ret[codepoint] = category

        for x in range(prior_codepoint+1, codepoint):
            ret[x] = fill_category

        if name.endswith("First>"):
            fill_category = category
        elif name.endswith("Last>"):
            fill_category = DEFAULT_CATEGORY

        prior_codepoint = codepoint

    return ret


def main():
    ucd.print_codegen_header()
    size = get_general_categories().dump_optimally()
    ucd.eprint("Expected size: " + str(size))

if __name__ == '__main__':
    main()

