#!/bin/bash

#
# VALOR framework code setup script for any grid site.
#
# Input argument: top-level directory
# If no argument given, sets up using the build that contains this file
#

# first check if spack is setup
if [[ $(command -v spack) == "" ]] ; then echo "Loading spack" ; source /cvmfs/larsoft.opensciencegrid.org/spack-packages/setup-env.sh ; fi

# Setup LArSoft. We no longer have a convenient way to setup all dependencies immediately.
# The AL9 `spack load` framework changes some of these variables. Manually check if we've loaded them inside our env

#Yes, you need to manually load gcc to get the g++ compiler.
GCC_VERSION=12.2.0
retExp=$(spack find --loaded | grep gcc@${GCC_VERSION})
retVal=$?
if [[ ${retVal} == 0 ]] ; then export SETUP_GCC="1" ; fi

if [ -z "$SETUP_GCC" ] ; then
    echo "Loading gcc version ${GCC_VERSION}..."
    spack load gcc@${GCC_VERSION}
    export SETUP_GCC="1" ; fi

GSL_VERSION=2.7.1
retExp=$(spack find --loaded | grep gsl@${GSL_VERSION})
retVal=$?
if [[ ${retVal} == 0 ]] ; then export SETUP_GSL="1" ; fi

if [ -z "$SETUP_GSL" ] ; then
    echo "Loading gsl version ${GSL_VERSION}..."
    spack load gsl@${GSL_VERSION}
    export SETUP_GSL="1" ; fi

retExp=$(spack find --loaded | grep log4cpp)
retVal=$?
if [[ ${retVal} == 0 ]] ; then export SETUP_LOG4CPP="1" ; fi

if [ -z "$SETUP_LOG4CPP" ] ; then
    echo "Loading log4cpp..."
    spack load log4cpp
    export SETUP_LOG4CPP="1" ; fi

retExp=$(spack find --loaded | grep lhapdf)
retVal=$?
if [[ ${retVal} == 0 ]] ; then export SETUP_LHAPDF="1" ; fi

LHAPDF_VERSION=6.5.3
if [ -z "$SETUP_LHAPDF" ] ; then
    echo "Loading lhapdf version ${LHAPDF_VERSION}..."
    spack load lhapdf@${LHAPDF_VERSION}
    export SETUP_LHAPDF="1" ; fi

# We need to manually append to the LD_LIBRARY_PATH now as spack does not append paths to LD_LIBRARY_PATH. Do this by querying paths
spack find --loaded -p log4cpp | tail -n 1 > ./tmp.txt
LOG4CPP_DIR=$(awk '{print $2}' tmp.txt) ; rm -f ./tmp.txt
LOG4CPP_LIB=${LOG4CPP_DIR}/lib
export LD_LIBRARY_PATH=${LOG4CPP_LIB}:${LD_LIBRARY_PATH}

spack find --loaded -p gsl | tail -n 1 > ./tmp.txt
GSL_DIR=$(awk '{print $2}' tmp.txt) ; rm -f ./tmp.txt
GSL_LIB=${GSL_DIR}/lib
export LD_LIBRARY_PATH=${GSL_LIB}:${LD_LIBRARY_PATH}

spack find --loaded -p lhapdf | tail -n 1 > ./tmp.txt
LHAPDF_DIR=$(awk '{print $2}' tmp.txt) ; rm -f ./tmp.txt
LHAPDF_LIB=${LHAPDF_DIR}/lib
export LD_LIBRARY_PATH=${LHAPDF_LIB}:${LD_LIBRARY_PATH}

# Now we can get genie

GENIE_VERSION=3.04.00
retExp=$(spack find --loaded | grep genie@${GENIE_VERSION})
retVal=$?
if [[ ${retVal} == 0 ]] ; then export SETUP_GENIE="1" ; fi

if [ -z "$SETUP_GENIE" ] ; then
    echo "Loading genie version ${GENIE_VERSION}..."
    spack load genie@3.04.00%gcc@12.2.0/timnunj
    export SETUP_GENIE="1"
fi

spack find --loaded -p genie | tail -n 1 > ./tmp.txt
GENIE_DIR=$(awk '{print $2}' tmp.txt) ; rm -f ./tmp.txt
GENIE_LIB=${GENIE_DIR}/lib
export LD_LIBRARY_PATH=${GENIE_LIB}:${LD_LIBRARY_PATH}

# You must also explicitly define GENIE_REWEIGHT... it is now packaged with the Generator product
#echo "Here's your GENIE variable... ${GENIE}"
export GENIE_REWEIGHT=${GENIE}

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

