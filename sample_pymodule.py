from pymacs_utils import expose


@expose(return_type='int')
def my_method():
    return 42
