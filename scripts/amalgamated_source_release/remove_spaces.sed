#! /usr/bin/sed -nf

s/^ *//
s/ *$//
/^$/d
/^\s*$/d
s/[[:space:]]\{1,\}/ /g
