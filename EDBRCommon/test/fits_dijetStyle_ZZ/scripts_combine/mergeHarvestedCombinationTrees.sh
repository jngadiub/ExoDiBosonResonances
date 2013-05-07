#! /bin/bash

###the order:
#  ./mergeCombinationTrees.sh
#  ./mergeHarvestedCombinationTrees.sh
# compile and run harvestMCMC.C passing the name of the grand total output file

#-------------------------------------------------------------
# EDIT HERE

LISTFILES_EXP=""
LISTFILES_OBS=""
LISTFILES_ASYMPT=""
LISTFILES_EXPSIGNIF=""
LISTFILES_OBSSIGNIF=""
STEMEXP="higgsCombineEXOZZ.exp.MarkovChainMC."
STEMOBS="higgsCombineEXOZZ.obs.MarkovChainMC."
STEMASYMPT="higgsCombineEXOZZ.Asymptotic."
STEMEXPSIGNIF="higgsCombineEXOZZExpSignif.ProfileLikelihood."
STEMOBSSIGNIF="higgsCombineEXOZZObsSignif.ProfileLikelihood."

#-------------------------------------------------------------

for file in $( /bin/ls "harvestedTrees/${STEMEXP}"*.TOTAL.root  )
  do
 # echo $file
  LISTFILES_EXP=${LISTFILES_EXP}" $file "
done

for file in $( /bin/ls "harvestedTrees/${STEMOBS}"*.TOTAL.root  )
  do
 # echo $file
  LISTFILES_OBS=${LISTFILES_OBS}" $file "
done

for file in $( /bin/ls "harvestedTrees/${STEMASYMPT}"*.TOTAL.root  )
  do
 # echo $file
  LISTFILES_ASYMPT=${LISTFILES_ASYMPT}" $file "
done

for file in $( /bin/ls "harvestedTrees/${STEMEXPSIGNIF}"*.TOTAL.root  )
  do
 # echo $file
  LISTFILES_EXPSIGNIF=${LISTFILES_EXPSIGNIF}" $file "
done

for file in $( /bin/ls "harvestedTrees/${STEMOBSSIGNIF}"*.TOTAL.root  )
  do
 # echo $file
  LISTFILES_OBSSIGNIF=${LISTFILES_OBSSIGNIF}" $file "
done

echo "Merging: $LISTFILES"
#hadd -f ./${STEMEXP}"SB375.TOTAL.root" $LISTFILES_EXP
#hadd -f ./${STEMOBS}"SB375.TOTAL.root" $LISTFILES_OBS
hadd -f ./${STEMASYMPT}"TOTAL.root" $LISTFILES_ASYMPT
hadd -f ./${STEMEXPSIGNIF}"TOTAL.root" $LISTFILES_EXPSIGNIF
hadd -f ./${STEMOBSSIGNIF}"TOTAL.root" $LISTFILES_OBSSIGNIF
