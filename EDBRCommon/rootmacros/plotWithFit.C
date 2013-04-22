#include <map>
#include <vector>
#include <string>
#include <Riostream.h>

#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TVectorD.h"
#include "TGraph.h"
#include "TMath.h"
#include "CMSLabels.h"

double mySpecIntegral(TF1 *f1,TH1 *h1,double minVal, double maxVal);
Double_t simpleExpo(Double_t *x, Double_t *par)
   {
      Float_t xx =x[0]-par[2];
      Double_t f = (par[0])*TMath::Exp(par[1]*xx);//(par[0]/fabs(par[1]))*TMath::Exp(par[1]*xx);
      return f;
   }

void plotWithFit(){

  //steering
  string leptType[2]={"ELE","MU"};
  string purType[2]={"1JLP","1JHP"};
  const double targetLumi=19538.0;//in pb

  const double bkgNorm[2][2]={{263, 221},//ee1JLP, ee1JHP
			      {410, 302}//mm1JLP, mm1JHP
  };
  
  const double bkgPar1[2][2]={{-4.6E-03,-5.2E-03},//ee1JLP, ee1JHP
			      {-4.6E-03,-5.2E-03}//mm1JLP, mm1JHP
  };
  double fitLow=600;
  double fitHigh=2400;


  //  take histos done by loopPlot and EDBRHistoMaker
  int ilep=0; int ipur=0;
  const int nDATA=12;//set to zero if you don't want to plot
  std::string dataLabels[nDATA]={"DoubleMu_Run2012A_13Jul2012",
				 "DoubleMu_Run2012A_recover",
  				 "DoubleMu_Run2012B_13Jul2012",
				 "DoubleMu_Run2012C_24Aug2012",
  				 "DoubleMu_Run2012C_PRv2",
  				 "DoubleMu_Run2012D_PRv1","Photon_Run2012A_13Jul2012",
  				 "Photon_Run2012A_recover",
  				 "DoublePhotonHighPt_Run2012B_13Jul2012",
				 "DoublePhotonHighPt_Run2012C_24Aug2012",
				 "DoublePhotonHighPt_Run2012C_PRv2",
				 "DoublePhotonHighPt_Run2012D_PRv1"
				 };
   
  const int nMC=7;//set to zero if you don't want to plot
  std::string mcLabels[nMC]={"TTBARpowheg",
			     "WW",
			     "WZ",
			     "ZZ",
			     "DYJetsPt50To70",
			     "DYJetsPt70To100",
			     "DYJetsPt100"};

  const int nMCSig=1;//set to zero if you don't want to plot
  std::string mcLabelsSig[nMCSig]={"BulkG_ZZ_lljj_c0p2_M1600"};

  
  std::vector<int> fColorsMC;
  fColorsMC.push_back(kGreen-3);
  fColorsMC.push_back(kMagenta-9);
  fColorsMC.push_back(kMagenta-6);
  fColorsMC.push_back(kMagenta-3);
  fColorsMC.push_back(kBlue-3);
  fColorsMC.push_back(kBlue-6);
  fColorsMC.push_back(kBlue-9);

  for(ilep=0;ilep<2;ilep++){
    for(ipur=0;ipur<2;ipur++){

  cout<<"Starting with lep="<<leptType[ilep]<<"  Pur="<<purType[ipur]<<endl;
  string inDir="plots_productionv1d_fullsig_"+purType[ipur]+"_"+leptType[ilep]+"/";


  //sum up data
  std::vector<TH1D*> datahistos;
  for(int i=0; i!= nDATA; ++i) {
    TFile *fData=new TFile((inDir+"histos_"+dataLabels[i]+".root").c_str(),"READ");
    cout<<"Take data file from "<<(inDir+"histos_"+dataLabels[i]+".root").c_str()<<endl;
    TH1D* histo = (TH1D*)(fData->Get("h_mZZ"));
    histo->Rebin(2);
    for(int j=1;j<=histo->GetNbinsX();j++){
      double binContNew=histo->GetBinContent(j)/histo->GetBinWidth(j);
      double binErrNew=histo->GetBinError(j)/histo->GetBinWidth(j); 
      histo->SetBinContent(j,binContNew);
      histo->SetBinError(j,binErrNew);
    }

    datahistos.push_back((TH1D*)histo->Clone(("h_mZZ_"+dataLabels[i]).c_str()));
  // delete histo;
  //  delete fData;
  }//end loop on data files
  cout<<"Finished loop on data"<<endl;
  TH1D *hData=NULL;
  if(datahistos.size() !=0) {
    hData = ((TH1D*)datahistos.at(0)->Clone("DataTOT"));
    hData->Reset();
 }
  for(int i=0; i!= nDATA; ++i) {
    hData->Add(datahistos.at(i));
  }
  cout<<"Area of DataTOT is "<<hData->Integral()<<endl;
  //stack MC bkgds
  cout<<"Starting loop on MC bkgd"<<endl;
  THStack* hs = new THStack("hs","");
  std::vector<TH1D*> mchistos;
  for(int i=0; i!= nMC; ++i) {
    TFile *fMC=new TFile((inDir+"histos_"+mcLabels[i]+".root").c_str(),"READ");
    cout<<"Take MC file from "<<(inDir+"histos_"+mcLabels[i]+".root").c_str()<<endl;
    TH1D* histo = (TH1D*)(fMC->Get("h_mZZ"));
    cout<<"histoMC has "<<histo->GetEntries()<<" entries  "<<flush;
    histo->Rebin(2);
    histo->SetFillColor(fColorsMC.at(i));

    for(int j=1;j<=histo->GetNbinsX();j++){
      double binContNew=histo->GetBinContent(j)/histo->GetBinWidth(j);
      histo->SetBinContent(j,binContNew);
      histo->SetBinError(j,0.0);
    }
    cout<<"Old integral="<<histo->Integral()<<flush;
    histo->Scale(targetLumi);
    cout<<"  New integral="<<histo->Integral()<<flush;
    mchistos.push_back((TH1D*)histo->Clone(("h_mZZ_"+mcLabels[i]).c_str()));
    cout<<"\tAdd to stack"<<endl;
    hs->Add(mchistos.at(i));
  
    //delete histo;
    //delete fMC;
  }//end loop over MC bkgd
 cout<<"Finished loop on MC"<<endl;
  TH1D *hMCTOT=NULL;
  if(mchistos.size() !=0) {
    hMCTOT = ((TH1D*)mchistos.at(0)->Clone("MCTOT"));
    hMCTOT->Reset();
  }  
 for(int i=0; i!= nMC; ++i) {
   hMCTOT->Add(mchistos.at(i));
    }

 //take MCsig
 std::vector<TH1D*> sighistos;
  for(int i=0; i!= nMCSig; ++i) {
    TFile *fMCSig=new TFile((inDir+"histos_"+mcLabelsSig[i]+".root").c_str(),"READ");
    TH1D* histo = (TH1D*)(fMCSig->Get("h_mZZ"));
    histo->Rebin(2);
    histo->SetLineColor(kOrange+1);
    histo->SetFillColor(kOrange+1);
    histo->SetFillStyle(3004);
    for(int j=1;j<=histo->GetNbinsX();j++){
      double binContNew=histo->GetBinContent(j)/histo->GetBinWidth(j);
      histo->SetBinContent(j,binContNew);
      histo->SetBinError(j,0.0);
    }
    histo->Scale(targetLumi*1000.0);
    sighistos.push_back((TH1D*)histo->Clone(("h_mZZ_"+mcLabelsSig[i]).c_str()));

  }
  //Fit function is a simple expo

  TF1 *fit1=new TF1("fitfunc1",simpleExpo,fitLow,fitHigh,2);
  fit1->SetParName(0,"Normalization");
  fit1->SetParName(1,"Slope");
  fit1->SetParName(2,"Shift");
  fit1->SetParameter(0,1.0);
  fit1->SetParameter(1,bkgPar1[ilep][ipur]);
  fit1->SetParameter(2,0.0);//fitLow);
  cout<<"Simple expo params (1): "<<fit1->GetParName(0)<<"="<<fit1->GetParameter(0)<<"   "<<fit1->GetParName(1)<<"="<<fit1->GetParameter(1)<<endl;

  double fitValLow=fit1->Eval(fitLow);
  double fitValHigh=fit1->Eval(fitHigh);
  double fit1Int0=fit1->Integral(fitLow,fitHigh);//
  double deltaMZZ=fitHigh-fitLow;
  double fit1Int0b=(fitValLow-fitValHigh)/fabs(fit1->GetParameter(1));//+fitValHigh*deltaMZZ;
  cout<<"Integral by hand="<<fit1Int0b<<endl;
  fit1->SetParameter(0,bkgNorm[ilep][ipur]/fit1Int0b);
 
  cout<<"Simple expo params (2): "<<fit1->GetParName(0)<<"="<<fit1->GetParameter(0)<<"   "<<fit1->GetParName(1)<<"="<<fit1->GetParameter(1)<<endl;
  cout<<"Integral of fit func is "<<fit1->Integral(fitLow,fitHigh)<<endl;
 cout<<"BACKGROUND INTEGRAL with TF1 built in. Before rescaling:"<< fit1Int0<<"  after:"<<fit1->Integral(fitLow,fitHigh)<<endl;
  //find max
  double maxData=hData->GetBinContent(hData->GetMaximumBin());
  double maxMC=hMCTOT->GetBinContent(hMCTOT->GetMaximumBin());
  double mymax=maxData;
  if(maxMC>maxData)mymax=maxMC;
  mymax*=1.15;
  double mymin=0.002;

  string canName="plot_mzz_"+leptType[ilep]+purType[ipur];
  TCanvas *c1=new TCanvas(canName.c_str(),canName.c_str(),900,900);
  c1->cd();
  gPad->SetRightMargin(0.07);
  gPad->SetTopMargin(0.08);
  gPad->SetLeftMargin(-0.03);
  hData->GetXaxis()->SetTitle("m_{ZZ} [GeV]");
  hData->GetYaxis()->SetTitle("events / GeV");

  hData->SetMarkerStyle(20);
  hData->SetMarkerSize(2.);
  hData->SetMarkerColor(kBlack);
  hData->SetMinimum(mymin);
  hData->SetMaximum(mymax);
  hs->SetMinimum(mymin);
  hs->SetMaximum(mymax);
 
  hs->Draw("HIST");
  hs->GetXaxis()->SetTitle("m_{ZZ} [GeV]");
  hs->GetYaxis()->SetTitle("events / GeV"); 
  hs->GetXaxis()->SetLabelSize(0.035);
  hs->GetYaxis()->SetLabelSize(0.035);
  hs->GetXaxis()->SetTitleSize(0.045);
  hs->GetYaxis()->SetTitleSize(0.045);
  hs->GetYaxis()->SetTitleOffset(1.15);
  hs->GetYaxis()->CenterTitle(); 
  cout<<"Signal integral="<<sighistos.at(0)->Integral()<<endl;
  sighistos.at(0)->Draw("HISTsame");
  hData->Draw("E1Psame");
  fit1->SetLineColor(kRed);
  fit1->SetLineWidth(3.5);
  fit1->Draw("Lsame");

  TLegend *l1=new TLegend(0.40,0.65,0.85,0.88);
  char type[32];
  if(ilep==0&&ipur==0)sprintf(type,"ee, 1JLP");
  else if(ilep==0&&ipur==1)sprintf(type,"ee, 1JHP");
  else if(ilep==1&&ipur==0)sprintf(type,"mm, 1JLP");
  else if(ilep==1&&ipur==1)sprintf(type,"mm, 1JHP");
  else sprintf(type,"UNKNOWN");

  char legEntry[128];
  sprintf(legEntry,"CMS 2012 (%s)",type);
  l1->AddEntry(hData,legEntry,"P");
  l1->AddEntry(fit1,"Background estimation","L");
  l1->AddEntry(mchistos.at(nMC-1),"Z+jets (Madgraph)","F");
  l1->AddEntry(mchistos.at(2),"WZ (Pythia)","F");
  l1->AddEntry(mchistos.at(3),"ZZ (Pythia)","F");
  l1->AddEntry(mchistos.at(0),"t#bar{t} (POWHEG)","F");
  l1->AddEntry(sighistos.at(0),"Bulk G (M_{1}=1600 GeV, #tilde{k}=0.2 (x1000) )","F");
  l1->SetFillColor(kWhite);
  l1->SetBorderSize(0);
  l1->Draw();

  gPad->RedrawAxis();

  // Nice labels
  TLatex* txt1 = makeCMSPreliminaryTop(8,0.10,0.945);
  txt1->Draw();
  txt1 = makeCMSLumi(19.5,0.75,0.945);
  txt1->Draw();
  //  l = makeChannelLabel(wantNXJets_,flavour_,isZZchannel_);
  //l->Draw();


  char outName[128];
  sprintf(outName,"%s/%s_withFit.root",inDir.c_str(),canName.c_str());
  c1->SaveAs(outName);
  sprintf(outName,"%s/%s_withFit.pdf",inDir.c_str(),canName.c_str());
  c1->SaveAs(outName);
  sprintf(outName,"%s/%s_withFit.png",inDir.c_str(),canName.c_str());
  c1->SaveAs(outName);
  sprintf(outName,"%s/%s_withFit.eps",inDir.c_str(),canName.c_str());
  c1->SaveAs(outName);

  gPad->SetLogy();
  sprintf(outName,"%s/%s_withFit_log.root",inDir.c_str(),canName.c_str());
  c1->SaveAs(outName);
  sprintf(outName,"%s/%s_withFit_log.pdf",inDir.c_str(),canName.c_str());
  c1->SaveAs(outName);
  sprintf(outName,"%s/%s_withFit_log.png",inDir.c_str(),canName.c_str());
  c1->SaveAs(outName);
  sprintf(outName,"%s/%s_withFit_log.eps",inDir.c_str(),canName.c_str());
  c1->SaveAs(outName);

  delete hs;
  delete hData;
  delete fit1;
  delete c1;
    }//end loop on ipur
  }//end loop on ilep


}//end main

double mySpecIntegral(TF1 *f1,TH1 *h1,double minVal, double maxVal){

  double myInt=0.0, myHistInt=0.0;
  for(int ib=1;ib<h1->GetNbinsX();ib++){
    
    double h1cont=h1->GetBinContent(ib);
    double cent=h1->GetBinCenter(ib);
    double lowedge=h1->GetBinLowEdge(ib);
    double width=h1->GetBinWidth(ib);
    if(lowedge<minVal || lowedge+width>maxVal)continue;
    double f1cont=f1->Eval(cent);
    myInt+=f1cont;
    myHistInt+=h1cont;
  }//end loop on bins

  cout<<"mySpecIntegral("<<f1->GetName()<<" , "<<h1->GetName()<<"): TF1 int ="<<myInt<<"  TH1 int="<<myHistInt<<endl;
  return myInt;
}
