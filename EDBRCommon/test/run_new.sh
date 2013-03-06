#! /bin/bash

MYWORKAREA=$CMSSW_BASE/src/
cd $MYWORKAREA
eval `scram runtime -sh`

LOGDIR=$1
cd $LOGDIR

cmsRun analyzerEDBR_<type>_<sample>_cfg.py  &> log_analyzerEDBR_<type>_<sample>.out

mv treeEDBR_<sample>.root ../../

