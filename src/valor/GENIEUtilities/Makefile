#
# Author: Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
#

SHELL    = /bin/sh
NAME     = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
#
include $(VALOR_FMWK)/make/Make.include

PACKAGE           = ValorGenieUtil
DICTIONARY        = _RootDict_$(PACKAGE)
LIBNAME           = lib$(PACKAGE)

# We need the GENIE includes added for this folder
INCLUDES := $(GENIE_INCLUDES) $(INCLUDES)

all : rootcint lib lib-link

# Include standard package makefile targets
#
include $(VALOR_FMWK)/make/Make.template

FORCE:

