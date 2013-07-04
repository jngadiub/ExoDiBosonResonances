#! /bin/bash

NUM=$1

for num in $1
do
echo $num
rm -rf $num/logs/
rm -rf $num/output_EXOZZ_Asymptotic_comb_xzz
./parallelizeCombine.sh $num
done

