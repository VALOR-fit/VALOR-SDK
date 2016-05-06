#!/bin/bash

#
# VALOR framework code setup script for any grid site.
#
# Input argument: top-level directory
# If no argument given, sets up using the build that contains this file
#


# Setup LArSoft. This gives us g++4.9, ROOT and GENIE
. /cvmfs/fermilab.opensciencegrid.org/products/larsoft/setup
setup "larsoft" "v05_10_00" -q "e9:prof"

if [ -z "$VALOR_FMWK" ]; then
    source="$( readlink -m "${BASH_SOURCE[0]}" )"
      export VALOR_FMWK="$( dirname "$( dirname "${source}" )" )/" # Directory above the one containing this file
fi


export LD_LIBRARY_PATH="${VALOR_FMWK}/lib:${LD_LIBRARY_PATH}"
export PATH=${VALOR_FMWK}/bin:${ROOTSYS}/bin:${PATH}

# To resolve bugs with Python - investigate why this helps
unset PYTHONHOME

