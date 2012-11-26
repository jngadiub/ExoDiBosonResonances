#include "ExoDiBosonResonances/PATtupleProduction/plugins/QjetsAdder.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

#include "FWCore/Framework/interface/MakerMacros.h"

void QjetsAdder::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {
  // read input collection
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(src_, jets);
  
  
  // prepare room for output
  std::vector<pat::Jet> outJets;   outJets.reserve(jets->size());

  for ( typename edm::View<pat::Jet>::const_iterator jetIt = jets->begin() ; jetIt != jets->end() ; ++jetIt ) {
    pat::Jet newCand(*jetIt);
    //edm::Ptr<pat::Jet> jetPtr = jets->ptrAt(jetIt - jets->begin());

    //refill and recluster
    vector<fastjet::PseudoJet> allconstits;
    for (unsigned k =0; k < newCand.getPFConstituents().size(); k++){
      const reco::PFCandidate* thisParticle = newCand.getPFConstituent(k).get();
      allconstits.push_back( fastjet::PseudoJet( thisParticle->px(), thisParticle->py(), thisParticle->pz(), thisParticle->energy() ) );
    }

    fastjet::JetDefinition jetDef(fastjet::cambridge_algorithm, jetRad_);
    if (mJetAlgo_== "AK") jetDef.set_jet_algorithm( fastjet::antikt_algorithm );
    else if (mJetAlgo_ == "CA") jetDef.set_jet_algorithm( fastjet::cambridge_algorithm );
    else throw cms::Exception("GroomedJetFiller") << " unknown jet algorithm " << std::endl;

    fastjet::ClusterSequence thisClustering_basic(allconstits, jetDef);
    std::vector<fastjet::PseudoJet> out_jets_basic = sorted_by_pt(thisClustering_basic.inclusive_jets(cutoff_));
    //std::cout << newCand.pt() << " " << out_jets_basic.size() <<std::endl;
    if(out_jets_basic.size()==0){ // jet reclustering failed, most likely due to the higher cutoff. Use a recognizeable default value for this jet
      newCand.addUserFloat("qjetsvolatility", -1. );
      outJets.push_back(newCand);
      break;
    }
    assert( out_jets_basic.size()==1 ); // jet reclustering of one jet should yield exactly one jet at this stage
    

    // setup obkjects for qjets computation
    fastjet::JetDefinition qjet_def(&qjetsAlgo_);

    std::vector<double> qjetmass;

    vector<fastjet::PseudoJet> constits;
    unsigned int nqjetconstits = out_jets_basic.at(0).constituents().size(); // there should always be exactly one reclsutered jet => always "at(0)"
    if (nqjetconstits < (unsigned int) QjetsPreclustering_) constits = out_jets_basic.at(0).constituents();
    else constits = out_jets_basic.at(0).associated_cluster_sequence()->exclusive_subjets_up_to(out_jets_basic.at(0),QjetsPreclustering_);

    // create probabilistic recusterings
    for(unsigned int ii = 0 ; ii < (unsigned int) ntrial_ ; ii++){
      fastjet::ClusterSequence qjet_seq(constits, qjet_def);
      vector<fastjet::PseudoJet> inclusive_jets2 = sorted_by_pt(qjet_seq.inclusive_jets(cutoff_));
      if (inclusive_jets2.size()>0) // fill the massvalue only if the reclustering was successfull
	qjetmass.push_back(inclusive_jets2[0].m());
      
    }
    
    double RMS = sqrt ( ( std::inner_product( qjetmass.begin(), qjetmass.end(), qjetmass.begin(), 0 ))/qjetmass.size()  );
    double mean = std::accumulate( qjetmass.begin( ) , qjetmass.end( ) , 0 ) /qjetmass.size() ;
    
    newCand.addUserFloat("qjetsvolatility", RMS/mean );

    outJets.push_back(newCand);
  }

  std::auto_ptr<std::vector<pat::Jet> > out(new std::vector<pat::Jet>(outJets));
  iEvent.put(out);
 
}




DEFINE_FWK_MODULE(QjetsAdder);
