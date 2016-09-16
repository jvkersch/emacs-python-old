#include <Python.h>  // Needs to be included before STL containers

#include "python_interpreter.h"

#include <iostream>
#include <sstream>
#include <map>


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

        Py_INCREF(value);
        const char *name = PyString_AsString(key);
        exported_methods_map[std::string(name)] = value;
    }
    
    Py_XDECREF(exposed_methods_dict);
    Py_XDECREF(basemod);
}


PyObject* PythonInterpreter::call_exposed_function(
    const std::string &name) const
{
    PyObject *pyfun;
    try {
        pyfun = exported_methods_map.at(name);
    } catch (std::out_of_range) {
        std::ostringstream os;
        os << "No such Python function: " << name;
        throw PythonError(os.str());
    }

    PyObject *retval = PyObject_CallObject(pyfun, NULL);
    if (retval == nullptr)
        throw_python_error();
    
    return retval;
}
