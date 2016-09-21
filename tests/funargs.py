from pymacs_utils import expose


@expose
def py_add_int(x, y):
    return x + y


@expose
def py_add_float(x, y):
    return x + y


@expose
def py_len_str(s):
    return len(s)
