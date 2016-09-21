#include <Python.h>
#include "converters.h"
#include "emacs_environment.h"
#include "error.h"

#include <iostream>


// Py_XDECREF as a function, used as a callback by emacs' make_user_ptr
static void _PyObject_DecRef(void *obj) noexcept
{
    Py_XDECREF(obj);
}


PyObject *convert_emacs_symbol(emacs_env *env, emacs_value symbol)
{
    emacs_value typ = env->type_of(env, symbol);
    PyObject *item = nullptr;

    // TODO add support for user-ptr

    // TODO Persist these symbols rather than instantiating them on each
    // invocation.
    if (typ == env->intern(env, "integer")) {
        item = PyInt_FromLong(env->extract_integer(env, symbol));
    } else if (typ == env->intern(env, "float")) {
        item = PyFloat_FromDouble(env->extract_float(env, symbol));
    } else if (typ == env->intern(env, "string")) {
        item = PyString_FromString(
            get_string_from_arg(env, symbol).c_str());
    } else if (typ == env->intern(env, "vector")) {
        ptrdiff_t size = env->vec_size(env, symbol);
        item = PyList_New(size);
        if (item == nullptr)
            throw Error(std::string("List creation failed"));
        for (ptrdiff_t i = 0; i < size; i++) {
            PyObject *list_item = convert_emacs_symbol(
                env, env->vec_get(env, symbol, i));
            PyList_SetItem(item, i, list_item);
        }
    } else
        throw Error(std::string("Argument type not supported"));

    return item;
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
    emacs_value args[])
{
    // TODO since this is used just for args passing, build a Python tuple here
    // instead of a C++ vector, which will have to be unpacked anyway

    std::vector<PyObject*> out(nargs);
    for (size_t i = 0; i < nargs; i++) {
        out[i] = convert_emacs_symbol(env, args[i]);
    }

    return out;
}
