#include <Riostream.h>
#include <vector>
#include <string>

#include "TROOT.h"
#include "TError.h"
#include "TFile.h"
#include "TCollection.h"
#include "TKey.h"
#include "TGraph.h"
#include "TF1.h"
#include "EDBRHistoMaker.h"
#include "EDBRHistoPlotter.h"

#include "CMSTDRStyle.h"

double getPunzi(double nSubjetinessCut){

  gErrorIgnoreLevel=kFatal;//suppresses all info messages
  setTDRStyle();//TDR style
   
  /// Boolean flags to steer the histogram making
  bool wantElectrons = false; // Will make histograms for electrons
  bool wantMuons     = true; // Will make histograms for muons
  bool wantSideband  = false; // Will make histograms for sideband region
  bool wantSignal    = true; // Will make histograms for signal region
  int  wantNXJets    = 1; // Will make histograms for 1 or 2 jet topology
  int  flavour = 0; 
  if(wantElectrons) flavour=11; if(wantMuons) flavour=13;
  
  /// Luminosity value in pb^-1
  double lumiValue = 19477.6;
  /// k-factor for LO to NNLO
  double kFactor = 1.2;
  
  /// Path to wherever the files with the trees are. 
  std::string pathToTrees="/afs/cern.ch/user/t/tomei/work/public/EXOVV_2012/analyzer_trees/trees_presel_AB_20130227_AK7/";
  /// Path to wherever you want to put the histograms (figures) in.
  std::string outputDir = "./test";

  /// Setup names of data files for trees.
  const int nDATA=0;
  std::vector<std::string> fData;
  fData.clear();

  /// Setup names of MC files for trees.
  const int nMC=7;//set to zero if you don't want to plot
  std::string mcLabels[nMC]={"TTBAR",
			     "WW",
			     "WZ",
			     "ZZ",
			     "DYJetsPt50To70",
			     "DYJetsPt70To100",
			     "DYJetsPt100"};

  std::vector<std::string> fMC;
  for(int ii=0;ii<nMC;ii++){
    fMC.push_back(pathToTrees+"treeEDBR_"+mcLabels[ii]+".root");
  }

  /// Setup names of files for histograms (data and MC)
  std::vector<std::string> fHistosData;
  std::vector<std::string> fHistosMC;
 
  char buffer[256];
  printf("All strings set\n");

  /// ----------------------------------------------------------------
  /// This first part is the loop over trees to create histogram files 
  /// ----------------------------------------------------------------

  /// The EDBRHistoMaker, for reference
  ///
  ///EDBRHistoMaker::EDBRHistoMaker(TTree* tree, 
  ///		       bool wantElectrons,
  ///		       bool wantMuons,
  ///		       bool wantSideband,
  ///		       bool wantSignal,
  ///		       int wantNXJets)


  //loop over MC files and make histograms individually for each of them
  
  double totalBackground = 0;
  for(int i=0;i<nMC;i++){
    std::cout<<"\n-------\nRunning over "<<mcLabels[i].c_str()<<std::endl;
    std::cout<<"The file is " <<fMC.at(i)<<std::endl;    
    sprintf(buffer,"histos_%s.root",mcLabels[i].c_str());
    fHistosMC.push_back(buffer);
    
   
    TFile *fileMC = TFile::Open(fMC.at(i).c_str());
    TTree *treeMC = (TTree*)fileMC->Get("SelectedCandidates");
    EDBRHistoMaker* maker = new EDBRHistoMaker(treeMC, 
					       wantElectrons, 
					       wantMuons, 
					       wantSideband, 
					       wantSignal, 
					       wantNXJets);
      maker->setUnitaryWeights(false);
      double eventsPassing = maker->FastLoop(lumiValue,kFactor,nSubjetinessCut,1000,0.15);
      printf("This sample has %g events passing\n",eventsPassing);
      //delete maker; // This class is badly written and deleting it isn't safe!
      fileMC->Close();

      totalBackground += eventsPassing;
    
  }//end loop on MC files

  printf("\n+++++++++++++++++++++++\n");
  printf("Total passing background = %g\n",totalBackground);
  printf("+++++++++++++++++++++++\n");

  // The signal:
  double totalSignal = 0.0;
  {
    std::string pathToSignal = pathToTrees+"treeEDBR_BulkG_ZZ_lljj_c0p2_M1000.root";
    //std::string pathToSignal = pathToTrees+"treeEDBR_RSG_ZZ_lljj_c0p2_M1000.root";
    printf("Running over %s\n",pathToSignal.c_str());
    TFile *fileMC = TFile::Open(pathToSignal.c_str());
    TTree *treeMC = (TTree*)fileMC->Get("SelectedCandidates");    
    EDBRHistoMaker* maker = new EDBRHistoMaker(treeMC, 
					       wantElectrons, 
					       wantMuons, 
					       wantSideband, 
					       wantSignal, 
					       wantNXJets);
    maker->setUnitaryWeights(false);
    double eventsPassing = maker->FastLoop(lumiValue,1.0,nSubjetinessCut,1000,0.15);
    printf("This sample has %g events passing\n",eventsPassing);
    totalSignal = eventsPassing;
  }

  double punzi = totalSignal/(1.0+sqrt(totalBackground));
  
  printf("\n+++++++++++++++++++++++\n");
  printf("PUNZI SIGNIFICANCE = %g\n",punzi);
  printf("+++++++++++++++++++++++\n");

  return punzi;
  
}//end main

void optimizer() {

  TCanvas* cv = new TCanvas("punzi","",600,600);

  int nPoints = 20;
  double step = 0.05;

  TGraph* g = new TGraph(nPoints);
  
  for(int i=0; i!=20; ++i) {
    double nsubjcut = 0.0+step*i;
    double punzi = getPunzi(nsubjcut);
    g->SetPoint(i,nsubjcut,punzi);
  }

  g->Draw("A*");
  g->GetXaxis()->SetTitle("#tau_{21} cut");
  g->GetYaxis()->SetTitle("Arbitrary units");
  g->GetYaxis()->CenterTitle();
  g->SetTitle("");

  TF1* f = new TF1("f","[0]+[1]*(0.5+0.5*tanh([2]*(x-[3])))",0,1);
  f->SetParameter(0,0.1);
  f->SetParameter(1,10);
  f->SetParameter(2,10);
  f->SetParameter(3,0.3);
  f->SetLineWidth(2);
 
  g->Fit(f);
}
