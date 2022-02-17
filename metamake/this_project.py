import os
from metamake import CxxProject

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

    def add_codegen(generator, result):
        nonlocal makefile

        UCD_DIR = os.path.join(home, "ucd")

        makefile.add_prerequisites(
                result,
                [generator, 
                    os.path.join(codegen, "ucd.py"),
                    os.path.join(codegen, "megatable.py")])

        makefile.set_recipe(
                result,
                f'UCD_DIR="{UCD_DIR}" python3 {generator} > {result}')

    def add_codegen_src(name):
        nonlocal l, add_codegen, codegen, src
        generator = os.path.join(codegen, f"gen_{name}.py")
        result = os.path.join(src, f"auto_{name}.cpp")
        add_codegen(generator, result)
        l.add_source_file(result, headers=[os.path.join(include, "unicode.hpp")])

    add_codegen_src("general_category")
    add_codegen_src("hangul_syllable_type")
    add_codegen_src("indic_syllabic_category")
    add_codegen_src("line_break_property")
    add_codegen_src("script")
    add_codegen_src("simple_property")

    def add_codegen_test(name):
        nonlocal l, add_codegen, codegen, test
        generator = os.path.join(codegen, f"gen_test_{name}.py")
        result = os.path.join(test, f"auto_test_{name}.hpp")
        add_codegen(generator, result)
        l.add_cxxtest_suite(result)

    add_codegen_test("grapheme_cluster_break_property")
    add_codegen_test("hangul_syllable_type")
    add_codegen_test("indic_syllabic_category")
    add_codegen_test("line_break_property")
    add_codegen_test("sentence_break_property")
    add_codegen_test("simple_property")

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

