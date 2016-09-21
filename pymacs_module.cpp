#include "emacs-module.h"

#include "converters.h"
#include "emacs_environment.h"
#include "python_interpreter.h"

#include <string>
#include <iostream>

extern "C"
{
int plugin_is_GPL_compatible;

PythonInterpreter interpreter;

static emacs_value
F_call_python_function(
    emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data) noexcept
{
    const char *funname = (const char *)data;
    emacs_value retval;

    try {
        std::vector<PyObject*> pyargs = from_emacs(env, nargs, args);
        PyObject *pyretval = interpreter.call_exposed_function(funname, pyargs);
        retval = to_emacs(env, pyretval);
    } catch (const Error &err) {
        retval = signal_error(env, err);
    }

    return retval;
}


static emacs_value
F_load_python_module(
    emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data) noexcept
{
    try {
        std::string module_name = get_string_from_arg(env, args[0]);

        interpreter.import(module_name);
        interpreter.get_exposed_functions();

        for (auto &namedfun : interpreter.exported_methods_map) {
            const char *name = namedfun.first.c_str();
            defun(env, name, 0, 32,  // TODO get min/max arity from Python?
                  F_call_python_function, "doc", (void *)name);
        }

    } catch (const Error &err) {
        return signal_error(env, err);
    }

    return env->intern (env, "t");
}


static emacs_value
F_add_to_sys_path(
    emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data) noexcept
{
    try {
        std::string path = get_string_from_arg(env, args[0]);
        interpreter.add_to_sys_path(path);
    } catch (const Error &err) {
        return signal_error(env, err);
    }

    return env->intern (env, "t");
}


int emacs_module_init(struct emacs_runtime *ert)
{
    emacs_env *env = ert->get_environment (ert);
    interpreter.start();

    defun(
        env,
        "load-python-module",
        1,
        1,
        F_load_python_module,
        "Load Python module with exposed functions.",
        NULL);

    defun(
        env,
        "add-to-sys-path",
        1,
        1,
        F_add_to_sys_path,
        "Add folder to sys.path",
        NULL);

    provide(env, "pymacs");

    return 0;
}
}
