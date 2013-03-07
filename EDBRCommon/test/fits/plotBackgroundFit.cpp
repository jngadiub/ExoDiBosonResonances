#include <sstream>
#include <string>
#include <fstream>
#include <Riostream.h>

#include "TH1D.h"
#include "TF1.h"
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
#include "RooExponential.h"
#include "RooDataSet.h"
#include "RooExtendPdf.h"
#include "RooGenericPdf.h"
#include "RooExtendPdf.h"
#include "RooBinning.h"
#include "RooAddPdf.h"

using namespace RooFit ;

const string outDir="FitSidebandsMJJ/";
const bool plotErrFromAlpha=false;
//binning for merged Jet topology 
const int nBins1=22;
const double bins1[nBins1]={480,500,520,560,600,640,680,720,760,800,840,920,
			    1000,1100,1250,1400,1600,1800,2000,2200,2400,2600};

//binning for double Jet topology 
const int nBins2=22;
const double bins2[nBins2]={480,500,520,560,600,640,680,720,760,800,840,920,
			    1000,1100,1250,1400,1600,1800,2000,2200,2400,2600};


int main(){

  ofstream logf("./log_plotBackground.log",ios::out);
  int inxj=0;
  for(inxj=1;inxj<3;inxj++){

    int nBins;
    const double* bins;

    if(inxj==2){
      nBins = nBins2;
      bins = bins2;
    }
    else{
      nBins = nBins1;
      bins = bins1;
    }


    stringstream ssnxj;
    ssnxj<<inxj;
    string leptType="ALL";

    //const string outDir="FitSidebandsELE";
    RooCurve *curve_AlphaErr_UP=0;
    RooCurve *curve_AlphaErr_DOWN=0;
    if(plotErrFromAlpha){
      TFile *fcurve=new TFile((outDir+"/rooCurves_"+ssnxj.str()+"J"+leptType+".root").c_str(),"READ");
      curve_AlphaErr_UP=(RooCurve*)fcurve->Get("upper_staterr_alpha");
      curve_AlphaErr_DOWN=(RooCurve*)fcurve->Get("lower_staterr_alpha");
    }

    logf<<"\n\nLoad file "<<std::flush;
    TFile *f=new TFile((outDir+"/workspace_"+ssnxj.str()+"J"+leptType+"_new.root").c_str(),"READ");
    logf<<f->GetName()<<std::endl;;
    RooWorkspace *w1=(RooWorkspace*)f->Get(("ws_alpha_"+ssnxj.str()+"J"+leptType).c_str());
    f->ls();
    logf<<"Dumping contents of the WS: "<<("ws_alpha_"+ssnxj.str()+"J"+leptType).c_str()<<std::endl;
    w1->Print();
    RooRealVar *x=w1->var("mZZ");
    //x->setRange(bins[0],bins[nBins-1]);
    RooRealVar *N=w1->var("bkgdNormalization"); 
    
    //  RooAbsPdf *fit=(RooAbsPdf*)w1->pdf("levelled_exp_decorr");
    RooAbsPdf *fit=(RooAbsPdf*)w1->pdf("exp_fit");
    RooAbsData *SIGdset=w1->data("dsDataSIG")->reduce(("nNxjets=="+ssnxj.str()).c_str());//+"&&mZZ>420&&mZZ<700"
    RooAbsData *SBrescaleddset=w1->data("dsDataSB2")->reduce(("nNxjets=="+ssnxj.str()).c_str());//+"&&mZZ>420&&mZZ<700"
    RooAbsData *SBdset=w1->data("dsDataSB");//->reduce(("nNxjs=="+ssnxj+" && lep==0").c_str());
    RooRealVar *Nent=new RooRealVar("sidebandNormalization","Integral of data in sidebands before signal-rescaling",SBdset->numEntries(),0.0,50000.0);
    // RooRealVar *Nerr=new RooRealVar("errNormalization","Statistical uncertainty on bkgd norm",sqrt(Nent->getVal()),0.0,50000.0);
    RooRealVar *Nerr=w1->var("errNormalization");
    
    N->setVal(SBrescaleddset->sumEntries());
    
    char fitResultName_eig[200];
    //    sprintf( fitResultName_eig, "resultsExpLevelledFit_%dJ_%s_decorr",inxj , leptType.c_str() );
    sprintf( fitResultName_eig, "resultsExpoFit_%dJ_%s",inxj , leptType.c_str() );
    RooFitResult *resfit=(RooFitResult*)f->Get(fitResultName_eig);
    
    
    logf<<"\n\nYields:"<<endl;
    logf<<"Sample\t\t\t\tTotal\t\t\t#[1000, 1200]"<<endl;
    logf<<"Data (SIG)\t\t\t" <<SIGdset->numEntries()<<"\t\t\t" <<SIGdset->reduce("mZZ>1000&&mZZ<1200")->numEntries()<<endl;
    logf<<"Data (SB)\t\t\t" <<SBdset->numEntries()<<"\t\t\t" <<SBdset->reduce("mZZ>1000&&mZZ<1200")->numEntries()<<endl;
    logf<<"Data (SB-rescaled)\t\t" <<SBrescaleddset->sumEntries()<<"\t\t\t" <<SBrescaleddset->reduce("mZZ>1000&&mZZ<1200")->sumEntries()<<"  (" <<SBrescaleddset->reduce("mZZ>1000&&mZZ<1200")->numEntries()<<")"<<endl;
    
    RooArgSet nset(*x) ;
    x->setRange("integrationRange",1000.0,1200.0);
    logf<<"Bkgd (pred.)\t\t\t" <<N->getVal()<<" * "<<fit->createIntegral(*x,NormSet(*x))->getVal() <<"\t\t\t"<<N->getVal()<<" * " <<fit->createIntegral(*x,Range("integrationRange"),NormSet(*x))->getVal() <<endl;
    logf<<"\nError on total normalization of the background prediciton: sqrt{ ("<<w1->var("errNormDataSB")->getVal()<<" )^2 + ( "<<w1->var("alphaNormErr")->getVal()<<" )^2 } = "<<Nerr->getVal()<<endl;
    RooRealVar *alphaErr1=0, *alphaErr2=0;//(RooRealVar*)w1->var();
    RooArgList bgPars = resfit->floatParsFinal();
    std::vector<std::string> varname; 
    varname.reserve(bgPars.getSize());
    for( int iVar=0; iVar<bgPars.getSize(); ++iVar ) {
      varname.push_back(bgPars.at(iVar)->GetName());
      varname.at(iVar)+="_alphaErr";
    }
    alphaErr1=w1->var(varname.at(0).c_str());
    //   alphaErr2=w1->var(varname.at(1).c_str());
    logf<<"##### ALPHA ERRS : "<<alphaErr1->getVal()<<" ,  "<<endl;
    RooPlot *xf=x->frame();
    
    logf<<"Normalizations: Expected background="<<N->getVal()<<"   StatUncertainty="<<Nerr->getVal()*N->getVal()<<std::endl;
    TCanvas *can1=new TCanvas("c1", "can1",800,800);
    can1->cd();
    SIGdset->plotOn(xf,Binning(RooBinning(nBins-1,bins)));
    // EXPfit->plotOn(xf, Normalization(23,RooAbsPdf::NumEvent));
    //   EXPfit->plotOn(xf, Normalization(SIGdset->numEntries(),RooAbsPdf::NumEvent));
    fit->plotOn(xf, Normalization(N->getVal(),RooAbsPdf::NumEvent), LineColor(kViolet-2),VisualizeError(*resfit,2.0,kFALSE),FillColor(kYellow));
    fit->plotOn(xf, Normalization(N->getVal(),RooAbsPdf::NumEvent), LineColor(kViolet-2),VisualizeError(*resfit,1.0,kFALSE),FillColor(kGreen));
    fit->plotOn(xf, Normalization(N->getVal(),RooAbsPdf::NumEvent), LineColor(kOrange+3));
    fit->plotOn(xf, Normalization((N->getVal()+Nerr->getVal()*Nent->getVal()),RooAbsPdf::NumEvent), LineColor(kViolet-2), LineStyle(kDashed));
    fit->plotOn(xf, Normalization((N->getVal()-Nerr->getVal()*Nent->getVal()),RooAbsPdf::NumEvent), LineColor(kViolet-2), LineStyle(kDashed));

  
    if(plotErrFromAlpha){
    RooCurve* upper = new RooCurve();
    RooCurve* lower = new RooCurve();
    upper->SetName("upper");
    lower->SetName("lower");
    float min = xf->GetXaxis()->GetXmin();
    float max = xf->GetXaxis()->GetXmax();
    float range=max-min;
    for(unsigned int x =0 ; x < 200 ; x++  ){
      float xval = min +x*range/200.;
      float yvalDOWN = curve_AlphaErr_DOWN->interpolate(xval)*SBrescaleddset->sumEntries();
      float yvalUP = curve_AlphaErr_UP->interpolate(xval)*SBrescaleddset->sumEntries();
      lower->addPoint(xval,yvalDOWN);
      upper->addPoint(xval,yvalUP);
    }
    lower->SetLineWidth(2);
    lower->SetLineColor(2);
    upper->SetLineWidth(2);
    upper->SetLineColor(2);
    xf->addPlotable(lower);
    xf->addPlotable(upper);
    // curve_AlphaErr_DOWN->plotOn(xf,);
    // curve_AlphaErr_UP->plotOn(xf,);
    }


    SBrescaleddset->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(25),MarkerColor(kRed));
    SIGdset->plotOn(xf,Binning(RooBinning(nBins-1,bins)));
    // SBdset->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(24),MarkerColor(kGreen+3));
    
    xf->Draw();
    can1->SaveAs((outDir+"/plotMZZ_DataSigRegionWithBkgdFit_"+ssnxj.str()+"J"+leptType+".root").c_str());
    can1->SaveAs((outDir+"/plotMZZ_DataSigRegionWithBkgdFit_"+ssnxj.str()+"J"+leptType+".eps").c_str());
    can1->SaveAs((outDir+"/plotMZZ_DataSigRegionWithBkgdFit_"+ssnxj.str()+"J"+leptType+".png").c_str());
    
    //can1->SaveAs((outDir+"/tmpMZZ_DataSigRegionWithBkgdFit_"+ssnxj.str()+"J"+leptType+"_Zoomed.root").c_str());
    //can1->SaveAs((outDir+"/tmpMZZ_DataSigRegionWithBkgdFit_"+ssnxj.str()+"J"+leptType+"_Zoomed.eps").c_str());
    //can1->SaveAs((outDir+"/tmpMZZ_DataSigRegionWithBkgdFit_"+ssnxj.str()+"J"+leptType+"_Zoomed.png").c_str());
    double maxfr= inxj==2? 500.0 : 1500.0;
    double minfr= inxj==2? 0.0005 : 0.1;
    xf->SetMaximum(maxfr);
    xf->SetMinimum(minfr); 
    gPad->SetLogy();
    xf->Draw();
    can1->SaveAs((outDir+"/plotMZZ_DataSigRegionWithBkgdFit_"+ssnxj.str()+"J"+leptType+"_log.root").c_str());
    can1->SaveAs((outDir+"/plotMZZ_DataSigRegionWithBkgdFit_"+ssnxj.str()+"J"+leptType+"_log.eps").c_str());
    can1->SaveAs((outDir+"/plotMZZ_DataSigRegionWithBkgdFit_"+ssnxj.str()+"J"+leptType+"_log.png").c_str());
    
    //can1->SaveAs((outDir+"/tmpMZZ_DataSigRegionWithBkgdFit_"+ssnxj.str()+"J"+leptType+"_Zoomed_log.root").c_str());
    //can1->SaveAs((outDir+"/tmpMZZ_DataSigRegionWithBkgdFit_"+ssnxj.str()+"J"+leptType+"_Zoomed_log.eps").c_str());
    //can1->SaveAs((outDir+"/tmpMZZ_DataSigRegionWithBkgdFit_"+ssnxj.str()+"J"+leptType+"_Zoomed_log.png").c_str());
    
    delete can1; 
    // delete SIGdset;delete SBrescaleddset;delete SBdset;
    delete w1;
    delete f;
  }
  
  return 0;
}//end main
