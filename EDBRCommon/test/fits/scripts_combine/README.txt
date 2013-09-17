For 1d limits (requires datacard creation in 1d mode)

#0: copy all scripts in the directory of the cards and move there
#1: for M in $( cat masses.txt ); do ./make_combined_cards.sh $M ; done
#2: edit paths and names in combine_exec.sh
#3: edit paths and names in parallelizeCombine.sh
#4: for M in $( cat masses.txt ); do ./parallelizeCombine.sh $M ; done
#5: wait for the jobs on LXB to be finished
#6: edit paths and names in mergeCombinationTrees.sh
#7: edit paths and names in mergeHarvestedCombinationTrees.sh
#8: for M in $( cat masses.txt ); do ./mergeCombinationTrees.sh $M ; done
#9: ./mergeHarvestedCombinationTrees.sh
#10: edit paths and names in plot_golfcourse_Asymptotic.C
#11: run with root: $> root -b
     .L plot_golfcourse_Asymptotic.C+
     plot_golfcourse_Asymptotic()


For 2d limits (requires datacard creation in 2d mode)
#0: copy all scripts in the directory of the cards and move there
#1: for M in $( cat masses.txt ); do for W in $( cat widths.txt );do ./make_combined_cards.sh $M $W ; done ; done
#2: edit paths and names in combine_exec.sh
#3: edit paths and names in parallelizeCombine.sh
#4: for M in $( cat masses.txt ); do for W in $( cat widths.txt );do ./parallelizeCombine.sh $M $W ; done ; done
#5: wait for the jobs on LXB to be finished
#6: edit paths and names in mergeCombinationTrees.sh
#7: edit paths and names in mergeHarvestedCombinationTrees.sh
#8: for M in $( cat masses.txt ); do for W in $( cat widths.txt );do ./mergeCombinationTrees.sh $M $W ; done ; done
#9 for W in $( cat widths.txt );do ./mergeHarvestedCombinationTrees.sh $W ; done#10: edit paths and names in plot_golfcourse_Asymptotic.C
#11: for W in $( cat widths.txt );do root -b -q plot_golfcourse_Asymptotic.C+\(1,\"$W\"\) ; done
#12 python 2dgraphs.py