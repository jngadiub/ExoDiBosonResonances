#! /bin/bash

############EDIT THIS PART#############
#save backup in OUTDIR
OUTDIR=LIMIT_WW_elemet80_newMJ_from50_newAlpha
#to make signal shapes
SIGTREE=/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/AnaSigTree_from50_noConv/
BKGDIR=FitSidebandsMJJ_CA8_WW_V11
CARDDIR=DataCards_XWW_V11_blind

#which step you want to do
#1. signal eff
DOSTEP1=0
#2. signal shapge
DOSTEP2=0
#3. bkg prediction
DOSTEP3=0
#4. create datacard 
DOSTEP4=0
#5. run limit 
DOSTEP5=0
#6. make limit plots
DOSTEP6=1

################################


#
eval `scram runtime -sh`
MYWORKAREA=$CMSSW_BASE/src/
FIT=$MYWORKAREA/ExoDiBosonResonances/EDBRCommon/test/fits/
mkdir -p $FIT/$OUTDIR

####################1.calculate efficiency##################################
if test $DOSTEP1 -eq 1
then
echo STEP1: signal efficiency
cd $MYWORKAREA/ExoDiBosonResonances/EDBRCommon/rootmacros
root -l -b -q absolute_eff.C >& signaleff_log
##for use
cp SignalEffPlots/efficiencies_MCSig.txt $FIT
##for backup
cp -r SignalEffPlots $FIT/$OUTDIR/
cp absolute_eff.C  $FIT/$OUTDIR/
cp signaleff_log $FIT/$OUTDIR/
fi
########################2.signal shapes######################################
if test $DOSTEP2 -eq 1
then
echo STEP2: signal shapes
cd $FIT/shape
rm -rf plots
mkdir plots
rm signalshape*_log
touch signalshape_command_log
#echo python shape.py -j 1 -p 1 -l 0 -f $SIGTREE >>signalshape_command_log
#echo python shape.py -j 1 -p 1 -l 1 -f $SIGTREE >>signalshape_command_log
#echo python shape.py -j 1 -p 1 -l 2 -f $SIGTREE >>signalshape_command_log
#echo python shape.py -j 1 -p 0 -l 0 -f $SIGTREE >>signalshape_command_log
#echo python shape.py -j 1 -p 0 -l 1 -f $SIGTREE >>signalshape_command_log
#echo python shape.py -j 1 -p 0 -l 2 -f $SIGTREE >>signalshape_command_log
echo python shape.py -j 1 -p -1 -l 0 -f $SIGTREE >>signalshape_command_log
echo python shape.py -j 1 -p -1 -l 1 -f $SIGTREE >>signalshape_command_log
#echo python shape.py -j 1 -p -1 -l 2 -f $SIGTREE >>signalshape_command_log
#python shape.py -j 1 -p 1 -l 0 -f $SIGTREE >& signalshape_ele_HP_log
#python shape.py -j 1 -p 1 -l 1 -f $SIGTREE >& signalshape_mu_HP_log
#python shape.py -j 1 -p 1 -l 2 -f $SIGTREE >& signalshape_all_HP_log
#python shape.py -j 1 -p 0 -l 0 -f $SIGTREE >& signalshape_ele_LP_log
#python shape.py -j 1 -p 0 -l 1 -f $SIGTREE >& signalshape_mu_LP_log
#python shape.py -j 1 -p 0 -l 2 -f $SIGTREE >& signalshape_all_LP_log
python shape.py -j 1 -p -1 -l 0 -f $SIGTREE >& signalshape_ele_ALLP_log
python shape.py -j 1 -p -1 -l 1 -f $SIGTREE >& signalshape_mu_ALLP_log
#python shape.py -j 1 -p -1 -l 2 -f $SIGTREE >& signalshape_all_ALLP_log
#for backup
cp -r plots $FIT/$OUTDIR
cp signalshape_* $FIT/$OUTDIR
cp -r pars  $FIT/$OUTDIR
fi
#######################3.get estimated bkg#################################
if test $DOSTEP3 -eq 1
then
echo STEP3: get estimated bkg
cd $FIT
make fitSidebands >& fitSidebands_make_log
./fitSidebands >&  fitSidebands_log
make fitBackground >& fitBackground_make_log
./fitBackground >& fitBackground_log
#for backup
cp fitSidebands.cpp $FIT/$OUTDIR
cp fitBackground.cpp $FIT/$OUTDIR
cp SidebandFitter.cc $FIT/$OUTDIR
cp -r $BKGDIR $FIT/$OUTDIR
cp fit*log $FIT/$OUTDIR
fi
#######################4.create datacard#################################
if test $DOSTEP4 -eq 1
then
echo STEP4: create datacard
#ww masses.txt
cd $FIT
cp $MYWORKAREA/ExoDiBosonResonances/backup/masses.txt $FIT
make create_datacards >& create_datacards_make_log
./create_datacards  >& create_datacards_log
#for backup
cp $FIT/masses.txt $FIT/$OUTDIR
cp create_datacards.cpp $FIT/$OUTDIR
cp -r $CARDDIR $FIT/$OUTDIR
cp create_datacards_*log $FIT/$OUTDIR
fi
######################5.run limit code, till bjobs##########################
if test $DOSTEP5 -eq 1
then
echo STEP5: run limit
cd $FIT/$CARDDIR
#cp ../scripts_combine/*.* ./
cp ../masses.txt ./
(for M in $( cat masses.txt ); do ./make_combined_cards.sh $M ; done)>& make_combined_cards_log
(for M in $( cat masses.txt ); do ./parallelizeCombine.sh $M ; done) >& parallelizeCombine_log
fi
#####################6.make limit plots#################################
if test $DOSTEP6 -eq 1
then
echo STEP6: make limit plots
cd $FIT/$CARDDIR
(for M in $( cat masses.txt ); do ./mergeCombinationTrees.sh $M ; done) >& mergeCombinationTrees_log
./mergeHarvestedCombinationTrees.sh >& mergeHarvestedCombinationTrees_log
root -l -b -q plot_golfcourse_Asymptotic.C++ >& plot_golfcourse_Asymptotic_log
#backup
cp -r ../$CARDDIR $FIT/$OUTDIR
fi


