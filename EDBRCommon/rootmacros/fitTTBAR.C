#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"

// Quadratic background function
Double_t background(Double_t *x, Double_t *par) {
   return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}

Double_t gaussianPeak(Double_t *x, Double_t *par)
// The signal function: a gaussian
{
   Double_t arg = 0;
   if (par[1]) arg = (x[0] - par[2])/par[1];

   Double_t sig = par[0]*TMath::Exp(-0.5*arg*arg);
   return sig;
}

// Sum of background and peak function
Double_t fitFunction(Double_t *x, Double_t *par) {
  return background(x,par) + gaussianPeak(x,&par[3]);
}

void fitTTBAR(){

  gROOT->Reset();
  gStyle->SetOptFit(1);

  //###### EDIT THIS PART #####

  bool IsData = false; // true = Data , false = MC

  double XminFit = 50;
  double XmaxFit = 130;

  double XminSignalRegion = 70;
  double XmaxSignalRegion = 100;

  //Input files
  TFile *_file_noMassCut_noNsubjettinessCut = TFile::Open("WW_mu_ttbarControlRegion_CA8/root/can_h_mJJ.root");
  TFile *_file_noMassCut_yesNsubjettinessCut = TFile::Open("WW_mu_ttbarControlRegion_nsubjettiness0p4_CA8/root/can_h_mJJ.root");

  //###########################

  //Read the canvas
  TCanvas* cv_h_mJJ_noCut;
  cv_h_mJJ_noCut = (TCanvas*) _file_noMassCut_noNsubjettinessCut->Get( "cv_h_mJJ" );
  TCanvas* cv_h_mJJ_WithCut;
  cv_h_mJJ_WithCut = (TCanvas*) _file_noMassCut_yesNsubjettinessCut->Get( "cv_h_mJJ" );

  //Get the histograms from the canvas
  THStack* histoStack_noCut = (THStack*) cv_h_mJJ_noCut->FindObject( "hs" );
  TH1D* histoData_noCut = (TH1D*) cv_h_mJJ_noCut->FindObject( "masterDATA" );
  TH1D* histoTotBkg_noCut = (TH1D*) histoStack_noCut->GetStack()->Last();
  histoData_noCut->SetName("Data");
  histoTotBkg_noCut->SetName("MC Bkg");
  histoTotBkg_noCut->SetFillColor(kGreen);
  histoData_noCut->SetTitle("");
  histoTotBkg_noCut->SetTitle("");
  histoData_noCut->GetYaxis()->SetTitleOffset(0.8);
  histoTotBkg_noCut->GetYaxis()->SetTitleOffset(0.8);
  histoData_noCut->GetXaxis()->SetTitleOffset(0.8);
  histoTotBkg_noCut->GetXaxis()->SetTitleOffset(0.8);
  histoData_noCut->GetXaxis()->SetTitle("Pruned jet mass (GeV)");
  histoTotBkg_noCut->GetXaxis()->SetTitle("Pruned jet mass (GeV)");
  //###
  THStack* histoStack_WithCut = (THStack*) cv_h_mJJ_WithCut->FindObject( "hs" );
  TH1D* histoData_WithCut = (TH1D*) cv_h_mJJ_WithCut->FindObject( "masterDATA" );
  TH1D* histoTotBkg_WithCut = (TH1D*) histoStack_WithCut->GetStack()->Last();
  histoData_WithCut->SetName("Data");
  histoTotBkg_WithCut->SetName("MC Bkg");
  histoTotBkg_WithCut->SetFillColor(kGreen);
  histoData_WithCut->SetTitle("");
  histoTotBkg_WithCut->SetTitle("");
  histoData_WithCut->GetYaxis()->SetTitleOffset(0.8);
  histoTotBkg_WithCut->GetYaxis()->SetTitleOffset(0.8);
  histoData_WithCut->GetXaxis()->SetTitleOffset(0.8);
  histoTotBkg_WithCut->GetXaxis()->SetTitleOffset(0.8);
  histoData_WithCut->GetXaxis()->SetTitle("Pruned jet mass (GeV)");
  histoTotBkg_WithCut->GetXaxis()->SetTitle("Pruned jet mass (GeV)");

  //Create new canvas for the fits
  TCanvas c_noCut;
  TCanvas c_WithCut;

  if(IsData==true)
    {
      c_noCut.cd();
      histoData_noCut->Draw("E"); 
      c_WithCut.cd();
      histoData_WithCut->Draw("E"); 
    }
  if(IsData==false)
    {
      c_noCut.cd();
      histoTotBkg_noCut->Draw("E");
      c_WithCut.cd();
      histoTotBkg_WithCut->Draw("E");
    }

  // create a TF1 with the range from 0 to 3 and 6 parameters
   TF1 *fitFcn_noCut = new TF1("fitFcn_noCut",fitFunction,XminFit,XmaxFit,6);
   fitFcn_noCut->SetNpx(500);
   fitFcn_noCut->SetLineWidth(4);
   fitFcn_noCut->SetLineColor(kMagenta);
   fitFcn_noCut->SetParameters(1,1,1,1,1,1);
   fitFcn_noCut->SetParameter(0,10); 
   fitFcn_noCut->SetParLimits(0,-200,200); 
   fitFcn_noCut->SetParameter(1,10); 
   fitFcn_noCut->SetParLimits(1,0,500);
   fitFcn_noCut->SetParameter(2,-1); 
   fitFcn_noCut->SetParLimits(2,0,-30); 
   fitFcn_noCut->SetParameter(3,10); 
   fitFcn_noCut->SetParLimits(3,0,1000); 
   fitFcn_noCut->SetParameter(4,5); 
   fitFcn_noCut->SetParLimits(4,3,30);   
   fitFcn_noCut->SetParameter(5,80);   
   fitFcn_noCut->SetParLimits(5,75,90);  

   TF1 *fitFcn_WithCut = new TF1("fitFcn_WithCut",fitFunction,XminFit,XmaxFit,6);
   fitFcn_WithCut->SetNpx(500);
   fitFcn_WithCut->SetLineWidth(4);
   fitFcn_WithCut->SetLineColor(kMagenta);
   fitFcn_WithCut->SetParameters(1,1,1,1,1,1);
   fitFcn_WithCut->SetParameter(0,10); 
   fitFcn_WithCut->SetParLimits(0,-200,200); 
   fitFcn_WithCut->SetParameter(1,10); 
   fitFcn_WithCut->SetParLimits(1,0,500);
   fitFcn_WithCut->SetParameter(2,-1); 
   fitFcn_WithCut->SetParLimits(2,0,-30); 
   fitFcn_WithCut->SetParameter(3,10); 
   fitFcn_WithCut->SetParLimits(3,0,1000); 
   fitFcn_WithCut->SetParameter(4,5); 
   fitFcn_WithCut->SetParLimits(4,3,30);   
   fitFcn_WithCut->SetParameter(5,80);   
   fitFcn_WithCut->SetParLimits(5,75,90);  

   //Fits
   if(IsData==true)
     {
       c_noCut.cd();
       histoData_noCut->Fit("fitFcn_noCut","RV+","ep");
       c_WithCut.cd();
       histoData_WithCut->Fit("fitFcn_WithCut","RV+","ep");
     }
   if(IsData==false)
     {
       c_noCut.cd();
       histoTotBkg_noCut->Fit("fitFcn_noCut","RV+","ep");
       c_WithCut.cd();
       histoTotBkg_WithCut->Fit("fitFcn_WithCut","RV+","ep");
     }

   //Make nice plots

   //noCut
   c_noCut.cd();

   TF1 *backFcn_noCut = new TF1("backFcn_noCut",background,50,130,3);
   backFcn_noCut->SetLineColor(kRed);
   //##
   TF1 *signalFcn_noCut = new TF1("signalFcn_noCut",gaussianPeak,50,130,3);
   signalFcn_noCut->SetLineColor(kBlue);
   signalFcn_noCut->SetLineStyle(2);
   signalFcn_noCut->SetNpx(500);
   //##
   Double_t par[6];
   fitFcn_noCut->GetParameters(par);
   //##
   backFcn_noCut->SetParameters(par);
   backFcn_noCut->Draw("same");
   //##
   signalFcn_noCut->SetParameters(&par[3]);
   signalFcn_noCut->Draw("same");
   //##   
   if(IsData==true)
     c_noCut.SaveAs("fitData_noCut.png");
   if(IsData==false)
     c_noCut.SaveAs("fitBkg_noCut.png");

   //WithCut
   c_WithCut.cd();

   TF1 *backFcn_WithCut = new TF1("backFcn_WithCut",background,50,130,3);
   backFcn_WithCut->SetLineColor(kRed);
   //##
   TF1 *signalFcn_WithCut = new TF1("signalFcn_WithCut",gaussianPeak,50,130,3);
   signalFcn_WithCut->SetLineColor(kBlue);
   signalFcn_WithCut->SetLineStyle(2);
   signalFcn_WithCut->SetNpx(500);
   //##
   Double_t par[6];
   fitFcn_WithCut->GetParameters(par);
   //##
   backFcn_WithCut->SetParameters(par);
   backFcn_WithCut->Draw("same");
   //##
   signalFcn_WithCut->SetParameters(&par[3]);
   signalFcn_WithCut->Draw("same");
   //##   
   if(IsData==true)
     c_WithCut.SaveAs("fitData_WithCut.png");
   if(IsData==false)
     c_WithCut.SaveAs("fitBkg_WithCut.png");

   //Calculate efficiency   
   double bin_width=0;
   int nbins=0;
   double range=0;
   if(IsData==true)
     {
       nbins = histoData_noCut->GetXaxis()->GetNbins();
       range = histoData_noCut->GetXaxis()->GetXmax() - histoData_noCut->GetXaxis()->GetXmin();
       bin_width = double(range / nbins); 
       //cout << bin_width << endl;
     }
   if(IsData==false)
     { 
       nbins = histoTotBkg_noCut->GetXaxis()->GetNbins();
       range = histoTotBkg_noCut->GetXaxis()->GetXmax() - histoTotBkg_noCut->GetXaxis()->GetXmin();
       bin_width = double(range / nbins); 
       //cout << bin_width << endl;
     }

   double N_W_noCut = signalFcn_noCut->Integral(XminFit,XmaxFit) / bin_width;
   double N_W_WithNsubjettinessCut = signalFcn_WithCut->Integral(XminFit,XmaxFit) / bin_width;
   double N_W_WithNsubjettinessCut_WithMassCut = signalFcn_WithCut->Integral(XminSignalRegion,XmaxSignalRegion) / bin_width;

   double eff_NsubjettinessCut = N_W_WithNsubjettinessCut / N_W_noCut;
   double eff_massCut = N_W_WithNsubjettinessCut_WithMassCut / N_W_WithNsubjettinessCut;
   double eff_tot = N_W_WithNsubjettinessCut_WithMassCut / N_W_noCut;

   cout << "Total number of boosted Ws : " << N_W_noCut << endl;
   cout << "Total number of boosted Ws + nsubjettiness cut: " << N_W_WithNsubjettinessCut << endl;
   cout << "Total number of boosted Ws + nsubjettiness cut + pruned mass cut: " << N_W_WithNsubjettinessCut_WithMassCut << endl;

   cout << "Efficiency of nsubjettiness cut : " << eff_NsubjettinessCut << endl;
   cout << "Efficiency of mass cut : " << eff_massCut << endl;
   cout << "Total Efficiency : " << eff_tot << endl;

}
