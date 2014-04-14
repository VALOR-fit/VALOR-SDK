#!/bin/bash

#
# VALOR_LBNX oscillation analysis code setup script for Oxford batch farm
#
# Input argument: oscillation analysis code top-level directory
#

export VALOR_LBNX=$1

source /data/t2k/dealtry/software/ROOT_53403/root/bin/thisroot.sh
#source /data/t2k/dealtry/software/v10r11p9/ROOT/v5r30p02n01/amd64_linux26/bin/thisroot.sh
#source /system/SL5/cern/root/x86_64/OxfordSetup-5.32.01.sh

export LD_LIBRARY_PATH=${VALOR_LBNX}/lib:${LD_LIBRARY_PATH}
export PATH=${VALOR_LBNX}/root:${VALOR_LBNX}/production:${PATH}


