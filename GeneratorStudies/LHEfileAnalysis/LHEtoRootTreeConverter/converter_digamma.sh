#!/bin/bash

f=$1

fr="${f%.*}"
ff="${f%.*}.txt"
echo $f, $ff

awk '/<event>/,/LesHouchesEvents>/' $f | grep -iv "<event>" | grep -iv "</event>" | grep -iv "</LesHouchesEvents>" | grep -iv "9   100" | grep -iv "11   0"| grep -iv "12   0" | grep -iv "13   0" | grep -iv "1   -1    0"  | grep -iv "21   -1    0"  | grep -iv '2   -1    0'  | grep -iv "\-1   -1    0" | grep -iv "\-2   -1    0" | grep -iv '3   -1    0'  | grep -iv "\-3   -1    0" | grep -iv '4   -1    0'  | grep -iv "\-4   -1    0" | grep -iv '5   -1    0'  | grep -iv "\-5   -1    0" | grep -iv "\# " | grep -iv "\<\!\-\-"> $ff


frt='"'$fr'"'
echo $frt
root -l -n "photonTree_hZ_VVll.C(${frt})"
#rm $ff

echo "done"



# |grep -iv "1023    2"|grep -iv "23    2"|grep -iv "25    2"|grep -iv "24    2"|grep -iv "\-24    2" |grep -iv "1    1"|grep -iv "\-1    1"|grep -iv "2    1"|grep -iv "\-2    1"|grep -iv "3    1"|grep -iv "\-3    1"|grep -iv "4    1"|grep -iv "\-4    1"|grep -iv "5    1"|grep -iv "\-5    1"|grep -iv "11    1"|grep -iv "\-11    1"|grep -iv "13    1"|grep -iv "\-13    1"
