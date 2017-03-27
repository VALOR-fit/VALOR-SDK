#!/bin/bash

#
# VALOR framework code setup script for Chris' MacBookPro
#
# Input argument: fmwk top-level directory
#

export VALOR_FMWK=$1
export ROOTSYS=/Users/rajs/work/software/root

echo "VALOR FMWK set to:"
echo "$VALOR_FMWK"
export DYLD_LIBRARY_PATH=${VALOR_FMWK}/lib:${ROOTSYS}/lib:${DYLD_LIBRARY_PATH}
export PATH=${VALOR_FMWK}/root:${ROOTSYS}/bin:${PATH}
