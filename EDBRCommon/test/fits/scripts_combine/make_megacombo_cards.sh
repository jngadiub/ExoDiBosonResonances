#!/bin/bash
if [[ "$1" == "" ]]; then echo "Usage: $0 mass"; exit 1; fi;
if test -d $1; then MASS=$1; else echo "Usage: $0 mass"; exit 1; fi; 
cd $MASS
WHAT=$2

COMB=""

label="xvv"


#combineCards.py -S =comb_xzz.txt =comb_xww.txt  > comb_xvv.txt
combineCards.py -S  Xvv.mX${MASS}.0_ZZ_8TeV_channel0.txt =comb_xzz.txt =comb_xww.txt   > comb_xvv.txt


echo "Done cards for mass $MASS"
