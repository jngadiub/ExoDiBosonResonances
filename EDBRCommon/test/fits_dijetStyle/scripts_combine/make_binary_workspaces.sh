#!/bin/bash
UPD=0; 
if [[ "$1" == "-u" ]]; then UPD=1; shift; fi;
if [[ "$1" == "-f" ]]; then UPD=0; shift; fi;

if test -d $1; then PURPOSE=$1; else echo "Usage: $0 purpose mass what"; exit 1; fi; 
cd $PURPOSE; shift;
if test -d $1; then MASS=$1; else echo "Usage: $0 purpose mass what"; exit 1; fi; 
cd $MASS; shift;
if [[ "$1" == "" ]]; then echo "Usage: $0 purpose mass what"; exit 1; fi;

if test -f $1; then
    OUT=$(echo $1 | sed 's/.txt\(.gz\)\?//').root
    if [[ "$UPD" == "2" ]]; then test $OUT -nt $1 && exit 2; fi;
    if [[ "$UPD" == "1" ]]; then test $OUT -nt $1 && exit 0; fi;
    test -f $OUT && rm $OUT 
    text2workspace.py $1 -m $MASS -o $OUT
    if test \! -f  $OUT; then
        echo "Failed to convert $1 at mH = $MASS to binary format"
        exit 1;
    fi;
    echo "Converted $1 at mH = $MASS to binary format $OUT "
else
    echo "Missing datacard $1 at mH = $MASS"
fi;

