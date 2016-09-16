CPP = g++
LD = g++

CFLAGS = -Wall -O0 -ggdb3

PY_CFLAGS := $$(python-config --cflags)
PY_LDFLAGS := $$(python-config --ldflags)
RPATH := $$(./find_rpath.py)

OBJS = pymacs_module.o python_interpreter.o emacs_environment.o converters.o

pymacs.so: $(OBJS)
	$(LD) -shared $(PY_LDFLAGS) $(LDFLAGS) -Wl,-rpath,$(RPATH) -o $@ $(OBJS)

%.o: %.cpp
	$(CPP) $(PY_CFLAGS) $(CFLAGS) -I. -fPIC -c -std=c++11 $<

.PHONY: clean tests

clean:
	rm -f pymacs.so $(OBJS)

tests:
	PYTHONPATH=tests emacs -batch -l ert -l tests/*.el -f ert-run-tests-batch-and-exit