#!/usr/bin/python3

import os
import sys

import cpp_helper
import sanity


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


def flatten(*args):
    ret = []
    for arg in args:
        for x in arg:
            ret.append(x)
    return ret


class Rule:

    def __init__(self, target):
        self.target = target
        self.prerequisites = set()
        self.recipe = None
        self.phony = False

    def add_prerequisite(self, prerequisite):
        self.prerequisites.add(prerequisite)

    def set_recipe(self, recipe):
        if self.recipe == recipe:
            return
        if self.recipe is not None:
            raise RuntimeError("Recipe already set to something else")
        self.recipe = recipe

    def set_phony(self, phony=True):
        self.phony = phony

    def dump(self):
        recipe = self.recipe
        pres_str = " ".join(self.prerequisites)

        print(f"{self.target}: {pres_str}")

        if recipe is not None:
            if isinstance(recipe, str):
                print("\t" + recipe)
            elif isinstance(recipe, list):
                for ln in recipe:
                    print("\t" + ln)
            else:
                sanity.type_error(recipe)

    def is_empty(self):
        return (len(self.prerequisites) == 0
                and self.recipe is None)



class Makefile:

    def __init__(self):
        self.rules = {}
        self.flags = set()

    def add_flag(self, flag):
        ret = flag in self.flags
        self.flags.add(flag)
        return ret

    def get_flag(flag):
        return flag in self.flags

    def get_rule(self, target):
        if self.rules.get(target) is None:
            self.rules[target] = Rule(target)
        return self.rules.get(target)

    def add_prerequisite(self, target, prerequisite):
        self.get_rule(target).add_prerequisite(prerequisite)

    def add_prerequisites(self, target, prerequisites):
        for prerequisite in prerequisites:
            self.add_prerequisite(target, prerequisite)

    def set_recipe(self, target, recipe):
        self.get_rule(target).set_recipe(recipe)

    def set_phony(self, target, phony=True):
        self.get_rule(target).set_phony(phony)

    def add_rule(self, target, prerequisites = None, recipe = None, phony=None):
        if prerequisites is not None:
            if isinstance(prerequisites, (list, set)):
                self.add_prerequisites(target, prerequisites)
            else:
                self.add_prerequisite(target, prerequisites)
        if recipe is not None:
            self.set_recipe(target, recipe)
        if phony is not None:
            self.set_phony(target, phony)

    def dump(self):
        phonies = []

        print("# This makefile is programmatically generated; do not edit directly")

        for rule in self.rules.values():
            if rule.is_empty():
                continue

            rule.dump()

            if rule.phony:
                phonies.append(rule)

        if len(phonies) > 0:
            print(".PHONY: ", end='')
            for phony in phonies:
                print(phony.target, end=' ')
            print()



class InclusionTracker:

    def __init__(self):
        self.target_inclusions = {}

    def get_inclusion_set(self, target):
        if self.target_inclusions.get(target) is None:
            self.target_inclusions[target] = set()
        return self.target_inclusions.get(target)

    def add_inclusion(self, target, inclusion):
        self.get_inclusion_set(target).add(inclusion)

    def add_inclusions(self, target, inclusions):
        s = self.get_inclusion_set(target)
        for i in inclusions:
            s.add(i)

    def infer_from_cpp_file(self, file, include_dirs):
        inclusions = cpp_helper.find_included_files(file, include_dirs)

        # TODO this is kind of a hack - files with 0 inclusions are read over and over
        # we should track the files we have read (like in a set)
        for inclusion in inclusions:
            if len(self.get_inclusion_set(inclusion)) == 0:
                self.infer_from_cpp_file(inclusion, include_dirs)

        self.add_inclusions(file, inclusions)

    def gen_inclusions(self, file):
        already = set()
        queue = [file]

        while len(queue) > 0:
            file = queue.pop()
            if file in already:
                continue
            already.add(file)
            yield file

            for x in self.get_inclusion_set(file):
                queue.append(x)


# If something (e.g. an include directory) is public, then all projects
# that include this as a dependency will also have those public things
class CxxProject:

    def __init__(self, name, bin_dir, executable=False):
        self.name = name
        self.bin_dir = os.path.join(bin_dir, name)
        self.flag = "CxxProject_" + name

        self.executable = executable
        self.executable_path = os.path.join(self.bin_dir, self.name)

        self.subprojects = []

        self.all_include_dirs = set()
        self.public_include_dirs = set()

        self.source_files = set()
        self.cxxtest_suites = set()

        self.inclusions = InclusionTracker()
  
    def _src2obj(self, source_file):
        source_file_basename = os.path.basename(source_file)
        obj_file_basename = source_file_basename + '.o'
        obj_file = os.path.join(self.bin_dir, obj_file_basename)
        return obj_file

    def add_inclusion(self, target, inclusion):
        self.get_rule(target).add_inclusion(inclusion)

    def add_inclusions(self, target, inclusions):
        for inclusion in inclusions:
            self.add_inclusion(target, inclusion)

    def add_subproject(self, subproj):
        self.subprojects.append(subproj)

    def add_include_dir(self, *include_dir_path_parts, public = True):
        include_dir = os.path.join(*include_dir_path_parts)

        self.all_include_dirs.add(include_dir)
        if public:
            self.public_include_dirs.add(include_dir)

    def gen_public_include_dirs(self):
        for x in self.public_include_dirs:
            yield x
        for subproj in self.subprojects:
            for x in subproj.gen_public_include_dirs():
                yield x

    def gen_all_include_dirs(self):
        for x in self.all_include_dirs:
            yield x
        for subproj in self.subprojects:
            for x in subproj.gen_public_include_dirs():
                yield x

    def gen_cxxtest_suites(self):
        for x in self.cxxtest_suites:
            yield x

    def has_any_cxxtest_suites(self):
        return len(self.cxxtest_suites) > 0

    def add_source_file(self, source_file, headers=None):
        self.source_files.add(source_file)

        if headers is None:
            self.inclusions.infer_from_cpp_file(source_file, [x for x in self.gen_all_include_dirs()])
        else:
            self.inclusions.add_inclusions(source_file, headers)

    def add_source_file_dir(self, source_dir, *files, headers=None):
        for file in files:
            self.add_source_file(
                    os.path.join(source_dir, file),
                    headers=headers)

    def add_cxxtest_suite(self, suite):
        self.cxxtest_suites.add(suite)

    def add_cxxtest_suite_dir(self, suite_dir, *files, headers=None):
        for file in files:
            self.add_cxxtest_suite(
                    os.path.join(suite_dir, file))

    def get_project_objects(self):
        for source_file in self.source_files:
            yield self._src2obj(source_file)

    def gen_linked_objects(self):
        for x in self.get_project_objects():
            yield x
        for dep in self.subprojects:
            for obj in dep.gen_linked_objects():
                yield obj

    def get_inclusions(self, source_file):
        ret = set()
        for x in self.inclusions.gen_inclusions(source_file):
            ret.add(x)
        return ret

    def configure(self, makefile):
        # This is to make sure that we don't run this twice
        if makefile.add_flag(self.flag):
            return

        default_cxx_compile_flags_list = []
        for x in self.gen_all_include_dirs():
            default_cxx_compile_flags_list.append(f"-I{x}")
        default_cxx_compile_flags = " ".join(default_cxx_compile_flags_list)

        cxx_compile = f"$(CXX) {default_cxx_compile_flags}"

        cxx_link = f"$(CXX)"
        
        LINKED_OBJS = [x for x in self.gen_linked_objects()]

        for sf in self.source_files:
            obj = self._src2obj(sf)
            inclusions = self.get_inclusions(sf)
            makefile.add_rule(
                    obj,
                    prerequisites=inclusions,
                    recipe=f"{cxx_compile} -o {obj} -c {sf}")

        if self.has_any_cxxtest_suites():

            test_exe_recipe = self.executable_path + "_test"
            suites = [x for x in self.gen_cxxtest_suites()]
            runner_name = os.path.join(self.bin_dir, self.name + "_test_runner.cpp")
            runner_obj = runner_name + ".o"

            makefile.add_rule(
                    runner_name,
                    suites,
                    f'cxxtestgen --error-printer --have-eh -o {runner_name} {" ".join(suites)}')

            makefile.add_rule(
                    runner_obj,
                    runner_name,
                    f'{cxx_compile} -I$$CXXTEST -o {runner_obj} -c {runner_name}')

            TEST_OBJS = flatten(LINKED_OBJS, [runner_obj])
            makefile.add_rule(
                    test_exe_recipe,
                    TEST_OBJS,
                    f'{cxx_link} -o {test_exe_recipe} {" ".join(TEST_OBJS)}')

            makefile.add_prerequisite("test", test_exe_recipe)

        if self.executable:
            makefile.add_rule(
                    self.executable_path,
                    prerequisites=LINKED_OBJS,
                    recipe=f"{cxx_link} -o {self.executable_path} {' '.join(LINKED_OBJS)}")

            makefile.add_prerequisite("all", self.executable_path)

        for subproj in self.subprojects:
            subproj.configure(makefile)

    def has_cxxtest_suites(self):
        return len(self.cxxtest_suites) > 0

    def get_cxxtest_recipe_name(self):
        return os.path.join(self.bin_dir, "test_" + self.key)

