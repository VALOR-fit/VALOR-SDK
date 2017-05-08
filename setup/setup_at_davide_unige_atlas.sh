#!/bin/bash

#
# T2KANA oscillation analysis code setup script for Costas' MacBookPro
#
# Input argument: oscillation analysis code top-level directory
#
 
export VALOR_FMWK=$1

source "/cvmfs/atlas.cern.ch/repo/sw/atlas-gcc/484/x86_64/setup.sh" # use G++ 484
source "/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/5.34.25-x86_64-slc6-gcc48-opt/bin/thisroot.sh" # and appropriate ROOT

tmppy="/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/python/2.7.4-x86_64-slc6-gcc48/sw/lcg/external/Python/2.7.4/x86_64-slc6-gcc48-opt/"
export PATH="${tmppy}/bin:${PATH}"
export PYTHONPATH="${tmppy}/lib/python2.7:${PYTHONPATH}"
export LD_LIBRARY_PATH="${tmppy}/lib:${LD_LIBRARY_PATH}"

export LD_LIBRARY_PATH=${LIBLOG4CPP}:${VALOR_FMWK}/lib:${ROOTSYS}/lib:${LD_LIBRARY_PATH}:/usr/lib64/
export DYLD_LIBRARY_PATH=${VALOR_FMWK}/lib:${ROOTSYS}/lib:${DYLD_LIBRARY_PATH}
export PATH=${LIBLOG4CPP}:${VALOR_FMWK}/root:${VALOR_FMWK}/production:${ROOTSYS}/bin:${VALOR_FMWK}/lib:${VALOR_FWMK}/bin:${PATH}
