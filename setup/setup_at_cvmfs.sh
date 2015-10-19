#!/bin/bash

#
# VALOR framework code setup script for any grid site.
#
# Input argument: top-level directory
# If no argument given, sets up using the build that contains this file
#



#~ source "/cvmfs/atlas.cern.ch/repo/sw/atlas-gcc/472/x86_64/setup.sh" # use G++ 472
#~ source "/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/5.34.21-x86_64-slc6-gcc47-opt/bin/thisroot.sh" # and appropriate ROOT

source "/cvmfs/atlas.cern.ch/repo/sw/atlas-gcc/484/x86_64/setup.sh" # use G++ 484
source "/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/5.34.25-x86_64-slc6-gcc48-opt/bin/thisroot.sh" # and appropriate ROOT

# Use T2K root
#~ source "/cvmfs/t2k.egi.eu/nd280v11r31p5/ROOT/v5r34p09n04/Linux-x86_64/bin/thisroot.sh"

export VALOR_FMWK="${1}"
if [ -z "$VALOR_FMWK" ]; then
    source="$( readlink -m "${BASH_SOURCE[0]}" )"
      export VALOR_FMWK="$( dirname "$( dirname "${source}" )" )/" # Directory above the one containing this file
fi

#export CVMFS_LIBRARIES="/cvmfs/t2k.egi.eu/lib-Linux2.6.32-431.23.3.el6.x86_64/"

# Use NOvA build of log4cpp since it matches the compiler above
export CVMFS_LIBRARIES="/cvmfs/nova.opensciencegrid.org/externals/log4cpp/v1_1_1a/Linux64bit+2.6-2.5-e5-prof/lib/"

export LD_LIBRARY_PATH="${VALOR_FMWK}/lib:${ROOTSYS}/lib:${LD_LIBRARY_PATH}:${CVMFS_LIBRARIES}"
export PATH=${VALOR_FMWK}/bin:${ROOTSYS}/bin:${PATH}

