#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TPaveText.h"

#include "RooPlot.h"
#include "RooBinning.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooCBShape.h"
#include "RooBreitWigner.h"
#include "RooProdPdf.h"
#include "RooAddPdf.h"
#include "RooFFTConvPdf.h"
#include "RooFitResult.h"
#include "RooWorkspace.h"
#include "RooProduct.h"
#include "RooArgList.h"

#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"

#include "PdfDiagonalizer.h"

#include "DataCardUtils.h"

//#include "binningFits_XWW.h"
//#include "Config_XWW.h"
#include "Config_XZZ.h"
#include "binningFits_XZZ.h"


float mZZmin_ = startFit;  // this should be synchronized with startFit in fitBackground.cpp

struct TheorSigParameters {

  float mH;
  float XSgg;
  float XSgg_p;
  float XSgg_m;
  float XSpdfgg_p;
  float XSpdfgg_m;
  //  float XSvbf;
  // float XSvbf_p;
  // float XSvbf_m;
  // float XSpdfvbf_p;
  // float XSpdfvbf_m;
  // float Gamma;
  float BRXtoZZ;
  float BRZZto2l2q;

};



double sign( double x ) {

  double returnSign = 0.;

  if( x>=0. ) returnSign =  1.;
  else returnSign =  -1.;

  return returnSign;

}



void create_singleDatacard( float mass,float width, float lumi, const std::string& leptType_str, int nxj, int pur, TF1* f1_eff_vs_mass );

TheorSigParameters get_thParameters( float mass );

double linear_interp( double x, double x_old, double mass, double mH, double mH_old );
double expo_interp(double s2, double s1,  double newM,double m2,double m1);
TF1* get_eff_vs_mass( const std::string& leptType_str, int nxj, int pur, float mZZmin, float mZZmax );


double get_signalParameter(int nxj,  const std::string& purType_str, const std::string& leptType_str, double massH, std::string varname);

std::string systString( std::pair<double,double> systPair, double maxDiff=0.01 );
std::pair<double,double> theorSyst( double errMinus, double errPlus, double addMinus=0., double addPlus=0. );
std::pair<double,double> theorSyst_HighmH( double mHVal );
std::pair<double,double> leptTriggerSyst( const std::string& leptType_str);
std::pair<double,double> leptEffSyst( const std::string& leptType_str);
std::pair<double,double> leptScaleSyst( const std::string& leptType_str, double mass=0.0);

std::pair<double,double> jetScaleSyst( double mass );
std::pair<double,double> puSyst( double mass );
std::pair<double,double> VTagEffSyst( const std::string& leptType_str, int nxj, double mass , int purity);
std::pair<double,double> bTagEffSyst( const std::string& leptType_str, int nbtag, double mass );

//double backgroundNorm( const std::string& dataset, const std::string& leptType_str, int nxj );


int main( int argc, char* argv[] ) {

  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING) ;

  float lumi_ELE;
  float lumi_MU;
  lumi_ELE=19712.; //pb^-1
  lumi_MU =19671.; //pb^-1

  std::cout<<"Starting card creation with the following settings:"<<endl;
  std::cout<<"InputDir with fits: "<<myOutDir.c_str()<<endl;
  std::cout<<"OutputDir with cards: "<<datacardDir.c_str()<<endl;
  std::cout<<"Lumi MU="<<lumi_MU<<"  ELE="<<lumi_ELE<<std::endl<<std::endl;

  //first loop over available signal MC files to fit efficiency:
  TF1* f1_eff_vs_mass_MU_1JHP = get_eff_vs_mass("MU", 1,1, mZZmin_, 2500);
  TF1* f1_eff_vs_mass_MU_1JLP = get_eff_vs_mass("MU", 1,0, mZZmin_, 2500);
  TF1* f1_eff_vs_mass_MU_2J   = 0;
  if(jetCats>1)f1_eff_vs_mass_MU_2J = get_eff_vs_mass("MU", 2,-1, mZZmin_,1400);//set purity to -1 for 2J cat

  TF1* f1_eff_vs_mass_ELE_1JHP = get_eff_vs_mass("ELE", 1,1, mZZmin_, 2500);
  TF1* f1_eff_vs_mass_ELE_1JLP = get_eff_vs_mass("ELE", 1,0, mZZmin_, 2500);
  TF1* f1_eff_vs_mass_ELE_2J   = 0;
  if(jetCats>1)f1_eff_vs_mass_ELE_2J = get_eff_vs_mass("ELE", 2,-1, mZZmin_, 1400);

  std::ifstream ifs;
  if(isZZChannel)
    ifs.open("masses.txt");
  else
    ifs.open("masses_xww.txt");

  while( ifs.good() ) {

    float mass;
    ifs >> mass;
    std::vector<float> widths;
    if(dims=="1d")
      widths.push_back(0.0);
    if(dims=="2d"){
      std::ifstream widthf;
      widthf.open("widths.txt");
      float width;
      while (widthf.good()){
	widthf >> width;
	widths.push_back(width);
      }
    }
    
    //    std::string datacardDir("DataCards_20120210") ;
    for(unsigned int i =0; i < widths.size() ; i++ ){
      //if(mass!=800)continue;
      std::cout << std::endl << std::endl;;
      std::cout << "++++++++++++++++++++++" << std::endl;
      std::cout << "+++ MASS: " << mass << std::endl;
      if (dims=="2d") std::cout << "+++ WIDTH: " << widths[i] <<  std::endl;
      std::cout << "++++++++++++++++++++++" << std::endl;
      std::cout << std::endl;
      char mkdir_command[100];
      if (dims=="1d")
	sprintf( mkdir_command, "mkdir -p %s/%.0f", datacardDir.c_str(), mass);
      if (dims=="2d")
	sprintf( mkdir_command, "mkdir -p %s/%.0f_%.3f", datacardDir.c_str(), mass,widths[i]);
      system(mkdir_command);

      if(leptType=="ELE"||leptType=="ALL")
	{
	  create_singleDatacard( mass,widths[i], lumi_ELE, "ELE", 1,1, f1_eff_vs_mass_ELE_1JHP);
	  create_singleDatacard( mass,widths[i], lumi_ELE, "ELE", 1,0, f1_eff_vs_mass_ELE_1JLP);
	}
      if(leptType=="MU"||leptType=="ALL")
	{
	  create_singleDatacard( mass,widths[i], lumi_MU,   "MU", 1,1, f1_eff_vs_mass_MU_1JHP);
	  create_singleDatacard( mass,widths[i], lumi_MU,   "MU", 1,0, f1_eff_vs_mass_MU_1JLP);
	}
      //   if(jetCats>1&&mass<=800){
      //	if(leptType=="ELE"||leptType=="ALL")create_singleDatacard( mass,widths[i], lumi_ELE, "ELE", 2,-1, f1_eff_vs_mass_ELE_2J);
      //	if(leptType=="MU"||leptType=="ALL")create_singleDatacard( mass,widths[i], lumi_MU,   "MU", 2,-1, f1_eff_vs_mass_MU_2J);
      // }

    } // for widths
  } //while masses

  /////  return 0;

}



void create_singleDatacard( float mass,float width, float lumi, const std::string& leptType_str, int nxj,int pur, TF1* f1_eff_vs_mass ) {

  std::cout<<"\n------------------------------------------------"<<std::endl;
  std::cout<<"Creating new datacard: "<< leptType_str.c_str()<<"  "<<nxj<<"J ; Purity="<<pur<<std::endl;

  if( leptType_str!="ELE" && leptType_str!="MU" ) {
    std::cout << "Unkown Lept Type '" << leptType_str.c_str() << "'. Exiting." << std::endl;
    exit(12333);
  }


  std::string pur_str="";
  if(pur==0)pur_str="LP";
  if(pur==1)pur_str="HP";



  TheorSigParameters hp = get_thParameters(mass);

  string rename_str="";
  std::stringstream ssnxj;
  ssnxj << nxj;
  rename_str += "_"+channel_marker+"_"+leptType_str+ssnxj.str()+"J"+pur_str;

  /////////////
  //-> open fitResults file (all lept types):
  std::string fitResultsFileName = DataCardUtils::get_fitResultsRootFileName( nxj,pur_str, leptType.c_str() ,myOutDir.c_str(),channel_marker);
  std::cout << "reading results from: "<< fitResultsFileName.c_str() << std::endl;
  TFile* fitResultsFile = TFile::Open(fitResultsFileName.c_str());
  // fitResultsFile->ls();

  /////////////////////
  //->  get fit results:
  char fitResultName[200];  
  
  //  if(pur==1 && isZZChannel ){//simple expo for HP category or XWW analysis
  //  sprintf( fitResultName, "resultsExpoFit_%s_%dJ_%s_%s",channel_marker.c_str(),nxj,pur_str.c_str(),leptType.c_str() );
  // }
  //else {
  sprintf( fitResultName, "resultsExpLevelledFit_%s_%dJ_%s_%s_decorr",channel_marker.c_str(), nxj ,pur_str.c_str(),leptType.c_str());
  //}
  cout<<"Trying to pick RooFitResult :"<<fitResultName<<endl;
  RooFitResult* bgFitResult = (RooFitResult*)fitResultsFile->Get(fitResultName);
  bgFitResult->Print("v");


  ////////////////
  //->  get workspace:
  char workspaceName[200];
  sprintf( workspaceName, "ws_alpha_%s_%dJ_%s_%s",channel_marker.c_str(), nxj,pur_str.c_str(),leptType.c_str() );
  RooWorkspace* bgws = (RooWorkspace*)fitResultsFile->Get(workspaceName);
  // cout<<"\n\nPrinting contents of the WorkSpace: "<<endl;
  //  bgws->Print("v");

  /////////////////////////////////////
  //-> get vars containing syst unc on alpha
  std::vector<RooRealVar*>alphaErr;
  RooArgList bgPars = bgFitResult->floatParsFinal();
  std::vector<std::string> parname; 
  parname.reserve(bgPars.getSize());
  std::cout<<"List of RooRealVars storing syst unc on bkgd shape parameters:"<<std::endl;
  for( int iVar=0; iVar<bgPars.getSize(); ++iVar ) {
    parname.push_back(bgPars.at(iVar)->GetName());
    parname.at(iVar)+="_alphaErr";
    std::cout<<"Var "<<iVar<<"  Parname: "<<parname.at(iVar).c_str()<<endl;//" read from ws:  "  << bgws->var(parname.at(iVar).c_str())->GetName()<<std::endl;
    alphaErr.push_back(bgws->var(parname.at(iVar).c_str()));
  }

  ///////////////////////////
  //-> get global alpha uncertainty
  //the +0.5 effectively leaves the bkgd normalization free to float when profiling the nuisances
  double globalAlphaErr =  bgws->var("alphaNormErr")->getVal();//+0.5
  if(mass>2000) globalAlphaErr+=0.5;
  //std::cout << globalAlphaErr << std::endl;
  //exit(0);

  //////////////////////////////
  ////->  get main variable from input workspace:
  if(nxj==2){mZZmin_=bins2[0];mZZmax_=bins2[nBins2-1];}//get in sync with was done in fitBackground for 2J category
  //  else {mZZmin_ =bins1[0];mZZmax_=bins1[nBins1-1];}//get in sync with was done in fitBackground for 2J category
  else{
    mZZmax_=bins1[nBins1-1];
    if(pur==1) mZZmin_ =startFit;
    else mZZmin_ =500.0;//650.0; use same range otherwise RooStats will get confused when merging cards
  }
  RooRealVar* CMS_xzz_mZZ = new RooRealVar("mZZ","mZZ",mZZmin_,mZZmax_);//it works
  CMS_xzz_mZZ->setMin(mZZmin_);  
  std::cout<<"\n==== Printing multiline verbose of variable created on-the-fly: ===="<<endl;
  CMS_xzz_mZZ->printMultiline(cout,99,true);
  cout<<"Dump other info:"<<endl;
  cout<<CMS_xzz_mZZ->getBin()<<"  "<<CMS_xzz_mZZ->getBins()<<"  "<<CMS_xzz_mZZ->getMin()<<"  "<<CMS_xzz_mZZ->getMax()<<"  "<<endl;
  std::cout<<"===== End printing multiline ======\n"<<endl;
  std::cout<<"\n*** Printing multiline verbose of variable in WS named \"mZZ\": *****"<<endl;
  // RooRealVar* CMS_hzz2l2q_mZZ = bgws->var("mZZ");//it does not work
  bgws->var("mZZ")->printMultiline(cout,99,true);
  cout<<"Dump other info:"<<endl;
  cout<<bgws->var("mZZ")->getBin()<<"  "<<bgws->var("mZZ")->getBins()<<"  "<<bgws->var("mZZ")->getMin()<<"  "<<bgws->var("mZZ")->getMax()<<"  "<<endl;
  std::cout<<"**** End printing multiline *******\n"<<endl;
  //   RooRealVar* CMS_hzz2l2q_mZZ = mzzws->var("mZZ");//reading it from MZZ-sideband ws works
 


  char suffix[100];
  sprintf( suffix, "%s%s%dJ%s", (DataCardUtils::leptType_datacards(leptType_str)).c_str(), (DataCardUtils::leptType_datacards(leptType_str)).c_str(), nxj,pur_str.c_str());
  std::string suffix_str(suffix);


  /////////////////////
  //////////
  //->  START TO PRINT THE DATACARD
  char datacardName[400];
  if(dims=="1d")
    sprintf( datacardName, "%s/%.0f/%s_%s.%.0f.txt", datacardDir.c_str(), mass, channel_marker.c_str(),suffix, mass);
  if(dims=="2d")
    sprintf( datacardName, "%s/%.0f_%.3f/%s_%s.%.0f.txt", datacardDir.c_str(), mass,width ,channel_marker.c_str(),suffix, mass);

  std::ofstream ofs(datacardName);

  std::string bkgd_shape_name=("background_decorrLevExpo"+rename_str);
  //   if(pur==1 && isZZChannel) bkgd_shape_name=("background_expo"+rename_str);

  ofs << "# Card for process XZZ->"<<suffix << std::endl;
  ofs << "#imax 1  number of channels" << std::endl;
  ofs << "#jmax 1  number of backgrounds" << std::endl;
  ofs << "#kmax *  number of nuisance parameters (sources of systematical uncertainties)" << std::endl;
  ofs << "------------ " << std::endl;
  ofs << "shapes "<<signalProcessName<<"_"<<channel_marker <<" CMS_"<< channel_marker <<"_" << suffix_str << " "<< channel_marker <<"_" << suffix_str << ".input.root  w:" <<(signalProcessName+rename_str).c_str()<< std::endl;
  ofs << "shapes background"<< channel_marker <<" CMS_"<< channel_marker <<"_" << suffix_str << " "<< channel_marker <<"_" << suffix_str << ".input.root w:"<<bkgd_shape_name.c_str()  << std::endl;
  ofs << "shapes data_obs   CMS_"<< channel_marker <<"_" << suffix_str << " "<< channel_marker <<"_" << suffix_str << ".input.root w:"<<("dataset_obs"+rename_str).c_str()  << std::endl;
  ofs << "------------ " << std::endl;
  ofs << "bin         CMS_"<< channel_marker <<"_" << suffix << std::endl;

  std::string name_dataobs="dsDataSIG";//+"_"+ssnxj.str()+"J_"+pur_str+"_"+leptType_str;
  RooDataSet* dataset_obs = DataCardUtils::get_observedDataset( bgws , leptType_str, nxj,pur, name_dataobs );
  dataset_obs->SetName(( dataset_obs->GetName()+rename_str).c_str());
  std::cout<<"Statistics of the observed dataset straight from the ws: "<<dataset_obs->numEntries()<<"  "<<dataset_obs->sumEntries() <<std::endl;

  //FIXME !!! unsolved problem: if datasets with different cuts on mzz 
  //are combined (e.g.: HP starting at 500 and LP startting at 650 )
  //instabilities in combine after combination  
  double mzzRangeMinCut=0.0;	 
  if(isZZChannel){	 
    mzzRangeMinCut=startFit;//mZZmin_  //bins1[0];	 
 
     // if(nxj==1&&pur==0)mzzRangeMinCut=600.0;
     //if(nxj==1&&pur==1)mzzRangeMinCut=480.0;
  }

  std::stringstream ssMinRangeCut;
  ssMinRangeCut << mzzRangeMinCut;
  std::string mzzRangeMinStr="mZZ>"+ssMinRangeCut.str();

  std::cout<<"Applying on the observed dataset in sig region this extra-cut: "<<mzzRangeMinStr.c_str()<<endl;
  RooDataSet* dataset_obs_reduced=new RooDataSet("dataset_obs","dataset_obs",dataset_obs,RooArgSet(*CMS_xzz_mZZ),mzzRangeMinStr.c_str());
  dataset_obs_reduced->SetName(("dataset_obs"+rename_str).c_str());
  float observedYield = dataset_obs_reduced->sumEntries();
  std::cout << "observation   " << observedYield << " (numEntries="<< dataset_obs_reduced->numEntries()<<")"<<std::endl;

  ofs << "observation   " << observedYield << std::endl;
  ofs << "------------ " << std::endl;
  ofs << "bin                CMS_"<< channel_marker <<"_" << suffix <<  "\tCMS_"<< channel_marker <<"_" << suffix << std::endl;
  ofs << "process            "<<signalProcessName<<"_"<<channel_marker <<"\t\t\tbackground"<< channel_marker  << std::endl;
  ofs << "process            0\t\t\t1" << std::endl;

  float eff = f1_eff_vs_mass->Eval(hp.mH);
  float rate_gg   = eff*hp.XSgg*hp.BRZZto2l2q*lumi; //xsect has both ee and mm

  ///////////////
  //->  compute expected BG yield from observed sideband events.
  //for ZZ: normalization in [500, 2800] always, also for LP
  //reason is that one must create workspaces for HiggsComb tool
  //using same RooRealVar otherwise mess with RooStats when 
  //combining channels
  Double_t rate_background = DataCardUtils::get_backgroundNormalization(bgws , leptType_str);
  std::cout <<"Background rate: "<< rate_background << std::endl;


  ofs << "rate               " << rate_gg << "\t\t" << rate_background << std::endl;
  ofs << "------------ " << std::endl;

  ////////////////////
  //->  and now systematics:

  ofs << "lumi_8TeV\t\t\tlnN\t1.026\t\t\t1.0" << std::endl;

  //std::pair<double,double> pdf_gg  = theorSyst( hp.XSpdfgg_m, hp.XSpdfgg_p, 0.04, 0.015 );
  //ofs << "pdf_gg\t\tlnN\t" << systString(pdf_gg) << "\t1.0" << std::endl;

  //std::pair<double,double> QCDscale_ggH = theorSyst( hp.XSgg_m, hp.XSgg_p);
  //ofs << "QCDscale_ggH\tlnN\t" << systString(QCDscale_ggH) << "\t1.0" << std::endl;

  ofs << "CMS_"<<channel_marker.c_str() <<"_trigger_" << DataCardUtils::leptType_datacards(leptType_str) << "\tlnN\t" << systString(leptTriggerSyst(leptType_str)) << "\t1.0" << std::endl;

  ofs << "CMS_eff_" << DataCardUtils::leptType_datacards(leptType_str) << "\t\tlnN\t" << systString(leptEffSyst(leptType_str)) << "\t1.0" << std::endl;

  ofs << "CMS_scale_" << DataCardUtils::leptType_datacards(leptType_str) << "\t\tlnN\t" << systString(leptScaleSyst(leptType_str)) <<  "\t1.0" << std::endl;

  ofs << "CMS_scale_j\t\tlnN\t" << systString(jetScaleSyst(hp.mH)) <<  "\t1.0" << std::endl;

  ofs << "CMS_eff_vtag_tau21_sf\t\tlnN\t" << systString(VTagEffSyst(leptType_str, nxj, hp.mH,pur),-1.) << "\t1.0" << std::endl;
  //  ofs << "CMS_eff_vtag_mass_sf\t\tlnN\t" << "1.0" << "\t1.0" << std::endl;
  //  ofs << "CMS_eff_vtag_model\t\tlnN\t" << "1.0" << "\t1.0" << std::endl;

  ofs << "CMS_pu\t\tlnN\t"<<systString(puSyst(hp.mH)) <<"\t\t\t1.0" << std::endl;
  std::cout << "CMS_pu\t\tlnN\t"<<systString(puSyst(hp.mH)) <<"\t\t\t1.0" << std::endl;

  ofs << "CMS_"<< channel_marker <<"_alphanorm"<<nxj<<"b\t\tlnN\t 1.0 "<<"\t\t\t" << 1.+globalAlphaErr << std::endl;



  // syst done. now finish with sig and bkg shape parameters:
	
  double bgNorm = 0.0;
  if(useVJetsNormFromMJFit&&nxj==1){
    double bgNormUnc=0.0;
    if(leptType_str=="ELE")bgNormUnc = extNorm_1J_err[0][pur];
    else if(leptType_str=="MU")bgNormUnc = extNorm_1J_err[1][pur];
    else if(leptType_str=="ALL")bgNormUnc = sqrt(extNorm_1J_err[0][pur]*extNorm_1J_err[0][pur] + extNorm_1J_err[1][pur]*extNorm_1J_err[1][pur]);
    else bgNormUnc = 0.0;
    bgNorm=bgNormUnc/rate_background;
    cout<<"Bkgd Normalization Uncertainty for "<<leptType_str.c_str()<<" Pur="<<pur<<" -> "<<bgNormUnc<<" ; rel err = "<<bgNorm<<endl;
  }
  else bgNorm = DataCardUtils::get_backgroundNormalization(bgws,leptType_str,nxj,pur,"dsDataSB");

  char bgNorm_char[100];
  sprintf( bgNorm_char, "%.0lf", bgNorm);
  std::string bgNorm_str(bgNorm_char);
  std::cout<<"SIDEBAND NorM: "<<bgNorm<<"  ("<<  DataCardUtils::get_backgroundNormalization(bgws,leptType_str,nxj,pur)<<")"<<std::endl;
  double alpha = rate_background/bgNorm;
  char alpha_char[100];
  sprintf( alpha_char, "%lf", alpha);
  std::string alpha_str(alpha_char);

  char bgNormName[200];
  sprintf( bgNormName, "CMS_%s_bkg%dJ%s%s%sp0",channel_marker.c_str(), nxj,pur_str.c_str(), (DataCardUtils::leptType_datacards(leptType_str)).c_str(), (DataCardUtils::leptType_datacards(leptType_str)).c_str() );
  std::string bgNormName_str(bgNormName);
  if(useVJetsNormFromMJFit)  ofs << bgNormName_str << "\tlnN    ---\t" <<(1.0+bgNorm)  << std::endl;
  else   ofs << bgNormName_str << "\tgmN " << bgNorm_str << "\t---\t"  << alpha_str << std::endl;
  //std::cout << bgNormName_str << "\tgmN " << bgNorm_str << "\t-----\t-----\t" << alpha_str << std::endl;

  //  RooArgList bgPars = bgFitResult->floatParsFinal();

  cout<<"Looping on "<<bgPars.getSize()<<" background shape params"<<endl;
  for( int iVar=0; iVar<bgPars.getSize(); ++iVar ) {
    RooRealVar* thisVar = dynamic_cast<RooRealVar*>(bgPars.at(iVar));
    cout<<"Var -> "<<thisVar->GetName()<<flush;
    double varValue=thisVar->getVal();
    cout<<"="<<varValue<<flush;
    double varError=thisVar->getError();
    double systError=0.0;
    if(doPseudoExp) systError=alphaErr.at(iVar)->getVal();//error on alpha from MC stats, estimated with pseudo-exp
    cout<<" +/- "<<varError<<" +/- "<<systError <<endl; 
    varError=sqrt(varError*varError +  systError*systError);
	  
    ofs << thisVar->GetName() << "\tparam\t\t" <<varValue  << "\t" << varError << std::endl;
	  
    //    std::cout << thisVar->GetName() << "\tparam\t\t" << varValue << "\t" << thisVar->getError()  <<" & "<<alphaErr.at(iVar)->getVal()<<" -> "<<varError<< std::endl;
    std::cout << thisVar->GetName() << "\tparam\t\t" << varValue << "\t" << thisVar->getError() <<" -> "<<varError<< std::endl;
  }

  /// Errors on signal shape
  /// https://twiki.cern.ch/twiki/bin/viewauth/CMS/EXO12022ReviewTwiki#Systematic_uncertainties
  float massH = hp.mH;
	
  //lepton scale 
  char sigSystp1_LepScale[200];//m
  char sigSystp2_LepScale[200];//width
  sprintf(sigSystp1_LepScale,"CMS_sig_p1_scale_%s",DataCardUtils::leptType_datacards(leptType_str).c_str());//,pur_str.c_str());
  sprintf(sigSystp2_LepScale,"CMS_sig_p2_scale_%s",DataCardUtils::leptType_datacards(leptType_str).c_str());//,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  char sigSystp1_TmpName[200];//m
  char sigSystp2_TmpName[200];//width
  sprintf(sigSystp1_TmpName,"%s_NomUnc",sigSystp1_LepScale);
  sprintf(sigSystp2_TmpName,"%s_NomUnc",sigSystp2_LepScale);

  //cout << "CB mean =" << CB_mean.getVal() << " CB sigma = " << CB_sigma.getVal() << endl;
  double peakSystFactor=0.0,widthSystFactor=0.0;
  if(leptType_str == "ELE") {peakSystFactor=CMS_sig1Je_p1_scale; widthSystFactor=CMS_sig1Je_p2_scale;}//ele width negligible. we won't consider it // Defined in Config_XZZ.h and Config_XWW.h
  if(leptType_str == "MU") {peakSystFactor=CMS_sig1Jm_p1_scale; widthSystFactor=CMS_sig1Jm_p2_scale;}  // Defined in Config_XZZ.h and Config_XWW.h
  RooRealVar shapeSystPeak_LepScale(sigSystp1_TmpName,sigSystp1_TmpName,peakSystFactor);
  RooRealVar shapeSystWidth_LepScale(sigSystp2_TmpName,sigSystp2_TmpName,widthSystFactor);
  shapeSystPeak_LepScale.setConstant(kTRUE);
  shapeSystWidth_LepScale.setConstant(kTRUE);
  ofs << std::string(sigSystp1_LepScale) << " param 0.0 1.0 "  <<endl;// peakSystFactor << endl; 
  if(leptType_str == "MU")ofs << std::string(sigSystp2_LepScale) << " param 0.0 1.0 "  <<endl;// widthSystFactor << endl;
  	
  //lepton resolution omitted because small
  char sigSystp1_LepRes[200];//m
  char sigSystp2_LepRes[200];//width
  sprintf(sigSystp1_LepRes,"CMS_%s_sig%dJ_p1_%s_res",channel_marker.c_str(),nxj,DataCardUtils::leptType_datacards(leptType_str).c_str());//,pur_str.c_str()
  sprintf(sigSystp2_LepRes,"CMS_%s_sig%dJ_p2_%s_res",channel_marker.c_str(),nxj,DataCardUtils::leptType_datacards(leptType_str).c_str());//,pur_str.c_str());
  sprintf(sigSystp1_TmpName,"%s_NomUnc",sigSystp1_LepRes);
  sprintf(sigSystp2_TmpName,"%s_NomUnc",sigSystp2_LepRes);

  //jet scale: same and fully correlated between ele and mu
  char sigSystp1_JetScale[200];//m
  char sigSystp2_JetScale[200];//width	
  //  sprintf(sigSystp1_JetScale,"CMS_%s_sig%dJ_p1_jes",channel_marker.c_str(),nxj);//,pur_str.c_str());
  //  sprintf(sigSystp2_JetScale,"CMS_%s_sig%dJ_p2_jes",channel_marker.c_str(),nxj);//,pur_str.c_str());
  sprintf(sigSystp1_JetScale,"CMS_sig_p1_jes");//,pur_str.c_str());
  sprintf(sigSystp2_JetScale,"CMS_sig_p2_jes");
  sprintf(sigSystp1_TmpName,"%s_%s_NomUnc",sigSystp1_JetScale,channel_marker.c_str());
  sprintf(sigSystp2_TmpName,"%s_%s_NomUnc",sigSystp2_JetScale,channel_marker.c_str());
  peakSystFactor=CMS_sig1J_p1_jes; 
  widthSystFactor=CMS_sig1J_p2_jes; // Defined in Config_XZZ.h and Config_XWW.h
  //for ww, use a line from 2% to 3% from 600 to 2500
  if(!isZZChannel)widthSystFactor = 0.02 + (0.03-0.02)/(2500-600)*(hp.mH-600);
  RooRealVar shapeSystPeak_JetScale(sigSystp1_TmpName,sigSystp1_TmpName,peakSystFactor);
  RooRealVar shapeSystWidth_JetScale(sigSystp2_TmpName,sigSystp2_TmpName,widthSystFactor);
  shapeSystPeak_JetScale.setConstant(kTRUE);
  shapeSystWidth_JetScale.setConstant(kTRUE);
  //in ZZ, negligible size of syst on peak shift from JES, omit it
  if(!isZZChannel)ofs << std::string(sigSystp1_JetScale) << " param 0.0 1.0" <<endl;// << peakSystFactor << endl; 
  ofs << std::string(sigSystp2_JetScale) << " param 0.0 1.0"  << endl;//widthSystFactor << endl;

  //jet resolution: same and fully correlated between ele and mu
  char sigSystp1_JetRes[200];//m
  char sigSystp2_JetRes[200];//width	
  //sprintf(sigSystp1_JetRes,"CMS_%s_sig%dJ_p1_jer",channel_marker.c_str(),nxj);//,pur_str.c_str());
  //sprintf(sigSystp2_JetRes,"CMS_%s_sig%dJ_p2_jer",channel_marker.c_str(),nxj);//,pur_str.c_str());
  sprintf(sigSystp1_JetRes,"CMS_sig_p1_jer");//,pur_str.c_str());
  sprintf(sigSystp2_JetRes,"CMS_sig_p2_jer");//,pur_str.c_str());
  sprintf(sigSystp1_TmpName,"%s_%s_NomUnc",sigSystp1_JetRes,channel_marker.c_str());
  sprintf(sigSystp2_TmpName,"%s_%s_NomUnc",sigSystp2_JetRes,channel_marker.c_str());
  peakSystFactor=CMS_sig1J_p1_jer;  // Defined in Config_XZZ.h and Config_XWW.h
  widthSystFactor=CMS_sig1J_p2_jer;  // Defined in Config_XZZ.h and Config_XWW.h
  RooRealVar shapeSystPeak_JetRes(sigSystp1_TmpName,sigSystp1_TmpName,peakSystFactor);
  RooRealVar shapeSystWidth_JetRes(sigSystp2_TmpName,sigSystp2_TmpName,widthSystFactor);
  shapeSystPeak_JetRes.setConstant(kTRUE);
  shapeSystWidth_JetRes.setConstant(kTRUE);
  //in ZZ, negligible size of syst on peak shift from JER, omit it
  if(!isZZChannel)ofs << std::string(sigSystp1_JetRes) << " param 0.0 1.0 "  <<endl;// peakSystFactor << endl; 
  ofs << std::string(sigSystp2_JetRes) << " param 0.0 1.0 "  << endl;//widthSystFactor << endl;
	
  ofs.close();
  fitResultsFile->Close();

  std::cout << std::endl << std::endl;
  std::cout << "+++ DATACARD FOR MASS " << mass << " ( " << nxj << " JETS, "<<pur_str.c_str()<<"   " << leptType_str << " CHANNEL ) IS DONE." << std::endl;
  std::cout << std::endl;

  /////////////////////////////////
  //////////////////
  // datacard is done. now create output workspace and write it to rootfile

  char outfileName[900];
  if(dims=="1d")
    sprintf( outfileName, "%s/%.0f/%s_%s.input.root", datacardDir.c_str(), mass,channel_marker.c_str(), suffix);
  if(dims=="2d")
    sprintf( outfileName, "%s/%.0f_%.3f/%s_%s.input.root", datacardDir.c_str(), mass,width,channel_marker.c_str(), suffix);

  TFile* outfile = TFile::Open( outfileName, "RECREATE");
  outfile->cd();
  RooWorkspace* w = new RooWorkspace("w","w");
  w->addClassDeclImportDir("/afs/cern.ch/cms/slc5_amd64_gcc434/lcg/roofit/5.28.00a-cms3/include/");
  //w->addClassDeclImportDir("/cmsrm/pc18/pandolf/CMSSW_4_2_3_patch1/src/HZZlljj/HZZlljjAnalyzer/test/analysis/FIT/PDFs");


  /////////////////////////////
  //->  import variable in output workspace:
  w->import(*CMS_xzz_mZZ);

  //////////////////////
  //-> import observed dataset:
  //dataset_obs->SetName("dataset_obs");
  w->import(*dataset_obs_reduced);

  ////////////////////
  //->  get Bkgd shape:

  RooAbsPdf* background_decorr =0;
  // //bgws->Print("v");
  //  RooAbsPdf *expo_fit =0;
  //  if(pur==1 && isZZChannel){	  
  //     expo_fit =bgws->pdf(("exp_fit_"+channel_marker).c_str());
  //     expo_fit->SetName(bkgd_shape_name.c_str());
  //     // and import it:
  //     w->import(*expo_fit, RooFit::RecycleConflictNodes());
  //   }
  //   else{
 
  //This if you want ot use the leveled expo
  background_decorr = bgws->pdf(("levexp_dcr_"+channel_marker).c_str());
  background_decorr->SetName(bkgd_shape_name.c_str());
  w->import(*background_decorr, RooFit::RecycleConflictNodes());
  //  }
  
  //// now define signal shape:
  //// (didn manage to do use get_signalShape without a crash):

  // ------------------- Crystal Ball (matched) -------------------------------
  cout<<"Starting Signal Shape part"<<endl;
  CMS_xzz_mZZ->setBins(10000.0,"cache");

  char sigp1name[200];//m
  char sigp2name[200];//width
  char sigp3name[200];//junction point of left pow law
  char sigp4name[200];//pow coeff of left pow law
  char sigp5name[200];//junction point of right pow law
  char sigp6name[200];//pow coeff of right pow law
  char sigp7name[200];//gamma
  sprintf(sigp1name,"CMS_%s_sig%dJ%s%s_p1",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigp2name,"CMS_%s_sig%dJ%s%s_p2",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigp3name,"CMS_%s_sig%dJ%s%s_p3",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigp4name,"CMS_%s_sig%dJ%s%s_p4",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigp5name,"CMS_%s_sig%dJ%s%s_p5",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str()); 
  sprintf(sigp6name,"CMS_%s_sig%dJ%s%s_p6",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str()); 
  sprintf(sigp7name,"CMS_%s_sig%dJ%s%s_p7",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str()); 
  char sigp1name_nom[200];//m
  char sigp2name_nom[200];//width
  char sigp3name_nom[200];//junction point of left pow law
  char sigp4name_nom[200];//pow coeff of left pow law
  char sigp5name_nom[200];//junction point of right pow law
  char sigp6name_nom[200];//pow coeff of right pow law
  char sigp7name_nom[200];//bw gamma
  sprintf(sigp1name_nom,"%s_nom",sigp1name);
  sprintf(sigp2name_nom,"%s_nom",sigp2name);
  sprintf(sigp3name_nom,"%s_nom",sigp3name);
  sprintf(sigp4name_nom,"%s_nom",sigp4name);
  sprintf(sigp5name_nom,"%s_nom",sigp5name);
  sprintf(sigp6name_nom,"%s_nom",sigp6name);
  sprintf(sigp7name_nom,"%s_nom",sigp7name);
  RooRealVar CB_mean_nom(sigp1name_nom,sigp1name_nom, get_signalParameter(nxj,pur_str,leptType_str, massH,"mean_match"));
  RooRealVar CB_sigma_nom(sigp2name_nom,sigp2name_nom,get_signalParameter(nxj,pur_str,leptType_str,massH,"sigma_match"));
  RooRealVar CB_alpha1_nom(sigp3name_nom,sigp3name_nom,get_signalParameter(nxj,pur_str,leptType_str,massH,"alpha1_match"));
  RooRealVar CB_n1_nom(sigp4name_nom,sigp4name_nom,get_signalParameter(nxj,pur_str,leptType_str,massH,"n1_match"));
  RooRealVar CB_alpha2_nom(sigp5name_nom,sigp5name_nom,get_signalParameter(nxj,pur_str,leptType_str,massH,"alpha2_match"));
  RooRealVar CB_n2_nom(sigp6name_nom,sigp6name_nom,get_signalParameter(nxj,pur_str,leptType_str,massH,"n2_match"));
  RooRealVar CB_gamma_nom(sigp7name_nom,sigp7name_nom,mass*width);

  //used for systematics
  RooRealVar CB_mean_lepscale(sigSystp1_LepScale,sigSystp1_LepScale,0.0);
  RooRealVar CB_mean_lepres(sigSystp1_LepRes,sigSystp1_LepRes,0.0);
  RooRealVar CB_mean_jes(sigSystp1_JetScale,sigSystp1_JetScale,0.0);
  RooRealVar CB_mean_jer(sigSystp1_JetRes,sigSystp1_JetRes,0.0);
  RooRealVar CB_sigma_lepscale(sigSystp2_LepScale,sigSystp2_LepScale,0.0);
  RooRealVar CB_sigma_lepres(sigSystp2_LepRes,sigSystp2_LepRes,0.0);
  RooRealVar CB_sigma_jes(sigSystp2_JetScale,sigSystp2_JetScale,0.0);
  RooRealVar CB_sigma_jer(sigSystp2_JetRes,sigSystp2_JetRes,0.0);


  RooArgList mean_sigshape_vars(CB_mean_nom,shapeSystPeak_LepScale,CB_mean_lepscale,shapeSystPeak_JetScale,CB_mean_jes,shapeSystPeak_JetRes,CB_mean_jer,"mean_sigshape_vars");
  RooArgList sigma_sigshape_vars(CB_sigma_nom,shapeSystWidth_LepScale,CB_sigma_lepscale,shapeSystWidth_JetScale,CB_sigma_jes,shapeSystWidth_JetRes,CB_sigma_jer,"sigma_sigshape_vars");
  // RooProduct CB_mean(sigp1name,sigp1name,mean_sigshape_vars);
  // RooProduct CB_sigma(sigp2name,sigp2name,sigma_sigshape_vars);

  char sigMeanname[200];//m
  char sigSigmaname[200];//width
  sprintf(sigMeanname,"CMS_%s_sig%dJ%s%s_MEAN",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str()); 
  sprintf(sigMeanname,"CMS_%s_sig%dJ%s%s_SIGMA",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str()); 
  RooFormulaVar CB_mean(sigMeanname,sigMeanname,"@0*(1+@1*@2)*(1+@3*@4)*(1+@5*@6)",mean_sigshape_vars);
  RooFormulaVar CB_sigma(sigSigmaname,sigSigmaname,"@0*(1+@1*@2)*(1+@3*@4)*(1+@5*@6)",sigma_sigshape_vars);

  RooAbsPdf* CB_SIG=0;
  
  if(dims=="1d"){
    CB_SIG = new RooDoubleCB("CB_SIG","Crystal Ball",*CMS_xzz_mZZ,CB_mean,CB_sigma,CB_alpha1_nom,CB_n1_nom,CB_alpha2_nom,CB_n2_nom);
  }

  RooRealVar zero("zero","zero",0);
  
  if(dims=="2d"){
    RooDoubleCB* Resol = new RooDoubleCB("CB_SIG","Crystal Ball",*CMS_xzz_mZZ,zero,CB_sigma,CB_alpha1_nom,CB_n1_nom,CB_alpha2_nom,CB_n2_nom);
  
    RooBreitWigner* Core = new RooBreitWigner("Core","Core",*CMS_xzz_mZZ,CB_mean,CB_gamma_nom);
    
    RooFFTConvPdf* CB_TMP = new RooFFTConvPdf("PlotFunc","PlotFunc",*CMS_xzz_mZZ, *Core, *Resol);
    
    CB_TMP->setBufferFraction(1.0);
    CB_SIG=CB_TMP;
  }

  cout<<"List of params of DoubleCB: CB_mean="<<CB_mean.getVal()<<"  CB_sigma="<<CB_sigma.getVal()<<"  CB_alpha1="<<CB_alpha1_nom.getVal()<<"  CB_n1="<<CB_n1_nom.getVal()<<"  CB_alpha2="<<CB_alpha2_nom.getVal()<<"   CB_n2="<<CB_n2_nom.getVal()<<"   CB_gamma="<<CB_gamma_nom.getVal()<<endl;

  // ------------------- SmearedTriangle (jets un-matched to gen-level) -------------------------------
  char sigUMp1name[200];//mean of CB
  char sigUMp2name[200];//width of CB
  char sigUMp3name[200];//junction point of pow law 
  char sigUMp4name[200];//pow coeff of pow law
  sprintf(sigUMp1name,"CMS_%s_sig%dJ%s%s_UnM_p1",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigUMp2name,"CMS_%s_sig%dJ%s%s_UnM_p2",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigUMp3name,"CMS_%s_sig%dJ%s%s_UnM_p3",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigUMp4name,"CMS_%s_sig%dJ%s%s_UnM_p4",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());

  RooRealVar CB_UMmean(sigUMp1name,sigUMp1name, get_signalParameter(nxj,pur_str,leptType_str,massH,"mean_unmatch"));
  RooRealVar CB_UMsigma(sigUMp2name,sigUMp2name,get_signalParameter(nxj,pur_str,leptType_str,massH,"sigma_unmatch"));
  RooRealVar CB_UMalpha(sigUMp3name,sigUMp3name,get_signalParameter(nxj,pur_str,leptType_str,massH,"alpha_unmatch"));
  RooRealVar CB_UMn(sigUMp4name,sigUMp4name,get_signalParameter(nxj,pur_str,leptType_str,massH,"n_unmatch"));
  RooCBShape* CB_UM = new RooCBShape("CB_UM","Crystal Ball unmacthed",*CMS_xzz_mZZ,CB_UMmean,CB_UMsigma ,CB_UMalpha,CB_UMn);

  char sigUMp5name[200];//left vertex of triangle
  char sigUMp6name[200];//top vertex of triangle
  char sigUMp7name[200];//right vertex of triangle
  sprintf(sigUMp1name,"CMS_%s_sig%dJ%s%s_UnM_p5",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigUMp2name,"CMS_%s_sig%dJ%s%s_UnM_p6",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigUMp3name,"CMS_%s_sig%dJ%s%s_UnM_p7",channel_marker.c_str(),nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  RooRealVar TRI_start(sigUMp5name,sigUMp5name, get_signalParameter(nxj,pur_str,leptType_str,massH,"unmatched_Mass_start"));
  RooRealVar TRI_turn(sigUMp6name,sigUMp6name, get_signalParameter(nxj,pur_str,leptType_str,massH,"unmatched_Mass_turn"));
  RooRealVar TRI_stop(sigUMp7name,sigUMp7name, get_signalParameter(nxj,pur_str,leptType_str,massH,"unmatched_Mass_stop"));
  Triangle* TRI = new Triangle("TRI","TRI",*CMS_xzz_mZZ,TRI_start,TRI_turn,TRI_stop);

  //------------------------ convolution -------------------------


  RooFFTConvPdf* TRI_SMEAR =0;
  if(nxj==2){
    TRI_SMEAR =new RooFFTConvPdf("TRI_SMEAR","triangle (X) CB",*CMS_xzz_mZZ,*TRI,*CB_UM);
    TRI_SMEAR->setBufferFraction(5.0);
  }

  //------------------------ add matched and unmatched -------------------------
  RooRealVar MATCH("MATCH","MATCH", get_signalParameter(nxj,pur_str,leptType_str,massH,"machfrac"));
  RooAddPdf* signal2J=0;
  if(nxj==2) signal2J= new RooAddPdf("sumCBTriangle_2J","sumCBTriangle_2J",*CB_SIG,*TRI_SMEAR,MATCH);

  // and import it:
  if(nxj==1){
    CB_SIG->SetName((signalProcessName+rename_str).c_str());
    w->import(*CB_SIG, RooFit::RecycleConflictNodes());
  }
  if(nxj==2){
    signal2J->SetName((signalProcessName+rename_str).c_str());
    w->import(*signal2J, RooFit::RecycleConflictNodes());
  }


  // done. now save:
  cout<<"Saving Signal shape"<<endl;
  outfile->cd();
  w->Write();
  outfile->Close();
  //  delete outfile;
  //  delete   mzzws; delete mzzFile;
  double sigWindowLow=CB_mean.getVal()-3.0*CB_sigma.getVal();
  double sigWindowHigh=CB_mean.getVal()+3.0*CB_sigma.getVal();
  CMS_xzz_mZZ->setRange("signalWindow",sigWindowLow,sigWindowHigh);
  double signalFrac= CB_SIG->createIntegral(*CMS_xzz_mZZ,RooFit::Range("signalWindow"),RooFit::NormSet(*CMS_xzz_mZZ))->getVal();
  std::cout<<"FRACTION of signal inside the +/-3 sigma window ["<<sigWindowLow<<" , "<< sigWindowHigh<<"]: "<<signalFrac<<std::endl;

  bool doPlot=false;
  // if(mass==650||mass==1000||mass==1500||mass==1600||mass==1700||mass==1900||mass==2000||mass==2100||mass==2400||mass==2500)doPlot=true;
  //if(mass==650||mass==1000||mass==1750||mass==1900||mass==2250||mass==2500)doPlot=true;
  if(mass==2200)doPlot=true;
  if(doPlot){
    const int nBinsTMP=nBins1;
    double binsTMP[nBinsTMP];
    for(int ibtmp=0;ibtmp<nBinsTMP;ibtmp++){
      binsTMP[ibtmp]= bins1[ibtmp];
    }
	  
    double sigSF=200.0;//just for plotting
    //    if(mass<1050)sigSF=100.0;
    TCanvas *can1=new TCanvas("canvasCardsMZZ1", "MZZ-cards-CANVAS1",800,800);
    can1->cd();
    RooPlot *xf=CMS_xzz_mZZ->frame();
    //    std::stringstream ssbtag;
    //ssbtag << nxj;
    std::stringstream ssM;
    ssM << mass;
    CMS_xzz_mZZ->setRange("plotRange",mZZmin_,mZZmax_) ;
    //  std::cout<<"\nNorm range of background_decorr (2): "<<background_decorr->normRange()<<std::endl;
    xf->SetTitle(("Sideband fit ("+ ssnxj.str() +"Jet "+ pur_str+", "+leptType_str+" leptons) - M="+ssM.str()+")").c_str());
    if(unblind)dataset_obs_reduced->plotOn(xf,RooFit::Binning(RooBinning(nBinsTMP-1,binsTMP)),RooFit::MarkerStyle(20),RooFit::MarkerColor(kBlack));
    std::cout<<" 1 "<<std::flush;
//     if(pur==1&&isZZChannel){
//       expo_fit->plotOn(xf, RooFit::Normalization(rate_background,RooAbsPdf::NumEvent), RooFit::LineColor(kViolet-2),RooFit::VisualizeError(*bgFitResult,2.0,kFALSE),RooFit::FillColor(kYellow),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
//       std::cout<<" 2 "<<std::flush;
//       expo_fit->plotOn(xf, RooFit::Normalization(rate_background,RooAbsPdf::NumEvent), RooFit::LineColor(kViolet-2),RooFit::VisualizeError(*bgFitResult,1.0,kFALSE),RooFit::FillColor(kGreen),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
//       expo_fit->plotOn(xf, RooFit::Normalization(rate_background,RooAbsPdf::NumEvent), RooFit::LineColor(kViolet-2),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
//       std::cout<<" 3 "<<std::flush;
//     }
//     else{
    if(dims=="1d") background_decorr->plotOn(xf, RooFit::Normalization(rate_background,RooAbsPdf::NumEvent), RooFit::LineColor(kViolet-2),RooFit::VisualizeError(*bgFitResult,2.0,kFALSE),RooFit::FillColor(kYellow),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
      std::cout<<" 2 "<<std::flush;
    if(dims=="1d")   background_decorr->plotOn(xf, RooFit::Normalization(rate_background,RooAbsPdf::NumEvent), RooFit::LineColor(kViolet-2),RooFit::VisualizeError(*bgFitResult,1.0,kFALSE),RooFit::FillColor(kGreen),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
      background_decorr->plotOn(xf, RooFit::Normalization(rate_background,RooAbsPdf::NumEvent), RooFit::LineColor(kViolet-2),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
      std::cout<<" 3 "<<std::flush;

      //  }//end else  if(pur==1&&isZZChannel)

    if(nxj==1){
      CB_SIG->plotOn(xf,RooFit::Normalization(MATCH.getVal()*rate_gg*sigSF,RooAbsPdf::NumEvent), RooFit::LineColor(kBlue),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
    }
    else{
      TRI_SMEAR->plotOn(xf,RooFit::Normalization((1-MATCH.getVal())*rate_gg,RooAbsPdf::NumEvent), RooFit::LineColor(kOrange+3),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
      signal2J->plotOn(xf,RooFit::Normalization(rate_gg,RooAbsPdf::NumEvent), RooFit::LineColor(kRed),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
    }
    std::cout<<" 4 "<<std::flush;
    if(unblind)dataset_obs_reduced->plotOn(xf,RooFit::Binning(RooBinning(nBinsTMP-1,binsTMP)),RooFit::MarkerStyle(20),RooFit::MarkerColor(kBlack));
    std::cout<<" 5 "<<std::flush;
	  
    char mkdir_command[100];
    sprintf( mkdir_command, "mkdir -p %s/fitPlotCards", datacardDir.c_str());
    system(mkdir_command);
    string canvasname= datacardDir+"/fitPlotCards/fitPlotCards_"+channel_marker.c_str() +"_"+ssnxj.str()+"J"+pur_str+"_"+leptType_str;
    std::cout<<canvasname.c_str()<<std::endl;
    xf->Draw();
    can1->SaveAs((canvasname+"_M"+ssM.str()+".eps").c_str());
    double mymax=nxj==2?100:250.0;
    double mymin=nxj==2?0.0008:0.003;
    xf->SetMinimum(mymin);
    xf->SetMaximum(mymax);
    gPad->SetLogy();
    xf->Draw();
    can1->SaveAs((canvasname+"_M"+ssM.str()+"_log.eps").c_str());
    delete xf;
    delete can1;
	  
  }//end if doPlot



}





TF1* get_eff_vs_mass( const std::string& leptType_str, int nxj, int pur, float mZZmin , float mZZmax ) {


  TH1F::AddDirectory(kTRUE);

  ifstream ifsMC;
  if(isZZChannel)
    ifsMC.open("efficiencies_MCSig.txt"); //the points at which we have MC samples
  else
    ifsMC.open("efficiencies_MCSig_xww.txt"); //the points at which we have MC samples
	  

  TGraph* gr_eff_vs_mass = new TGraph(0);
  double highestMass=-1.0;

  int iPoint=0;
  while( ifsMC.good() ) {

    double mass,efficiency[6];//one for each category: EE1JHP, MM1JHP ,EE1JLP, MM1JLP , EE2J ,MM2J
    ifsMC >> mass >> efficiency[0] >> efficiency[1] >> efficiency[2] >>efficiency[3] >> efficiency[4] >>efficiency[5] ; 

    if(mass<mZZmin||mass>mZZmax)continue;

    int index = DataCardUtils::convert_leptType(leptType_str) + (nxj-1) + 2*(1-pur);
    if(nxj==2)index--;
    gr_eff_vs_mass->SetPoint( iPoint++, mass, efficiency[index] );
    if(mass>highestMass)highestMass=mass;
    //    gr_eff_vs_mass->Print("v");

  } //while masses


  char functName[200];
  sprintf( functName, "eff_vs_mass_%s_%dJ", leptType_str.c_str(), nxj );
  TF1* f1_eff_vs_mass = new TF1(functName, "[0] + [1]*x + [2]*x*x + [3]*x*x*x", 550., 2550.);
  gr_eff_vs_mass->Fit(f1_eff_vs_mass, "RQN");
  f1_eff_vs_mass->SetLineStyle(2);
  f1_eff_vs_mass->SetLineColor(38);


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* axes = new TH2D("axes", "", 10, 550., highestMass*1.10, 10, 0., 0.25);
  axes->SetStats(0);
  axes->SetXTitle("m_{H} [GeV]");
  axes->SetYTitle("Efficiency");
  axes->Draw();

  gr_eff_vs_mass->SetMarkerStyle(20);
  gr_eff_vs_mass->SetMarkerSize(1.3);
  gr_eff_vs_mass->SetMarkerColor(46);
  f1_eff_vs_mass->Draw("same");
  gr_eff_vs_mass->Draw("Psame");

  /*
    TPaveText* labelCMS = db->get_labelCMS();
    TPaveText* labelSqrt = db->get_labelSqrt();

    labelCMS->Draw("same");
    labelSqrt->Draw("same");
  */


  std::string leptType_forlabel = (leptType_str=="ELE") ? "electron" : "muon";
  std::string purType_forlabel = "";
  if(pur==1) purType_forlabel="HP";
  if(pur==0) purType_forlabel="LP";
  char channelLabelText[300];
  sprintf(channelLabelText, "%d Jet category %s (%s channel)", nxj, purType_forlabel.c_str(),leptType_forlabel.c_str() );

  TPaveText* labelChannel = new TPaveText( 0.4, 0.4, 0.85, 0.45, "brNDC");
  labelChannel->SetTextSize(0.035);
  labelChannel->SetFillColor(0);
  labelChannel->AddText(channelLabelText);
  labelChannel->Draw("same");

  char effDirName[200]="EfficiencyFits";
  char mkdirCommand[200];
  sprintf( mkdirCommand, "mkdir -p %s", effDirName );

  system( mkdirCommand ); 

  char canvasName[500];
  sprintf( canvasName, "%s/effFit_%s_%s_%dJ%s.eps", effDirName,channel_marker.c_str() ,leptType_str.c_str(), nxj,purType_forlabel.c_str());
  c1->SaveAs(canvasName);
  sprintf( canvasName, "%s/effFit_%s_%s_%dJ%s.pdf", effDirName,channel_marker.c_str() , leptType_str.c_str(), nxj,purType_forlabel.c_str());
  c1->SaveAs(canvasName);

  delete c1;

  return f1_eff_vs_mass;

}



TheorSigParameters get_thParameters( float mass ) {

  std::string nameXsecFile = "../../data/xsect_BulkG_ZZ_c0p5_xsect_in_pb.txt";
  if(!isZZChannel)nameXsecFile = "../../data/xsect_BulkG_WW_c0p5_xsect_in_pb.txt";
  std::ifstream xsect_file(nameXsecFile.c_str());

  if (! xsect_file.is_open()) { 
    std::cout << "Failed to open file with xsections"<<endl;
    exit(13111);
  }

  xsect_file.clear();
  xsect_file.seekg(0);

  float mH(-99.0), XSgg(-99.0), XSgg_p(-99.0), XSgg_m(-99.0), XSpdfgg_p(-99.0),XSpdfgg_m(-99.0), BRXtoZZ(-99.0), BRZZto2l2q(0.0941);
  //XSvbf, XSvbf_p, XSvbf_m,XSpdfvbf_p,XSpdfvbf_m,    Gamma;

  float mH_old, XSgg_old, XSgg_p_old, XSgg_m_old, XSpdfgg_p_old,XSpdfgg_m_old;// BRXtoZZ_old, BRZZto2l2q_old;
  //XSvbf_old, XSvbf_p_old, XSvbf_m_old,XSpdfvbf_p_old,XSpdfvbf_m_old,   Gamma_old;

  TheorSigParameters hp;

  while(xsect_file.good()) {

    mH_old = mH;
    XSgg_old = XSgg;
    XSgg_p_old = XSgg_p;
    XSgg_m_old = XSgg_m;
    XSpdfgg_p_old = XSpdfgg_p;
    XSpdfgg_m_old = XSpdfgg_m;
    //    BRXtoZZ_old = BRXtoZZ;
    // BRZZto2l2q_old = BRZZto2l2q;

    xsect_file >> mH >> XSgg;
    BRXtoZZ=0.0;//unknown to me in this moment
    BRZZto2l2q=0.0941;
    if(!isZZChannel)BRZZto2l2q=0.2882464;

    if( mH == mass ) {

      hp.mH = mH;
      hp.XSgg = XSgg;//XSgg must be in pb !!!
      hp.XSgg_p = XSgg_p;
      hp.XSgg_m = XSgg_m;
      hp.XSpdfgg_p = XSpdfgg_p;
      hp.XSpdfgg_m = XSpdfgg_m;

      hp.BRXtoZZ = BRXtoZZ;
      hp.BRZZto2l2q = BRZZto2l2q;

      break;

    } if( mass > mH_old && mass < mH ) {

      hp.mH = mass;
      // hp.XSgg       = linear_interp( XSgg, XSgg_old, mass, mH, mH_old );
      hp.XSgg       = expo_interp( XSgg, XSgg_old, mass, mH, mH_old );
      hp.XSgg_p     = linear_interp( XSgg_p, XSgg_p_old, mass, mH, mH_old );
      hp.XSgg_m     = linear_interp( XSgg_m, XSgg_m_old, mass, mH, mH_old );
      hp.XSpdfgg_p  = linear_interp( XSpdfgg_p, XSpdfgg_p_old, mass, mH, mH_old );
      hp.XSpdfgg_m  = linear_interp( XSpdfgg_m, XSpdfgg_m_old, mass, mH, mH_old );
      hp.BRXtoZZ = BRXtoZZ;
      hp.BRZZto2l2q = BRZZto2l2q;

      break;

    } // if

  } //while ifs

  return hp;

}




double linear_interp( double x, double x_old, double mass, double mH, double mH_old ) {

  return (x_old + ( x-x_old ) * ( mass-mH_old ) / ( mH-mH_old ));

}

double expo_interp(double s2, double s1,  double newM,double m2,double m1){

  if(m1>m2){
    double tmps=s1;
    double tmpm=m1;
    s1=s2;
    m1=m2;
    s2=tmps;
    m2=tmpm;
  }
  double deltaM=m2-m1;
  double alpha=(log(s2)-log(s1)) / deltaM;
  double newS=s1*pow(exp(newM-m1),alpha);
  return newS;
}



double get_signalParameter(int nxj,  const std::string& purType_str,const  std::string& leptType_str, double massH, std::string varname) {

  // std::string leptType_str2="ELE";//hack for using only ELE shapes

  const int nMasses=20;
  int masses[nMasses] = {600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500};

  RooRealVar var(varname.c_str(),varname.c_str(),0.);
  RooArgSet paramsup, paramslow;

  paramsup.add(var);
  paramslow.add(var);

  char filename[200];

  //which files to read
  for(int i =0 ; i <nMasses ; i++){
    if(masses[i]==massH){//direct Match outpars_BulkG_ZZ_lljj_c0p2_M1800_1.config
      //      sprintf(filename,"shape/pars/outpars_BulkG_ZZ_lljj_c0p2_M%d_%dJ_%s_%s.config",masses[i],nxj,purType_str.c_str(),leptType_str.c_str());
      sprintf(filename,"shape/pars/outpars_BulkG_ZZ_lljj_c0p2_M%d_%dJ__%s.config",masses[i],nxj,leptType_str.c_str());
      if(!isZZChannel)sprintf(filename,"shape/pars/outpars_BulkG_WW_lvjj_c0p2_M%d_xww_%dJ__%s.config",masses[i],nxj,leptType_str.c_str());
      paramsup.readFromFile(filename, "READ");
      //  cout<<"For MH="<<massH<<" "<<varname.c_str()<<" = "<<var.getVal()<<endl;
      return var.getVal();
    }
  }
  //  cout<<"get_signalParameter -> interpolate signal shape param "<<varname.c_str()<<endl;
  //no direct match => interpolation
  int indexlow = -1;
  int indexhigh= -1;
  for(int i =0 ; i <nMasses ; i++){
    if(masses[i]>massH){
      indexhigh=i;
      break;
    }
  }
  for(int i =nMasses-1 ; i >-1 ; i--){
    if(masses[i]<massH){
      indexlow=i;
      break;
    }
  }
  if(indexlow==-1 || indexhigh== -1){
    std::cout << "Out of Range"<< std::endl;
    exit(1);
  }

  //std::cout << indexlow << " " << indexhigh <<std::endl;

  //  sprintf(filename,"shape/pars/outpars_BulkG_ZZ_lljj_c0p2_M%d_%dJ_%s_%s.config",masses[indexlow],nxj,purType_str.c_str(),leptType_str.c_str());
  sprintf(filename,"shape/pars/outpars_BulkG_ZZ_lljj_c0p2_M%d_%dJ__%s.config",masses[indexlow],nxj,leptType_str.c_str());
  if(!isZZChannel)sprintf(filename,"shape/pars/outpars_BulkG_WW_lvjj_c0p2_M%d_xww_%dJ__%s.config",masses[indexlow],nxj,leptType_str.c_str());
  paramsup.readFromFile(filename, "READ");
  double low = var.getVal();
  //  sprintf(filename,"shape/pars/outpars_BulkG_ZZ_lljj_c0p2_M%d_%dJ_%s_%s.config",masses[indexhigh],nxj,purType_str.c_str(),leptType_str.c_str());
  sprintf(filename,"shape/pars/outpars_BulkG_ZZ_lljj_c0p2_M%d_%dJ__%s.config",masses[indexhigh],nxj,leptType_str.c_str());
  if(!isZZChannel)sprintf(filename,"shape/pars/outpars_BulkG_WW_lvjj_c0p2_M%d_xww_%dJ__%s.config",masses[indexhigh],nxj,leptType_str.c_str());
  paramsup.readFromFile(filename, "READ");
  double high = var.getVal();

  double deltaM = masses[indexhigh] - masses[indexlow];

  return (massH-masses[indexlow])/deltaM*(high-low) + low;

}




std::string systString( std::pair<double,double> systPair, double maxDiff ) {

  double syst_ave = 1. + 0.5*(fabs(systPair.first-1.) + fabs(systPair.second-1.));

  char syst_char[100];
  if( fabs(syst_ave-systPair.second)/syst_ave < maxDiff )
    sprintf( syst_char, "%f    ", syst_ave );
  else
    sprintf( syst_char, "%f/%f", systPair.first, systPair.second );

  std::string syst_str(syst_char);

  return syst_str;

}



std::pair<double,double> theorSyst( double errMinus, double errPlus, double addMinus, double addPlus ) {

  float systPlus  = sign(errPlus) *sqrt(errPlus*errPlus   + addPlus*addPlus);
  float systMinus = sign(errMinus)*sqrt(errMinus*errMinus + addMinus*addMinus);

  systPlus  += 1.;
  systMinus += 1.;

  std::pair<double,double> returnPair;
  returnPair.first = systMinus;
  returnPair.second = systPlus;

  return returnPair;

}

std::pair<double,double> theorSyst_HighmH( double mHVal){

  //extra error due to Higgs width
  double theoryHighMass = 1.5*(mHVal/1000)*(mHVal/1000.0)*(mHVal/1000.0);//mHVal=Higgs mass in GeV
  double systPlus=1.0*theoryHighMass;
  double systMinus=-1.0*theoryHighMass;

  systPlus  += 1.;
  systMinus += 1.;

  std::pair<double,double> returnPair;
  returnPair.first = systMinus;
  returnPair.second = systPlus;

  return returnPair;

}

std::pair<double,double> leptTriggerSyst( const std::string& leptType_str) {

  double syst;

  if( leptType_str=="MU" )  syst = CMS_trigger_m;// Defined in Config_XZZ.h and Config_XWW.h
  if( leptType_str=="ELE" ) syst = CMS_trigger_e;// Defined in Config_XZZ.h and Config_XWW.h

  std::pair<double,double> returnPair;
  returnPair.first  = syst; //symmetrical for now
  returnPair.second = syst;

  return returnPair;

}

std::pair<double,double> leptEffSyst( const std::string& leptType_str) {

  double syst;

  if( leptType_str=="MU" )  syst = CMS_eff_m;// Defined in Config_XZZ.h and Config_XWW.h
  if( leptType_str=="ELE" ) syst = CMS_eff_e;// Defined in Config_XZZ.h and Config_XWW.h

  std::pair<double,double> returnPair;
  returnPair.first  = syst; //symmetrical for now
  returnPair.second = syst;

  return returnPair;

}

std::pair<double,double> leptScaleSyst( const std::string& leptType_str, double mass) {

  double syst;

  if( leptType_str=="MU" ){
    syst = CMS_scale_m; // Defined in Config_XZZ.h and Config_XWW.h

    //following comes from prelim study by Thiago, to be revisited
    //if(isZZChannel){//
    //  if(mass<=1200)syst=-0.00678 + 1.22E-05*mass;
    //  else       syst= -0.14218 + 12.177E-05*mass;
    // }
    

  }


  if( leptType_str=="ELE" ) syst = CMS_scale_e; // Defined in Config_XZZ.h and Config_XWW.h

  std::pair<double,double> returnPair;
  returnPair.first  = syst; //symmetrical for now
  returnPair.second = syst;

  return returnPair;

}


std::pair<double,double> jetScaleSyst( double mass ) {



  std::pair<double,double> returnPair;

  /*  //  float p0= 8.3  , p1=-0.0215 ;
  // float m0=-8.6, m1=0.02 ;
  float p0=-0.0135   , p1=-0.004189 ;
  float m0=-3.3649   , m1= 0.006892 ;
  returnPair.first  = 1.0 + 0.01*(m0+m1*mass);//upper syst
  returnPair.second = 1.0 + 0.01*(p0+p1*mass);//lower syst
 
  */

  returnPair.first   = CMS_scale_j_up;  // Defined in Config_XZZ.h and Config_XWW.h
  returnPair.second  = CMS_scale_j_down;  // Defined in Config_XZZ.h and Config_XWW.h
  
  //temporary solution for WW: use a line from 1% to 3% from 600 to 2500
  if(!isZZChannel)
  {
    double error = 0.01 + (0.03-0.01)/(2500-600)*(mass-600);
    returnPair.first = 1+error;
    returnPair.second= 1-error;
  }

  return returnPair;

}

std::pair<double,double> puSyst( double mass ) {

  double systPU=0.006;// 0.6 permille for ZZ

  std::pair<double,double> returnPair;
  returnPair.first  = 1.0 - systPU;
  returnPair.second = 1.0 + systPU;



  return returnPair;

}


std::pair<double,double> VTagEffSyst( const std::string& leptType_str, int nxj, double mass, int purity ) {

  std::pair<double,double> returnPair;
  returnPair.first  = (1.0 - CMS_eff_vtag_tau21_sf_LP);//taken from Config
  returnPair.second = (1.0 + CMS_eff_vtag_tau21_sf_LP); 
  if(purity == 1){//1J HP
    returnPair.first  = (1.0 + CMS_eff_vtag_tau21_sf_HP);//sign must be oppostite of LP because they are anti-correlated
    returnPair.second = (1.0 - CMS_eff_vtag_tau21_sf_HP); 
  }
	
  return returnPair;

}



std::pair<double,double> bTagEffSyst( const std::string& leptType_str, int nxj, double mass ) {

  float p0=0.0, p1=0.0;
  float m0=0.0, m1=0.0;

  if( leptType_str=="ELE" ) {
    if(nxj==0){
      p0=0.98644 ;//0.983256647923;
      p1=-0.000100716 ;//-0.0000883532570978;
      m0= 1.0176 ;//1.02907356239;
      m1=8.12716e-05 ;//0.0000713061639147;
    }
    else if(nxj==1){
      p0=1.04847 ;//1.04446110956;
      p1=-1.47685e-05 ;//-0.0000195508160829;
      m0=0.943369 ;//0.940063743731;
      m1=4.63272e-05 ;//0.0000737044467898;
    }
    else if(nxj==2){
      p0=1.1035 ;//1.13365470372;
      p1=1.74066e-05 ;//0.00000584572717646;
      m0=0.885855 ;//0.82161771535;
      m1=-4.08034e-07 ;//-0.0000161054152592;
    }
  }
  else if(leptType_str=="MU" ) {
    if(nxj==0){
      p0=0.990292 ;//0.984636818312;
      p1=-0.000108687;//-0.0000898705296203;
      m0= 1.02153;//1.02836905579;
      m1= 7.44788e-05 ;//0.0000726807344479;
    }
    else if(nxj==1){
      p0=1.04438 ;//1.04385580002;
      p1=-9.26161e-06 ;//0.0000206096278947;
      m0=1.02153 ;//0.938029 ;//0.942713582987;
      m1=5.95741e-05 ;//0.0000719882385098;
    }
    else if(nxj==2){
      p0=1.11229;//1.1333366687;
      p1=6.55678e-07 ;//0.00000462542786413;
      m0=0.897045 ;//0.813316607701;
      m1=-2.25839e-05;//-0.00000205840248842;
    }
  }

  std::pair<double,double> returnPair;
  returnPair.first  = m1*mass+m0;
  returnPair.second = p1*mass+p0;

  return returnPair;

}



// double backgroundNorm( const std::string& dataset, const std::string& leptType_str, int nxj ) {

//   int leptType_int = SidebandFitter::convert_leptType( leptType_str );

//   std::string fileName = "HZZlljjRM_DATA_" + dataset + "_optLD_looseBTags_v2_ALL.root";
//   TFile* file_data = TFile::Open(fileName.c_str());
//   TTree* tree = (TTree*)file_data->Get("tree_passedEvents");
//   char selection[400];
//   sprintf(selection, "isSidebands && leptType==%d && nxj==%d", leptType_int, nxj);
//   float nEvents_sidebands = tree->GetEntries(selection);


//   return nEvents_sidebands;

// }


