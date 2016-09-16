#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <Python.h>
#include "emacs-module.h"

emacs_value to_emacs(emacs_env *env, PyObject *retval);

#endif // CONVERTERS_H
