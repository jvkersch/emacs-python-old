#ifndef PYTHON_INTERPRETER_H
#define PYTHON_INTERPRETER_H

#include <map>
#include <string>
#include <vector>

#include <Python.h>

#include "error.h"


class PythonInterpreter
{
public:
    void start() const;
    void finalize() const;

    void send_command(const std::string &cmd) const;
    PyObject *import(const std::string &module_name) const;
    void add_to_sys_path(const std::string &path) const;

    void get_exposed_functions();
    PyObject *call_exposed_function(
        const std::string &name, const std::vector<PyObject*> &args) const;

    // TODO make this private
    typedef std::map<std::string, PyObject*> pyfun_map;
    pyfun_map exported_methods_map;

    std::map<std::string, std::vector<long> > arg_types;

protected:
    void throw_python_error() const;
};


class PythonError : public Error
{
public:
    PythonError(const std::string &msg) : Error(msg) {}
};

#endif // PYTHON_INTERPRETER_H
