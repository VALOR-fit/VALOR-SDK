#!/bin/bash

#
# VALOR framework code setup script for RAL/PPD Tier2
#
# Input argument: top-level directory
#


source "/opt/ppd/atlas/gcc/4.7.2/x86_64-slc5-gcc47-opt/setup.sh"
source "/opt/ppd/atlas/root/5.34.23-p3/x86_64-sl6-gcc47/setup.sh"

export VALOR_FMWK="${1}"
if [ -z "$VALOR_FMWK" ]; then
    source="$( readlink -m "${BASH_SOURCE[0]}" )"
      export VALOR_FMWK="$( dirname "$( dirname "${source}" )" )/" # Directory above the one containing this file
fi

#export ROOTSYS=/opt/ppd/t2k/softw/ROOT/builds/SL5_64bit/v5.34.03/
export LD_LIBRARY_PATH=${VALOR_FMWK}/lib:${ROOTSYS}/lib:${LD_LIBRARY_PATH}:/opt/ppd/t2k/softw/LOG4CPP/builds/SL5_64bit/v1.0/src/.libs/
export PATH=${VALOR_FMWK}/root:${ROOTSYS}/bin:${PATH}

