#include <Riostream.h>
#include <vector>
#include <string>

#include "TROOT.h"
#include "TFile.h"
#include "TCollection.h"
//#include "EDBRHistoMaker.h"
//#include "EDBRHistoPlotter.h"

#include "CMSTDRStyle.h"

void loopPlot(){

  setTDRStyle();
  
  int statusMakeHisto = gROOT->ProcessLine(".L makeHisto.C+");
  int statusPlotHisto = gROOT->ProcessLine(".L plotHisto.C+");

  if((statusMakeHisto != 0) || (statusPlotHisto != 0)) {
    printf("Macro compilation failed!!! Aborting...\n");
    return;
  }
  
  /// Path to whenever the files with the trees are. 
  //std::string pathToTrees="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1/";
  std::string pathToTrees = "/afs/cern.ch/user/t/tomei/EXOVV_2012/analyzer_trees/productionv1/";
  /// Path to whenever you want to put the histograms (figures) in.
  std::string outputDir = "./test_outPlots";

  /// Setup names of data files for trees.
  const int nDATA=5;//set to zero if you don't want to plot
  std::string dataLabels[nDATA]={"DoubleMu_Run2012A_13Jul2012",
				 "DoubleMu_Run2012A_recover",
				 "DoubleMu_Run2012B_13Jul2012",
				 "DoubleMu_Run2012C_24Aug2012",
				 "DoubleMu_Run2012D_PRv1"};
  std::vector<std::string> fData;
  for(int ii=0;ii<nDATA;ii++){
    fData.push_back(pathToTrees+"treeEDBR_"+dataLabels[ii]+".root");
  }

  /// Setup names of MC files for trees.
  const int nMC=4;//set to zero if you don't want to plot
  std::string mcLabels[nMC]={"TTBAR","DYJetsPt50To70","DYJetsPt70To100","DYJetsPt100"};//examples for now
  std::vector<std::string> fMC;
  for(int ii=0;ii<nMC;ii++){
    fMC.push_back(pathToTrees+"treeEDBR_"+mcLabels[ii]+".root");
  }

  /// Setup names of files for histograms (data and MC)
  std::vector<std::string> fHistosData;
  std::vector<std::string> fHistosMC;

  /// Luminosity value
  double lumiValue = 13086.0*0.2;
  
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
  char buffer[256];
  for(int i=0;i<nDATA;i++){
    TFile *fileData = TFile::Open(fData.at(i).c_str());
    TTree *treeData = (TTree*)fileData->Get("SelectedCandidates");
    sprintf(buffer,"\nRunning over %s\n\n",dataLabels[i].c_str());
    EDBRHistoMaker* maker = new EDBRHistoMaker(treeData, 
					       false, 
					       true, 
					       true, 
					       true, 
					       2);
    sprintf(buffer,"histos_%s.root",dataLabels[i].c_str());
    maker->Loop(buffer);
    std::string oneString(buffer);
    fHistosData.push_back(buffer);
    //delete maker; // This class is badly written and deleting it isn't safe!
    fileData->Close();
  }//end loop on data files

  printf("Loop over data done\n");

  //loop over MC files and make histograms individually for each of them
  for(int i=0;i<nMC;i++){
    TFile *fileMC = TFile::Open(fMC.at(i).c_str());
    TTree *treeMC = (TTree*)fileMC->Get("SelectedCandidates");
    sprintf(buffer,"\nRunning over %s\n\n",mcLabels[i].c_str());
    EDBRHistoMaker* maker = new EDBRHistoMaker(treeMC, 
					       false, 
					       true, 
					       true, 
					       true, 
					       2);
    sprintf(buffer,"histos_%s.root",mcLabels[i].c_str());
    maker->Loop(buffer);
    std::string oneString(buffer);
    fHistosMC.push_back(buffer);
    //delete maker; // This class is badly written and deleting it isn't safe!
    fileMC->Close();
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
    TFile* oneFile = TFile::Open(buffer);
    TIter next(oneFile->GetListOfKeys());
    TKey *key;
    
    while ((key = (TKey*)next())) {
      TClass *cl = gROOT->GetClass(key->GetClassName());
      if (!cl->InheritsFrom("TH1")) continue;
      TH1 *hTMP = (TH1*)key->ReadObj();
      std::string hName=hTMP->GetName();
      printf("Histogram found: %s\n",hName.c_str());
      listOfHistos.push_back(hName);
    }//end while loop
  }//end if fmc size >0

  EDBRHistoPlotter *plotter=new EDBRHistoPlotter("./",
						 fHistosData,
						 fHistosMC,
						 lumiValue,
						 true);
  
  printf("Set output dir\n");
  plotter->setOutDir(outputDir);

  int numOfHistos = listOfHistos.size();
  for(int i = 0; i != numOfHistos; ++i) 
    plotter->makeStackPlots(listOfHistos.at(i));      

  printf("Plotting done\n");
    
  delete plotter;

}//end main
