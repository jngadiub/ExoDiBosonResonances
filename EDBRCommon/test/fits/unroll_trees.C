#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TH1D.h"
#include "TF1.h"
#include "TString.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include <iostream>
using namespace std;
#include "Config_XZZ.h"

void CopyTreeVecToPlain(TChain *t1, std::string wType, std::string f2Name, std::string t2Name,int nxjCut=-1,bool ScaleTTbar=0, double samplingFactor=-1.0);


//const int nxjCut=-1;//if negative: no cut
const std::string tmpTreeName="SelectedCandidatesV2";


int unroll_trees( ) {

  std::string weighting = "weight";
 

  string inDir=inDirSIG;//"/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv2c/fullsig/";

  char foutn[64];
  std::string tmpFileName="NODIR";

  //chainMC->Add( (inDir+"treeEDBR_DYJetsPt70To100.root").c_str());
  // chainMC->Add( (inDir+"treeEDBR_DYJetsPt100.root").c_str());
  // chainMC->Add( (inDir+"treeEDBR_TTBARpowheg.root").c_str());



  int MX=600;
  while (MX<=2500){
    std::stringstream ssMX;
    ssMX << MX;
    MX+=100;
    
    std::string sampleLabel="BulkG_ZZ_lljj_c0p2_M"+ssMX.str();
    TChain* chainMC = new TChain(InTreeName.c_str());
    chainMC->Add( (inDir+"treeEDBR_"+sampleLabel+".root").c_str());
    
    gROOT->cd(); //magic!
    // sprintf(foutn,"EXOVVTree_forAlpha_SIG_%dJ.root",nxjCut);
    sprintf(foutn,"EXOVVTree_%s_SIG.root",sampleLabel.c_str());
    tmpFileName=foutn;
    //    CopyTreeVecToPlain(chainMC,weighting,tmpFileName,tmpTreeName,-1.0,false,-1.0);
    
    delete chainMC;
  }
  

  TChain* chainVJets = new TChain(InTreeName.c_str());
  chainVJets->Add( (inDirSIG+"treeEDBR_DYJetsPt70To100.root").c_str());    
  chainVJets->Add( (inDirSIG+"treeEDBR_DYJetsPt100.root").c_str());    
  chainVJets->Add( (inDirSB+"treeEDBR_DYJetsPt70To100.root").c_str());    
  chainVJets->Add( (inDirSB+"treeEDBR_DYJetsPt100.root").c_str());    
  gROOT->cd(); //magic!
  sprintf(foutn,"./ZJetsNormalization/Data/EXOVVTree_unrolled_VJets_SIG.root");
  tmpFileName=foutn;
  CopyTreeVecToPlain(chainVJets,weighting,tmpFileName,tmpTreeName,-1.0,false,-1.0);
  delete chainVJets;

  TChain* chainVV = new TChain(InTreeName.c_str());
  chainVV->Add( (inDirSIG+"treeEDBR_WW.root").c_str());    
  chainVV->Add( (inDirSIG+"treeEDBR_WZ.root").c_str());    
  chainVV->Add( (inDirSIG+"treeEDBR_ZZ.root").c_str());  
  chainVV->Add( (inDirSB+"treeEDBR_WW.root").c_str());    
  chainVV->Add( (inDirSB+"treeEDBR_WZ.root").c_str());    
  chainVV->Add( (inDirSB+"treeEDBR_ZZ.root").c_str());    
  gROOT->cd(); //magic!
  sprintf(foutn,"./ZJetsNormalization/Data/EXOVVTree_unrolled_VV_SIG.root");
  tmpFileName=foutn;
  CopyTreeVecToPlain(chainVV,weighting,tmpFileName,tmpTreeName,-1.0,false,-1.0);
  delete chainVV;


  TChain* chainTT = new TChain(InTreeName.c_str());
  chainTT->Add( (inDirSIG+"treeEDBR_TTBARpowheg.root").c_str());    
  chainTT->Add( (inDirSB+"treeEDBR_TTBARpowheg.root").c_str());    
  gROOT->cd(); //magic!
  sprintf(foutn,"./ZJetsNormalization/Data/EXOVVTree_unrolled_TT_SIG.root");
  tmpFileName=foutn;
  CopyTreeVecToPlain(chainTT,weighting,tmpFileName,tmpTreeName,-1.0,false,-1.0);
  delete chainTT;

  TChain* chainElectron = new TChain(InTreeName.c_str());
  chainElectron->Add( (inDirSIG+"treeEDBR_Photon*.root").c_str());    
  chainElectron->Add( (inDirSIG+"treeEDBR_DoublePhoton*.root").c_str());    
  chainElectron->Add( (inDirSB+"treeEDBR_Photon*.root").c_str());    
  chainElectron->Add( (inDirSB+"treeEDBR_DoublePhoton*.root").c_str());    
  gROOT->cd(); //magic!
  sprintf(foutn,"./ZJetsNormalization/Data/EXOVVTree_unrolled_Electron_SIG.root");
  tmpFileName=foutn;
  CopyTreeVecToPlain(chainElectron,weighting,tmpFileName,tmpTreeName,-1.0,false,-1.0);
  delete chainElectron;


  TChain* chainMuon = new TChain(InTreeName.c_str());
  chainMuon->Add( (inDirSIG+"treeEDBR_DoubleMu*.root").c_str());    
  chainMuon->Add( (inDirSB+"treeEDBR_DoubleMu*.root").c_str());    
  gROOT->cd(); //magic!
  sprintf(foutn,"./ZJetsNormalization/Data/EXOVVTree_unrolled_Muon_SIG.root");
  tmpFileName=foutn;
  CopyTreeVecToPlain(chainMuon,weighting,tmpFileName,tmpTreeName,-1.0,false,-1.0);
  delete chainMuon;



   return 0;
}//end main


void CopyTreeVecToPlain(TChain *t1, std::string wType, std::string f2Name,std::string t2Name,int nxjCut,bool ScaleTTbar, double samplingFactor){

  int ncands; 
  double eventWeight,lumiWeight;
  unsigned int nrun,nevt;
  double lept[35];
  int mynxj[35];
  double mZZd[35],region[35],mZqq[35],mZqqNoKF[35];
  double vTagPurity[35];
  double nsubj21[35];

  t1->SetBranchAddress("nCands",&ncands);
  t1->SetBranchAddress("run",&nrun);
  t1->SetBranchAddress("event",&nevt);
  t1->SetBranchAddress("lep",lept);
  t1->SetBranchAddress(wType.c_str(),&eventWeight);
  t1->SetBranchAddress("LumiWeight",&lumiWeight);
  t1->SetBranchAddress("mZZ",mZZd);
  t1->SetBranchAddress("nXjets",mynxj);
  t1->SetBranchAddress("mJJ",mZqq);
  t1->SetBranchAddress("mJJNoKinFit",mZqqNoKF);
  t1->SetBranchAddress("region",region);
  t1->SetBranchAddress("vTagPurity",vTagPurity);
  t1->SetBranchAddress("nsubj21",nsubj21);

  TFile *fOut=new TFile(f2Name.c_str(),"RECREATE");
  fOut->cd();

  int ncands_2, mynxj_2;
  double eventWeight_2,lumiWeight_2;
  unsigned int nrun_2,nevt_2;
  int lept_2;
  double mZZd_2,mZqq_2,mZqqNoKF_2, nsubj21_2;
  int vTagPurity_2,region_2;

  TTree *t2=new TTree(t2Name.c_str(),t2Name.c_str());

  t2->Branch("nCands",&ncands_2,"nCands/I");
  t2->Branch("run",&nrun_2,"run/i");
  t2->Branch("event",&nevt_2,"event/i");
  t2->Branch("weight",&eventWeight_2,"weight/D");
  t2->Branch("lumiweight",&lumiWeight_2,"lumiweight/D");
  t2->Branch("nXjets",&mynxj_2 , "nXjets/I");
  t2->Branch("lep",&lept_2,"lep/I");
  t2->Branch("mZZ",&mZZd_2 , "mZZ/D");
  t2->Branch("mJJ",&mZqq_2 , "mJJ/D");
  t2->Branch("mJJNoKinFit",&mZqqNoKF_2, "mJJNoKinFit/D");
  t2->Branch("region",&region_2 , "region/I");
  t2->Branch("nsubj21",&nsubj21_2, "nsubj21/D");
  t2->Branch("vTagPurity",&vTagPurity_2, "vTagPurity/I");

  TRandom3 *rndm1=new TRandom3(13579);

  for(int i=0;i<t1->GetEntries();i++){

    t1->GetEntry(i);

    if(samplingFactor>=0.0){
      if(rndm1->Rndm()>samplingFactor)continue;
    }

    for(int j=0;j<ncands;j++){
      ncands_2=ncands;
      nrun_2=nrun;
      nevt_2=nevt;
      eventWeight_2=eventWeight;
      lumiWeight_2=lumiWeight;
      lept_2=int(lept[j]);
      mZZd_2=mZZd[j];
      mZqq_2=mZqq[j];
      mZqqNoKF_2=mZqqNoKF[j];
      region_2=int(region[j]);
      if(isZZChannel&&(mynxj_2==1&&mZqqNoKF_2>110.0))region_2=2;//upper sideband kept separated
      mynxj_2=int(mynxj[j]);
      nsubj21_2=nsubj21[j]; 
      vTagPurity_2=int(vTagPurity[j]); 

      //if(nsubj21_2>0.45)continue;


      //now add ttbar scale factor to t1, and only to ttbar 
      if(ScaleTTbar)
	{
	  double ttbar_scale =1;//default no scale
				//double tttar_scale_error=0;

				//for the analysis
	  if(InTreeName=="SelectedCandidates")
	    {
	      if(vTagPurity_2==0&&lept_2==0)ttbar_scale = 1.297481;//eleLP
	      if(vTagPurity_2==1&&lept_2==0)ttbar_scale = 0.966417;//eleHP
	      if(vTagPurity_2==0&&lept_2==1)ttbar_scale = 1.226198;//muLP
	      if(vTagPurity_2==1&&lept_2==1)ttbar_scale = 0.958444;//muHP
	    }
	  else if (InTreeName=="SelectedCandidatesAB")
	    {
	      //for closure test A->B
	      if(vTagPurity_2==0&&lept_2==0)ttbar_scale = 1.296917;//eleLP
	      if(vTagPurity_2==1&&lept_2==0)ttbar_scale = 0.957699;//eleHP
	      if(vTagPurity_2==0&&lept_2==1)ttbar_scale = 1.208456;//muLP
	      if(vTagPurity_2==1&&lept_2==1)ttbar_scale = 1.046983;//muHP
	    }

	  TString filename = t1->GetFile()->GetEndpointUrl()->GetUrl();
	  //cout<<filename<<endl;
	  if(filename.Contains("TTBAR")||filename.Contains("SingleTop"))
	    {
	      //cout<<filename<<endl;
	      eventWeight_2 =eventWeight_2*ttbar_scale;
	    }
	}

      if(region[j]<0||mZZd_2>9999.0||mynxj_2>10||mZqq_2>999.0){
	//cout<<"Event="<<nevt<<" cand="<<j<<" has reg="<<region[j]<<" mZZ="<<mZZd_2<<endl;
	continue;
      }

      if(mynxj_2==nxjCut||nxjCut<0) {
	t2->Fill();
	//	if(i%1000==1)cout<<"Filled "<<nb<<" bytes"<<endl;
	//	if(nb<0)cout<<"====>  Event"<<nevt_2 <<"  Filled "<<nb<<" bytes"<<endl;
      }
    }//end loop on j -> nCands

  }//end loop on i -> entries of input tree

  cout<<"returning t2 with "<<t2->GetEntries()<<" entries"<<endl;




  //  cout<<"Writing unrolled tree"<<endl;
  // t2->Write();
  fOut->Write();
  delete t2;
  delete fOut;
  delete rndm1;

  //  cout<<"returning"<<endl;
}
