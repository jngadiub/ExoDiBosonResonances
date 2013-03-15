#! /bin/bash

###the order:
#  ./mergeCombinationTrees.sh <MASS>
#  ./mergeHarvestedCombinationTrees.sh
# compile and run harvestMCMC.C passing the name of the grand total output file

MASS=0

if [ $# -gt 0 ]
then
    MASS=$1
else
    echo "Required one input, the mass point"
    exit 1
fi

DIR="output_EXOZZ_Asymptotic_comb_xzz_2l1J"
LISTFILES_EXP=""
LISTFILES_OBS=""
LISTFILES_ASYMPT=""
LISTFILES_EXPSIGNIF=""
LISTFILES_OBSSIGNIF=""
STEMEXP="higgsCombineEXOZZ.exp.MarkovChainMC.mH${MASS}."
STEMOBS="higgsCombineEXOZZ.obs.MarkovChainMC.mH${MASS}."
STEMASYMPT="higgsCombineEXOZZ.Asymptotic.mH${MASS}."
STEMEXPSIGNIF="higgsCombineEXOZZExpSignif.ProfileLikelihood.mH${MASS}."
STEMOBSSIGNIF="higgsCombineEXOZZObsSignif.ProfileLikelihood.mH${MASS}."

for file in $( /bin/ls "${MASS}/${DIR}/${STEMEXP}"[0-9]*root  )
  do
 # echo $file
  LISTFILES_EXP=${LISTFILES_EXP}" $file "
done

for file in $( /bin/ls "${MASS}/${DIR}/${STEMOBS}"[0-9]*root  )
  do
 # echo $file
  LISTFILES_OBS=${LISTFILES_OBS}" $file "
done

for file in $( /bin/ls "${MASS}/${DIR}/${STEMASYMPT}"[0-9]*root  )
  do
 # echo $file
  LISTFILES_ASYMPT=${LISTFILES_ASYMPT}" $file "
done

for file in $( /bin/ls "${MASS}/${DIR}/${STEMEXPSIGNIF}"[0-9]*root  )
  do
 # echo $file
  LISTFILES_EXPSIGNIF=${LISTFILES_EXPSIGNIF}" $file "
done


for file in $( /bin/ls "${MASS}/${DIR}/${STEMOBSSIGNIF}"root  )
  do
 # echo $file
  LISTFILES_OBSSIGNIF=${LISTFILES_OBSSIGNIF}" $file "
done




echo "Merging: $LISTFILES"
#hadd ${MASS}/$DIR/${STEMEXP}"TOTAL.root" $LISTFILES_EXP
#hadd ${MASS}/$DIR/${STEMOBS}"TOTAL.root" $LISTFILES_OBS
hadd -f ${MASS}/$DIR/${STEMASYMPT}"TOTAL.root" $LISTFILES_ASYMPT
hadd -f ${MASS}/$DIR/${STEMEXPSIGNIF}"TOTAL.root" $LISTFILES_EXPSIGNIF
hadd -f ${MASS}/$DIR/${STEMOBSSIGNIF}"TOTAL.root" $LISTFILES_OBSSIGNIF

mkdir harvestedTrees/
cp ${MASS}/$DIR/*"TOTAL.root" harvestedTrees/

