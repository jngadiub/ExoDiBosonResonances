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
#include "TGaxis.h"
#include "TMath.h"
#include "CMSLabels.h"

#include "Math/QuantFuncMathCore.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"


//////////////
//
//    /afs/cern.ch/cms/slc5_amd64_gcc462/lcg/root/5.34.01-cms9/bin/root -b
//

double mySpecIntegral(TF1 *f1,TH1 *h1,double minVal, double maxVal);
void SetPoissonianErrors( TGraphAsymmErrors * g,TH1* h=NULL,bool ErrBinZeroContent=true);
Double_t simpleExpo(Double_t *x, Double_t *par)
   {
      Float_t xx =x[0]-par[1];
      Double_t f = (par[0])*TMath::Exp(par[2]*xx);//(par[0]/fabs(par[1]))*TMath::Exp(par[1]*xx);
      return f;
   }

Double_t levExpo(Double_t *x, Double_t *par)
   {
      Float_t xx =x[0]-par[1];
      Double_t f = (par[0])*TMath::Exp( (-1.0*xx) / (par[2]+par[3]*xx));//(par[0]/fabs(par[1]))*TMath::Exp(par[1]*xx);
      return f;
   }

void plotWithFit(){

  //steering
  string leptType[2]={"ELE","MU"};
  string purType[3]={"1JLP","1JHP","2J"};
  const double targetLumi=19538.0;//in pb

  /***
   //these normalizations use the range [650, inf] for the LP category:
  const double bkgNorm[2][3]={{198.278, 323.593, 1577.2},//ee1JLP, ee1JHP, ee2J
			      {328.374, 562.107, 2240.9}//mm1JLP, mm1JHP, mm2J			      
  };
  ***/


  //these normalizations use the range [500, inf] for the LP category:
  const double bkgNorm[2][3]={{408.2, 323.593, 1577.2},//ee1JLP, ee1JHP, ee2J
			      {671.4, 562.107, 2240.9}//mm1JLP, mm1JHP, mm2J			      
  };

  const double bkgPar1[2][3]={{190.2, 211.3, 85.94},//ee1JLP, ee1JHP, ee2J
			      {190.2, 211.3, 85.94}//mm1JLP, mm1JHP,mm2J
  };
  const double bkgPar2[2][3]={{0.023, -0.0013, 0.05976},//ee1JLP, ee1JHP
			      {0.023, -0.0013, 0.05976},//mm1JLP, mm1JHP
  };
  double fitLow=500;
  double fitHigh=2800;
  const bool mergeOB=true;

  //  take histos done by loopPlot and EDBRHistoMaker
  int ilep=0; int ipur=0;
  const int nDATA=4;//set to zero if you don't want to plot
  std::string dataLabelsMu[nDATA]={
     "DoubleMu_Run2012A_22Jan2013",
     "DoubleMuParked_Run2012B_22Jan2013",
     "DoubleMuParked_Run2012C_22Jan2013",
     "DoubleMuParked_Run2012D_22Jan2013",
  };

  std::string dataLabelsEle[nDATA]={
       "Photon_Run2012A_22Jan2013",
       "DoublePhotonHighPt_Run2012B_22Jan2013",
       "DoublePhotonHighPt_Run2012C_22Jan2013",
       "DoublePhotonHighPt_Run2012D_22Jan2013"
  };
   
  const int nMC=5;//set to zero if you don't want to plot
  std::string mcLabels[nMC]={"TTBARpowheg",
			     // "WW",
			     "WZ",
			     "ZZ",
			     //	     "DYJetsPt50To70",
			     "DYJetsPt70To100",
			     "DYJetsPt100"};

  const int nMCSig=1;//set to zero if you don't want to plot
  std::string mcLabelsSig[nMCSig]={"BulkG_ZZ_lljj_c0p2_M1000"};

  
  std::vector<int> fColorsMC;
  fColorsMC.push_back(kGreen-3);//ttbar
  //  fColorsMC.push_back(kMagenta-9); //WW
  fColorsMC.push_back(kMagenta-6); //WZ
  fColorsMC.push_back(kMagenta-3); //ZZ
     //  fColorsMC.push_back(kBlue-3); //DY pt 50-70
  fColorsMC.push_back(kBlue-6); //DY pt 70 -100
  fColorsMC.push_back(kBlue-9); //DY pt>100

  for(ilep=0;ilep<2;ilep++){
    for(ipur=0;ipur<3;ipur++){

      if(ipur==2)continue;
      //if(ipur==2)fitHigh=1400.0;
      // else fitHigh=2800.0;

      //if(ipur!=1)continue;
      //if(ilep!=1)continue;

      cout<<"Starting with lep="<<leptType[ilep]<<"  Pur="<<purType[ipur]<<endl;
      string inDir="plots_productionv2i_fullsig_"+purType[ipur]+"_"+leptType[ilep]+"/";
      //string inDir="plots_productionv2d_fullsig_"+purType[ipur]+"_"+leptType[ilep]+"_testDip/";
      

  //sum up data
  std::vector<TH1D*> datahistos;
  for(int i=0; i!= nDATA; ++i) {

    string dataLabelTMP=dataLabelsMu[i];
    if(ilep==0) dataLabelTMP=dataLabelsEle[i];
    string fDataName="histos_"+dataLabelTMP+".root";
   
    TFile *fData=new TFile((inDir+fDataName).c_str(),"READ");
    cout<<"Take data file from "<<(inDir+fDataName).c_str()<<endl;
    TH1D* histo = (TH1D*)(fData->Get("h_mZZ"));
    // cout<<"Data file has "<<histo->GetNbinsX()<<"  bins up to "<<histo->GetBinLowEdge(histo->GetNbinsX())<<endl;
    // histo->Rebin(2);

    TH1D* histoORIG=(TH1D*)histo->Clone("h_mZZ_STDErrs");
    TFile *ff1=new TFile("histo_withPoissonErrs.root","RECREATE");
    ff1->cd();
 
    // cout<<"Data File has bin20content="<<histo->GetBinContent(20) <<"  err on bin20="<<histo->GetBinErrorUp(20)<<flush;
    histo->SetBinErrorOption(TH1::kPoisson);//add poissonian errors (Garwood intervals); works only with ROOT >=5.33
    if(ipur==1&&ilep==1){//mu 1JHP
      histoORIG->Write();
      histo->Write();
    }
    delete ff1;
    fData->cd();  


    datahistos.push_back((TH1D*)histo->Clone(("h_mZZ_"+dataLabelTMP).c_str()));
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

  cout<<"Data File has bin28content="<<hData->GetBinContent(28) <<"  err on bin28="<<hData->GetBinErrorUp(28)/hData->GetBinWidth(28)<<flush;
  hData->SetBinErrorOption(TH1::kPoisson);
  TH1D* hDataORIG=(TH1D*)hData->Clone("h_mZZ_nonScaled");
  for(int j=1;j<=hData->GetNbinsX();j++){
    double binErrNew=hData->GetBinErrorUp(j)/hData->GetBinWidth(j); 
    double binContNew=hData->GetBinContent(j)/hData->GetBinWidth(j);

    //    double binErrUpNew=hData->GetBinErrorUp(j)/hData->GetBinWidth(j); 
    // double binErrLowNew=hData->GetBinErrorLow(j)/hData->GetBinWidth(j); 
    if(j>28)cout<<"Data binCenter="<<hData->GetBinCenter(j)<<" has old="<<hData->GetBinContent(j)<<"  new="<<binContNew<<"  newErr="<<binErrNew<<endl;
    hData->SetBinContent(j,binContNew);
    //    hData->SetBinError(j,binErrNew);
    hData->SetBinError(j,0.0);//we draw errors via the TGraph gData


  }

  cout<<"  NEW err on bin28="<<hData->GetBinErrorUp(28)<<endl;
  cout<<"Area of DataTOT is "<<hData->Integral("width")<<endl;

 //we are in ROOT 5.32, we must use a work around
  TGraphAsymmErrors * gData = new TGraphAsymmErrors(hData);
  /*    cout<<"Filling TGraph: "<<endl;
  for (int i = 0; i < g->GetN(); ++i) {
    gData->SetPointEYlow(i, hData->GetBinErrorLow(i+1));
    gData->SetPointEYhigh(i,hData->GetBinErrorUp(i+1) );
    gData->SetPoint(i, hData->GetBinContent(i+1));
    cout<<"i="<<i<<"  "<<gData->GetPoint(i)<<" +"<<  gData->GetPointEYhigh(i)<<"  -"<<  gData->GetPointEYlow(i)<<endl;
    }*/

  SetPoissonianErrors(gData,hDataORIG, true);

  //stack MC bkgds
  cout<<"Starting loop on MC bkgd"<<endl;
  THStack* hs = new THStack("hs","");
  double bkgdMCIntegral=0.0;
  std::vector<TH1D*> mchistos;
  TH1D* histoDYMC=NULL;
  TH1D* histoOBMC=NULL;//OB--> Other backgrounds, not DY
  bool initDYMC=false,initOBMC=false;
  for(int i=0; i!= nMC; ++i) {
    TFile *fMC=new TFile((inDir+"histos_"+mcLabels[i]+".root").c_str(),"READ");
    cout<<"Take MC file from "<<(inDir+"histos_"+mcLabels[i]+".root").c_str()<<endl;
    TH1D* histo = (TH1D*)(fMC->Get("h_mZZ"));
    // cout<<"histoMC has "<<histo->GetNbinsX()<<"  bins up to "<<histo->GetBinLowEdge(histo->GetNbinsX())<<endl;
    // cout<<"histoMC has "<<histo->GetEntries()<<" entries  "<<flush;
    // histo->Rebin(2);
    histo->SetFillColor(fColorsMC.at(i));

    for(int j=1;j<=histo->GetNbinsX();j++){
      double binContNew=histo->GetBinContent(j)/histo->GetBinWidth(j);
      //if(j>28)cout<<"MC binCenter="<<histo->GetBinCenter(j)<<" has old="<<histo->GetBinContent(j)<<"  new="<<binContNew<<endl;
      histo->SetBinContent(j,binContNew);
      histo->SetBinError(j,0.0);
    }
    cout<<"Old integral="<<histo->Integral("width")<<flush;
    histo->Scale(targetLumi);
    cout<<"  New integral="<<histo->Integral("width")<<flush;
    bkgdMCIntegral+=histo->Integral();
    mchistos.push_back((TH1D*)histo->Clone(("h_mZZ_"+mcLabels[i]).c_str()));

    if(mergeOB){
      if(mcLabels[i].find("DY")!=std::string::npos){
	if(!initDYMC ){
	  histoDYMC= ((TH1D*)mchistos.at(i)->Clone("MCDY"));
	  histoDYMC->Reset();
	  initDYMC=true;
	}
	histoDYMC->Add(mchistos.at(i));
      }//end if it is DY MC
      else{//other background, not DY
	if(!initOBMC ){
	  histoOBMC= ((TH1D*)mchistos.at(i)->Clone("MCOB"));
	  histoOBMC->Reset();
	  initOBMC=true;
	}
	histoOBMC->Add(mchistos.at(i));
      }//end else it is other bkgd
    }//end if clump together non-DY bkgds
    else{
      cout<<"\tAdd to stack"<<endl;
      hs->Add(mchistos.at(i));
    }  
    //delete histo;
    //delete fMC;
  }//end loop over MC bkgd


  if(mergeOB){
    histoOBMC->SetFillColor(kGreen-3);
    histoDYMC->SetFillColor(kBlue-9);
    hs->Add(histoOBMC);
    hs->Add(histoDYMC);
  }

  cout<<"Finished loop on MC. Total area of stacked MC backgrounds: "<<bkgdMCIntegral<<endl;


  TH1D *hMCTOT=NULL;
  if(mchistos.size() !=0) {
    hMCTOT = ((TH1D*)mchistos.at(0)->Clone("MCTOT"));
    hMCTOT->Reset();
  }  
  for(int i=0; i!= nMC; ++i) {
    hMCTOT->Add(mchistos.at(i));
  }
  cout<<"Area of total bkgd MC : "<<hMCTOT->Integral("width")<<endl;




 //take MCsig
 std::vector<TH1D*> sighistos;
  for(int i=0; i!= nMCSig; ++i) {
    TFile *fMCSig=new TFile((inDir+"histos_"+mcLabelsSig[i]+".root").c_str(),"READ");
    TH1D* histo = (TH1D*)(fMCSig->Get("h_mZZ"));
    // histo->Rebin(2);
    histo->SetLineColor(kOrange+1);
    histo->SetFillColor(kOrange+1);
    histo->SetFillStyle(3004);
    for(int j=1;j<=histo->GetNbinsX();j++){
      double binContNew=histo->GetBinContent(j)/histo->GetBinWidth(j);
      histo->SetBinContent(j,binContNew);
      histo->SetBinError(j,0.0);
    }
    const double sfKtilde=6.21777;// ratio of xsect of BulkG wioth k=0.5 and k=0.2 at M=1000;
    histo->Scale(targetLumi*100.0*sfKtilde);
    sighistos.push_back((TH1D*)histo->Clone(("h_mZZ_"+mcLabelsSig[i]).c_str()));
  }

  TF1 *fit1=0;
  /*
  if(ipur==1){//1JHP
  //Fit function is a simple expo
  fit1=new TF1("fitfunc1",simpleExpo,fitLow,fitHigh,3);
  fit1->SetParName(0,"Normalization");
  fit1->SetParName(1,"Shift");
  fit1->SetParName(2,"Slope");
  fit1->SetParameter(0,1.0);
  fit1->SetParameter(1,0.0);//fitLow);
  fit1->SetParameter(2,bkgPar1[ilep][ipur]);
  cout<<"Simple expo params (1): "<<fit1->GetParName(0)<<"="<<fit1->GetParameter(0)<<"   "<<fit1->GetParName(1)<<"="<<fit1->GetParameter(1)<<"  "<<fit1->GetParName(2)<<"="<<fit1->GetParameter(2)<<endl;

  double fitValLow=fit1->Eval(fitLow);
  double fitValHigh=fit1->Eval(fitHigh);
  double fit1Int0=fit1->Integral(fitLow,fitHigh);//
  double deltaMZZ=fitHigh-fitLow;
  double fit1Int0b=(fitValLow-fitValHigh)/fabs(fit1->GetParameter(2));//+fitValHigh*deltaMZZ;
  cout<<"Integral with TF1="<<fit1Int0<<"  Integral by hand="<<fit1Int0b<<endl;
  fit1->SetParameter(0,bkgNorm[ilep][ipur]/fit1Int0b);
 
  cout<<"Simple expo params (2): "<<fit1->GetParName(0)<<"="<<fit1->GetParameter(0)<<"   "<<fit1->GetParName(2)<<"="<<fit1->GetParameter(2)<<endl;
  cout<<"Integral of fit func is "<<fit1->Integral(fitLow,fitHigh)<<endl;
 cout<<"BACKGROUND INTEGRAL with TF1 built in. Before rescaling:"<< fit1Int0<<"  after:"<<fit1->Integral(fitLow,fitHigh)<<endl;

  }//end if ipur==1
  else{//1JLP or 2J
  */
    //Fit function is a simple expo
    fit1=new TF1("fitfunc2",levExpo,fitLow,fitHigh,4);
    fit1->SetParName(0,"Normalization");
    fit1->SetParName(1,"Shift");
    fit1->SetParName(2,"Sigma");
    fit1->SetParName(3,"P0");
    fit1->SetParameter(0,1.0);
    fit1->SetParameter(1,490.0);//fitLow);
    fit1->SetParameter(2,bkgPar1[ilep][ipur]);
    fit1->SetParameter(3,bkgPar2[ilep][ipur]);
    cout<<"Lev expo params (1): "<<fit1->GetParName(0)<<"="<<fit1->GetParameter(0)<<"   "<<fit1->GetParName(1)<<"="<<fit1->GetParameter(1)<<"  "<<fit1->GetParName(2)<<"="<<fit1->GetParameter(2)<<"  "<<fit1->GetParName(3)<<"="<<fit1->GetParameter(3)<<endl;

    double fitValLow=fit1->Eval(fitLow);
    double fitValHigh=fit1->Eval(fitHigh);
    double fit1Int0=fit1->Integral(fitLow,fitHigh);//
    double deltaMZZ=fitHigh-fitLow;
    double fit1Int0b=(fitValLow-fitValHigh)/fabs(fit1->GetParameter(2));//+fitValHigh*deltaMZZ;
    cout<<"Integral with TF1="<<fit1Int0<<"  Integral by hand="<<fit1Int0b<<endl;
    fit1->SetParameter(0,bkgNorm[ilep][ipur]/fit1Int0);
 
    cout<<"Lev expo params (2): "<<fit1->GetParName(0)<<"="<<fit1->GetParameter(0)<<"   "<<fit1->GetParName(2)<<"="<<fit1->GetParameter(2)<<"   "<<fit1->GetParName(3)<<"="<<fit1->GetParameter(3)<<endl;
    cout<<"Integral of fit func is "<<fit1->Integral(fitLow,fitHigh)<<endl;
    cout<<"BACKGROUND INTEGRAL with TF1 built in. Before rescaling:"<< fit1Int0<<"  after:"<<fit1->Integral(fitLow,fitHigh)<<endl;

    //  }//end if ipur different from 1

  //find max
  double maxData=hData->GetBinContent(hData->GetMaximumBin());
  double maxMC=hMCTOT->GetBinContent(hMCTOT->GetMaximumBin());
  double mymax=maxData;
  if(maxMC>maxData)mymax=maxMC;
  if(ipur==2)mymax*=2.5;//1.45;
  else mymax*=1.65;
  double mymin=0.0004;

  string canName="plot_mzz_"+leptType[ilep]+purType[ipur];
  TCanvas *c1=new TCanvas(canName.c_str(),canName.c_str(),900,900);
  c1->cd();
  gPad->SetRightMargin(0.07);
  gPad->SetTopMargin(0.08);
  gPad->SetLeftMargin(-0.03);
  hData->GetXaxis()->SetTitle("M_{ZZ} [GeV]");
  hData->GetYaxis()->SetTitle("Events / GeV");

  hData->SetMarkerStyle(20);
  hData->SetMarkerSize(2.);
  hData->SetMarkerColor(kBlack);
  hData->SetMinimum(mymin);
  hData->SetMaximum(mymax);

  //need TGraph for Poissonian errors
  gData->GetXaxis()->SetTitle("M_{ZZ} [GeV]");
  gData->GetYaxis()->SetTitle("Events / GeV");
  gData->SetMarkerStyle(20);
  gData->SetMarkerSize(2.);
  gData->SetMarkerColor(kBlack);
  gData->SetMinimum(mymin);
  gData->SetMaximum(mymax);

  hs->SetMinimum(mymin);
  hs->SetMaximum(mymax);
 

  hs->Draw("HIST");
  if(ipur==2)hs->GetXaxis()->SetRangeUser(0.0,1370.0);
  else hs->GetXaxis()->SetLimits(500.0,3000.0);
  //hs->GetXaxis()->SetNdivisions(510,kTRUE);
  hs->GetXaxis()->SetTitle("M_{ZZ} [GeV]");
  hs->GetYaxis()->SetTitle("Events / GeV"); 
  hs->GetXaxis()->SetLabelSize(0.032);
  hs->GetYaxis()->SetLabelSize(0.032);
  hs->GetXaxis()->SetTitleSize(0.045);
  hs->GetYaxis()->SetTitleSize(0.045);
  hs->GetYaxis()->SetTitleOffset(1.15);
  hs->GetYaxis()->CenterTitle(); 
  cout<<"Signal integral="<<sighistos.at(0)->Integral()<<endl;
  hs->GetXaxis()->SetRangeUser(fitLow,fitHigh);
  sighistos.at(0)->Draw("HISTsame");
  gData->Draw("E0Psame");
  // hData->Draw("P0same");
  fit1->SetLineColor(kRed);
  fit1->SetLineWidth(3.5);
  fit1->Draw("Lsame");

  TLegend *l1=new TLegend(0.362,0.65,0.88,0.88);
  char type[32];
  if(ilep==0&&ipur==0)sprintf(type,"ee, 1JLP");
  else if(ilep==0&&ipur==1)sprintf(type,"ee, 1JHP");
  else if(ilep==1&&ipur==0)sprintf(type,"#mu#mu, 1JLP");
  else if(ilep==1&&ipur==1)sprintf(type,"#mu#mu, 1JHP");
  else if(ilep==0&&ipur==2)sprintf(type,"ee, 2J");
  else if(ilep==1&&ipur==2)sprintf(type,"#mu#mu, 2J");
  else sprintf(type,"UNKNOWN");


  char legEntry[128];
  sprintf(legEntry,"CMS 2012 (%s)",type);
  l1->AddEntry(gData,legEntry,"PE");
  l1->AddEntry(fit1,"Background estimation","L");
  if(mergeOB){
    l1->AddEntry(histoDYMC,"Z+jets (Madgraph)","F");
    l1->AddEntry(histoOBMC,"Other Backgrounds (t#bar{t}, VV)","F");
  }
  else{
    l1->AddEntry(mchistos.at(nMC-1),"Z+jets (Madgraph)","F");
    //    l1->AddEntry(mchistos.at(2),"WW (Pythia)","F");
    l1->AddEntry(mchistos.at(2),"WZ (Pythia)","F");
    l1->AddEntry(mchistos.at(3),"ZZ (Pythia)","F");
    l1->AddEntry(mchistos.at(0),"t#bar{t} (POWHEG)","F");
  }
  l1->AddEntry(sighistos.at(0),"Bulk G* (M_{G*}=1000 GeV, #tilde{k}=0.5 (x100) )","F");
  l1->SetTextFont(42);
  l1->SetTextSize(0.0275);
  l1->SetFillColor(kWhite);
  l1->SetBorderSize(0);
  l1->Draw();

  gPad->RedrawAxis();

  // Nice labels
  /*
  TLatex* txt1 = makeCMSPreliminaryTop(8,0.10,0.945);
  txt1->Draw();
  txt1 = makeCMSLumi(19.7,0.75,0.945);
   txt1->Draw();
  */

  TLatex* txt1 = makeCMSFinalTop(0.10,0.945);
  txt1->Draw();
  txt1 = makeCMSLumi(8,19.7,0.58,0.945);
  txt1->Draw();

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


void SetPoissonianErrors( TGraphAsymmErrors * g, TH1* h,bool ErrBinZeroContent){


  const double alpha = 1 - 0.6827;

  bool t0=false,t1=false,t2=false,t3=false;
  for (int i = 0; i < g->GetN(); ++i) {
    int N = g->GetY()[i];

    if(h!=NULL)N=h->GetBinContent(i+1);


    double L =  (N==0) ? 0  : (ROOT::Math::gamma_quantile(alpha/2,N,1.));
    double U =  ROOT::Math::gamma_quantile_c(alpha/2,N+1,1) ;

    double SF= 1.0/h->GetBinWidth(i+1) ;

    //cout<<"TGraph "<<g->GetName()<<"  Point i="<<i<<" at MZZ="<< g->GetX()[i]<<"  Cont="<< g->GetY()[i]<<"  OldErrLow="<<g->GetErrorYlow(i)<<"  OldErrHigh="<< g->GetErrorYhigh(i)<<endl;
    //  if(!t0&&N==0){ cout<<"N==0 ; NewLowErr="<<N-L<<"  NewHighErr="<<U-N<<endl;t0=true;}
    // if(!t1&&N==1){ cout<<"N==1 ; NewLowErr="<<N-L<<"  NewHighErr="<<U-N<<endl;t1=true;}
    // if(!t2&&N==2){ cout<<"N==2 ; NewLowErr="<<N-L<<"  NewHighErr="<<U-N<<endl;t2=true;}
    // if(!t3&&N==3){ cout<<"N==3 ; NewLowErr="<<N-L<<"  NewHighErr="<<U-N<<endl;t3=true;}

    double errLowNew=(N-L)*SF;
    double errHighNew=(U-N)*SF;
    if(!ErrBinZeroContent && N==0){errLowNew=0.0;errHighNew=0.0;}
    g->SetPointEYlow(i, errLowNew);
    g->SetPointEYhigh(i, errHighNew);

    //cout<<"TGraph "<<g->GetName()<<"  Point i="<<i<<" at MZZ="<< g->GetX()[i]<<"  Cont="<< g->GetY()[i]<<"  NewErrLow="<<(N-L)*SF<<"  NewErrHigh="<< (U-N)*SF<<endl;
  }

}
