(for M in $( cat masses.txt ); do ./mergeCombinationTrees.sh $M ; done) >& mergeCombinationTrees_log
./mergeHarvestedCombinationTrees.sh >& mergeHarvestedCombinationTrees_log
root -l -b -q plot_golfcourse_Asymptotic.C++ >& plot_golfcourse_Asymptotic_log
