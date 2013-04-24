#0: copy all scripts in the directory of the cards and move there
#1: BASH: for M in $( cat masses.txt ); do ./make_combined_cards.sh $M ; done 
#1: TCSH: foreach M ( `cat masses.txt` ); ./make_combined_cards.sh $M ; end
#2: edit paths and names in combine_exec.sh
#3: edit paths and names in parallelizeCombine.sh
#4: BASH: for M in $( cat masses.txt ); do ./parallelizeCombine.sh $M ; done
#4: TCSH: foreach M ( `cat masses.txt` ); ./parallelizeCombine.sh $M ; end
#5: wait for the jobs on LXB to be finished
#6: edit paths and names in mergeCombinationTrees.sh
#7: edit paths and names in mergeHarvestedCombinationTrees.sh
#8: BASH: for M in $( cat masses.txt ); do ./mergeCombinationTrees.sh $M ; done
#8: TCSH: foreach M ( `cat masses.txt` ); ./mergeCombinationTrees.sh $M ; end
#9: ./mergeHarvestedCombinationTrees.sh
#10: edit paths and names in plot_golfcourse_Asymptotic.C
#11: run with root: $> root -b
     .L plot_golfcourse_Asymptotic.C+
     plot_golfcourse_Asymptotic()


