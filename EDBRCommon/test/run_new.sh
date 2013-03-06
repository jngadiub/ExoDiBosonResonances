#! /bin/bash

MYWORKAREA=$CMSSW_BASE/src/
cd $MYWORKAREA
eval `scram runtime -sh`

TREEDIR=$1

date=$(date +"%y%m%d_%k%M%S")
mkdir -p /tmp/${USER}/treeEDBR_<type>_<sample>_${date}
cd /tmp/${USER}/treeEDBR_<type>_<sample>_${date}

cmsRun $TREEDIR/logs/<sample>/analyzerEDBR_<type>_<sample>_cfg.py  &> log_analyzerEDBR_<type>_<sample>.out

mv treeEDBR_<sample>.root  $TREEDIR
mv log_analyzerEDBR_<type>_<sample>.out $TREEDIR/logs/<sample>


