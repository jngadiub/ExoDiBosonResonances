#!/bin/bash
if [[ "$1" == "" ]]; then echo "Usage: $0 mass"; exit 1; fi;
if test -d $1; then MASS=$1; else echo "Usage: $0 mass"; exit 1; fi; 
cd $MASS
WHAT=$2

COMB=""

#if expr index $MASS .5 > /dev/null; then MASSD="$MASS"; else MASSD="$MASS.0"; fi
if echo $M | grep -F -q .5; then MASSD="$MASS"; else MASSD="$MASS.0"; fi

EXOZZ="xzz_ee1J=xzz_ee1J.${MASS}.txt xzz_mm1J=xzz_mm1J.${MASS}.txt"


##HZZ4L="hzz4l_2e2mu=hzz4l_2e2muS.${MASSD}.txt hzz4l_4e=hzz4l_4eS.${MASSD}.txt hzz4l_4mu=hzz4l_4muS.${MASSD}.txt"
##HZZ2Q="hzz2l2q_ee0b=hzz2l2q_ee0b.${MASS}.txt  hzz2l2q_ee1b=hzz2l2q_ee1b.${MASS}.txt  hzz2l2q_ee2b=hzz2l2q_ee2b.${MASS}.txt "
##HZZ2Q="hzz2l2q_mm0b=hzz2l2q_mm0b.${MASS}.txt  hzz2l2q_mm1b=hzz2l2q_mm1b.${MASS}.txt  hzz2l2q_mm2b=hzz2l2q_mm2b.${MASS}.txt $HZZ2Q"
##HZZ2N="hzz2l2nu_ee=hzz2l2nu_ee_${MASS}.txt  hzz2l2nu_mm=hzz2l2nu_mm_${MASS}.txt"


## ZZ 2Q
#if (( $MASS >= 100)); then 
COMB="$COMB $EXOZZ";
combineCards.py -S $EXOZZ > comb_xzz_2l1J.txt
#fi;

combineCards.py -S $COMB > comb.txt

echo "Done cards for mass $MASS"
