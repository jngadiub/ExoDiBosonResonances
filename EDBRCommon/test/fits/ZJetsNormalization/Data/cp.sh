#!/bin/bash

###TREEPATH=/afs/cern.ch/user/b/bonato/scratch0/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv2f/fullsig
TREEPATH=/afs/cern.ch/user/b/bonato/scratch0/PhysAnalysis/EXOVV_2012/CMGTools/CMSSW_5_3_9/src/ExoDiBosonResonances/EDBRCommon/test/fits/ZJetsNormalization/Data/

#cp ${TREEPATH}/EXOVVTree_unrolled_TT_SIG.root ./treeEDBR_TTBARpowheg.root 
#cp ${TREEPATH}/EXOVVTree_unrolled_VJets_SIG.root ./treeEDBR_VJets.root 
#cp ${TREEPATH}/EXOVVTree_unrolled_VV_SIG.root ./treeEDBR_VV.root 
hadd -f  $TREEPATH/EXOVVTree_unrolled_DATA_SIG.root $TREEPATH/EXOVVTree_unrolled_Muon_SIG.root  $TREEPATH/EXOVVTree_unrolled_Electron_SIG.root


########################### even older....
#hadd -f treeEDBR_VJets.root $TREEPATH/treeEDBR_DYJetsPt100.root $TREEPATH/treeEDBR_DYJetsPt70To100.root
#hadd -f treeEDBR_VV.root $TREEPATH/treeEDBR_WW.root $TREEPATH/treeEDBR_WZ.root $TREEPATH/treeEDBR_ZZ.root






