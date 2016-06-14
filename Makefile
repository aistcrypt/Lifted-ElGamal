include common.mk

all: bin
	$(MAKE) -C test
	$(MAKE) -C src

test: bin
	$(MAKE) -C test test

bin:
	$(MKDIR) bin

clean:
#	$(MAKE) -C src clean
	$(MAKE) -C test clean

header:
	python tool/dispatch.py tool/dispatch_tmpl.txt include/impl/elgamal_dispatch.hpp
