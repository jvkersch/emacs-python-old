#include <Python.h>  // Needs to be included before STL containers

#include "python_interpreter.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>

// Turn a Python list of ints into a vector.
std::vector<long> to_vector(PyObject *obj)
{
    assert(PyIter_Check(obj) == 0);
    std::vector<long> out;

    PyObject *iterator = PyObject_GetIter(obj);
    if (iterator == nullptr) {
        throw PythonError("Object does not support iterating");
    }

    PyObject *item;
    while ((item = PyIter_Next(iterator))) {
        if (!PyInt_Check(item)) {
            throw PythonError("Not an integer");
        }

        long val = PyInt_AsLong(item);
        out.push_back(val);

        Py_DECREF(item);
    }

    Py_DECREF(iterator);

    return out;
}


void PythonInterpreter::start() const
{
    Py_Initialize();
    send_command("import sys");
    send_command("sys.path.append('')");
}


void PythonInterpreter::finalize() const
{
    Py_Finalize();
}


void PythonInterpreter::send_command(const std::string &cmd) const
{
    int ret = PyRun_SimpleString(cmd.c_str());
    if (ret < 0)
        throw_python_error();
}


PyObject *PythonInterpreter::import(const std::string &module_name) const
{
    PyObject *module = PyImport_ImportModule(module_name.c_str());
    if (module == nullptr)
        throw_python_error();
    return module;
}


void PythonInterpreter::add_to_sys_path(const std::string &path) const
{
    std::ostringstream os;
    os << "sys.path.insert(0, \"" << path << "\")";
    send_command(os.str());
}


void PythonInterpreter::throw_python_error() const
{
    PyObject *ptype, *pvalue, *ptraceback;
    PyErr_Fetch(&ptype, &pvalue, &ptraceback);

    // FIXME This function should always throw something, and never return
    if (pvalue == nullptr)
        return;

    char *py_error = PyString_AsString(pvalue);
    std::string msg(py_error);

    Py_XDECREF(ptype);
    Py_XDECREF(pvalue);
    Py_XDECREF(ptraceback);

    throw PythonError(msg);
}


void PythonInterpreter::get_exposed_functions()
{
    PyObject *basemod = import("pymacs_utils");
    PyObject *exposed_methods_dict = PyObject_GetAttrString(
        basemod, "_exposed_methods");
    if ((exposed_methods_dict == nullptr) ||
        !PyDict_Check(exposed_methods_dict))
        throw PythonError("Could not load _exposed_methods dictionary.");

    PyObject *key, *value;
    Py_ssize_t index = 0;
    while (PyDict_Next(exposed_methods_dict, &index, &key, &value)) {
        if (!PyString_Check(key))
            continue;

        Py_INCREF(value);  // FIXME copy string into buffer, and remove INcref
        const char *name = PyString_AsString(key);
        exported_methods_map[std::string(name)] = value;
    }

    // Load argument typecodes
    PyObject *arg_types_dict = PyObject_GetAttrString(
        basemod, "_arg_types");
    if ((arg_types_dict == nullptr) ||
        !PyDict_Check(arg_types_dict))
        throw PythonError("Could not load _arg_types dictionary.");

    // PyObject *key, *value;
    index = 0;
    while (PyDict_Next(arg_types_dict, &index, &key, &value)) {
        if (!PyString_Check(key))
            continue;

        Py_INCREF(value);  // FIXME copy string into buffer, and remove incref
        const char *name = PyString_AsString(key);
        arg_types[std::string(name)] = to_vector(value);
    }

    Py_XDECREF(arg_types_dict);
    Py_XDECREF(exposed_methods_dict);
    Py_XDECREF(basemod);
}


PyObject* PythonInterpreter::call_exposed_function(
    const std::string &name,
    const std::vector<PyObject*> &args) const
{
    PyObject *pyfun;
    try {
        pyfun = exported_methods_map.at(name);
    } catch (std::out_of_range) {
        // TODO instead of messing with string streams here, make constructor
        // take variable arguments and do argument concatenation internally in
        // PythonError
        std::ostringstream os;
        os << "No such Python function: " << name;
        throw PythonError(os.str());
    }

    // assemble arguments tuple
    size_t len = args.size();
    PyObject *args_tuple = PyTuple_New(len);
    for (size_t i = 0; i < len; i++) {
        PyTuple_SetItem(args_tuple, i, args[i]);
    }

    std::cout << "calling with " << len << " arguments" << std::endl;

    PyObject *retval = PyObject_CallObject(pyfun, args_tuple);

    Py_XDECREF(args_tuple);

    if (retval == nullptr)
        throw_python_error();


    return retval;
}
