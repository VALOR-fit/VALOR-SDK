#!/bin/bash

#
# VALOR framework code setup script for Costas' MacBookPro2
#
# Optional input argument: top-level directory
#

if [ -z "$VALOR_FMWK" ]; then
    source="$( greadlink -m "${BASH_SOURCE[0]}" )"
      export VALOR_FMWK="$( dirname "$( dirname "${source}" )" )/" # Directory above the one containing this file
fi

export ROOTSYS=/hep/soft/root/v5.34.34/

export DYLD_LIBRARY_PATH=${VALOR_FMWK}/lib:${ROOTSYS}/lib:${DYLD_LIBRARY_PATH}
export PATH=${VALOR_FMWK}/root:${ROOTSYS}/bin:${PATH}
