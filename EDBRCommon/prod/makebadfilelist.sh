#!/bin/bash
cd logs
egrep "Input file root://eoscms//eos/cms/store/group" -r *| awk '{print $3}'  > ../badfilelist_temp.txt
cat ../badfilelist_temp.txt | sed -e s%root://eoscms//eos/cms%%g  > ../badfilelist.txt
rm -f ../badfilelist_temp.txt
