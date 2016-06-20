#! /bin/sh

# Script to generate the libValor.a archive library.
# Adapted by Costas Andreopoulos from $ROOTSYS/build/unix/makestaticlib.sh
# Original Author: Fons Rademakers, 21/01/2001

VALORALIB=$VALOR_FMWK/lib/libValor.a
rm -f $VALORALIB

#STATICOBJECTLIST=$1
STATICOBJECTLIST=$@
#objs=`$STATICOBJECTLIST`
objs=$STATICOBJECTLIST
echo $objs

echo "Making $VALORALIB..."
echo ar rv $VALORALIB $objs
ar rv $VALORALIB $objs > /dev/null 2>&1

arstat=$?
if [ $arstat -ne 0 ]; then
   exit $arstat
fi

exit 0
