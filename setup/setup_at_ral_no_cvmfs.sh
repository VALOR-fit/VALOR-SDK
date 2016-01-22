#!/bin/bash

#
# VALOR framework code setup script for RAL (no CVMFS)
#
# Input argument: top-level <directory>
#

export VALOR_FMWK="${1}"
if [ -z "$VALOR_FMWK" ]; then
    source="$( readlink -m "${BASH_SOURCE[0]}" )"
      export VALOR_FMWK="$( dirname "$( dirname "${source}" )" )/" # Directory above the one containing this file
fi

export ROOTSYS=/opt/ppd/t2k/softw/ROOT/builds/SL6.x86_64/v5.34.30/

export DYLD_LIBRARY_PATH=${VALOR_FMWK}/lib:${ROOTSYS}/lib:${DYLD_LIBRARY_PATH}
export PATH=${VALOR_FMWK}/root:${ROOTSYS}/bin:${PATH}
