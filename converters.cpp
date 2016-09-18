#include <Python.h>
#include "converters.h"
#include "error.h"

#include <iostream>

// Py_XDECREF as a function, used as a callback by emacs' make_user_ptr
static void _PyObject_DecRef(void *obj) noexcept
{
    Py_XDECREF(obj);
}

emacs_value to_emacs(emacs_env *env, PyObject *retval)
{
    // TODO: support non-basic types (eg. nested lists)

    if (PyObject_TypeCheck(retval, &PyInt_Type)) {
        return env->make_integer(
            env, PyInt_AsLong(retval));
    }

    if (PyObject_TypeCheck(retval, &PyFloat_Type)) {
        return env->make_float(
            env, PyFloat_AsDouble(retval));
    }

    if (PyObject_TypeCheck(retval, &PyString_Type)) {
        return env->make_string(
            env, PyString_AsString(retval), PyString_Size(retval));
    }

    // Make a user pointer and return an opaque Python object.
    return env->make_user_ptr(env, _PyObject_DecRef, retval);
}

std::vector<PyObject*> from_emacs(
    emacs_env *env,
    ptrdiff_t nargs,
    emacs_value args[],
    const std::vector<long> &argtypes)
{
    // TODO since this is used just for args passing, build a Python tuple here
    // instaed of a C++ vector, which will have to be unpacked anyway

    if (nargs != argtypes.size()) {
        // TODO This is probably not the right errror to raise here
        throw Error(std::string("Mismatch between Emacs/Python declared number of arguments"));
    }

    std::vector<PyObject*> out(nargs);
    for (size_t i = 0; i < nargs; i++) {
        long typ = argtypes[i];

        PyObject *item;
        switch (typ) {
        case 1:
            item = PyInt_FromLong(env->extract_integer(env, args[i]));
            out[i] = item;
            break;
        default:
            throw Error(std::string("Argument type not supported"));
        }
    }

    return out;
}
