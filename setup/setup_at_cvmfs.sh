#!/bin/bash

#
# VALOR framework code setup script for any grid site.
#
# Input argument: top-level directory
# If no argument given, sets up using the build that contains this file
#


# Setup LArSoft. This gives us g++4.9, ROOT and GENIE
# The "&&" here prevents "set -e" killing these scripts, which apparently have bad returns inside them.
if [[ ! $PRODUCTS == *"larsoft"* ]] ; then
  . /cvmfs/fermilab.opensciencegrid.org/products/larsoft/setup && retval="$?" 
fi
if [ -z "$SETUP_LARSOFT" ] ; then
  setup genie v2_10_6 -q e9:prof:r5 && retval="$?"
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

