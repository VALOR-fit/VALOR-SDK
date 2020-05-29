#!/bin/bash

#
# T2KANA oscillation analysis code setup script for Steve Dennis' laptop
#
# Input argument: oscillation analysis code top-level directory
# If no argument given, just use the directory containing the script
#

if [ ! -z ${1} ] ; then
  export VALOR_FMWK="$(readlink -m ${1})"
fi

if [ -z ${VALOR_FMWK} ]; then
  export VALOR_FMWK="$( dirname `cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd `)/" # Directory above the one containing this file
  export VALOR_FMWK="$(readlink -m ${VALOR_FMWK} )"
fi

#~ if [ -z $LOG4CPP_LIB ]; then
  #~ export LOG4CPP_LIB="/home/steve/packages/log4cpp/build"
#~ fi

if [ -z $ROOTSYS ] ; then 
  # Setup GENIE
  . /mnt/l/steve/genie/Reweight/setup.sh
fi

export LD_LIBRARY_PATH="${VALOR_FMWK}/lib:${LOG4CPP_LIB}:${LD_LIBRARY_PATH}"
export PATH="${VALOR_FMWK}/bin:${VALOR_FMWK}/production:${PATH}"

alias root2k="root2k -l"


export OPT_WITH_CXX_USERDEF_FLAGS="-Wno-deprecated"
