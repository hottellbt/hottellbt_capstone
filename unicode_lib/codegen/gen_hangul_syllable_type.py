from megatable import MegaTable
import ucd
import table_helper

def get_hangul_syllable_types():
    special_function ="""Unicode::HangulSyllableType Unicode::get_hangul_syllable_type(Unicode::codepoint_t codepoint) {
return HangulSyllableTypeData::lookup(codepoint);
}"""

    ret = MegaTable(
            "NA",
            "X",
            "HangulSyllableTypeData",
            namespace_preamble = "using X = Unicode::HangulSyllableType;",
            global_postamble = special_function,
            out_of_bounds_value = "X::NA",
            value_print_converter = table_helper.prepend_x,
            sizeof_chunk_elem = 1)

    for parts in ucd.preprocess_parts('HangulSyllableType.txt'):
        codepoint_min, codepoint_max = ucd.parse_codepoint_range(parts[0])
        value = parts[1].strip()

        for i in range(codepoint_min, codepoint_max+1):
            ret[i] = value

    return ret


def main():
    ucd.print_codegen_header()
    size = get_hangul_syllable_types().dump_optimally()
    ucd.eprint("Expected size: " + str(size))

if __name__ == '__main__':
    main()

