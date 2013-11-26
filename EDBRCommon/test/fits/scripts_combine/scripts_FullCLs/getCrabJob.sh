#! /bin/bash


if [ $# -lt 1 ]
    then
    echo "Need at least one input argument, the mass value (corresponding to the dir name with the crab subdir). Exiting"
    exit 1
fi

M=$1
ACTION="status" #default is to ask the crab status

if [ $# -eq 2 ]
    then
    ACTION=$2
    if [ "$ACTION" == "get" ] || [ "$ACTION" == "status" ]
	then
	ACTION=$2
    else
    echo "Second (optional) argument is the action required. Default action, performed also without specifying the second argument is \"status\". "
    echo "Other possibility is \"get\", for retrieving the output of the crab jobs."
    echo "You passed $2 , which is wrong. Exiting."
    exit 1

    fi
fi


cd $M

for crabdir in $( ls -d crab_*_[0-9]* )
  do
  if [ ${ACTION} == "status" ]  
      then
      crab -c $crabdir -status 
  elif [ ${ACTION} == "get" ] 
      then
      crab -c $crabdir -getoutput 
  else
      echo "Wrong action requested: ${ACTION}. Skipping."
  fi
done

cd -