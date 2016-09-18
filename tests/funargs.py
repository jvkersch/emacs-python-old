from pymacs_utils import expose


@expose(args=(int, int))
def py_add(x, y):
    return x + y
