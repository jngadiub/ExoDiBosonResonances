#!/bin/bash
if [[ "$1" == "" ]]; then echo "Usage: $0 mass"; exit 1; fi;
if test -d $1; then MASS=$1; else echo "Usage: $0 mass"; exit 1; fi; 
cd $MASS
WHAT=$2

COMB=""

label="xzz"

#if expr index $MASS .5 > /dev/null; then MASSD="$MASS"; else MASSD="$MASS.0"; fi
if echo $M | grep -F -q .5; then MASSD="$MASS"; else MASSD="$MASS.0"; fi

EXOZZ1JLP="${label}_ee1JLP=${label}_ee1JLP.${MASS}.txt ${label}_mm1JLP=${label}_mm1JLP.${MASS}.txt"
EXOZZ1JHP="${label}_ee1JHP=${label}_ee1JHP.${MASS}.txt ${label}_mm1JHP=${label}_mm1JHP.${MASS}.txt"
EXOZZ2J="${label}_ee2J=${label}_ee2J.${MASS}.txt ${label}_mm2J=${label}_mm2J.${MASS}.txt"
EXOZZ1JELE="${label}_ee1JLP=${label}_ee1JLP.${MASS}.txt ${label}_ee1JHP=${label}_ee1JHP.${MASS}.txt"
EXOZZ1JMU="${label}_mm1JLP=${label}_mm1JLP.${MASS}.txt ${label}_mm1JHP=${label}_mm1JHP.${MASS}.txt"
EXOZZ1J="$EXOZZ1JLP $EXOZZ1JHP "
EXOZZ="$EXOZZ1JLP $EXOZZ1JHP $EXOZZ2J "

## ZZ 2Q
#if (( $MASS >= 100)); then 
COMB="$COMB $EXOZZ";
combineCards.py -S $EXOZZ1JLP > comb_${label}_2l1JLP.txt
combineCards.py -S $EXOZZ1JHP > comb_${label}_2l1JHP.txt
combineCards.py -S $EXOZZ1JELE > comb_${label}_ee1J.txt
combineCards.py -S $EXOZZ1JMU > comb_${label}_mm1J.txt
if (( $MASS < 800)); then 
    combineCards.py -S $EXOZZ2J   > comb_${label}_2l2J.txt
    combineCards.py -S $EXOZZ1J   > comb_${label}_2l1J.txt
#    combineCards.py -S $EXOZZ     > comb_${label}.txt
    combineCards.py -S $EXOZZ1J   > comb_${label}.txt
else
    combineCards.py -S $EXOZZ1J   > comb_${label}.txt
fi;
###combineCards.py -S $COMB > comb.txt

echo "Done cards for mass $MASS"
