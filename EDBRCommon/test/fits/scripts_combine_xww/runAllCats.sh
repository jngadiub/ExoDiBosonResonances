#! /bin/bash

eval `scram runtime -sh`

cp -r AllKindsOfCards/* ../

RUN=1
CLEAN=0
BACKUP=0

#run
if test $RUN -eq 1
then
for folder in $(ls AllKindsOfCards)
do
	#echo $folder
	cd ../$folder
	echo I am in $PWD
	#cp AllKindsOfCards/$folder/* ../$folder/
	nohup sh runlimit.sh &
	cd ../scripts_combine_xww
done
fi


#clean
if test $CLEAN -eq 1
then
for folder in $(ls AllKindsOfCards)
do
	echo rm -rf ../$folder/
	rm -rf ../$folder/
done
fi

#backup
