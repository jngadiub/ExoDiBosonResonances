#include "SidebandFitter.h"

#include <cstdlib>
#include <fstream>
#include <sstream>
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TFile.h"
#include "TStyle.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TMatrixDSym.h"
#include "TRandom3.h"

#include "RooBinning.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooPlot.h"
#include "RooArgSet.h"
#include "RooGaussian.h"
#include "RooCBShape.h"
#include "RooProdPdf.h"
#include "RooFitResult.h"
#include "RooPolynomial.h"
#include "RooDataSet.h"
#include "RooWorkspace.h"
#include "RooDataHist.h"
#include "RooChi2Var.h"
#include "RooMinuit.h"
#include "RooExponential.h"
//#include "RooEllipse.h"
//#include "RooLevelledExp.h"
#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"


//#include "PdfDiagonalizer.h"
#include <algorithm>

#include "TMath.h"


using namespace RooFit;

/*****************
 *
 * All configurations now are in the header file fitSidebandsConfig_XZZ.h and binningFits_XZZ.h
 *
 *****************/


SidebandFitter::SidebandFitter(const std::string& PUType ) {

  wType_ = PUType;

  mZZmin_ = bins1[0];
  mZZmax_ = 1250.0;

  rangecut_ = 480.0;// mZZmax_;//600

  CMS_hzz2l2q_mZZ_ = new RooRealVar("CMS_exovv_mVV", "m_{VV}", mZZmin_, mZZmax_, "GeV");
  fitfuncName_="alpha_fitfunc";
  canvas_label_="";

}





RooWorkspace* SidebandFitter::getAlphaFit(TTree* treeMC, int nxjCategory, const std::string& leptType_str,  int purCut, bool withRooFit ) {
  std::cout<<"Inside SidebandFitter::getAlphaFit"<<std::endl;

  //  std::string outdir = get_outdir();
  std::string mkdir_command = "mkdir -p " + outdir_;
  system(mkdir_command.c_str());

  std::string leptType_text;
  if( leptType_str=="ELE" ) leptType_text = "_ELE";
  else if( leptType_str=="MU" ) leptType_text = "_MU";
  else if( leptType_str=="ALL" ) leptType_text = "";
  else {
    std::cout << "UNKNOWN LEPT TYPE: " << leptType_str << ". EXITING." << std::endl;
    exit(1111);
  }

  std::string pur_str="";
  if(purCut==0)pur_str="LP";
  if(purCut==1)pur_str="HP";

  double mZZd;
  double eventWeight;
  int mynxj; //double mynxj;
  double mZqq;double region;
  double leptType; double vTagPur;
  cout<<"SidebandFitter::getAlphaFit addressing branches"<<endl;
  //treeMC->SetBranchAddress("mZZ",&mZZ);
  treeMC->SetBranchAddress("mZZ",&mZZd);
  treeMC->SetBranchAddress(wType_.c_str(),&eventWeight);
  treeMC->SetBranchAddress("nXjets",&mynxj);
  treeMC->SetBranchAddress("mJJ",&mZqq);
  treeMC->SetBranchAddress("lep",&leptType);
  treeMC->SetBranchAddress("region",&region);
  treeMC->SetBranchAddress("vTagPurity",&vTagPur);
  
  int nb;
  const double* binpointer;
  
  if(nxjCategory==2){
    nb = nBins2-1;
    binpointer = bins2;
  }
  else if(nxjCategory==1){
    nb = nBins1-1;
    binpointer = bins1;
  }
  else{
    std::cout<<"ERROR in SidebandFitter::getAlphaFit ! WRONG nxjCategory = "<<   nxjCategory<<std::endl; 
  }

 
  mZZmax_=binpointer[nb];
  CMS_hzz2l2q_mZZ_->setMax(mZZmax_);

  TH1D h1_mZZ_signalRegion("mX_signalRegion", "",nb ,binpointer );
  h1_mZZ_signalRegion.Sumw2();
  TH1D h1_mZZ_sidebands("mX_sidebands", "", nb,binpointer );
  h1_mZZ_sidebands.Sumw2();
  //save additionaly the unweighted histos to evaluate Poisson-Errors later
  TH1D h1_mZZ_signalRegion_nw("mX_signalRegion_noWeight", "",nb ,binpointer ); 
  h1_mZZ_signalRegion_nw.Sumw2();
  TH1D h1_mZZ_sidebands_nw("mX_sidebands_noWeight", "", nb,binpointer );
  h1_mZZ_sidebands_nw.Sumw2();
  TH1D h1_jj("mJJ", "",20 , 50, 150 );
  

  //
  cout<<"SidebandFitter::getAlphaFit filling histograms "<<treeMC->GetEntries()<<endl;
  for( unsigned int iEntry=0; iEntry<treeMC->GetEntries(); ++iEntry ) {
    treeMC->GetEntry(iEntry);
    // eventWeight=1.0;
   
    if( leptType_str=="MU" && leptType!=1 ) continue;
    if( leptType_str=="ELE" && leptType!=0 ) continue;
    if( mynxj!=nxjCategory ) continue;
    if( vTagPur!=purCut && purCut>=0)continue;
    if( mZZd>mZZmax_ || mZZd < mZZmin_ ) continue;
  
    bool isSignalRegion = (region==1.0);
    if( isSignalRegion ) h1_mZZ_signalRegion.Fill(mZZd, eventWeight);
    if( isSignalRegion ) h1_mZZ_signalRegion_nw.Fill(mZZd);
    if( !isSignalRegion && region==0.0 &&mZqq<70.0) h1_mZZ_sidebands.Fill(mZZd, eventWeight);
    if( !isSignalRegion && region==0.0 &&mZqq<70.0) h1_mZZ_sidebands_nw.Fill(mZZd);
  
    h1_jj.Fill(mZqq, eventWeight);
    //std::cout << "Entry (2): " << iEntry << "/" << treeMC->GetEntries() << std::endl;
  }
  cout<<"SidebandFitter::getAlphaFit histograms filled. Integrals are: "<<h1_mZZ_signalRegion.Integral()<<"  "<<h1_mZZ_sidebands.Integral() <<endl;
  //the alpha ratio used for extrapolating data distribtuion in sideband region
  TH1D *h1_alpha=new TH1D(h1_mZZ_signalRegion);
  h1_alpha->SetName("h_alpha");
  //smoothening procedure for taking care of low-stat MC
  TH1D *h1_alpha_smooth=(TH1D*)DivideAndSmoothAlphaHist(  h1_mZZ_signalRegion, h1_mZZ_sidebands,*h1_alpha);
  char newAlphaname[200];      
  sprintf(newAlphaname,"%s_smoothened",h1_alpha->GetName());
  h1_alpha_smooth->SetName(newAlphaname);
 
  h1_alpha->SetMinimum(0);
  h1_alpha->SetMaximum((nxjCategory==2?2.25 : 2.75));
  h1_alpha_smooth->SetMinimum(0);
  h1_alpha_smooth->SetMaximum((nxjCategory==2?2.25 : 2.75));
  gStyle->SetOptStat(0);
	
  std::vector<double> myFitPars;
  std::vector<double> myFitErrs;
  alphaFit(h1_alpha_smooth, myFitPars,myFitErrs);
  TF1 *fpol0 =(TF1*)h1_alpha_smooth->GetFunction(fitfuncName_.c_str());//name of tf1 to be matched with what is used in SidebandFitter::alphaFit
  TF1 *fpolH =(TF1*)h1_alpha_smooth->GetFunction((fitfuncName_+"_HighRange").c_str());
  char canvasName[400];
  TCanvas* c1 = new TCanvas("c1", "can_fit_hist", 600, 600);
  c1->Divide(1,3);
  c1->cd(1);
  c1->cd(1)->SetLogy();
  h1_mZZ_signalRegion.SetMarkerStyle(7);
  h1_mZZ_signalRegion.SetMarkerColor(kBlue);
  h1_mZZ_signalRegion.Draw();
  c1->cd(2);
  c1->cd(2)->SetLogy();
  h1_mZZ_sidebands.SetMarkerStyle(7);
  h1_mZZ_sidebands.SetMarkerColor(kRed);
  h1_mZZ_sidebands.Draw();
  c1->cd(3);
  h1_alpha_smooth->SetMarkerStyle(20);
  h1_alpha->SetMarkerStyle(20);
  h1_alpha->SetMarkerColor(kGreen);
  // gStyle->SetOptFit(1111);
  h1_alpha->Draw("F");
  h1_alpha_smooth->Draw("Fsames");
  fpol0->SetLineColor(kMagenta);
  //  fpol0->Draw("Lsames");
  fpolH->SetLineColor(kBlue-9);
  //  fpolH->Draw("Lsames");
  TLegend *l1=new TLegend(0.18,0.86,0.45,0.99);
  l1->AddEntry(h1_alpha,"Original","P");
  l1->AddEntry(h1_alpha_smooth,"Smoothened","P");
  l1->Draw();
  sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s.eps", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
  c1->SaveAs(canvasName);
  sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s.png", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
  sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s.root", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
  c1->SaveAs(canvasName);

  TCanvas* c1sig = new TCanvas("c1Sig", "can_fit_hist_SigReg", 600, 600);
  c1sig->cd();
  h1_mZZ_signalRegion.SetMarkerStyle(20);
  h1_mZZ_signalRegion.SetMarkerColor(kBlue);
  h1_mZZ_signalRegion.Draw();
  sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s_SIGONLY.root", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
  c1sig->SaveAs(canvasName);
  TCanvas* c1sb = new TCanvas("c1SB", "can_fit_hist_SBReg", 600, 600);
  c1sb->cd();
  h1_mZZ_sidebands.SetMarkerStyle(21);
  h1_mZZ_sidebands.SetMarkerColor(kRed);
  h1_mZZ_sidebands.Draw();
  sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s_SBONLY.root", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
  c1sb->SaveAs(canvasName);

  std::cout<<" PART1 of SidebandFitter::getAlphaFit is FINISHED !\n\n"<<std::endl;

  //////////////////////////////////////////////
  ///// NOW WE RE-DO the THING WITH ROOFIT IN A DIFFERENT WAY
  ///// (not needed, just for check) :
  ///// fit the MC histos in sig and sb region, make ratios of fits,
  ///// use this analytical ratio as scaling factor for extrapolation 

  //fill a RooDataHist from the TH1D; the errors will be the proper ones
  double minMZZ=bins1[0];
  rangecut_=binpointer[nb];
  //  RooRealVar *mZZ = new RooRealVar("mZZ", "m_{ZZ}", mZZmin_, mZZmax_, "GeV");
  RooRealVar mZZ("mZZ","mZZ",minMZZ, bins1[nBins1-1]);//range to be synchronized with array of histo
  mZZ.setRange("fullRange",minMZZ,bins1[nBins1-1]);
  // mZZ.setRange("fitRange",minMZZ,rangecut_);
  mZZ.setRange("fitRange",startFit,rangecut_);//600 for zz and 800
  // mZZ.setBins(1000);

  RooDataHist *dhAlpha=new RooDataHist("DataHistAlpha","DataHist Alpha vs MZZ",mZZ,Import(*h1_alpha)) ;
  //define the model, i.e. the fit function we want to use (pol0)
  RooRealVar c0("c0","c0_poly",myFitPars.at(0),-5.0,5.0);
  c0.setConstant(kTRUE);
  RooPolynomial *pol0_fit=new RooPolynomial("pol0_func","Polynomial 0th (constant) for fit",mZZ,c0,0);
  ///////////////////////////////

  if(withRooFit){
    //now we try to do the same with RooFit...
    //other vars on which one cuts
    std::cout<<"From SidebadFitter : doing the game with RooFit"<<std::endl;
    RooRealVar *nXjets=new RooRealVar("nXjets","nXjets",-0.1,2.1);
    RooRealVar *mJJ=new RooRealVar("mJJ","mJJ",50.0,150.0);
    RooRealVar *lep=new RooRealVar("lep","lep",0.0,1.0);
    RooRealVar *region=new RooRealVar("region","region",0.0,1.0);
    RooRealVar *vTagPurity=new RooRealVar("vTagPurity","vTagPurity",-5.0,5.0);
    RooRealVar *weight=new RooRealVar("weight","weight",0.0,10.0);
    stringstream strmcut;
    strmcut<<minMZZ;
    stringstream ssnxj;
    ssnxj<<nxjCategory;
    stringstream strpurcut;
    strpurcut<<purCut;
    std::string vtagcutstr;
    if(purCut<0)vtagcutstr="";
    else vtagcutstr=" &&vTagPurity=="+strpurcut.str();

    string cutSB="nXjets=="+ssnxj.str()+vtagcutstr+" &&region==0.0  &&mZZ>"+strmcut.str();
    string cutSIG="nXjets=="+ssnxj.str()+vtagcutstr+" && region==1.0 &&mZZ>"+ strmcut.str();
    
   
    
    RooDataSet *mcSigDSet=new RooDataSet("dsMCSig","dMCSig",(TTree*)treeMC,RooArgSet(mZZ,*nXjets,*mJJ,*lep,*region,*vTagPurity,*weight),cutSIG.c_str(),"weight");
    RooDataSet *mcSBDSet=new RooDataSet("dsMCSB","dMCSB",(TTree*)treeMC,RooArgSet(mZZ,*nXjets,*mJJ,*lep,*region,*vTagPurity,*weight),cutSB.c_str(),"weight");
    // ------------------------ fit with a single exponential ------------------------------
    RooRealVar *slope_SIG = new RooRealVar("slopeSIG","exponential slope (SIGNAL)",-0.1,-5.0,0.0);
    RooExponential *bkgd_fit_SIG = new RooExponential("background_SIG","background_SIG",mZZ,*slope_SIG);

    //for levelled exponential
    RooRealVar *f0_SIG=new RooRealVar("f0_SIG","sigma_SIG",30,0.0,300.0);
    RooRealVar *f1_SIG=new RooRealVar("f1_SIG","alpha_SIG",0.0,-0.5,2.0);
    RooRealVar *f2_SIG=new RooRealVar("f2_SIG","m_SIG",480,200.0,500.0);
    RooRealVar *f3_SIG=new RooRealVar("f3_SIG","theta_SIG",0.0);
    f2_SIG->setConstant(kTRUE);
    f3_SIG->setConstant(kTRUE);
    //f1_SIG->setConstant(kTRUE);//if f1 set constant to zero, levelled expo becomes a normal exponential
    RooLevelledExp *expLev_fit_SIG=new RooLevelledExp("levelled_exp_SIG","levelled_exp_fit (SIGNAL region)",mZZ,*f0_SIG,*f1_SIG,*f2_SIG,*f3_SIG);


    
    //same for sb
    RooRealVar *slope_SB = new RooRealVar("slopeSB","exponential slope (SIDEBAND)",-0.1,-5.0,0.0);
    RooExponential *bkgd_fit_SB = new RooExponential("background_SB","background_SB",mZZ,*slope_SB);
    
    RooRealVar *f0_SB=new RooRealVar("f0_SB","sigma_SB",30,0.0,300.0);
    RooRealVar *f1_SB=new RooRealVar("f1_SB","alpha_SB",0.0,-0.5,2.0);
    RooRealVar *f1b_SB=new RooRealVar("f1b_SB","beta_SB",0.0,-0.5,2.0);
    RooRealVar *f2_SB=new RooRealVar("f2_SB","m_SB",480,200.0,500.0);
    RooRealVar *f3_SB=new RooRealVar("f3_SB","theta_SB",0.0);
    f2_SB->setConstant(kTRUE);
    f3_SB->setConstant(kTRUE);
    f1_SB->setConstant(kTRUE);//if both f1 and f1b ==0, RooLevelledExp2 -> Simple Expo
    RooLevelledExp2 *expLev_fit_SB=new RooLevelledExp2("levelled_exp_SB","levelled_exp_fit (SB region)",mZZ,*f0_SB,*f1_SB,*f1b_SB,*f2_SB,*f3_SB);


    RooFitResult* res_cb_SIG= bkgd_fit_SIG->fitTo(*mcSigDSet,Save(kTRUE),SumW2Error(kTRUE),Range("fitRange")) ;
    RooFitResult* res_cb_SB = bkgd_fit_SB->fitTo(*mcSBDSet,Save(kTRUE),SumW2Error(kTRUE),Range("fitRange")) ;
    RooFitResult* r_sig_expLev_SIG = expLev_fit_SIG->fitTo(*mcSigDSet,Save(kTRUE),SumW2Error(kTRUE),Range("fitRange")) ;
    RooFitResult* r_sig_expLev_SB = expLev_fit_SB->fitTo(*mcSBDSet,Save(kTRUE),SumW2Error(kTRUE),Range("fitRange")) ;

    cout<<"LevExpo fit in SIGNAL region done: Sigma = "<<f0_SIG->getVal()<<"   Alpha="<<f1_SIG->getVal()<<"   m="<<f2_SIG->getVal()<<"  Theta="<<f3_SIG->getVal()<<std::endl;
    cout<<"LevExpo fit in SB region done: Sigma = "<<f0_SB->getVal()<<"   Alpha="<<f1_SB->getVal()<<"   m="<<f2_SB->getVal()<<"  Theta="<<f3_SB->getVal()<<std::endl;

    //    cout<<"Printing results of leveled fit in signal region:"<<endl;
    // r_sig_expLev_SIG->printMultiline(std::cout,3);
    cout<<endl<<endl;
    TCanvas* c2 = new TCanvas("c2", "can_fit_roofit", 600, 900);
    c2->Divide(1,3);
   
    TCanvas* c2a = new TCanvas("c2a", "can_fit_roofit_SIG", 600, 900);
    TCanvas* c2b = new TCanvas("c2b", "can_fit_roofit_SB", 600, 900);

    RooPlot *xf=mZZ.frame();
    xf->SetTitle("MC - Signal region");
    double minyscale = nxjCategory==2? 0.0000006 : 0.0000006;
    double maxyscale = 0.15;

    RooRealVar *NbkgSIG=new RooRealVar("bkgdNormalizationSIG","Background normalization in fit range (SIG region)",mcSigDSet->reduce(CutRange("fitRange"))->sumEntries(),0.0,10000.0);
    cout<<"Entries in fit range (signal region) = "<<NbkgSIG->getVal()<<endl;
    RooRealVar *NbkgSB=new RooRealVar("bkgdNormalizationSB","Background normalization in fit range (SB region)",mcSBDSet->reduce(CutRange("fitRange"))->sumEntries(),0.0,10000.0);
    cout<<"Entries in fit range (sb region) = "<<NbkgSB->getVal()<<endl;
    //double integralSB=mcSBDSet->createIntegral(*mZZ,RooFit::Range("fitRange"))->getVal();



    mcSigDSet->plotOn(xf,Binning(RooBinning(nBins1-1,bins1)),MarkerStyle(21),MarkerColor(kBlue));
    bkgd_fit_SIG->plotOn(xf, Normalization(NbkgSIG->getVal(),RooAbsPdf::NumEvent), LineColor(kOrange),Range("fitRange"));
    expLev_fit_SIG->plotOn(xf, Normalization(NbkgSIG->getVal(),RooAbsPdf::NumEvent), LineColor(kGreen),LineStyle(kDashed),Range("fitRange"));// Normalization(mcSigDSet->sumEntries(),RooAbsPdf::NumEvent)
    // mcSigDSet->plotOn(xf,Binning(RooBinning(nBins-1,bins1)),MarkerStyle(21),MarkerColor(kBlue));
    c2a->cd();
    xf->SetMinimum(0.0);
    xf->SetMaximum((nxjCategory==2?0.5:  0.15) );
    xf->Draw();

    c2->cd(1);
    c2->cd(1)->Clear();
    gPad->SetLogy();
    xf->SetMinimum(minyscale);
    xf->SetMaximum(maxyscale);
    xf->Draw();
    
    
    RooPlot *xf2=mZZ.frame();
    xf2->SetTitle("MC - Sideband region");
    mcSBDSet->plotOn(xf2,Binning(RooBinning(nBins1-1,bins1)),MarkerStyle(21),MarkerColor(kRed));
    bkgd_fit_SB->plotOn(xf2, Normalization(NbkgSB->getVal(),RooAbsPdf::NumEvent), LineColor(kOrange));//,RooAbsPdf::NumEvent
    expLev_fit_SB->plotOn(xf2, Normalization(NbkgSB->getVal(),RooAbsPdf::NumEvent), LineColor(kGreen),LineStyle(kDashed),Range("fitRange"));
    // mcSBDSet->plotOn(xf2,Binning(RooBinning(nBins-1,bins1)),MarkerStyle(21),MarkerColor(kRed));
    c2b->cd();
    xf2->SetMinimum(0.0);
    xf2->SetMaximum((nxjCategory==2?0.5:0.15) );
    xf2->Draw();


    xf2->SetMinimum(minyscale);
    xf2->SetMaximum(maxyscale);
    c2->cd(2);
    gPad->SetLogy();
    xf2->Draw();
    c2->cd(3);
    h1_alpha_smooth->SetLineColor(kBlack);
    TF1 *fitPoly6 = new TF1("fitPolyRooFit", "pol6", 600.0, rangecut_);
    fitPoly6->SetLineColor(kGreen+3);
    fitPoly6->SetLineStyle(kDashed);
    h1_alpha_smooth->Fit(fitPoly6,"QRS+");


    double alpha_fit_SIG=slope_SIG->getVal();
    double alpha_fit_SB=slope_SB->getVal();
    double alpha_fit_ratio=alpha_fit_SIG/alpha_fit_SB;
    double alpha_hist_int=h1_alpha_smooth->Integral(h1_alpha_smooth->FindBin(startFit),h1_alpha_smooth->FindBin(rangecut_));
    cout<<"Ratio of exponential fits: SIG_slope="<<alpha_fit_SIG<<"  SB_slope="<<alpha_fit_SB<<"  Sig/SB="<<alpha_fit_ratio<<"  Integral of ratio in ["<<startFit<<","<<rangecut_<<"]="<<alpha_hist_int <<endl;
    /*
      double tmpintegral=0.0;
      for(int tmpbin=1;tmpbin<=h1_alpha_smooth->GetNbinsX();tmpbin++){
      double tmpcont=h1_alpha_smooth->GetBinContent(tmpbin);
      if(h1_alpha_smooth->GetBinCenter(tmpbin)<600.0)continue;
      tmpintegral+=tmpcont;
      }
      cout<<"Same Integral calculated by hand: "<<tmpintegral<<endl;
    */

    TF1 *fitExpoRatio=new TF1("ratio_fit_expo","[0]*expo(1)",startFit, rangecut_);
    fitExpoRatio->FixParameter(1,0.0);
    fitExpoRatio->FixParameter(2,(alpha_fit_SIG-alpha_fit_SB));
    fitExpoRatio->SetLineColor(kMagenta);
    h1_alpha_smooth->Fit(fitExpoRatio,"QRN");   

    h1_alpha_smooth->SetStats(0);
    h1_alpha_smooth->Draw("F");

    RooRealVar *f0_SBinv=new RooRealVar("f0_SBinv","sigma_SB",-1.0*f0_SB->getVal(),0.0,300.0);
    RooRealVar *f1_SBinv=new RooRealVar("f1_SBinv","alpha_SB",-1.0*f1_SB->getVal(),-0.5,2.0);
    RooRealVar *f1b_SBinv=new RooRealVar("f1b_SBinv","beta_SB",-1.0*f1b_SB->getVal(),-0.5,2.0);
    f0_SBinv->setConstant(kTRUE);
    f1_SBinv->setConstant(kTRUE);
    f1b_SBinv->setConstant(kTRUE);
    RooLevelledExp2 *expLev_fit_SBinv=new RooLevelledExp2("levelled_exp_SBinv","inverse levelled_exp_fit (SB region)",mZZ,*f0_SBinv,*f1_SBinv,*f1b_SBinv,*f2_SB,*f3_SB);
    RooPlot *xfRatio=mZZ.frame();
    RooProdPdf *ratioFit2=new RooProdPdf("ratioFitV2","ratio of Expo_SIG / levExpo_SB",*bkgd_fit_SIG,*expLev_fit_SBinv);
    // double tmpRatioInt=ratioFit2->createIntegral(mZZ,RooFit::Range("fitRange"))->getVal();
    // cout<<"TMP RATIO INT ="<<tmpRatioInt<<endl;
    // double corrIntegral=alpha_hist_int/tmpRatioInt;
    //
    // ratioFit2->plotOn(xfRatio, RooFit::Normalization(1.1*alpha_hist_int,RooAbsPdf::NumEvent), RooFit::LineColor(kRed),RooFit::NormRange("fitRange"),RooFit::Range("fitRange"));
    //xfRatio->SetMinimum(h1_alpha_smooth->GetMinimum());
    // xfRatio->SetMaximum(h1_alpha_smooth->GetMaximum());
    //xfRatio->Draw("same");

    /*
    //draw ratio of functions fitted above 
    TH1D *h1_funcRatio=new TH1D("h_alphaRatioMCFit","Ratio of fits to MC",nBins1*20,bins1[0],bins1[nBins1-1]);
    h1_funcRatio->SetLineColor(kViolet);
    h1_funcRatio->SetMarkerColor(kViolet);
    h1_funcRatio->SetMarkerStyle(20);
    double mcSigNorm=NbkgSIG->getVal();//mcSigDSet->sumEntries();
    double mcSBNorm=NbkgSB->getVal();//mcSBDSet->sumEntries();
    double rNorm=mcSigNorm/mcSBNorm;
    
    cout<<"\n\n\n\nNORMALIZATIONS "<<nxjCategory<<"-jets : SIG="<<mcSigNorm<<" ("<<h1_mZZ_signalRegion.GetEntries()<<")      SB="<< mcSBNorm<<" ("<<h1_mZZ_sidebands.GetEntries()<<")   Ratio="<<rNorm<<std::endl<<std::endl<<std::endl<<std::endl;
    
    for(int ib=1;ib<=h1_funcRatio->GetNbinsX();ib++){
    mZZ.setVal(h1_funcRatio->GetBinCenter(ib));   
    double sigval=bkgd_fit_SIG->getVal();
    double sbval=bkgd_fit_SB->getVal();
    h1_funcRatio->SetBinContent(ib,rNorm*(sigval/sbval) );
    }
    h1_funcRatio->Draw("sames");
    */


    sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s_ROOFIT.eps", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
    c2->SaveAs(canvasName);
    sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s_ROOFIT_SIGONLY.eps", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
    c2a->SaveAs(canvasName);
    sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s_ROOFITSBONLY.eps", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
    c2b->SaveAs(canvasName);
    sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s_ROOFIT.png", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
    c2->SaveAs(canvasName);
    sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s_ROOFIT_SIGONLY.png", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
    c2a->SaveAs(canvasName);
    sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s_ROOFITSBONLY.png", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
    c2b->SaveAs(canvasName);
    sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s_ROOFIT.root", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
    c2->SaveAs(canvasName);
    sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s_ROOFIT_SIGONLY.root", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
    c2a->SaveAs(canvasName);
    sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s_ROOFITSBONLY.root", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
    c2b->SaveAs(canvasName);

    sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s_ROOFIT.pdf", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
    c2->SaveAs(canvasName);
    sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s_ROOFIT_SIGONLY.pdf", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
    c2a->SaveAs(canvasName);
    sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s%s_ROOFITSBONLY.pdf", outdir_.c_str(), nxjCategory,pur_str.c_str(), leptType_str.c_str(),canvas_label_.c_str());
    c2b->SaveAs(canvasName);
 

    delete c2;    delete c2a;    delete c2b;

    delete bkgd_fit_SIG; delete expLev_fit_SIG; 
    delete bkgd_fit_SB; delete expLev_fit_SB;delete expLev_fit_SBinv;delete ratioFit2;
    delete xf2; delete xfRatio;

    delete mcSBDSet;
    delete mcSigDSet;
  }//end if with roofit
  cout<<"\n\nContinuing..."<<std::endl;
  

  //create a RooWorkspace and save there:
  //- the RooDataHist with alpha
  //- the fit result
  char ws_name[200];
  sprintf(ws_name,"ws_alpha_%dJ_%s_%s",nxjCategory,pur_str.c_str(), leptType_str.c_str());
  RooWorkspace* alphaws=new RooWorkspace(ws_name,ws_name);
  alphaws->import(*dhAlpha);
  alphaws->import(*pol0_fit);
 
  //char outFileName[400];
  //sprintf( outFileName,"%s/Workspaces_alpha_%dJ_%s.root",outdir_.c_str(), nxjCategory, leptType_str.c_str());
  TFile* outFile = new TFile(outfile_.c_str(),"RECREATE");
  outFile->cd();
  alphaws->Write();
  h1_mZZ_signalRegion.Write();
  h1_mZZ_sidebands.Write();
  h1_mZZ_signalRegion_nw.Write();
  h1_mZZ_sidebands_nw.Write();
  h1_jj.Write();
  h1_alpha->Write();
  h1_alpha_smooth->Write();
  fpol0->Write();
  // outFile->Close();
  delete outFile;

  delete h1_alpha; delete h1_alpha_smooth;
  std::cout<<"Finishing getAlphaFit for nXjets=="<<nxjCategory<<"  VTagPurity=="<<purCut<<std::endl;
  return alphaws;
}


std::string SidebandFitter::get_outdir() {

  std::string returnString = "FitSidebands";

  return returnString;

}


void SidebandFitter::setOutDir(string new_outdir){
  outdir_=new_outdir;
}

void SidebandFitter::setOutFile(string new_outfile){
  outfile_=new_outfile;
}

void SidebandFitter::setCanvasLabel(string new_label){
  canvas_label_=new_label;
}


void SidebandFitter::alphaFit( TH1D* alpha_hist , std::vector<double> & fitpars,std::vector<double> & fiterrs){
  fitpars.clear();
  fiterrs.clear();
  TF1 *fpol0 = new TF1(fitfuncName_.c_str(), "[0]+[1]*TMath::Power(x,[2])-[3]*x", bins1[0], bins1[nBins1-1]);
  TF1 *fpolLow = new TF1((fitfuncName_+"_LowRange").c_str(), "[0]+pow(x,[1])", bins1[0], rangecut_);
  fpolLow->SetLineColor(kTeal);
  TF1 *fpolHigh = new TF1((fitfuncName_+"_HighRange").c_str(), "pol1",rangecut_ , bins1[nBins1-1]);
  fpolHigh->SetLineColor(kTeal);
  TFitResultPtr fitResHist =alpha_hist->Fit(fitfuncName_.c_str(),"Q0S+");
  TFitResultPtr fitResHistLow =alpha_hist->Fit((fitfuncName_+"_LowRange").c_str(),"Q0RS+");
  TFitResultPtr fitResHistHigh =alpha_hist->Fit((fitfuncName_+"_HighRange").c_str(),"Q0RS+");
  int nPars=fpol0->GetNumberFreeParameters();
  fitpars.reserve(nPars);
  fiterrs.reserve(nPars);
  for(int ip=0;ip<nPars;ip++){
    fitpars.push_back(fpol0->GetParameter(ip));
    
    double newerr=fabs(fpolLow->GetParameter(ip)- fpolHigh->GetParameter(ip));
    //    fiterrs.push_back(fpol0->GetParError(ip));
    fiterrs.push_back(newerr);
  }


  //  cout<<"\n\n~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\nFollowing are fit results from fit to TH1 ("<<nxjCategory <<" jet): "<<endl;
  // ///////fitResHist->Print("V");
  //cout<<"P0="<<fpol0->GetParameter(0)<<" +/- "<< fpol0->GetParError(0) <<"   Chi2="<<fpol0->GetChisquare()<<"  NDOF="<<fpol0->GetNDF()<<endl;
  //cout<<endl<<endl;



}

TH1D* SidebandFitter::shuffle( TH1D* inhist, TRandom3* random, char *histName, TH1D* inhist2) {

  TH1D* outhist = (TH1D*) inhist->Clone(histName);
  //TF1 *ftmp=(TF1*)outhist->GetFunction((fitfuncName_+"_LowRange").c_str());
  for(int i=1 ; i <= outhist->GetNbinsX() ; i++) {

    float val = outhist->GetBinContent(i);
    float err; 
    //comment by AB: honestly, I don't remember why we were doing such a thing...
    if (inhist2) err=sqrt(inhist2->GetBinContent(i) *inhist2->GetBinContent(i) +outhist->GetBinError(i)*outhist->GetBinError(i)); 
    else err=outhist->GetBinError(i);//comment by AB: this is what I would have expected to do
    //float valfit=ftmp->Eval(outhist->GetBinCenter(i));
    //float errfit =TMath::Max(float(fabs(val-valfit)),float(inhist->GetBinError(i))) ;  //outhist->GetBinError(i);

    //use val/err for shuffling around actual alpha hist, valfit/errfit for shuffling around the fit

    //if(val==0. || err==0.)
    //continue;
    //std::cout << outhist->GetBinCenter(i) << " " << valfit << std::endl;
    outhist->SetBinContent(i,TMath::Max(0.0,random->Gaus(val,err)));

  }
  return outhist;
}


TH1D* SidebandFitter::realpharize( TH1D* signalRegion , TH1D*sidebands,TH1D* signalRegion_nw , TH1D*sidebands_nw,TH1D*R0, TRandom3* random, char *histName){
  TH1D *h1_shuffled_sig  = (TH1D *)signalRegion->Clone("tmp_sig");
  TH1D *h1_shuffled_side = (TH1D *)sidebands->Clone("tmp_side");
  
  for(int nbin =1 ; nbin <= h1_shuffled_sig->GetNbinsX(); nbin++){
    int nsig_orig = signalRegion_nw->GetBinContent(nbin);
    int nside_orig = sidebands_nw->GetBinContent(nbin);
    
    int nsig_new = random->Poisson(nsig_orig);
    int nside_new = random->Poisson(nside_orig);

    if(nsig_orig!=0)
      h1_shuffled_sig->SetBinContent(nbin,signalRegion->GetBinContent(nbin)*((float)nsig_new)/((float)nsig_orig));
    else
      h1_shuffled_sig->SetBinContent(nbin,0);
    
    if(nside_orig!=0)
      h1_shuffled_side->SetBinContent(nbin,sidebands->GetBinContent(nbin)*((float)nside_new)/((float)nside_orig));
    else
      h1_shuffled_side->SetBinContent(nbin,0);
  }
  
  TH1D *h1_alpha = (TH1D*)signalRegion->Clone("alpha_tmp"); h1_alpha->Reset();
  TH1D *h1_alpha_smooth = (TH1D*)DivideAndSmoothAlphaHist( *h1_shuffled_sig , *h1_shuffled_side ,*h1_alpha);
  //TH1D* h1_alpha_smooth = new TH1D(*h1_alpha);
  
  TH1D * alpha_Final = (TH1D*)h1_alpha_smooth->Clone(histName);

  for (int iBin = 1 ; iBin <= alpha_Final->GetNbinsX() ; ++iBin){
    double alpha_ORI_c = h1_alpha_smooth->GetBinContent(iBin);
    double alpha_ORI_e = h1_alpha_smooth->GetBinError(iBin);
    double R0_c = R0->GetBinContent(iBin);
    double R0_e = R0->GetBinError(iBin);
    double alpha_Final_c = (1-R0_c)*alpha_ORI_c;
    double alpha_Final_e = sqrt(alpha_ORI_c*alpha_ORI_c*R0_e*R0_e+(1-R0_c)*(1-R0_c)*alpha_ORI_e*alpha_ORI_e);
    alpha_Final->SetBinContent(iBin,alpha_Final_c);
    alpha_Final->SetBinError(iBin,alpha_Final_e);
  }
  
  delete h1_shuffled_sig;  
  delete h1_shuffled_side; 
  delete h1_alpha;
  delete h1_alpha_smooth;
  
  
  return alpha_Final;
  

}


TH1D* SidebandFitter::dummyAlphaHist( float alpha , TH1D* inhist , char* histName  ) {

  TH1D* outhist = (TH1D*) inhist->Clone(histName);
  outhist->Reset();
  for(int i=1 ; i <= outhist->GetNbinsX() ; i++) {
    outhist->SetBinContent(i,alpha);
  }
  return outhist;
}


TH1D* SidebandFitter::DivideAndSmoothAlphaHist( TH1D hnum, TH1D hden,TH1D &halpha){
  // std::cout<<"Creating alpha ratio"<<std::endl;
  TH1D *hfin=(TH1D*)halpha.Clone();
  char newname[200];
  sprintf(newname,"%s_smoothened",halpha.GetName());
  hfin->Reset();
  hfin->SetName(newname);

  //  cout<<"HDEN #entries="<<hden.GetEntries()<<"  Bin#1="<<hden.GetBinContent(1) <<"  Integral="<<hden.Integral()<<std::endl;
  //cout<<"HNUM #entries="<<hnum.GetEntries()<<"  Bin#1="<<hnum.GetBinContent(1) <<"  Integral="<<hnum.Integral()<<std::endl;
  TH1D hdencopy(hden);
  char denname[200];
  sprintf(denname,"%s_copy",hden.GetName());
  hdencopy.SetName(denname);
  int smoothRes1=smoothHist(hdencopy,true);
  if(smoothRes1!=0) {
    std::cout<<"ERROR from SidebandFitter::DivideAndSmoothAlphaHist : first bin of DENOMINATOR hist equals zero. Exiting with an empty copy of the input alpha histo."<<std::endl;
    return hfin;
  }

  hnum.Sumw2();
  hden.Sumw2();
  hdencopy.Sumw2();
  halpha.Divide(&hnum,&hdencopy);
  halpha.Sumw2();

  /****
   //for debug
   for(int b=0;b<halpha.GetNbinsX();b++){
   if(halpha.GetBinCenter(b)>800.0 && halpha.GetBinCenter(b)<900.0 ){
   cout<<"\n\nCHECK-SMOOTHED: bin test: b="<<b<<" -> M="<<halpha.GetBinCenter(b)<<endl;
   cout<<"NUM= "<<hnum.GetBinContent(b)<<" +/- "<<hnum.GetBinError(b)<<"  (tot integral="<<hnum.Integral() <<")"<<endl;
   cout<<"DEN= "<<hden.GetBinContent(b)<<" +/- "<<hden.GetBinError(b)<<endl;
   cout<<"DENCOPY= "<<hdencopy.GetBinContent(b)<<" +/- "<<hdencopy.GetBinError(b)<<"  (tot integral="<<hdencopy.Integral() <<")"<<endl;
   cout<<"ALPHA= "<<halpha.GetBinContent(b)<<" +/- "<<halpha.GetBinError(b)<<endl<<endl<<endl;
   } 
   }   
  ***/ 


  hfin=(TH1D*)halpha.Clone();
  
  int smoothRes2=smoothHist(*hfin,false,2);
  if(smoothRes2!=0) {
    std::cout<<"ERROR from SidebandFitter::DivideAndSmoothAlphaHist : first bin of ALPHA hist equals zero. Exiting with an empty copy of the input alpha histo."<<std::endl;
    hfin->Reset();
    return hfin;
  }


  /******
   //ok, now we have a ratio that has a value for all bins.
   //some bins might be at zero or be outliers. Smooth them.
   const  int nbins=hfin->GetNbinsX();
   for(int b=1;b<=nbins;b++){
   double bincont=0.0,bincontM=0.0,bincontP=0.0;
   double binerr=0.0,binerrM=0.0,binerrP=0.0;
    
   bincont=hfin->GetBinContent(b);
   binerr=hfin->GetBinError(b);

   if(b>1){
   bincontM=hfin->GetBinContent(b-1);
   binerrM=hfin->GetBinError(b-1);
   }
   else{
   bincontM=bincont;
   binerrM=binerr;
   }

   if(b<nbins){
   bincontP=hfin->GetBinContent(b+1);
   binerrP=hfin->GetBinError(b+1);
   }
   else{
   bincontP=hfin->GetBinContent(b-2);
   binerrP=hfin->GetBinError(b-2);
   }

   if(b==1&&(bincont==0||binerr==0)){
   std::cout<<"ERROR from SidebandFitter::DivideAndSmoothAlphaHist : first bin of alpha ratio equals zero. Exiting with a copy of the input alpha histo."<<std::endl;  
   return hfin;
   }

   double avgRef=(bincontM+bincontP) / 2.0;
   double errRef=sqrt(binerrM*binerrM + binerrP*binerrP);
   double sigma=min(binerr, errRef);
   //smooth if bin is >= 3 sigma away; assign large uncertainty
   if(fabs(bincont-avgRef)/sigma >=3.0  &&bincont!=0){
   bincont=avgRef;
   binerr=errRef;

   hfin->SetBinContent(b,bincont);
   hfin->SetBinError(b,binerr);
   }


   }//end loop on ib -> bins
  ****/




  return hfin;
}



int SidebandFitter::smoothHist(TH1 &h, bool forceCorrZero,int smoothLevel){

  const  int nbins=h.GetNbinsX();
  // std::cout<<"Dump bin content of histo to be smoothed:"<<std::endl;
  //  for(int b=1;b<=nbins;b++){
  //  std::cout<<"b="<<b<<" M="<< h.GetBinCenter(b)<<" -> "<<h.GetBinContent(b)<<std::endl;
  //  }
  for(int b=1;b<=nbins;b++){
    //bincontM: content of bin n-1 ; bincontP: content of bin n+1
    double bincont=0.0,bincontM=0.0,bincontP=0.0;
    double binerr=0.0,binerrM=0.0,binerrP=0.0;
    
    bincont=h.GetBinContent(b);
    binerr=h.GetBinError(b);
    if(b==1&&(bincont==0||binerr==0)){   
      std::cout<<"First bin of "<<h.GetName() <<" with zero val: "<<h.GetBinCenter(b)<<" -> "<<h.GetBinContent(b)<<" +/- "<<h.GetBinError(b)<<endl;
      return 1;
    }

    //fill bincontM and bincotnP
    if(b>1){
      bincontM=h.GetBinContent(b-1);
      binerrM=h.GetBinError(b-1);
    }
    else{
      bincontM=bincont;
      binerrM=binerr;
    }

    if(b<nbins){
      bincontP=h.GetBinContent(b+1);
      binerrP=h.GetBinError(b+1);
    }
  
    //calculate average of last 5 bins (or less if b<5)
    int brun=5;
    if(b<5)brun=b;
    double runSum=0.0;
    for(int b2=b;b2>b-brun;b2--){ runSum+=h.GetBinContent(b2); }
    double runAvg=runSum/brun;

    //if any of the two neighbouring bins is null,
    //try to use the value of the next non-null bin for smoothing.
    //Don't be so aggressive if the switch forceCorrZero is false

    if(!forceCorrZero&&bincont==0 &&bincontM==0&&bincontP==0){
      //bin with zero content surrounded by two zero bins: move on
      continue;
    }
    
    if(bincontM==0){//mmmh, I put this safety but it looks strange...
      int i2=2;
      while(b-i2>0 && bincontM==0){
	bincontM=h.GetBinContent(b-i2);
	binerrM=h.GetBinError(b-i2);
	i2++;
      }
    }
    if(bincontP==0){//mmmh, I put this safety but it looks strange...
      int i2=2;
      if(b+i2<nbins){
	
	while(b+i2<nbins && bincontP==0){
	  bincontP=h.GetBinContent(b+i2);
	  binerrP=h.GetBinError(b+i2);
	  i2++;
	}
      }
      else{
	bincontP=h.GetBinContent(b-2);
	binerrP=h.GetBinError(b-2);
      }
    }


    //do not average with some bin clearly out of the trend
    if((bincontP>3.0*runAvg) && (binerrP/bincontP>0.50) ){
      int i2=2;
      std::cout<<"Fixing outlier binPlus "<<bincontP<<" >> "<<runAvg<<endl;
      while( (bincontP>3.0*runAvg) && (binerrP/bincontP>0.50)){
	int binNEW=b+i2;
	if(binNEW>nbins){
	  binNEW=nbins;
	  bincontP=runAvg;
	  binerrP=runAvg*2.0;
	  break;
	}
	else{
	  bincontP=h.GetBinContent(binNEW);
	  binerrP=h.GetBinError(binNEW);
	  i2++;
	}
	 	
      }//end while bincontP>3.0*runAvg
    }//end if bincontP>3.0*runAvg
    
    //well, in the end we do an average
    double avgRef=(bincontP+bincontM) / 2.0;
    double errRef=sqrt(binerrM*binerrM + binerrP*binerrP);
    double sigma=min(binerr, errRef);


    // bin with zero entries
    if(bincont==0 &&b<nbins ){
    
      cout<<"Changing bin to "<<h.GetName() <<" ! b: "<<b<<"  New Val="<<avgRef<<" +/- "<<errRef<<endl;
      h.SetBinContent(b,avgRef);
      h.SetBinError(b,errRef);
    }//end if bin has zero entries
    else if(smoothLevel>1){//relatively normal bin, correct if it is too far from neighbours

      if(!forceCorrZero &&(bincontM==0||bincontP==0)){
	//bin with zero content surrounded by some zero bins:skip
	continue;
      }

      //smooth if bin is >= 3 sigma away; assign large uncertainty
      if(fabs(bincont-avgRef)/sigma >=3.0  &&bincont!=0){
	bincont=avgRef;
	binerr=errRef;
	cout<<"Changing bin to "<<h.GetName() <<" ! b: "<<b<<"  New Val="<<avgRef<<" +/- "<<errRef<<endl;
	h.SetBinContent(b,bincont);
	h.SetBinError(b,binerr);
      }
      
    }//end if(smoothLevel>1)

    

  }//end loop on ib -> bins

  return 0;
}//end smooth hist


