#!/bin/bash

if [[ "$1" == "" ]]; then echo "Usage: $0 mass"; exit 1; fi;
if test -d $1; then MASS=$1; else echo "Usage: $0 mass"; exit 1; fi; 
cd $MASS
WHAT=$2

#if expr index $MASS .5 > /dev/null; then MASSD="$MASS"; else MASSD="$MASS.0"; fi
if echo $M | grep -F -q .5; then MASSD="$MASS"; else MASSD="$MASS.0"; fi

#just rename the existing cards
EXOZZ1JELELP="xzz_ee1JLP=Xvv.mX${MASS}_WW_8TeV_channel0.txt"
EXOZZ1JMULP="xzz_mm1JLP=Xvv.mX${MASS}_WW_8TeV_channel2.txt"
EXOZZ1JELEHP="xzz_ee1JHP=Xvv.mX${MASS}_WW_8TeV_channel1.txt"
EXOZZ1JMUHP="xzz_mm1JHP=Xvv.mX${MASS}_WW_8TeV_channel3.txt"

combineCards.py -S $EXOZZ1JELELP  > comb_xzz_2l1JELELP.txt
combineCards.py -S $EXOZZ1JMULP  > comb_xzz_2l1JMULP.txt
combineCards.py -S $EXOZZ1JELEHP  > comb_xzz_2l1JELEHP.txt
combineCards.py -S $EXOZZ1JMUHP  > comb_xzz_2l1JMUHP.txt

#actual combination
EXOZZ1JLP="xzz_ee1JLP=Xvv.mX${MASS}_WW_8TeV_channel0.txt  xzz_mm1JLP=Xvv.mX${MASS}_WW_8TeV_channel2.txt"
EXOZZ1JHP="xzz_ee1JHP=Xvv.mX${MASS}_WW_8TeV_channel1.txt xzz_mm1JHP=Xvv.mX${MASS}_WW_8TeV_channel3.txt"
EXOZZ1JELE="xzz_ee1JLP=Xvv.mX${MASS}_WW_8TeV_channel0.txt xzz_ee1JHP=Xvv.mX${MASS}_WW_8TeV_channel1.txt"
EXOZZ1JMU="xzz_mm1JLP=Xvv.mX${MASS}_WW_8TeV_channel2.txt xzz_mm1JHP=Xvv.mX${MASS}_WW_8TeV_channel3.txt"
EXOZZ1J="$EXOZZ1JLP $EXOZZ1JHP"
EXOZZ="$EXOZZ1JLP $EXOZZ1JHP"

combineCards.py -S $EXOZZ1JLP  > comb_xzz_2l1JLP.txt
combineCards.py -S $EXOZZ1JHP  > comb_xzz_2l1JHP.txt
combineCards.py -S $EXOZZ1JELE > comb_xzz_ee1J.txt
combineCards.py -S $EXOZZ1JMU  > comb_xzz_mm1J.txt
combineCards.py -S $EXOZZ1J    > comb_xzz.txt

echo "Done cards for mass $MASS"
  
