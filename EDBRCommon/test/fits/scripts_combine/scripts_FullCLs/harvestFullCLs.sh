#!/bin/bash

rm observedLimit.txt
rm expectedLimit.txt
rm limitM2.txt
rm limitM1.txt
rm limitP1.txt
rm limitP2.txt

for i in `cat masses.txt`; do
    grep '95%' $i/limitOb*txt | awk '{print $4}' >> observedLimit.txt
    grep '95%' $i/limitM2*txt | awk '{print $4}' >> limitM2.txt
    grep '95%' $i/limitM1*txt | awk '{print $4}' >> limitM1.txt
    grep '95%' $i/limitE0*txt | awk '{print $4}' >> expectedLimit.txt
    grep '95%' $i/limitP1*txt | awk '{print $4}' >> limitP1.txt
    grep '95%' $i/limitP2*txt | awk '{print $4}' >> limitP2.txt
done

root -b makeFullCLsTree.C
