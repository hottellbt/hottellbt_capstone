from megatable import MegaTable
import ucd
import table_helper

def get_data():
    special_function ="""Unicode::LineBreak Unicode::get_line_break(Unicode::codepoint_t codepoint) {
return LineBreakData::lookup(codepoint);
}"""

    ret = MegaTable(
            "XX",
            "X",
            "LineBreakData",
            namespace_preamble = "using X = Unicode::LineBreak;",
            global_postamble = special_function,
            out_of_bounds_value = "X::XX",
            value_print_converter = table_helper.prepend_x,
            sizeof_chunk_elem = 1)

    for parts in ucd.preprocess_parts('LineBreak.txt'):
        codepoint_min, codepoint_max = ucd.parse_codepoint_range(parts[0])
        value = parts[1].strip()

        for i in range(codepoint_min, codepoint_max+1):
            ret[i] = value

    return ret


def main():
    ucd.print_codegen_header()
    size = get_data().dump_optimally()
    ucd.eprint("Expected size: " + str(size))

if __name__ == '__main__':
    main()

