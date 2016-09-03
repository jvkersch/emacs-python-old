_exposed_methods = {}
_return_types = {}


def expose(**argtypes):
    def wrapper(fn):
        _exposed_methods[fn.__name__] = fn
        _return_types[fn.__name__] = argtypes.get('return_type', 'int')
        return fn
    return wrapper
