#include <Python.h>
#include "converters.h"

// Py_XDECREF as a function, used as a callback by emacs' make_user_ptr
static void _PyObject_DecRef(void *obj) noexcept
{
    Py_XDECREF(obj);
}

emacs_value to_emacs(emacs_env *env, PyObject *retval)
{
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
