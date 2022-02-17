import os
from metamake import CxxProject, flatten

def get_unicode_lib(makefile, home, bin_dir):
    src = os.path.join(home, "src")
    test = os.path.join(home, "test")
    codegen = os.path.join(home, "codegen")
    include = os.path.join(home, "include")

    l = CxxProject("unicode_lib", bin_dir)

    l.add_include_dir(home, "include")

    l.add_source_file(os.path.join(src, "utf8.cpp"))
    l.add_source_file(os.path.join(src, "unicode.cpp"))

    # Manual tests
    l.add_cxxtest_suite_dir(
            test,
            "test_codepoint_to_string.hpp",
            "test_utf8.hpp")

    def add_codegen(generator, result, ucd_files):
        nonlocal makefile

        UCD_DIR = os.path.join(home, "ucd")

        ucd_files_full = [os.path.join(UCD_DIR, x) for x in ucd_files]

        makefile.add_prerequisites(
                result,
                flatten(
                    [generator,
                        os.path.join(codegen, "ucd.py"),
                        os.path.join(codegen, "megatable.py")],
                    ucd_files_full))

        makefile.set_recipe(
                result,
                f'UCD_DIR="{UCD_DIR}" python3 {generator} > {result}')


        for ucd_file in ucd_files:
            
            ucd_file_full = os.path.join(UCD_DIR, ucd_file)
            ucd_file_dir = os.path.dirname(ucd_file_full)

            makefile.set_recipe(
                    ucd_file_full,
                    f"mkdir -p '{ucd_file_dir}' && cd '{ucd_file_dir}' && wget 'https://www.unicode.org/Public/UCD/latest/ucd/{ucd_file}'")

    def add_codegen_src(name, ucd_files):
        nonlocal l, add_codegen, codegen, src
        generator = os.path.join(codegen, f"gen_{name}.py")
        result = os.path.join(src, f"auto_{name}.cpp")
        add_codegen(generator, result, ucd_files)
        l.add_source_file(result, headers=[os.path.join(include, "unicode.hpp")])

    add_codegen_src("general_category",        ["UnicodeData.txt"                     ])
    add_codegen_src("hangul_syllable_type",    ["HangulSyllableType.txt"              ])
    add_codegen_src("indic_syllabic_category", ["IndicSyllabicCategory.txt"           ])
    add_codegen_src("line_break_property",     ["LineBreak.txt"                       ])
    add_codegen_src("script",                  ["Scripts.txt"                         ])
    add_codegen_src("simple_property",         ["PropList.txt", "emoji/emoji-data.txt"])

    def add_codegen_test(name, ucd_files):
        nonlocal l, add_codegen, codegen, test
        generator = os.path.join(codegen, f"gen_test_{name}.py")
        result = os.path.join(test, f"auto_test_{name}.hpp")
        add_codegen(generator, result, ucd_files)
        l.add_cxxtest_suite(result)

    add_codegen_test("grapheme_cluster_break_property", ["auxiliary/GraphemeBreakProperty.txt" ])
    add_codegen_test("hangul_syllable_type",            ["HangulSyllableType.txt"              ])
    add_codegen_test("indic_syllabic_category",         ["IndicSyllabicCategory.txt"           ])
    add_codegen_test("line_break_property",             ["LineBreak.txt"                       ])
    add_codegen_test("sentence_break_property",         ["auxiliary/SentenceBreakProperty.txt" ])
    add_codegen_test("simple_property",                 ["PropList.txt", "emoji/emoji-data.txt"])

    return l


def get_main_project(makefile, home, bin_dir):
    unicode_lib = get_unicode_lib(makefile, os.path.join(home, "unicode_lib"), bin_dir)

    src = os.path.join(home, "src")
    include = os.path.join(home,  "include")

    proj = CxxProject("hottellbt_capstone", bin_dir, executable=True)
    proj.add_subproject(unicode_lib)

    proj.add_include_dir(include)

    proj.add_source_file(os.path.join(src, "main.cpp"))

    proj.configure(makefile)

    return proj


def configure(makefile, home, bin_dir):
    main_project = get_main_project(makefile, home, bin_dir)
    main_project.configure(makefile)

    # special recipes
    makefile.add_rule("clean", recipe=f"find {bin_dir} -type f | xargs rm", phony=True)
    makefile.add_rule("run", prerequisites=[main_project.executable_path], recipe=f"./{main_project.executable_path}", phony=True)
    makefile.add_rule("test", recipe='@for x in $^ ; do echo "$$x" && ./"$$x" ; done', phony=True)

