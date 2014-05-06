#!/bin/bash

#
# VALOR framework code setup script for RAL/PPD Tier2
#
# Input argument: top-level directory
#

export VALOR_FMWK=$1
export ROOTSYS=/opt/ppd/t2k/softw/ROOT/builds/SL5_64bit/v5.34.03/
export LD_LIBRARY_PATH=${VALOR_FMWK}/lib:${ROOTSYS}/lib:${LD_LIBRARY_PATH}
export PATH=${VALOR_FMWK}/root:${ROOTSYS}/bin:${PATH}

