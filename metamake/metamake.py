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
        self.file_inclusions = {}
        self.examined_files = set()

    def _get_inclusion_set(self, file):
        if file in self.examined_files:
            return self.file_inclusions[file]
        else:
            ret = set()
            self.file_inclusions[file] = ret;
            return ret

    def set_inclusions(self, file, inclusions):
        s = self._get_inclusion_set(file)
        for i in inclusions:
            s.add(i)
        self.examined_files.add(file)

    def infer_from_cpp_file(self, file, include_dirs):
        if file in self.examined_files:
            return

        inclusions = cpp_helper.find_included_files(file, include_dirs)

        for inclusion in inclusions:
            self.infer_from_cpp_file(inclusion, include_dirs)

        self.set_inclusions(file, inclusions)

    def get_inclusions(self, file, include_dirs):
        if not file in self.examined_files:
            if file.endswith('.hpp') or file.endswith('.cpp'):
                self.infer_from_cpp_file(file, include_dirs)

        ret = set()
        queue = [file]

        while len(queue) > 0:
            file = queue.pop()
            if file in ret:
                continue
            ret.add(file)

            for x in self._get_inclusion_set(file):
                queue.append(x)

        return ret


# If something (e.g. an include directory) is public, then all projects
# that include this as a dependency will also have those public things
class CxxProject:

    def __init__(self, name, bin_dir, executable=False):
        self.name = name
        self.bin_dir = os.path.join(bin_dir, name)
        self.flag = "CxxProject_" + name

        self.executable = executable
        self.executable_path = os.path.join(self.bin_dir, self.name)

        self.links = set()

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

    def add_link(self, link):
        self.links.add(link)

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

    def get_all_links(self):
        ret = set()
        for x in self.links:
            ret.add(x)
        for subproj in self.subprojects:
            for x in subproj.get_all_links():
                ret.add(x)
        return ret

    def get_public_include_dirs(self):
        ret = set()
        for x in self.public_include_dirs:
            ret.add(x)
        for subproj in self.subprojects:
            for x in subproj.get_public_include_dirs():
                ret.add(x)
        return ret

    def get_all_include_dirs(self):
        ret = set()
        for x in self.all_include_dirs:
            ret.add(x)
        for subproj in self.subprojects:
            for x in subproj.get_public_include_dirs():
                ret.add(x)
        return ret

    def get_cxxtest_suites(self):
        ret = set()
        for x in self.cxxtest_suites:
            ret.add(x)
        return ret

    def has_any_cxxtest_suites(self):
        return len(self.cxxtest_suites) > 0

    def add_source_file(self, source_file, headers=None):
        self.source_files.add(source_file)

        if headers is not None:
            self.inclusions.set_inclusions(source_file, headers)

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

    def get_linked_objects(self):
        ret = set()

        for x in self.get_project_objects():
            ret.add(x)

        for dep in self.subprojects:
            for obj in dep.get_linked_objects():
                ret.add(obj)

        return ret

    def get_inclusions(self, source_file):
        return self.inclusions.get_inclusions(source_file, self.get_all_include_dirs())

    def configure(self, makefile):
        # This is to make sure that we don't run this twice
        if makefile.add_flag(self.flag):
            return

        cxx_i_flags = []
        for x in self.get_all_include_dirs():
            cxx_i_flags.append(f"-I{x}")

        cxx_l_flags = []
        for x in self.get_all_links():
            cxx_l_flags.append(f"-l{x}")

        cxx_i_flags_str = ' '.join(cxx_i_flags)
        cxx_l_flags_str = ' '.join(cxx_l_flags)

        cxx = "$(CXX) -g -Og -Werror -Wall -std=c++17"

        cxx_compile = f"{cxx} {cxx_i_flags_str}"

        cxx_link = f"{cxx} {cxx_l_flags_str}"
        
        LINKED_OBJS = self.get_linked_objects()

        for sf in self.source_files:
            obj = self._src2obj(sf)
            inclusions = self.get_inclusions(sf)
            makefile.add_rule(
                    obj,
                    prerequisites=inclusions,
                    recipe=f"mkdir -p {os.path.dirname(obj)} && {cxx_compile} -o {obj} -c {sf}")

        if self.has_any_cxxtest_suites():

            test_exe_recipe = self.executable_path + "_test"
            suites = self.get_cxxtest_suites()

            runner_prerequisites = set()
            for suite in suites:
                runner_prerequisites.add(suite)
                for x in self.get_inclusions(suite):
                    runner_prerequisites.add(x)

            runner_name = os.path.join(self.bin_dir, self.name + "_test_runner.cpp")
            runner_obj = runner_name + ".o"
            runner_obj_dir = os.path.dirname(runner_obj)

            makefile.add_rule(
                    runner_name,
                    runner_prerequisites,
                    f'mkdir -p {runner_obj_dir} && cxxtestgen --error-printer --have-eh -o {runner_name} {" ".join(suites)}')

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

