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
  
  
  //#####################EDIT THE OPTIONS##############################
  /// Boolean flags to steer the histogram making
  bool wantElectrons = false; // Will make histograms for electrons
  bool wantMuons     = true; // Will make histograms for muons
  bool wantSideband  = true; // Will make histograms for sideband region
  bool wantSignal    = false; // Will make histograms for signal region
  int  wantNXJets    = 1; // Will make histograms for 1 or 2 jet topology
  int  isZZchannel   = false; //plot label for zz (true) or ww (false)
  int  flavour = 0; 
  if(wantElectrons) flavour=11; if(wantMuons) flavour=13;
  
  /// Luminosity value in pb^-1
  //double lumiValue = 19477.6;// for DoubleEle2012?
  double lumiValue = 19538.85;// for SingleMu2012
  /// Should we scale the histograms to data?
  bool scaleToData = false;
  /// Should we plot the Data/Bkg and Data-Bkg/Error ratios?
  bool makeRatio = true;
  /// Should we REDO histograms?
  bool redoHistograms = true;
  /// Should we put the signal MC stacked on top of the background (or just plot the signal alone)?
  bool isSignalStackOnBkg = false;

  /// Path to wherever the files with the trees are. 
  //CA8 (cmgTuple_08032013_CA8)
  std::string pathToTrees="/afs/cern.ch/work/s/santanas/public/EXOVV_2012/ntuples/WW_08_03_2013_muOnly_CA8/fullsideband/";
  //std::string pathToTrees="/afs/cern.ch/work/s/santanas/public/EXOVV_2012/ntuples/WW_08_03_2013_muOnly_CA8/fullsig/";
  //std::string pathToTrees="/afs/cern.ch/work/s/santanas/public/EXOVV_2012/ntuples/WW_08_03_2013_muOnly_CA8/merged/";

  /// Path to wherever you want to put the histograms (figures) in.
  std::string outputDir = "./WW_mu_sideband_CA8";

  /// Setup names of data files for trees.
 
  /*
  const int nDATA=6;//set to zero if you don't want to plot
  std::string dataLabels[nDATA]={"DoubleMu_Run2012A_13Jul2012",
				 "DoubleMu_Run2012A_recover",
  				 "DoubleMu_Run2012B_13Jul2012",
				 "DoubleMu_Run2012C_24Aug2012",
  				 "DoubleMu_Run2012C_PRv2",
  				 "DoubleMu_Run2012D_PRv1"};
  */

  /*
  const int nDATA=6;//set to zero if you don't want to plot
  std::string dataLabels[nDATA]={"Photon_Run2012A_13Jul2012",
  				 "Photon_Run2012A_recover",
  				 "DoublePhotonHighPt_Run2012B_13Jul2012",
				 "DoublePhotonHighPt_Run2012C_24Aug2012",
				 "DoublePhotonHighPt_Run2012C_PRv2",
				 "DoublePhotonHighPt_Run2012D_PRv1"};
  */ 
 
 
  const int nDATA=6;//set to zero if you don't want to plot
  std::string dataLabels[nDATA]={"SingleMu_Run2012A_13Jul2012_xww",
				 "SingleMu_Run2012A_recover_xww",
				 "SingleMu_Run2012B_13Jul2012_xww",
				 "SingleMu_Run2012C_24Aug2012_xww",
				 "SingleMu_Run2012C_PromptReco_xww",
				 "SingleMu_Run2012D_PromptReco_xww"};  

  /*    
  const int nDATA=0;//set to zero if you don't want to plot
  std::string dataLabels[nDATA]={};
  */

  std::vector<std::string> fData;
  for(int ii=0;ii<nDATA;ii++){
    fData.push_back(pathToTrees+"treeEDBR_"+dataLabels[ii]+".root");
  }

  /// Setup names of MC files for trees.

  /*  
  const int nMC=7;//set to zero if you don't want to plot
  std::string mcLabels[nMC]={"TTBAR",
			     "WW",
			     "WZ",
			     "ZZ",
			     "DYJetsPt50To70",
			     "DYJetsPt70To100",
			     "DYJetsPt100"};
  double kFactorsMC_array[nMC] = {1., 1., 1., 1., 1., 1., 1.};
  */

  const int nMC=16;//set to zero if you don't want to plot
  std::string mcLabels[nMC]={"TTBAR_xww",
			     "SingleTopBarTWchannel_xww",
			     "SingleTopTWchannel_xww",
			     "SingleTopBarSchannel_xww", 
			     "SingleTopSchannel_xww",
			     "SingleTopBarTchannel_xww",
			     "SingleTopTchannel_xww",
			     "WW_xww",
			     "WZ_xww",
			     "ZZ_xww",
			     "DYJetsPt50To70_xww",
			     "DYJetsPt70To100_xww",
			     "DYJetsPt100_xww",
			     "WJetsPt50To70_xww",
			     "WJetsPt70To100_xww",
			     "WJetsPt100_xww",
			     };
  double kFactorsMC_array[nMC] = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.3, 1.3, 1.3};
  
  std::vector<std::string> fMC;
  for(int ii=0;ii<nMC;ii++){
    fMC.push_back(pathToTrees+"treeEDBR_"+mcLabels[ii]+".root");
  }

  std::vector<double> kFactorsMC;
  //std::cout << "The contents of kFactorsMC are:" << std::endl;
  for (int index=0; index<nMC; index++)
    {
      //std::cout << kFactorsMC_array[index] << std::endl;
      kFactorsMC.push_back( kFactorsMC_array[index] );	
    }

  /// Setup names of MC signal files for trees.
  const int nMCSig=3;//set to zero if you don't want to plot
  std::string mcLabelsSig[nMCSig]={"BulkG_WW_lvjj_c1p0_M600_xww",
				   "BulkG_WW_lvjj_c1p0_M1000_xww",
				   "BulkG_WW_lvjj_c1p0_M1500_xww",
                                  };
  
  /*
  const int nMCSig=1;//set to zero if you don't want to plot
  std::string mcLabelsSig[nMCSig]={"BulkG_ZZ_lljj_c1p0_M1500"};
  */
  
  std::vector<std::string> fMCSig;
  for(int ii=0;ii<nMCSig;ii++){
    fMCSig.push_back(pathToTrees+"treeEDBR_"+mcLabelsSig[ii]+".root");
  }

  /// Setup names of files for histograms (data and MC)
  std::vector<std::string> fHistosData;
  std::vector<std::string> fHistosMC;
  std::vector<std::string> fHistosMCSig;
 
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
  ///		       int  wantNXJets,
  ///              bool isZZchannel)

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
						 wantNXJets,
						 isZZchannel);
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
						 wantNXJets,
						 isZZchannel);
      maker->setUnitaryWeights(false);
      maker->Loop(buffer);
      //delete maker; // This class is badly written and deleting it isn't safe!
      fileMC->Close();
    }
    
  }//end loop on MC files

  printf("Loop over MC done\n");

  //loop over MC signal files and make histograms individually for each of them
  for(int i=0;i<nMCSig;i++){
    std::cout<<"\n-------\nRunning over "<<mcLabelsSig[i].c_str()<<std::endl;
    std::cout<<"The file is " <<fMCSig.at(i)<<std::endl;    
    sprintf(buffer,"histos_%s.root",mcLabelsSig[i].c_str());
    fHistosMCSig.push_back(buffer);
    
    if(redoHistograms){
      TFile *fileMCSig = TFile::Open(fMCSig.at(i).c_str());
      TTree *treeMCSig = (TTree*)fileMCSig->Get("SelectedCandidates");
      EDBRHistoMaker* maker = new EDBRHistoMaker(treeMCSig, 
						 wantElectrons, 
						 wantMuons, 
						 wantSideband, 
						 wantSignal, 
						 wantNXJets,
						 isZZchannel);
      maker->setUnitaryWeights(false);
      maker->Loop(buffer);
      //delete maker; // This class is badly written and deleting it isn't safe!
      fileMCSig->Close();
    }
    
  }//end loop on MC files

  printf("Loop over MC signal done\n");

  /// ------------------------------------------------------------------
  /// This second part is the loop over histograms to create stack plots
  /// ------------------------------------------------------------------  

  //  EDBRHistoMaker::EDBRHistoMaker(TTree* tree,
  //			 bool wantElectrons,
  //			 bool wantMuons,
  //			 bool wantSideband,
  //			 bool wantSignal,
  //			 int  wantNXJets,
  //			 bool isZZchannel){
    
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
      printf("Histogram found: %s\n",hName.c_str());
      if(hName=="h_mj_vs_mzz")continue;//skip 2D histos
      listOfHistos.push_back(hName);
    }//end while loop
    oneFile->Close();
  }//end if fmc size >0

  EDBRHistoPlotter *plotter=new EDBRHistoPlotter("./",
						 fHistosData,
						 fHistosMC,
						 fHistosMCSig,
						 lumiValue,
						 wantNXJets,
						 flavour,
						 isZZchannel,
						 scaleToData,
						 makeRatio,
						 isSignalStackOnBkg,
						 kFactorsMC);
  std::cout<<"Set output dir"<<std::endl;
  plotter->setOutDir(outputDir);
  plotter->setDebug(false);

  //colors are assigned in the same order of mcLabels
  ////// {"TTBAR","WW","WZ","ZZ","DYJetsPt50To70","DYJetsPt70To100","DYJetsPt100","WJetsPt50To70","WJetsPt70To100","WJetsPt100"};
  std::vector<int> fColorsMC;
  fColorsMC.push_back(kGreen-3);
  fColorsMC.push_back(kYellow-9);
  fColorsMC.push_back(kYellow-6);
  fColorsMC.push_back(kYellow-3);
  fColorsMC.push_back(kYellow+3);
  fColorsMC.push_back(kYellow+6);
  fColorsMC.push_back(kYellow+9);
  fColorsMC.push_back(kMagenta-9);
  fColorsMC.push_back(kMagenta-6);
  fColorsMC.push_back(kMagenta-3);
  fColorsMC.push_back(kBlue-3);
  fColorsMC.push_back(kBlue-6);
  fColorsMC.push_back(kBlue-9);
  fColorsMC.push_back(kRed+3);
  fColorsMC.push_back(kRed);
  fColorsMC.push_back(kRed-4);

  ////// {"BulkG_WW_lvjj_c1p0_M600_xww","BulkG_WW_lvjj_c1p0_M1000_xww","BulkG_WW_lvjj_c1p0_M1500_xww"};
  std::vector<int> fColorsMCSig;
  fColorsMCSig.push_back(kBlack);
  fColorsMCSig.push_back(kMagenta);
  fColorsMCSig.push_back(kBlue);
  fColorsMCSig.push_back(kBlue+3);

  plotter->setFillColor(fColorsMC);
  plotter->setLineColor(fColorsMCSig);

  int numOfHistos = listOfHistos.size();
  for(int i = 0; i != numOfHistos; ++i) 
    plotter->makeStackPlots(listOfHistos.at(i));      

  printf("Plotting done\n");
    
  delete plotter;

}//end main
