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
