#!/usr/bin/bash

./configure --prefix=$(pwd)/bin \
    --enable-fault-on-die \
    --enable-genie \
    --with-log4cpp-inc=${LOG4CPP_INC} \
    --with-log4cpp-lib=${LOG4CPP_LIB} \
    --with-gsl-inc=${GSL_INC} \
    --with-gsl-lib=${GSL_LIB} \
    --with-lhapdf-inc=${LHAPDF_INC} \
    --with-lhapdf-lib=${LHAPDF_LIB}
