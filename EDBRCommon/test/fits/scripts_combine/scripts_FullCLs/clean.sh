#! /bin/bash

OPT="standard"

if [ $# -gt 0 ]
then 
    OPT=$1
fi

if [ "$OPT" != "standard" ] && [ "$OPT" != "crab" ] && [ "$OPT" != "all" ]
    then
    echo "Wrong input argument. Option choosing what to clean can be: standard crab all"
    exit 1
fi

for dir in $( ls -d *0 )
do
  echo Moving in $dir
  cd $dir

if [ "$OPT" == "standard" ] || [ "$OPT" == "all" ]
    then
    rm -f *.log
    rm -f log_*
    rm -f batchScript*
    rm -f model*
    rm -f roostats*
    rm -fr output_*
    rm -fr logs
fi

if  [ "$OPT" == "crab" ] || [ "$OPT" == "all" ]
    then
    rm -fr crab_*
    rm -f crab.*
fi

if [ "$OPT" == "all" ]
   then
    rm -fr comb_*
fi


cd ..
done

