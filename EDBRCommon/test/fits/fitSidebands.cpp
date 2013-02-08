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

#include "SidebandFitter.h"

void CopyTreeVecToPlain(TTree *t1, std::string wType, std::string f2Name, std::string t2Name,int nxjCut=-1);
void doAlpha(TTree *chMC, std::string wType);

int main( int argc, char* argv[] ) {

  std::string weighting = "weight";
  string inDir="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1/thiago20130206/";
  
  TChain* chainMC = new TChain("SelectedCandidates");
  std::string bgTreeName;
  chainMC->Add( (inDir+"treeEDBR_DYJetsPt70To100.root").c_str());
  chainMC->Add( (inDir+"treeEDBR_DYJetsPt100.root").c_str());
  chainMC->Add( (inDir+"treeEDBR_TTBAR.root").c_str());
  chainMC->Add( (inDir+"treeEDBR_ZZ.root").c_str());
  gROOT->cd(); //magic!

  const int nxjCut=-1;//if negative: no cut
  const std::string tmpTreeName="SelectedCandidatesV2";
  char foutn[64];
  if(nxjCut>=0)  sprintf(foutn,"EXOVVTree_forAlpha_%d.root",nxjCut);
  else   sprintf(foutn,"EXOVVTree_forAlpha_NOcut.root");
  std::string tmpFileName=foutn;

  CopyTreeVecToPlain(chainMC,weighting,tmpFileName,tmpTreeName);//(TTree*)

  delete chainMC; //delete chainData;
  
  TFile *ftree=new TFile(foutn,"READ");
  TTree *tTmp=(TTree*)ftree->Get(tmpTreeName.c_str());
  doAlpha(tTmp,weighting);

  return 0;

}//end main

void doAlpha(TTree *chMC, std::string wType){
  unsigned int nToys = 500;
  
  TRandom3* randomGen = new TRandom3(13);

  ///loop on different topologies/categories: the code
  //will performa a separate bkg estimation for each of them
  for( unsigned inxj=1; inxj<=2; ++inxj ) {
    
    SidebandFitter *sf = new SidebandFitter( wType);
    std::string myOutDir="FitSidebandsMJJ_TEST1";
    sf->setOutDir(myOutDir);
 
    int nxjCut=inxj;
    int nentriesTOT=chMC->GetEntries();
    std::cout<<"Cutting nXjets=="<<nxjCut<<" on a chain with "<< nentriesTOT<<" entries"<<std::endl;
 
    int nxjOld;
    chMC->SetBranchAddress("nXjets",&nxjOld);
    TTree* treeMC_nxj=(TTree*)chMC->CloneTree(0);
    for (Int_t iOld=0;iOld<nentriesTOT; iOld++) {
      chMC->GetEntry(iOld);
      if(nxjOld==nxjCut)treeMC_nxj->Fill();
    }

    std::cout<<"Cut applied: "<<treeMC_nxj->GetEntries()<< " entries remain"<<std::endl;
    string outFileName;
    string leptStr="ALL";
    std::stringstream ss;
    ss << inxj;
 
    outFileName=myOutDir+"/Workspaces_alpha_"+ss.str()+"J_"+leptStr+".root";
    sf->setOutFile(outFileName);
    sf->setCanvasLabel("_Madgraph");
    RooWorkspace* alpha_nxj = sf->getAlphaFit( inxj, leptStr, treeMC_nxj ,true);

   

    // now estimate stat errors by throwing toys
    // 1: get the histos produced before and saved in the output file
    cout<<"\n*** Throwing toys for category "<<inxj<<"Jet ***"<<endl<<endl;
    outFileName=myOutDir+"/Workspaces_alpha_"+ss.str()+"J_"+leptStr+".root";
    TFile *fWS=new TFile(outFileName.c_str(),"UPDATE");
    // fWS->ls();
    TH1D *myalpha=(TH1D*)fWS->Get("h_alpha_smoothened");
  
    /*
    outFileName=myOutDir+"/Workspaces_alpha_"+ss.str()+"btag_"+leptStr+".root";
    TFile *fWS=new TFile(outFileName.c_str(),"UPDATE");
    // fWS->ls();
    TH1D *myalpha=(TH1D*)fWS->Get("h_alpha_smoothened");
       cout<<"Fillin averaged histo"<<endl;
    for(int i=1 ; i <= halfdiff->GetNbinsX() ; i++) {
      double valSh=myalphaSh->GetBinContent(i);
      double errSh=myalphaSh->GetBinError(i);
      double valMd=myalphaMd->GetBinContent(i);
      double errMd=myalphaMd->GetBinError(i);
      double wNorm =1.0/(errSh*errSh) + 1.0/(errMd*errMd);
      double wSh=  1.0/(errSh*errSh)  ;
      double wMd=1.0/(errMd*errMd);
      halfdiff->SetBinContent(i,fabs(valSh-valMd)/2.);
      myalpha->SetBinContent(i,(wSh/wNorm)*valSh+(wMd/wNorm)*valMd);
      double err2=(wSh/wNorm)*(wSh/wNorm)*errSh*errSh + (wMd/wNorm)*(wMd/wNorm)*errMd*errMd;
      myalpha->SetBinError(i,sqrt(err2));
    }
    */
    TCanvas *calphaAVG=new TCanvas("c_avg","CANVAS with Alpha histo",800,800);
    calphaAVG->cd();
    std::vector<double> avgpars;
    std::vector<double> avgerrs;
    sf->alphaFit( myalpha , avgpars,avgerrs);
    myalpha->SetMarkerStyle(20);
    myalpha->SetMarkerColor(kBlue);
    myalpha->Draw("PE0");
    char canvasName[400];
    sprintf( canvasName, "%s/mZZ_alpha_%dJ_%s.eps", myOutDir.c_str(), inxj, "ALL");
    calphaAVG->SaveAs( canvasName  );
    delete calphaAVG;

    cout<<"Generating toys"<<endl;
    //2: use it as template for generating toys
    TH1D *h_dist_p0=new TH1D("h_dist_p0","Distribution of par0 of fit to alpha for 500 toys",1000,-5.0,5.0);
    for(unsigned int i = 0 ; i <nToys ; i++) {
      
      char histotitle[50];
      sprintf(histotitle,"tmp_%d",i);
      TH1D* variedHisto = sf->shuffle(myalpha, randomGen ,histotitle, myalpha);
    
      variedHisto->Write();
      //3:for each toy make a fit and save the results in the output histo
      std::vector<double> tmppars;
      std::vector<double> tmperrs;
      sf->alphaFit( variedHisto , tmppars,tmperrs);
      h_dist_p0->Fill(tmppars.at(0));
      // sf->fitPseudo( treeMC_Xbtag, treeDATA_Xbtag, ibtag, "ALL", variedHisto,i);
      //delete variedHisto;
    }
    h_dist_p0->Write();

    float alpha =  ((TF1*)myalpha->GetFunction(sf->getFitFunc("_LowRange").c_str()))->Eval(400);
    std::cout << "alpha (M=400) : " << alpha << std::endl;    
    char hlphname[50];
    sprintf(hlphname,"nominal_alpha_%dJ",inxj);
    TH1D* dummy_alpha=sf->dummyAlphaHist(alpha,myalpha,hlphname);
    dummy_alpha->Write();
    TCanvas can("ctoy","ctoy",600,600);
    for(unsigned int i = 0 ; i <nToys ; i++) {
      char histotitle[50];
      sprintf(histotitle,"tmp_%d",i);
      TH1D* variedHisto = (TH1D*)fWS->Get(histotitle);
      if(i==0)
	variedHisto->Draw("HIST");
      else{
	variedHisto->Draw("HIST,same");
      }
    }
    myalpha->SetLineColor(2);
    myalpha->SetMarkerColor(2);
    myalpha->Draw("same");
    //    char canvasName[400];
    sprintf( canvasName, "%s/mZZ_alphaToys_%dJ_%s.eps", myOutDir.c_str(), inxj, "ALL");
    can.SaveAs(canvasName);
  

    myalpha->Write();
    fWS->Close();
    //delete h_dist_p0;
    delete fWS;
    delete alpha_nxj;


    //     RooFitResult* fr = sf->fitSidebands( treeMC_Xbtag, treeDATA_Xbtag, inxj, "ALL", alpha_Xbtag );
    
    //     for(int i = 0 ; i <nToys ; i++) {
    //       std::cout << std::endl << "[ " << inxj << " jets ]  Toy: " << i << "/" << nToys << std::endl;
    //       TH1D* variedHisto = sf->shuffle(alpha_Xbtag, randomNum ,"tmp");
    //       sf->fitPseudo( treeMC_Xbtag, treeDATA_Xbtag, ibtag, "ALL", variedHisto,i);
    //       delete variedHisto;
    //     }
    
    //     if( nToys > 0 )
    //       sf->pseudoMassge(nToys, inxj,"ALL",fr);
    
    //     delete fr;
    //     delete sf;
    
    /////// delete alpha_Xbtag;


  } //end loop on nXjets

  //  return 0;
 
}//end void doAlpha



void CopyTreeVecToPlain(TTree *t1, std::string wType, std::string f2Name,std::string t2Name,int nxjCut){
 
  int ncands; 
  double eventWeight;
  unsigned int nrun,nevt;
  double leptType;
  int mynxj[35];
  double mZZd[35],region[35],mZqq[35];


  t1->SetBranchAddress("nCands",&ncands);
  t1->SetBranchAddress("run",&nrun);
  t1->SetBranchAddress("event",&nevt);
  t1->SetBranchAddress("lep",&leptType);
  t1->SetBranchAddress(wType.c_str(),&eventWeight);
  t1->SetBranchAddress("mZZ",mZZd);
  t1->SetBranchAddress("nXjets",mynxj);
  t1->SetBranchAddress("mJJ",mZqq);
  t1->SetBranchAddress("region",region);


  TFile *fOut=new TFile(f2Name.c_str(),"RECREATE");
  fOut->cd();
 
  int ncands_2, mynxj_2;
  double eventWeight_2;
  unsigned int nrun_2,nevt_2;
  double leptType_2;
  double mZZd_2,region_2,mZqq_2;

  TTree *t2=new TTree(t2Name.c_str(),t2Name.c_str());
 
  t2->Branch("nCands",&ncands_2,"nCands/I");
  t2->Branch("run",&nrun_2,"run/i");
  t2->Branch("event",&nevt_2,"event/i");
  t2->Branch("weight",&eventWeight_2,"weight/D");
  t2->Branch("nXjets",&mynxj_2 , "nXjets/I");
  t2->Branch("lep",&leptType_2,"lep/D");
  t2->Branch("mZZ",&mZZd_2 , "mZZ/D");
  t2->Branch("mJJ",&mZqq_2 , "mJJ/D");
  t2->Branch("region",&region_2 , "region/D");


  for(int i=0;i<t1->GetEntries();i++){

    t1->GetEntry(i);
    
    for(int j=0;j<ncands;j++){
      ncands_2=ncands;
      nrun_2=nrun;
      nevt_2=nevt;
      eventWeight_2=eventWeight;
      leptType_2=leptType;
      mZZd_2=mZZd[j];
      region_2=region[j];
      mZqq_2=mZqq[j];
      mynxj_2=int(mynxj[j]);
 
      if(region[j]<0||mZZd_2>9999.0||mynxj_2>10||mZqq_2>999.0){
	//cout<<"Event="<<nevt<<" cand="<<j<<" has reg="<<region[j]<<" mZZ="<<mZZd_2<<endl;
	continue;
      }

      if(mynxj_2==nxjCut||nxjCut<0) {
	int nb=t2->Fill();
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
 

  //  cout<<"returning"<<endl;
  // return t2;
}
