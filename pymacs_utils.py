_exposed_methods = {}


def expose(fn):
    _exposed_methods[fn.__name__] = fn
    return fn
