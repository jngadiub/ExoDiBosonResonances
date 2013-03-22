
#include <string>
#include <Riostream.h>
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"

void createTreesClosureTest(){

  // string inDirSB="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1c/AK7/fullsb/";
  //string outDirSIG="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1c/AK7/closureA1toA2_SIG/";
  //string outDirSB="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1c/AK7/closureA1toA2_SB/";

//thiago's files:
 string inDirSB="/afs/cern.ch/user/t/tomei/work/public/EXOVV_2012/analyzer_trees/productionv5/fullsidebandCA8/";
 //output dirs must already exist
 string outDirSIG="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv5b/CA8/closureA1toA2_SIG/";
 string outDirSB="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv5b/CA8/closureA1toA2_SB/";

 //to be done for all data and MC samples:
 const int Nsamples=16;
  string sample[Nsamples]={"DYJetsPt100","TTBAR","WW","WZ","ZZ",
		   "DoubleMu_Run2012A_13Jul2012","DoubleMu_Run2012A_recover","DoubleMu_Run2012B_13Jul2012",
		   "DoubleMu_Run2012C_24Aug2012","DoubleMu_Run2012C_PRv2","DoubleMu_Run2012D_PRv1",
		   "Photon_Run2012A_13Jul2012","Photon_Run2012A_recover","DoublePhotonHighPt_Run2012B_13Jul2012",
		   "DoublePhotonHighPt_Run2012C_24Aug2012","DoublePhotonHighPt_Run2012D_PRv1"};

  for(int isample=0;isample<Nsamples;isample++){
    string treeName="treeEDBR_"+sample[isample]+".root";
    
    cout<<"Processing "<<treeName.c_str()<<endl;
    TFile *fIn=new TFile((inDirSB+treeName).c_str(),"READ");
    TTree *tIn=(TTree*)fIn->Get("SelectedCandidates");
    // Declaration of leaf types
    int    nCands;
    unsigned int nevent, run;
    double lep[99],region[99],mZZ[99],mJJ[99],mLL[99],nsubj21[99], prMJ[99];
    double vTagPurity[99];
    int nXjets[99];
    double PUweight, LumiWeight,weight;
    
    TBranch        *b_nCands;   //!
    TBranch        *b_event;   //!
    TBranch        *b_run;   //!
    TBranch        *b_lep;  TBranch        *b_region;   //!
    TBranch        *b_nXjets; TBranch        *b_mZZ; TBranch *b_vTagPurity;  //!
    TBranch        *b_prunedmass;	TBranch        *b_mLL;TBranch        *b_mJJ;
    TBranch        *b_nsubj21;  TBranch        *b_PUweight; 	TBranch        *b_LumiWeight; 
    TBranch        *b_weight;
    
    tIn->SetBranchAddress("nCands", &nCands, &b_nCands);
    tIn->SetBranchAddress("run", &run, &b_run);
    tIn->SetBranchAddress("event", &nevent, &b_event);
    tIn->SetBranchAddress("lep", lep, &b_lep);
    tIn->SetBranchAddress("region", region, &b_region);
    tIn->SetBranchAddress("mZZ", mZZ, &b_mZZ);
    tIn->SetBranchAddress("mLL", mLL, &b_mLL);
    tIn->SetBranchAddress("mJJ", mJJ, &b_mJJ);
    tIn->SetBranchAddress("nXjets", nXjets, &b_nXjets);
    tIn->SetBranchAddress("vTagPurity", vTagPurity, &b_vTagPurity);
    tIn->SetBranchAddress("prunedmass", prMJ, &b_prunedmass);
    tIn->SetBranchAddress("nsubj21", nsubj21, &b_nsubj21);
    tIn->SetBranchAddress("PUweight", &PUweight, &b_PUweight);
    tIn->SetBranchAddress("LumiWeight", &LumiWeight, &b_LumiWeight);
    tIn->SetBranchAddress("weight", &weight, &b_weight);
    
    
    TFile *fOutSB=new TFile((outDirSB+treeName).c_str(),"RECREATE");
    TTree *tSB=new TTree("SelectedCandidatesSB","SelectedCandidatesSB");
    double regionSB[99];
    int    nCandsSB;
    unsigned int neventSB, runSB;
    double lepSB[99],mZZSB[99],mJJSB[99];
    double mLLSB[99],nsubj21SB[99],prMJSB[99], vTagPuritySB[99];
    int nXjetsSB[99];
    double PUweightSB, LumiWeightSB,weightSB;
    tSB->Branch("nCands" , &nCandsSB , "nCands/I");
    tSB->Branch("event"           ,&neventSB        ,"event/i");
    tSB->Branch("run"             ,&runSB           ,"run/i");
    tSB->Branch("lep"             ,&lepSB           ,"lep[nCands]/D"   );
    tSB->Branch("region"             ,&regionSB           ,"reg[nCands]/D"   );
    tSB->Branch("nXjets"          ,&nXjetsSB        ,"nXjets[nCands]/I");
    tSB->Branch("vTagPurity"          ,&vTagPuritySB        ,"vTagPurity[nCands]/I");
    tSB->Branch("mZZ"             ,&mZZSB           ,"mZZ[nCands]/D"   );
    tSB->Branch("mLL"             ,&mLLSB           ,"mLL[nCands]/D"  );
    tSB->Branch("mJJ"             ,&mJJSB           ,"mJJ[nCands]/D");
    tSB->Branch("prunedmass"      , &prMJSB, "prunedmass[nCands]/D");
    tSB->Branch("nsubj21"       ,&nsubj21SB    ,"nsubj21[nCands]/D");
    tSB->Branch("PUweight"        ,&PUweightSB            ,"PUweight/D" );
    tSB->Branch("LumiWeight"      ,&LumiWeightSB         ,"LumiWeight/D"  );
    tSB->Branch("weight"          ,&weightSB             ,"weight/D");
    //  tSB->Branch();
    
    
    TFile *fOutSIG=new TFile((outDirSIG+treeName).c_str(),"RECREATE");
    TTree *tSIG=new TTree("SelectedCandidatesSIG","SelectedCandidatesSIG");
    double regionSIG[99];
    int    nCandsSIG;
    unsigned int neventSIG, runSIG;
    double lepSIG[99],mZZSIG[99],mJJSIG[99];
    double mLLSIG[99],nsubj21SIG[99],prMJSIG[99], vTagPuritySIG[99];
    int nXjetsSIG[99];
    double PUweightSIG, LumiWeightSIG,weightSIG;
    
    tSIG->Branch("nCands" , &nCandsSIG , "nCands/I");
    tSIG->Branch("event"           ,&neventSIG        ,"event/i");
    tSIG->Branch("run"             ,&runSIG           ,"run/i");
    tSIG->Branch("lep"             ,&lepSIG          ,"lep[nCands]/D"   );
    tSIG->Branch("region"             ,&regionSIG      ,"reg[nCands]/D"   );
    tSIG->Branch("nXjets"          ,&nXjetsSIG        ,"nXjets[nCands]/I");    
    tSIG->Branch("vTagPurity"   ,&vTagPuritySIG        ,"vTagPurity[nCands]/I");
    tSIG->Branch("mZZ"             ,&mZZSIG           ,"mZZ[nCands]/D"   );
    tSIG->Branch("mLL"             ,&mLLSIG           ,"mLL[nCands]/D"  );
    tSIG->Branch("mJJ"             ,&mJJSIG           ,"mJJ[nCands]/D");
    tSIG->Branch("prunedmass"      , &prMJSIG, "prunedmass[nCands]/D");
    tSIG->Branch("nsubj21"       ,&nsubj21SIG    ,"nsubj21[nCands]/D");
    tSIG->Branch("PUweight"        ,&PUweightSIG            ,"PUweight/D" );
    tSIG->Branch("LumiWeight"      ,&LumiWeightSIG          ,"LumiWeight/D"  );
    tSIG->Branch("weight"          ,&weightSIG             ,"weight/D");
    //  tSIG->Branch();
    
    
    int ncSB=0,ncSIG=0,ncTOT=0, nc1JTOT=0;
    const double SBNewLow=0.0;
    const double SBNewHigh=60.0;
    const double SIGNewLow=60.0;
    const double SIGNewHigh=75.0;

    for(int i=0;i<tIn->GetEntries();i++){
      tIn->GetEntry(i);
      nCandsSB=0;
      nCandsSIG=0;
      // if(i%1000==0){cout<<"nCands=="<<nCands<<flush;
      //  if(nCands>0)cout<<" Pruned-Mass="<<prMJ[0]<<"  "<<flush;
      // }
      bool filledSB=false;
      bool filledSIG=false;

      for(int j=0;j<nCands;j++){
	ncTOT++;
	if(nXjets[j]>1)continue;//don't save 2-jet cands
	nc1JTOT++;
	//cut on n-subjettiness
	//	if( (1.0/nsubj21[j]) > 0.4 )continue;

	if(prMJ[j]<SBNewHigh){
	  neventSB=nevent;
	  runSB=run;
	  lepSB[nCandsSB]=lep[j];
	  nXjetsSB[nCandsSB]=nXjets[j];
	  regionSB[nCandsSB]=0.0;
	  mZZSB[nCandsSB]=mZZ[j];
	  mLLSB[nCandsSB]=mLL[j];
	  mJJSB[nCandsSB]=prMJ[j];
	  prMJSB[nCandsSB]=prMJ[j];
	  nsubj21SB[nCandsSB]=nsubj21[j];
	  vTagPuritySB[nCandsSB]=vTagPurity[j];
	  PUweightSB=PUweight;
	  LumiWeightSB=LumiWeight;
	  weightSB=weight;
	  
	//	if(i%1000==0)cout<<" -> 1-Jet event, SB-reg, mJJ="<<mJJSB[nCandsSB]<<endl;
	  nCandsSB++;
	  filledSB=true;
	  ncSB++;

	}//end if mjj<75
	else if(prMJ[j]>SIGNewLow&&prMJ[j]<SIGNewHigh){
	  
	  neventSIG=nevent;
	  runSIG=run;
	  lepSIG[nCandsSIG]=lep[j];
	  nXjetsSIG[nCandsSIG]=nXjets[j];
	  regionSIG[nCandsSIG]=1.0;
	  mZZSIG[nCandsSIG]=mZZ[j];
	  mLLSIG[nCandsSIG]=mLL[j];
	  mJJSIG[nCandsSIG]=prMJSIG[j];
	  prMJSIG[nCandsSIG]=prMJ[j];
	  nsubj21SIG[nCandsSIG]=nsubj21[j];
	  vTagPuritySIG[nCandsSIG]=vTagPurity[j];
	  PUweightSIG=PUweight;
	  LumiWeightSIG=LumiWeight;
	  weightSIG=weight;
	  
	  //	if(i%1000==0)cout<<" -> 1-Jet event, SIGreg, mJJ="<<mJJSIG[nCandsSIG]<<endl;
	  nCandsSIG++;
	  filledSIG=true;	  
	  
	  neventSB=nevent;
	  runSB=run;
	  lepSB[nCandsSB]=lep[j];
	  nXjetsSB[nCandsSB]=nXjets[j];
	  regionSB[nCandsSB]=1.0;
	  mZZSB[nCandsSB]=mZZ[j];
	  mLLSB[nCandsSB]=mLL[j];
	  mJJSB[nCandsSB]=prMJ[j];
	  prMJSB[nCandsSB]=prMJ[j];
	  nsubj21SB[nCandsSB]=nsubj21[j];
	  PUweightSB=PUweight;
	  LumiWeightSB=LumiWeight;
	  weightSB=weight;
	  
	  //	if(i%1000==0)cout<<" -> 1-Jet event, SB-reg, mJJ="<<mJJSB[nCandsSB]<<endl;
	  nCandsSB++;
	  filledSB=true;
	  ncSIG++;
	}
	//      else{	
	//	cout<<"ERROR !!! This event should be in sideband. MJJ="<<mJJ[j]<<endl;
	// }

  
      }//end loop on ncands

      if(filledSB)   tSB->Fill();
      if(filledSIG) tSIG->Fill();
	

    }//end loop on entries
    
    cout<<"Out of loops. #entries in TOT="<<tIn->GetEntries()<<"  New-SB="<<tSB->GetEntries()<<"  New-SIG="<<tSIG->GetEntries()<<endl;
    cout<<"#CandsTOT="<<ncTOT<<" #Cands1JTOT="<<nc1JTOT<<"  #CandsSB="<<ncSB<<"  #CandsSIG="<<ncSIG<<endl;
    fOutSB->cd();
  tSB->SetName("SelectedCandidates");
  tSB->Write();
  cout<<"deleting SB  "<<flush;
  delete tSB;
  delete fOutSB;


  fOutSIG->cd();
  tSIG->SetName("SelectedCandidates");
  tSIG->Write();
  cout<<"deleting SIG"<<endl;
  delete tSIG;
  delete fOutSIG;

  delete tIn;
  delete fIn;

  }//end loop on samples


}
