.PHONY: all tests

all:
	cd src && make
	cp src/pymacs.so .

tests:
	cd tests && ./runtests.sh
