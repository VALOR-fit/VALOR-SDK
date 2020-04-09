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
		   build-libs \
		   | copy-rootdict-to-lib

# define targets

all:     $(BUILD_TARGETS)
install: 

print-make-info: FORCE
	@echo " "
	@echo " "
	@echo "***** Building VALOR framework from source tree at: $(VALOR_FMWK)"
	@echo "***** The source tree corresponds to VALOR framework version $(VALOR_VERSION)"
	@echo " "

copy-scripts-to-bin: | make-bin-lib-dir FORCE
	@echo " "
	@echo "** Copying scripts to bin..."
	cp ${VALOR_FMWK}/make/valor_fmwk-config ${VALOR_FMWK}/bin
	cp ${VALOR_FMWK}/root/valor ${VALOR_FMWK}/bin
	cp ${VALOR_FMWK}/root/pyvalor ${VALOR_FMWK}/bin
	cp ${VALOR_FMWK}/scripts/data_distribution/valor_data-* ${VALOR_FMWK}/bin
	[ ! -d "${VALOR_FMWK}/external/color/build/" ] || cp "${VALOR_FMWK}/external/color/build/"* "${VALOR_FMWK}/bin"


build-libs: | build-conventions   \
		   build-utilities     \
		   build-messenger     \
		   build-varray        \
		   build-genie-utilities \
		   build-oscillation   \
		   build-ntuple        \
		   build-registry      \
		   build-cmdline       \
		   build-core-fitter   \
		   build-kinebinning   \
		   build-fit-elements  \
		   build-testing       \
		   FORCE

build-conventions: | make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building Conventions package..."${VALOR_FMWK}/src;\
	$(MAKE) -C "${VALOR_FMWK}/src/valor/Conventions"

build-utilities: | make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building Utilities package..."
	$(MAKE) -C "${VALOR_FMWK}/src/valor/Utilities"

build-messenger: | make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building Messenger package..."
	$(MAKE) -C "${VALOR_FMWK}/src/valor/Messenger"

build-varray: | make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building VArray package..."
	$(MAKE) -C "${VALOR_FMWK}/src/valor/VArray"

build-oscillation: | make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building Oscillation package..."
	$(MAKE) -C "${VALOR_FMWK}/src/valor/Oscillation"

build-ntuple: | make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building Ntuple package..."
	$(MAKE) -C "${VALOR_FMWK}/src/valor/Ntuple"

build-registry: | make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building Registry package..."
	$(MAKE) -C "${VALOR_FMWK}/src/valor/Registry"

build-cmdline: | make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building CmdLine package..."
	$(MAKE) -C "${VALOR_FMWK}/src/valor/CmdLine"

build-core-fitter: | make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building FitCore package..."
	$(MAKE) -C "${VALOR_FMWK}/src/valor/FitCore"

build-fit-elements: | make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building FitElements package..."
	$(MAKE) -C "${VALOR_FMWK}/src/valor/FitElements"

build-kinebinning: | make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building KineBinning package..."
	$(MAKE) -C "${VALOR_FMWK}/src/valor/KineBinning"

build-testing: | make-bin-lib-dir FORCE
	@echo " "
	@echo "** Building Testing package..."
	$(MAKE) -C "${VALOR_FMWK}/src/valor/Testing"

build-genie-utilities: | make-bin-lib-dir FORCE
ifeq ("YES","$(OPT_ENABLE_GENIE)")
	@echo " "
	@echo "** Building GENIE Utilities package..."
	$(MAKE) -C ${VALOR_FMWK}/src/valor/GENIEUtilities;
else
	@echo " "
	@echo "** GENIE not enabled - skipping GENIEUtilities"
endif

make-bin-lib-dir: FORCE
	@echo " "
	@echo "** Creating VALOR framework lib and bin directories..."
	cd ${VALOR_FMWK}; \
	[ -d bin ] || mkdir bin; chmod 755 bin;\
	[ -d lib ] || mkdir lib; chmod 755 lib;

copy-rootdict-to-lib: make-bin-lib-dir build-libs FORCE
	@echo " "
	@echo "** Copying ROOT dictionary PCM files to ${VALOR_FMWK}/lib"
	$(shell find "${VALOR_FMWK}/src/valor/" -name "_RootDict*_rdict.pcm" | xargs -n 1 -I{} cp {} "${VALOR_FMWK}/lib/" )

static: FORCE
	@echo " "
	@echo "** Creating static library of the VALOR framework..."
	./make/build_static_object_list.sh --source-path=${VALOR_FMWK}/src/valor | xargs ./make/build_static_lib.sh 

color:
	$(MAKE) -C "${VALOR_FMWK}/external/color"

colorclean:
	rm ./bin/g++ ./bin/gcc ./bin/c++ ./bin/cc 2>/dev/null || true
	$(MAKE) -C "${VALOR_FMWK}/external/color" clean

python:
	@echo "Building python dependencies for VALOR"
	cd "${VALOR_FMWK}/external/python" && . "${VALOR_FMWK}/external/python/build_pyenv.sh" && cd ${VALOR_FMWK}

pythonclean:
	rm -r "${VALOR_FMWK}/external/python/valor" \
		    "${VALOR_FMWK}/external/python/bin" \
				"${VALOR_FMWK}/external/python/lib" \
				"${VALOR_FMWK}/external/python/get-pip.py" 2>/dev/null || true

purge: FORCE
	@echo " "
	@echo "** Purging..."
	cd ${VALOR_FMWK}/src/valor;\
	cd Conventions;               make purge; cd ..; \
	cd Utilities;                 make purge; cd ..; \
	cd Messenger;                 make purge; cd ..; \
	cd VArray;                    make purge; cd ..; \
	cd GENIEUtilities;            make purge; cd ..; \
	cd Oscillation;               make purge; cd ..; \
	cd Ntuple;                    make purge; cd ..; \
	cd Registry;                  make purge; cd ..; \
	cd CmdLine;                   make purge; cd ..; \
	cd FitCore;                   make purge; cd ..; \
	cd KineBinning;               make purge; cd ..; \
	cd FitElements;               make purge; cd ..; \
	cd Testing;                   make purge; cd ..; \
	cd ${VALOR_FMWK}

clean: clean-files clean-dir clean-etc

clean-files: FORCE
	@echo " "
	@echo "** Cleaning..."
	rm -f ${VALOR_FMWK}/bin/valor_fmwk-config; \
	cd ${VALOR_FMWK}/src/valor;\
	cd Conventions;             make clean; cd ..; \
	cd Utilities;               make clean; cd ..; \
	cd Messenger;               make clean; cd ..; \
	cd VArray;                  make clean; cd ..; \
	cd GENIEUtilities;          make clean; cd ..; \
	cd Oscillation;             make clean; cd ..; \
	cd Ntuple;                  make clean; cd ..; \
	cd Registry;                make clean; cd ..; \
	cd CmdLine;                 make clean; cd ..; \
	cd FitCore;                 make clean; cd ..; \
	cd KineBinning;             make clean; cd ..; \
	cd FitElements;             make clean; cd ..; \
	cd Testing;                 make clean; cd ..; \
	cd $(VALOR_FMWK);\
        [ ! -d ./bin ] || rm -f ./bin/*;\
        [ ! -d ./lib ] || rm -f ./lib/*_rdict.pcm;\
        [ ! -d ./lib ] || rm -f ./lib/*a;\
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
	
force-clean: clean-files force-clean-dir clean-etc
	

force-clean-dir: FORCE
	@echo "Force deleting VALOR framework lib and bin directories...";
	cd $(VALOR_FMWK);
	-rm -rf ./bin;
	-rm -rf ./lib 

FORCE:

