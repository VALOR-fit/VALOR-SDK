#!/bin/bash

#
# VALOR framework code setup script for Toms' MacBookPro
#
# Input argument: top-level directory
#

export VALOR_FMWK=$1
export ROOTSYS=/Users/thomasdealtry/T2K/software/ROOT/root-v5-34-00/

export DYLD_LIBRARY_PATH=${VALOR_FMWK}/lib:${ROOTSYS}/lib:${DYLD_LIBRARY_PATH}
export PATH=${VALOR_FMWK}/root:${ROOTSYS}/bin:${PATH}
