/** \macro H2GGFitter.cc
 *
 * $Id: R2JJFitter.cc,v 1.13 2013/08/05 15:36:57 santanas Exp $
 *
 * Software developed for the CMS Detector at LHC
 *
 *
 *  \author Serguei Ganjour - CEA/IRFU/SPP, Saclay
 *  \modified by Maxime Gouzevitch for the Dijet Bump Search - IPNL, Lyon 
 *
 * Macro is implementing the unbinned maximum-likelihood model for 
 * the Higgs to gamma gamma analysis. PDF model and RooDataSets 
 * are stored in the workspace which is feeded to  HiggsAnalysis/CombinedLimit tools:
 * 
 * http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HiggsAnalysis/CombinedLimit
 * http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HiggsAnalysis/CombinedLimit/data/lhc-hcg/cms-hgg-1fb/
 * 
 * The analysis root trees produced in a simple format 
 *
 *     TFile file(filename,"RECREATE", "X->jj input tree for unbinned maximum-likelihood fit");
 *     TTree* outTree  = new TTree("XTojj","X->jj input tree for unbinned maximum-likelihood fit");
 *     Float_t mass;
 *     Int_t CAT3;
 *     Float_t weight;
 *
 *     outTree->Branch("mass",&mass,"mass/F");
 *     outTree->Branch("weight",&weight,"weight/F");
 *     outTree->Branch("CAT4",&CAT4,"CAT4/I");
 *     {
 *       .............
 *       outTree->Fill();
 *     }
 *
 *     file.Write();
 *     file.Close();
 *     delete outTree;
 *
 * are used as input files. They have to be produced for 
 * data and Monte Carlo signal and background data sets 
 * after all analysis selections to be applied. It is recommended to put   
 * loose kinematical cuts on pt1 and pt2 (20 GeV) since further selections 
 * are possible based on RooDataSets. 
 * It is recommended to use Root 5.28/00 (CMSSW_4_1_3).
 *
 *
 */
// Loading:  .L H2GGFitter.cc
// Running:  runfits("hgg120-shapes-combined-Unbinned.root")  
//                

/*
#include <cstring>
#include <cerrno>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <unistd.h>
#include <errno.h>
#include <iomanip>
// ROOT headers
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TIterator.h"

#include "TLatex.h"
#include "TString.h"
#include "TLegend.h"
#include "TGraphAsymmErrors.h"


// RooFit headers
#include "RooAbsPdf.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooHistPdf.h"
#include "RooMsgService.h"
#include "RooNLLVar.h"
#include "RooPlot.h"
#include "RooRandom.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"
#include "TStyle.h"

// RooStats headers
#include "RooStats/HLFactory.h"

#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooAbsData.h"
#include "RooPlot.h"
#include "RooGaussian.h"
#include "RooProduct.h"
#include "RooExtendPdf.h"
#include "RooBernstein.h"
#include "RooFitResult.h"
#include "RooMinimizer.h"
#include "RooCmdArg.h"
#include "RooConstVar.h"
#include "RooRealVar.h"
*/
//#include "HiggsCSandWidth.h"
//#include "HiggsCSandWidth.cc"
//#include "RooPower.h"

#include <sstream>
using namespace RooFit;
using namespace RooStats ;

static const Int_t NCAT = 4;
static const Double_t MMIN = 700;
static const Double_t MMAX = 3200;
static const Double_t BINSIZEPLOT = 100; //GeV
static const Double_t BINSIZEPLOTSIGNAL = 10; //GeV
static const Double_t BINSIZEPLOTTEST = 100; //GeV
static const Double_t MassForFtest = 700; //GeV

void AddSigData(RooWorkspace*, Float_t);
void AddBkgData(RooWorkspace*);
void ReadFromFile(ifstream& , double* );
void SigModelFit(RooWorkspace*, Float_t);
void SigModelSet(RooWorkspace* , double* , int );
RooFitResult*  BkgModelFitBernstein(RooWorkspace*, Bool_t);
void BkgModelTest(RooWorkspace*, Bool_t);
void MakePlots(RooWorkspace*, Float_t, RooFitResult* , bool);
void MakeSigWS(RooWorkspace* w, const char* filename);
void MakeBkgWS(RooWorkspace* w, const char* filename);
void SetParamNames(RooWorkspace*);
void SetConstantParams(const RooArgSet* params);


RooArgSet* defineVariables()
{

  // define variables of the input ntuple
  RooRealVar* mZZ  = new RooRealVar("mZZ","M(VV)",MMIN,MMAX,"GeV");
  RooRealVar* weight   = new RooRealVar("weight","Reweightings",0,100000,"");
  RooRealVar* region   = new RooRealVar("region","jet mass region",-3,3,"");
  RooCategory* categories = new RooCategory("categories","event category 4") ;
  categories->defineType("VV_mass_ele_LP",0);
  categories->defineType("VV_mass_ele_HP",1);
  categories->defineType("VV_mass_mu_LP",2);
  categories->defineType("VV_mass_mu_HP",3);
  //   categories->defineType("dijet_mass_1mtag_1mdtag",4);
  //   categories->defineType("dijet_mass_0mtag",5);

  RooArgSet* ntplVars = new RooArgSet(*mZZ, *categories, *weight, *region);
 
  return ntplVars;
}



void runfits(const Float_t mass=1000, bool isWW = true, Bool_t dobands = false)
{

  //******************************************************************//
  //  Running mode  corresponds to the following cases
  //         - full run set:
  //         - create signal and background data sets 
  //         - make and fit signal and background  models 
  //         - write signal and background workspaces in root files
  //         - write data card

  //*******************************************************************//

  TString fileBaseName(TString::Format("Xvv.mX%d_ZZ_8TeV", int(mass)));
  if (isWW) fileBaseName = TString::Format("Xvv.mX%d_WW_8TeV", int(mass));

  TString fileBkgName(TString::Format("Xvv.inputbkg_8TeV", mass));
  TString card_name("Xvv_models_Bkg_8TeV_test.rs");
  HLFactory hlf("HLFactory", card_name, false);
  RooWorkspace* w = hlf.GetWs();

  RooFitResult* fitresults;

  // Add data to the workspace
  AddSigData(w, mass, isWW);
  AddBkgData(w);
  
  // Fit with models
  SigModelFit(w, mass);
  fitresults = BkgModelFitBernstein(w, dobands);

  //Test different fit models for background (use chi2, F-test, etc..)
  BkgModelTest(w, dobands);
      
  // Create Workspaces
  MakeSigWS(w, fileBaseName);
  MakeBkgWS(w, fileBkgName);

  // Make datacards for "combine" tool
  int ncat = NCAT;
  for (int c = 0; c < ncat; c++) 
    MakeDataCard_1Channel(w, fileBaseName, fileBkgName, c, mass);

  /*
  // Make plots for data and fit results
  MakePlots(w, mass, fitresults, isWW);
  */

  return;
}



void AddSigData(RooWorkspace* w, Float_t mass, bool isWW) {

  //TString inDir   = "./MiniTrees/Signal_VV/";
  //TString inDir   = "/afs/cern.ch/work/s/santanas/public/EXOVV_2012/ntuples/WW_02_05_2013_ForUnblinding/fullallrange/AnaSigTree_mWW_Type2_corrected/"; 
  //TString inDir   = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv8/AnaSigTree/"; 
  //TString inDir   = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv9/AnaSigTree/"; 
  //TString inDir   = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv10/AnaSigTree/"; 
  TString inDir   = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv12/AnaSigTree/"; 

  int iMass = abs(mass);       
  /*
  TFile sigFile1(inDir+TString(Form("dijetWtag_Moriond_ZZHppOUT%d_miniTree.root", iMass)));  
  if (isWW) {
    sigFile1.Close();
    TFile sigFile1(inDir+TString(Form("dijetWtag_Moriond_WWHppOUT%d_miniTree.root", iMass)));
  }
  */                     

  /// A bit of workaround. I don't want to change the inner workings of the script.
  /// If we're dealing with a mass point for which we have no MC... we just open another file
  int iMassOfFileToOpen; 
  if(iMass%100 == 0) 
    iMassOfFileToOpen=iMass;
  else
    iMassOfFileToOpen=(iMass-iMass%100);
      
  cout << "iMassOfFileToOpen = " << iMassOfFileToOpen << endl;

  //== ZZ
  //TFile sigFile1(inDir+TString(Form("EXOVVTree_BulkG_ZZ_lljj_c0p2_M%d_SIG_NOcut.root", iMassOfFileToOpen)));
  //== WW
  //PAS: 
  //TFile sigFile1(inDir+TString(Form("treeEDBR_BulkG_WW_lvjj_c0p2_M%d_xww.root", iMassOfFileToOpen)));
  //PAPER:
  TFile sigFile1(inDir+TString(Form("treeEDBR_BulkG_WW_inclusive_c0p2_M%d_xww.root", iMassOfFileToOpen)));
  //TFile sigFile1(inDir+TString(Form("treeEDBR_BulkG_WW_lvjj_c0p2_M%d_xww.root", iMass)));

  /*
  if (!isWW) {
    TFile sigFile1(inDir+TString(Form("treeEDBR_BulkG_ZZ_lljj_c0p2_M%d_xww.root", iMass)));  
  }

  if (isWW) {
    //sigFile1.Close();
    TFile sigFile1(inDir+TString(Form("treeEDBR_BulkG_WW_lvjj_c0p2_M%d_xww.root", iMass)));
  }
  */

  // common preselection cut
  //TString mainCut("1");
  TString mainCut("region==1 && categories>=0 && categories<4"); //signal (reg=1) + ignore 2jet categories (for the moment)

  // Luminosity:
  Float_t Lum = 19700.0;
  RooRealVar lumi("lumi","lumi",Lum);
  w->import(lumi); 

  Int_t ncat = NCAT;  
  Float_t MASS(mass);
  //Float_t sqrts(8);

  //HiggsCSandWidth *myCSW = new HiggsCSandWidth();

  //Float_t xs_ggqq  = 0.05;
  //Float_t br       = 1.0;

  /*
  cout << "++++++++++++++++++++++++++++++++" << endl;
  cout << "sigma_X x BR (mX = " << MASS << ") = "  << xs_ggqq*br << endl;
  cout << "++++++++++++++++++++++++++++++++" << endl;
  */

  // number of generated events in each signal dataset
  //Float_t n_ggqq(30000);

  // Scale factor for the signal
  //Float_t scaleX  = lumi.getVal()*xs_ggqq * br /n_ggqq;

  TTree* sigTree1 = (TTree*) sigFile1.Get("SelectedCandidatesPlain");

  // Variables
  RooArgSet* ntplVars = defineVariables();
  RooRealVar weightVar("weightVar","",1,0,1000);

  // add weight var into the list of ntuple variables
  weightVar.setVal(1.);
  ntplVars->add(RooArgList(weightVar));

  //****************************//
  // Signal  Data Set
  //****************************//

  // Create non scaled signal dataset composed with  different productions 
  // according to their cross sections

  //NOTE: it seems that "sigScaled" and "Sig_cat" are actually never really used..

  cout << "=========" << endl;
  cout << "========= sigScaled" << endl;
  cout << "=========" << endl;
  RooDataSet sigScaled("sigScaled","dataset",sigTree1,*ntplVars,mainCut,"weight");
  //RooDataSet sigScaled("sigScaled","dataset",sigTree1,*ntplVars,mainCut); //NO WEIGHTS
  sigScaled.Print("v");

  cout << "=========" << endl;
  cout << "========= sigToFit vs category" << endl;
  cout << "=========" << endl;
  RooDataSet* sigToFit[NCAT];
  for (int c = 0; c < ncat; ++c) {
    sigToFit[c] =  (RooDataSet*) sigScaled.reduce(*w->var("mZZ"),mainCut+TString::Format(" && categories==%d",c));
    w->import(*sigToFit[c],Rename(TString::Format("Sig_cat%d",c)));
    sigToFit[c]->Print("v");
  }
          
  // Create full signal data set without categorization
  cout << "=========" << endl;
  cout << "========= sigToFitAll" << endl;
  cout << "=========" << endl;
  RooDataSet* sigToFitAll  = (RooDataSet*) sigScaled.reduce(*w->var("mZZ"),mainCut);
  w->import(*sigToFitAll,Rename("Sig"));
  sigToFitAll->Print("v");

  // Create weighted signal dataset composed with  different 
  // production processes according to their cross sections
  // no common preselection cut applied yet 

  //---
  //RooRealVar *weightVar1 = (RooRealVar*) (*ntplVars)["weight"] ;
  //RooRealVar *weightVar2 = (RooRealVar*) (*ntplVars)["normWeight"] ;
  //RooFormulaVar *weightVar3 = new RooFormulaVar( "weight3", "", "@0", RooArgList(*weightVar1));
  //---
  //weightVar.setVal(1.);
  //ntplVars->setRealValue("normWeight", scaleX);
  //RooDataSet sigWeightedTmp1("sigData","dataset",sigTree1,*ntplVars,mainCut,"weightVar");
  //RooRealVar *weightX = (RooRealVar*) sigWeightedTmp1.addColumn(*weightVar3) ;
  //   RooDataSet sigWeighted("sigData","dataset",
  // 			 RooArgList((*ntplVars)["mZZ"],
  // 				    (*ntplVars)["categories"],*weightX),
  // 			 Import(sigWeightedTmp1),WeightVar(*weightX));
  //---

  cout << "=========" << endl;
  cout << "========= sigWeighted" << endl;
  cout << "=========" << endl;
  RooDataSet sigWeighted("sigData","dataset",sigTree1,*ntplVars,mainCut,"weight");
  //RooDataSet sigWeighted("sigData","dataset",sigTree1,*ntplVars,mainCut); //NO WEIGHTS
  sigWeighted.Print("v");

  /*
  cout << "---- nX:  " << sigWeighted.sumEntries() << endl; 
  for (int c = 0; c < ncat; ++c) {
    Float_t nExpEvt = sigWeighted.reduce(*w->var("mZZ"),TString::Format("categories==%d",c))->sumEntries();
    cout << TString::Format("nEvt exp.  cat%d : ",c) << nExpEvt 
	 << TString::Format("   eff x Acc  cat%d : ",c) << nExpEvt*100/scaleX/100000. << "%"  
	 << endl; 
  }
  */

  cout << "=========" << endl;
  cout << "========= signal vs category" << endl;
  cout << "=========" << endl;
  RooDataSet* signal[NCAT];
  for (int c = 0; c < ncat; ++c) {
    signal[c] =  (RooDataSet*) sigWeighted.reduce(*w->var("mZZ"),mainCut+TString::Format(" && categories==%d",c));
    w->import(*signal[c],Rename(TString::Format("SigWeight_cat%d",c)));
    signal[c]->Print("v");
  }

  // Create full weighted signal data set without categorization
  cout << "=========" << endl;
  cout << "========= signalAll" << endl;
  cout << "=========" << endl;
  RooDataSet* signalAll  = (RooDataSet*) sigWeighted.reduce(*w->var("mZZ"),mainCut);
  w->import(*signalAll, Rename("SigWeight"));

}


void AddBkgData(RooWorkspace* w) {

  //TString inDir   = "./MiniTrees/Data_VV/";
  //TString inDir   = "/afs/cern.ch/work/s/santanas/public/EXOVV_2012/ntuples/WW_02_05_2013_ForUnblinding/fullallrange/AnaSigTree_mWW_Type2_corrected/"; 
  //TString inDir   = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv8/AnaSigTree/"; 
  //TString inDir   = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv10/AnaSigTree/"; 
  //TString inDir   = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv10/AnaSBTree/"; 
  TString inDir   = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv12/AnaSigTree/"; 

  //TFile dataFile(inDir+"dijetWtag_Moriond_Mar6_miniTree.root");   
  TFile dataFile(inDir+"treeEDBR_data_xww.root");   
  //TFile dataFile(inDir+"treeEDBR_allBkg_xww.root");   

  // common preselection cut
  //TString mainCut("1");
  TString mainCut("region==1 && categories>=0 && categories<4"); //signal (reg=1) + ignore 2jet categories

  Int_t ncat = NCAT;
  Float_t minMassFit(MMIN),maxMassFit(MMAX); 

  TTree* dataTree     = (TTree*) dataFile.Get("SelectedCandidatesPlain");

  // Variables
  RooArgSet* ntplVars = defineVariables();

  // Apply a common preselection cut;
  cout << "=========" << endl;
  cout << "========= Data" << endl;
  cout << "=========" << endl;

  //============== IMPORTANT ==========================

  
  //--
  // For real data
  RooDataSet Data("Data","dataset",dataTree,*ntplVars,mainCut,"weight");
  //--
  

  /*
  //--
  // For MC background
  
  Float_t LumForBkg = 19700.0;
  RooRealVar lumiForBkg ("lumiForBkg","lumiForBkg",LumForBkg);  

  RooRealVar *weightOriginal = (RooRealVar*) (*ntplVars)["weight"] ;
  RooFormulaVar *weightNormLumiFormula = new RooFormulaVar("weightNormLumiFormula","","@0*@1",RooArgList(*weightOriginal, lumiForBkg));    

  RooDataSet *DataNoWeight = new RooDataSet("DataNoWeight","DataNoWeight",dataTree,*ntplVars,mainCut);
  RooRealVar *weightFinal = (RooRealVar*) DataNoWeight->addColumn(*weightNormLumiFormula) ;
  RooDataSet Data("Data","dataset",DataNoWeight,*DataNoWeight->get(),mainCut,weightFinal->GetName());
  
  //--
  */

  //====================================================

  Data.Print("v");

  // split into NCAT  categories;  
  cout << "=========" << endl;
  cout << "========= dataToFit vs category" << endl;
  cout << "=========" << endl;
  RooDataSet* dataToFit[NCAT];
  for (int c = 0; c < ncat; ++c) {
    // Real data
    dataToFit[c]   = (RooDataSet*) Data.reduce(*w->var("mZZ"),mainCut+TString::Format(" && categories==%d",c));
    w->import(*dataToFit[c],Rename(TString::Format("Data_cat%d",c)));
    dataToFit[c]->Print("v");
  }

  // Create full data set without categorization
  cout << "=========" << endl;
  cout << "========= data (same as -Data-?)" << endl;
  cout << "=========" << endl;
  RooDataSet* data    = (RooDataSet*) Data.reduce(*w->var("mZZ"),mainCut);
  w->import(*data, Rename("Data"));
  data->Print("v");

}


void ReadFromFile(ifstream& myfile, double* params) {
  /* File structure
     mean_match =  606.57 +/- 254.58 L(400 - 800) 
     sigma_match =  22.978 +/- 48.225 L(20 - 70) 
     width_match =  1.0000 L(0 - 1000) 
     mean_unmatch =  600.00 C L(200 - 1500) 
     sigma_unmatch =  30.000 C L(1 - +INF) 
     alpha_unmatch =  1.0000 C L(-INF - +INF) 
     n_unmatch =  0.30000 C L(0 - 20) 
     machfrac =  0.99900 C L(0 - 1) 
     alpha1_match =  2.2912 +/- 2.5776 L(0.01 - 5) 
     n1_match =  1.7910 +/- 5.4302 L(0 - 10) 
     alpha2_match =  2.3328 +/- 3.1736 L(0.01 - 5) 
     n2_match =  1.8134 +/- 7.7558 L(0 - 10) 
  */
    string line; // Line of the input file
    string buf; // Have a buffer string
    stringstream ss; // Insert the string into a stream
    double value;
    if (myfile.is_open()) {
      // Ok, file is open.
      getline (myfile,line); /// mean_match
      ss.str(line); ss >> buf; ss >> buf; ss >> buf; value = atof(buf.c_str()); params[0]=value;
      getline (myfile,line); /// sigma_match
      ss.str(line); ss >> buf; ss >> buf; ss >> buf; value = atof(buf.c_str()); params[1]=value;
      getline (myfile,line); /// width_match (discard)
      getline (myfile,line); /// mean_unmatch (discard)
      getline (myfile,line); /// sigma_unmatch (discard)
      getline (myfile,line); /// alpha_unmatch (discard)
      getline (myfile,line); /// n_unmatch (discard)
      getline (myfile,line); /// machfrac (discard)
      getline (myfile,line); /// alpha1_match
      ss.str(line); ss >> buf; ss >> buf; ss >> buf; value = atof(buf.c_str()); params[2]=value;
      getline (myfile,line); /// n1_match
      ss.str(line); ss >> buf; ss >> buf; ss >> buf; value = atof(buf.c_str()); params[3]=value;
      getline (myfile,line); /// alpha2_match
      ss.str(line); ss >> buf; ss >> buf; ss >> buf; value = atof(buf.c_str()); params[4]=value;
      getline (myfile,line); /// n2_match
      ss.str(line); ss >> buf; ss >> buf; ss >> buf; value = atof(buf.c_str()); params[5]=value;
    }
    else {
      printf("WHAT THE FILE IS NOT OPEN!!!\n");
    }
    myfile.close();  
}

void SigModelSet(RooWorkspace* w, double* params, int c) {
  w->var(TString::Format("mgg_sig_m0_cat%d",c))->setVal(params[0]);
  w->var(TString::Format("mgg_sig_sigma_cat%d",c))->setVal(params[1]);
  w->var(TString::Format("mgg_sig_alpha1_cat%d",c))->setVal(params[2]);
  w->var(TString::Format("mgg_sig_n1_cat%d",c))->setVal(params[3]);
  w->var(TString::Format("mgg_sig_alpha2_cat%d",c))->setVal(params[4]);
  w->var(TString::Format("mgg_sig_n2_cat%d",c))->setVal(params[5]);
}

void SigModelFit(RooWorkspace* w, Float_t mass) {

  Int_t ncat = NCAT;
  Float_t MASS(mass);

  //******************************************//
  // Fit signal with model pdfs
  //******************************************//
  // retrieve pdfs and datasets from workspace to fit with pdf models

  RooDataSet* sigToFit[NCAT];
  RooAbsPdf* MggSig[NCAT];

  Float_t minMassFit(MMIN),maxMassFit(MMAX),BinSizePlotSignal(BINSIZEPLOTSIGNAL); 

  Int_t nBinsMassSignal( int( (maxMassFit-minMassFit)/BinSizePlotSignal) );

  // Fit Signal 
  for (int c = 0; c < ncat; ++c) {
  //for (int c = 3; c < 4; ++c) {

    cout << "---------- category = " << c << endl;
    //    sigToFit[c]   = (RooDataSet*) w->data(TString::Format("Sig_cat%d",c));
    sigToFit[c]   = (RooDataSet*) w->data(TString::Format("SigWeight_cat%d",c));
    MggSig[c]     = (RooAbsPdf*)  w->pdf(TString::Format("MggSig_cat%d",c));
    w->Print();

    ((RooRealVar*) w->var(TString::Format("mgg_sig_m0_cat%d",c)))->setVal(MASS);  
    //((RooRealVar*) w->var(TString::Format("mgg_sig_gsigma_cat%d",c)))->setVal(MASS*0.1);  ---> OLD
    ((RooRealVar*) w->var(TString::Format("mgg_sig_sigma_cat%d",c)))->setVal(MASS*0.1);  
    cout << "---------------- Peak Val = " << w->var(TString::Format("mgg_sig_m0_cat%d",c))->getVal() << " Mass = " << MASS << endl;

    //define a set with the parameters
    w->defineSet(TString::Format("SigPdfParam_cat%d",c), RooArgSet(*w->var(TString::Format("mgg_sig_m0_cat%d",c)),
								   *w->var(TString::Format("mgg_sig_sigma_cat%d",c)),
								   *w->var(TString::Format("mgg_sig_alpha1_cat%d",c)),
								   *w->var(TString::Format("mgg_sig_n1_cat%d",c)), 
								   *w->var(TString::Format("mgg_sig_alpha2_cat%d",c)),
								   *w->var(TString::Format("mgg_sig_n2_cat%d",c))) );

    /* OLD
    w->defineSet(TString::Format("SigPdfParam_cat%d",c), RooArgSet(*w->var(TString::Format("mgg_sig_m0_cat%d",c)),
								   *w->var(TString::Format("mgg_sig_sigma_cat%d",c)),
								   *w->var(TString::Format("mgg_sig_alpha_cat%d",c)),
								   *w->var(TString::Format("mgg_sig_n_cat%d",c)), 
								   *w->var(TString::Format("mgg_sig_gsigma_cat%d",c)),
								   *w->var(TString::Format("mgg_sig_frac_cat%d",c))) );
								   */

    //read initial values from text file
    w->set(TString::Format("SigPdfParam_cat%d",c))->readFromFile(TString::Format("SigPdfParam/SigPdfParamLast_%d_cat%d.dat",(int)MASS,c));
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


    /// Let's read from the file!
    /// There SHOULD be a smarter way to do this....
    /// First: do we actually have a file for this?
    int iMass = (int)MASS;
    bool weHaveAFileForThis = ((iMass%100)==0);
    int nxj = 1; /// We're doing the 1-jet case only.
    std::string leptType_str = (c<2)?"ELE":"MU";
 
    if(weHaveAFileForThis) {
      /// Ok, open and read the file
      cout << "Take parameters from the file" << endl;
      double params[6];
      ifstream myfile(TString::Format(//"../fits/shape/pars/outpars_BulkG_ZZ_lljj_c0p2_M%d_%dJ__%s.config",
                                      "../fits/shape/pars/outpars_BulkG_WW_lvjj_c0p2_M%d_xww_%dJ__%s.config",
				      iMass,
				      nxj,
				      leptType_str.c_str()));
      ReadFromFile(myfile, params);
      SigModelSet(w, params, c);
    }  
    else {
      cout << "Try to interpolate" << endl;
      /// Ok, open the two neighbouring files and average the values (LOL?)
      double params[6];
      double paramslow[6];
      double paramshigh[6];
      int lowTag = iMass-50;
      int highTag = iMass+50;
      /* ZZ
      ifstream myfileLow(TString::Format("../fits/shape/pars/outpars_BulkG_ZZ_lljj_c0p2_M%d_%dJ__%s.config",
					 lowTag,
					 nxj,
					 leptType_str.c_str()));
      ifstream myfileHigh(TString::Format("../fits/shape/pars/outpars_BulkG_ZZ_lljj_c0p2_M%d_%dJ__%s.config",
					  highTag,
					  nxj,
					  leptType_str.c_str()));
      */

      ifstream myfileLow(TString::Format("../fits/shape/pars/outpars_BulkG_WW_lvjj_c0p2_M%d_xww_%dJ__%s.config",
					 lowTag,
					 nxj,
					 leptType_str.c_str()));
      ifstream myfileHigh(TString::Format("../fits/shape/pars/outpars_BulkG_WW_lvjj_c0p2_M%d_xww_%dJ__%s.config",
					  highTag,
					  nxj,
					  leptType_str.c_str()));
      ReadFromFile(myfileLow, paramslow);
      ReadFromFile(myfileHigh, paramshigh);
      for(int ii=0; ii!=6; ++ii) {
	params[ii] = (paramslow[ii]+paramshigh[ii])/2.0;
      }
      SigModelSet(w, params, c);
    }

    bool redoFit = true;
    if(redoFit) {
      //MggSig[c]     ->fitTo(*sigToFit[c],Range(minMassFit,maxMassFit),SumW2Error(kTRUE));
      MggSig[c]     ->fitTo(*sigToFit[c],Range( TMath::Max(minMassFit,MASS-2.5*MASS*0.1) , MASS+2.5*MASS*0.1 ),SumW2Error(kTRUE));
    }

    //write new fit results to file
    //w->set(TString::Format("SigPdfParam_cat%d",c))->writeToFile(TString::Format("SigPdfParam/SigPdfParamLast_%d_cat%d.dat",(int)MASS,c));
    w->set(TString::Format("SigPdfParam_cat%d",c))->writeToFile(TString::Format("SigPdfParam/SigPdfParamNew_%d_cat%d.dat",(int)MASS,c));

    // IMPORTANT: fix all pdf parameters to constant
    SetConstantParams(w->set(TString::Format("SigPdfParam_cat%d",c)));

    //=== Plots --> MOVE THIS PART IN A SEPARATE FUNCTION?
    //---> RooRealVar *weightVar1 = (RooRealVar*) (*ntplVars)["weight"] ;
    // retrieve mass observable from the workspace
    RooRealVar* massForPlot     = w->var("mZZ");  
    massForPlot->setUnit("GeV");
    Int_t nBinMass(nBinsMassSignal);
    RooPlot* myframe = massForPlot->frame(nBinMass) ;
    sigToFit[c]->plotOn(myframe) ;
    MggSig[c]->plotOn(myframe) ;
    //     MggSig[c]->plotOn(myframe
    // 		      //,
    // 		      //Components(TString::Format("MggGaussSig_cat%d",c)),
    // 		      //Components(TString::Format("MggCBSig_cat%d",c)),
    // 		      //LineStyle(kDashed)
    // 		      );
    //MggSig[c]->paramOn(myframe,sigToFit[c]);
    myframe->GetXaxis()->SetRangeUser( TMath::Max(minMassFit,MASS-2.5*MASS*0.1) , MASS+2.5*MASS*0.1 );
    myframe->SetMinimum(0.);    
    myframe->GetXaxis()->SetTitle("m_{WW} [GeV]");
    myframe->GetYaxis()->SetTitleOffset(1.0);
    myframe->GetYaxis()->SetTitleSize(0.04);
    myframe->GetXaxis()->SetTitleSize(0.04);
    myframe->SetTitle("");

    //TCanvas* canvas = new TCanvas("canvas","canvas",1200,400);
    TCanvas* canvas = new TCanvas("canvas","canvas",0,0,500,500);
    gPad->SetLeftMargin(0.15) ;
    myframe->Draw();
    canvas->SaveAs(TString::Format("plots/SignalShape_m%d_cat%d.png",int(MASS),c));
    canvas->SaveAs(TString::Format("plots/SignalShape_m%d_cat%d.pdf",int(MASS),c));
    canvas->SaveAs(TString::Format("plots/SignalShape_m%d_cat%d.eps",int(MASS),c));
    delete canvas;

  }

  //  w->pdf("MggSig")->fitTo(*w->data("Sig"), Range(minMassFit,maxMassFit),SumW2Error(kTRUE));
}



RooFitResult* BkgModelFitBernstein(RooWorkspace* w, Bool_t dobands) {

  Int_t ncat = NCAT;

  std::vector<TString> catdesc;
  catdesc.push_back("#scale[0.8]{WW_mass_ele_LP}");
  catdesc.push_back("#scale[0.8]{WW_mass_ele_HP}");
  catdesc.push_back("#scale[0.8]{WW_mass_mu_LP}");
  catdesc.push_back("#scale[0.8]{WW_mass_mu_HP}");


  //******************************************//
  // Fit background with model pdfs
  //******************************************//

  // retrieve pdfs and datasets from workspace to fit with pdf models


  RooDataSet* data[NCAT];
  RooBernstein* MggBkg[NCAT];
  RooFitResult* fitresult[NCAT];;
  RooPlot* plotMggBkg[NCAT];
  RooPlot* plotMggBkgPull[NCAT];
  RooHist* histMggBkgPull[NCAT];
  RooDataHist* histdata[NCAT];
  RooHist histdataTest[NCAT];
  RooCurve* bkgFunction[NCAT];
  TH1D *h_data[NCAT];

  // dobands and dosignal
  RooDataSet* signal[NCAT];
  RooAbsPdf*  MggSig[NCAT];

  Float_t minMassFit(MMIN),maxMassFit(MMAX),BinSizePlot(BINSIZEPLOT); 

  // Fit data with background pdf for data limit

  RooRealVar* mgg     = w->var("mZZ");  
  mgg->setUnit("GeV");
  
  TLatex *text = new TLatex();
  text->SetNDC();
  text->SetTextSize(0.04);

  for (int c = 0; c < ncat; ++c) {
    data[c]   = (RooDataSet*) w->data(TString::Format("Data_cat%d",c));
    
    //parameter 3 set to constant            
    ((RooRealVar*) w->var(TString::Format("mgg_bkg_8TeV_slope3_cat%d",c)))->setConstant(true);
    cout << "---------------- Parameter 3 set to const" << endl;

    /*
    ((RooRealVar*) w->var(TString::Format("mgg_bkg_8TeV_slope2_cat%d",c)))->setVal(0); 
    ((RooRealVar*) w->var(TString::Format("mgg_bkg_8TeV_slope2_cat%d",c)))->setConstant(true);
    cout << "---------------- Parameter 2 set to const" << endl;    
    */

    RooFormulaVar *p1mod = new RooFormulaVar(TString::Format("p1mod_cat%d",c),"","@0",*w->var(TString::Format("mgg_bkg_8TeV_slope1_cat%d",c)));
    RooFormulaVar *p2mod = new RooFormulaVar(TString::Format("p2mod_cat%d",c),"","@0",*w->var(TString::Format("mgg_bkg_8TeV_slope2_cat%d",c)));
    RooFormulaVar *p3mod = new RooFormulaVar(TString::Format("p3mod_cat%d",c),"","@0",*w->var(TString::Format("mgg_bkg_8TeV_slope3_cat%d",c)));

    //--
    //shape
    //--
    //-- Standard di-jet function
    //     RooFormulaVar *sqrtS = new RooFormulaVar(TString::Format("sqrtS_cat%d",c),"","@0",*w->var("sqrtS"));
    //     RooFormulaVar *x = new RooFormulaVar(TString::Format("x_cat%d",c),"","@0/@1",RooArgList(*mgg, *sqrtS));    
    //     RooAbsPdf* MggBkgTmp0 = new RooGenericPdf(TString::Format("DijetBackground_%d",c), "pow(1-@0, @1)/pow(@0, @2+@3*log(@0))", RooArgList(*x, *p1mod, *p2mod, *p3mod)); 

    //-- Leveled exponential 
    //   y = exp( -x / ( sigma + alpha*x + beta*x^2 ) )
    RooFormulaVar *xNoScale = new RooFormulaVar(TString::Format("x_NoScale_cat%d",c),"","@0",RooArgList(*mgg));    
    RooAbsPdf* MggBkgTmp0 = new RooGenericPdf(TString::Format("LevelExpoBackground_%d",c), "exp(- @0 / (@1 + @2*@0 + @3*@0*@0) )", RooArgList(*xNoScale, *p1mod, *p2mod, *p3mod)); 
    //
    //

    //--
    //normalization
    //--
    w->factory(TString::Format("mgg_bkg_8TeV_norm_cat%d[4000.0,0.0,10000000]",c));
    RooExtendPdf MggBkgTmp(TString::Format("MggBkg_cat%d",c),"",*MggBkgTmp0,*w->var(TString::Format("mgg_bkg_8TeV_norm_cat%d",c)));

    fitresult[c] = MggBkgTmp.fitTo(*data[c], Strategy(1),Minos(kFALSE), Range(minMassFit,maxMassFit),SumW2Error(kTRUE), Save(kTRUE));
    w->import(MggBkgTmp);

    //************************************************//
    // Plot Mgg background fit results per categories 
    //************************************************//
    // Plot Background Categories 
    //****************************//

    TCanvas* ctmp = new TCanvas("ctmp","M(VV) Background Categories",0,0,500,500);

    TPad* fPads1 = NULL;
    TPad* fPads2 = NULL;

    fPads1 = new TPad("pad1", "", 0.00, 0.28, 0.99, 0.99);
    fPads2 = new TPad("pad2", "", 0.00, 0.00, 0.99, 0.28);

    fPads1->SetFillColor(0);
    fPads1->SetLineColor(0);
    fPads2->SetFillColor(0);
    fPads2->SetLineColor(0);
    fPads1->Draw();
    fPads2->Draw();

    //---- pad1
    fPads1->cd();
    fPads1->SetLogy() ;
    gPad->SetLeftMargin(0.15) ;
    gPad->SetTopMargin(0.05) ;
    gPad->SetBottomMargin(0.12) ;

    Int_t nBinsMass( int( (maxMassFit-minMassFit)/BinSizePlot) );
    plotMggBkg[c] = mgg->frame(nBinsMass);
    plotMggBkg[c]->GetXaxis()->SetTitle("m_{WW} [GeV]");
    plotMggBkg[c]->GetXaxis()->SetTitleOffset(0.8);
    plotMggBkg[c]->GetYaxis()->SetTitleOffset(0.8);
    plotMggBkg[c]->GetYaxis()->SetTitleSize(0.06);
    plotMggBkg[c]->GetXaxis()->SetTitleSize(0.06);
    plotMggBkg[c]->SetTitle("");

    data[c]->plotOn(plotMggBkg[c],Name("ThisData"));    
    //data[c]->plotOn(plotMggBkg[c],DataError(RooAbsData::SumW2));    
    data[c]->statOn(plotMggBkg[c],"N","",2,"NELU",0.66797,0.86551,0.864278) ;
    MggBkgTmp.plotOn(plotMggBkg[c],LineColor(kBlue),Range("fitrange"),NormRange("fitrange"),Name("ThisBackgroundFunction")); 
    MggBkgTmp.paramOn(plotMggBkg[c],data[c],"",2,"NELU", 0.413408,0.887911,0.784027);
    plotMggBkg[c]->SetMinimum(0.1);    
    plotMggBkg[c]->SetMaximum(1000);    
    plotMggBkg[c]->Draw();

    //chi2
    int nparameters = 3;

    //method 1
    Double_t chi2_method1 = plotMggBkg[c]->chiSquare(nparameters);

    //method 2 
    ((RooRealVar*) data[c]->get()->find("mZZ"))->setBins(nBinsMass) ;
    histdata[c] = data[c]->binnedClone();
    cout << "number of events: " << histdata[c]->sumEntries() << endl;
    cout << "number of bins: " << histdata[c]->numEntries() << endl;
    RooChi2Var chi2var ("chi2var", "chi2var", MggBkgTmp, *histdata[c], Extended(kTRUE)); 
    Double_t chi2_val = chi2var.getVal(); 
    Double_t nodf = (histdata[c]->numEntries() - nparameters);
    Double_t chi2_method2 = chi2_val / nodf;

    //method 3 (should match with method1 and it does..)
    histdataTest[c] = (RooHist) plotMggBkg[c]->findObject("ThisData") ;
    bkgFunction[c] = (RooCurve*) plotMggBkg[c]->findObject("ThisBackgroundFunction") ;
    Double_t chi2_method3 = bkgFunction[c]->chiSquare(histdataTest[c],nparameters);

    //method 4 (by hand) --> following http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/ExoDiBosonResonances/EDBRCommon/test/fits/FTestExoVV.cpp?revision=1.4&view=markup
    double chi2_method4_raw=0.;
    double chi2_method4=0.;
    double ndof_method4=0.;

    h_data[c]=(TH1D*)data[c]->createHistogram( TString::Format("h1d_Data_cat%d",c),*mgg,RooFit::Binning(RooBinning(nBinsMass,minMassFit,maxMassFit)));
    h_data[c]->Sumw2();
    
    double integralFit = 0;
    for(int ib=1;ib<=h_data[c]->GetNbinsX();ib++)
      {
	double low=h_data[c]->GetBinLowEdge(ib);
	double center=h_data[c]->GetBinCenter(ib);
	double high=h_data[c]->GetBinLowEdge(ib)+h_data[c]->GetBinWidth(ib);
	double bincont=h_data[c]->GetBinContent(ib); 
	double binerr=h_data[c]->GetBinError(ib);

	double fit=bkgFunction[c]->average(low,high);
	integralFit+=fit;

	if(bincont>0&&binerr>0){
	  double tmpChi2=(bincont-fit)*(bincont-fit)/(binerr*binerr);
	  chi2_method4_raw+=tmpChi2;
	  ndof_method4+=1.0;
	  cout <<"  OK\t\t"<<center << "\t\t"<<bincont<<"\t\t"<<binerr<<"\t\t"<<fit<<"\t\t"<<tmpChi2<<endl;
	}
	else{
	  cout <<"  ZERO ! BinCenter=" <<center <<"  BinCont=" << bincont << "   BinErr=" << binerr << endl;
	}      
      }
    ndof_method4 = ndof_method4 - nparameters;
    chi2_method4 = chi2_method4_raw / ndof_method4;

    cout << "h_data[c]->GetEntries(): " << h_data[c]->GetEntries() << endl;
    cout << "integralFit: " << integralFit << endl;        

    //-- Comparing different methods
    cout << "================ chi2_method1 : " << chi2_method1 << endl;
    cout << "================ chi2_method2 : " << chi2_val  << " / " << nodf << " = " << chi2_method2 << endl;
    cout << "================ chi2_method3 : " << chi2_method3 << endl; //same as method1
    cout << "================ chi2_method4 : " << chi2_method4_raw  << " / " << ndof_method4 << " = " << chi2_method4 << endl; //by hand

    //-- Put chi2 on plot
    char Chi2Text[80]; 
    sprintf(Chi2Text,"#chi^{2} / ndf = %.1f / %d = %.2f", chi2_method4_raw , int(ndof_method4) , chi2_method4); 
    TLatex *texf = new TLatex(maxMassFit/1.0357,15.66,Chi2Text);
    texf->SetTextAlign(32);
    texf->SetTextSize(0.0353107);
    texf->Draw();

    //----

    //---- pad2
    fPads2->cd();
    fPads2->SetGridx();
    fPads2->SetGridy();
    gPad->SetLeftMargin(0.15) ;

    // Construct a histogram with the pulls of the data w.r.t the curve
    histMggBkgPull[c] = plotMggBkg[c]->pullHist("ThisData","ThisBackgroundFunction",true) ;

    // Create a new frame to draw the pull distribution and add the distribution to the frame
    plotMggBkgPull[c] = mgg->frame(nBinsMass) ;
    plotMggBkgPull[c]->GetXaxis()->SetTitle("");
    plotMggBkgPull[c]->GetYaxis()->SetTitle("#frac{data - fit}{#sigma}      ");
    plotMggBkgPull[c]->GetYaxis()->SetTitleOffset(0.4);
    plotMggBkgPull[c]->GetYaxis()->SetTitleSize(0.12);
    plotMggBkgPull[c]->SetTitle("");
    plotMggBkgPull[c]->addPlotable(histMggBkgPull[c],"P") ;
    plotMggBkgPull[c]->SetMinimum(-4);    
    plotMggBkgPull[c]->SetMaximum(4);    

    //modify statbox and fitbox
    TString ParamBoxName = TString::Format("MggBkg_cat%d_paramBox",c);
    cout << "ParamBoxName : " << ParamBoxName << endl;
    //gPad->Update();
    TPaveStats *ParamBox = (TPaveStats*)fPads1->FindObject(ParamBoxName.Data());
    ParamBox->SetLineColor(0);
    ParamBox->SetTextAlign(32);
    ParamBox->SetTextSize(0.0353107);
    ParamBox->SetX1NDC(0.370642);
    ParamBox->SetX2NDC(0.681305);
    ParamBox->SetY1NDC(0.889948);
    ParamBox->SetY2NDC(0.861068);
    ParamBox->SetMargin(0.05);


    plotMggBkgPull[c]->Draw();

    //lines
    TLine* lineAtZero = NULL; 
    TLine* lineAtPlusTwo = NULL; 
    TLine* lineAtMinusTwo = NULL; 

    lineAtZero = new TLine(plotMggBkgPull[c]->GetXaxis()->GetXmin(),0,plotMggBkgPull[c]->GetXaxis()->GetXmax(),0);
    lineAtZero->SetLineColor(2);
    lineAtZero->Draw();
    lineAtPlusTwo = new TLine(plotMggBkgPull[c]->GetXaxis()->GetXmin(),2,plotMggBkgPull[c]->GetXaxis()->GetXmax(),2);
    lineAtPlusTwo->SetLineColor(2);
    lineAtPlusTwo->SetLineStyle(2);
    lineAtPlusTwo->Draw();
    lineAtMinusTwo = new TLine(plotMggBkgPull[c]->GetXaxis()->GetXmin(),-2,plotMggBkgPull[c]->GetXaxis()->GetXmax(),-2);
    lineAtMinusTwo->SetLineColor(2);
    lineAtMinusTwo->SetLineStyle(2);
    lineAtMinusTwo->Draw();

    //----

    ctmp->SaveAs(TString::Format("plots/FitBackground_cat%d.png",c));
    ctmp->SaveAs(TString::Format("plots/FitBackground_cat%d.pdf",c));
    ctmp->SaveAs(TString::Format("plots/FitBackground_cat%d.eps",c));
    ctmp->SaveAs(TString::Format("plots/FitBackground_cat%d.root",c));

    delete ctmp;    

    //********************************************************************************//

    /*    
    if (dobands) {

      RooAbsPdf *cpdf; cpdf = MggBkgTmp0;
      TGraphAsymmErrors *onesigma = new TGraphAsymmErrors();
      TGraphAsymmErrors *twosigma = new TGraphAsymmErrors();
      
      RooRealVar *nlim = new RooRealVar(TString::Format("nlim%d",c),"",0.0,0.0,10.0);
      nlim->removeRange();
      
      RooCurve *nomcurve = dynamic_cast<RooCurve*>(plotMggBkg[c]->getObject(1));
      
      for (int i=1; i<(plotMggBkg[c]->GetXaxis()->GetNbins()+1); ++i) {
	double lowedge = plotMggBkg[c]->GetXaxis()->GetBinLowEdge(i);
	double upedge  = plotMggBkg[c]->GetXaxis()->GetBinUpEdge(i);
	double center  = plotMggBkg[c]->GetXaxis()->GetBinCenter(i);
	
	double nombkg = nomcurve->interpolate(center);
	nlim->setVal(nombkg);
	mgg->setRange("errRange",lowedge,upedge);
	RooAbsPdf *epdf = 0;
	epdf = new RooExtendPdf("epdf","",*cpdf,*nlim,"errRange");
	
	RooAbsReal *nll = epdf->createNLL(*(data[c]),Extended());
	RooMinimizer minim(*nll);
	minim.setStrategy(0);
	double clone = 1.0 - 2.0*RooStats::SignificanceToPValue(1.0);
	double cltwo = 1.0 - 2.0*RooStats::SignificanceToPValue(2.0);
	
	minim.migrad();
	minim.minos(*nlim);
	// printf("errlo = %5f, errhi = %5f\n",nlim->getErrorLo(),nlim->getErrorHi());
	
	onesigma->SetPoint(i-1,center,nombkg);
	onesigma->SetPointError(i-1,0.,0.,-nlim->getErrorLo(),nlim->getErrorHi());
	
	minim.setErrorLevel(0.5*pow(ROOT::Math::normal_quantile(1-0.5*(1-cltwo),1.0), 2)); // the 0.5 is because qmu is -2*NLL
	// eventually if cl = 0.95 this is the usual 1.92!      
	
	
	minim.migrad();
	minim.minos(*nlim);
	
	twosigma->SetPoint(i-1,center,nombkg);
	twosigma->SetPointError(i-1,0.,0.,-nlim->getErrorLo(),nlim->getErrorHi());
	
	
	delete nll;
	delete epdf;
	
      }
      mgg->setRange("errRange",minMassFit,maxMassFit);
      
      twosigma->SetLineColor(kGreen);
      twosigma->SetFillColor(kGreen);
      twosigma->SetMarkerColor(kGreen);
      twosigma->Draw("L3 SAME");
      
      onesigma->SetLineColor(kYellow);
      onesigma->SetFillColor(kYellow);
      onesigma->SetMarkerColor(kYellow);
      onesigma->Draw("L3 SAME");
      
      plotMggBkg[c]->Draw("SAME"); 
     
    }//end dobands if-statement
    */     

  }//end loop over categories
  
  return fitresult;

}

void BkgModelTest(RooWorkspace* w, Bool_t dobands) {


  Int_t ncat = NCAT;

  //******************************************//
  // Fit background with model pdfs
  //******************************************//

  // retrieve pdfs and datasets from workspace to fit with pdf models

  RooDataSet* data[NCAT];
  RooFitResult* fitresult_expo_1par[NCAT];;
  RooFitResult* fitresult_expo_2par[NCAT];;
  RooFitResult* fitresult_expo_3par[NCAT];;
  RooPlot* plotMggBkg[NCAT];
  RooPlot* plotMggBkgPull[NCAT];
  RooHist* histMggBkgPull_expo_1par[NCAT];
  RooHist* histMggBkgPull_expo_2par[NCAT];
  RooHist* histMggBkgPull_expo_3par[NCAT];
  RooCurve* bkgFunction_expo_1par[NCAT];
  RooCurve* bkgFunction_expo_2par[NCAT];
  RooCurve* bkgFunction_expo_3par[NCAT];
  TH1D *h_data[NCAT];

  Float_t minMassFit(MMIN),maxMassFit(MMAX),BinSizePlot(BINSIZEPLOTTEST); 

  // Fit data with background pdf for data limit

  RooRealVar* mgg     = w->var("mZZ");  
  mgg->setUnit("GeV");
  
  for (int c = 0; c < ncat; ++c) {

    //The data to fit
    data[c]   = (RooDataSet*) w->data(TString::Format("Data_cat%d",c));

    //The variable
    RooFormulaVar *xNoScale = new RooFormulaVar(TString::Format("x_NoScale_cat%d",c),"","@0",RooArgList(*mgg));    

    //-- Leveled exponential 
    //   y = exp( -x / ( p1 + p2*x + p3*x^2 ) )

    //Leveled expo - 1par + norm
    RooRealVar *p1_expo_1par=new RooRealVar(TString::Format("p1_expo_1par_cat%d",c),"",15,-100,500);
    RooRealVar *norm_expo_1par=new RooRealVar(TString::Format("norm_expo_1par_cat%d",c),"",4000.0,0.0,10000000);

    //Leveled expo - 2par + norm
    RooRealVar *p1_expo_2par=new RooRealVar(TString::Format("p1_expo_2par_cat%d",c),"",15,-100,500);
    RooRealVar *p2_expo_2par=new RooRealVar(TString::Format("p2_expo_2par_cat%d",c),"",10,0.,100);
    RooRealVar *norm_expo_2par=new RooRealVar(TString::Format("norm_expo_2par_cat%d",c),"",4000.0,0.0,10000000);

    //Leveled expo - 3par + norm
    RooRealVar *p1_expo_3par=new RooRealVar(TString::Format("p1_expo_3par_cat%d",c),"",15,-100,500);
    RooRealVar *p2_expo_3par=new RooRealVar(TString::Format("p2_expo_3par_cat%d",c),"",10,0.,100);
    RooRealVar *p3_expo_3par=new RooRealVar(TString::Format("p3_expo_3par_cat%d",c),"",0,-10,10);
    RooRealVar *norm_expo_3par=new RooRealVar(TString::Format("norm_expo_3par_cat%d",c),"",4000.0,0.0,10000000);

    //Set parameter values
    /*
    p3_expo_3par->setVal(0);
    p3_expo_3par->setConstant(true);
    cout << "---------------- Parameter 3 set to const" << endl;    
    */

    //Define the function
    RooAbsPdf* MggBkgTmp_expo_1par = new RooGenericPdf(TString::Format("LevelExpoBackground_expo_1par_%d",c), "exp(- @0 / (@1) )", RooArgList(*xNoScale, *p1_expo_1par)); 
    RooAbsPdf* MggBkgTmp_expo_2par = new RooGenericPdf(TString::Format("LevelExpoBackground_expo_2par_%d",c), "exp(- @0 / (@1 + @2*@0) )", RooArgList(*xNoScale, *p1_expo_2par, *p2_expo_2par)); 
    RooAbsPdf* MggBkgTmp_expo_3par = new RooGenericPdf(TString::Format("LevelExpoBackground_expo_3par_%d",c), "exp(- @0 / (@1 + @2*@0 + @3*@0*@0) )", RooArgList(*xNoScale, *p1_expo_3par, *p2_expo_3par, *p3_expo_3par)); 

    //Normalization
    RooExtendPdf MggBkg_expo_1par(TString::Format("MggBkg_expo_1par_cat%d",c),"",*MggBkgTmp_expo_1par,*norm_expo_1par);
    RooExtendPdf MggBkg_expo_2par(TString::Format("MggBkg_expo_2par_cat%d",c),"",*MggBkgTmp_expo_2par,*norm_expo_2par);
    RooExtendPdf MggBkg_expo_3par(TString::Format("MggBkg_expo_3par_cat%d",c),"",*MggBkgTmp_expo_3par,*norm_expo_3par);

    //Fit
    fitresult_expo_1par[c] = MggBkg_expo_1par.fitTo(*data[c], Strategy(1),Minos(kFALSE), Range(minMassFit,maxMassFit),SumW2Error(kTRUE), Save(kTRUE));
    p1_expo_2par->setVal( p1_expo_1par->getVal() ); 
    fitresult_expo_2par[c] = MggBkg_expo_2par.fitTo(*data[c], Strategy(1),Minos(kFALSE), Range(minMassFit,maxMassFit),SumW2Error(kTRUE), Save(kTRUE));
    p1_expo_3par->setVal( p1_expo_2par->getVal() ); 
    p2_expo_3par->setVal( p2_expo_2par->getVal() ); 
    fitresult_expo_3par[c] = MggBkg_expo_3par.fitTo(*data[c], Strategy(1),Minos(kFALSE), Range(minMassFit,maxMassFit),SumW2Error(kTRUE), Save(kTRUE));

    //************************************************//
    // Plot Mgg background fit results per categories 
    //************************************************//

    TCanvas* ctmp = new TCanvas("ctmp","Test different functions",0,0,500,500);

    TPad* fPads1 = NULL;
    TPad* fPads2 = NULL;

    fPads1 = new TPad("pad1", "", 0.00, 0.28, 0.99, 0.99);
    fPads2 = new TPad("pad2", "", 0.00, 0.00, 0.99, 0.28);

    fPads1->SetFillColor(0);
    fPads1->SetLineColor(0);
    fPads2->SetFillColor(0);
    fPads2->SetLineColor(0);
    fPads1->Draw();
    fPads2->Draw();

    int color_expo_1par = 4;
    int color_expo_2par = 2;
    int color_expo_3par = 3;

    //---- pad1
    fPads1->cd();
    fPads1->SetLogy() ;
    gPad->SetLeftMargin(0.15) ;
    gPad->SetTopMargin(0.05) ;
    gPad->SetBottomMargin(0.12) ;

    Int_t nBinsMass( int( (maxMassFit-minMassFit)/BinSizePlot) );
    plotMggBkg[c] = mgg->frame(nBinsMass);
    plotMggBkg[c]->GetXaxis()->SetTitle("m_{WW} [GeV]");
    plotMggBkg[c]->GetXaxis()->SetTitleOffset(0.8);
    plotMggBkg[c]->GetYaxis()->SetTitleOffset(0.8);
    plotMggBkg[c]->GetYaxis()->SetTitleSize(0.06);
    plotMggBkg[c]->GetXaxis()->SetTitleSize(0.06);
    plotMggBkg[c]->SetTitle("");

    data[c]->plotOn(plotMggBkg[c],Name("ThisData"));    

    MggBkg_expo_1par.plotOn(plotMggBkg[c],LineColor(color_expo_1par),Range("fitrange"),NormRange("fitrange"),Name("ThisBackgroundFunction_expo_1par")); 
    MggBkg_expo_1par.paramOn(plotMggBkg[c],data[c],"",2,"NELU", 0.175138,0.466357,0.873909);

    MggBkg_expo_2par.plotOn(plotMggBkg[c],LineColor(color_expo_2par),Range("fitrange"),NormRange("fitrange"),Name("ThisBackgroundFunction_expo_2par")); 
    MggBkg_expo_2par.paramOn(plotMggBkg[c],data[c],"",2,"NELU", 0.47654,0.877729,0.877119);

    MggBkg_expo_3par.plotOn(plotMggBkg[c],LineColor(color_expo_3par),Range("fitrange"),NormRange("fitrange"),Name("ThisBackgroundFunction_expo_3par")); 
    MggBkg_expo_3par.paramOn(plotMggBkg[c],data[c],"",2,"NELU", 0.47654,0.877729,0.671674);

    //Draw
    plotMggBkg[c]->SetMinimum(0.1);    
    plotMggBkg[c]->SetMaximum(100000);    
    plotMggBkg[c]->Draw();

    //---- chi2 and F-test
    int nparameters_expo_1par = 1+1;
    int nparameters_expo_2par = 2+1;
    int nparameters_expo_3par = 3+1;

    bkgFunction_expo_1par[c] = (RooCurve*) plotMggBkg[c]->findObject("ThisBackgroundFunction_expo_1par") ;
    bkgFunction_expo_2par[c] = (RooCurve*) plotMggBkg[c]->findObject("ThisBackgroundFunction_expo_2par") ;
    bkgFunction_expo_3par[c] = (RooCurve*) plotMggBkg[c]->findObject("ThisBackgroundFunction_expo_3par") ;

    h_data[c]=(TH1D*)data[c]->createHistogram( TString::Format("h1d_DataTest_cat%d",c),*mgg,RooFit::Binning(RooBinning(nBinsMass,minMassFit,maxMassFit)));
    h_data[c]->Sumw2();

    double chi2_reduce_expo_1par = 0; 
    double chi2_reduce_expo_2par = 0; 
    double chi2_reduce_expo_3par = 0;

    double chi2_expo_1par = 0; 
    double chi2_expo_2par = 0; 
    double chi2_expo_3par = 0;

    double ndof_expo_1par = 0; 
    double ndof_expo_2par = 0; 
    double ndof_expo_3par = 0;
    
    double integralFit_expo_1par = 0; 
    double integralFit_expo_2par = 0; 
    double integralFit_expo_3par = 0;

    //Variables for F-Test: 
    double rss1=0;
    double rss2=0;
    double rss3=0;

    for(int ib=h_data[c]->FindBin(MassForFtest);ib<=h_data[c]->GetNbinsX();ib++)
      {
	double low=h_data[c]->GetBinLowEdge(ib);
	double center=h_data[c]->GetBinCenter(ib);
	double high=h_data[c]->GetBinLowEdge(ib)+h_data[c]->GetBinWidth(ib);
	double bincont=h_data[c]->GetBinContent(ib); 
	double binerr=h_data[c]->GetBinError(ib);

	double fit_expo_1par=bkgFunction_expo_1par[c]->average(low,high);
	double fit_expo_2par=bkgFunction_expo_2par[c]->average(low,high);
	double fit_expo_3par=bkgFunction_expo_3par[c]->average(low,high);

	integralFit_expo_1par+=fit_expo_1par;
	integralFit_expo_2par+=fit_expo_2par;
	integralFit_expo_3par+=fit_expo_3par;

	if(bincont>0&&binerr>0){
	  double tmpChi2_expo_1par=(bincont-fit_expo_1par)*(bincont-fit_expo_1par)/(binerr*binerr);
	  double tmpChi2_expo_2par=(bincont-fit_expo_2par)*(bincont-fit_expo_2par)/(binerr*binerr);
	  double tmpChi2_expo_3par=(bincont-fit_expo_3par)*(bincont-fit_expo_3par)/(binerr*binerr);

	  rss1+=(bincont-fit_expo_1par)*(bincont-fit_expo_1par);
	  rss2+=(bincont-fit_expo_2par)*(bincont-fit_expo_2par);
	  rss3+=(bincont-fit_expo_3par)*(bincont-fit_expo_3par);

	  chi2_expo_1par+=tmpChi2_expo_1par;
	  chi2_expo_2par+=tmpChi2_expo_2par;
	  chi2_expo_3par+=tmpChi2_expo_3par;

	  ndof_expo_1par+=1.0;
	  ndof_expo_2par+=1.0;
	  ndof_expo_3par+=1.0;

	  //cout <<"  OK\t\t"<<center << "\t\t"<<bincont<<"\t\t"<<binerr<<"\t\t"<<fit<<"\t\t"<<tmpChi2<<endl;
	}
	else{
	  //cout <<"  ZERO ! BinCenter=" <<center <<"  BinCont=" << bincont << "   BinErr=" << binerr << endl;
	}      
      }
    ndof_expo_1par = ndof_expo_1par - nparameters_expo_1par;
    ndof_expo_2par = ndof_expo_2par - nparameters_expo_2par;
    ndof_expo_3par = ndof_expo_3par - nparameters_expo_3par;

    chi2_reduce_expo_1par = chi2_expo_1par / ndof_expo_1par;
    chi2_reduce_expo_2par = chi2_expo_2par / ndof_expo_2par;
    chi2_reduce_expo_3par = chi2_expo_3par / ndof_expo_3par;

    char Chi2Text_expo_1par[80]; 
    sprintf(Chi2Text_expo_1par,"#chi^{2} / ndf = %.1f / %d = %.2f", chi2_expo_1par , int(ndof_expo_1par) , chi2_reduce_expo_1par); 
    char Chi2Text_expo_2par[80]; 
    sprintf(Chi2Text_expo_2par,"#chi^{2} / ndf = %.1f / %d = %.2f", chi2_expo_2par , int(ndof_expo_2par) , chi2_reduce_expo_2par); 
    char Chi2Text_expo_3par[80]; 
    sprintf(Chi2Text_expo_3par,"#chi^{2} / ndf = %.1f / %d = %.2f", chi2_expo_3par , int(ndof_expo_3par) , chi2_reduce_expo_3par); 


    //Calculate F-Test
    //http://en.wikipedia.org/wiki/F-test
    //http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/DiJetHighMass/test/fitcode/F_test_default.C?revision=1.3&view=markup
    const double alphaFTest=0.05;
    cout <<"\nStarting F-Test evaluation with alpha-value="<<alphaFTest<<endl;
    cout <<"Mass for f-test: " << MassForFtest << " , corresponding to bin " << h_data[c]->FindBin(MassForFtest) << endl;
    double Ftest_21 = (rss1-rss2)*ndof_expo_2par/ (rss2*1.0);
    double Ftest_32 = (rss2-rss3)*ndof_expo_3par/ (rss3*1.0);
    cout  << "Ftest 21 value = " << Ftest_21 << endl;
    cout  << "Ftest 32 value = " << Ftest_32 << endl;
    double good_CL21 =  1.-TMath::FDistI(Ftest_21,1,ndof_expo_2par);
    double good_CL32 =  1.-TMath::FDistI(Ftest_32,1,ndof_expo_3par);
    cout  << " Ftest21 CL = " << good_CL21 << endl;
    cout  << " Ftest32 CL = " << good_CL32 << endl;
    if (good_CL21>alphaFTest) {
      cout  << " No need for anything more than a simple exponential" << endl;
    } else if (good_CL32>alphaFTest) {
      cout  << "A levelled exponential with 2 parameters is needed for these data. " << endl;
    } else {
      cout  << " A levelled exponential with 3 parameters is needed for these data. " << endl;
      cout  << " You should check for higher-order functions. " << endl;
    }
    cout  << endl;   

    //----

    //modify statbox and fitbox
    TString ParamBoxName_expo_1par = TString::Format("MggBkg_expo_1par_cat%d_paramBox",c);
    TPaveStats *ParamBox_expo_1par = (TPaveStats*)fPads1->FindObject(ParamBoxName_expo_1par.Data());
    ParamBox_expo_1par->SetFillColor(kWhite);
    ParamBox_expo_1par->SetLineColor(color_expo_1par);
    ParamBox_expo_1par->SetTextAlign(32);
    ParamBox_expo_1par->SetTextSize(0.0288906);
    ParamBox_expo_1par->SetX1NDC(0.175138);
    ParamBox_expo_1par->SetX2NDC(0.466357);
    ParamBox_expo_1par->SetY1NDC(0.748716);
    ParamBox_expo_1par->SetY2NDC(0.873909);
    ParamBox_expo_1par->SetMargin(0.05);
    ParamBox_expo_1par->AddText(Chi2Text_expo_1par);

    TString ParamBoxName_expo_2par = TString::Format("MggBkg_expo_2par_cat%d_paramBox",c);
    TPaveStats *ParamBox_expo_2par = (TPaveStats*)fPads1->FindObject(ParamBoxName_expo_2par.Data());
    ParamBox_expo_2par->SetFillColor(kWhite);
    ParamBox_expo_2par->SetLineColor(color_expo_2par);
    ParamBox_expo_2par->SetTextAlign(32);
    ParamBox_expo_2par->SetTextSize(0.0288906);
    ParamBox_expo_2par->SetX1NDC(0.492832);
    ParamBox_expo_2par->SetX2NDC(0.853291);
    ParamBox_expo_2par->SetY1NDC(0.690935);
    ParamBox_expo_2par->SetY2NDC(0.873909);
    ParamBox_expo_2par->SetMargin(0.05);
    ParamBox_expo_2par->AddText(Chi2Text_expo_2par);

    TString ParamBoxName_expo_3par = TString::Format("MggBkg_expo_3par_cat%d_paramBox",c);
    TPaveStats *ParamBox_expo_3par = (TPaveStats*)fPads1->FindObject(ParamBoxName_expo_3par.Data());
    ParamBox_expo_3par->SetFillColor(kWhite);
    ParamBox_expo_3par->SetLineColor(color_expo_3par);
    ParamBox_expo_3par->SetTextAlign(32);
    ParamBox_expo_3par->SetTextSize(0.0288906);
    ParamBox_expo_3par->SetX1NDC(0.492832);
    ParamBox_expo_3par->SetX2NDC(0.855327);
    ParamBox_expo_3par->SetY1NDC(0.430919);
    ParamBox_expo_3par->SetY2NDC(0.671674);
    ParamBox_expo_3par->SetMargin(0.05);
    ParamBox_expo_3par->AddText(Chi2Text_expo_3par);


    //---- pad2
    fPads2->cd();
    fPads2->SetGridx();
    fPads2->SetGridy();
    gPad->SetLeftMargin(0.15) ;

    // Construct a histogram with the pulls of the data w.r.t the curve
    histMggBkgPull_expo_1par[c] = plotMggBkg[c]->pullHist("ThisData","ThisBackgroundFunction_expo_1par",true) ;
    histMggBkgPull_expo_2par[c] = plotMggBkg[c]->pullHist("ThisData","ThisBackgroundFunction_expo_2par",true) ;
    histMggBkgPull_expo_3par[c] = plotMggBkg[c]->pullHist("ThisData","ThisBackgroundFunction_expo_3par",true) ;

    histMggBkgPull_expo_1par[c]->SetMarkerColor(color_expo_1par);
    histMggBkgPull_expo_2par[c]->SetMarkerColor(color_expo_2par);
    histMggBkgPull_expo_3par[c]->SetMarkerColor(color_expo_3par);

    histMggBkgPull_expo_1par[c]->SetMarkerStyle(21);
    histMggBkgPull_expo_2par[c]->SetMarkerStyle(20);
    histMggBkgPull_expo_3par[c]->SetMarkerStyle(23);

    histMggBkgPull_expo_1par[c]->SetMarkerSize(0.8);
    histMggBkgPull_expo_2par[c]->SetMarkerSize(1);
    histMggBkgPull_expo_3par[c]->SetMarkerSize(0.8);

    histMggBkgPull_expo_1par[c]->SetLineColor(color_expo_1par);
    histMggBkgPull_expo_2par[c]->SetLineColor(color_expo_2par);
    histMggBkgPull_expo_3par[c]->SetLineColor(color_expo_3par);
    
    // Create a new frame to draw the pull distribution and add the distribution to the frame
    plotMggBkgPull[c] = mgg->frame(nBinsMass) ;
    plotMggBkgPull[c]->GetXaxis()->SetTitle("");
    plotMggBkgPull[c]->GetYaxis()->SetTitle("#frac{data - fit}{#sigma}      ");
    plotMggBkgPull[c]->GetYaxis()->SetTitleOffset(0.4);
    plotMggBkgPull[c]->GetYaxis()->SetTitleSize(0.12);
    plotMggBkgPull[c]->SetTitle("");
    plotMggBkgPull[c]->addPlotable(histMggBkgPull_expo_1par[c],"P") ;
    plotMggBkgPull[c]->addPlotable(histMggBkgPull_expo_2par[c],"P") ;
    plotMggBkgPull[c]->addPlotable(histMggBkgPull_expo_3par[c],"P") ;
    plotMggBkgPull[c]->SetMinimum(-4);    
    plotMggBkgPull[c]->SetMaximum(4);    

    plotMggBkgPull[c]->Draw();

    //lines
    TLine* lineAtZero = NULL; 
    TLine* lineAtPlusTwo = NULL; 
    TLine* lineAtMinusTwo = NULL; 

    lineAtZero = new TLine(plotMggBkgPull[c]->GetXaxis()->GetXmin(),0,plotMggBkgPull[c]->GetXaxis()->GetXmax(),0);
    lineAtZero->SetLineColor(2);
    lineAtZero->Draw();
    lineAtPlusTwo = new TLine(plotMggBkgPull[c]->GetXaxis()->GetXmin(),2,plotMggBkgPull[c]->GetXaxis()->GetXmax(),2);
    lineAtPlusTwo->SetLineColor(2);
    lineAtPlusTwo->SetLineStyle(2);
    lineAtPlusTwo->Draw();
    lineAtMinusTwo = new TLine(plotMggBkgPull[c]->GetXaxis()->GetXmin(),-2,plotMggBkgPull[c]->GetXaxis()->GetXmax(),-2);
    lineAtMinusTwo->SetLineColor(2);
    lineAtMinusTwo->SetLineStyle(2);
    lineAtMinusTwo->Draw();

    //----

    ctmp->SaveAs(TString::Format("plots/FitBackground_test_cat%d.png",c));
    ctmp->SaveAs(TString::Format("plots/FitBackground_test_cat%d.pdf",c));
    ctmp->SaveAs(TString::Format("plots/FitBackground_test_cat%d.eps",c));
    ctmp->SaveAs(TString::Format("plots/FitBackground_test_cat%d.root",c));


    //----

    delete ctmp;    

    //--
    //shape
    //--
    //-- Standard di-jet function
    //     RooFormulaVar *sqrtS = new RooFormulaVar(TString::Format("sqrtS_cat%d",c),"","@0",*w->var("sqrtS"));
    //     RooFormulaVar *x = new RooFormulaVar(TString::Format("x_cat%d",c),"","@0/@1",RooArgList(*mgg, *sqrtS));    
    //     RooAbsPdf* MggBkgTmp0 = new RooGenericPdf(TString::Format("DijetBackground_%d",c), "pow(1-@0, @1)/pow(@0, @2+@3*log(@0))", RooArgList(*x, *p1mod, *p2mod, *p3mod)); 

  }
}


void SetConstantParams(const RooArgSet* params) {

  TIterator* iter(params->createIterator());
  for (TObject *a = iter->Next(); a != 0; a = iter->Next()) {
    RooRealVar *rrv = dynamic_cast<RooRealVar *>(a);
    if (rrv) { rrv->setConstant(true); std::cout << " " << rrv->GetName(); }
  }  

}

void MakePlots(RooWorkspace* w, Float_t mass, RooFitResult* fitresults, bool isWW) {

  cout << "Start plotting" << endl; 
  
  Int_t ncat = NCAT;

  std::vector<TString> catdesc;
  catdesc.push_back("#scale[0.8]{WW_mass_ele_HP}");
  catdesc.push_back("#scale[0.8]{WW_mass_ele_LP}");
  catdesc.push_back("#scale[0.8]{WW_mass_mu_HP}");
  catdesc.push_back("#scale[0.8]{WW_mass_mu_LP}");
  
// retrieve data sets from the workspace
  RooDataSet* dataAll         = (RooDataSet*) w->data("Data");
  RooDataSet* signalAll       = (RooDataSet*) w->data("Sig");

  RooDataSet* data[9];  
  RooDataSet* signal[9];
  RooAbsPdf*  MggGaussSig[9];
  RooAbsPdf*  MggCBSig[9];
  RooAbsPdf*  MggSig[9];
  RooAbsPdf*  MggBkg[9];  
//  RooAbsPdf*  MggBkg2[9];  

  for (int c = 0; c < ncat; ++c) {
    data[c]         = (RooDataSet*) w->data(TString::Format("Data_cat%d",c));
//    signal[c]       = (RooDataSet*) w->data(TString::Format("Sig_cat%d",c));
    signal[c]       = (RooDataSet*) w->data(TString::Format("SigWeight_cat%d",c));
    MggGaussSig[c]  = (RooAbsPdf*)  w->pdf(TString::Format("MggGaussSig_cat%d",c));
    MggCBSig[c]     = (RooAbsPdf*)  w->pdf(TString::Format("MggCBSig_cat%d",c));
    MggSig[c]       = (RooAbsPdf*)  w->pdf(TString::Format("MggSig_cat%d",c));
    MggBkg[c]       = (RooAbsPdf*)  w->pdf(TString::Format("MggBkg_cat%d",c));
//    MggBkg2[c]      = (RooAbsPdf*)  w->pdf(TString::Format("MggBkg2_cat%d",c));
  }

// retrieve mass observable from the workspace
  RooRealVar* mgg     = w->var("mZZ");  
  mgg->setUnit("GeV");

// retrieve pdfs after the fits
// Signal Model

  RooAbsPdf* MggGaussSigAll  = w->pdf("MggGaussSig");
  RooAbsPdf* MggCBSigAll     = w->pdf("MggCBSig");
  RooAbsPdf* MggSigAll       = w->pdf("MggSig");

//  RooAbsPdf* MggBkgAll       = w->pdf("MggBkg");
  RooAbsPdf* MggBkgAll       = w->pdf("MggBkgAll");
  
  cout << "Progress plotting" << endl;
 
  //****************************//
  // Plot Mgg Fit results
  //****************************//
  // Set P.D.F. parameter names
  // WARNING: Do not use it if Workspaces are created
  //  SetParamNames(w);
  
  
  Float_t minMassFit(MMIN),maxMassFit(MMAX); 
  Float_t MASS(mass);

  Int_t nBinsMass(50);
  
  RooPlot* plotMggAll = mgg->frame(Range(minMassFit,maxMassFit),Bins(nBinsMass));
  signalAll->plotOn(plotMggAll);
 
  gStyle->SetOptTitle(0);
  MggSigAll->plotOn(plotMggAll);
  MggSigAll->plotOn(plotMggAll,Components("MggGaussSig"),LineStyle(kDashed),LineColor(kGreen));
  MggSigAll->plotOn(plotMggAll,Components("MggCBSig"),LineStyle(kDashed),LineColor(kRed));

  MggSigAll->paramOn(plotMggAll, ShowConstants(true), Layout(0.15,0.55,0.9), Format("NEU",AutoPrecision(2)));
  plotMggAll->getAttText()->SetTextSize(0.03);

  TCanvas* c1 = new TCanvas("c1","M(VV)",0,0,500,500);
  c1->cd(1);
  plotMggAll->Draw();  


  cout << "Progress plotting" << endl;

//********************************************//
// Plot Mgg signal fit results per categories 
//********************************************//
// Plot Signal Categories 
//****************************//

  TLatex *text = new TLatex();
  text->SetNDC();
  text->SetTextSize(0.04);

//  TCanvas* c2 = new TCanvas("c2","Mgg Categories",0,0,1000,1000);

//  c2->Divide(3,3);
  RooPlot* plotMgg[9];
  for (int c = 0; c < ncat; ++c) {
    plotMgg[c] = mgg->frame(Range(minMassFit,maxMassFit),Bins(nBinsMass));
    signal[c]->plotOn(plotMgg[c],LineColor(kWhite),MarkerColor(kWhite));    

    MggSig[c]  ->plotOn(plotMgg[c]);
    MggSig[c]  ->plotOn(plotMgg[c],Components(TString::Format("MggGaussSig_cat%d",c)),LineStyle(kDashed),LineColor(kGreen));
    MggSig[c]  ->plotOn(plotMgg[c],Components(TString::Format("MggCBSig_cat%d",c)),LineStyle(kDashed),LineColor(kRed));
    

    MggSig[c]  ->paramOn(plotMgg[c]);
    signal[c]  ->plotOn(plotMgg[c]);

        
    TCanvas* dummy = new TCanvas("dummy", "dummy",0, 0, 400, 400);
    TH1F *hist = new TH1F("hist", "hist", 400, minMassFit, maxMassFit);
 
    plotMgg[c]->SetTitle("");      
    plotMgg[c]->SetMinimum(0.0);
    plotMgg[c]->SetMaximum(1.40*plotMgg[c]->GetMaximum());
    plotMgg[c]->GetXaxis()->SetTitle("m_{#gamma#gamma} (GeV)");

    TCanvas* ctmp = new TCanvas("ctmp","M(VV) Background Categories",0,0,500,500);
    plotMgg[c]->Draw();  
//    hist->Draw("same");
    
    plotMgg[c]->Draw("SAME");  
    TLegend *legmc = new TLegend(0.62,0.75,0.92,0.9);
    legmc->AddEntry(plotMgg[c]->getObject(5),"Simulation","LPE");
    legmc->AddEntry(plotMgg[c]->getObject(1),"Parametric Model","L");
    legmc->AddEntry(plotMgg[c]->getObject(3),"Crystal Ball component","L");
    legmc->AddEntry(plotMgg[c]->getObject(2),"Gaussian Outliers","L");
    
    legmc->SetBorderSize(0);
    legmc->SetFillStyle(0);
    legmc->Draw();    
    
    float effS = effSigma(hist);
//    text->DrawLatex(0.65,0.4, TString::Format("#sigma_{eff} = %.2f GeV",effS));
//    cout<<"effective sigma [" << c << "] = " << effS <<endl;
    
    TLatex *lat  = new TLatex(minMassFit+1.5,0.85*plotMgg[c]->GetMaximum(),"#scale[1.0]{CMS Preliminary}");
    lat->Draw();
    TLatex *lat2 = new TLatex(minMassFit+1.5,0.75*plotMgg[c]->GetMaximum(),catdesc.at(c));
    lat2->Draw();
    TLatex *lat3 = new TLatex(minMassFit+1.5,0.55*plotMgg[c]->GetMaximum(),TString::Format("#scale[0.8]{#sigma_{eff} = %.2f GeV}",effS));
    lat3->Draw();

    int iMass = abs(mass);

    if (!isWW){
      ctmp->SaveAs(TString::Format("plots/sigmodel_ZZ%d_cat%d.png", iMass, c));
      ctmp->SaveAs(TString::Format("plots/sigmodel_ZZ%d_cat%d.pdf", iMass, c));
      ctmp->SaveAs(TString::Format("plots/sigmodel_ZZ%d_cat%d.eps", iMass, c));
      ctmp->SaveAs(TString::Format("plots/sigmodel_ZZ%d_cat%d.C", iMass, c));
    } else {
      ctmp->SaveAs(TString::Format("plots/sigmodel_WW%d_cat%d.png", iMass, c));
      ctmp->SaveAs(TString::Format("plots/sigmodel_WW%d_cat%d.pdf", iMass, c));
      ctmp->SaveAs(TString::Format("plots/sigmodel_WW%d_cat%d.eps", iMass, c));
      ctmp->SaveAs(TString::Format("plots/sigmodel_WW%d_cat%d.C", iMass, c));
    }


  }


//****************************//
// Plot Mgg Background 
//****************************//
/*
  nBinsMass=80;
  RooPlot* plotMggBkgAll = mgg->frame(nBinsMass);
  dataAll->plotOn(plotMggBkgAll,LineColor(kWhite),MarkerColor(kWhite)); 
  fitresults->Print();

  MggBkgAll->plotOn(plotMggBkgAll,FillColor(kYellow),Range("fitrange"),NormRange("fitrange"), VisualizeError(*fitresults, 2.0,kFALSE));
  MggBkgAll->plotOn(plotMggBkgAll,FillColor(kGreen),Range("fitrange"),NormRange("fitrange"), VisualizeError(*fitresults, 1.0,kFALSE));
  MggBkgAll->plotOn(plotMggBkgAll,LineColor(kBlue),Range("fitrange"),NormRange("fitrange"));
  MggBkgAll->plotOn(plotMggBkgAll,Components("MggBkgAll2"),LineStyle(kDashed),LineColor(kRed));
  dataAll->plotOn(plotMggBkgAll); 
//  MggBkgAll->paramOn(plotMggBkgAll, ShowConstants(true), Layout(0.65,0.9,0.9), Format("NEU",AutoPrecision(4)));
//  plotMggBkgAll->getAttText()->SetTextSize(0.03);



  TCanvas* c3 = new TCanvas("c3","Mgg Background",0,0,500,500);
  plotMggBkgAll->Draw();  
  TLegend *legmc = new TLegend(0.62,0.75,0.92,0.9);
  legmc->AddEntry(plotMggBkgAll->getObject(4),"Data (5.3/fb)","LPE");
  legmc->AddEntry(plotMggBkgAll->getObject(3),"Bkg Model","L");
  legmc->AddEntry(plotMggBkgAll->getObject(2),"#pm1 #sigma","F");
  legmc->AddEntry(plotMggBkgAll->getObject(1),"#pm2 #sigma","F");
  legmc->SetBorderSize(0);
  legmc->SetFillStyle(0);
  legmc->Draw();

  text->DrawLatex(0.2,0.85,"CMS Preliminary");
  text->DrawLatex(0.2,0.78,"#sqrt{s} = 8 TeV L =5.3 fb^{-1}");

*/
//************************************************//
// Plot Mgg background fit results per categories 
//************************************************//
// Plot Background Categories 
//****************************//

  TCanvas* c4 = new TCanvas("c4","M(VV) Background Categories",0,0,1000,1000);
  c4->Divide(2,3);

  RooPlot* plotMggBkg[9];
  for (int c = 0; c < ncat; ++c) {
    plotMggBkg[c] = mgg->frame(nBinsMass);
    data[c]->plotOn(plotMggBkg[c],LineColor(kWhite),MarkerColor(kWhite));    
    MggBkg[c]->plotOn(plotMggBkg[c],LineColor(kBlue),Range("fitrange"),NormRange("fitrange")); 
    data[c]->plotOn(plotMggBkg[c]);    
    MggBkg[c]->paramOn(plotMggBkg[c], ShowConstants(true), Layout(0.65,0.9,0.9), Format("NEU",AutoPrecision(4)));
    plotMggBkg[c]->getAttText()->SetTextSize(0.03);
    c4->cd(c+1);
    plotMggBkg[c]->Draw();  
    gPad->SetLogy(1);
  }

  c4->SaveAs(TString::Format("plots/backgrounds_log.png",c));
  c4->SaveAs(TString::Format("plots/backgrounds_log.pdf",c));
  c4->SaveAs(TString::Format("plots/backgrounds_log.eps",c));
  c4->SaveAs(TString::Format("plots/backgrounds_log.C",c));



  TCanvas* c5 = new TCanvas("c5","M(VV) Background Categories",0,0,1000,1000);
  c5->Divide(2,3);

  RooPlot* plotMggBkg[9];
  for (int c = 0; c < ncat; ++c) {
    plotMggBkg[c] = mgg->frame(nBinsMass);
    data[c]->plotOn(plotMggBkg[c],LineColor(kWhite),MarkerColor(kWhite));    
    MggBkg[c]->plotOn(plotMggBkg[c],LineColor(kBlue),Range("fitrange"),NormRange("fitrange")); 
    data[c]->plotOn(plotMggBkg[c]);    
    MggBkg[c]->paramOn(plotMggBkg[c], ShowConstants(true), Layout(0.65,0.9,0.9), Format("NEU",AutoPrecision(4)));
    plotMggBkg[c]->getAttText()->SetTextSize(0.03);
    c5->cd(c+1);
    plotMggBkg[c]->Draw();  
  }

  c5->SaveAs(TString::Format("plots/backgrounds.png",c));
  c5->SaveAs(TString::Format("plots/backgrounds.pdf",c));
  c5->SaveAs(TString::Format("plots/backgrounds.eps",c));
  c5->SaveAs(TString::Format("plots/backgrounds.C",c));

}

//OLD
void SetParamNames(RooWorkspace* w) {
  
  Int_t ncat = NCAT;

//****************************//
// Mgg signal all categories
//****************************//

  RooRealVar* mgg_sig_m0     = w->var("mgg_sig_m0");  
  RooRealVar* mgg_sig_sigma  = w->var("mgg_sig_sigma");
  RooRealVar* mgg_sig_alpha  = w->var("mgg_sig_alpha"); 
  RooRealVar* mgg_sig_n      = w->var("mgg_sig_n"); 
  RooRealVar* mgg_sig_gsigma = w->var("mgg_sig_gsigma");
  RooRealVar* mgg_sig_frac   = w->var("mgg_sig_frac");

  mgg_sig_m0    ->SetName("m_{0}");
  mgg_sig_sigma ->SetName("#sigma_{CB}");
  mgg_sig_alpha ->SetName("#alpha");
  mgg_sig_n     ->SetName("n");
  mgg_sig_gsigma->SetName("#sigma_G");  
  mgg_sig_frac  ->SetName("f_G");  

  mgg_sig_m0    ->setUnit("GeV");
  mgg_sig_sigma ->setUnit("GeV");
  mgg_sig_gsigma->setUnit("GeV"); 

//****************************//
// Mgg background  
//****************************//

  RooRealVar* mgg_bkg_8TeV_slope1  = w->var("mgg_bkg_8TeV_slope1");
  mgg_bkg_8TeV_slope1              ->SetName("a_{B}");
  mgg_bkg_8TeV_slope1              ->setUnit("1/GeV");

  RooRealVar* mgg_bkg_8TeV_slope2  = w->var("mgg_bkg_8TeV_slope2");
  mgg_bkg_8TeV_slope2              ->SetName("a_{B}");
  mgg_bkg_8TeV_slope2              ->setUnit("1/GeV");



//****************************//
// Mgg per category  
//****************************//

  for (int c = 0; c < ncat; ++c) {


    mgg_sig_m0     = (RooRealVar*) w->var(TString::Format("mgg_sig_m0_cat%d",c));
    mgg_sig_sigma  = (RooRealVar*) w->var(TString::Format("mgg_sig_sigma_cat%d",c));
    mgg_sig_alpha  = (RooRealVar*) w->var(TString::Format("mgg_sig_alpha_cat%d",c));
    mgg_sig_n      = (RooRealVar*) w->var(TString::Format("mgg_sig_n_cat%d",c));
    mgg_sig_gsigma = (RooRealVar*) w->var(TString::Format("mgg_sig_gsigma_cat%d",c));
    mgg_sig_frac   = (RooRealVar*) w->var(TString::Format("mgg_sig_frac_cat%d",c));

    mgg_sig_m0     ->SetName("m_{0}"); 
    mgg_sig_sigma  ->SetName("#sigma_{CB}");
    mgg_sig_alpha  ->SetName("#alpha");
    mgg_sig_n      ->SetName("n"); 
    mgg_sig_gsigma ->SetName("#sigma_{G}");
    mgg_sig_frac   ->SetName("f_{G}");


    mgg_sig_m0     ->setUnit("GeV");
    mgg_sig_sigma  ->setUnit("GeV");
    mgg_sig_gsigma ->setUnit("GeV");


    mgg_bkg_8TeV_slope1 = w->var(TString::Format("mgg_bkg_8TeV_slope1_cat%d",c));
    mgg_bkg_8TeV_slope1 ->SetName("p_{B}^{1}");
    mgg_bkg_8TeV_slope1 ->setUnit("1/GeV");

    mgg_bkg_8TeV_slope2 = w->var(TString::Format("mgg_bkg_8TeV_slope2_cat%d",c));
    mgg_bkg_8TeV_slope2 ->SetName("p_{B}^{2}");
    mgg_bkg_8TeV_slope2 ->setUnit("1/GeV^{2}");

//    RooRealVar* mgg_bkg_8TeV_frac = w->var(TString::Format("mgg_bkg_8TeV_frac_cat%d",c));
//    mgg_bkg_8TeV_frac ->SetName("f");

  }

}



void MakeSigWS(RooWorkspace* w, const char* fileBaseName) {

  TString wsDir   = "workspaces/";
  Int_t ncat = NCAT;

  //**********************************************************************//
  // Write pdfs and datasets into the workspace 
  // for statistical tests. 
  // 0 ele_LP
  // 1 ele_HP
  // 2 mu_LP
  // 3 mu_HP
  //**********************************************************************//

  //********************************//
  // Retrieve P.D.F.s
  //********************************//

  RooAbsPdf* MggSigPdf[4];
  
  // (1) import signal P.D.F.s

  RooWorkspace *wAll = new RooWorkspace("w_all","w_all");


  for (int c = 0; c < ncat; ++c) {
    MggSigPdf[c] = (RooAbsPdf*)  w->pdf(TString::Format("MggSig_cat%d",c)); //actually, not used...
    wAll->import(*w->pdf(TString::Format("MggSig_cat%d",c)));
  }

  // (2) Systematics on energy scale and resolution

  //was [1,1.0,1.0].....
  wAll->factory("CMS_hgg_sig_m0_absShift[1,0.5,1.5]");
  /*
  wAll->factory("CMS_hgg_sig_m0_absShift_cat0[1,0.5,1.5]");
  wAll->factory("CMS_hgg_sig_m0_absShift_cat1[1,0.5,1.5]");
  wAll->factory("CMS_hgg_sig_m0_absShift_cat2[1,0.5,1.5]");
  wAll->factory("CMS_hgg_sig_m0_absShift_cat3[1,0.5,1.5]");
  */

  //multiply by scale factor
  wAll->factory("prod::CMS_hgg_sig_m0_cat0(mgg_sig_m0_cat0, CMS_hgg_sig_m0_absShift)");
  wAll->factory("prod::CMS_hgg_sig_m0_cat1(mgg_sig_m0_cat1, CMS_hgg_sig_m0_absShift)");
  wAll->factory("prod::CMS_hgg_sig_m0_cat2(mgg_sig_m0_cat2, CMS_hgg_sig_m0_absShift)");
  wAll->factory("prod::CMS_hgg_sig_m0_cat3(mgg_sig_m0_cat3, CMS_hgg_sig_m0_absShift)");
  /*
  wAll->factory("prod::CMS_hgg_sig_m0_cat0(mgg_sig_m0_cat0, CMS_hgg_sig_m0_absShift_cat0)");
  wAll->factory("prod::CMS_hgg_sig_m0_cat1(mgg_sig_m0_cat1, CMS_hgg_sig_m0_absShift_cat1)");
  wAll->factory("prod::CMS_hgg_sig_m0_cat2(mgg_sig_m0_cat2, CMS_hgg_sig_m0_absShift_cat2)");
  wAll->factory("prod::CMS_hgg_sig_m0_cat3(mgg_sig_m0_cat3, CMS_hgg_sig_m0_absShift_cat3)");
  */

  // (3) Systematics on resolution: create new sigmas
  
  //was [1,1.0,1.0].....
  wAll->factory("CMS_hgg_sig_sigmaScale[1,0.5,1.5]");
  /*
  wAll->factory("CMS_hgg_sig_sigmaScale_cat0[1,0.5,1.5]");
  wAll->factory("CMS_hgg_sig_sigmaScale_cat1[1,0.5,1.5]");
  wAll->factory("CMS_hgg_sig_sigmaScale_cat2[1,0.5,1.5]");
  wAll->factory("CMS_hgg_sig_sigmaScale_cat3[1,0.5,1.5]");
  */

  //multiply by scale factor
  wAll->factory("prod::CMS_hgg_sig_sigma_cat0(mgg_sig_sigma_cat0, CMS_hgg_sig_sigmaScale)");
  wAll->factory("prod::CMS_hgg_sig_sigma_cat1(mgg_sig_sigma_cat1, CMS_hgg_sig_sigmaScale)");
  wAll->factory("prod::CMS_hgg_sig_sigma_cat2(mgg_sig_sigma_cat2, CMS_hgg_sig_sigmaScale)");
  wAll->factory("prod::CMS_hgg_sig_sigma_cat3(mgg_sig_sigma_cat3, CMS_hgg_sig_sigmaScale)");
  /*
  wAll->factory("prod::CMS_hgg_sig_sigma_cat0(mgg_sig_sigma_cat0, CMS_hgg_sig_sigmaScale_cat0)");
  wAll->factory("prod::CMS_hgg_sig_sigma_cat1(mgg_sig_sigma_cat1, CMS_hgg_sig_sigmaScale_cat1)");
  wAll->factory("prod::CMS_hgg_sig_sigma_cat2(mgg_sig_sigma_cat2, CMS_hgg_sig_sigmaScale_cat2)");
  wAll->factory("prod::CMS_hgg_sig_sigma_cat3(mgg_sig_sigma_cat3, CMS_hgg_sig_sigmaScale_cat3)");
  */

  // (4) do reparametrization of signal
  for (int c = 0; c < ncat; ++c) {
    wAll->factory(
		  TString::Format("EDIT::CMS_hgg_sig_cat%d(MggSig_cat%d,",c,c) +
		  TString::Format(" mgg_sig_m0_cat%d=CMS_hgg_sig_m0_cat%d, ", c,c) +
		  TString::Format(" mgg_sig_sigma_cat%d=CMS_hgg_sig_sigma_cat%d) ", c,c)
		  );
                 //+ TString::Format(" mgg_sig_gsigma_cat%d=CMS_hgg_sig_gsigma_cat%d)", c,c) //OLD
    
  }

  TString filename(wsDir+TString(fileBaseName)+".inputsig.root");
  wAll->writeToFile(filename);
  cout << "Write signal workspace in: " << filename << " file" << endl;

  return;
}


void MakeBkgWS(RooWorkspace* w, const char* fileBaseName) {

  TString wsDir   = "workspaces/";
  Int_t ncat = NCAT;  

  //**********************************************************************//
  // Write pdfs and datasets into the workspace 
  // for statistical tests. 
  // 
  // Suffix to attribute belonging to given the category
  // "catN"    - N shower shape, pseudo-rapidity, pt or Njets categories 
  //  N=0-3    - default shower shape, pseudo-rapidity 4 categories
  //  N=0-3    - Pt<40 GeV or 0-jet (Central Jet Veto)
  //  N=4-7    - Pt>40 GeV or 1-jet (one-jet tagging)
  //**********************************************************************//

  //********************************//
  // Retrieve the datasets and PDFs
  //********************************//

  RooDataSet* data[NCAT];
  RooExtendPdf* MggBkgPdf[NCAT];

  // (1) import everything

  cout << "Start importing everything for data" << endl;

  RooWorkspace *wAll = new RooWorkspace("w_all","w_all");

  for (int c = 0; c < ncat; ++c) {
 
    cout << "For category " << c << endl;

    data[c]      = (RooDataSet*) w->data(TString::Format("Data_cat%d",c));
    //binning the dataset (1 GeV) --> why not unbinned?
    ((RooRealVar*) data[c]->get()->find("mZZ"))->setBins(MMAX-MMIN) ;
    RooDataHist* dataBinned = data[c]->binnedClone();

    MggBkgPdf[c] = (RooExtendPdf*)  w->pdf(TString::Format("MggBkg_cat%d",c)); //actually, not used...
    //wAll->import(*data[c], Rename(TString::Format("data_obs_cat%d",c))); // unbinned 
    wAll->import(*dataBinned, Rename(TString::Format("data_obs_cat%d",c)));   // binned
    wAll->import(*w->pdf(TString::Format("MggBkg_cat%d",c)));

    //NOTE: Check the ranges in the file : Xvv_models_Bkg_8TeV_test.rs
    double mean = wAll->var(TString::Format("mgg_bkg_8TeV_norm_cat%d",c))->getVal();
    double min = wAll->var(TString::Format("mgg_bkg_8TeV_norm_cat%d",c))->getMin();
    double max = wAll->var(TString::Format("mgg_bkg_8TeV_norm_cat%d",c))->getMax();
    wAll->factory(TString::Format("CMS_hgg_bkg_8TeV_cat%d_norm[%g,%g,%g]", c, mean, min, max));

    mean = wAll->var(TString::Format("mgg_bkg_8TeV_slope1_cat%d",c))->getVal();
    min = wAll->var(TString::Format("mgg_bkg_8TeV_slope1_cat%d",c))->getMin();
    max = wAll->var(TString::Format("mgg_bkg_8TeV_slope1_cat%d",c))->getMax();
    wAll->factory(TString::Format("CMS_hgg_bkg_8TeV_slope1_cat%d[%g,%g,%g]", c, mean, min, max));

    mean = wAll->var(TString::Format("mgg_bkg_8TeV_slope2_cat%d",c))->getVal();
    min = wAll->var(TString::Format("mgg_bkg_8TeV_slope2_cat%d",c))->getMin();
    max = wAll->var(TString::Format("mgg_bkg_8TeV_slope2_cat%d",c))->getMax();
    wAll->factory(TString::Format("CMS_hgg_bkg_8TeV_slope2_cat%d[%g,%g,%g]", c, mean, min, max));

    mean = wAll->var(TString::Format("mgg_bkg_8TeV_slope3_cat%d",c))->getVal();
    min = wAll->var(TString::Format("mgg_bkg_8TeV_slope3_cat%d",c))->getMin();
    max = wAll->var(TString::Format("mgg_bkg_8TeV_slope3_cat%d",c))->getMax();
    wAll->factory(TString::Format("CMS_hgg_bkg_8TeV_slope3_cat%d[%g,%g,%g]", c, mean, mean, mean)); //P3 fixed
    //wAll->factory(TString::Format("CMS_hgg_bkg_8TeV_slope3_cat%d[%g,%g,%g]", c, mean, min, max)); //P3 floating
    //If you change this part you have to change also this line above:
    // ((RooRealVar*) w->var(TString::Format("mgg_bkg_8TeV_slope3_cat%d",c)))->setConstant(true);

    cout << "Done For category " << c << endl;    
  }
  
  cout << "Imported" << endl;

  // (2) do reparametrization of background

  for (int c = 0; c < ncat; ++c) {
    wAll->factory(
		  TString::Format("EDIT::CMS_hgg_bkg_8TeV_cat%d(MggBkg_cat%d,",c,c) +
		  TString::Format(" mgg_bkg_8TeV_norm_cat%d=CMS_hgg_bkg_8TeV_cat%d_norm,", c,c)+
		  TString::Format(" mgg_bkg_8TeV_slope1_cat%d=CMS_hgg_bkg_8TeV_slope1_cat%d,", c,c)+
		  TString::Format(" mgg_bkg_8TeV_slope2_cat%d=CMS_hgg_bkg_8TeV_slope2_cat%d,", c,c)+
		  TString::Format(" mgg_bkg_8TeV_slope3_cat%d=CMS_hgg_bkg_8TeV_slope3_cat%d)", c,c)
		  );
  } 


  TString filename(wsDir+TString(fileBaseName)+".root");
  wAll->writeToFile(filename);
  cout << "Write background workspace in: " << filename << " file" << endl;

  cout << endl;
  std::cout << "observation " << endl;
  for (int c = 0; c < ncat; ++c) {
    std::cout << "  " << wAll->data(TString::Format("data_obs_cat%d",c))->sumEntries() << endl;
    wAll->data(TString::Format("data_obs_cat%d",c))->Print();
    cout << endl;
  }
  std::cout << std::endl;
  
  //## Check that what we write makes sense?
  for (int c = 0; c < ncat; ++c) {
    cout << endl;
    printf("CMS_hgg_bkg_8TeV_slope1_cat%d  param  %.4f  # Mean of background slope\n",
   	   c, wAll->var(TString::Format("CMS_hgg_bkg_8TeV_slope1_cat%d",c))->getVal());
    printf("CMS_hgg_bkg_8TeV_slope2_cat%d  param  %.4f  # Mean of background slope\n",
   	   c, wAll->var(TString::Format("CMS_hgg_bkg_8TeV_slope2_cat%d",c))->getVal());
    printf("CMS_hgg_bkg_8TeV_slope3_cat%d  param  %.4f  # Mean of background slope\n",
   	   c, wAll->var(TString::Format("CMS_hgg_bkg_8TeV_slope3_cat%d",c))->getVal());
  }

  return;
}

Double_t effSigma(TH1 *hist) {

  TAxis *xaxis = hist->GetXaxis();
  Int_t nb = xaxis->GetNbins();
  if(nb < 10) {
    std::cout << "effsigma: Not a valid histo. nbins = " << nb << std::endl;
    return 0.;
  }

  Double_t bwid = xaxis->GetBinWidth(1);
  if(bwid == 0) {
    std::cout << "effsigma: Not a valid histo. bwid = " << bwid << std::endl;
    return 0.;
  }
  Double_t xmax = xaxis->GetXmax();
  Double_t xmin = xaxis->GetXmin();
  Double_t ave = hist->GetMean();
  Double_t rms = hist->GetRMS();

  Double_t total=0.;
  for(Int_t i=0; i<nb+2; i++) {
    total+=hist->GetBinContent(i);
  }
  if(total < 100.) {
    std::cout << "effsigma: Too few entries " << total << std::endl;
    return 0.;
  }
  Int_t ierr=0;
  Int_t ismin=999;

  Double_t rlim=0.683*total;
  Int_t nrms=rms/(bwid);    // Set scan size to +/- rms
  if(nrms > nb/10) nrms=nb/10; // Could be tuned...

  Double_t widmin=9999999.;
  for(Int_t iscan=-nrms;iscan<nrms+1;iscan++) { // Scan window centre
    Int_t ibm=(ave-xmin)/bwid+1+iscan;
    Double_t x=(ibm-0.5)*bwid+xmin;
    Double_t xj=x;
    Double_t xk=x;
    Int_t jbm=ibm;
    Int_t kbm=ibm;
    Double_t bin=hist->GetBinContent(ibm);
    total=bin;
    for(Int_t j=1;j<nb;j++){
      if(jbm < nb) {
        jbm++;
        xj+=bwid;
        bin=hist->GetBinContent(jbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
      if(kbm > 0) {
        kbm--;
        xk-=bwid;
        bin=hist->GetBinContent(kbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
    }
    Double_t dxf=(total-rlim)*bwid/bin;
    Double_t wid=(xj-xk+bwid-dxf)*0.5;
    if(wid < widmin) {
      widmin=wid;
      ismin=iscan;
    }
  }
  if(ismin == nrms || ismin == -nrms) ierr=3;
  if(ierr != 0) std::cout << "effsigma: Error of type " << ierr << std::endl;

  return widmin;
}



void MakeDataCard_1Channel(RooWorkspace* w, const char* fileBaseName, const char* fileBkgName, int iChan, float mass) {

  double ScaleFactorSignal = 1.;
  //double ScaleFactorSignal = 500.; // Such that we expect ~10 events for Bulk Graviton, M=2 TeV, c=0.2

  TString cardDir = TString::Format("datacards/%d/",int(mass));
  Int_t ncat = NCAT;
  TString wsDir   = "../workspaces/";

  //   //Create directory for each mass point

  //   sprintf(thisMassPointDir,"datacards/%d",int(mass)); 
  //   cout << "Creating directory: " << thisMassPointDir << endl;
  //   System("mkdir pippo");

  //**********************//
  // Retrieve the datasets
  //**********************//

  cout << "Start retrieving dataset" << endl;

  RooDataSet* data[NCAT];
  RooDataSet* signal[NCAT];
  for (int c = 0; c < ncat; ++c) {
    data[c]        = (RooDataSet*) w->data(TString::Format("Data_cat%d",c));
    signal[c]      = (RooDataSet*) w->data(TString::Format("SigWeight_cat%d",c));
  }

  RooRealVar*  lumi = w->var("lumi");

  //*****************************//
  // Print Expected event yields
  //*****************************//

  cout << "======== Expected Events Number =====================" << endl;  
  cout << ".........Measured Data for L = " << lumi->getVal() << " pb-1 ............................" << endl;  
  cout << "#Events data:        " <<  w->data("Data")->sumEntries()  << endl;
  for (int c = 0; c < ncat; ++c) {
    cout << TString::Format("#Events data cat%d:   ",c) << data[c]->sumEntries()  << endl;
  }
  cout << ".........Expected Signal for L = " << lumi->getVal() << " pb-1 ............................" << endl;  
  cout << "#Events Signal:      " << w->data("SigWeight")->sumEntries()  << endl;

  //Float_t siglikeErr[NCAT];
  for (int c = 0; c < ncat; ++c) {
    cout << TString::Format("#Events Signal cat%d: ",c) << signal[c]->sumEntries()*lumi->getVal() << endl;
    //siglikeErr[c]=0.6*signal[c]->sumEntries();
  }
  cout << "====================================================" << endl;  


  //*************************//
  // Print Data Crd int file
  //*************************//


  TString filename(cardDir+TString(fileBaseName)+Form("_channel%d.txt",iChan));
  ofstream outFile(filename);


  outFile << "#CMS-HGG DataCard for Unbinned Limit Setting, " << lumi->getVal() <<  " pb-1 " << endl;
  outFile << "#Run with: combine -d hgg.mH130.0.shapes-Unbinned.txt -U -m 130 -H ProfileLikelihood -M MarkovChainMC --rMin=0 --rMax=20.0  -b 3000 -i 50000 --optimizeSim=1 --tries 30" << endl;
  outFile << "# Lumi =  " << lumi->getVal() << " pb-1" << endl;
  outFile << "imax 1" << endl; // channel or bins
  outFile << "jmax 1" << endl; // backgrounds
  outFile << "kmax *" << endl; // nuisance parameters
  outFile << "---------------" << endl;

  outFile << "shapes *      * " << wsDir+TString(fileBkgName)+".root" << " w_all:$PROCESS_$CHANNEL" << endl;
  outFile << "shapes MggBkg * "<<  wsDir+TString(fileBkgName)+".root" << " w_all:CMS_hgg_bkg_8TeV_$CHANNEL" << endl;
  outFile << "shapes MggSig * " << wsDir+TString(fileBaseName)+".inputsig.root" << " w_all:CMS_hgg_sig_$CHANNEL" << endl;

  outFile << "---------------" << endl;
  outFile << Form("bin          cat%d", iChan) << endl;
  outFile <<  "observation   "  <<  Form("%.10lg",data[iChan]->sumEntries()) << endl;
  outFile << "------------------------------" << endl;
  outFile << "bin                      "<< Form("cat%d       cat%d      ", iChan, iChan) << endl;
  outFile << "process                 MggSig     MggBkg     " << endl;
  outFile << "process                    0          1          " << endl;
  outFile <<  "rate                      " 
	  << "  " << signal[iChan]->sumEntries()*lumi->getVal()*ScaleFactorSignal << "  " << 1 << endl;
  //<< "  " << signal[iChan]->sumEntries()/signal[2]->sumEntries()*20  << "  " << 1 << endl;
  //should we put the correct rate of events?
  outFile << "--------------------------------" << endl;
  
  //== Uncertainties on SIGNAL NORMALIZATION ==

  //Lumi
  outFile << "lumi_8TeV               lnN  0.974/1.026    - " << endl;
  
  //Trigger
  outFile <<   "CMS_Trigger_eff_g      lnN    0.99/1.01    - " << endl;
  
  //Lepton ID
  if(iChan==0 || iChan==1)
    outFile << "CMS_LeptonID_eff_g_ELE  lnN  0.97/1.03      - # Signal Efficiency" << endl;
  if(iChan==2 || iChan==3)
    outFile << "CMS_LeptonID_eff_g_MU   lnN  0.99/1.01      - # Signal Efficiency" << endl;
  
  //JES
  double sigma = 0.0037 + 0.0000105 * mass;
  outFile << Form("CMS_JES_eff_g           lnN  %f/%f      - # Signal Efficiency",1-sigma,1+sigma) << endl;

  //W-tag efficiency
  if(iChan==0 || iChan==2)
    outFile << "CMS_VV_eff_g_LP         lnN  1.237/0.763      - # Signal Efficiency" << endl;
  if(iChan==1 || iChan==3)
    outFile << "CMS_VV_eff_g_HP         lnN  0.906/1.094      - # Signal Efficiency" << endl;


  //== Uncertainties on SIGNAL SHAPE ==
  outFile << "# Parametric shape uncertainties, entered by hand." << endl;
  outFile << "CMS_hgg_sig_m0_absShift    param   1   0.013   # uncertainty on the mean" << endl;
  outFile << "CMS_hgg_sig_sigmaScale     param   1   0.045   # uncertainty on the sigma" << endl;
  /*
  outFile << Form("CMS_hgg_sig_m0_absShift_cat%d    param   1   0.013   # uncertainty on the mean",iChan) << endl;
  outFile << Form("CMS_hgg_sig_sigmaScale_cat%d     param   1   0.04   # uncertainty on the sigma",iChan) << endl;
  */
 
  //== Uncertainties on BACKGROUND SHAPE/NORMALIZATION ==
  outFile << Form("CMS_hgg_bkg_8TeV_cat%d_norm           flatParam  # Normalization uncertainty on background slope",iChan) << endl;
  outFile << Form("CMS_hgg_bkg_8TeV_slope1_cat%d         flatParam  # Mean and absolute uncertainty on background slope",iChan) << endl;
  outFile << Form("CMS_hgg_bkg_8TeV_slope2_cat%d         flatParam  # Mean and absolute uncertainty on background slope",iChan) << endl;
  outFile << Form("CMS_hgg_bkg_8TeV_slope3_cat%d         flatParam  # Mean and absolute uncertainty on background slope",iChan) << endl;

  outFile.close();

  cout << "Write data card in: " << filename << " file" << endl;

  return;
}


Double_t effSigma(TH1 *hist) {

  TAxis *xaxis = hist->GetXaxis();
  Int_t nb = xaxis->GetNbins();
  if(nb < 10) {
    std::cout << "effsigma: Not a valid histo. nbins = " << nb << std::endl;
    return 0.;
  }

  Double_t bwid = xaxis->GetBinWidth(1);
  if(bwid == 0) {
    std::cout << "effsigma: Not a valid histo. bwid = " << bwid << std::endl;
    return 0.;
  }
  Double_t xmax = xaxis->GetXmax();
  Double_t xmin = xaxis->GetXmin();
  Double_t ave = hist->GetMean();
  Double_t rms = hist->GetRMS();

  Double_t total=0.;
  for(Int_t i=0; i<nb+2; i++) {
    total+=hist->GetBinContent(i);
  }
  if(total < 100.) {
    std::cout << "effsigma: Too few entries " << total << std::endl;
    return 0.;
  }
  Int_t ierr=0;
  Int_t ismin=999;

  Double_t rlim=0.683*total;
  Int_t nrms=rms/(bwid);    // Set scan size to +/- rms
  if(nrms > nb/10) nrms=nb/10; // Could be tuned...

  Double_t widmin=9999999.;
  for(Int_t iscan=-nrms;iscan<nrms+1;iscan++) { // Scan window centre
    Int_t ibm=(ave-xmin)/bwid+1+iscan;
    Double_t x=(ibm-0.5)*bwid+xmin;
    Double_t xj=x;
    Double_t xk=x;
    Int_t jbm=ibm;
    Int_t kbm=ibm;
    Double_t bin=hist->GetBinContent(ibm);
    total=bin;
    for(Int_t j=1;j<nb;j++){
      if(jbm < nb) {
        jbm++;
        xj+=bwid;
        bin=hist->GetBinContent(jbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
      if(kbm > 0) {
        kbm--;
        xk-=bwid;
        bin=hist->GetBinContent(kbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
    }
    Double_t dxf=(total-rlim)*bwid/bin;
    Double_t wid=(xj-xk+bwid-dxf)*0.5;
    if(wid < widmin) {
      widmin=wid;
      ismin=iscan;
    }
  }
  if(ismin == nrms || ismin == -nrms) ierr=3;
  if(ierr != 0) std::cout << "effsigma: Error of type " << ierr << std::endl;

  return widmin;
}

void R2JJFitter(double mass)
{
    gSystem->Load("$CMSSW_BASE/lib/slc5_amd64_gcc462/libHiggsAnalysisCombinedLimit.so");
    
    runfits(mass, true);
    //runfits(mass, false);
}
