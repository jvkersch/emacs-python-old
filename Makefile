CPP = g++
LD = g++

CFLAGS = -Wall -O0 -ggdb3

PY_CFLAGS := $$(python-config --cflags)
PY_LDFLAGS := $$(python-config --ldflags)
RPATH := $$(./find_rpath.py)

OBJS = pymacs_module.o python_interpreter.o emacs_environment.o

pymacs.so: $(OBJS)
	$(LD) -shared $(PY_LDFLAGS) $(LDFLAGS) -Wl,-rpath,$(RPATH) -o $@ $(OBJS)

%.o: %.cpp
	$(CPP) $(PY_CFLAGS) $(CFLAGS) -I. -fPIC -c -std=c++11 $<

.PHONY: clean

clean:
	rm -f pymacs.so $(OBJS)
