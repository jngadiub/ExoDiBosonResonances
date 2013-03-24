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
#include "RooProdPdf.h"
#include "RooAddPdf.h"
#include "RooFFTConvPdf.h"
#include "RooFitResult.h"
#include "RooWorkspace.h"

#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"

#include "PdfDiagonalizer.h"

#include "DataCardUtils.h"

const std::string wsDir="FitSidebandsMJJ_CA8_V5/";
const std::string datacardDir("DataCards_XZZ_20130322_V5");
float mZZmin_ = 600.;


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



void create_singleDatacard( float mass, float lumi, const std::string& leptType_str, int nxj, int pur, TF1* f1_eff_vs_mass );

TheorSigParameters get_thParameters( float mass );

double linear_interp( double x, double x_old, double mass, double mH, double mH_old );
double expo_interp(double s2, double s1,  double newM,double m2,double m1);
TF1* get_eff_vs_mass( const std::string& leptType_str, int nxj, int pur, float mZZmin );


double get_signalParameter(int nxj,  const std::string& purType_str, const std::string& leptType_str, double massH, std::string varname);

std::string systString( std::pair<double,double> systPair, double maxDiff=0.01 );
std::pair<double,double> theorSyst( double errMinus, double errPlus, double addMinus=0., double addPlus=0. );
std::pair<double,double> theorSyst_HighmH( double mHVal );
std::pair<double,double> leptTriggerSyst( const std::string& leptType_str);
std::pair<double,double> leptEffSyst( const std::string& leptType_str);
std::pair<double,double> leptScaleSyst( const std::string& leptType_str);

std::pair<double,double> jetScaleSyst( double mass );
std::pair<double,double> puSyst( double mass );
std::pair<double,double> VTagEffSyst( const std::string& leptType_str, int nxj, double mass );
std::pair<double,double> bTagEffSyst( const std::string& leptType_str, int nbtag, double mass );

//double backgroundNorm( const std::string& dataset, const std::string& leptType_str, int nxj );


int main( int argc, char* argv[] ) {



  float lumi_ELE;
  float lumi_MU;
  lumi_ELE=19538.85; //pb^-1
  lumi_MU =19538.85; //pb^-1

  //first loop over available signal MC files to fit efficiency:
  TF1* f1_eff_vs_mass_MU_1JHP = get_eff_vs_mass("MU", 1,1, mZZmin_);
  TF1* f1_eff_vs_mass_MU_1JLP = get_eff_vs_mass("MU", 1,0, mZZmin_);
  TF1* f1_eff_vs_mass_MU_2J = get_eff_vs_mass("MU", 2,-1, mZZmin_);//set purity to -1 for 2J cat

  TF1* f1_eff_vs_mass_ELE_1JHP = get_eff_vs_mass("ELE", 1,1, mZZmin_);
  TF1* f1_eff_vs_mass_ELE_1JLP = get_eff_vs_mass("ELE", 1,0, mZZmin_);
  TF1* f1_eff_vs_mass_ELE_2J = get_eff_vs_mass("ELE", 2,-1, mZZmin_);

  std::ifstream ifs("masses.txt");
  
  while( ifs.good() ) {
    
    float mass;
    ifs >> mass;

    std::cout << std::endl << std::endl;;
    std::cout << "++++++++++++++++++++++" << std::endl;
    std::cout << "+++ MASS: " << mass << std::endl;
    std::cout << "++++++++++++++++++++++" << std::endl;
    std::cout << std::endl;

    //    std::string datacardDir("DataCards_20120210") ;
    char mkdir_command[100];
    sprintf( mkdir_command, "mkdir -p %s/%.0f", datacardDir.c_str(), mass);
    system(mkdir_command);

    create_singleDatacard( mass, lumi_ELE, "ELE", 1,1, f1_eff_vs_mass_ELE_1JHP);
    //create_singleDatacard( mass, lumi_ELE, "ELE", 1,0, f1_eff_vs_mass_ELE_1JLP);
    //    create_singleDatacard( mass, lumi_ELE, "ELE", 2,-1, f1_eff_vs_mass_ELE_2J);
    create_singleDatacard( mass, lumi_MU,   "MU", 1,1, f1_eff_vs_mass_MU_1JHP);
    //    create_singleDatacard( mass, lumi_MU,   "MU", 1,0, f1_eff_vs_mass_MU_1JLP);
    //create_singleDatacard( mass, lumi_MU,   "MU", 2,-1, f1_eff_vs_mass_MU_2J);

  } //while masses

  return 0;

}



void create_singleDatacard( float mass, float lumi, const std::string& leptType_str, int nxj,int pur, TF1* f1_eff_vs_mass ) {

  if( leptType_str!="ELE" && leptType_str!="MU" ) {
    std::cout << "Unkown Lept Type '" << leptType_str << "'. Exiting." << std::endl;
    exit(12333);
  }


  std::string pur_str="";
  if(pur==0)pur_str="LP";
  if(pur==1)pur_str="HP";
    


  TheorSigParameters hp = get_thParameters(mass);

  string rename_str="";
  std::stringstream ssnxj;
  ssnxj << nxj;
  rename_str += "_"+leptType_str+ssnxj.str()+"J"+pur_str;

  // open fitResults file (all lept types):
  std::string fitResultsFileName = DataCardUtils::get_fitResultsRootFileName( nxj,pur_str, "ALL" ,wsDir.c_str());
  std::cout << "reading results from: "<< fitResultsFileName.c_str() << std::endl;
  TFile* fitResultsFile = TFile::Open(fitResultsFileName.c_str());

  // fitResultsFile->ls();

  // get fit result:
  char fitResultName[200]; 
  //  sprintf( fitResultName, "resultsExpLevelledFit_%dJ_ALL_decorr", nxj );
  //  sprintf( fitResultName, "resultsExpoFit_%dJ_%s",nxj , leptType_str.c_str() );
  sprintf( fitResultName, "resultsExpoFit_%dJ_%s_ALL",nxj,pur_str.c_str() );
  cout<<"Trying to pick RooFitResult :"<<fitResultName<<endl;
  RooFitResult* bgFitResult = (RooFitResult*)fitResultsFile->Get(fitResultName);
  bgFitResult->Print("v");

 
  // get workspace:
  char workspaceName[200];
  sprintf( workspaceName, "ws_alpha_%dJ_%s_ALL", nxj,pur_str.c_str() );
  RooWorkspace* bgws = (RooWorkspace*)fitResultsFile->Get(workspaceName);
  // cout<<"\n\nPrinting contents of the WorkSpace: "<<endl;
  //  bgws->Print("v");

  //get vars containing syst unc on alpha
  std::vector<RooRealVar*>alphaErr;
  RooArgList bgPars = bgFitResult->floatParsFinal();
  std::vector<std::string> parname; 
  parname.reserve(bgPars.getSize());
  for( int iVar=0; iVar<bgPars.getSize(); ++iVar ) {
    parname.push_back(bgPars.at(iVar)->GetName());
    parname.at(iVar)+="_alphaErr";
    std::cout<<"Var "<<iVar<<"  Parname: "<<parname.at(iVar).c_str()<<std::endl;
    alphaErr.push_back(bgws->var(parname.at(iVar).c_str()));
  }

  //get global alpha uncertainty
  //the +0.5 effectively leaves the bkgd normalization free to float when profiling the nuisances
  double globalAlphaErr =  bgws->var("alphaNormErr")->getVal()+0.5;
  //std::cout << globalAlphaErr << std::endl;
  //exit(0);

  //// get main variable from input workspace:
  RooRealVar* CMS_xzz_mZZ = new RooRealVar("mZZ","mZZ",mZZmin_,2400.0);//it works
  //   RooRealVar* CMS_hzz2l2q_mZZ = bgws->var("mZZ");//it does not work
  //   RooRealVar* CMS_hzz2l2q_mZZ = mzzws->var("mZZ");//reading it from MZZ-sideband ws works
  CMS_xzz_mZZ->setMin(mZZmin_); 


  char suffix[100];
  sprintf( suffix, "%s%s%dJ%s", (DataCardUtils::leptType_datacards(leptType_str)).c_str(), (DataCardUtils::leptType_datacards(leptType_str)).c_str(), nxj,pur_str.c_str());
  std::string suffix_str(suffix);

 

  char datacardName[400];
  sprintf( datacardName, "%s/%.0f/xzz_%s.%.0f.txt", datacardDir.c_str(), mass, suffix, mass);


  std::ofstream ofs(datacardName);
  ofs << "# Card for process XZZ->"<<suffix << std::endl;
  ofs << "#imax 1  number of channels" << std::endl;
  ofs << "#jmax 1  number of backgrounds" << std::endl;
  ofs << "#kmax *  number of nuisance parameters (sources of systematical uncertainties)" << std::endl;
  ofs << "------------ " << std::endl;
  ofs << "shapes sig CMS_xzz_" << suffix_str << " xzz_" << suffix_str << ".input.root  w:" <<("signal"+rename_str).c_str()<< std::endl;
  ofs << "shapes background CMS_xzz_" << suffix_str << " xzz_" << suffix_str << ".input.root w:"<<("background_expo"+rename_str).c_str()  << std::endl;
  ofs << "shapes data_obs   CMS_xzz_" << suffix_str << " xzz_" << suffix_str << ".input.root w:"<<("dataset_obs"+rename_str).c_str()  << std::endl;
  ofs << "------------ " << std::endl;
  ofs << "bin         CMS_xzz_" << suffix << std::endl;

  RooDataSet* dataset_obs = DataCardUtils::get_observedDataset( bgws , leptType_str, nxj,pur, "dsDataSIG" );
  dataset_obs->SetName(( dataset_obs->GetName()+rename_str).c_str());
  std::cout<<"Statistics of the observed dataset straight from the ws: "<<dataset_obs->numEntries()<<"  "<<dataset_obs->sumEntries() <<std::endl;
  RooDataSet* dataset_obs_reduced=new RooDataSet("dataset_obs","dataset_obs",dataset_obs,RooArgSet(*CMS_xzz_mZZ));
  dataset_obs_reduced->SetName(("dataset_obs"+rename_str).c_str());
  float observedYield = dataset_obs_reduced->sumEntries();
  std::cout << "observation   " << observedYield << std::endl;

  ofs << "observation   " << observedYield << std::endl;
  ofs << "------------ " << std::endl;
  ofs << "bin                CMS_xzz_" << suffix <<  "\tCMS_xzz_" << suffix << std::endl;
  ofs << "process            sig\t\t\tbackground" << std::endl;
  ofs << "process            0\t\t\t1" << std::endl;

  float eff = f1_eff_vs_mass->Eval(hp.mH);
  float rate_gg   = eff*hp.XSgg*hp.BRZZto2l2q*lumi; //xsect has both ee and mm

  // compute expected BG yield from observed sideband events:
  Double_t rate_background = DataCardUtils::get_backgroundNormalization(bgws , leptType_str);
  std::cout <<"Background rate: "<< rate_background << std::endl;


  ofs << "rate               " << rate_gg << "\t\t" << rate_background << std::endl;
  ofs << "------------ " << std::endl;


  // and now systematics:

  ofs << "lumi\t\t\tlnN\t1.022\t\t\t1.0" << std::endl;

  //std::pair<double,double> pdf_gg  = theorSyst( hp.XSpdfgg_m, hp.XSpdfgg_p, 0.04, 0.015 );
  //ofs << "pdf_gg\t\tlnN\t" << systString(pdf_gg) << "\t1.0" << std::endl;
    
  //std::pair<double,double> QCDscale_ggH = theorSyst( hp.XSgg_m, hp.XSgg_p);
  //ofs << "QCDscale_ggH\tlnN\t" << systString(QCDscale_ggH) << "\t1.0" << std::endl;
  
  ofs << "CMS_trigger_" << DataCardUtils::leptType_datacards(leptType_str) << "\tlnN\t" << systString(leptTriggerSyst(leptType_str)) << "\t1.0" << std::endl;

  ofs << "CMS_eff_" << DataCardUtils::leptType_datacards(leptType_str) << "\t\tlnN\t" << systString(leptEffSyst(leptType_str)) << "\t1.0" << std::endl;

  ofs << "CMS_scale_" << DataCardUtils::leptType_datacards(leptType_str) << "\t\tlnN\t" << systString(leptScaleSyst(leptType_str)) <<  "\t1.0" << std::endl;

  ofs << "CMS_scale_j\t\tlnN\t" << systString(jetScaleSyst(hp.mH)) <<  "\t1.0" << std::endl;

  ofs << "CMS_eff_vtag\t\tlnN\t" << systString(VTagEffSyst(leptType_str, nxj, hp.mH)) << "\t1.0" << std::endl;

  ofs << "CMS_xzz_pu\t\tlnN\t"<<systString(puSyst(hp.mH)) <<"\t\t\t1.0" << std::endl;
  std::cout << "CMS_xzz_pu\t\tlnN\t"<<systString(puSyst(hp.mH)) <<"\t\t\t1.0" << std::endl;

  ofs << "CMS_xzz_alphanorm"<<nxj<<"b\t\tlnN\t 1.0 "<<"\t\t\t" << 1.+globalAlphaErr << std::endl;

  

  // syst done. now finish with parameters:

  double bgNorm = DataCardUtils::get_backgroundNormalization(bgws,leptType_str,nxj,pur,"dsDataSB");
  char bgNorm_char[100];
  sprintf( bgNorm_char, "%.0lf", bgNorm);
  std::string bgNorm_str(bgNorm_char);
  std::cout<<"SIDEBAND NorM: "<<bgNorm<<"  ("<<  DataCardUtils::get_backgroundNormalization(bgws,leptType_str,nxj,pur)<<")"<<std::endl;
  double alpha = rate_background/bgNorm;
  char alpha_char[100];
  sprintf( alpha_char, "%lf", alpha);
  std::string alpha_str(alpha_char);

  char bgNormName[200];
  sprintf( bgNormName, "CMS_xzz_bkg%dJ%s%s%sp0", nxj,pur_str.c_str(), (DataCardUtils::leptType_datacards(leptType_str)).c_str(), (DataCardUtils::leptType_datacards(leptType_str)).c_str() );
  std::string bgNormName_str(bgNormName);
  ofs << bgNormName_str << "\tgmN " << bgNorm_str << "\t---\t" << alpha_str << std::endl;
  //std::cout << bgNormName_str << "\tgmN " << bgNorm_str << "\t-----\t-----\t" << alpha_str << std::endl;

  //  RooArgList bgPars = bgFitResult->floatParsFinal();

  cout<<"Looping on "<<bgPars.getSize()<<" params"<<endl;
  for( int iVar=0; iVar<bgPars.getSize(); ++iVar ) {
    RooRealVar* thisVar = dynamic_cast<RooRealVar*>(bgPars.at(iVar));
    cout<<"Var -> "<<thisVar->GetName()<<flush;
    double varValue=thisVar->getVal();
    cout<<"="<<varValue<<flush;
    double varError=thisVar->getError();
    cout<<" +/- "<<varError<<endl; 
    varError=sqrt(varError*varError);//+  alphaErr.at(iVar)->getVal()*alphaErr.at(iVar)->getVal());
 
    ofs << thisVar->GetName() << "\tparam\t\t" <<varValue  << "\t" << varError << std::endl;

    //    std::cout << thisVar->GetName() << "\tparam\t\t" << varValue << "\t" << thisVar->getError()  <<" & "<<alphaErr.at(iVar)->getVal()<<" -> "<<varError<< std::endl;
    std::cout << thisVar->GetName() << "\tparam\t\t" << varValue << "\t" << thisVar->getError() <<" -> "<<varError<< std::endl;
  }

  ofs.close();
  fitResultsFile->Close();
  
  std::cout << std::endl << std::endl;
  std::cout << "+++ DATACARD FOR MASS " << mass << " ( " << nxj << " JETS, "<<pur_str.c_str()<<"   " << leptType_str << " CHANNEL ) IS DONE." << std::endl;
  std::cout << std::endl;

  // datacard is done. now create output workspace and write it to rootfile

  char outfileName[900];
  sprintf( outfileName, "%s/%.0f/xzz_%s.input.root", datacardDir.c_str(), mass, suffix);
  TFile* outfile = TFile::Open( outfileName, "RECREATE");
  outfile->cd();


  RooWorkspace* w = new RooWorkspace("w","w");
  w->addClassDeclImportDir("/afs/cern.ch/cms/slc5_amd64_gcc434/lcg/roofit/5.28.00a-cms3/include/");
  //w->addClassDeclImportDir("/cmsrm/pc18/pandolf/CMSSW_4_2_3_patch1/src/HZZlljj/HZZlljjAnalyzer/test/analysis/FIT/PDFs");



  // import variable in output workspace:
  w->import(*CMS_xzz_mZZ);

  // import observed dataset:
  //dataset_obs->SetName("dataset_obs");
  //dataset_obs->SetName("dataset_obs");
  w->import(*dataset_obs_reduced);
  
  // get BG shape:
  RooAbsPdf *expo_fit = bgws->pdf("exp_fit");
  expo_fit->SetName(("background_expo"+rename_str).c_str());
  // and import it:
  w->import(*expo_fit, RooFit::RecycleConflictNodes());

  //  RooAbsPdf* background_decorr = bgws->pdf("levexp_dcr");
  // background_decorr->SetName("background_decorr");
  //  w->import(*background_decorr, RooFit::RecycleConflictNodes());


  //// now define signal shape:

  // now define signal shape (didn manage to do use get_signalShape without a crash):

  // ------------------- Crystal Ball (matched) -------------------------------
  cout<<"Starting Signal Shape part"<<endl;
  CMS_xzz_mZZ->setBins(10000.0,"cache");
  float massH = hp.mH;
  char sigp1name[200];//m
  char sigp2name[200];//width
  char sigp3name[200];//junction point of left pow law
  char sigp4name[200];//pow coeff of left pow law
  char sigp5name[200];//junction point of right pow law
  char sigp6name[200];//pow coeff of right pow law
  sprintf(sigp1name,"CMS_xzz_sig%dJ%s%s_p1",nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigp2name,"CMS_xzz_sig%dJ%s%s_p2",nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigp3name,"CMS_xzz_sig%dJ%s%s_p3",nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigp4name,"CMS_xzz_sig%dJ%s%s_p4",nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigp5name,"CMS_xzz_sig%dJ%s%s_p5",nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str()); 
  sprintf(sigp6name,"CMS_xzz_sig%dJ%s%s_p6",nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str()); 
  RooRealVar CB_mean(sigp1name,sigp1name, get_signalParameter(nxj,pur_str,leptType_str, massH,"mean_match"));
  RooRealVar CB_sigma(sigp2name,sigp2name,get_signalParameter(nxj,pur_str,leptType_str,massH,"sigma_match"));
  RooRealVar CB_alpha1(sigp3name,sigp3name,get_signalParameter(nxj,pur_str,leptType_str,massH,"alpha1_match"));
  RooRealVar CB_n1(sigp4name,sigp4name,get_signalParameter(nxj,pur_str,leptType_str,massH,"n1_match"));
  RooRealVar CB_alpha2(sigp5name,sigp5name,get_signalParameter(nxj,pur_str,leptType_str,massH,"alpha2_match"));
  RooRealVar CB_n2(sigp6name,sigp6name,get_signalParameter(nxj,pur_str,leptType_str,massH,"n2_match"));

  RooDoubleCB* CB_SIG = new RooDoubleCB("CB_SIG","Crystal Ball",*CMS_xzz_mZZ,CB_mean,CB_sigma,CB_alpha1,CB_n1,CB_alpha2,CB_n2);

  cout<<"List of params of DoubleCB: CB_mean="<<CB_mean.getVal()<<"  CB_sigma="<<CB_sigma.getVal()<<"  CB_alpha1="<<CB_alpha1.getVal()<<"  CB_n1="<<CB_n1.getVal()<<"  CB_alpha2="<<CB_alpha2.getVal()<<"   CB_n2="<<CB_n2.getVal()<<endl;

  // ------------------- SmearedTriangle (jets un-matched to gen-level) -------------------------------
  char sigUMp1name[200];//mean of CB
  char sigUMp2name[200];//width of CB
  char sigUMp3name[200];//junction point of pow law 
  char sigUMp4name[200];//pow coeff of pow law
  sprintf(sigUMp1name,"CMS_xzz_sig%dJ%s%s_UnM_p1",nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigUMp2name,"CMS_xzz_sig%dJ%s%s_UnM_p2",nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigUMp3name,"CMS_xzz_sig%dJ%s%s_UnM_p3",nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigUMp4name,"CMS_xzz_sig%dJ%s%s_UnM_p4",nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());

  RooRealVar CB_UMmean(sigUMp1name,sigUMp1name, get_signalParameter(nxj,pur_str,leptType_str,massH,"mean_unmatch"));
  RooRealVar CB_UMsigma(sigUMp2name,sigUMp2name,get_signalParameter(nxj,pur_str,leptType_str,massH,"sigma_unmatch"));
  RooRealVar CB_UMalpha(sigUMp3name,sigUMp3name,get_signalParameter(nxj,pur_str,leptType_str,massH,"alpha_unmatch"));
  RooRealVar CB_UMn(sigUMp4name,sigUMp4name,get_signalParameter(nxj,pur_str,leptType_str,massH,"n_unmatch"));
  RooCBShape* CB_UM = new RooCBShape("CB_UM","Crystal Ball unmacthed",*CMS_xzz_mZZ,CB_UMmean,CB_UMsigma ,CB_UMalpha,CB_UMn);

  char sigUMp5name[200];//left vertex of triangle
  char sigUMp6name[200];//top vertex of triangle
  char sigUMp7name[200];//right vertex of triangle
  sprintf(sigUMp1name,"CMS_xzz_sig%dJ%s%s_UnM_p5",nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigUMp2name,"CMS_xzz_sig%dJ%s%s_UnM_p6",nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
  sprintf(sigUMp3name,"CMS_xzz_sig%dJ%s%s_UnM_p7",nxj,pur_str.c_str(),DataCardUtils::leptType_datacards(leptType_str).c_str());
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
    CB_SIG->SetName(("signal"+rename_str).c_str());
    w->import(*CB_SIG, RooFit::RecycleConflictNodes());
  }
  if(nxj==2){
    signal2J->SetName(("signal"+rename_str).c_str());
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
  if(mass==1000||mass==1500||mass==1900||mass==2000)doPlot=true;
  if(doPlot){
    const int nBinsTMP=22;
    const double binsTMP[nBinsTMP]={480,500,520,560,600,640,680,720,760,800,840,920,
			    1000,1100,1250,1400,1600,1800,2000,2200,2400,2600};
  
    TCanvas *can1=new TCanvas("canvasCardsMZZ1", "MZZ-cards-CANVAS1",800,800);
    can1->cd();
    RooPlot *xf=CMS_xzz_mZZ->frame();
    //    std::stringstream ssbtag;
    //ssbtag << nxj;
    std::stringstream ssM;
    ssM << mass;
    CMS_xzz_mZZ->setRange("plotRange",480,2600) ;
    //  std::cout<<"\nNorm range of background_decorr (2): "<<background_decorr->normRange()<<std::endl;
    xf->SetTitle(("Sideband fit ("+ ssnxj.str() +"Jet "+ pur_str+", "+leptType_str+" leptons) - M="+ssM.str()+")").c_str());
    dataset_obs_reduced->plotOn(xf,RooFit::Binning(RooBinning(nBinsTMP-1,binsTMP)),RooFit::MarkerStyle(20),RooFit::MarkerColor(kBlack));
    std::cout<<" 1 "<<std::flush;
    expo_fit->plotOn(xf, RooFit::Normalization(rate_background,RooAbsPdf::NumEvent), RooFit::LineColor(kViolet-2),RooFit::VisualizeError(*bgFitResult,2.0,kFALSE),RooFit::FillColor(kYellow),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
    std::cout<<" 2 "<<std::flush;
    expo_fit->plotOn(xf, RooFit::Normalization(rate_background,RooAbsPdf::NumEvent), RooFit::LineColor(kViolet-2),RooFit::VisualizeError(*bgFitResult,1.0,kFALSE),RooFit::FillColor(kGreen),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
    expo_fit->plotOn(xf, RooFit::Normalization(rate_background,RooAbsPdf::NumEvent), RooFit::LineColor(kViolet-2),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
    std::cout<<" 3 "<<std::flush;

    if(nxj==1){
      CB_SIG->plotOn(xf,RooFit::Normalization(MATCH.getVal()*rate_gg*1000.0,RooAbsPdf::NumEvent), RooFit::LineColor(kBlue),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
    }
    else{
      TRI_SMEAR->plotOn(xf,RooFit::Normalization((1-MATCH.getVal())*rate_gg,RooAbsPdf::NumEvent), RooFit::LineColor(kOrange+3),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
      signal2J->plotOn(xf,RooFit::Normalization(rate_gg,RooAbsPdf::NumEvent), RooFit::LineColor(kRed),RooFit::NormRange("plotRange"),RooFit::Range("plotRange"));
    }
    std::cout<<" 4 "<<std::flush;
    dataset_obs_reduced->plotOn(xf,RooFit::Binning(RooBinning(nBinsTMP-1,binsTMP)),RooFit::MarkerStyle(20),RooFit::MarkerColor(kBlack));
     std::cout<<" 5 "<<std::flush;

    char mkdir_command[100];
    sprintf( mkdir_command, "mkdir -p %s/fitPlotCards", datacardDir.c_str());
    system(mkdir_command);
    string canvasname= datacardDir+"/fitPlotCards/fitPlotCards_"+ssnxj.str()+"J"+leptType_str;
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





TF1* get_eff_vs_mass( const std::string& leptType_str, int nxj, int pur, float mZZmin ) {


  TH1F::AddDirectory(kTRUE);

  ifstream ifsMC("efficiencies_MCSig.txt"); //the points at which we have MC samples

  TGraph* gr_eff_vs_mass = new TGraph(0);

  int iPoint=0;
  while( ifsMC.good() ) {

    double mass,efficiency[6];//one for each category: EE1JHP, MM1JHP ,EE1JLP, MM1JLP , EE2J ,MM2J
    ifsMC >> mass >> efficiency[0] >> efficiency[1] >> efficiency[2] >>efficiency[3] >> efficiency[4] >>efficiency[5] ; 

    int index = DataCardUtils::convert_leptType(leptType_str) + (nxj-1) + (1-pur);
    
    gr_eff_vs_mass->SetPoint( iPoint++, mass, efficiency[index] );

    gr_eff_vs_mass->Print("v");

  } //while masses


  char functName[200];
  sprintf( functName, "eff_vs_mass_%s_%dJ", leptType_str.c_str(), nxj );
  TF1* f1_eff_vs_mass = new TF1(functName, "[0] + [1]*x + [2]*x*x + [3]*x*x*x", 550., 2050.);
  gr_eff_vs_mass->Fit(f1_eff_vs_mass, "RQN");
  f1_eff_vs_mass->SetLineStyle(2);
  f1_eff_vs_mass->SetLineColor(38);

 
  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* axes = new TH2D("axes", "", 10, 550., 2250., 10, 0., 0.40);
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

  TPaveText* labelChannel = new TPaveText( 0.4, 0.2, 0.85, 0.25, "brNDC");
  labelChannel->SetTextSize(0.035);
  labelChannel->SetFillColor(0);
  labelChannel->AddText(channelLabelText);
  labelChannel->Draw("same");

  char effDirName[200]="EfficiencyFits";
  char mkdirCommand[200];
  sprintf( mkdirCommand, "mkdir -p %s", effDirName );
  
  system( mkdirCommand ); 

  char canvasName[500];
  sprintf( canvasName, "%s/effFit_%s_%dJ%s.eps", effDirName, leptType_str.c_str(), nxj,purType_forlabel.c_str());
  c1->SaveAs(canvasName);

  delete c1;

  return f1_eff_vs_mass;

}



TheorSigParameters get_thParameters( float mass ) {

  std::string nameXsecFile = "../../data/xsect_BulkG_ZZ_c0p5_xsect_in_pb.txt";
  std::ifstream xsect_file(nameXsecFile.c_str());

  if (! xsect_file.is_open()) { 
    std::cout << "Failed to open file with xsections"<<endl;
    exit(13111);
  }

  xsect_file.clear();
  xsect_file.seekg(0);
 
  float mH, XSgg, XSgg_p, XSgg_m, XSpdfgg_p,XSpdfgg_m, BRXtoZZ, BRZZto2l2q;
  //XSvbf, XSvbf_p, XSvbf_m,XSpdfvbf_p,XSpdfvbf_m,    Gamma;

  float mH_old, XSgg_old, XSgg_p_old, XSgg_m_old, XSpdfgg_p_old,XSpdfgg_m_old, BRXtoZZ_old, BRZZto2l2q_old;
  //XSvbf_old, XSvbf_p_old, XSvbf_m_old,XSpdfvbf_p_old,XSpdfvbf_m_old,   Gamma_old;

  TheorSigParameters hp;

  while(xsect_file.good()) {

    mH_old = mH;
    XSgg_old = XSgg;
    XSgg_p_old = XSgg_p;
    XSgg_m_old = XSgg_m;
    XSpdfgg_p_old = XSpdfgg_p;
    XSpdfgg_m_old = XSpdfgg_m;
    BRXtoZZ_old = BRXtoZZ;
    BRZZto2l2q_old = BRZZto2l2q;

    xsect_file >> mH >> XSgg;
    BRXtoZZ=0.0;//unknown to me in this moment
    BRZZto2l2q=0.0941;

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



double get_signalParameter(int nxj,  const std::string& purType_str, const std::string& leptType_str, double massH, std::string varname) {

  const int nMasses=15;
  int masses[nMasses] = {600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000};

  RooRealVar var(varname.c_str(),varname.c_str(),0.);
  RooArgSet paramsup, paramslow;

  paramsup.add(var);
  paramslow.add(var);

  char filename[200];

  //which files to read
  for(int i =0 ; i <nMasses ; i++){
    if(masses[i]==massH){//direct Match outpars_BulkG_ZZ_lljj_c0p2_M1800_1.config
      sprintf(filename,"shape/pars/outpars_BulkG_ZZ_lljj_c0p2_M%d_%dJ_%s_%s.config",masses[i],nxj,purType_str.c_str(),leptType_str.c_str());
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

  sprintf(filename,"shape/pars/outpars_BulkG_ZZ_lljj_c0p2_M%d_%dJ_%s_%s.config",masses[indexlow],nxj,purType_str.c_str(),leptType_str.c_str());
  paramsup.readFromFile(filename, "READ");
  double low = var.getVal();
  sprintf(filename,"shape/pars/outpars_BulkG_ZZ_lljj_c0p2_M%d_%dJ_%s_%s.config",masses[indexhigh],nxj,purType_str.c_str(),leptType_str.c_str());
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

  if( leptType_str=="MU" )  syst = 1.02;
  if( leptType_str=="ELE" ) syst = 1.01;

  std::pair<double,double> returnPair;
  returnPair.first  = syst; //symmetrical for now
  returnPair.second = syst;

  return returnPair;

}

std::pair<double,double> leptEffSyst( const std::string& leptType_str) {

  double syst;

  if( leptType_str=="MU" )  syst = 1.008;
  if( leptType_str=="ELE" ) syst = 1.034;

  std::pair<double,double> returnPair;
  returnPair.first  = syst; //symmetrical for now
  returnPair.second = syst;

  return returnPair;

}

std::pair<double,double> leptScaleSyst( const std::string& leptType_str) {

  double syst;

  if( leptType_str=="MU" )  syst = 1.01;
  if( leptType_str=="ELE" ) syst = 1.03;

  std::pair<double,double> returnPair;
  returnPair.first  = syst; //symmetrical for now
  returnPair.second = syst;

  return returnPair;

}


std::pair<double,double> jetScaleSyst( double mass ) {

  //  float p0= 8.3  , p1=-0.0215 ;
  // float m0=-8.6, m1=0.02 ;
  float p0=-0.0135   , p1=-0.004189 ;
  float m0=-3.3649   , m1= 0.006892 ;

  std::pair<double,double> returnPair;
  returnPair.first  = 1.0 + 0.01*(m0+m1*mass);
  returnPair.second = 1.0 + 0.01*(p0+p1*mass);

  return returnPair;

}

std::pair<double,double> puSyst( double mass ) {


  float c0=-0.824   , c1=0.00446 ;
 
  std::pair<double,double> returnPair;
  returnPair.first  = 1.0 + 0.01*(c0+c1*mass);
  returnPair.second = 1.0 + 0.01*(c0+c1*mass);

  return returnPair;

}


std::pair<double,double> VTagEffSyst( const std::string& leptType_str, int nxj, double mass ) {

 std::pair<double,double> returnPair;
 returnPair.first  = 1.1;
 returnPair.second = 0.9; 

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


