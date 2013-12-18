#ifndef JetsNtuplizer_H
#define JetsNtuplizer_H

#include "NtupleBranches.h"

using namespace NtupleBranches;

namespace JetsNtuplizer{

  void fillJetsBranches( void ){

   /*here we want to save the jets info*/
    /*njetsAK5 = 0;
    for(unsigned j=0; j<jetsAK5->size(); ++j){
      njetsAK5++;
      jetAK5_pt  .push_back((*jetsAK5)[j].pt()    );
      jetAK5_eta .push_back((*jetsAK5)[j].eta()   );
      jetAK5_mass.push_back((*jetsAK5)[j].phi()   );
      jetAK5_phi .push_back((*jetsAK5)[j].mass()  );
      jetAK5_e   .push_back((*jetsAK5)[j].energy());
    }*/

    njetsCA8 = 0;
    for(unsigned j=0; j<jetsCA8->size(); ++j){
      njetsCA8++;
      jetCA8_pt  .push_back((*jetsCA8)[j].pt()    );
      jetCA8_eta .push_back((*jetsCA8)[j].eta()   );
      jetCA8_mass.push_back((*jetsCA8)[j].phi()   );
      jetCA8_phi .push_back((*jetsCA8)[j].mass()  );
      jetCA8_e   .push_back((*jetsCA8)[j].energy());
    }

    njetsCA8pruned = 0;
    std::vector<float> vSubjetpt;
    for(unsigned int j=0; j<jetsCA8pruned->size(); ++j){
      njetsCA8pruned++;
      vSubjetpt.clear();
      jetCA8pruned_pt  .push_back((*jetsCA8pruned)[j].pt()    );
      jetCA8pruned_eta .push_back((*jetsCA8pruned)[j].eta()   );
      jetCA8pruned_mass.push_back((*jetsCA8pruned)[j].phi()   );
      jetCA8pruned_phi .push_back((*jetsCA8pruned)[j].mass()  );
      jetCA8pruned_e   .push_back((*jetsCA8pruned)[j].energy());
      nsubjets         .push_back((*jetsCA8pruned)[j].numberOfDaughters());
      for(unsigned int sj=0; sj<(*jetsCA8pruned)[j].numberOfDaughters(); ++sj){
        vSubjetpt.push_back( (*jetsCA8pruned)[j].daughter(sj)->pt() );
      }
      subjet_pt.push_back(vSubjetpt);
    }             
  }

}

#endif // JetsNtuplizer_H
