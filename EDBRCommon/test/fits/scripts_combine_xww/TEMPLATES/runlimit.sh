#! /bin/bash

(for M in $( cat masses_xww.txt ); do ./make_combined_cards.sh $M ; done)>& make_combined_cards_log
(for M in $( cat masses_xww.txt ); do ./parallelizeCombine.sh $M ; done) >& parallelizeCombine_log
(for M in $( cat masses_xww.txt ); do ./mergeCombinationTrees.sh $M ; done) >& mergeCombinationTrees_log
./mergeHarvestedCombinationTrees.sh >& mergeHarvestedCombinationTrees_log
root -l -b -q plot_golfcourse_Asymptotic.C++ >& plot_golfcourse_Asymptotic_log
root -l -b -q plot_Significance.C++ >& plot_Significance_log
