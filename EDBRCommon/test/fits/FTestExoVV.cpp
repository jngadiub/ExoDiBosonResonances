#include <sstream>
#include <string>
#include <fstream>
#include <Riostream.h>

#include "TH1D.h"
#include "TF1.h"
#include "TMath.h"
#include "TFitResultPtr.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveText.h"


#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooPlot.h"
#include "RooCurve.h"

#include "RooFitResult.h"
#include "RooWorkspace.h"
#include "RooDataHist.h"
#include "RooChi2Var.h"
#include "RooExponential.h"
#include "RooDataSet.h"
#include "RooExtendPdf.h"
#include "RooGenericPdf.h"
#include "RooExtendPdf.h"
#include "RooBinning.h"
#include "RooAddPdf.h"

//#include "RooLevelledExp.h"
#include "HZZ2L2QRooPdfs.h"
#include "DataCardUtils.h"
#include "Config_XZZ.h"

//using namespace RooFit ;
double get_signalParameter(int cat, double massH, std::string varname) ;
int findMyBin(const int nb,double *bin_array,double val);
Double_t myfunction(Double_t *x, Double_t *par){
  Float_t xx =x[0];
  Double_t f = par[0]*TMath::Exp(-1.0*(fabs(xx-240.0)/par[1]));
  return f;
}

//binning for merged Jet topology 
const int nBins1=23;
const double bins1[nBins1]={480,500,520,560,600,640,680,720,760,800,840,920,
			    1000,1100,1200,1300,1500,1700,1900,2100,2300,2600,2800};

//binning for double Jet topology 
const int nBins2=16;
const double bins2[nBins2]={480,500,520,560,600,640,680,720,760,800,840,920,
			    1000,1100,1250,1400};

int main(){
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING) ;


  string leptType_str="ALL";//ELE // "MU"
  // const string outDir="FitSidebandsAlphaHistSmoothenedWithToys";
  const string outDir="FitSidebandsMJJ_ZZ_20130701_prodv2c_ALL/";

  ofstream logf((outDir+"./log_FTest_MZZ_Std.log").c_str(),ios::out);

  //double iM=0.0;
  //  const double limitM=1000.0;
  TFile *fout=new TFile((outDir+"./histosChi2.root").c_str(),"RECREATE");

  int nxj=0,ipur=0;
  int icat=0;//icat==0 ->1JHP, icat==1 ->1JLP , icat==2 -> 2J
  for(nxj=1;nxj<3;nxj++){

    for(ipur=1;ipur>-1;ipur--){
      if(nxj==2){
	if(ipur>0)continue;
      }
      stringstream ssnxj;
      ssnxj<<nxj;

      std::string pur_str="";
      if(nxj==1){
	if(ipur==0)pur_str="LP";
	if(ipur==1)pur_str="HP";
      }

      logf<<"\n\n\n========================"<<endl;
      logf<<"Starting chi2 calculation for cat: "<<nxj<<"J "<<pur_str.c_str()<<"  "<<leptType_str.c_str()<<endl<<endl;
      

      // TFile *fcurve=new TFile((outDir+"/rooCurves_"+sscat.str()+"cat"+leptType_str+".root").c_str(),"READ");
      // RooCurve *curve_AlphaErr_UP=(RooCurve*)fcurve->Get("upper_staterr_alpha");
      // RooCurve *curve_AlphaErr_DOWN=(RooCurve*)fcurve->Get("lower_staterr_alpha");
      
      const int nBins1=23;
      const double bins1[nBins1]={480,500,520,560,600,640,680,720,760,800,840,920,
				  1000,1100,1200,1300,1500,1700,1900,2100,2300,2600,2800};


      std::string fitResultsFileName = DataCardUtils::get_fitResultsRootFileName( nxj,pur_str, leptType_str ,outDir,channel_marker);
      std::cout << "reading results from: "<< fitResultsFileName.c_str() << std::endl;
 
      logf<<"Load file "<<fitResultsFileName.c_str()<<std::flush;
      TFile *f=new TFile(fitResultsFileName.c_str(),"READ");
      logf<<"\t"<<f->GetName()<<std::endl;
      f->ls();
      char workspaceName[200];
      sprintf( workspaceName, "ws_alpha_%s_%dJ_%s_%s", channel_marker.c_str(),nxj,pur_str.c_str(),leptType_str.c_str() );
      cout<<"Trying to retrieve the ws named: "<<workspaceName<<endl;
      RooWorkspace *w1=(RooWorkspace*)f->Get(workspaceName);
  
      //logf<<"\nDumping contents of the WS: "<<("ws_alpha_"+sscat.str()+"cat"+leptType).c_str()<<std::endl;
      // w1->Print();

      const double minMZZ=bins1[0];
      const double maxMZZ=bins1[nBins1-1];
      RooRealVar *x=w1->var("mZZ");
      double minFitRange=500;// (nxj==1 ? 600.0 : 600);
      if(nxj==1&&ipur==0)minFitRange=650;
      double maxFitRange=(nxj==1 ? maxMZZ : bins2[nBins2-1]);
      x->setRange("fitRange",minFitRange,maxFitRange) ;

      cout<<"Loaded RooRealVar "<<std::flush;

      //  x->setRange(bins0[0],bins0[nBins-1]);
      ///  x->setRange("zoomedRange",bins0[0],bins0[nBins-1]) ;

      //  RooDataSet *SIGdsetFULL=(RooDataSet*)w1->data("dsDataSIGfull")->reduce(("nCats=="+sscat.str()).c_str());//+"&&mZZ>420&&mZZ<700"
      //  RooDataSet *SIGdset=(RooDataSet*)w1->data("dsDataSBSIG")->reduce(("nCats=="+sscat.str()).c_str());//+"&&mZZ>420&&mZZ<700"

      RooAbsData *SBdset=w1->data("dsDataSB2");//->reduce(("nCats=="+sscat+" && lep==0").c_str()); 
      RooAbsData *SIGdset=w1->data("dsDataSIG");

      cout<<" RooDataSet in SB, integral="<<SBdset->sumEntries()<<endl;
      logf<<" RooDataSet in SB, integral="<<SBdset->sumEntries()<<endl;

      // get BG shape:
      //  RooAbsPdf *expo_fit = w1->pdf("exp_fit");
      // RooAbsPdf *levexp_fit = w1->pdf("levexp_dcr");


      //////////////////
      // Levelled expo2: y = exp( -(x-m) / (sigma + alpha*(x-m) + beta*(x-m)^2 )
      // It becomes a simple expo for alpha=0 and beta=0

      //for simple expo
      double initf0a=90.0;
      if(nxj==1)initf0a=200.0;
      if(nxj==2)initf0a=200.0;
      double initf1a=0.0;
      if(nxj==1)initf1a=0.0;
      if(nxj==2)initf1a=0.0;
      double initf2a=0.0;
      if(nxj==1)initf2a=0.0;
      if(nxj==2)initf2a=0.0;
      double initm=60.0;
      if(nxj==2)initm=400.0;
      RooRealVar *f0a=new RooRealVar("f0a","sigma",initf0a,0.0,300.0);
      RooRealVar *f1a=new RooRealVar("f1a","alpha",0.0,-0.5,2.0);
      RooRealVar *f2a=new RooRealVar("f2a","beta",0.0,-0.5,2.0);
      RooRealVar *ma=new RooRealVar("ma","m",initm,200.0,500.0);
      RooRealVar *ta=new RooRealVar("ta","theta",0.0);
      f1a->setConstant(kTRUE);
      f2a->setConstant(kTRUE);
      ma->setConstant(kTRUE);
      ta->setConstant(kTRUE);
      RooLevelledExp2 *fit_1par=new RooLevelledExp2("fitFunc_1p","Simple exponential",*x,*f0a,*f1a,*f2a,*ma,*ta);
      RooFitResult* res_fit_1par = fit_1par->fitTo(*SBdset,RooFit::Save(kTRUE),RooFit::SumW2Error(kTRUE),RooFit::PrintLevel(-1),RooFit::Range("fitRange")) ;
      cout<<"PRINTING SIMPLE EXPO FIT FROM LEV EXPO:"<<endl;
      res_fit_1par->Print("v");

      double inislope;
      if(nxj==1)inislope=-0.25;
      if(nxj==2)inislope=-0.25;
      char slopePar_name[32];
      sprintf(slopePar_name,"a0_%dJ",nxj);
      RooRealVar *a0=new RooRealVar(slopePar_name,slopePar_name,inislope,-10.0,0.0);
      RooExponential *expo_fit=new RooExponential("exp_fit","exp_fit",*x,*a0);
      RooFitResult* r_sig2 = expo_fit->fitTo(*SBdset,RooFit::Save(kTRUE),RooFit::SumW2Error(kTRUE),RooFit::PrintLevel(-1),RooFit::Range("fitRange")) ;
      cout<<"PRINTING SIMPLE EXPO FIT:"<<endl;
      r_sig2->Print("v");

      ///for lev expo 
      RooRealVar *f0b=new RooRealVar("f0b","sigma",initf0a,0.0,300.0);
      RooRealVar *f1b=new RooRealVar("f1b","alpha",initf1a,-0.05,2.0);
      RooRealVar *f2b=new RooRealVar("f2b","beta",0.0,-0.05,2.0);
      RooRealVar *mb=new RooRealVar("mb","m",initm,200.0,500.0);
      RooRealVar *tb=new RooRealVar("tb","theta",0.0);
      f2b->setConstant(kTRUE);
      mb->setConstant(kTRUE);
      tb->setConstant(kTRUE);
      RooLevelledExp2 *fit_2par=new RooLevelledExp2("fitFunc_2p","Levelled expo (2par)",*x,*f0b,*f1b,*f2b,*mb,*tb);
      RooFitResult* res_fit_2par = fit_2par->fitTo(*SBdset,RooFit::Save(kTRUE),RooFit::SumW2Error(kTRUE),RooFit::PrintLevel(-1),RooFit::Range("fitRange")) ;
      res_fit_2par->Print("v");
      logf<<"PRINTING LEVELLED EXPO FIT (2pars):"<<endl;
      res_fit_2par->printMultiline(logf,99,true);

      //for lev expo2
      RooRealVar *f0c=new RooRealVar("f0c","sigma",initf0a,0.0,300.0);
      RooRealVar *f1c=new RooRealVar("f1c","alpha",initf1a,-0.5,2.0);
      RooRealVar *f2c=new RooRealVar("f2c","beta",initf2a,-0.5,2.0);
      RooRealVar *mc=new RooRealVar("mc","m",400,200.0,500.0);
      RooRealVar *tc=new RooRealVar("tc","theta",0.0);
      mc->setConstant(kTRUE);
      tc->setConstant(kTRUE);
      RooLevelledExp2 *fit_3par=new RooLevelledExp2("fitFunc_3p","Levelled expo (3par)",*x,*f0c,*f1c,*f2c,*mc,*tc);
      RooFitResult* res_fit_3par = fit_3par->fitTo(*SBdset,RooFit::Save(kTRUE),RooFit::SumW2Error(kTRUE),RooFit::PrintLevel(-1),RooFit::Range("fitRange")) ;
      logf<<"PRINTING LEVELLED EXPO FIT (3pars):"<<endl;
      res_fit_3par->printMultiline(logf,99,true);

      //////////////////
      // CTEQ funciton: y = (1-x)^alpha / (x^(beta+ ) )
      // It becomes a simple expo for alpha=0 and beta=0

      //     //for simple expo
      //     double initf0a=90.0;
      //     if(nxj==1)initf0a=200.0;
      //     if(nxj==2)initf0a=200.0;
      //     double initf1a=0.0;
      //     if(nxj==1)initf1a=0.0;
      //     if(nxj==2)initf1a=0.0;
      //     double initf2a=0.0;
      //     if(nxj==1)initf2a=0.0;
      //     if(nxj==2)initf2a=0.0;
      //     double initm=60.0;
      //     if(nxj==2)initm=400.0;
      //     RooRealVar *f0a=new RooRealVar("f0a","sigma",initf0a,0.0,300.0);
      //     RooRealVar *f1a=new RooRealVar("f1a","alpha",0.0,-0.5,2.0);
      //     RooRealVar *f2a=new RooRealVar("f2a","beta",0.0,-0.5,2.0);
      //     RooRealVar *ma=new RooRealVar("ma","m",initm,200.0,500.0);
      //     RooRealVar *ta=new RooRealVar("ta","theta",0.0);
      //     f1a->setConstant(kTRUE);
      //     f2a->setConstant(kTRUE);
      //     ma->setConstant(kTRUE);
      //     ta->setConstant(kTRUE);
      //     RooLevelledExp2 *fit_1par=new RooLevelledExp2("fitFunc_1p","Simple exponential",*x,*f0a,*f1a,*f2a,*ma,*ta);
      //     RooFitResult* res_fit_1par = fit_1par->fitTo(*SBdset,RooFit::Save(kTRUE),RooFit::SumW2Error(kTRUE),RooFit::PrintLevel(-1),RooFit::Range("fitRange")) ;
      //     cout<<"PRINTING SIMPLE EXPO FIT FROM LEV EXPO:"<<endl;
      //     res_fit_1par->Print("v");
  
  
      //  RooRealVar *Nent=new RooRealVar("sidebandNormalization","Integral of data in sidebands before signal-rescaling",SBdset->numEntries(),0.0,50000.0);
      // RooRealVar *Nerr=new RooRealVar("errNormalization","Statistical uncertainty on bkgd norm",sqrt(Nent->getVal()),0.0,50000.0);
 
      Double_t rate_background = DataCardUtils::get_backgroundNormalization(w1 , leptType_str);
      // RooRealVar *N=w1->var("bkgdNormalization"); 
      cout<<"\nRead from WS the bkgd normalization: "<<rate_background<<endl;
      RooRealVar *Nbkgd=new RooRealVar("bkgdNormFitRange","bkgdNorm",rate_background,0,rate_background*2.0);
      Nbkgd->setConstant(true);
      double globalAlphaErr =  w1->var("alphaNormErr")->getVal();
      double relBkgdNormErr=sqrt(1.0/(sqrt(SBdset->numEntries())*sqrt(SBdset->numEntries()) )+globalAlphaErr*globalAlphaErr);
      RooRealVar *Nbkgderr=new RooRealVar("bkgdNormErr","bkgdNormErr",relBkgdNormErr*Nbkgd->getVal(),0,rate_background*2.0);
      cout<<" Normalizations "<<flush;
 

      // get fit result:
      char fitResultName_eig[200]; 
      //  sprintf( fitResultName, "resultsExpLevelledFit_%dJ_%s_ALL_decorr", nxj ,pur_str.c_str());
      sprintf( fitResultName_eig, "resultsExpoFit_%s_%dJ_%s_ALL",channel_marker.c_str(),nxj,pur_str.c_str() );
      logf<<"Retrieving fit results: "<<fitResultName_eig<<std::endl;

      std::cout<<"Retriveing fit results: "<<fitResultName_eig<<std::endl;
      RooFitResult *resfit=(RooFitResult*)f->Get(fitResultName_eig);
      resfit->Print("v");
      cout<<" RooFitResults"<<endl;

      logf<<"\nYields:"<<endl;
      logf<<"Sample\t\t\t\tTotal ["<<x->getMin() <<","<<x->getMax() <<"]\t\t\t#[800, 1000]\t\t\t#[1700, 1900]"<<endl;
      //  logf<<"Data (SIG)\t\t\t" <<SIGdsetFULL->numEntries()<<"\t\t\t" <<SIGdsetFULL->reduce("mZZ>450&&mZZ<650")->numEntries()<<"\t\t\t" <<SIGdsetFULL->reduce("mZZ>375&&mZZ<1250")->numEntries()<<endl;
      logf<<"Data (SB-rescaled)\t\t"<<SBdset->sumEntries()<<"\t\t\t"<<SBdset->reduce("mZZ>800&&mZZ<1000")->sumEntries()<<"\t\t\t"<<SBdset->reduce("mZZ>1700&&mZZ<1900")->sumEntries()<<endl;
      logf<<"\nrate_background="<<rate_background<<endl;

      ////////////////
      // Chi2 from binned histo
      RooDataHist* dsetBinned =new RooDataHist("dhALLData","My Binned dataset",RooArgSet(*x), *SBdset);
      cout<<"Datahist loaded"<<endl;
      // TH1D *hbin2=(TH1D*)SIGdsetFULL->reduce("mZZ>375&&mZZ<1250")->createHistogram( ("h_dsbinned2_M"+ssM.str()+sscat.str()+"b").c_str(),*x,Binning(RooBinning(nBins-1,bins0)));
      TH1D *hbin2=(TH1D*)SBdset->createHistogram( ("h_dsbinned2_"+ssnxj.str()+"J"+pur_str).c_str(),*x,RooFit::Binning(RooBinning(80,465,2865)));  //RooBinning(37,600,2450)
      TH1D *hbinEE=(TH1D*)SBdset->reduce("lep==0")->createHistogram( ("h_dsbinnedEE_"+ssnxj.str()+"J"+pur_str).c_str(),*x,RooFit::Binning(RooBinning(80,465,2865))); //RooBinning(37,600,2450)

  
      //  TH1D *hbin=(TH1D*)dsetBinned->createHistogram("h_dsbinned",*x);
      double p1_totChi2=0.0,p2_totChi2=0.0,p3_totChi2=0.0,totObs=0.0;
      double ndof=0.0,p1_ndof=0.0,p2_ndof=0.0,p3_ndof=0.0;
      const double normFit=Nbkgd->getVal();
      const double normFitRange=SBdset->reduce(RooFit::CutRange("fitRange"))->sumEntries();
      logf<<"normFit="<<normFit<<"   normFitRange="<<normFitRange<<endl;
      //integral of fit func
      double fit_1parIntFitR=fit_1par->createIntegral(*x,RooFit::Range("fitRange"))->getVal();
      double fit_1parInt=fit_1par->createIntegral(*x,RooFit::NormSet(*x))->getVal();
      double fit_1parIntALL=fit_1par->createIntegral(*x)->getVal();
      logf<<"fit_1parIntALL="<<fit_1parIntALL <<"   fit_1parInt="<<fit_1parInt<<"  fit_1parIntFitR="<<fit_1parIntFitR<<endl;
      double fit_2parIntFitR=fit_2par->createIntegral(*x,RooFit::Range("fitRange"))->getVal();
      double fit_3parIntFitR=fit_3par->createIntegral(*x,RooFit::Range("fitRange"))->getVal();
      //Variables for F-Test: 
      double rss1=0;
      double rss2=0;
      double rss3=0;

      logf<<"\n\nDumping bin-by-bin chi2 calculation ("<<hbin2->GetNbinsX() <<" bins):"<<endl;
      logf<<"NonZero\t\tBinCenter\tBinContent\tBinErr\tChi2Contrib(Expo)\tChi2Contrib(LevExpo2pars)\tChi2Contrib(LevExpo3pars)"<<endl;
      for(int ib=1;ib<=hbin2->GetNbinsX();ib++){
	double low=hbin2->GetBinLowEdge(ib);
	double center=hbin2->GetBinCenter(ib);
	double high=hbin2->GetBinLowEdge(ib)+hbin2->GetBinWidth(ib);
	double bincont=hbin2->GetBinContent(ib); 
	double binerr=hbin2->GetBinError(ib);
	//x->setVal(center);

	char rangeName[128];
	sprintf(rangeName,"rangeIntegral%d",ib);
	x->setRange(rangeName,low,high);

	//calculate integrals of fit funcs in the bin (relative to full range of x, larger than fitRange!)
	double Fit1pFrac=fit_1par->createIntegral(*x,RooFit::Range(rangeName))->getVal() / fit_1parIntFitR;
	double Fit2pFrac=fit_2par->createIntegral(*x,RooFit::Range(rangeName))->getVal() / fit_2parIntFitR;;
	double Fit3pFrac=fit_3par->createIntegral(*x,RooFit::Range(rangeName))->getVal() / fit_3parIntFitR;;

	//   double Fit2pFrac=fit_2par->createIntegral(*x,RooFit::Range(rangeName),RooFit::NormSet(*x))->getVal();
	// double Fit3pFrac=fit_3par->createIntegral(*x,RooFit::Range(rangeName),RooFit::NormSet(*x))->getVal();

	double p1val=Fit1pFrac*normFitRange;
	double p2val=Fit2pFrac*normFitRange;
	double p3val=Fit3pFrac*normFitRange;

	totObs+=bincont;
	if(bincont>0&&binerr>0){
	  //fits are done on the same exptrap SB, don't double count errors
	  double p1_tmpChi2=(bincont-p1val)*(bincont-p1val)/(binerr*binerr);
	  rss1+=(bincont-p1val)*(bincont-p1val);
	  p1_totChi2+=p1_tmpChi2;

	  double p2_tmpChi2=(bincont-p2val)*(bincont-p2val)/(binerr*binerr);
	  rss2+=(bincont-p2val)*(bincont-p2val);
	  p2_totChi2+=p2_tmpChi2;
	  double p3_tmpChi2=(bincont-p3val)*(bincont-p3val)/(binerr*binerr);
	  rss3+=(bincont-p3val)*(bincont-p3val);
	  p3_totChi2+=p3_tmpChi2;
	  ndof+=1.0;
	  logf<<"  OK\t\t"<<center << "\t\t"<<bincont<<"\t\t"<<binerr<<"\t\t"<<p1_tmpChi2<<"\t\t"<<p2_tmpChi2<<"\t\t"<<p3_tmpChi2<<endl;//"\t\t"<<bincont-p1val<<"\t\t"<<bincont-p3val <<endl;
	}
	else{
	  logf<<"  ZERO ! BinCenter="<<center <<"  BinCont="<<bincont<<"   BinErr="<<binerr<<endl;
	}

      }//end loop on bins of histogram

      p1_ndof=ndof-1;
      p2_ndof=ndof-2;
      p3_ndof=ndof-3;
      logf<<"\nSimple Exponential Fit:"<<endl;
      logf<<"Hand-made chi2/ndof is Chi2="<<p1_totChi2<<" ; Ndof="<<p1_ndof<<" => "<<p1_totChi2/p1_ndof<<" Chi2Prob="<<TMath::Prob(p1_totChi2,p1_ndof) <<"  (TotObs="<<totObs<<")"<<endl;
      logf<<"\nLevelled Exponential Fit (2pars):"<<endl;
      logf<<"Hand-made chi2/ndof is Chi2="<<p2_totChi2<<" ; Ndof="<<p2_ndof<<" => "<<p2_totChi2/p2_ndof<<" Chi2Prob="<<TMath::Prob(p2_totChi2,p2_ndof) <<"  (TotObs="<<totObs<<")"<<endl;
      logf<<"\nLevelled Exponential Fit (3pars):"<<endl;
      logf<<"Hand-made chi2/ndof is Chi2="<<p3_totChi2<<" ; Ndof="<<p3_ndof<<" => "<<p3_totChi2/p3_ndof<<" Chi2Prob="<<TMath::Prob(p3_totChi2,p3_ndof) <<"  (TotObs="<<totObs<<")"<<endl;


      //Calculate F-Test
      //http://en.wikipedia.org/wiki/F-test
      //http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/DiJetHighMass/test/fitcode/F_test_default.C?revision=1.3&view=markup
      const double alphaFTest=0.05;
      logf<<"\nStarting F-Test evaluation with alpha-value="<<alphaFTest<<endl;
      double Ftest_21 = (rss1-rss2)*p2_ndof/ (rss2*1.0);
      double Ftest_32 = (rss2-rss3)*p3_ndof/ (rss3*1.0);
      logf << "Ftest 21 value = " << Ftest_21 << endl;
      logf << "Ftest 32 value = " << Ftest_32 << endl;
      double good_CL21 =  1.-TMath::FDistI(Ftest_21,1,p2_ndof);
      double good_CL32 =  1.-TMath::FDistI(Ftest_32,1,p3_ndof);
      logf << " Ftest21 CL = " << good_CL21 << endl;
      logf << " Ftest32 CL = " << good_CL32 << endl;
      if (good_CL21>alphaFTest) {
	logf << " No need for anything more than a simple exponential" << endl;
      } else if (good_CL32>alphaFTest) {
	logf << "A levelled exponential with 2 parameters is needed for these data. " << endl;
      } else {
	logf << " A levelled exponential with 3 parameters is needed for these data. " << endl;
	logf << " You should check for higher-order functions. " << endl;
      }
      logf << endl;


      ///Plots
      std::cout<<"Drawing"<<std::endl;

      TCanvas *can1=new TCanvas("canvasFTest", "canvas F-Test",800,800);
      can1->cd();
      RooPlot *xf=x->frame();
      string frameTitle="F-Test validation ("+ssnxj.str()+"J "+pur_str +" - "+leptType_str+" lept flav)";
      xf->SetTitle(frameTitle.c_str());
      SBdset->plotOn(xf,RooFit::Binning(RooBinning(79,480,2850)),RooFit::MarkerStyle(21),RooFit::MarkerColor(kRed));
      fit_1par->plotOn(xf, RooFit::Normalization(normFitRange,RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), RooFit::LineColor(kBlue), RooFit::LineStyle(kDotted));//RooFit::NormRange("fitRange"),
      fit_2par->plotOn(xf, RooFit::Normalization(normFitRange,RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), RooFit::LineColor(kOrange+5), RooFit::LineStyle(kDashed));//RooFit::NormRange("fitRange"),
      fit_3par->plotOn(xf, RooFit::Normalization(normFitRange,RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), RooFit::LineColor(kBlack), RooFit::LineStyle(kSolid));//RooFit::NormRange("fitRange"),


      xf->Draw();
      can1->SaveAs((outDir+"/FTestPlot_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType_str+".root").c_str());
      can1->SaveAs((outDir+"/FTestPlot_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType_str+".eps").c_str());
      can1->SaveAs((outDir+"/FTestPlot_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType_str+".pdf").c_str());
      xf->SetMinimum(0.06);
      gPad->SetLogy();
      xf->Draw();
      can1->SaveAs((outDir+"/FTestPlot_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType_str+"_log.root").c_str());
      can1->SaveAs((outDir+"/FTestPlot_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType_str+"_log.eps").c_str());
      can1->SaveAs((outDir+"/FTestPlot_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType_str+"_log.pdf").c_str());
      delete xf; 

      delete fit_1par;    delete fit_2par;    delete fit_3par;

    }//ned loop on purities
  }//end loop on nXjets

  logf.close();
  delete fout;
  return 0;
}//end main

double get_signalParameter(int cat, double massH, std::string varname) {

  int masses[6] = {300,500,600,700,800,1000};

  RooRealVar var(varname.c_str(),varname.c_str(),0.);
  RooArgSet paramsup, paramslow;

  paramsup.add(var);
  paramslow.add(var);

  char filename[200];

  //which files to read?
  for(int i =0 ; i <6 ; i++){
    if(masses[i]==massH){//direct Match
      sprintf(filename,"signalFitResults/out-%d-%s-cat%d.config",masses[i],"EM",cat);
      paramsup.readFromFile(filename, "READ", "Parameters");
      return var.getVal();
    }
  }

  //no direct match => interpolation
  int indexlow = -1;
  int indexhigh= -1;
  for(int i =0 ; i <6 ; i++){
    if(masses[i]>massH){
      indexhigh=i;
      break;
    }
  }
  for(int i =5 ; i >-1 ; i--){
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

  sprintf(filename,"signalFitResults/out-%d-%s-cat%d.config",masses[indexlow],"EM",cat);
  paramsup.readFromFile(filename, "READ", "Parameters");
  double low = var.getVal();
  sprintf(filename,"signalFitResults/out-%d-%s-cat%d.config",masses[indexhigh],"EM",cat);
  paramsup.readFromFile(filename, "READ", "Parameters");
  double high = var.getVal();
  
  double deltaM = masses[indexhigh] - masses[indexlow];
  
  return (massH-masses[indexlow])/deltaM*(high-low) + low;
  
}//end get_signalParameter

int findMyBin(const int nb, double *bin_array,double val){

  int mybin=-1;
  
  for(int i=0;i<nb;i++){
    if(i<nb-1&&val>=bin_array[i]&&val<bin_array[i+1]){
      mybin=i;
      break;
    }
  }

  return mybin;
}
