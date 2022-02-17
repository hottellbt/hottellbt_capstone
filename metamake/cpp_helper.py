import os
import sys


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


def extract_quoted_include(line):
    INCLUDE_DIRECTIVE="#include "

    line = line.strip()
    if line.startswith(INCLUDE_DIRECTIVE):
        line = line[len(INCLUDE_DIRECTIVE):]
        line = line.strip()

        if line[0] == '"' and line[len(line)-1] == '"':
            return line[1:-1]

    return None


def read_quoted_includes(file_path):
    ret = set()

    with open(file_path) as f:
        for ln in f:
            inc = extract_quoted_include(ln)
            
            if inc is not None:
                ret.add(inc)

    return ret


def find_included_files(file_path, include_dirs):
    ret = set()

    actual_include_dirs = [x for x in include_dirs]
    actual_include_dirs.append(os.path.dirname(file_path))

    for quoted_include in read_quoted_includes(file_path):
        candidates = set()

        found_one = False

        for include_dir in actual_include_dirs:
            candidate = os.path.join(include_dir, quoted_include)

            if os.path.isfile(candidate):
                ret.add(candidate)
                found_one = True
                break

        if not found_one:
            eprint(f"Failed to find the included file '{quoted_include}' for {file_path}")
            raise RuntimeError()

    return ret
