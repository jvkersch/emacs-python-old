#ifndef EMACS_ENVIRONMENT_H
#define EMACS_ENVIRONMENT_H

#include <string>

#include "emacs-module.h"
#include "error.h"


// Un-marshall string from Emacs argument.
std::string get_string_from_arg(emacs_env *env, emacs_value arg);

// Define emacs function.
void defun(
    emacs_env *env,
    const char *name,
    ptrdiff_t min_arity,
    ptrdiff_t max_arity,
    emacs_value (*function) (
        emacs_env *env,
        ptrdiff_t nargs,
        emacs_value args[],
        void *),
    const char *doc,
    void *data);

// Provide feature to Emacs environment.
void provide(emacs_env *env, const char *feature);

// Bind symbol to name.
void bind_function(emacs_env *env, const char *name, emacs_value Sfun);

// Propagate error up to Emacs.
emacs_value signal_error(emacs_env *env, const std::string &msg);


class EmacsError : public Error
{
public:
    EmacsError(const std::string &msg) : Error(msg) {}
};


#endif // EMACS_ENVIRONMENT_H
