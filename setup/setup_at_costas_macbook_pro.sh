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

#source /hep/soft/root/v6.08.00/build/bin/thisroot.sh
source /hep/soft/root/v5.34.34/bin/thisroot.sh

export DYLD_LIBRARY_PATH=${VALOR_FMWK}/lib:${DYLD_LIBRARY_PATH}
export PATH=${VALOR_FMWK}/root:${PATH}
