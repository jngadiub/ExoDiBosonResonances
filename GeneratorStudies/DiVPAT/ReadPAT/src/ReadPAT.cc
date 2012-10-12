// -*- C++ -*-
//
// Package:    ReadPAT
// Class:      ReadPAT
// 
/**\class ReadPAT ReadPAT.cc DivPAT/ReadPAT/src/ReadPAT.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  qiang li
//         Created:  Thu Oct 11 09:31:09 CDT 2012
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

//TFileService
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

// necessary objects:
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"  

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METFwd.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"


#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "DataFormats/Common/interface/View.h"
// C++ includes
#include <iostream>
// some root includes
#include <TLorentzVector.h>
#include <TMath.h>
#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>
//using namespace
using namespace std;
using namespace edm;
using namespace reco;
#define NMAX 2000

//
// class declaration
//

class ReadPAT : public edm::EDAnalyzer {
   public:
      explicit ReadPAT(const edm::ParameterSet&);
      ~ReadPAT();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      void change(double &a, double &b);
      void sortbypt(double * xpt, double * xeta, double * xphi, int Nx);

      // ----------member data ---------------------------
	TTree *ExTree;
	int nEvt; 

        int Nmu;
	double mupt[NMAX];
        double mueta[NMAX];
        double muphi[NMAX];
        double mue[NMAX];
        double mupz[NMAX];

        int Nele;
        double elept[NMAX];
        double eleeta[NMAX];
        double elephi[NMAX];
        double elee[NMAX];

        int Nmet;
        double metpt[NMAX];
        double metphi[NMAX];

        int Njet;
        double jetpt[NMAX];
        double jetphi[NMAX];
        double jeteta[NMAX];
        double jete[NMAX];

        double mjj;
        double phitw;
        double mtw;
        double mwreco;
        double ptwreco;
        double M4;
        double nPV1,nPV2;
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
ReadPAT::ReadPAT(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


ReadPAT::~ReadPAT()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
ReadPAT::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
       nEvt++;
       Nmu=0;
       Nele=0;
       Nmet=0;
       Njet=0;
       mjj=-200.;
       phitw=-200.;
       mtw=-200.;
       mwreco=-200.;
       ptwreco=-200.;
       M4=-200.;
       nPV1=-200.;
       nPV2=-200.;

	for(int i=0; i<NMAX; ++i) {
		mupt[i]=-200.;
		mueta[i]=-200.;
		muphi[i]=-200.;
                mue[i]=-200.;
                mupz[i]=-200.;
		elept[i]=-200.;
		eleeta[i]=-200.;
		elephi[i]=-200.;
                elee[i]=-200.;
		metpt[i]=-200.;
		metphi[i]=-200.;
		jetpt[i]=-200.;
		jetphi[i]=-200.;
		jeteta[i]=-200.;
                jete[i]=-200.;
	}

	TLorentzVector j1;
        TLorentzVector j2;
        TLorentzVector j12;
        TLorentzVector lmu;
        TLorentzVector lmet;
        TLorentzVector lwreco;
        TLorentzVector l4;


	edm::Handle<reco::VertexCollection> vertexes1;
	edm::Handle<reco::VertexCollection> vertexes2;
	iEvent.getByLabel( "offlinePrimaryVertices" , vertexes1);
	iEvent.getByLabel( "offlinePrimaryVerticesWithBS" , vertexes2);
	nPV1 = vertexes1->size(); 
	nPV2 = vertexes2->size(); 

     using pat::Muon;
     edm::Handle<std::vector<Muon> > muons;
     iEvent.getByLabel("patMuonsWithTrigger", muons);
     for(std::vector<Muon>::const_iterator p=muons->begin(); p!=muons->end(); ++p)
	{
       
          	 mupt[Nmu]=p->pt();
                 mueta[Nmu]=p->eta();
                 muphi[Nmu]=p->phi();
                 mue[Nmu]=p->energy();
                 mupz[Nmu]=p->pz();
                 Nmu++;
        }


     Handle<pat::METCollection> met;
     iEvent.getByLabel(InputTag("patMETs"), met);
     for(pat::METCollection::const_iterator pb=met->begin(); pb!= met->end(); pb++)
        {
//                cout<<"111"<<endl;

                metpt[Nmet]=pb->et();
                metphi[Nmet]=pb->phi();
                Nmet++;
        }




     Handle<pat::JetCollection> xjet;
     iEvent.getByLabel(InputTag("selectedPatJetsCHSpruned"), xjet);
    for(pat::JetCollection::const_iterator pjet=xjet->begin(); pjet!= xjet->end(); ++pjet)
       {
         jetpt[Njet]=pjet->pt();
         jetphi[Njet]=pjet->phi();
         jeteta[Njet]=pjet->eta();
         jete[Njet]=pjet->energy();
         Njet++;
       }

     if(Njet>0) {
     j1.SetPtEtaPhiE(jetpt[0],jeteta[0],jetphi[0],jete[0]);
     mjj=j1.M();
     }

     if(Njet>1) {
     j2.SetPtEtaPhiE(jetpt[1],jeteta[1],jetphi[1],jete[1]);
     j12=j1+j2;
//     mjj=j1.M();      
     }

     if(Nmu>0) {
     phitw=fabs(fabs(fabs(muphi[0]-metphi[0])-TMath::Pi())-TMath::Pi());
     mtw=sqrt(2.0*mupt[0]*metpt[0]*(1-TMath::Cos(phitw)));
     lmu.SetPtEtaPhiE(mupt[0],mueta[0],muphi[0],mue[0]);
     double ftmp1=metpt[0]/mupt[0]*mupz[0];
     double ftmp2=metpt[0]*TMath::Cos(metphi[0])*metpt[0]*TMath::Cos(metphi[0]);
     ftmp2=ftmp2+metpt[0]*TMath::Sin(metphi[0])*metpt[0]*TMath::Sin(metphi[0]);
     ftmp2=ftmp2+ftmp1*ftmp1;
     ftmp2=sqrt(ftmp2);
     lmet.SetPxPyPzE(metpt[0]*TMath::Cos(metphi[0]),metpt[0]*TMath::Sin(metphi[0]),ftmp1,ftmp2);
     lwreco=lmu+lmet;
     mwreco=lwreco.M();
     ptwreco=lwreco.Pt();
     if(Njet>0) {
     l4=lwreco+j1;
     M4=l4.M();
     }

     }

      ExTree->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
ReadPAT::beginJob()
{
    nEvt=0;
    edm:: Service<TFileService> fs; 
    ExTree = fs->make<TTree>("ExTree","ExTree");   


	ExTree->Branch("Nmu",&Nmu,"Nmu/I");
	ExTree->Branch("mupt",mupt,"mupt[Nmu]/D");
	ExTree->Branch("mueta",mueta,"mueta[Nmu]/D");
	ExTree->Branch("muphi",muphi,"muphi[Nmu]/D");
        ExTree->Branch("mue",mue,"mue[Nmu]/D");
        ExTree->Branch("Nmet",&Nmet,"Nmet/I");
	ExTree->Branch("metpt",metpt,"metpt[Nmet]/D");
        ExTree->Branch("metphi",metphi,"metphi[Nmet]/D");	
	ExTree->Branch("Njet",&Njet,"Njet/I");
	ExTree->Branch("jetpt",jetpt,"jetpt[Njet]/D");
	ExTree->Branch("jeteta",jeteta,"jeteta[Njet]/D");
	ExTree->Branch("jetphi",jetphi,"jetphi[Njet]/D");
        ExTree->Branch("jete",jete,"jete[Njet]/D");
        ExTree->Branch("mjj",&mjj,"mjj/D");
        ExTree->Branch("phitw",&phitw,"phitw/D");
        ExTree->Branch("mtw",&mtw,"mtw/D");
        ExTree->Branch("mwreco",&mwreco,"mwreco/D");
        ExTree->Branch("ptwreco",&ptwreco,"ptwreco/D");
        ExTree->Branch("M4",&M4,"M4/D");
        ExTree->Branch("nPV1",&nPV1,"nPV1/D");
        ExTree->Branch("nPV2",&nPV2,"nPV2/D");

}	

// ----------- method called once each job just after ending the event loop  ------------
void 
ReadPAT::endJob() 
{
	std::cout<<"++++++++++++++++++++++++++++++++++++++"<<std::endl;
	std::cout<<"analyzed "<<nEvt<<" events "<<std::endl;
}

// ------------ method called when starting to processes a run  ------------
void 
ReadPAT::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
ReadPAT::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
ReadPAT::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
ReadPAT::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ReadPAT::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


void ReadPAT::sortbypt(double * xpt, double * xeta, double * xphi, int Nx)
{
	for(int j=0;j<Nx;j++)
	{
		for(int k=j;k<Nx-1;k++)
		{
			if(xpt[k]<xpt[k+1])
			{
				change(xpt[k],xpt[k+1]);
				change(xeta[k],xeta[k+1]);
				change(xphi[k],xphi[k+1]);		
			}
		}
	}


}

void ReadPAT::change(double &a, double &b)
{
	double temp;
	temp = a;
	a = b;
	b= temp;
}


//define this as a plug-in
DEFINE_FWK_MODULE(ReadPAT);
