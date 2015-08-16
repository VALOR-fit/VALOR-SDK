#
# VALOR framework project Makefile.
# 
# Author: Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
#

SHELL = /bin/sh
NAME = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
#
include $(VALOR_FMWK)/make/Make.include

# define composite targets
#
BUILD_TARGETS =    print-make-info    \
		   make-bin-lib-dir   \
		   build-conventions  \
		   build-utilities    \
		   build-messenger    \
		   build-parser       \
		   build-core-fitter  \
                   build-example-fits 

# define targets

all:     $(BUILD_TARGETS)
install: 

print-make-info: FORCE
	@echo " "
	@echo " "
	@echo "***** Building VALOR framework from source tree at: $(VALOR_FMWK)"
	@echo "***** The source tree corresponds to VALOR framework version $(VALOR_VERSION)"
	@echo " "

build-conventions: FORCE
	@echo " "
	@echo "** Building Conventions package..."
	cd ${VALOR_FMWK}/src;\
	cd Conventions;\
	make;\
	cd ${VALOR_FMWK}

build-utilities: FORCE
	@echo " "
	@echo "** Building Utilities package..."
	cd ${VALOR_FMWK}/src;\
	cd Utilities;\
	make;\
	cd ${VALOR_FMWK}

build-messenger: FORCE
	@echo " "
	@echo "** Building Messenger package..."
	cd ${VALOR_FMWK}/src;\
	cd Messenger;\
	make;\
	cd ${VALOR_FMWK}

build-parser: FORCE
	@echo " "
	@echo "** Building Parser package..."
	cd ${VALOR_FMWK}/src;\
	cd Parser;\
	make;\
	cd ${VALOR_FMWK}

build-core-fitter: FORCE
	@echo " "
	@echo "** Building FitCore package..."
	cd ${VALOR_FMWK}/src;\
	cd FitCore;\
	make;\
	cd ${VALOR_FMWK}

build-example-fits: FORCE
	@echo " "
	@echo "** Building Example packages..."
	cd ${VALOR_FMWK}/src;\
	cd Examples/X01;\
	make;\
	cd ${VALOR_FMWK}

make-bin-lib-dir: FORCE
	@echo " "
	@echo "** Creating VALOR framework lib and bin directories..."
	cd ${VALOR_FMWK}; \
	[ -d bin ] || mkdir bin; chmod 755 bin;\
	[ -d lib ] || mkdir lib; chmod 755 lib;

purge: FORCE
	@echo " "
	@echo "** Purging..."
	cd ${VALOR_FMWK}/src;\
	cd Conventions;               make purge; cd ..; \
	cd Utilities;                 make purge; cd ..; \
	cd Messenger;                 make purge; cd ..; \
	cd Parser;                    make purge; cd ..; \
	cd FitCore;                   make purge; cd ..; \
	cd Examples/X01;              make purge; cd ..; \
	cd ${VALOR_FMWK}

clean: clean-files clean-dir clean-etc

clean-files: FORCE
	@echo " "
	@echo "** Cleaning..."
	cd ${VALOR_FMWK}/src;\
	cd Conventions;             make clean; cd ..; \
	cd Utilities;               make clean; cd ..; \
	cd Messenger;               make clean; cd ..; \
	cd Parser;                  make clean; cd ..; \
	cd FitCore;                 make clean; cd ..; \
	cd Examples/X01;            make clean; cd ..; \
	cd $(VALOR_FMWK);\
        [ ! -d ./bin ] || rm ./bin/*;\
	cd $(VALOR_FMWK)

clean-dir: FORCE
	@echo "Deleting VALOR framework lib and bin directories...";\
	cd $(VALOR_FMWK);\
	[ ! -d ./bin ] || rmdir ./bin;\
	[ ! -d ./lib ] || rmdir ./lib 

clean-etc: FORCE
	cd $(VALOR_FMWK); \
	rm -f ./*log; \
	cd ${VALOR_FMWK}

FORCE:

