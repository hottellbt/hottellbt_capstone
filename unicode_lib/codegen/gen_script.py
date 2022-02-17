#!/usr/bin/python3

from megatable import MegaTable
import ucd

def prepend_x(value):
    if not isinstance(value, str):
        raise TypeError("expected string, got " + str(type(value)) + " (" + str(value) + ")")
    return "X::" + value


def get_data():
    special_function ="""Unicode::Script Unicode::get_script(Unicode::codepoint_t codepoint) {
return ScriptData::lookup(codepoint);
}"""

    ret = MegaTable(
            "Unknown",
            "X",
            "ScriptData",
            namespace_preamble = "using X = Unicode::Script;",
            global_postamble = special_function,
            out_of_bounds_value = "X::Unknown",
            value_print_converter = prepend_x,
            sizeof_chunk_elem = 1)

    for parts in ucd.preprocess_parts('Scripts.txt'):
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

