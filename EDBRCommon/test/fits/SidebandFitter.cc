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
//#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"


//#include "PdfDiagonalizer.h"
#include <algorithm>

#include "TMath.h"


using namespace RooFit;

//const int nBins=17;
//const  float bins0[nBins]={270,290,320,340,360,380,400,430,460,490,520,550,600,650,700,800,1000};



//binning for merged Jet topology 
const int nBins1=17;
const double bins1[nBins1]={480,500,520,560,600,640,680,720,760,800,840,920,
			    1000,1100,1250,1500,1750};

//binning for double Jet topology 
const int nBins2=17;
const double bins2[nBins2]={480,500,520,560,600,640,680,720,760,800,840,920,
			    1000,1100,1250,1500,1750};



SidebandFitter::SidebandFitter(const std::string& PUType ) {

  wType_ = PUType;

  mZZmin_ = bins1[0];
  mZZmax_ = 1250.0;

  rangecut_ = 570.0;// mZZmax_;//600

  CMS_hzz2l2q_mZZ_ = new RooRealVar("CMS_exovv_mVV", "m_{VV}", mZZmin_, mZZmax_, "GeV");
  fitfuncName_="alpha_fitfunc";
  canvas_label_="";
}





RooWorkspace* SidebandFitter::getAlphaFit( int nxjCategory, const std::string& leptType_str, TTree* treeMC, bool withRooFit ) {
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

  double mZZd;
  double eventWeight;
  int mynxj; //double mynxj;
  double mZqq;double region;
  double leptType;
  cout<<"SidebandFitter::getAlphaFit addressing branches"<<endl;
  //treeMC->SetBranchAddress("mZZ",&mZZ);
  treeMC->SetBranchAddress("mZZ",&mZZd);
  treeMC->SetBranchAddress(wType_.c_str(),&eventWeight);
  treeMC->SetBranchAddress("nXjets",&mynxj);
  treeMC->SetBranchAddress("mJJ",&mZqq);
  treeMC->SetBranchAddress("lep",&leptType);
  treeMC->SetBranchAddress("region",&region);
  
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
  TH1D h1_jj("mJJ", "",20 , 50, 150 );
  

  //
  cout<<"SidebandFitter::getAlphaFit filling histograms "<<treeMC->GetEntries()<<endl;
  for( unsigned int iEntry=0; iEntry<treeMC->GetEntries(); ++iEntry ) {
    treeMC->GetEntry(iEntry);
    // eventWeight=1.0;
   
    if( leptType_str=="MU" && leptType!=1 ) continue;
    if( leptType_str=="ELE" && leptType!=0 ) continue;
    if( mynxj!=nxjCategory ) continue;
    if( mZZd>mZZmax_ || mZZd < mZZmin_ ) continue;
  
    bool isSignalRegion = (region==1.0);
    if( isSignalRegion ) h1_mZZ_signalRegion.Fill(mZZd, eventWeight);
    if( !isSignalRegion && region==0.0) h1_mZZ_sidebands.Fill(mZZd, eventWeight);
    h1_jj.Fill(mZqq, eventWeight);
    //std::cout << "Entry (2): " << iEntry << "/" << treeMC->GetEntries() << std::endl;
  }
  cout<<"SidebandFitter::getAlphaFit histograms filled"<<endl;
  //the alpha ratio used for extrapolating data distribtuion in sideband region
  TH1D *h1_alpha=new TH1D(h1_mZZ_signalRegion);
  h1_alpha->SetName("h_alpha");
  //smoothening procedure for taking care of low-stat MC
  TH1D *h1_alpha_smooth=(TH1D*)DivideAndSmoothAlphaHist(  h1_mZZ_signalRegion, h1_mZZ_sidebands,*h1_alpha);
  char newAlphaname[200];
  sprintf(newAlphaname,"%s_smoothened",h1_alpha->GetName());
  h1_alpha_smooth->SetName(newAlphaname);
 
  h1_alpha->SetMinimum(0);
  h1_alpha->SetMaximum((nxjCategory==2?2.25 : 1.5));
  h1_alpha_smooth->SetMinimum(0);
  h1_alpha_smooth->SetMaximum((nxjCategory==2?2.25 : 1.5));

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
  gStyle->SetOptFit(1111);
  h1_alpha->Draw("F");
  h1_alpha_smooth->Draw("Fsames");
  fpol0->SetLineColor(kMagenta);
  fpol0->Draw("Lsames");
  fpolH->SetLineColor(kBlue-9);
  fpolH->Draw("Lsames");
  TLegend *l1=new TLegend(0.18,0.86,0.45,0.99);
  l1->AddEntry(h1_alpha,"Original","P");
  l1->AddEntry(h1_alpha_smooth,"Smoothened","P");
  l1->Draw();
  sprintf( canvasName, "%s/mZZ_alpha_%dJ_%s%s.eps", outdir_.c_str(), nxjCategory, leptType_str.c_str(),canvas_label_.c_str());
  c1->SaveAs(canvasName);
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
  mZZ.setRange("fitRange",minMZZ,rangecut_);

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
    RooRealVar *nXjets=new RooRealVar("nXjets","nXjets",0,2);
    RooRealVar *mJJ=new RooRealVar("mJJ","mJJ",50.0,150.0);
    RooRealVar *lep=new RooRealVar("lep","lep",0.0,1.0);
    RooRealVar *region=new RooRealVar("region","region",0.0,1.0);
    RooRealVar *weight=new RooRealVar("weight","weight",0.0,10.0);
    stringstream strmcut;
    strmcut<<minMZZ;
    stringstream ssnxj;
    ssnxj<<nxjCategory;
    string cutSB="nXjets=="+ssnxj.str()+" &&region==0.0 &&mZZ>"+strmcut.str();
    string cutSIG="nXjets=="+ssnxj.str()+" && region==1.0 &&mZZ>"+ strmcut.str();
    
   
    
    RooDataSet *mcSigDSet=new RooDataSet("dsMCSig","dMCSig",(TTree*)treeMC,RooArgSet(mZZ,*nXjets,*mJJ,*lep,*region,*weight),cutSIG.c_str(),"weight");
    RooDataSet *mcSBDSet=new RooDataSet("dsMCSB","dMCSB",(TTree*)treeMC,RooArgSet(mZZ,*nXjets,*mJJ,*lep,*region,*weight),cutSB.c_str(),"weight");
    // ------------------------ fit with a single exponential ------------------------------
    RooRealVar *slope_SIG = new RooRealVar("slopeSIG","exponential slope (SIGNAL)",-0.1,-5.0,0.0);
    RooExponential *bkgd_fit_SIG = new RooExponential("background_SIG","background_SIG",mZZ,*slope_SIG);
    
    //same for sb
    RooRealVar *slope_SB = new RooRealVar("slopeSB","exponential slope (SIDEBAND)",-0.1,-5.0,0.0);
    RooExponential *bkgd_fit_SB = new RooExponential("background_SB","background_SB",mZZ,*slope_SB);
    

    RooFitResult* res_cb_SIG= bkgd_fit_SIG->fitTo(*mcSigDSet,Save(kTRUE),SumW2Error(kTRUE),Range("fitRange")) ;//,Range("fitRange")
    RooFitResult* res_cb_SB = bkgd_fit_SB->fitTo(*mcSBDSet,Save(kTRUE),SumW2Error(kTRUE),Range("fitRange")) ;//,Range("fitRange")

    TCanvas* c2 = new TCanvas("c2", "can_fit_roofit", 600, 900);
    c2->Divide(1,3);
   
    TCanvas* c2a = new TCanvas("c2a", "can_fit_roofit_SIG", 600, 900);
    TCanvas* c2b = new TCanvas("c2b", "can_fit_roofit_SB", 600, 900);

    RooPlot *xf=mZZ.frame();
    
    double minyscale = nxjCategory==2? 0.000006 : 0.000006;
    double maxyscale = 0.15;
    mcSigDSet->plotOn(xf,Binning(RooBinning(nBins1-1,bins1)),MarkerStyle(21),MarkerColor(kBlue));
    bkgd_fit_SIG->plotOn(xf, Normalization(mcSigDSet->sumEntries(),RooAbsPdf::NumEvent), LineColor(kOrange),Range("fitRange"));//,RooAbsPdf::NumEvent
    // mcSigDSet->plotOn(xf,Binning(RooBinning(nBins-1,bins1)),MarkerStyle(21),MarkerColor(kBlue));
    c2a->cd();
    xf->SetMinimum(0.0);
    xf->SetMaximum((nxjCategory==2?0.15:  0.15) );
    xf->Draw();

    c2->cd(1);
    c2->cd(1)->Clear();
    gPad->SetLogy();
    xf->SetMinimum(minyscale);
    xf->SetMaximum(maxyscale);
    xf->Draw();
    
    
    RooPlot *xf2=mZZ.frame();
    mcSBDSet->plotOn(xf2,Binning(RooBinning(nBins1-1,bins1)),MarkerStyle(21),MarkerColor(kRed));
    bkgd_fit_SB->plotOn(xf2, Normalization(mcSBDSet->sumEntries(),RooAbsPdf::NumEvent), LineColor(kGreen));//,RooAbsPdf::NumEvent
    // mcSBDSet->plotOn(xf2,Binning(RooBinning(nBins-1,bins1)),MarkerStyle(21),MarkerColor(kRed));
    c2b->cd();
    xf2->SetMinimum(0.0);
    xf2->SetMaximum((nxjCategory==2?0.2:0.1) );
    xf2->Draw();


    xf2->SetMinimum(minyscale);
    xf2->SetMaximum(maxyscale);
    c2->cd(2);
    gPad->SetLogy();
    xf2->Draw();
    c2->cd(3);
    h1_alpha_smooth->Draw("F");
    //draw ratio of functions fitted above 
    TH1D *h1_funcRatio=new TH1D("h_alphaRatioMCFit","Ratio of fits to MC",nBins1*20,bins1[0],bins1[nBins1-1]);
    h1_funcRatio->SetLineColor(kViolet);
    h1_funcRatio->SetMarkerColor(kViolet);
    h1_funcRatio->SetMarkerStyle(20);
    double mcSigNorm=mcSigDSet->sumEntries();
    double mcSBNorm=mcSBDSet->sumEntries();
    double rNorm=mcSigNorm/mcSBNorm;
    
    cout<<"\n\n\n\nNORMALIZATIONS "<<nxjCategory<<"-jets : SIG="<<mcSigNorm<<" ("<<h1_mZZ_signalRegion.GetEntries()<<")      SB="<< mcSBNorm<<" ("<<h1_mZZ_sidebands.GetEntries()<<")   Ratio="<<rNorm<<std::endl<<std::endl<<std::endl<<std::endl;
    
    for(int ib=1;ib<=h1_funcRatio->GetNbinsX();ib++){
      mZZ.setVal(h1_funcRatio->GetBinCenter(ib));   
      double sigval=bkgd_fit_SIG->getVal();
      double sbval=bkgd_fit_SB->getVal();
      h1_funcRatio->SetBinContent(ib,rNorm*(sigval/sbval) );
    }
    h1_funcRatio->Draw("sames");
    sprintf( canvasName, "%s/mZZ_alpha_%dJ_%s%s_ROOFIT.eps", outdir_.c_str(), nxjCategory, leptType_str.c_str(),canvas_label_.c_str());
    c2->SaveAs(canvasName);
    sprintf( canvasName, "%s/mZZ_alpha_%dJ_%s%s_ROOFIT_SIGONLY.eps", outdir_.c_str(), nxjCategory, leptType_str.c_str(),canvas_label_.c_str());
    c2a->SaveAs(canvasName);
    sprintf( canvasName, "%s/mZZ_alpha_%dJ_%s%s_ROOFITSBONLY.eps", outdir_.c_str(), nxjCategory, leptType_str.c_str(),canvas_label_.c_str());
    c2b->SaveAs(canvasName);


    delete c2;    delete c2a;    delete c2b;
    
    delete mcSBDSet; delete bkgd_fit_SB;//delete expLev_fit_SB;
    delete mcSigDSet; delete bkgd_fit_SIG;//delete expLev_fit_SIG;
  }//end if with roofit
  cout<<"\n\nContinuing..."<<std::endl;
  

  //create a RooWorkspace and save there:
  //- the RooDataHist with alpha
  //- the fit result
  char ws_name[200];
  sprintf(ws_name,"ws_alpha_%dJ%s",nxjCategory, leptType_str.c_str());
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
  h1_jj.Write();
  h1_alpha->Write();
  h1_alpha_smooth->Write();
  fpol0->Write();
  // outFile->Close();
  delete outFile;

  delete h1_alpha; delete h1_alpha_smooth;
  std::cout<<"Finishing getAlphaFit for nXjets=="<<nxjCategory<<std::endl;
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
  TF1 *ftmp=(TF1*)outhist->GetFunction((fitfuncName_+"_LowRange").c_str());
  for(int i=1 ; i <= outhist->GetNbinsX() ; i++) {

    float val = outhist->GetBinContent(i);
    float err; 
    if (inhist2) err=sqrt(pow(inhist2->GetBinContent(i),2)+pow(outhist->GetBinError(i),2)); 
    else err=outhist->GetBinError(i);
    float valfit=ftmp->Eval(outhist->GetBinCenter(i));
    float errfit =TMath::Max(float(fabs(val-valfit)),float(inhist->GetBinError(i))) ;  //outhist->GetBinError(i);

    //use val/err for shuffling around actual alpha hist, valfit/errfit for shuffling around the fit

    //if(val==0. || err==0.)
    //continue;
    //std::cout << outhist->GetBinCenter(i) << " " << valfit << std::endl;
    outhist->SetBinContent(i,TMath::Max(0.0,random->Gaus(val,err)));

  }
  return outhist;
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
  std::cout<<"Creating alpha ratio"<<std::endl;
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
  
    //if any of the two neighbouring bins is null,
    //try to use the value of the next non-null bin for smoothing.
    //Don't be so aggressive if the switch forceCorrZero is false

    if(!forceCorrZero&&bincont==0 &&bincontM==0&&bincontP==0){
      //bin with zero content surrounded by two zero bins: move on
      continue;
    }
    
    if(bincontM==0){//mmmh, I put this safety but it looks strange...
      int i2=2;
      while(b-i2>0 || bincontM!=0){
	bincontM=h.GetBinContent(b-i2);
	binerrM=h.GetBinError(b-i2);
	  i2++;
      }
    }
    if(bincontP==0){//mmmh, I put this safety but it looks strange...
      int i2=2;
      if(b+i2<nbins){
	
	while(b+i2<nbins || bincontP!=0){
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


