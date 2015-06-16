#!/bin/bash

#
# VALOR framework code setup script for RAL/PPD Tier2
#
# Input argument: top-level directory
#

export VALOR_FMWK=$1
export ROOTSYS=/opt/ppd/t2k/softw/ROOT/builds/SL6.x86_64/v5.34.30/
export LOG4CPP=/opt/ppd/t2k/softw/LOG4CPP/builds/SL6.x86_64/v1.1/
export LD_LIBRARY_PATH=${VALOR_FMWK}/lib:${ROOTSYS}/lib:${LD_LIBRARY_PATH}:${LOG4CPP}/src/.libs/
export PATH=${VALOR_FMWK}/root:${ROOTSYS}/bin:${PATH}

