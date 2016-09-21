from pymacs_utils import expose


@expose
def py_add_int(x, y):
    return x + y


@expose
def py_add_float(x, y):
    return x + y


@expose
def py_len(s):
    return len(s)


@expose
def py_nested_len(arg):
    # arg is a list of lists of strings
    flattened = sum(arg, [])
    return sum(len(s) for s in flattened)
