#!/bin/bash

#
# VALOR_LBNX oscillation analysis code setup script for Costas' desktop
#
# Input argument: oscillation analysis code top-level directory
#

export VALOR_LBNX=$1
export ROOTSYS=/usr/local/hepsoft/root/v5.34.03
#export ROOTSYS=/usr/local/hepsoft/root/v5.28.00a
#export ROOTSYS=/usr/local/hepsoft/root/v5.26.00b

export DYLD_LIBRARY_PATH=${VALOR_LBNX}/lib:${ROOTSYS}/lib:${DYLD_LIBRARY_PATH}
export PATH=${VALOR_LBNX}/root:${VALOR_LBNX}/production:${ROOTSYS}/bin:${PATH}
