// -*- C++ -*-
//
// Package:    Gen_ana
// Class:      Gen_ana
// 
/**\class Gen_ana Gen_ana.cc GEN_ANA/Gen_ana/src/Gen_ana.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
 */
//
// Original Author:  Shuai Liu,591 R-005,+41227671371,
//         Created:  Wed Jun  6 17:49:52 CEST 2012
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"



// necessary objects:
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"  //control the GenParticleCollection Lable and the reco namesapce


//TFileService
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

// C++ includes
#include <iostream>
// some root includes
#include "TLorentzVector.h"
#include <TMath.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TTree.h>
//using namespace
using namespace std;
using namespace edm;
using namespace reco;

//
// class declaration
//
// class declaration
//

class Gen_ana : public edm::EDAnalyzer {
	public:
		explicit Gen_ana(const edm::ParameterSet&);
		~Gen_ana();

		static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


	private:
		virtual void beginJob() ;
		virtual void analyze(const edm::Event&, const edm::EventSetup&);
		virtual void endJob() ;

		virtual void beginRun(edm::Run const&, edm::EventSetup const&);
		virtual void endRun(edm::Run const&, edm::EventSetup const&);
		virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

		// ----------member data ---------------------------


		TTree *ExTree;
		int nEvt;  
		// used to count the number of events
		vector<const reco::Candidate *> *phig;
		vector<const reco::Candidate *> *pwplus;
		vector<const reco::Candidate *> *pwminus;
		vector<const reco::Candidate *> *pnu;
		vector<const reco::Candidate *> *pele;
		vector<const reco::Candidate *> *pmu;
		vector<const reco::Candidate *> *pjet;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
Gen_ana::Gen_ana(const edm::ParameterSet& iConfig)

{
	//now do what ever initialization is needed
}


Gen_ana::~Gen_ana()
{

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
	void
Gen_ana::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	nEvt++;

	Handle<reco::GenParticleCollection> genParticles;//define genParticle
	iEvent.getByLabel(InputTag("genParticles"), genParticles);//connect genparticle to event . so p=genParticles->begin(); p!= genParticles->end() can loop over all event

	vector<const reco::Candidate *> hig;
	vector<const reco::Candidate *> wplus;
	vector<const reco::Candidate *> wminus;
	vector<const reco::Candidate *> nu;
	vector<const reco::Candidate *> ele;
	vector<const reco::Candidate *> mu;
	vector<const reco::Candidate *> jet;
	
	bool tauflag=0;

	for(reco::GenParticleCollection::const_iterator p=genParticles->begin(); p!= genParticles->end(); ++p)
	{
		if((p->pdgId()==39)&&p->status()==3)//Graviton
		{
			//cout<<"graviton!"<<endl;
			const reco::Candidate* ph = &(*p);
			hig.push_back(ph);
			for(int i=0;p->daughter(i)!=NULL;i++)//loop on Graviton daughter
			{
				if(abs(p->daughter(i)->pdgId())==24)//w
				{
					const reco::Candidate* pw = p->daughter(i);
					if(pw->charge()==1)wplus.push_back(pw);
					else wminus.push_back(pw);

					for(int i=0;pw->daughter(i)!=NULL;i++)//loop on w daughter
					{
						const reco::Candidate* pl = pw->daughter(i);
						//cout<<pl->pdgId()<<endl;
						//cout<<pl->status()<<endl;
						if(abs(pl->pdgId())==15||abs(pl->pdgId())==16)tauflag=1;//descard tau
						else if(abs(pl->pdgId())==11)ele.push_back(pl);
						else if(abs(pl->pdgId())==13)mu.push_back(pl);
						else if(abs(pl->pdgId())==12||abs(pl->pdgId())==14)nu.push_back(pl);
						else if(pl->status()==3)jet.push_back(pl); 

					}//end of loop on w daughter
				}//end of if w


			}//end of loop on higgs daughter
		}//end of if higgs
	}//end of partile loop
	phig=&hig;
	pwplus=&wplus;
	pwminus=&wminus;
	pnu=&nu;
	pele=&ele;
	pmu=&mu;
	pjet=&jet;
	
	if(!tauflag)ExTree->Fill();	
}


// ------------ method called once each job just before starting event loop  ------------
	void 
Gen_ana::beginJob()
{
	nEvt=0;
	edm:: Service<TFileService> fs; 
	ExTree = fs->make<TTree>("ExTree","ExTree");

	ExTree->Branch("phig", "vector<const reco::Candidate *>", &phig);
	ExTree->Branch("pwplus", "vector<const reco::Candidate *>", &pwplus);
	ExTree->Branch("pwminus", "vector<const reco::Candidate *>", &pwminus);
	ExTree->Branch("pele", "vector<const reco::Candidate *>", &pele);
	ExTree->Branch("pmu", "vector<const reco::Candidate *>", &pmu);
	ExTree->Branch("pjet", "vector<const reco::Candidate *>", &pjet);
	ExTree->Branch("pnu", "vector<const reco::Candidate *>", &pnu);


}

// ------------ method called once each job just after ending the event loop  ------------
	void 
Gen_ana::endJob() 
{
	std::cout<<"++++++++++++++++++++++++++++++++++++++"<<std::endl;
	std::cout<<"analyzed "<<nEvt<<" events "<<std::endl;
}

// ------------ method called when starting to processes a run  ------------
	void 
Gen_ana::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
	void 
Gen_ana::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
	void 
Gen_ana::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
	void 
Gen_ana::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
Gen_ana::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(Gen_ana);
