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

   Double_t normFactor = 1 / ( par[1]*sqrt(2*TMath::Pi()) );

   Double_t sig = par[0] * normFactor * TMath::Exp(-0.5*arg*arg);
   return sig;
}

// Sum of background and peak function
Double_t fitFunction(Double_t *x, Double_t *par) {
  return background(x,par) + gaussianPeak(x,&par[3]);
}

// Randomize pars
TVector randomizePars(int nPar, TMatrixDSym covarianceMatrix, int randomSeed, TF1 *fitFcn){

  TVector newparameters(nPar);
  TRandom myRandomGaus(randomSeed);

  // calculate the elements of the upper-triangular matrix L that gives Lt*L = C
  // where Lt is the transpose of L (the "square-root method")
  TMatrix L(nPar,nPar);
  for(Int_t iPar= 0; iPar < nPar; iPar++) {
    // calculate the diagonal term first    
    L(iPar,iPar) = covarianceMatrix(iPar,iPar);
    //L(iPar,iPar)= covariance(iPar,iPar);
    for(Int_t k= 0; k < iPar; k++) {
      Double_t tmp= L(k,iPar);
      L(iPar,iPar)-= tmp*tmp;
    }
    L(iPar,iPar)= sqrt(L(iPar,iPar));
    // then the off-diagonal terms
    for(Int_t jPar= iPar+1; jPar < nPar; jPar++) {
      L(iPar,jPar)= covarianceMatrix(iPar,jPar);
      for(Int_t k= 0; k < iPar; k++) {
	L(iPar,jPar)-= L(k,iPar)*L(k,jPar);
      }
      L(iPar,jPar)/= L(iPar,iPar);
    }
  }
  // remember Lt
  TMatrix* _Lt= new TMatrix(TMatrix::kTransposed,L);
  
  // create a vector of unit Gaussian variables
  TVector g(nPar);
  for(Int_t k= 0; k < nPar; k++) {
    g(k) = myRandomGaus.Gaus(0,1);  
  }
  //g.Print();
  // multiply this vector by Lt to introduce the appropriate correlations
  g*= (*_Lt);

  /*  
  for(Int_t iPar= 0; iPar < nPar; iPar++) {
    for(Int_t kPar= 0; kPar < nPar; kPar++) {      
      std::cout << _Lt(iPar, kPar) << " ";      
    }
    std::cout << "" << endl;
  }
  */

  for(int thispar=0; thispar<nPar; thispar++)
    {      
      //      cout << "--- " << thispar << endl;
      //      cout << "old: " << fitFcn->GetParameter(thispar) << endl;
      newparameters(thispar) = fitFcn->GetParameter(thispar) + g(thispar); 
      //      cout << "new: " << newparameters(thispar) << endl;
    }

  return newparameters;

}

void fitTTBAR(){

  gROOT->Reset();
  gStyle->SetOptFit(1);

  //###### EDIT THIS PART #####

  bool IsData = false; // true = Data , false = MC

  double XminFit = 40;
  double XmaxFit = 130;

  double XminSignalRegion = 70;
  double XmaxSignalRegion = 100;

  //Input files
  //TFile *_file_noMassCut_noNsubjettinessCut = TFile::Open("WW_mu_fullrange_ttbar_CA8/root/can_h_mJJ.root");
  //TFile *_file_noMassCut_yesNsubjettinessCut = TFile::Open("WW_mu_fullrange_ttbar_CA8_nsub0p45/root/can_h_mJJ.root");
  TFile *_file_noMassCut_noNsubjettinessCut = TFile::Open("WW_mu_fullrange_ttbarPowheg_CA8/root/can_h_mJJ.root");
  TFile *_file_noMassCut_yesNsubjettinessCut = TFile::Open("WW_mu_fullrange_ttbarPowheg_CA8_nsub0p45/root/can_h_mJJ.root");

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
  fitFcn_noCut->SetParLimits(3,0,6000); 
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
  fitFcn_WithCut->SetParLimits(3,0,6000); 
  fitFcn_WithCut->SetParameter(4,5); 
  fitFcn_WithCut->SetParLimits(4,3,30);   
  fitFcn_WithCut->SetParameter(5,80);   
  fitFcn_WithCut->SetParLimits(5,75,90);  

  //Fits
  TFitResultPtr fitResult_noCut;
  TFitResultPtr fitResult_WithCut;

  if(IsData==true)
    {
      c_noCut.cd();
      fitResult_noCut = histoData_noCut->Fit("fitFcn_noCut","RVS","ep");
      c_WithCut.cd();
      fitResult_WithCut = histoData_WithCut->Fit("fitFcn_WithCut","RVS","ep");
    }
  if(IsData==false)
    {
      c_noCut.cd();
      fitResult_noCut = histoTotBkg_noCut->Fit("fitFcn_noCut","RVS","ep");
      c_WithCut.cd();
      fitResult_WithCut = histoTotBkg_WithCut->Fit("fitFcn_WithCut","RVS","ep");
    }

  cout << "******* Fit default no cut ********" << endl;
  fitResult_noCut->Print();
  cout << endl;

  cout << "******* Fit default with cut ********" << endl;
  fitResult_WithCut->Print();
  cout << endl;


  //Make nice plots

  //noCut
  c_noCut.cd();

  TF1 *backFcn_noCut = new TF1("backFcn_noCut",background,XminFit,XmaxFit,3);
  backFcn_noCut->SetLineColor(kRed);
  //##
  TF1 *signalFcn_noCut = new TF1("signalFcn_noCut",gaussianPeak,XminFit,XmaxFit,3);
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

  TF1 *backFcn_WithCut = new TF1("backFcn_WithCut",background,XminFit,XmaxFit,3);
  backFcn_WithCut->SetLineColor(kRed);
  //##
  TF1 *signalFcn_WithCut = new TF1("signalFcn_WithCut",gaussianPeak,XminFit,XmaxFit,3);
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


  //Calculate efficiency (default fit)

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

  double N_W_noCut = fitFcn_noCut->GetParameter(3) / bin_width;
  double N_W_WithNsubjettinessCut = fitFcn_WithCut->GetParameter(3) / bin_width;
  double N_W_WithNsubjettinessCut_WithMassCut = signalFcn_WithCut->Integral(XminSignalRegion,XmaxSignalRegion) / bin_width;

  double eff_NsubjettinessCut = N_W_WithNsubjettinessCut / N_W_noCut;
  double err_eff_NsubjettinessCut = sqrt( eff_NsubjettinessCut * (1-eff_NsubjettinessCut) / N_W_noCut );

  double eff_massCut = N_W_WithNsubjettinessCut_WithMassCut / N_W_WithNsubjettinessCut;
  double err_eff_massCut = sqrt( eff_massCut * (1-eff_massCut) / N_W_WithNsubjettinessCut );

  double eff_tot = N_W_WithNsubjettinessCut_WithMassCut / N_W_noCut;
  double err_eff_tot = sqrt( eff_tot * (1-eff_tot) / N_W_noCut );

  cout << "Total number of boosted Ws : " << N_W_noCut << endl;
  cout << "Total number of boosted Ws + nsubjettiness cut: " << N_W_WithNsubjettinessCut << endl;
  cout << "Total number of boosted Ws + nsubjettiness cut + pruned mass cut: " << N_W_WithNsubjettinessCut_WithMassCut << endl;

  cout << "Efficiency of nsubjettiness cut : " << eff_NsubjettinessCut << " +/- " << err_eff_NsubjettinessCut << endl;
  cout << "Efficiency of mass cut : " << eff_massCut << " +/- " << err_eff_massCut << endl;
  cout << "Total Efficiency : " << eff_tot << " +/- " << err_eff_tot << endl;


  // Calculate efficiencies using shapes modified using the covariance matrix

  TH1D *histo_efficiency_NsubjettinessCut = new TH1D("histo_efficiency_NsubjettinessCut","histo_efficiency_NsubjettinessCut",50,0.5,1);
  TH1D *histo_err_efficiency_NsubjettinessCut = new TH1D("histo_err_efficiency_NsubjettinessCut","histo_err_efficiency_NsubjettinessCut",50,0,0.05);

  TH1D *histo_efficiency_massCut = new TH1D("histo_efficiency_massCut","histo_efficiency_massCut",50,0.5,1);
  TH1D *histo_err_efficiency_massCut = new TH1D("histo_err_efficiency_massCut","histo_err_efficiency_massCut",50,0,0.05);

  TH1D *histo_efficiency_tot = new TH1D("histo_efficiency_tot","histo_efficiency_tot",50,0.5,1);
  TH1D *histo_err_efficiency_tot = new TH1D("histo_err_efficiency_tot","histo_err_efficiency_tot",50,0,0.05);

  TMatrixDSym covarianceMatrix_noCut = fitResult_noCut->GetCovarianceMatrix();
  TMatrixDSym covarianceMatrix_WithCut = fitResult_WithCut->GetCovarianceMatrix();

  int nToys = 1000;
  for(int toy=1; toy<=nToys; toy++)
    {

      TVector newPars_noCut = randomizePars(6, covarianceMatrix_noCut, 55673+toy, fitFcn_noCut);  
      TVector newPars_WithCut = randomizePars(6, covarianceMatrix_WithCut, 86954+toy, fitFcn_WithCut);  

      TF1 *fitFcn_noCut_NEW = new TF1("fitFcn_noCut_NEW",fitFunction,XminFit,XmaxFit,6);  
      TF1 *fitFcn_WithCut_NEW = new TF1("fitFcn_WithCut_NEW",fitFunction,XminFit,XmaxFit,6);  
      
      for(int thispar=0; thispar<6; thispar++)
	{
	  fitFcn_noCut_NEW->SetParameter(thispar,newPars_noCut(thispar));
	  fitFcn_WithCut_NEW->SetParameter(thispar,newPars_WithCut(thispar));
	}

      TF1 *signalFcn_noCut_NEW = new TF1("signalFcn_noCut_NEW",gaussianPeak,XminFit,XmaxFit,3);
      signalFcn_noCut_NEW->SetParameter( 0 , fitFcn_noCut_NEW->GetParameter(3) );
      signalFcn_noCut_NEW->SetParameter( 1 , fitFcn_noCut_NEW->GetParameter(4) );
      signalFcn_noCut_NEW->SetParameter( 2 , fitFcn_noCut_NEW->GetParameter(5) );

      TF1 *signalFcn_WithCut_NEW = new TF1("signalFcn_WithCut_NEW",gaussianPeak,XminFit,XmaxFit,3);
      signalFcn_WithCut_NEW->SetParameter( 0 , fitFcn_WithCut_NEW->GetParameter(3) );
      signalFcn_WithCut_NEW->SetParameter( 1 , fitFcn_WithCut_NEW->GetParameter(4) );
      signalFcn_WithCut_NEW->SetParameter( 2 , fitFcn_WithCut_NEW->GetParameter(5) );

      double N_W_noCut_NEW = fitFcn_noCut_NEW->GetParameter(3) / bin_width;
      double N_W_WithNsubjettinessCut_NEW = fitFcn_WithCut_NEW->GetParameter(3) / bin_width;
      double N_W_WithNsubjettinessCut_WithMassCut_NEW = signalFcn_WithCut_NEW->Integral(XminSignalRegion,XmaxSignalRegion) / bin_width;
      
      double eff_NsubjettinessCut_NEW = N_W_WithNsubjettinessCut_NEW / N_W_noCut_NEW;
      double err_eff_NsubjettinessCut_NEW = sqrt( eff_NsubjettinessCut_NEW * (1-eff_NsubjettinessCut_NEW) / N_W_noCut_NEW );
      
      double eff_massCut_NEW = N_W_WithNsubjettinessCut_WithMassCut_NEW / N_W_WithNsubjettinessCut_NEW;
      double err_eff_massCut_NEW = sqrt( eff_massCut_NEW * (1-eff_massCut_NEW) / N_W_WithNsubjettinessCut_NEW );
      
      double eff_tot_NEW = N_W_WithNsubjettinessCut_WithMassCut_NEW / N_W_noCut_NEW;
      double err_eff_tot_NEW = sqrt( eff_tot_NEW * (1-eff_tot_NEW) / N_W_noCut_NEW );

      histo_efficiency_NsubjettinessCut->Fill(eff_NsubjettinessCut_NEW);
      histo_err_efficiency_NsubjettinessCut->Fill(err_eff_NsubjettinessCut_NEW);

      histo_efficiency_massCut->Fill(eff_massCut_NEW);
      histo_err_efficiency_massCut->Fill(err_eff_massCut_NEW);

      histo_efficiency_tot->Fill(eff_tot_NEW);
      histo_err_efficiency_tot->Fill(err_eff_tot_NEW);

      delete fitFcn_noCut_NEW;
      delete fitFcn_WithCut_NEW;
      delete signalFcn_noCut_NEW;
      delete signalFcn_WithCut_NEW;
    }

  if(IsData==true)
    {  
      string out_title = "output_fitTTBAR_Data.root";
    }
  if(IsData==false)
    {
      string out_title = "output_fitTTBAR_Bkg.root";
    }

  TFile *rootfile = new TFile(out_title.c_str(),"RECREATE");
  histo_efficiency_NsubjettinessCut->Write();
  histo_err_efficiency_NsubjettinessCut->Write();
  histo_efficiency_massCut->Write();
  histo_err_efficiency_massCut->Write();
  histo_efficiency_tot->Write();
  histo_err_efficiency_tot->Write();
  
}
