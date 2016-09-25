Pymacs: a Python module for Emacs
=================================

[![Build Status](https://travis-ci.org/jvkersch/pymacs.svg?branch=master)](https://travis-ci.org/jvkersch/pymacs)

Pymacs is a very lightweight bridge between Emacs 25 and Python. It provides an
Emacs extension module that runs the Python interpreter inside Emacs, so that
Python functions can be run transparently from within Emacs.

Requirements
------------

Pymacs requires a recent build of Emacs (25.1) *with module support
enabled*. Most likely, this will involve compiling Emacs from source with the
`--with-modules` flag.

Pymacs also requires Python. Currently only Python 2.7 is supported, but
getting Python 3 up and running is a priority.

License
-------
Pymacs is released under the [GPL, version 3.0](http://www.gnu.org/licenses/gpl-3.0.en.html).
