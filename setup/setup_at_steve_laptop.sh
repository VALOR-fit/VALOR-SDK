#!/bin/bash

#
# T2KANA oscillation analysis code setup script for Steve Dennis' laptop
#
# Input argument: oscillation analysis code top-level directory
# If no argument given, just use the directory containing the script
#

export VALOR_FMWK=$1

if [ -z $VALOR_FMWK ]; then
  export VALOR_FMWK="$( dirname `cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd `)/" # Directory above the one containing this file
fi

source /home/steve/packages/root/root-v5-34/bin/thisroot.sh

export LD_LIBRARY_PATH=${VALOR_FMWK}/lib:${LD_LIBRARY_PATH}
export PATH=${VALOR_FMWK}/root:${VALOR_FMWK}/production:${PATH}

alias root2k="root2k -l"

export T2KCOMPILE="-Wextra -Wshadow"
