from pymacs_utils import expose


@expose(args=(int, int))
def py_add_int(x, y):
    return x + y


@expose(args=(float, float))
def py_add_float(x, y):
    return x + y


@expose(args=(str,))
def py_len_str(s):
    return len(s)
