#! /bin/sh

# Script to generate the libValor.a archive library.
# Adapted by Costas Andreopoulos from $ROOTSYS/build/unix/makestaticlib.sh
# Original Author: Fons Rademakers, 21/01/2001

version=$(awk 'NR==1' $VALOR_FMWK/VERSION)
echo "VALOR version: $version"

VALORALIB=$VALOR_FMWK/lib/libValor-$version.a
VALORALIB_SYMLINK=$VALOR_FMWK/lib/libValor.a

rm -f $VALORALIB
rm -f $VALORALIB_SYMLINK

STATICOBJECTLIST=$@
objs=$STATICOBJECTLIST
echo $objs

echo "Building $VALORALIB..."
echo ar rv $VALORALIB $objs
ar rv $VALORALIB $objs > /dev/null 2>&1

echo "Adding $VALORALIB_SYMLINK symbolic link..."
ln -sf $VALORALIB $VALORALIB_SYMLINK

arstat=$?
if [ $arstat -ne 0 ]; then
   exit $arstat
fi



exit 0
