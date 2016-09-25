.PHONY: all tests

all:
	cd src && make

tests:
	cd tests && ./runtests.sh
