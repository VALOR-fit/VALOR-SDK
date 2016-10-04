#!/bin/bash

#
# VALOR framework code setup script for Liverpool
#
# Input argument: top-level directory
#

export VALOR_FMWK=$1
# Will implement in a future version of Valor
# source /cvmfs/atlas.cern.ch/repo/sw/atlas-gcc/472/x86_64/setup.sh # used to setup gcc 4.7 for c++11 flag
export ROOTSYS=/user/cbarry/software/root_v5.34.23
export LOG4CPP=/user/cbarry/software/log4cpp-install
export LD_LIBRARY_PATH=${VALOR_FMWK}/lib:${ROOTSYS}/lib:${LD_LIBRARY_PATH}:${LOG4CPP}/src/.libs/
export PATH=${VALOR_FMWK}/root:${ROOTSYS}/bin:${PATH} 
