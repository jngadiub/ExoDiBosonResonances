#include <string>
#include <sstream>
#include <Riostream.h>

#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "TChain.h"
#include "TH1D.h"
#include "TString.h"
#include "TROOT.h"
#include "TF1.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TSystem.h"

#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooPlot.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooProdPdf.h"
#include "RooFitResult.h"
#include "RooPolynomial.h"
#include "RooWorkspace.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooChi2Var.h"
#include "RooMinuit.h"
#include "RooExponential.h"
#include "RooFitResult.h"
#include "RooGenericPdf.h"
#include "RooExtendPdf.h"
#include "RooBinning.h"
#include "RooEllipse.h"
#include "RooAddPdf.h"

#include "PdfDiagonalizer.h"
#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"

using namespace std ;
using namespace RooFit ;

//ROOT::gErrorIgnoreLevel=kWarning;
//gErrorIgnoreLevel = kWarning;


RooPlot* ContourPlot(RooRealVar* var1,RooRealVar* var2, RooFitResult* r);
TTree* weightTree(TTree* tree, TH1D* h1_alpha,const std::string& name ,bool verbose=false);
void fitPseudo( RooDataSet& ModSideband, RooWorkspace& ws,int seed,char* initalvalues, int nxj);
void pseudoMassge(int nxj , RooFitResult* r_nominal, RooWorkspace& ws,char* initalvalues, double NormRelErr, RooRealVar &errV1, RooRealVar &errV2);
void CopyTreeVecToPlain(TTree *t1, std::string wType, std::string f2Name,std::string t2Name,int nxjCut=-1);

const string inDir="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1/thiago20130206/";
const string outDir="FitSidebandsMJJ_TEST1";
const string leptType="ALL";
const unsigned int nToys = 500;

//binning for merged Jet topology 
const int nBins1=17;
const double bins1[nBins1]={480,500,520,560,600,640,680,720,760,800,840,920,
			    1000,1100,1250,1500,1750};

//binning for double Jet topology 
const int nBins2=17;
const double bins2[nBins2]={480,500,520,560,600,640,680,720,760,800,840,920,
			    1000,1100,1250,1500,1750};

int main(){
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING) ;
  //DEBUG=0, INFO=1, PROGRESS=2, WARNING=3, ERROR=4, FATAL=5
  ofstream logf("./log_fitBackground.log",ios::out);

  TChain* chainData = new TChain("SelectedCandidates");
  chainData->Add( (inDir+"treeEDBR_DoubleMu_Run2012*.root").c_str()  );
  //  chainData->Add( (inDir+"treeEDBR_DoublePhoton_Run2012*.root").c_str()  );
  //  chainData->Add( (inDir+"treeEDBR_Photon_Run2012*.root").c_str()  );

  logf<<"In the data chain there are "<<chainData->GetEntries()<<" events"<<endl;

  //write in a plain tree because RooFit does not like trees with branches storing vectors 
  const int nxjCut=-1;//if negative: no cut
  const std::string tmpTreeName="SelectedCandidatesV2";
  char foutn[64];
  if(nxjCut>=0)  sprintf(foutn,"EXOVVTree_DATA_%d.root",nxjCut);
  else   sprintf(foutn,"EXOVVTree_DATA_NOcut.root");
  std::string tmpFileName=foutn;
  std::string weighting = "weight";
  CopyTreeVecToPlain(chainData,weighting,tmpFileName,tmpTreeName);//(TTree*)
  delete chainData;

  TFile *ftree=new TFile(foutn,"READ");
  TTree *treeDATA_tmp=(TTree*)ftree->Get(tmpTreeName.c_str());

  // gROOT->cd(); //magic!
 
  const double minMZZ=bins1[0];
  const double maxMZZ=bins1[nBins1-1];
  int inxj=0;
  RooRealVar* mZZ = new RooRealVar("mZZ","mZZ",minMZZ,maxMZZ);//bins[nBins-1]);
  RooRealVar *nXjets=new RooRealVar("nXjets","nXjets",-0.1,1.1);
  RooRealVar *mJJ=new RooRealVar("mJJ","mJJ",50.0,150.0);
  RooRealVar *lep=new RooRealVar("lep","lep",0.0,1.0);
  RooRealVar *region=new RooRealVar("region","region",-0.1,1.1);
  RooRealVar* alpha = new RooRealVar("alpha","alpha",1.,0.,100000.);//(RooRealVar*) dsDataSB->addColumn(*alpha_formula) ;

  for( inxj=0;inxj<3;inxj++){

    int nBins;
    const double* bins=0;

    if(inxj==2){
      nBins = nBins2;
      bins = bins2;
    }
    else if(inxj==1){
      nBins = nBins1;
      bins = bins1;
    }
    else{
      std::cout<<"Value of nxj not allowed: "<<inxj<<" . Skipping to the next."<<std::endl;
      continue;
    }

    std::stringstream ssnxj;
    ssnxj << inxj;
    logf<<"\n\n\n\n****** NEW NXJ = "<<inxj<<"  ---> "<<(outDir+"/Workspaces_alpha_"+ssnxj.str()+"J_"+leptType+".root").c_str() <<std::endl; 
    std::cout<<"\n\n\n\n****** NEW NXJ = "<<inxj<<"  ---> "<<(outDir+"/Workspaces_alpha_"+ssnxj.str()+"J_"+leptType+".root").c_str() <<std::endl; 
    TFile *falpha=new TFile( (outDir+"/Workspaces_alpha_"+ssnxj.str()+"J_"+leptType+".root").c_str() );
    char alphahname[50];
    //    sprintf(alphahname,"nominal_alpha_%dnxj",inxj);//histo with fit to alpha
    sprintf(alphahname,"h_alpha_smoothened");
    TTree* weightedData = weightTree(treeDATA_tmp ,(TH1D*)falpha->Get(alphahname),"alphaWeightedTree" );

    //stat uncertainty on alpha normalization

    double area_MCsig=( (TH1D*)falpha->Get("mX_signalRegion"))->Integral();
    double area_MCsb=( (TH1D*)falpha->Get("mX_sidebands"))->Integral();
    double nent_MCsig=( (TH1D*)falpha->Get("mX_signalRegion"))->GetEntries();
    double nent_MCsb=( (TH1D*)falpha->Get("mX_sidebands"))->GetEntries();
    double totAlpha=area_MCsig/area_MCsb;

    //this is if you have another alpha coming from a different model
    //(for example, Sherpa instead of MAdgraph)
    //    TFile *falphaAlt=new TFile( (outDir+"/Workspaces_alpha_"+ssnxj.str()+"J_"+leptType+".root").c_str() );
    // double area_MCsigAlt=( (TH1D*)falphaAlt->Get("mX_signalRegion"))->Integral();
    // double area_MCsbAlt=( (TH1D*)falphaAlt->Get("mX_sidebands"))->Integral();
    //double totAlphaAlt=area_MCsigAlt/area_MCsbAlt;
    // double alphaErrMod= (totAlpha-totAlphaAlt);
    double alphaErrMod= 0.0;
    double alphaErrtmp= totAlpha*sqrt(1.0/nent_MCsig +1.0/nent_MCsb) ;  
    double alphaErrTot= sqrt(alphaErrtmp*alphaErrtmp + alphaErrMod*alphaErrMod);//absolute error
    double alphaFitErr=((TF1*)( (TH1D*)falpha->Get(alphahname))->GetFunction("alpha_fitfunc"))->GetParError(0);//wow !
    logf<<"MC total statistics: SIG="<<nent_MCsig<<"  SB="<<nent_MCsb<<"  alphaERR="<<alphaErrtmp<<" (from fit: "<<alphaFitErr<<")"<<endl;
    RooRealVar* alphaErr=new RooRealVar("alphaNormErr","alphaNormErr (relative)", alphaErrTot/totAlpha);

    ////////    double minMZZ=bins[0];
    stringstream strmcut;
    strmcut<<minMZZ;
    //read in the file with the alpha fit function
    TF1 *alpha_func=(TF1*)falpha->Get("alpha_fitfunc");
    RooWorkspace *ws = (RooWorkspace*)falpha->Get( ("ws_alpha_"+ssnxj.str()+"J"+leptType).c_str() );
    ws->Print();
  
    RooPolynomial *ws_alpha_pol=(RooPolynomial*)ws->pdf("pol0_func");
    RooRealVar *c0=(RooRealVar*)ws->var("c0");
    RooPolynomial *pol0_new=new RooPolynomial("newpol0_func","Polynomial 0th (constant) for fit",*mZZ,*c0,0);
    RooFormulaVar *alpha_formula=new RooFormulaVar("alpha_formula","alpha_formula","@0",RooArgList(*c0));
    
    //select the data in the sidebands, convert it in a RooDataSet
    //weight events by the alpha function
    string cutSB= "nXjets=="+ssnxj.str()+" &&region==0.0 &&mZZ>"+strmcut.str();
    string cutSIG="nXjets=="+ssnxj.str()+" &&region==1.0 &&mZZ>"+ strmcut.str();
    string cutDum="nXjets=="+ssnxj.str()+" && mZZ>"+ strmcut.str();
 
    
    logf<<"Applying the following cuts: "<<cutSIG.c_str()<<std::endl;
    RooDataSet *dsDataSB=new RooDataSet("dsDataSB","dsDataSB",(TTree*)treeDATA_tmp,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep),cutSB.c_str()) ;
    //real data; weight each event by the alpha factor in order to scale to signal region

    logf<<"\n\n================================="<<std::endl;
    mZZ->setVal(500.0);
    logf<<"===> MZZ="<<mZZ->getVal()<<"    ALPHA (fromFIT)="<<alpha->getVal()<<std::endl;
    mZZ->setVal(700.0);
    logf<<"===> MZZ="<<mZZ->getVal()<<"    ALPHA (fromFIT)="<<alpha->getVal()<<std::endl;
    mZZ->setVal(900.0);
    logf<<"===> MZZ="<<mZZ->getVal()<<"    ALPHA (fromFIT)="<<alpha->getVal()<<std::endl;
    logf<<"=================================\n\n"<<std::endl;

    RooDataSet *dsDataSB2=new RooDataSet("dsDataSB2","dsDataSB2",weightedData,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep,*alpha),cutSB.c_str(),"alpha") ;
    RooDataSet *dsDataSIG=new RooDataSet("dsDataSIG","dsDataSIG",(TTree*)treeDATA_tmp,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep),cutSIG.c_str()) ;//real data in signal region; cuts on mjj and nXjets
    // dsDataSB->Print();
    // dsDataSIG->Print();
    // dsDataSB2->Print();
   

    //fit the weighted dataset with a custom function
    std::cout<<"STARTING TO FIT WITH CUSTOM FUNCTION"<<std::endl;
    mZZ->setRange("signalRange",minMZZ,1750.0) ;
    
    double inislope;
    if(inxj==1)inislope=-0.25;
    if(inxj==2)inislope=-0.1;
    RooRealVar *a0=new RooRealVar("a0","a0",inislope,-10.0,0.0);
    RooExponential *expo_fit=new RooExponential("exp_fit","exp_fit",*mZZ,*a0);
    //  RooRealVar *exp_norm=new RooRealVar("exp_N","exp_N",0.0,10000.0);
    
    //  RooFitResult* r_sig = expo_fit->fitTo(*dsDataSB,Save(kTRUE),Range("signalRange")) ;
    RooFitResult* r_sig2 = expo_fit->fitTo(*dsDataSB2,Save(kTRUE),SumW2Error(kTRUE),RooFit::PrintLevel(-1)) ;
    logf<<"\n\n\n\n\n#################\nCheck entries: "<<dsDataSB->sumEntries()<<"   "<<dsDataSB2->sumEntries()
	<<"##############\n\n\n\n\n"<<endl;
    RooRealVar *Nent=new RooRealVar("sidebandNormalization","Integral of data in sidebands before signal-rescaling",dsDataSB->numEntries(),0.0,50000.0);
    RooRealVar *NormErr=new RooRealVar("errNormDataSB","Statistical uncertainty on bkgd norm (relative)",1.0/sqrt(Nent->getVal()),0.0,50000.0);
    RooRealVar *Nerr=new RooRealVar("errNormalization","Total uncertainty on bkgd norm (relative)",sqrt(NormErr->getVal()*NormErr->getVal()+alphaErr->getVal()*alphaErr->getVal()) ,0.0,5.0);
    alphaErr->setConstant(kTRUE);
    NormErr->setConstant(kTRUE);
    Nerr->setConstant(kTRUE);
    RooRealVar *Nbkg=new RooRealVar("bkgdNormalization","Total uncertainty on background normalization",dsDataSB2->sumEntries(),0.0,10000.0);
    Nbkg->setConstant(kTRUE);
    // a0->setConstant(kTRUE);
    logf<<"Normalization errors: Nent="<<Nent->getVal()<<" NormErr="<<NormErr->getVal()<<"  Nerr="<<Nerr->getVal()<<std::endl;
    logf<<"Exp fit done: Norm = "<<Nbkg->getVal()<<"   Slope="<<a0->getVal()<<std::endl;
    
    
    //for levelled exponential
    double initf0=90.0;
    if(inxj==1)initf0=85.0;
    if(inxj==2)initf0=80.0;
    double initf1=0.0;
    if(inxj==1)initf1=0.0;
    if(inxj==2)initf1=0.0;

    RooRealVar *f0=new RooRealVar("f0","sigma",initf0,0.0,300.0);
    RooRealVar *f1=new RooRealVar("f1","alpha",initf1,-0.5,2.0);
    RooRealVar *f2=new RooRealVar("f2","m",240,200.0,500.0);
    RooRealVar *f3=new RooRealVar("f3","theta",0.0);
    f2->setConstant(kTRUE);
    f3->setConstant(kTRUE);
    //    if(inxj==2)   
    f1->setConstant(kTRUE);
    //  RooGenericPdf *expLev_fit=new RooGenericPdf("levelled_exp_fit","exp(-1.0*(mZZ-f2)/(f0 + f1*(mZZ-f2)))",RooArgList(*mZZ,*f0,*f1,*f2));
    RooLevelledExp *expLev_fit=new RooLevelledExp("levelled_exp_fit","levelled_exp_fit",*mZZ,*f0,*f1,*f2,*f3);
    RooFitResult* r_sig_expLev = expLev_fit->fitTo(*dsDataSB2,Save(kTRUE),SumW2Error(kTRUE),RooFit::PrintLevel(-1)) ;//,SumW2Error(kTRUE)
    
    logf<<"Alpha = "<<f0->getVal()<<"   Sigma="<<f1->getVal()<<"   m="<<f2->getVal()<<"  Theta="<<f3->getVal()<<std::endl;
    logf<<"FIT PERFORMED ! DECORRELATING PARAMETERS."<<std::endl;
    
    //now decorrelate parameters:
    RooWorkspace *wstmp=new RooWorkspace("tempWorkSpace","tempWorkSpac");
    char diagonalizerName[200];
    sprintf( diagonalizerName, "expLev_%db", inxj);
    PdfDiagonalizer diago(diagonalizerName, wstmp, *r_sig_expLev );
    RooAbsPdf  *background_decorr_ =diago.diagonalize(*expLev_fit);//RooAbsPdf
    background_decorr_->SetName("levelled_exp_decorr");
    char var1[50],var2[50];
    sprintf(var1,"expLev_%db_eig0",inxj);
    sprintf(var2,"expLev_%db_eig1",inxj);
    RooRealVar *f0rot=(RooRealVar*)wstmp->var(var1);
    RooRealVar *f1rot=0;
    f1rot=new RooRealVar(*f1); f1rot->SetName(var2);
    // if(inxj==2){f1rot=new RooRealVar(*f1); f1rot->SetName(var2);}
    // else f1rot=(RooRealVar*)wstmp->var(var2);  
    // wstmp->Print();
    //refit to get a new covariance matrix to check that this has worked
    RooFitResult *r_sig_expLev_decorr = background_decorr_->fitTo(*dsDataSB2, Save(),SumW2Error(kTRUE),RooFit::PrintLevel(-1));
    char fitResultName_eig[200];
    sprintf( fitResultName_eig, "resultsExpLevelledFit_%dJ_%s_decorr",inxj , leptType.c_str() );
    r_sig_expLev_decorr->SetName(fitResultName_eig); 
    logf<<"Alpha= "<<f0->getVal()<<" -> AlphaDecorr="<<f0rot->getVal()<<std::endl;
    logf<<"Sigma= "<<f1->getVal()<<" -> SigmaDecorr="<<f1rot->getVal()<<std::endl;
    logf<<"m= "<<f2->getVal()<<" Theta="<<f3->getVal()<<std::endl;
    //  return 0;
    sprintf( fitResultName_eig, "%s/resultsExpLevelledFit_%dJ_%s_decorr.txt",outDir.c_str(),inxj , leptType.c_str() );
    RooArgSet rotvars( *f0rot,*f1rot );
    rotvars.writeToFile(fitResultName_eig);

    //plot the correlated/decorrelated uncertainties
 
    bool plotDecorr=false;
    if(plotDecorr){
    TCanvas* c_rot = new TCanvas("canvas_rot", "CANVAS PARS ROT", 800, 800);
    c_rot->cd();
    double f0val=f0->getVal();
    double f1val=f1->getVal();
    double f0err=f0->getError();
    double f1err=f1->getError();
    Double_t rho= r_sig_expLev->correlation(f0->GetName(),f1->GetName());
    RooEllipse *contour= new RooEllipse("contour",f0val,f1val,f0err,f1err,rho,500);
    contour->SetLineWidth(2) ;
    RooPlot *plot = new RooPlot(*f0,*f1,f0val-2*f0err,f0val+2*f0err,f1val-2*f1err,f1val+2*f1err);
    plot->addPlotable(contour);
    plot->Draw();
    c_rot->SaveAs((outDir+"/checkRotation_"+ssnxj.str()+"J"+leptType+".eps").c_str());
    
    f0val=f0rot->getVal();
    f1val=f1rot->getVal();
    f0err=f0rot->getError();
    f1err=f1rot->getError();
    rho= r_sig_expLev_decorr->correlation(f0rot->GetName(),f1rot->GetName());
    RooEllipse *contourRot= new RooEllipse("contourRot",f0val,f1val,f0err,f1err,rho,500);
    contourRot->SetLineWidth(2) ;
    contourRot->SetLineColor(kRed) ;
    RooPlot *plotRot = new RooPlot(*f0rot,*f1rot,f0val-2*f0err,f0val+2*f0err,f1val-2*f1err,f1val+2*f1err);
    plotRot->addPlotable(contourRot);
    plotRot->Draw();
    // ContourPlot(f0,f1,r_sig_expLev);
    c_rot->SaveAs((outDir+"/checkRotation_"+ssnxj.str()+"J"+leptType+"_decorrelated.eps").c_str());
    
    delete contour;
    delete c_rot;
    }//end plot Decorr

    //save everything in a RooWorkspace
    TFile *fout=new TFile((outDir+"/workspace_"+ssnxj.str()+"J"+leptType+"_new.root").c_str(),"RECREATE");
    RooWorkspace *wsnew=new RooWorkspace(*ws);
    wsnew->SetName(("ws_alpha_"+ssnxj.str()+"J"+leptType).c_str());
    logf<<"\n\nName of new WS: "<<wsnew->GetName()<<std::endl;
    //wsnew->import(*expo_fit);
    wsnew->import(*Nbkg);
    wsnew->import(*Nent);
    wsnew->import(*alphaErr);
    wsnew->import(*NormErr);
    wsnew->import(*Nerr);
    wsnew->import(*expLev_fit);
    wsnew->import(*background_decorr_, RooFit::RecycleConflictNodes()); 
    wsnew->import(*dsDataSB);
    wsnew->import(*dsDataSB2);
    wsnew->import(*dsDataSIG);
    wsnew->import(*r_sig_expLev_decorr);
    logf<<"Everything imported. Saving."<<std::endl;
  
    r_sig_expLev_decorr->Write();
    weightedData->Write();
    
    
    //do pseudo-experiments for alpha-error
    for( unsigned n=0 ;n<nToys;n++){
      //get alternative weights
      if(n%50==0)std::cout<<"Throwing toy #"<<n<<"/"<<nToys<<" "<<std::endl;
      sprintf(alphahname,"tmp_%d",n);
      TTree* weightedPseudo = weightTree(treeDATA_tmp ,(TH1D*)falpha->Get(alphahname), alphahname);
      RooDataSet pseudoSB2("pseudoSB2","pseudoSB2",weightedPseudo,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep,*alpha),cutSB.c_str(),"alpha") ;
      fitPseudo(pseudoSB2,*wsnew,n,fitResultName_eig,inxj);
      
      //      delete weightedPseudo;
    }
    char var1errA[50];
    char var2errA[50];
    sprintf(var1errA,"expLev_%db_eig0_alphaErr",inxj);
    sprintf(var2errA,"expLev_%db_eig1_alphaErr",inxj);
    RooRealVar errV1(var1errA,var1errA,0.0);
    RooRealVar errV2(var2errA,var2errA,0.0);
    std::cout<<"Starting pseudoMassge"<<std::endl;
    pseudoMassge( inxj ,r_sig_expLev_decorr,*wsnew,fitResultName_eig, Nerr->getVal(), errV1,errV2);

    wsnew->import(errV1);
    wsnew->import(errV2);
    fout->cd();
    wsnew->Write();
   
    //draw and save plots
    std::cout<<"Drawing"<<std::endl;

    TCanvas *can1=new TCanvas("canvas1", "can1",800,800);
    can1->cd();
    RooPlot *xf=mZZ->frame();
    //DO NOT CHANGE THE ORDER !!!!!!! DATA AS FIRST ONES !!!!
    dsDataSB2->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(21),MarkerColor(kRed));
    background_decorr_->plotOn(xf, Normalization(Nbkg->getVal(),RooAbsPdf::NumEvent), LineColor(kViolet-2),VisualizeError(*r_sig_expLev_decorr,2.0,kFALSE),FillColor(kYellow));
    background_decorr_->plotOn(xf, Normalization(Nbkg->getVal(),RooAbsPdf::NumEvent), LineColor(kViolet-2),VisualizeError(*r_sig_expLev_decorr,1.0,kFALSE),FillColor(kGreen));
    expo_fit->plotOn(xf, Normalization(Nbkg->getVal(),RooAbsPdf::NumEvent), LineColor(kBlue), LineStyle(kDashed));
    expLev_fit->plotOn(xf, Normalization(Nbkg->getVal(),RooAbsPdf::NumEvent), LineColor(kOrange+3));//RooAbsPdf::NumEvent
    background_decorr_->plotOn(xf, Normalization(Nbkg->getVal(),RooAbsPdf::NumEvent), LineColor(kViolet-2));
    background_decorr_->plotOn(xf, Normalization((Nbkg->getVal()+Nerr->getVal()*Nbkg->getVal()),RooAbsPdf::NumEvent), LineColor(kViolet-2), LineStyle(kDashed));
    background_decorr_->plotOn(xf, Normalization((Nbkg->getVal()-Nerr->getVal()*Nbkg->getVal()),RooAbsPdf::NumEvent), LineColor(kViolet-2), LineStyle(kDashed));
    dsDataSB2->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(21),MarkerColor(kRed));//,Normalization(dsDataSB2->numEntries(),RooAbsPdf::NumEvent)
    logf<<"Check nromalization: NumEntries of dsDataSIG= "<<dsDataSIG->numEntries() <<"("<<dsDataSIG->sumEntries() <<")    SumEntries of dsDataSB2="<<dsDataSB2->sumEntries()<<"   numEntries="<<dsDataSB2->numEntries()<<"    Nbkg (NORM)="<<Nbkg->getVal()<<"   Nent="<<Nent->getVal()<<"     Nerr="<<Nerr->getVal() <<std::endl;
    xf->Draw();
    can1->SaveAs((outDir+"/fitPlot_"+ssnxj.str()+"J_"+leptType+".root").c_str());
    can1->SaveAs((outDir+"/fitPlot_"+ssnxj.str()+"J_"+leptType+".eps").c_str());
    xf->SetMinimum(0.06);
    gPad->SetLogy();
    xf->Draw();
    can1->SaveAs((outDir+"/fitPlot_"+ssnxj.str()+"J_"+leptType+"_log.root").c_str());
    can1->SaveAs((outDir+"/fitPlot_"+ssnxj.str()+"J_"+leptType+"_log.eps").c_str());
    delete xf;
     
    //don't change this order, for God's sake !
    delete expLev_fit;delete r_sig_expLev;delete r_sig_expLev_decorr;
   
    delete dsDataSB;delete dsDataSB2;delete dsDataSIG;
    delete wsnew;// delete wstmp;
    delete ws;
    delete fout;
    // if(inxj==2) delete falpha;
  }//end loop on nxj
  logf.close();
  


}//end main



RooPlot* ContourPlot(RooRealVar* var1,RooRealVar* var2, RooFitResult* r){

  Double_t x1= var1->getVal();
  Double_t x2= var2->getVal();
  Double_t s1= var1->getError();
  Double_t s2= var2->getError();
  Double_t rho= r->correlation(var1->GetName(),var2->GetName());

  RooEllipse *contour= new RooEllipse("contour",x1,x2,s1,s2,rho,500);
  contour->SetLineWidth(2) ;


  RooPlot *plot = new RooPlot(*var1,*var2,x1-2*s1,x1+2*s1,x2-2*s2,x2+2*s2);
  plot->addPlotable(contour);

  r->plotOn(plot,*var1,*var2,"M12");

  plot->Draw();
  return plot;

}

TTree* weightTree(TTree* tree, TH1D* h1_alpha,const std::string& name ,bool verbose){
  gROOT->cd(); //magic!


  Double_t mZZ;
  tree->SetBranchAddress( "mZZ", &mZZ );

  TTree* newTree = tree->CloneTree(0);
  newTree->SetName(name.c_str());

  Double_t alpha;
  newTree->Branch( "alpha", &alpha, "alpha/D" );
   
  unsigned nentries = tree->GetEntries();

  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    tree->GetEntry( iEntry );
    if( (iEntry % 10000)==0 && verbose) std::cout << "weightTree: Entry " << iEntry << "/" << nentries << std::endl;

    int alphabin = h1_alpha->FindBin( mZZ );
    if(alphabin==0 ||alphabin> h1_alpha->GetNbinsX())
      alpha=1.;
    else
      alpha = h1_alpha->GetBinContent( alphabin );
    
    newTree->Fill();

  }
  
  //delete tree;
  return newTree;

}

void fitPseudo( RooDataSet& ModSideband, RooWorkspace& ws ,int seed,char* initalvalues , int nxj) {
  //reset parameters
  char var1[50];
  char var2[50];
  sprintf(var1,"expLev_%db_eig0",nxj);
  //  sprintf(var2,"expLev_%db_eig1",nxj);
  char both[100];
  if(nxj==2) sprintf(both,"%s",var1);
  else sprintf(both,"%s,%s",var1,var2);
  sprintf(both,"%s",var1);
  ws.argSet(both).readFromFile(initalvalues);

  RooFitResult* r_pseudo = ws.pdf("levelled_exp_decorr")->fitTo(ModSideband, SumW2Error(kTRUE), Save(),RooFit::PrintLevel(-1) );

  char indexstring[200];
  sprintf(indexstring,"TMPResult_%dtag_%d",nxj,seed);
  RooArgSet tmpset(r_pseudo->floatParsFinal());
  tmpset.writeToFile(indexstring);

}

void pseudoMassge(int nxj , RooFitResult* r_nominal, RooWorkspace& ws,char* initalvalues, double NormRelErr, RooRealVar &errV1, RooRealVar &errV2){
  char var1[50];
  char var2[50];
  sprintf(var1,"expLev_%db_eig0",nxj);
  sprintf(var2,"expLev_%db_eig1",nxj);
  char both[100];
  if(nxj==2) sprintf(both,"%s",var1);
  else sprintf(both,"%s,%s",var1,var2);
  sprintf(both,"%s",var1);
  ws.argSet(both).readFromFile(initalvalues);

  RooPlot *plot_MCbkg = ws.var("mZZ")->frame();
  
  std::vector<float> vals;
  std::vector<float> vals1;
  std::vector<float> vals2;
  vals.reserve(nToys);
  vals1.reserve(nToys);
  vals2.reserve(nToys);

  char indexstring[200];
  for(unsigned i =0 ; i < nToys ; i++){
    sprintf(indexstring,"TMPResult_%dtag_%d",nxj,i);
    ws.argSet(both).readFromFile(indexstring);
    vals1.push_back(ws.var(var1)->getVal());
    //    if(nxj!=2)vals2.push_back(ws.var(var2)->getVal());
    ws.pdf("levelled_exp_decorr")->plotOn(plot_MCbkg,LineWidth(1),LineColor(1));
    std::string mkdir_command("rm ");  
    mkdir_command+= indexstring;
    system(mkdir_command.c_str());
    if(i%100==0)    std::cout << mkdir_command << std::endl;
  }

  RooCurve* upper = new RooCurve();
  RooCurve* lower = new RooCurve();
  upper->SetName("upper_staterr_alpha");
  lower->SetName("lower_staterr_alpha");
  float min = plot_MCbkg->GetXaxis()->GetXmin();
  float max = plot_MCbkg->GetXaxis()->GetXmax();
  float range=max-min;
  for(unsigned int x =0 ; x < 200 ; x++  ){
    float xval = min +x*range/200.;
    for(unsigned i =0 ; i < nToys ; i++){
      vals.push_back(((RooCurve*)(plot_MCbkg->getObject(i)))->interpolate(xval));
    }

    std::sort(vals.begin(),vals.end());
    lower->addPoint(xval,vals[floor(0.166*nToys)]);
    upper->addPoint(xval,vals[floor(0.832*nToys)]);
    vals.clear();
  }

  ws.argSet(both).readFromFile(initalvalues);// read nominal best fit value
  double x1= ws.var(var1)->getVal();
  double x2= 0.0;
  //  if(nxj!=2)x2=ws.var(var2)->getVal();
  double e1= ws.var(var1)->getError();
  double e2= 0.0;
  //  if(nxj!=2)e2=ws.var(var2)->getError();


  lower->SetLineWidth(2);
  lower->SetLineColor(2);
  upper->SetLineWidth(2);
  upper->SetLineColor(2);


  ws.pdf("levelled_exp_decorr")->plotOn(plot_MCbkg,VisualizeError(*r_nominal,2.0,kFALSE),FillColor(kYellow));
  ws.pdf("levelled_exp_decorr")->plotOn(plot_MCbkg,VisualizeError(*r_nominal,1.0,kFALSE),FillColor(kGreen));
  ws.pdf("levelled_exp_decorr")->plotOn(plot_MCbkg);
  ws.pdf("levelled_exp_decorr")->plotOn(plot_MCbkg, LineColor(kViolet-2), LineStyle(kDashed), Normalization(1.0+NormRelErr,RooAbsReal::Relative));
  ws.pdf("levelled_exp_decorr")->plotOn(plot_MCbkg, LineColor(kViolet-2), LineStyle(kDashed), Normalization(1.0-NormRelErr,RooAbsReal::Relative));
  plot_MCbkg->addPlotable(lower);
  plot_MCbkg->addPlotable(upper);

 
  /*
  double x0,up0,low0;
  upper->GetPoint(0,x0,up0);  
  lower->GetPoint(0,x0,low0);  
  RooRealVar *r0=new RooRealVar("mZZ","mZZ",x0);
  RooArgSet *tmparg=new RooArgSet(*r0);
  double central0=1.0;//ws.pdf("levelled_exp_decorr")->getValV();
  RooRealVar normUp("norm_upper_staterr_alpha","norm_upper_staterr_alpha",up0/central0);
  RooRealVar normDown("norm_lower_staterr_alpha","norm_lower_staterr_alpha",low0/central0);
  std::cout<<"###Norm for UP/LOW: "<<x0<<" - "<< r0->getVal()<<" - "<<up0<<" - "<<low0<<" - "<<central0<<" - "<<normUp.getVal()<<std::endl;
  */

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();
  plot_MCbkg->Draw();
  char canvasName[400];
  sprintf( canvasName, "%s/mZZ_sidenbandData_alphaVar_%dnxj_%s", outDir.c_str(), nxj, "ALL");
  std::string canvasName_str(canvasName);
  std::string canvasName_eps = canvasName_str + ".eps";
  c1->SaveAs(canvasName_eps.c_str());

  plot_MCbkg->SetMinimum(0.0001);
  plot_MCbkg->SetMaximum(1.0);
  plot_MCbkg->Draw();
  c1->SetLogy();

  canvasName_str += "_log";
  canvasName_eps = canvasName_str + ".eps";
  c1->SaveAs(canvasName_eps.c_str());

  c1->Clear();
  c1->SetLogy(false);

  char dumname[200];
   sprintf(dumname,"%s/rooCurves_%dJ_ALL.root",outDir.c_str(),nxj);
   TFile *fdumout=new TFile(dumname ,"RECREATE"); 
   char upname[50];
   char lowname[50];
   sprintf(upname,"roocurve_%dJ_UP",nxj);
   sprintf(lowname,"roocurve_%dJ_LOW",nxj);
  //sprintf(var2errA,"%s_alphaErr",var2);
   upper->Write();
     lower->Write();
   //   normUp.Write();
   // normDown.Write();
   //fdumout->Close();
   delete fdumout;

  delete plot_MCbkg;

  // plotting value distributions
  std::sort(vals1.begin(),vals1.end());

  TH1F* histo1= new TH1F("test1","test1",100,vals1[0],vals1[nToys-1]);
  char tit[20];
  sprintf(tit,"#alpha_{%d}",nxj);
  histo1->GetXaxis()->SetTitle(tit);
  histo1->GetYaxis()->SetTitle("Nr trials");

  std::sort(vals2.begin(),vals2.end());
  TH1F* histo2;
  //  if(nxj!=2)histo2 = new TH1F("test2","test2",100,vals2[0],vals2[nToys-1]);  
  //  else histo2 = (TH1F*)histo1->Clone("test2");
  histo2 = (TH1F*)histo1->Clone("test2");
  sprintf(tit,"#beta_{%d}",nxj);
  histo2->GetYaxis()->SetTitle("Nr trials");
  histo2->GetXaxis()->SetTitle(tit);
  
  
  TLine* line = new TLine();
  line->SetLineColor(2);
  line->SetLineWidth(2);
  for(unsigned i =0 ; i <  nToys; i++){
    histo1->Fill(vals1[i]);
    //    if(nxj!=2) histo2->Fill(vals2[i]);
    //    else histo2->Fill(0.0);
    histo2->Fill(0.0);
  }
  
//   std::cout << "XXXX before fits" << std::endl;

//   histo1->Print("v");
//   histo2->Print("all");


  histo1->Fit("gaus");
  double s1 = histo1->GetFunction("gaus")->GetParameter("Sigma"); // we need this to update the errors 
  histo2->Fit("gaus");
  //  double s2 = 0.0;
  double s2 = histo2->GetFunction("gaus")->GetParameter("Sigma"); // we need this to update the errors
  histo1->Draw();
  line->SetLineColor(2);
  line->DrawLine(x1,histo1->GetMinimum(),x1,histo1->GetMaximum());
  line->SetLineColor(4);
  line->DrawLine(x1+e1,histo1->GetMinimum(),x1+e1,histo1->GetMaximum());
  line->DrawLine(x1-e1,histo1->GetMinimum(),x1-e1,histo1->GetMaximum());


  sprintf( canvasName, "%s/alphaVar_par1_%dJ_%s", outDir.c_str(), nxj, "ALL");
  canvasName_str = canvasName;
  canvasName_eps = canvasName_str + ".eps";
  c1->SaveAs(canvasName_eps.c_str());

  histo2->Draw();
  line->SetLineColor(2);
  line->DrawLine(x2,histo2->GetMinimum(),x2,histo2->GetMaximum());
  line->SetLineColor(4);
  line->DrawLine(x2+e2,histo2->GetMinimum(),x2+e2,histo2->GetMaximum());
  line->DrawLine(x2-e2,histo2->GetMinimum(),x2-e2,histo2->GetMaximum());

  sprintf( canvasName, "%s/alphaVar_par2_%dJ_%s", outDir.c_str(), nxj, "ALL");
  canvasName_str = canvasName;
  canvasName_eps = canvasName_str + ".eps";
  c1->SaveAs(canvasName_eps.c_str());


  // ws->import(upper);//not supported by RooFit
  // ws->import(lower);

  //  char var1errA[50];
  // char var2errA[50];
  // sprintf(var1errA,"%s_alphaErr",var1);
  //sprintf(var2errA,"%s_alphaErr",var2);
  // RooRealVar errV1(var1errA,var1errA,s1);
  //RooRealVar errV2(var2errA,var2errA,s2);
   errV1.setVal(s1);
   errV2.setVal(s2);
   errV1.setConstant(kTRUE);
   errV2.setConstant(kTRUE);
  // ws.import(errV1);
  // ws.import(errV2);
   //   RooArgSet *res=new RooArgSet(errV1,errV2);
   //   cout<<"Exiting."<<endl;

   std::cout<<"Finished pseudoMassge !"<<std::endl;
}



void CopyTreeVecToPlain(TTree *t1, std::string wType, std::string f2Name,std::string t2Name,int nxjCut){
 
  int ncands; 
  double eventWeight;
  unsigned int nrun,nevt;
  double leptType;
  int mynxj[35];
  double mZZd[35],region[35],mZqq[35];


  t1->SetBranchAddress("nCands",&ncands);
  t1->SetBranchAddress("run",&nrun);
  t1->SetBranchAddress("event",&nevt);
  t1->SetBranchAddress("lep",&leptType);
  t1->SetBranchAddress(wType.c_str(),&eventWeight);
  t1->SetBranchAddress("mZZ",mZZd);
  t1->SetBranchAddress("nXjets",mynxj);
  t1->SetBranchAddress("mJJ",mZqq);
  t1->SetBranchAddress("region",region);


  TFile *fOut=new TFile(f2Name.c_str(),"RECREATE");
  fOut->cd();
 
  int ncands_2, mynxj_2;
  double eventWeight_2;
  unsigned int nrun_2,nevt_2;
  double leptType_2;
  double mZZd_2,region_2,mZqq_2;

  TTree *t2=new TTree(t2Name.c_str(),t2Name.c_str());
 
  t2->Branch("nCands",&ncands_2,"nCands/I");
  t2->Branch("run",&nrun_2,"run/i");
  t2->Branch("event",&nevt_2,"event/i");
  t2->Branch("weight",&eventWeight_2,"weight/D");
  t2->Branch("nXjets",&mynxj_2 , "nXjets/I");
  t2->Branch("lep",&leptType_2,"lep/D");
  t2->Branch("mZZ",&mZZd_2 , "mZZ/D");
  t2->Branch("mJJ",&mZqq_2 , "mJJ/D");
  t2->Branch("region",&region_2 , "region/D");


  for(int i=0;i<t1->GetEntries();i++){

    t1->GetEntry(i);
    
    for(int j=0;j<ncands;j++){
      ncands_2=ncands;
      nrun_2=nrun;
      nevt_2=nevt;
      eventWeight_2=eventWeight;
      leptType_2=leptType;
      mZZd_2=mZZd[j];
      region_2=region[j];
      mZqq_2=mZqq[j];
      mynxj_2=int(mynxj[j]);
 
      if(region[j]<0||mZZd_2>9999.0||mynxj_2>10||mZqq_2>999.0){
	//cout<<"Event="<<nevt<<" cand="<<j<<" has reg="<<region[j]<<" mZZ="<<mZZd_2<<endl;
	continue;
      }

      if(mynxj_2==nxjCut||nxjCut<0) {
	int nb=t2->Fill();
	//	if(i%1000==1)cout<<"Filled "<<nb<<" bytes"<<endl;
	//	if(nb<0)cout<<"====>  Event"<<nevt_2 <<"  Filled "<<nb<<" bytes"<<endl;
      }
    }//end loop on j -> nCands

  }//end loop on i -> entries of input tree

  cout<<"returning t2 with "<<t2->GetEntries()<<" entries"<<endl;




  //  cout<<"Writing unrolled tree"<<endl;
  // t2->Write();
  fOut->Write();
  delete t2;
  delete fOut;
 

  //  cout<<"returning"<<endl;
  // return t2;
}

