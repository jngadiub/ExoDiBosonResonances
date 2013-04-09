#!/bin/bash
if [[ "$1" == "" ]]; then echo "Usage: $0 mass"; exit 1; fi;
if test -d $1; then MASS=$1; else echo "Usage: $0 mass"; exit 1; fi; 
cd $MASS
WHAT=$2

COMB=""

#if expr index $MASS .5 > /dev/null; then MASSD="$MASS"; else MASSD="$MASS.0"; fi
if echo $M | grep -F -q .5; then MASSD="$MASS"; else MASSD="$MASS.0"; fi

EXOZZ1JLP="xzz_ee1JLP=xzz_ee1JLP.${MASS}.txt xzz_mm1JLP=xzz_mm1JLP.${MASS}.txt"
EXOZZ1JHP="xzz_ee1JHP=xzz_ee1JHP.${MASS}.txt xzz_mm1JHP=xzz_mm1JHP.${MASS}.txt"
EXOZZ2J="xzz_ee2J=xzz_ee2J.${MASS}.txt xzz_mm2J=xzz_mm2J.${MASS}.txt"
EXOZZ1J="$EXOZZ1JLP $EXOZZ1JHP "
EXOZZ="$EXOZZ1JLP $EXOZZ1JHP $EXOZZ2J "

## ZZ 2Q
#if (( $MASS >= 100)); then 
COMB="$COMB $EXOZZ";
combineCards.py -S $EXOZZ1JLP > comb_xzz_2l1JLP.txt
combineCards.py -S $EXOZZ1JHP > comb_xzz_2l1JHP.txt
if (( $MASS < 800)); then 
    combineCards.py -S $EXOZZ2J   > comb_xzz_2l2J.txt
    combineCards.py -S $EXOZZ     > comb_xzz.txt
else
    combineCards.py -S $EXOZZ1J   > comb_xzz.txt
fi;
###combineCards.py -S $COMB > comb.txt

echo "Done cards for mass $MASS"
