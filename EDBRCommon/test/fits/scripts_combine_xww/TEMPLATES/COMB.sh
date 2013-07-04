cp ../masses.txt ./
(for M in $( cat masses.txt ); do ./make_combined_cards.sh $M ; done)>& make_combined_cards_log
#(for M in $( cat masses.txt ); do ./parallelizeCombine.sh $M ; done) >& parallelizeCombine_log
