#!/bin/bash
MASSPOINT=sedMP
RVALUE=`expr $1 - 1`
echo Argument 1 is ${RVALUE} # CRAB jobs start from 1, python arrays start from 0
echo Argument 2 is ${MASSPOINT}
python run_fullCLs_TF.py ${RVALUE} ${MASSPOINT}
echo; echo '----';
ls -lh;
echo '----';
source ./*src
