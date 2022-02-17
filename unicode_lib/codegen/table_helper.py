def prepend_x(value):
    if not isinstance(value, str):
        raise TypeError("expected string, got " + str(type(value)) + " (" + str(value) + ")")
    return "X::" + value


