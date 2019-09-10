#!/bin/bash

#
# VALOR framework code setup script for any grid site.
#
# Input argument: top-level directory
# If no argument given, sets up using the build that contains this file
#


# Setup LArSoft. This gives us modern g++, ROOT and GENIE
# The "&&" here prevents "set -e" killing these scripts, which apparently have bad returns inside them.
if [ -z "$SETUP_GENIE" ] ; then
  . /cvmfs/fermilab.opensciencegrid.org/products/larsoft/setup && retval="$?"
  setup genie v3_00_04_rc -q e17:prof
fi

if [ -z "$VALOR_FMWK" ]; then
  source="$( readlink -e "${BASH_SOURCE[0]}" )"
  export VALOR_FMWK="$( dirname "$( dirname "${source}" )" )/" # Directory above the one containing this file
fi


export LD_LIBRARY_PATH="${VALOR_FMWK}/lib:${LD_LIBRARY_PATH}"
export PATH="${VALOR_FMWK}/bin:${PATH}"

# If we've got a python virtualenv, set it up
if [ -e "${VALOR_FMWK}/external/python/valor/bin/activate" ] ; then
  export VIRTUAL_ENV_DISABLE_PROMPT=1
  . "${VALOR_FMWK}/external/python/valor/bin/activate"
fi

