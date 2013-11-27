#! /bin/bash

###the order:
#  ./mergeCombinationTrees.sh
#  ./mergeHarvestedCombinationTrees.sh
# compile and run harvestMCMC.C passing the name of the grand total output file

ONED=1
FMOD=""

if [ $# -gt 0 ]
    then
    ONED=0
    WIDTH=$1    
    FMOD=$WIDTH"."
fi


LISTFILES_EXP=""
LISTFILES_OBS=""
LISTFILES_ASYMPT=""
LISTFILES_EXPSIGNIF=""
LISTFILES_OBSSIGNIF=""
#STEMEXP="higgsCombineEXOZZ.exp.MarkovChainMC."
#STEMOBS="higgsCombineEXOZZ.obs.MarkovChainMC."
STEMASYMPT="higgsCombineEXOZZ.Asymptotic."
STEMEXPSIGNIF="higgsCombineEXOZZExpSignif.ProfileLikelihood."
STEMOBSSIGNIF="higgsCombineEXOZZObsSignif.ProfileLikelihood."

# for file in $( /bin/ls "harvestedTrees/${STEMEXP}"*"${FMOD}"TOTAL.root  )
#   do
#  # echo $file
#   LISTFILES_EXP=${LISTFILES_EXP}" $file "
# done

# for file in $( /bin/ls "harvestedTrees/${STEMOBS}"*"${FMOD}"TOTAL.root  )
#   do
#  # echo $file
#   LISTFILES_OBS=${LISTFILES_OBS}" $file "
# done

for file in $( /bin/ls "harvestedTrees/${STEMASYMPT}"*"${FMOD}"TOTAL.root  )
  do
 # echo $file
  LISTFILES_ASYMPT=${LISTFILES_ASYMPT}" $file "
done

for file in $( /bin/ls "harvestedTrees/${STEMEXPSIGNIF}"*"${FMOD}"TOTAL.root  )
  do
 # echo $file
  LISTFILES_EXPSIGNIF=${LISTFILES_EXPSIGNIF}" $file "
done

for file in $( /bin/ls "harvestedTrees/${STEMOBSSIGNIF}"*"${FMOD}"TOTAL.root  )
  do
 # echo $file
  LISTFILES_OBSSIGNIF=${LISTFILES_OBSSIGNIF}" $file "
done


echo "Merging: $LISTFILES"
#hadd ./${STEMEXP}"SB375.TOTAL.root" $LISTFILES_EXP
#hadd ./${STEMOBS}"SB375.TOTAL.root" $LISTFILES_OBS
hadd ./${STEMASYMPT}${FMOD}"TOTAL.root" $LISTFILES_ASYMPT
hadd ./${STEMEXPSIGNIF}${FMOD}"TOTAL.root" $LISTFILES_EXPSIGNIF
hadd ./${STEMOBSSIGNIF}${FMOD}"TOTAL.root" $LISTFILES_OBSSIGNIF
