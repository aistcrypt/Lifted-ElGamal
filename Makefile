include common.mk

all:
	-$(MKDIR) bin
	$(MAKE) -C test
	$(MAKE) -C src

test:
	$(MAKE) -C test test

clean:
#	$(MAKE) -C src clean
	$(MAKE) -C test clean

header:
	python tool/dispatch.py tool/dispatch_tmpl.txt include/impl/elgamal_dispatch.hpp
