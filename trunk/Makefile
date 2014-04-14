#
# Main VALOR/LBNx project Makefile.
# 
# Author: Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
#

SHELL = /bin/sh
NAME = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
#
include $(VALOR_LBNX)/src/make/Make.include

# define composite targets
#
BUILD_TARGETS =    print-make-info    \
		   make-bin-lib-dir   \
		   build-conventions  \
		   build-utilities    \
		   build-messenger    \
		   build-core-fitter  \
		   build-example-fits \
		   build-nd-fit

# define targets

all:     $(BUILD_TARGETS)
install: 

print-make-info: FORCE
	@echo " "
	@echo " "
	@echo "***** Building VALOR/LBNx from source tree at: $(VALOR_LBNX)"
	@echo "***** The source tree corresponds to VALOR version $(VALOR_VERSION)"
	@echo " "

build-conventions: FORCE
	@echo " "
	@echo "** Building Conventions package..."
	cd ${VALOR_LBNX}/src;\
	cd Conventions;\
	make;\
	cd ${VALOR_LBNX}

build-utilities: FORCE
	@echo " "
	@echo "** Building Utilities package..."
	cd ${VALOR_LBNX}/src;\
	cd Utilities;\
	make;\
	cd ${VALOR_LBNX}

build-messenger: FORCE
	@echo " "
	@echo "** Building Messenger package..."
	cd ${VALOR_LBNX}/src;\
	cd Messenger;\
	make;\
	cd ${VALOR_LBNX}

build-core-fitter: FORCE
	@echo " "
	@echo "** Building FitCore package..."
	cd ${VALOR_LBNX}/src;\
	cd FitCore;\
	make;\
	cd ${VALOR_LBNX}

build-example-fits: FORCE
	@echo " "
	@echo "** Building Example packages..."
	cd ${VALOR_LBNX}/src;\
	cd Examples/X01;\
	make;\
	cd ${VALOR_LBNX}

build-nd-fit: FORCE
	@echo " "
	@echo "** Building NDFit package..."
	cd ${VALOR_LBNX}/src;\
	cd NDFit;\
	make;\
	cd ${VALOR_LBNX}

make-bin-lib-dir: FORCE
	@echo " "
	@echo "** Creating VALOR/LBNx lib and bin directories..."
	cd ${VALOR_LBNX}; \
	[ -d bin ] || mkdir bin; chmod 755 bin;\
	[ -d lib ] || mkdir lib; chmod 755 lib;

purge: FORCE
	@echo " "
	@echo "** Purging..."
	cd ${VALOR_LBNX}/src;\
	cd Conventions;               make purge; cd ..; \
	cd Utilities;                 make purge; cd ..; \
	cd Messenger;                 make purge; cd ..; \
	cd FitCore;                   make purge; cd ..; \
	cd Examples/X01;              make purge; cd ../..; \
	cd NDFit;                     make purge; cd ..; \
	cd ${VALOR_LBNX}

clean: clean-files clean-dir clean-etc

clean-files: FORCE
	@echo " "
	@echo "** Cleaning..."
	cd ${VALOR_LBNX}/src;\
	cd Conventions;             make clean; cd ..; \
	cd Utilities;               make clean; cd ..; \
	cd Messenger;               make clean; cd ..; \
	cd FitCore;                 make clean; cd ..; \
	cd Examples/X01;            make clean; cd ../..; \
	cd NDFit;                   make clean; cd ..; \
	cd $(VALOR_LBNX);\
    [ ! -d ./bin ] || rm ./bin/*;\
	cd $(VALOR_LBNX)

clean-dir: FORCE
	@echo "Deleting VALOR/LBNx lib and bin directories...";\
	cd $(VALOR_LBNX);\
	[ ! -d ./bin ] || rmdir ./bin;\
	[ ! -d ./lib ] || rmdir ./lib 

clean-etc: FORCE
	cd $(VALOR_LBNX); \
	rm -f ./*log; \
	cd ${VALOR_LBNX}

FORCE:

