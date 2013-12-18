#ifndef NtupleBranches_H
#define NtupleBranches_H

#include <DataFormats/PatCandidates/interface/Jet.h>
#include <DataFormats/JetReco/interface/Jet.h>
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"


#include "TTree.h"

/*here we declare the input and output variables*/

namespace NtupleBranches {


//=================================================================================================================== 
  /* output tree variables*/
    
    int 	        	      njetsAK5         ;
    std::vector<float>  	      jetAK5_pt        ;
    std::vector<float>  	      jetAK5_eta       ;
    std::vector<float>  	      jetAK5_mass      ;
    std::vector<float>  	      jetAK5_phi       ;
    std::vector<float>  	      jetAK5_e         ;
    int 	        	      njetsCA8         ;
    std::vector<float>  	      jetCA8_pt        ;
    std::vector<float>  	      jetCA8_eta       ;
    std::vector<float>  	      jetCA8_mass      ;
    std::vector<float>  	      jetCA8_phi       ;
    std::vector<float>  	      jetCA8_e         ;
    int 	        	      njetsCA8pruned   ;
    std::vector<float>  	      jetCA8pruned_pt  ;
    std::vector<float>  	      jetCA8pruned_eta ;
    std::vector<float>  	      jetCA8pruned_mass;
    std::vector<float>  	      jetCA8pruned_phi ;
    std::vector<float>  	      jetCA8pruned_e   ;
    std::vector<int>    	      nsubjets         ;
    std::vector< std::vector<float> > subjet_pt        ;
    std::vector< std::vector<float> > subjet_eta       ;
    std::vector< std::vector<float> > subjet_mass      ;
    std::vector< std::vector<float> > subjet_phi       ;
    std::vector< std::vector<float> > subjet_e         ;
		       
//=================================================================================================================== 
  /* input variables*/
  
  edm::Handle<std::vector<pat::Jet> > jetsAK5      ;
  edm::Handle<std::vector<pat::Jet> > jetsCA8      ;
  edm::Handle<std::vector<pat::Jet> > jetsCA8pruned;

  /* labels */
  
  edm::InputTag jetAK5Label      ("selectedPatJets"				);
  edm::InputTag jetCA8Label      ("selectedPatJetsCA8CHSwithQJetsForBoostedTaus");
  edm::InputTag jetCA8prunedLabel("selectedPatJetsCA8CHSpruned" 		);
  
//===================================================================================================================      
  void branch( TTree* tree ){

    tree->Branch( "njetsAK5"         , &njetsAK5    	  );
    tree->Branch( "jetAK5_pt"        , &jetAK5_pt   	  );
    tree->Branch( "jetAK5_eta"       , &jetAK5_eta  	  );
    tree->Branch( "jetAK5_mass"      , &jetAK5_mass 	  );
    tree->Branch( "jetAK5_phi"       , &jetAK5_phi  	  );
    tree->Branch( "jetAK5_e"         , &jetAK5_e    	  );
    tree->Branch( "njetsCA8"         , &njetsCA8    	  );
    tree->Branch( "jetCA8_pt"        , &jetCA8_pt   	  );
    tree->Branch( "jetCA8_eta"       , &jetCA8_eta  	  );
    tree->Branch( "jetCA8_mass"      , &jetCA8_mass 	  );
    tree->Branch( "jetCA8_phi"       , &jetCA8_phi  	  );
    tree->Branch( "jetCA8_e"         , &jetCA8_e    	  );
    tree->Branch( "njetsCA8pruned"   , &njetsCA8pruned    );         
    tree->Branch( "jetCA8pruned_pt"  , &jetCA8pruned_pt   );            
    tree->Branch( "jetCA8pruned_eta" , &jetCA8pruned_eta  );            
    tree->Branch( "jetCA8pruned_mass", &jetCA8pruned_mass );     
    tree->Branch( "jetCA8pruned_phi" , &jetCA8pruned_phi  );            
    tree->Branch( "jetCA8pruned_e"   , &jetCA8pruned_e    ); 
    tree->Branch( "nsubjets"         , &nsubjets          );
    tree->Branch( "subjet_pt"        , &subjet_pt         );                
  
  }

//===================================================================================================================        
  void getEventByLabels( edm::EventBase const & event ){

    //event.getByLabel(jetAK5Label      , jetsAK5      );
    event.getByLabel(jetCA8Label      , jetsCA8      );
    event.getByLabel(jetCA8prunedLabel, jetsCA8pruned);
  }

//=================================================================================================================== 
  void reset( void ){
           
    jetAK5_pt	     .clear();
    jetAK5_eta       .clear();
    jetAK5_mass      .clear();
    jetAK5_phi       .clear();
    jetAK5_e	     .clear();
    jetCA8_pt	     .clear();
    jetCA8_eta       .clear();
    jetCA8_mass      .clear();
    jetCA8_phi       .clear();
    jetCA8_e	     .clear();
    jetCA8pruned_pt  .clear();
    jetCA8pruned_eta .clear();
    jetCA8pruned_mass.clear();
    jetCA8pruned_phi .clear();
    jetCA8pruned_e   .clear();
    nsubjets         .clear();
    subjet_pt        .clear();
  }
  
}

#endif // NtupleBranches_H
