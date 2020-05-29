#
# Author: Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
#

SHELL    = /bin/sh
NAME     = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
#
include $(VALOR_FMWK)/make/Make.include

PACKAGE           = ValorCmdLine
DICTIONARY        = undefined
LIBNAME           = lib$(PACKAGE)

all : lib lib-link

# Include standard package makefile targets
#
include $(VALOR_FMWK)/make/Make.template

FORCE:

