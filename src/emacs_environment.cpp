#include "emacs_environment.h"

#include <sstream>


std::string get_string_from_arg(emacs_env *env, emacs_value arg)
{
    if (!env)
        throw EmacsError("Environment not initialized");
    
    bool status = false;
    ssize_t s = 1024;
    std::string str;
    
    for (int i = 0; i < 2; i++) {
        char buf[s];
        status = env->copy_string_contents(env, arg, buf, &s);
        if (status)
            return std::string(buf);
    }
    throw EmacsError("Error retrieving string from Emacs environment.");
}


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
    void *data)
{
    if (!env)
        throw EmacsError("Environment not initialized");

    emacs_value f = env->make_function(
        env,
        min_arity,
        max_arity,
        function,
        doc,
        data);
    
    bind_function(env, name, f);
}


void
bind_function (emacs_env *env, const char *name, emacs_value Sfun)
{
  /* Set the function cell of the symbol named NAME to SFUN using
     the 'fset' function.  */

  /* Convert the strings to symbols by interning them */
  emacs_value Qfset = env->intern (env, "fset");
  emacs_value Qsym = env->intern (env, name);

  /* Prepare the arguments array */
  emacs_value args[] = { Qsym, Sfun };

  /* Make the call (2 == nb of arguments) */
  env->funcall (env, Qfset, 2, args);
}


/* Provide FEATURE to Emacs.  */
void
provide (emacs_env *env, const char *feature)
{
  /* call 'provide' with FEATURE converted to a symbol */

  emacs_value Qfeat = env->intern (env, feature);
  emacs_value Qprovide = env->intern (env, "provide");
  emacs_value args[] = { Qfeat };

  env->funcall (env, Qprovide, 1, args);
}


/* Provide information about an error to Emacs. */
emacs_value
signal_error(emacs_env *env, const std::string &msg)
{
    emacs_value errmsg = env->make_string(env, msg.c_str(), msg.size());

    /* Pack the error message in a string. */
    emacs_value list = env->intern (env, "list");
    emacs_value list_args[] = {errmsg};
    list = env->funcall (env, list, 1, list_args);

    /* Set signal. */
    env->non_local_exit_signal(env, env->intern(env, "error"), list);
    return env->intern(env, "nil");
}
