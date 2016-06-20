#! /bin/sh

#
# Script to generate the list of object files to be included in the static VALOR 
# library or that are needed to link to static VALOR applications.
#
# Adapted by Costas Andreopoulos from $ROOTSYS/build/unix/staticobjectlist.sh 
# Original author: Fons Rademakers, 13/12/2010
#

#in case called with argument -d then return only the dictionary object files (_RootDict_*.o) (G__*.o).
if [ "$1" = "-d" ]; then
   dictonly="yes"
fi

excl=""

objs=""
gobjs=""
for i in $VALOR_FMWK/src/valor/* ; do
   inc=$i
#  echo $inc;
   for j in $excl ; do
      if [ $j = $i ]; then
         continue 2
      fi
   done
   ls $inc/*.o > /dev/null 2>&1 && objs="$objs `ls $inc/*.o`"
   ls $inc/_RootDict_*.o > /dev/null 2>&1 && gobjs="$gobjs `ls $inc/_RootDict_*.o`"
   if [ -d $i ]; then
      for k in $i/* ; do
         inc=$k
         for j in $excl ; do
            if [ $j = $k ]; then
               continue 2
            fi
         done
         ls $inc/*.o > /dev/null 2>&1 && objs="$objs `ls $inc/*.o`"
         ls $inc/_RootDict_*.o > /dev/null 2>&1 && gobjs="$gobjs `ls $inc/_RootDict_*.o`"
      done
   fi
done

if [ "x$dictonly" = "xyes" ]; then
   echo $gobjs
else
   echo $objs
fi

exit 0
