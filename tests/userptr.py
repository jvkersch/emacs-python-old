from pymacs_utils import expose


class MyObject(object):

    def __init__(self):
        self.counter = 0


@expose
def make_object():
    return MyObject()


@expose
def increase_counter(obj):
    obj.counter += 1
    return obj.counter


@expose
def get_value(obj):
    return obj.counter
