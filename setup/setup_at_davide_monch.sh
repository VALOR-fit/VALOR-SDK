#!/bin/bash

#
# T2KANA oscillation analysis code setup script for Costas' MacBookPro
#
# Input argument: oscillation analysis code top-level directory
#
 
export VALOR_FMWK=$1
# #export ROOTSYS=/hep/root/v5.32.01/
# #export ROOTSYS=/Users/davidesgalaberna/root/

# #ROOTwork_nd280

# #source /users/davides/root_v5_34_34/root/bin/thisroot.sh
# export ROOTSYS=/usr/lib64/

export LIBLOG4CPP=/users/davides/mylib/lib

export LD_LIBRARY_PATH=${LIBLOG4CPP}:${VALOR_FMWK}/lib:${ROOTSYS}/lib:${LD_LIBRARY_PATH}:/usr/lib64/
export DYLD_LIBRARY_PATH=${VALOR_FMWK}/lib:${ROOTSYS}/lib:${DYLD_LIBRARY_PATH}
export PATH=${LIBLOG4CPP}:${VALOR_FMWK}/root:${VALOR_FMWK}/production:${ROOTSYS}/bin:${VALOR_FMWK}/lib:${VALOR_FWMK}/bin:${PATH}
