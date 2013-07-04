#! /bin/bash
CARDDIR=DataCards_XWW_V24


cp TEMPLATES/* $CARDDIR

rm -rf AllKindsOfCards
mkdir AllKindsOfCards

#datacard=TEMPLATE_CARDCAT  ##"comb_xww", "xww_mm1JLP.\$mass", "xww_mm1JHP.\$mass", "xww_ee1JLP.\$mass", "xww_ee1JHP.\$mass", "comb_xww_ee1J" , "comb_xww_mm1J"
#DIR=RESULTDIR_TEMPLATE #"output_EXOZZ_Asymptotic_comb_xww", "output_EXOZZ_Asymptotic_xww_ee1JHP.\$MASS", "output_EXOZZ_Asymptotic_xww_ee1JLP.\$MASS", "output_EXOZZ_Asymptotic_xww_mm1JHP.\$MASS", "output_EXOZZ_Asymptotic_xww_mm1JLP.\$MASS", "output_EXOZZ_Asymptotic_comb_xww_mm1J", "output_EXOZZ_Asymptotic_comb_xww_ee1J"

#-------------------- with systematic
#All cates
DIRNAME=${CARDDIR}_all
cp -r ${CARDDIR} AllKindsOfCards/${DIRNAME}
cat TEMPLATES/combine_exec.sh | sed -e s%TEMPLATE_CARDDIR%${DIRNAME}%g | sed -e s%TEMPLATE_CARDCAT%\"comb_xww\"%g | sed -e s%TEMPLATE_WITHSYS%1%g  > AllKindsOfCards/${DIRNAME}/combine_exec.sh
cat TEMPLATES/mergeCombinationTrees.sh | sed -e s%RESULTDIR_TEMPLATE%\"output_EXOZZ_Asymptotic_comb_xww\"%g > AllKindsOfCards/${DIRNAME}/mergeCombinationTrees.sh

# m LP
DIRNAME=${CARDDIR}_mmLP
cp -r ${CARDDIR} AllKindsOfCards/${DIRNAME}
cat TEMPLATES/combine_exec.sh | sed -e s%TEMPLATE_CARDDIR%${DIRNAME}%g | sed -e s%TEMPLATE_CARDCAT%\"xww_mm1JLP.\$mass\"%g | sed -e s%TEMPLATE_WITHSYS%1%g  > AllKindsOfCards/${DIRNAME}/combine_exec.sh
cat TEMPLATES/mergeCombinationTrees.sh | sed -e s%RESULTDIR_TEMPLATE%\"output_EXOZZ_Asymptotic_xww_mm1JLP.\$MASS\"%g > AllKindsOfCards/${DIRNAME}/mergeCombinationTrees.sh

# m HP
DIRNAME=${CARDDIR}_mmHP
cp -r ${CARDDIR} AllKindsOfCards/${DIRNAME}
cat TEMPLATES/combine_exec.sh | sed -e s%TEMPLATE_CARDDIR%${DIRNAME}%g | sed -e s%TEMPLATE_CARDCAT%\"xww_mm1JHP.\$mass\"%g | sed -e s%TEMPLATE_WITHSYS%1%g  > AllKindsOfCards/${DIRNAME}/combine_exec.sh
cat TEMPLATES/mergeCombinationTrees.sh | sed -e s%RESULTDIR_TEMPLATE%\"output_EXOZZ_Asymptotic_xww_mm1JHP.\$MASS\"%g > AllKindsOfCards/${DIRNAME}/mergeCombinationTrees.sh

# e LP
DIRNAME=${CARDDIR}_eeLP
cp -r ${CARDDIR} AllKindsOfCards/${DIRNAME}
cat TEMPLATES/combine_exec.sh | sed -e s%TEMPLATE_CARDDIR%${DIRNAME}%g | sed -e s%TEMPLATE_CARDCAT%\"xww_ee1JLP.\$mass\"%g | sed -e s%TEMPLATE_WITHSYS%1%g  > AllKindsOfCards/${DIRNAME}/combine_exec.sh
cat TEMPLATES/mergeCombinationTrees.sh | sed -e s%RESULTDIR_TEMPLATE%\"output_EXOZZ_Asymptotic_xww_ee1JLP.\$MASS\"%g > AllKindsOfCards/${DIRNAME}/mergeCombinationTrees.sh

# e HP
DIRNAME=${CARDDIR}_eeHP
cp -r ${CARDDIR} AllKindsOfCards/${DIRNAME}
cat TEMPLATES/combine_exec.sh | sed -e s%TEMPLATE_CARDDIR%${DIRNAME}%g | sed -e s%TEMPLATE_CARDCAT%\"xww_ee1JHP.\$mass\"%g | sed -e s%TEMPLATE_WITHSYS%1%g  > AllKindsOfCards/${DIRNAME}/combine_exec.sh
cat TEMPLATES/mergeCombinationTrees.sh | sed -e s%RESULTDIR_TEMPLATE%\"output_EXOZZ_Asymptotic_xww_ee1JHP.\$MASS\"%g > AllKindsOfCards/${DIRNAME}/mergeCombinationTrees.sh

# e
DIRNAME=${CARDDIR}_ee
cp -r ${CARDDIR} AllKindsOfCards/${DIRNAME}
cat TEMPLATES/combine_exec.sh | sed -e s%TEMPLATE_CARDDIR%${DIRNAME}%g | sed -e s%TEMPLATE_CARDCAT%\"comb_xww_ee1J\"%g | sed -e s%TEMPLATE_WITHSYS%1%g  > AllKindsOfCards/${DIRNAME}/combine_exec.sh
cat TEMPLATES/mergeCombinationTrees.sh | sed -e s%RESULTDIR_TEMPLATE%\"output_EXOZZ_Asymptotic_comb_xww_ee1J\"%g > AllKindsOfCards/${DIRNAME}/mergeCombinationTrees.sh

# m
DIRNAME=${CARDDIR}_mm
cp -r ${CARDDIR} AllKindsOfCards/${DIRNAME}
cat TEMPLATES/combine_exec.sh | sed -e s%TEMPLATE_CARDDIR%${DIRNAME}%g | sed -e s%TEMPLATE_CARDCAT%\"comb_xww_mm1J\"%g | sed -e s%TEMPLATE_WITHSYS%1%g  > AllKindsOfCards/${DIRNAME}/combine_exec.sh
cat TEMPLATES/mergeCombinationTrees.sh | sed -e s%RESULTDIR_TEMPLATE%\"output_EXOZZ_Asymptotic_comb_xww_mm1J\"%g > AllKindsOfCards/${DIRNAME}/mergeCombinationTrees.sh


#-------------------------------- without systematic


#All cates
DIRNAME=${CARDDIR}_all_nosys
cp -r ${CARDDIR} AllKindsOfCards/${DIRNAME}
cat TEMPLATES/combine_exec.sh | sed -e s%TEMPLATE_CARDDIR%${DIRNAME}%g | sed -e s%TEMPLATE_CARDCAT%\"comb_xww\"%g | sed -e s%TEMPLATE_WITHSYS%0%g  > AllKindsOfCards/${DIRNAME}/combine_exec.sh
cat TEMPLATES/mergeCombinationTrees.sh | sed -e s%RESULTDIR_TEMPLATE%\"output_EXOZZ_Asymptotic_comb_xww\"%g > AllKindsOfCards/${DIRNAME}/mergeCombinationTrees.sh

# m LP
DIRNAME=${CARDDIR}_mmLP_nosys
cp -r ${CARDDIR} AllKindsOfCards/${DIRNAME}
cat TEMPLATES/combine_exec.sh | sed -e s%TEMPLATE_CARDDIR%${DIRNAME}%g | sed -e s%TEMPLATE_CARDCAT%\"xww_mm1JLP.\$mass\"%g | sed -e s%TEMPLATE_WITHSYS%0%g  > AllKindsOfCards/${DIRNAME}/combine_exec.sh
cat TEMPLATES/mergeCombinationTrees.sh | sed -e s%RESULTDIR_TEMPLATE%\"output_EXOZZ_Asymptotic_xww_mm1JLP.\$MASS\"%g > AllKindsOfCards/${DIRNAME}/mergeCombinationTrees.sh

# m HP
DIRNAME=${CARDDIR}_mmHP_nosys
cp -r ${CARDDIR} AllKindsOfCards/${DIRNAME}
cat TEMPLATES/combine_exec.sh | sed -e s%TEMPLATE_CARDDIR%${DIRNAME}%g | sed -e s%TEMPLATE_CARDCAT%\"xww_mm1JHP.\$mass\"%g | sed -e s%TEMPLATE_WITHSYS%0%g  > AllKindsOfCards/${DIRNAME}/combine_exec.sh
cat TEMPLATES/mergeCombinationTrees.sh | sed -e s%RESULTDIR_TEMPLATE%\"output_EXOZZ_Asymptotic_xww_mm1JHP.\$MASS\"%g > AllKindsOfCards/${DIRNAME}/mergeCombinationTrees.sh

# e LP
DIRNAME=${CARDDIR}_eeLP_nosys
cp -r ${CARDDIR} AllKindsOfCards/${DIRNAME}
cat TEMPLATES/combine_exec.sh | sed -e s%TEMPLATE_CARDDIR%${DIRNAME}%g | sed -e s%TEMPLATE_CARDCAT%\"xww_ee1JLP.\$mass\"%g | sed -e s%TEMPLATE_WITHSYS%0%g  > AllKindsOfCards/${DIRNAME}/combine_exec.sh
cat TEMPLATES/mergeCombinationTrees.sh | sed -e s%RESULTDIR_TEMPLATE%\"output_EXOZZ_Asymptotic_xww_ee1JLP.\$MASS\"%g > AllKindsOfCards/${DIRNAME}/mergeCombinationTrees.sh

# e HP
DIRNAME=${CARDDIR}_eeHP_nosys
cp -r ${CARDDIR} AllKindsOfCards/${DIRNAME}
cat TEMPLATES/combine_exec.sh | sed -e s%TEMPLATE_CARDDIR%${DIRNAME}%g | sed -e s%TEMPLATE_CARDCAT%\"xww_ee1JHP.\$mass\"%g | sed -e s%TEMPLATE_WITHSYS%0%g  > AllKindsOfCards/${DIRNAME}/combine_exec.sh
cat TEMPLATES/mergeCombinationTrees.sh | sed -e s%RESULTDIR_TEMPLATE%\"output_EXOZZ_Asymptotic_xww_ee1JHP.\$MASS\"%g > AllKindsOfCards/${DIRNAME}/mergeCombinationTrees.sh

# e
DIRNAME=${CARDDIR}_ee_nosys
cp -r ${CARDDIR} AllKindsOfCards/${DIRNAME}
cat TEMPLATES/combine_exec.sh | sed -e s%TEMPLATE_CARDDIR%${DIRNAME}%g | sed -e s%TEMPLATE_CARDCAT%\"comb_xww_ee1J\"%g | sed -e s%TEMPLATE_WITHSYS%0%g  > AllKindsOfCards/${DIRNAME}/combine_exec.sh
cat TEMPLATES/mergeCombinationTrees.sh | sed -e s%RESULTDIR_TEMPLATE%\"output_EXOZZ_Asymptotic_comb_xww_ee1J\"%g > AllKindsOfCards/${DIRNAME}/mergeCombinationTrees.sh

# m
DIRNAME=${CARDDIR}_mm_nosys
cp -r ${CARDDIR} AllKindsOfCards/${DIRNAME}
cat TEMPLATES/combine_exec.sh | sed -e s%TEMPLATE_CARDDIR%${DIRNAME}%g | sed -e s%TEMPLATE_CARDCAT%\"comb_xww_mm1J\"%g | sed -e s%TEMPLATE_WITHSYS%0%g  > AllKindsOfCards/${DIRNAME}/combine_exec.sh
cat TEMPLATES/mergeCombinationTrees.sh | sed -e s%RESULTDIR_TEMPLATE%\"output_EXOZZ_Asymptotic_comb_xww_mm1J\"%g > AllKindsOfCards/${DIRNAME}/mergeCombinationTrees.sh
