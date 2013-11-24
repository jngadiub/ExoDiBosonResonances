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


################################################################
##### For Full CLs limits (requires datacard creation in 1d mode)
#####
#### EXPLANATION (originally written by TT, edited by AB):

We use CRAB to do these limits. Since I couldn't
 really understand how to adapt the official scripts for using combine
 with CRAB I decided to roll my own. Each CRAB task/directory takes
 care of one mass point. Each job in that task takes care of one value
 of the signal strength. In the ZZ analysis, the signal strengths
 excluded range from 1E-01 to 1E+03 [!!!]. The file run_fullCLs_TF.py
 has an array with the values from asymptotic limits; those values are
 used to delimit the range where combine should search for the limit.

WARNING: before starting, make sure to have enough free disk space.
The space occupied by the full output of one CLs limit on 20 mass
points is about 2Gb.

#0: copy all scripts in the directory of the cards and move there
#1: for M in $( cat masses.txt ); do ./make_combined_cards.sh $M ; done
#2: edit paths and names in run_fullCLs_TF.sh
#2: edit paths and names in gridificateCombine.sh
#4: for M in $( cat masses.txt ); do ./gridificateCombine.sh $M >> log_gridificate_$(date +%y%m%d).log ; done
#5: you can monitor all the jobs with this script
    for M in $( cat masses.txt ); do ./getCrabJob.sh $M status >> log_crabStatus_$(date +%y%m%d).log ; done
#6: wait for the jobs in CRAB to be finished. When they are done, you can retrieve 
    the output with
    for M in $( cat masses.txt ); do ./getCrabJob.sh $M get ; done
#7: edit paths, names and R values in mergeFullCLsTrees.sh
#8: edit paths and names in harvestFullCLs.sh and makeFullCLsTree.C
#9: for M in $( cat masses.txt); do ./mergeFullCLsTrees.sh $M ; done
#10: ./harvestFullCLs.sh
#11: edit paths and names in plot_golfcourse_HybridNew.C
#12: run with root: $> root -b plot_golfcourse_HybridNew.C+
