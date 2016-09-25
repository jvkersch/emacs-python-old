CFLAGS = -Wall -O0 -ggdb3

PY_CFLAGS := $$(python-config --cflags)
PY_LDFLAGS := $$(python-config --ldflags)
RPATH := $$(../tools/find_rpath.py)

OBJS = pymacs_module.o python_interpreter.o emacs_environment.o converters.o utils.o

pymacs.so: $(OBJS)
	$(CXX) -shared $(OBJS) -o $@ $(PY_LDFLAGS) $(LDFLAGS) -Wl,-rpath,$(RPATH)

%.o: %.cpp
	$(CXX) $(PY_CFLAGS) $(CFLAGS) -I. -fPIC -c -std=c++11 $<

.PHONY: clean tests

clean:
	rm -f pymacs.so *.pyc $(OBJS)
