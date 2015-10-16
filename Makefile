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
BUILD_TARGETS =    print-make-info     \
		   make-bin-lib-dir    \
		   copy-scripts-to-bin \
		   build-conventions   \
		   build-utilities     \
		   build-messenger     \
		   build-parser        \
		   build-core-fitter   \
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

copy-scripts-to-bin: make-bin-lib-dir FORCE
	@echo " "
	@echo "** Copying scripts to bin..."
	cp ${VALOR_FMWK}/make/valor_fmwk-config ${VALOR_FMWK}/bin
	cp ${VALOR_FMWK}/root/valor ${VALOR_FMWK}/bin
	[ ! -d "${VALOR_FMWK}/external/color/build/" ] || cp "${VALOR_FMWK}/external/color/build/"* "${VALOR_FMWK}/bin"

build-conventions: make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building Conventions package..."${VALOR_FMWK}/src;\
	$(MAKE) -C "${VALOR_FMWK}/src/Conventions"

build-utilities: make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building Utilities package..."
	$(MAKE) -C "${VALOR_FMWK}/src/Utilities"

build-messenger: make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building Messenger package..."
	$(MAKE) -C "${VALOR_FMWK}/src/Messenger"

build-parser: make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building Parser package..."
	$(MAKE) -C "${VALOR_FMWK}/src/Parser"

build-core-fitter: make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building FitCore package..."
	$(MAKE) -C "${VALOR_FMWK}/src/FitCore"

build-example-fits: make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building Example packages..."
	$(MAKE) -C "${VALOR_FMWK}/src/Examples/X01"

make-bin-lib-dir: FORCE
	@echo " "
	@echo "** Creating VALOR framework lib and bin directories..."
	cd ${VALOR_FMWK}; \
	[ -d bin ] || mkdir bin; chmod 755 bin;\
	[ -d lib ] || mkdir lib; chmod 755 lib;

color:
	$(MAKE) -C "${VALOR_FMWK}/external/color"

colorclean:
	rm ./bin/g++ ./bin/gcc ./bin/c++ ./bin/cc 2>/dev/null || true
	$(MAKE) -C "${VALOR_FMWK}/external/color" clean

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
	rm -f ${VALOR_FMWK}/bin/valor_fmwk-config; \
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

