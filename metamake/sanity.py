import os


def require_type(obj, _type):
    if not isinstance(obj, _type):
        raise TypeError(f"Expected type {str(_type)} but got {str(type(obj))}: {str(obj)}")
    return obj


def require_dir(path):
    require_type(path, str)
    if not os.path.isdir(path):
        raise RuntimeError("Not a directory: " + path)
    return path


def require_or_make_dir(path):
    require_type(path, str)
    if os.path.isdir(path):
        return path
    os.makedirs(path)
    return path


def require_file(path):
    require_type(path, str)
    if not os.path.isfile(path):
        raise RuntimeError("Not a file: " + path)
    return path

def type_error(obj):
    raise TypeError(f"Unexpected type {str(type(obj))}: {str(obj)}")
