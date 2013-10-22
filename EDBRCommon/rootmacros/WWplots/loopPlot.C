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
  bool wantSideband  = false; // Will make histograms for sideband region
  bool wantSignal    = true; // Will make histograms for signal region
  bool wantFullRange = false; // Will not check signal or sideband, ie, pick all jet mass range
  int  wantPurity    = 1;    //0 for LP, 1 for HP, -1 for ALLP
  bool isTTcontrol   = false;   //if ttbar control region
  int  wantNXJets    = 1; // Will make histograms for 1 or 2 jet topology
  int  isZZchannel   = 0; //plot label for zz (1) or ww (0)
  int  flavour = 0; 
  if(wantElectrons) flavour=11; if(wantMuons) flavour=13;
  
  /// Luminosity value in pb^-1
  double lumiValue = 0;  
  if(wantMuons)lumiValue = 19706.255;
  if(wantElectrons)lumiValue = 19683.255;
  /// Should we scale the histograms to data?
  bool scaleToData = false;
  // Should we scale only wjets to make total MC = DATA?
  bool scaleOnlyWJets = (!isTTcontrol);
  /// Should we plot the Data/Bkg and Data-Bkg/Error ratios?
  bool makeRatio = true;
  /// Should we REDO histograms?
  bool redoHistograms = true;
  /// Should we put the signal MC stacked on top of the background (or just plot the signal alone)?
  bool isSignalStackOnBkg = false;

  /// Path to wherever the files with the trees are. 
  std::string pathToTrees="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv12/fullrange/";

  /// Path to wherever you want to put the histograms (figures) in.
 std::string outputDir = "./plots_WW_mu_signal_bvetoM_HP_v12_ratio_v1";

  /// Setup names of data files for trees.

  const int nDATA=1;//set to zero if you don't want to plot
  std::string dataLabels[nDATA]={"data_xww"};

/*   
  const int nDATA=0;//set to zero if you don't want to plot
  std::string dataLabels[nDATA]={};
 */

  std::vector<std::string> fData;
  for(int ii=0;ii<nDATA;ii++){
    fData.push_back(pathToTrees+"treeEDBR_"+dataLabels[ii]+".root");
  }

  const int nMC=4;//set to zero if you don't want to plot
  //list for ttbar control region (draw ttbar on top)
/*
  std::string mcLabels[nMC]={
				"SingleTop_xww",
				"VV_xww",
			    "WZJets_xww",
				"TTBARpowheg_xww"
			     };
*/
//list for analysis (draw wjets on top)
  std::string  mcLabels[nMC]={
	  			"TTBARpowheg_xww",
	  			"SingleTop_xww",
				"VV_xww",
				"WZJets_xww"
  				};

  //WW, muon channel
  double tempscale =0;
  if(!isTTcontrol) tempscale =1;
  else tempscale=1.5;
  double kFactorsMC_array[nMC] = {1, 1., tempscale, 1};//for tt, use 1.5
  
  //WW, electron channel

  std::vector<std::string> fMC;
  for(int ii=0;ii<nMC;ii++){
	  if(mcLabels[ii]=="WZJets_xww")fMC.push_back(pathToTrees+"treeEDBR_WJetsPt100_xww.root");
	else    fMC.push_back(pathToTrees+"treeEDBR_"+mcLabels[ii]+".root");
  }

  std::vector<double> kFactorsMC;
  //std::cout << "The contents of kFactorsMC are:" << std::endl;
  for (int index=0; index<nMC; index++)
    {
      //std::cout << kFactorsMC_array[index] << std::endl;
      kFactorsMC.push_back( kFactorsMC_array[index] );	
    }

  /// Setup names of MC signal files for trees.
 
 /*
  const int nMCSig=0;//set to zero if you don't want to plot
  std::string mcLabelsSig[nMCSig]={
                                  };
  double kFactorsSig_array[nMCSig] = {};
  */


  const int nMCSig=1;//set to zero if you don't want to plot
  std::string mcLabelsSig[nMCSig]={"BulkG_WW_inclusive_c0p2_M1500_xww"
                                  };
  double kFactorsSig_array[nMCSig] = {60000};//60000 for mu, 30000 for ele (PAS)
  


  std::vector<double> kFactorsMCSig;
  for (int index=0; index<nMCSig; index++)
    {
      kFactorsMCSig.push_back( kFactorsSig_array[index] );	
    }
  
  
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
						 wantFullRange,
						 wantPurity,
						 isTTcontrol,
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
						 wantFullRange,
						 wantPurity,
						 isTTcontrol,
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
						 wantFullRange,
						 wantPurity,
						 isTTcontrol,
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
						 wantPurity,
						 isTTcontrol,
						 flavour,
						 isZZchannel,
						 scaleToData,
						 scaleOnlyWJets,
						 makeRatio,
						 isSignalStackOnBkg,
						 kFactorsMC,kFactorsMCSig);
  std::cout<<"Set output dir"<<std::endl;
  plotter->setOutDir(outputDir);
  plotter->setDebug(false);

  //colors are assigned in the same order of mcLabels
  //For WW
  //{ "TTBARpowheg_xww", "SingleTop_xww", "VV_xww", "WJetsPt100_xww"}
  std::vector<int> fColorsMC;

  if(isTTcontrol==false)fColorsMC.push_back(210);
  fColorsMC.push_back(7);
  fColorsMC.push_back(4);
  fColorsMC.push_back(2);
  if(isTTcontrol==true)fColorsMC.push_back(210);

  std::vector<int> fColorsMCSig;
  fColorsMCSig.push_back(kBlack);
  
  plotter->setFillColor(fColorsMC);
  plotter->setLineColor(fColorsMCSig);

  int numOfHistos = listOfHistos.size();
  for(int i = 0; i != numOfHistos; ++i) 
    plotter->makeStackPlots(listOfHistos.at(i));      

  printf("Plotting done\n");
    
  delete plotter;

}//end main
