#
# Author: Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
#

SHELL    = /bin/sh
NAME     = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
#
include $(VALOR_DUNE)/make/Make.include

PACKAGE           = ValorDuneGenieUtil
DICTIONARY        = _RootDict_$(PACKAGE)
LIBNAME           = lib$(PACKAGE)

# We need the GENIE includes added for this folder
INCLUDES := $(GENIE_INCLUDES) $(INCLUDES)

all : rootcint lib lib-link

# Include standard package makefile targets
#
include $(VALOR_DUNE)/make/Make.template

FORCE:

