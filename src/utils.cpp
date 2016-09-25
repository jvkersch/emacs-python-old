#include <sstream>
#include <stdexcept>

#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include "utils.h"


std::string get_working_path()
{
    // Adapted from http://stackoverflow.com/a/2203853/394770
    // but see http://insanecoding.blogspot.co.uk/2007/11/pathmax-simply-isnt.html
    // probably should use boost::path here

    char temp[PATH_MAX];
    if ( getcwd(temp, PATH_MAX) != 0) 
        return std::string (temp);

    int error = errno;

    switch (error) {
        // EINVAL can't happen - size argument > 0

        // PATH_MAX includes the terminating nul, 
        // so ERANGE should not be returned

    case EACCES:
        throw std::runtime_error("Access denied");

    case ENOMEM:
        // I'm not sure whether this can happen or not 
        throw std::runtime_error("Insufficient storage");

    default: {
        std::ostringstream str;
        str << "Unrecognised error" << error;
        throw std::runtime_error(str.str());
    }
    }
}
