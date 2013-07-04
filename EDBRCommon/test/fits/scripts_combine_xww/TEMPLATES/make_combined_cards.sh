#!/bin/bash
if [[ "$1" == "" ]]; then echo "Usage: $0 mass"; exit 1; fi;
if test -d $1; then MASS=$1; else echo "Usage: $0 mass"; exit 1; fi; 
cd $MASS
WHAT=$2

COMB=""

#if expr index $MASS .5 > /dev/null; then MASSD="$MASS"; else MASSD="$MASS.0"; fi
if echo $M | grep -F -q .5; then MASSD="$MASS"; else MASSD="$MASS.0"; fi

EXOZZ1JLP="xww_ee1JLP=xww_ee1JLP.${MASS}.txt xww_mm1JLP=xww_mm1JLP.${MASS}.txt"
EXOZZ1JHP="xww_ee1JHP=xww_ee1JHP.${MASS}.txt xww_mm1JHP=xww_mm1JHP.${MASS}.txt"
EXOZZ2J="xww_ee2J=xww_ee2J.${MASS}.txt xww_mm2J=xww_mm2J.${MASS}.txt"
EXOZZ1JELE="xww_ee1JLP=xww_ee1JLP.${MASS}.txt xww_ee1JHP=xww_ee1JHP.${MASS}.txt"
EXOZZ1JMU="xww_mm1JLP=xww_mm1JLP.${MASS}.txt xww_mm1JHP=xww_mm1JHP.${MASS}.txt"
EXOZZ1J="$EXOZZ1JLP $EXOZZ1JHP "
EXOZZ="$EXOZZ1JLP $EXOZZ1JHP $EXOZZ2J "

## ZZ 2Q
#if (( $MASS >= 100)); then 
COMB="$COMB $EXOZZ";
combineCards.py -S $EXOZZ1JLP > comb_xww_2l1JLP.txt
combineCards.py -S $EXOZZ1JHP > comb_xww_2l1JHP.txt
combineCards.py -S $EXOZZ1JELE > comb_xww_ee1J.txt
combineCards.py -S $EXOZZ1JMU > comb_xww_mm1J.txt
if (( $MASS < 800)); then 
#    combineCards.py -S $EXOZZ2J   > comb_xww_2l2J.txt
    combineCards.py -S $EXOZZ1J   > comb_xww_2l1J.txt
#    combineCards.py -S $EXOZZ     > comb_xww.txt
    combineCards.py -S $EXOZZ1J   > comb_xww.txt
else
    combineCards.py -S $EXOZZ1J   > comb_xww.txt
fi;
###combineCards.py -S $COMB > comb.txt

echo "Done cards for mass $MASS"
