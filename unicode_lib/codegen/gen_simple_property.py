#!/bin/python3

import ucd
from megatable import MegaTable


def get_simple_properties():
    FLAGS={
            'White_Space'                       :0b0000000000000000000000000000000000000000000000000000000000000001,
            'Bidi_Control'                      :0b0000000000000000000000000000000000000000000000000000000000000010,
            'Join_Control'                      :0b0000000000000000000000000000000000000000000000000000000000000100,
            'Dash'                              :0b0000000000000000000000000000000000000000000000000000000000001000,
            'Hyphen'                            :0b0000000000000000000000000000000000000000000000000000000000010000,
            'Quotation_Mark'                    :0b0000000000000000000000000000000000000000000000000000000000100000,
            'Terminal_Punctuation'              :0b0000000000000000000000000000000000000000000000000000000001000000,
            'Other_Math'                        :0b0000000000000000000000000000000000000000000000000000000010000000,
            'Hex_Digit'                         :0b0000000000000000000000000000000000000000000000000000000100000000,
            'Other_Alphabetic'                  :0b0000000000000000000000000000000000000000000000000000001000000000,
            'Ideographic'                       :0b0000000000000000000000000000000000000000000000000000010000000000,
            'Diacritic'                         :0b0000000000000000000000000000000000000000000000000000100000000000,
            'Extender'                          :0b0000000000000000000000000000000000000000000000000001000000000000,
            'Other_Lowercase'                   :0b0000000000000000000000000000000000000000000000000010000000000000,
            'Other_Uppercase'                   :0b0000000000000000000000000000000000000000000000000100000000000000,
            'Noncharacter_Code_Point'           :0b0000000000000000000000000000000000000000000000001000000000000000,
            'Other_Grapheme_Extend'             :0b0000000000000000000000000000000000000000000000010000000000000000,
            'IDS_Binary_Operator'               :0b0000000000000000000000000000000000000000000000100000000000000000,
            'IDS_Trinary_Operator'              :0b0000000000000000000000000000000000000000000001000000000000000000,
            'Radical'                           :0b0000000000000000000000000000000000000000000010000000000000000000,
            'Unified_Ideograph'                 :0b0000000000000000000000000000000000000000000100000000000000000000,
            'Deprecated'                        :0b0000000000000000000000000000000000000000001000000000000000000000,
            'Soft_Dotted'                       :0b0000000000000000000000000000000000000000010000000000000000000000,
            'Other_ID_Start'                    :0b0000000000000000000000000000000000000000100000000000000000000000,
            'Other_ID_Continue'                 :0b0000000000000000000000000000000000000001000000000000000000000000,
            'Sentence_Terminal'                 :0b0000000000000000000000000000000000000010000000000000000000000000,
            'Variation_Selector'                :0b0000000000000000000000000000000000000100000000000000000000000000,
            'Pattern_White_Space'               :0b0000000000000000000000000000000000001000000000000000000000000000,
            'Pattern_Syntax'                    :0b0000000000000000000000000000000000010000000000000000000000000000,
            'Prepended_Concatenation_Mark'      :0b0000000000000000000000000000000000100000000000000000000000000000,
            'Other_Default_Ignorable_Code_Point':0b0000000000000000000000000000000001000000000000000000000000000000,
            'ASCII_Hex_Digit'                   :0b0000000000000000000000000000000010000000000000000000000000000000,
            'Logical_Order_Exception'           :0b0000000000000000000000000000000100000000000000000000000000000000,
            'Regional_Indicator'                :0b0000000000000000000000000000001000000000000000000000000000000000,
            'Emoji'                             :0b0000000000000000000000000000010000000000000000000000000000000000,
            'Emoji_Presentation'                :0b0000000000000000000000000000100000000000000000000000000000000000,
            'Emoji_Modifier'                    :0b0000000000000000000000000001000000000000000000000000000000000000,
            'Emoji_Modifier_Base'               :0b0000000000000000000000000010000000000000000000000000000000000000,
            'Emoji_Component'                   :0b0000000000000000000000000100000000000000000000000000000000000000,
            'Extended_Pictographic'             :0b0000000000000000000000001000000000000000000000000000000000000000,
    }

    flag_vals = []
    for k,v in FLAGS.items():

        flag = f"FLAG_{k.upper()}"

        flag_vals.append(f'constexpr uint64_t {flag} = {hex(v)};')

    preamble = '\n'.join(flag_vals)


    global_declares = []
    for k,v in FLAGS.items():

        flag = f"SimplePropertyData::FLAG_{k.upper()}"

        global_declares.append((
            f'bool Unicode::is_{k.lower().replace("-","_")}(Unicode::codepoint_t i) {{ '
            f'return (SimplePropertyData::lookup(i) & {flag}) == {flag}; }}'))


    ret = MegaTable(
            0,
            "uint64_t",
            "SimplePropertyData",
            namespace_preamble = preamble,
            global_postamble = '\n'.join(global_declares),
            out_of_bounds_value = 0,
            sizeof_chunk_elem = 8,
            value_print_converter = hex)

    def eat_file(file_name):
        for parts in ucd.preprocess_parts(file_name):
            codepoint_min, codepoint_max = ucd.parse_codepoint_range(parts[0])
            value = parts[1].strip()

            flag = FLAGS.get(value, None)
            if flag is  None:
                ucd.eprint(f"Ignoring: {hex(codepoint_min)}..{hex(codepoint_max)} {value}")
                continue

            for i in range(codepoint_min, codepoint_max+1):
                ret[i] = ret[i] | flag

    eat_file('PropList.txt')
    eat_file('emoji/emoji-data.txt')
    return ret


def main():
    ucd.print_codegen_header()
    size = get_simple_properties().dump_optimally()
    ucd.eprint("Expected size: " + str(size))

if __name__ == '__main__':
    main()

