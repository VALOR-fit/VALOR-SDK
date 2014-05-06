#!/bin/bash

#
# VALOR framework code setup script for Costas' desktop
#
# Input argument: top-level directory
#

export VALOR_FMWK=$1
export ROOTSYS=/usr/local/hepsoft/root/v5.34.03
#export ROOTSYS=/usr/local/hepsoft/root/v5.28.00a
#export ROOTSYS=/usr/local/hepsoft/root/v5.26.00b

export DYLD_LIBRARY_PATH=${VALOR_FMWK}/lib:${ROOTSYS}/lib:${DYLD_LIBRARY_PATH}
export PATH=${VALOR_FMWK}/root:${ROOTSYS}/bin:${PATH}
