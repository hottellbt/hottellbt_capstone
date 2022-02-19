from megatable import MegaTable
import ucd
import table_helper

def get_indic_syllabic_types():
    special_function ="""Unicode::IndicSyllabicCategory Unicode::get_indic_syllabic_category(Unicode::codepoint_t codepoint) {
return IndicSyllabicCategoryData::lookup(codepoint);
}"""

    ret = MegaTable(
            "NA",
            "X",
            "IndicSyllabicCategoryData",
            namespace_preamble = "using X = Unicode::IndicSyllabicCategory;",
            global_postamble = special_function,
            out_of_bounds_value = "X::NA",
            value_print_converter = table_helper.prepend_x,
            sizeof_chunk_elem = 1)

    for parts in ucd.preprocess_parts('IndicSyllabicCategory.txt'):
        codepoint_min, codepoint_max = ucd.parse_codepoint_range(parts[0])
        value = parts[1].strip()

        for i in range(codepoint_min, codepoint_max+1):
            ret[i] = value

    return ret


def main():
    ucd.print_codegen_header()
    size = get_indic_syllabic_types().dump_optimally()
    ucd.eprint("Expected size: " + str(size))

if __name__ == '__main__':
    main()

