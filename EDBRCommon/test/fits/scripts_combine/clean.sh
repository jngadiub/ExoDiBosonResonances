#! /bin/bash

for dir in $( ls -d * )
do
echo Moving in $dir
cd $dir
rm -f *.log
rm -f log*
rm -f batchScript*
rm -f model*
rm -f comb.txt

rm -fr crab_*
rm -fr comb_*
rm -fr output_*
rm -fr logs
cd ..
done

