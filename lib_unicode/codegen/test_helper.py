
def print_codepoint_checker_funcs(
        table_type,
        table_lookup_func):

    print(f"""
void check_codepoint(
        const Unicode::codepoint_t cp,
        const {table_type} expected) {{

    {table_type} actual = {table_lookup_func}(cp);

    if (actual != expected) {{
        TS_FAIL("incorrect response for " 
                + Unicode::to_string(cp)
                + " (" + std::to_string(cp) + ")"
                ", expected "
                + Unicode::to_string(expected)
                + ", got " + Unicode::to_string(actual));
    }}
}}

void check_range(
        Unicode::codepoint_t cp_min,
        Unicode::codepoint_t cp_max,
        {table_type} x) {{

    for (Unicode::codepoint_t i = cp_min; i <= cp_max; i++) {{
        check_codepoint(i, x);
    }}
}}
""")
