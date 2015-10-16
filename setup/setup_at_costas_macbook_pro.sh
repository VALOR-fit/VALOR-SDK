#!/bin/bash

#
# VALOR framework code setup script for Costas' MacBookPro2
#
# Input argument: top-level directory
#

export VALOR_FMWK=$1
export ROOTSYS=/hep/soft/root/v5.34.34/

export DYLD_LIBRARY_PATH=${VALOR_FMWK}/lib:${ROOTSYS}/lib:${DYLD_LIBRARY_PATH}
export PATH=${VALOR_FMWK}/root:${ROOTSYS}/bin:${PATH}
