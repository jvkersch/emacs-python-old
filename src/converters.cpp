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
    } else if (typ == env->intern(env, "user-ptr")) {
        item = static_cast<PyObject *>(env->get_user_ptr(env, symbol));
    } else
        throw Error(std::string("Argument type not supported"));

    return item;
}


emacs_value to_emacs(emacs_env *env, PyObject *retval)
{
    emacs_value e_retval;

    if (PyObject_TypeCheck(retval, &PyInt_Type)) {
        e_retval = env->make_integer(
            env, PyInt_AsLong(retval));
        Py_DECREF(retval);
        return e_retval;
    }

    if (PyObject_TypeCheck(retval, &PyFloat_Type)) {
        e_retval = env->make_float(
            env, PyFloat_AsDouble(retval));
        Py_DECREF(retval);
        return e_retval;
    }

    if (PyObject_TypeCheck(retval, &PyString_Type)) {
        e_retval = env->make_string(
            env, PyString_AsString(retval), PyString_Size(retval));
        Py_DECREF(retval);
        return e_retval;
    }

    Py_ssize_t len = PyObject_Length(retval);
    if (len != -1) {
        emacs_value Flst = env->intern(env, "list");
        std::vector<emacs_value> lst_args;

        for (Py_ssize_t i = 0; i < len; i++) {
            PyObject *index = PyInt_FromLong(i);
            PyObject *item = PyObject_GetItem(retval, index);
            Py_DECREF(index);

            lst_args.push_back(to_emacs(env, item));
            Py_DECREF(item);
        }

        return env->funcall(env, Flst, len, &lst_args[0]);
    }

    // Make a user pointer and return an opaque Python object.
    return env->make_user_ptr(env, _PyObject_DecRef, retval);
}

PyObject* from_emacs(
    emacs_env *env,
    ptrdiff_t nargs,
    emacs_value args[])
{
    PyObject *args_tuple = PyTuple_New(nargs);

    for (size_t i = 0; i < nargs; i++) {
        PyObject *arg = convert_emacs_symbol(env, args[i]);
        Py_INCREF(arg);
        PyTuple_SetItem(args_tuple, i, arg);
    }

    return args_tuple;
}
