#
# Author: Andy Chappell <A.Chappell \at warwick.ac.uk>
#

SHELL    = /bin/sh
NAME     = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
#
include $(VALOR_FMWK)/make/Make.include

PACKAGE           = ValorMarginalisation
DICTIONARY        = _RootDict_$(PACKAGE)
LIBNAME           = lib$(PACKAGE)

all : rootcint lib lib-link

# Include standard package makefile targets
#
include $(VALOR_FMWK)/make/Make.template

FORCE:

