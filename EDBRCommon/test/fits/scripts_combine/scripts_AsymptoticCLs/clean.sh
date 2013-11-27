#! /bin/bash

for dir in $( ls -d *[0-9] )
do
echo Moving in $dir
cd $dir
rm -f *.log
rm -f log_*.out
rm -f log_*.log
rm -f batchScript*
rm -f model*
rm -f comb.txt

rm -fr crab_*
rm -fr comb_*
rm -fr output_*
rm -fr logs
cd ..
done

