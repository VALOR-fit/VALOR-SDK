#! /bin/sh

#
# Script to generate the list of object files to be included in the static VALOR 
# library or that are needed to link to static VALOR applications.
#
# Adapted by Costas Andreopoulos from $ROOTSYS/build/unix/staticobjectlist.sh 
# Original author: Fons Rademakers, 13/12/2010
#

for arg in "$@"
do
case $arg in
        # Optional -d or --dictionary-only argument
        # in case called with this argument then return only the dictionary object files (_RootDict_*.o).
        -d|--dictionary-only)
        dictonly="yes"
        shift	
        ;;
        # Mandatory -s=/path or --source-path=/path argument
        # Script will search for .o files within the input path (including sub-directories)
        -s=*|--source-path=*)
        path="${arg#*=}"
	shift
	;;
        # Uknown option
	*)
	;;
esac
done

# Exclude (Empty for now, but plugged-in in the code below. Could be promoted to command-line argument)
excl=""

objs=""
gobjs=""
#for i in $VALOR_FMWK/src/valor/* ; do
for i in $path/* ; do
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
