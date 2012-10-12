// -*- C++ -*-
//
// Package:    DiVMC
// Class:      DiVMC
// 
/**\class DiVMC DiVMC.cc DiVCheckMC/DiVMC/src/DiVMC.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
 */
//
// Original Author:  qiang li
//         Created:  Wed Oct  3 14:32:23 CDT 2012
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


//#include "DataFormats/PatCandidates/interface/Muon.h"
//#include "DataFormats/PatCandidates/interface/MET.h"
//#include "DataFormats/PatCandidates/interface/Jet.h"

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

class DiVMC : public edm::EDAnalyzer {
	public:
		explicit DiVMC(const edm::ParameterSet&);
		~DiVMC();

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
		//	std::string theRootFileName;
		//	TFile *theRootFile;
		TTree *ExTree;
		int nEvt;  

		int nPV1;
		int nPV2;
		int nPV3;

		int Nid;
		int pdgID[NMAX];

		double wmgenpt,wmgeneta,wmgenphi;
		double wpgenpt,wpgeneta,wpgenphi;
		double mww,phiww,ptww;
		double wppx,wppy,wppz,wpm,wpe,wmpx,wmpy,wmpz,wmm,wme;
		double grapt,gram,graphi;
		int Nmu;
		double mupt[NMAX];
		double mueta[NMAX];
		double muphi[NMAX];

		int Nele;
		double elept[NMAX];
		double eleeta[NMAX];
		double elephi[NMAX];  

		int Nmet;
		double metpt[NMAX];
		double metphi[NMAX];

		int Njet;
		double jetpt[NMAX];
		double jetphi[NMAX];
		double jeteta[NMAX];

		double lep_p_pt;
		double lep_p_eta;
		double lep_p_phi;
		double lep_p_e;
        double lep_m_pt;
        double lep_m_eta;
        double lep_m_phi;
        double lep_m_e;
		double lep2m;

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
DiVMC::DiVMC(const edm::ParameterSet& iConfig)

{
	//now do what ever initialization is needed
	//   theRootFileName = iConfig.getUntrackedParameter<string>("rootFileName");
}


DiVMC::~DiVMC()
{

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
	void
DiVMC::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

	nEvt++;
	Nid = 0;
	Nmu = 0;
	Nele  = 0;
	Nmet = 0;
	Njet  = 0;

	wmgenpt=-200.;
	wmgeneta=-200.;
	wmgenphi=-200.;
	wpgenpt=-200.;
	wpgeneta=-200.;
	wpgenphi=-200.;
	wppx=-200.;
	wppy=-200.;
	wppz=-200.;
	wpm=-200.;
	wpe=-200.;
	wmpx=-200.;
	wmpy=-200.;
	wmpz=-200.;
	wmm=-200.;
	wme=-200.;
	mww=-200.;
	ptww=-200.;
	phiww=-200.;

	for(int i=0; i<NMAX; ++i) {
		pdgID[i]=-200;
		mupt[i]=-200.;
		mueta[i]=-200.;
		muphi[i]=-200.;
		elept[i]=-200.;
		eleeta[i]=-200.;
		elephi[i]=-200.;
		metpt[i]=-200.;
		metphi[i]=-200.;
		jetpt[i]=-200.;
		jetphi[i]=-200.;
		jeteta[i]=-200.;
	}

	TLorentzVector wp;
	TLorentzVector wm;
	TLorentzVector ww;
	TLorentzVector lp;
	TLorentzVector lm;
	TLorentzVector l2;

	int lep_p_number=0;
	int lep_m_number=0;

	Handle<reco::GenParticleCollection> genParticles;//define genParticle
	iEvent.getByLabel(InputTag("genParticles"), genParticles);

	/*----this metod also works
	  edm::Handle <edm::View<reco::Vertex> > recVtxs;
	//iEvent.getByLabel( "goodOfflinePrimaryVertices" , recVtxs);//-----------------------this is wrong
	iEvent.getByLabel( "offlinePrimaryVertices" , recVtxs);
	nPV = recVtxs->size();
	cout<<nPV<<endl;
	 */

	edm::Handle<reco::VertexCollection> vertexes1;
	edm::Handle<reco::VertexCollection> vertexes2;
	edm::Handle<reco::VertexCollection> vertexes3;
	iEvent.getByLabel( "offlinePrimaryVertices" , vertexes1);
	iEvent.getByLabel( "offlinePrimaryVerticesWithBS" , vertexes2);
	//iEvent.getByLabel( "goodOfflinePrimaryVertices" , vertexes3);

	nPV1 = vertexes1->size(); 
	nPV2 = vertexes2->size(); 
	//nPV3 = vertexes3->size(); 
	//cout<<nPV1<<endl;
	//cout<<nPV2<<endl;

	for(reco::GenParticleCollection::const_iterator p=genParticles->begin(); p!= genParticles->end(); ++p)
	{
		pdgID[Nid]=p->pdgId();
		Nid++;

		if((p->pdgId()==39||p->pdgId()==5000039)&&p->status()==3)//graviton
		{
			//cout<<p->pdgId()<<endl;
			//cout<<p->status()<<endl;
			//cout<<p->pt()<<endl;
			grapt=p->pt();
			gram=p->mass();
			graphi=p->phi();

			for(int i=0;p->daughter(i)!=NULL;i++)//loop on graviton daughter
			{
				if(abs(p->daughter(i)->pdgId())==24)
				{
					const reco::Candidate* pw = p->daughter(i);

					if(pw->pdgId()==24&&pw->status()==3)//w+
					{  
						wpgenpt=pw->pt();
						wpgeneta=pw->eta();
						wpgenphi=pw->phi();                 
						wppx=pw->px();
						wppy=pw->py();
						wppz=pw->pz();
						wpm=pw->mass();
						wpe=pw->energy();
						wp.SetPtEtaPhiE(wpgenpt, wpgeneta,wpgenphi,wpe);
						//cout<<pw->mother()->pdgId()<<endl;
					}

					if(pw->pdgId()==-24&&pw->status()==3)//w-
					{
						wmgenpt=pw->pt();
						wmgeneta=pw->eta();
						wmgenphi=pw->phi();
						wmpx=pw->px();
						wmpy=pw->py();
						wmpz=pw->pz();
						wmm=pw->mass();
						wme=pw->energy();
						wm.SetPtEtaPhiE(wmgenpt, wmgeneta,wmgenphi,wme);
						//cout<<pw->mother()->pdgId()<<endl;
					}

					for(int i=0;pw->daughter(i)!=NULL;i++)//loop on w daughter
					{
						const reco::Candidate* pl = pw->daughter(i);
						if(pl->pdgId()==11||pl->pdgId()==13||pl->pdgId()==15)
						{
							lep_p_number++;
							lep_p_pt=pl->pt();
							lep_p_eta=pl->eta();
							lep_p_phi=pl->phi();
							lep_p_e=pl->energy();
							lp.SetPtEtaPhiE(lep_p_pt,lep_p_eta,lep_p_phi,lep_p_e);
						}
                       if(pl->pdgId()==-11||pl->pdgId()==-13||pl->pdgId()==-15)
                        {   
                            lep_m_number++;
                            lep_m_pt=pl->pt();
                            lep_m_eta=pl->eta();
                            lep_m_phi=pl->phi();
                            lep_m_e=pl->energy();
							lm.SetPtEtaPhiE(lep_m_pt,lep_m_eta,lep_m_phi,lep_m_e);
                        }   

					}//end of w daugter loop

				}//end of if w
			}//end of graviton daughter loop
		}//end of graviton
		//                if(p->pdgId()==39/*&&p->status()==3*/)
		//                {
		//                 mww=p->pt();
		//                 ptww=p->eta();
		//                 phiww=p->phi();
		//                }

	}//end of genparticle loop
	ww = wp+wm;
	/*
	   mww=(wme+wpe)*(wme+wpe);
	   mww=mww-(wmpx+wppx)*(wmpx+wppx);
	   mww=mww-(wmpy+wppy)*(wmpy+wppy);
	   mww=mww-(wmpz+wppz)*(wmpz+wppz);
	   if (mww>0) {
	   mww=sqrt(mww);
	   ptww=sqrt((wmpx+wppx)*(wmpx+wppx)+(wmpy+wppy)*(wmpy+wppy));  
	//phiww=(wmpx*wppx+wmpy*wppy)/wmgenpt/wpgenpt;
	phiww= atan((wmpy+wppy)/(wmpx+wppx));
	if((wmpx+wppx)<0&&(wmpy+wppy)>0)phiww=3.141+phiww;
	else if((wmpx+wppx)<0&&(wmpy+wppy)<0)phiww=-3.141+phiww;
	}
	cout<<"-------------"<<endl;
	cout<<mww<<" "<<ww.M()<<endl;
	cout<<ptww<<" "<<ww.Pt()<<endl;
	cout<<phiww<<" "<<ww.Phi()<<endl;
	 */
	mww=ww.M();
	ptww=ww.Pt();
	//cout<<ptww<<endl;
	phiww=ww.Phi();
	
	if(lep_p_number==1&&lep_m_number==1)
	{
		l2=lp+lm;
		lep2m=l2.M();
	}
	else lep2m=0;



	Handle<reco::MuonCollection> mus;
	iEvent.getByLabel("muons",mus);

	Handle<reco::GsfElectronCollection> eles;
	iEvent.getByLabel("gsfElectrons",eles);

	Handle<reco::PFMETCollection> mets ;
	iEvent.getByLabel("pfMet",mets);

	Handle<reco::PFJetCollection> jets;
	iEvent.getByLabel("ak5PFJets",jets);


	for(reco::MuonCollection::const_iterator p=mus->begin(); p!=mus->end(); ++p)
	{
		mupt[Nmu]=p->pt();
		mueta[Nmu]=p->eta();  
		muphi[Nmu]=p->phi();
		Nmu++;
	}
	sortbypt(mupt,mueta,muphi,Nmu);

	for(reco::GsfElectronCollection::const_iterator p=eles->begin(); p!=eles->end(); ++p)
	{
		elept[Nele]=p->pt();
		eleeta[Nele]=p->eta(); 
		elephi[Nmu]=p->phi();
		Nele++;
	}
	sortbypt(elept,eleeta,elephi,Nele);


	for(reco::PFMETCollection::const_iterator p=mets->begin(); p!=mets->end(); ++p)
	{
		metpt[Nmet]=p->pt();
		metphi[Nmet]=p->phi();		
		Nmet++;	
	}

	for(reco::PFJetCollection::const_iterator p=jets->begin(); p!=jets->end(); ++p)
	{
		jetpt[Njet]=p->pt();
		jetphi[Njet]=p->phi();
		jeteta[Njet]=p->eta();
		Njet++;  
	}
	sortbypt(jetpt,jeteta,jetphi,Njet);

//	cout<<lep_p_number<<endl;
//	cout<<lep_p_number<<endl;
	if(lep_p_number==1&&lep_m_number==1)
	ExTree->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
	void 
DiVMC::beginJob()
{
	nEvt=0;

	//        theRootFile = new TFile(theRootFileName.c_str(),"recreate");
	//        ExTree = new TTree("ExTree","ExTree");

	edm:: Service<TFileService> fs; 
	ExTree = fs->make<TTree>("ExTree","ExTree");

	ExTree->Branch("Nid",&Nid,"Nid/I");
	ExTree->Branch("nPV1",&nPV1,"nPV1/I");
	ExTree->Branch("nPV2",&nPV2,"nPV2/I");
	ExTree->Branch("nPV3",&nPV3,"nPV3/I");
	ExTree->Branch("pdgID",pdgID,"pdgID[Nid]/I");
	ExTree->Branch("wpgenpt",&wpgenpt,"wpgenpt/D");
	ExTree->Branch("wpgeneta",&wpgeneta,"wpgeneta/D");
	ExTree->Branch("wpgenphi",&wpgenphi,"wpgenphi/D");
	ExTree->Branch("wmgenpt",&wmgenpt,"wmgenpt/D");
	ExTree->Branch("wmgeneta",&wmgeneta,"wmgeneta/D");
	ExTree->Branch("wmgenphi",&wmgenphi,"wmgenphi/D");
	ExTree->Branch("mww",&mww,"mww/D");
	ExTree->Branch("ptww",&ptww,"ptww/D");
	ExTree->Branch("phiww",&phiww,"phiww/D");
	ExTree->Branch("gram",&gram,"gram/D");
	ExTree->Branch("grapt",&grapt,"grapt/D");
	ExTree->Branch("graphi",&graphi,"graphi/D");
	ExTree->Branch("Nmu",&Nmu,"Nmu/I");
	ExTree->Branch("mupt",mupt,"mupt[Nmu]/D");
	ExTree->Branch("mueta",mueta,"mueta[Nmu]/D");
	ExTree->Branch("muphi",muphi,"muphi[Nmu]/D");
	ExTree->Branch("Nele",&Nele,"Nele/I");
	ExTree->Branch("elept",elept,"elept[Nele]/D");
	ExTree->Branch("eleeta",eleeta,"eleeta[Nele]/D");
	ExTree->Branch("elephi",elephi,"elephi[Nele]/D");
	ExTree->Branch("Nmet",&Nmet,"Nmet/I");
	ExTree->Branch("metpt",metpt,"metpt[Nmet]/D");
	ExTree->Branch("metphi",metphi,"metphi[Nmet]/D");	
	ExTree->Branch("Njet",&Njet,"Njet/I");
	ExTree->Branch("jetpt",jetpt,"jetpt[Njet]/D");
	ExTree->Branch("jeteta",jeteta,"jeteta[Njet]/D");
	ExTree->Branch("jetphi",jetphi,"jetphi[Njet]/D");
	
	ExTree->Branch("lep_p_pt",&lep_p_pt,"lep_p_pt/D");
	ExTree->Branch("lep_p_eta",&lep_p_eta,"lep_p_eta/D");
	ExTree->Branch("lep_p_phi",&lep_p_phi,"lep_p_phi/D");
	ExTree->Branch("lep_p_e",&lep_p_e,"lep_p_e/D");
    ExTree->Branch("lep_m_pt",&lep_m_pt,"lep_m_pt/D");
    ExTree->Branch("lep_m_eta",&lep_m_eta,"lep_m_eta/D");
    ExTree->Branch("lep_m_phi",&lep_m_phi,"lep_m_phi/D");
    ExTree->Branch("lep_m_e",&lep_m_e,"lep_m_e/D");
    ExTree->Branch("lep2m",&lep2m,"lep2m/D");

}

// ------------ method called once each job just after ending the event loop  ------------
	void 
DiVMC::endJob() 
{
	std::cout<<"++++++++++++++++++++++++++++++++++++++"<<std::endl;
	std::cout<<"analyzed "<<nEvt<<" events "<<std::endl;
	//        ExTree->Write();
	//        theRootFile->Close();
}

// ------------ method called when starting to processes a run  ------------
	void 
DiVMC::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
	void 
DiVMC::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
	void 
DiVMC::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
	void 
DiVMC::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DiVMC::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

void DiVMC::sortbypt(double * xpt, double * xeta, double * xphi, int Nx)
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

void DiVMC::change(double &a, double &b)
{
	double temp;
	temp = a;
	a = b;
	b= temp;
}

//define this as a plug-in
DEFINE_FWK_MODULE(DiVMC);
