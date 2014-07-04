#!/bin/bash

#
# VALOR framework code setup script for Costas' MacBookPro
#
# Input argument: top-level directory
#

export VALOR_FMWK=$1
export ROOTSYS=/hep/root/v5.32.01/

export DYLD_LIBRARY_PATH=${VALOR_FMWK}/lib:${ROOTSYS}/lib:${DYLD_LIBRARY_PATH}
export PATH=${VALOR_FMWK}/root:${ROOTSYS}/bin:${PATH}
