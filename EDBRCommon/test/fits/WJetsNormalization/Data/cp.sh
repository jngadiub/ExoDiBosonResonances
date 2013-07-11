#!/bin/bash

TREEPATH=/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv10/AnaSigTree


cp ${TREEPATH}/treeEDBR_SingleTop_xww.root  ./
cp ${TREEPATH}/treeEDBR_TTBARpowheg_xww.root ./ 
hadd -f treeEDBR_VJets_xww.root  $TREEPATH/treeEDBR_WJetsPt180_xww.root $TREEPATH/treeEDBR_DYJets_xww.root
cp $TREEPATH/treeEDBR_VV_xww.root  ./
cp $TREEPATH/treeEDBR_data_xww.root ./

