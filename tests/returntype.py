from pymacs_utils import expose


@expose
def method_that_returns_an_int():
    return 42


@expose
def method_that_returns_a_float():
    return 3.1415


@expose
def method_that_returns_a_string():
    return "str"


@expose
def method_that_returns_an_object():
    return object()


@expose
def method_that_returns_a_list():
    return [1, 2, 3, 4]
