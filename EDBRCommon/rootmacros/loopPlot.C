#include <Riostream.h>
#include <vector>
#include <string>

#include "TROOT.h"
#include "TError.h"
#include "TFile.h"
#include "TCollection.h"
#include "TKey.h"
#include "EDBRHistoMaker.h"
#include "EDBRHistoPlotter.h"

#include "CMSTDRStyle.h"

void loopPlot(){

  gErrorIgnoreLevel=kFatal;//suppresses all info messages
  setTDRStyle();//TDR style
   
  /// Boolean flags to steer the histogram making
  bool wantElectrons = true; // Will make histograms for electrons
  bool wantMuons     = false; // Will make histograms for muons
  bool wantSideband  = true; // Will make histograms for sideband region
  bool wantSignal    = false; // Will make histograms for signal region
  int  wantNXJets    = 2; // Will make histograms for 1 or 2 jet topology
  int  flavour = 0; 
  if(wantElectrons) flavour=11; if(wantMuons) flavour=13;
  
  /// Luminosity value in pb^-1
  double lumiValue = 19477.6;
  /// k-factor for LO to NNLO
  double kFactor = 1.2;
  /// Should we scale the histograms to data?
  bool scaleToData = true;
  /// Should we REDO histograms?
  bool redoHistograms = true;
  
  /// Path to wherever the files with the trees are. 
  std::string pathToTrees="/afs/cern.ch/user/t/tomei/EXOVV_2012/analyzer_trees/productionv1_round2/fullselCA8/";
  /// Path to wherever you want to put the histograms (figures) in.
  std::string outputDir = "./doubleJetElectrons_fullsel";

  /// Setup names of data files for trees.
  const int nDATA=6;//set to zero if you don't want to plot
  /*std::string dataLabels[nDATA]={"DoubleMu_Run2012A_13Jul2012",
				 "DoubleMu_Run2012A_recover",
  				 "DoubleMu_Run2012B_13Jul2012",
				 "DoubleMu_Run2012C_24Aug2012",
  				 "DoubleMu_Run2012C_PRv2",
  				 "DoubleMu_Run2012D_PRv1"};
  */
  std::string dataLabels[nDATA]={"Photon_Run2012A_13Jul2012",
  				 "Photon_Run2012A_recover",
  				 "DoublePhotonHighPt_Run2012B_13Jul2012",
				 "DoublePhotonHighPt_Run2012C_24Aug2012",
				 "DoublePhotonHighPt_Run2012C_PRv2",
				 "DoublePhotonHighPt_Run2012D_PRv1"};
  
  std::vector<std::string> fData;
  for(int ii=0;ii<nDATA;ii++){
    fData.push_back(pathToTrees+"treeEDBR_"+dataLabels[ii]+".root");
  }

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

  printf("\nStart making histograms\n\n");

  //loop over data files and make histograms individually for each of them
  for(int i=0;i<nDATA;i++){

    std::cout<<"\n-------\nRunning over "<<dataLabels[i].c_str()<<std::endl;
    std::cout<<"The file is " <<fData.at(i)<<std::endl;
    sprintf(buffer,"histos_%s.root",dataLabels[i].c_str());
    fHistosData.push_back(buffer);
    
    if(redoHistograms) {
      TFile *fileData = TFile::Open(fData.at(i).c_str());
      TTree *treeData = (TTree*)fileData->Get("SelectedCandidates");
      EDBRHistoMaker* maker = new EDBRHistoMaker(treeData, 
						 wantElectrons,
						 wantMuons,
						 wantSideband,
						 wantSignal,
						 wantNXJets);
      maker->setUnitaryWeights(true);
      maker->Loop(buffer);
      //delete maker; // This class is badly written and deleting it isn't safe!
      fileData->Close();
    }
    
  }//end loop on data files

  printf("Loop over data done\n");

  //loop over MC files and make histograms individually for each of them
  for(int i=0;i<nMC;i++){
    std::cout<<"\n-------\nRunning over "<<mcLabels[i].c_str()<<std::endl;
    std::cout<<"The file is " <<fMC.at(i)<<std::endl;    
    sprintf(buffer,"histos_%s.root",mcLabels[i].c_str());
    fHistosMC.push_back(buffer);
    
    if(redoHistograms){
      TFile *fileMC = TFile::Open(fMC.at(i).c_str());
      TTree *treeMC = (TTree*)fileMC->Get("SelectedCandidates");
      EDBRHistoMaker* maker = new EDBRHistoMaker(treeMC, 
						 wantElectrons, 
						 wantMuons, 
						 wantSideband, 
						 wantSignal, 
						 wantNXJets);
      maker->setUnitaryWeights(false);
      maker->Loop(buffer);
      //delete maker; // This class is badly written and deleting it isn't safe!
      fileMC->Close();
    }
    
  }//end loop on data files

  printf("Loop over MC done\n");

  /// ------------------------------------------------------------------
  /// This second part is the loop over histograms to create stack plots
  /// ------------------------------------------------------------------  

  //  EDBRHistoMaker::EDBRHistoMaker(TTree* tree,
  //			 bool wantElectrons,
  //			 bool wantMuons,
  //			 bool wantSideband,
  //			 bool wantSignal,
  //			 int wantNXJets){
    
  printf("\nStart looping over histograms\n\n");
  //make nice plots
  std::vector<std::string> listOfHistos;
  if(nMC>0){
    // Open one of the histogram files just to get the list of histograms
    // produced, then loop over all the histograms inheriting 
    // from TH1 contained in the file.
    sprintf(buffer,"histos_%s.root",mcLabels[0].c_str());
    std::cout<<"Opening "<<buffer<<std::endl;
    TFile* oneFile = TFile::Open(buffer);
    TIter next(oneFile->GetListOfKeys());
    TKey *key;
    
    while ((key = (TKey*)next())) {
      TClass *cl = gROOT->GetClass(key->GetClassName());
      if (!cl->InheritsFrom("TH1")) continue;
      TH1 *hTMP = (TH1*)key->ReadObj();
      std::string hName=hTMP->GetName();
      //   printf("Histogram found: %s\n",hName.c_str());
      listOfHistos.push_back(hName);
    }//end while loop
    oneFile->Close();
  }//end if fmc size >0

  EDBRHistoPlotter *plotter=new EDBRHistoPlotter("./",
						 fHistosData,
						 fHistosMC,
						 lumiValue,
						 kFactor,
						 wantNXJets,
						 flavour,
						 scaleToData);
  std::cout<<"Set output dir"<<std::endl;
  plotter->setOutDir(outputDir);
  plotter->setDebug(false);

  //colors are assigned in the same order of mcLabels
  ////// {"TTBAR","WW","WZ","ZZ","DYJetsPt50To70","DYJetsPt70To100","DYJetsPt100"};
  std::vector<int> fColorsMC;
  fColorsMC.push_back(kGreen-3);
  fColorsMC.push_back(kMagenta-9);
  fColorsMC.push_back(kMagenta-6);
  fColorsMC.push_back(kMagenta-3);
  fColorsMC.push_back(kBlue-3);
  fColorsMC.push_back(kBlue-6);
  fColorsMC.push_back(kBlue-9);
  fColorsMC.push_back(kGreen+3);
  fColorsMC.push_back(kGreen);
  fColorsMC.push_back(kGreen-3);

  plotter->setFillColor(fColorsMC);

  int numOfHistos = listOfHistos.size();
  for(int i = 0; i != numOfHistos; ++i) 
    plotter->makeStackPlots(listOfHistos.at(i));      

  printf("Plotting done\n");
    
  delete plotter;

}//end main
