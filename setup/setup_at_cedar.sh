#!/bin/bash

#
# VALOR framework code setup script for cedar
#
# Input argument: top-level directory
# If no argument given, sets up using the build that contains this file
#

module load gsl
module load root/5.34.36

if [ -z "$VALOR_FMWK" ]; then
  source="$( readlink -e "${BASH_SOURCE[0]}" )"
  export VALOR_FMWK="$( dirname "$( dirname "${source}" )" )/" # Directory above the one containing this file
fi


export LD_LIBRARY_PATH="/cvmfs/soft.computecanada.ca/easybuild/software/2017/avx2/Compiler/intel2016.4/gsl/2.2.1/lib:${VALOR_FMWK}/lib:${LD_LIBRARY_PATH}"
export PATH="${VALOR_FMWK}/bin:${PATH}"

# If we've got a python virtualenv, set it up
if [ -e "${VALOR_FMWK}/external/python/valor/bin/activate" ] ; then
  echo "setting up python"	
  export VIRTUAL_ENV_DISABLE_PROMPT=1
  . "${VALOR_FMWK}/external/python/valor/bin/activate"
fi

