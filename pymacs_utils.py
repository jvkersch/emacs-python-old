_exposed_methods = {}
_arg_types = {}

# This is a kludge, better to let PythonInterpreter handle Python types
# directly.
DEFAULT_TYPECODE = 0
TYPEMAP = {
    int: 1,
    float: 2,
    str: 3,
}


def expose(**argtypes):
    def wrapper(fn):
        _exposed_methods[fn.__name__] = fn

        args = argtypes.get('args', [])
        _arg_types[fn.__name__] = [
            TYPEMAP.get(typ, DEFAULT_TYPECODE) for typ in args
        ]
        return fn
    return wrapper
