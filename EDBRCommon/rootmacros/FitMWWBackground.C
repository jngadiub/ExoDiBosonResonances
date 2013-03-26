{

  gROOT->Reset();
  
  gStyle->SetOptFit(1111);
  
  //###### EDIT THIS PART #####

  //TFile *_file_sideband = TFile::Open("WW_mu_sideband_CA8_nsub0p45/root/can_h_mWW.root");
  //TFile *_file_signal = TFile::Open("WW_mu_signal_CA8_nsub0p45/root/can_h_mWW.root");

  //TFile *_file_sideband = TFile::Open("WW_mu_sidebandA_nsub0p45/root/can_h_mWW.root");
  //TFile *_file_signal = TFile::Open("WW_mu_sidebandB_nsub0p45/root/can_h_mWW.root");

  TFile *_file_sideband = TFile::Open("WW_mu_sidebandA1_nsub0p45/root/can_h_mWW.root");
  TFile *_file_signal = TFile::Open("WW_mu_sidebandA2_nsub0p45/root/can_h_mWW.root");

  double minX_mass = 700.;
  double maxX_mass = 2600.;

  double minX_mass_final = 800.;
  double maxX_mass_final = 2600.;

  bool unblind=true;

  const int FunctionTypeAlfa = 0;
  const int FunctionTypeBkg = 2;
  
  //int number_of_variableWidth_bins = 53 - 1;
  //Double_t massBins[53] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 340, 381, 424, 468, 514, 561, 610, 661, 714, 768, 824, 882, 942, 1004, 1069, 1136, 1205, 1277, 1351, 1428, 1507, 1589, 1674, 1762, 1853, 1947, 2045, 2146, 2251, 2359, 2471, 2587, 2707, 2831, 2960, 3093, 3231, 3374, 3522, 3675, 3833, 4000};

  int number_of_variableWidth_bins = 23 - 1;
  //Double_t massBins[23]={0,480,500,520,560,600,640,680,720,760,800,840,920,1000,1100,1250,1400,1600,1800,2000,2200,2400,2600};
  Double_t massBins[23]={0,480,500,520,560,600,640,680,720,760,800,840,920,1000,1100,1200,1300,1500,1700,1900,2100,2300,2600};


  //------------------------------------------------
  //Sigma (GeV) = 30 + 0.035*X (GeV)
  //------------------------------------------------

  //###########################


  int nPar=-1;

  //## Read the canvas
  TCanvas* cv_h_mWW_sideband;
  cv_h_mWW_sideband = (TCanvas*) _file_sideband->Get( "cv_h_mWW" );
  TCanvas* cv_h_mWW_signal;
  cv_h_mWW_signal = (TCanvas*) _file_signal->Get( "cv_h_mWW" );

  //## Get the histograms from the canvas
  TH1D* histo_Data_sideband;
  TH1D* histo_Data_signal;
  TH1D* histo_TotBkg_sideband;
  TH1D* histo_TotBkg_signal;
  THStack* histoStack_sideband;
  THStack* histoStack_signal;

  histo_Data_sideband = (TH1D*) cv_h_mWW_sideband->FindObject( "masterDATA" );
  histo_Data_sideband->SetName("Data SB");
  histo_Data_sideband->SetTitle("");
  histo_Data_sideband->GetYaxis()->SetTitle("Events");
  histo_Data_sideband->GetYaxis()->SetTitleOffset(0.8);
  histo_Data_sideband->GetXaxis()->SetTitleOffset(0.8);
  histo_Data_sideband->GetXaxis()->SetTitle("m_{WW} [GeV]");
  histo_Data_sideband->Rebin( number_of_variableWidth_bins, "histo_Data_sideband_varbin", massBins);  

  if(unblind==true)
    {
      histo_Data_signal = (TH1D*) cv_h_mWW_signal->FindObject( "masterDATA" );
      histo_Data_signal->SetName("Data SB");
      histo_Data_signal->SetTitle("");
      histo_Data_signal->GetYaxis()->SetTitle("Events");
      histo_Data_signal->GetYaxis()->SetTitleOffset(0.8);
      histo_Data_signal->GetXaxis()->SetTitleOffset(0.8);
      histo_Data_signal->GetXaxis()->SetTitle("m_{WW} [GeV]");
      histo_Data_signal->Rebin( number_of_variableWidth_bins, "histo_Data_signal_varbin", massBins);  
    }

  histoStack_sideband = (THStack*) cv_h_mWW_sideband->FindObject( "hs" );
  histo_TotBkg_sideband = (TH1D*) histoStack_sideband->GetStack()->Last();
  histo_TotBkg_sideband->SetName("MC Bkg SB");
  histo_TotBkg_sideband->SetTitle("");
  histo_TotBkg_sideband->GetYaxis()->SetTitle("Events");
  histo_TotBkg_sideband->GetYaxis()->SetTitleOffset(0.8);
  histo_TotBkg_sideband->GetXaxis()->SetTitleOffset(0.8);
  histo_TotBkg_sideband->GetXaxis()->SetTitle("m_{WW} [GeV]");
  histo_TotBkg_sideband->Rebin( number_of_variableWidth_bins, "histo_TotBkg_sideband_varbin", massBins);  

  histoStack_signal = (THStack*) cv_h_mWW_signal->FindObject( "hs" );
  histo_TotBkg_signal = (TH1D*) histoStack_signal->GetStack()->Last();
  histo_TotBkg_signal->SetName("MC Bkg SIG");
  histo_TotBkg_signal->SetTitle("");
  histo_TotBkg_signal->GetYaxis()->SetTitle("Events");
  histo_TotBkg_signal->GetYaxis()->SetTitleOffset(0.8);
  histo_TotBkg_signal->GetXaxis()->SetTitleOffset(0.8);
  histo_TotBkg_signal->GetXaxis()->SetTitle("m_{WW} [GeV]");
  histo_TotBkg_signal->Rebin( number_of_variableWidth_bins, "histo_TotBkg_signal_varbin", massBins);  

  //## Divide all histograms by bin width
  for(int bin=1; bin<number_of_variableWidth_bins+1; bin++)
    {

      if( histo_Data_sideband_varbin->GetBinContent(bin) > 0 )
	{
	  double content = histo_Data_sideband_varbin->GetBinContent(bin);
	  double error = histo_Data_sideband_varbin->GetBinError(bin);
	  double width = histo_Data_sideband_varbin->GetBinWidth(bin);

	  histo_Data_sideband_varbin->SetBinContent(bin, content / width );
	  histo_Data_sideband_varbin->SetBinError(bin, error / width );
	}

      if(unblind==true)
	{	  
	  if( histo_Data_signal_varbin->GetBinContent(bin) > 0 )
	    {
	      double content = histo_Data_signal_varbin->GetBinContent(bin);
	      double error = histo_Data_signal_varbin->GetBinError(bin);
	      double width = histo_Data_signal_varbin->GetBinWidth(bin);

	      histo_Data_signal_varbin->SetBinContent(bin, content / width );
	      histo_Data_signal_varbin->SetBinError(bin, error / width );
	    }
	}

      if( histo_TotBkg_sideband_varbin->GetBinContent(bin) > 0 )
	{
	  double content = histo_TotBkg_sideband_varbin->GetBinContent(bin);
	  double error = histo_TotBkg_sideband_varbin->GetBinError(bin);
	  double width = histo_TotBkg_sideband_varbin->GetBinWidth(bin);

	  histo_TotBkg_sideband_varbin->SetBinContent(bin, content / width );
	  histo_TotBkg_sideband_varbin->SetBinError(bin, error / width );
	}

      if( histo_TotBkg_signal_varbin->GetBinContent(bin) > 0 )
	{
	  double content = histo_TotBkg_signal_varbin->GetBinContent(bin);
	  double error = histo_TotBkg_signal_varbin->GetBinError(bin);
	  double width = histo_TotBkg_signal_varbin->GetBinWidth(bin);
	  
	  histo_TotBkg_signal_varbin->SetBinContent(bin, content / width );
	  histo_TotBkg_signal_varbin->SetBinError(bin, error / width );      
	}  
    }

  TCanvas *Canvas0 = new TCanvas("Canvas0","Canvas0",11,51,700,600);
  Canvas0->Divide(1,3);
  Canvas0->cd(1);
  histo_TotBkg_signal_varbin->Draw();  
  Canvas0->cd(2);
  histo_TotBkg_sideband_varbin->Draw();  


  //## Calculate alfa (histogram)
  TH1D* histo_alfa;
  histo_alfa = (TH1D*) histo_TotBkg_signal_varbin->Clone("histo_alfa");
  histo_alfa->Divide(histo_TotBkg_sideband_varbin);
  histo_alfa->SetLineColor(kBlue);
  histo_alfa->SetMarkerColor(kBlue);

  //## Calculate alfa (fits)
  TF1 *M1Bkg_sideband;
  TF1 *M1Bkg_signal;

  // 0: DEFAULT (3 par.) - "( [0]*TMath::Power(1-x/8000,[1]) ) / ( TMath::Power(x/8000,[2]) )" 
   if( FunctionTypeAlfa==0 )    
     {
       M1Bkg_sideband = new TF1("M1Bkg_sideband","( [0]*TMath::Power(1-x/8000,[1]) ) / ( TMath::Power(x/8000,[2]) )",minX_mass,maxX_mass);
       M1Bkg_sideband->SetParameter(0,0.1);
       M1Bkg_sideband->SetParameter(1,15);
       M1Bkg_sideband->SetParameter(2,3);
       M1Bkg_sideband->SetLineColor(kRed);

       M1Bkg_signal = new TF1("M1Bkg_signal","( [0]*TMath::Power(1-x/8000,[1]) ) / ( TMath::Power(x/8000,[2]) )",minX_mass,maxX_mass);
       M1Bkg_signal->SetParameter(0,0.1);
       M1Bkg_signal->SetParameter(1,15);
       M1Bkg_signal->SetParameter(2,3);
       M1Bkg_signal->SetLineColor(kRed);
     }

  // 2: EXPO (2 par.) - "exp([0]+x/[1])" 
  if( FunctionTypeAlfa==2 )    
    {
      M1Bkg_sideband = new TF1("M1Bkg_sideband","exp([0]+x/[1])",minX_mass,maxX_mass);
      M1Bkg_sideband->SetParameter(0,5);
      M1Bkg_sideband->SetParameter(1,-300);
      M1Bkg_sideband->SetLineColor(kRed);
      
      M1Bkg_signal = new TF1("M1Bkg_signal","exp([0]+x/[1])",minX_mass,maxX_mass);
      M1Bkg_signal->SetParameter(0,5);
      M1Bkg_signal->SetParameter(1,-300);
      M1Bkg_signal->SetLineColor(kRed); 
    }


  //---- fits

  Canvas0->cd(1);

  TFitResultPtr r_sideband;
  int stopProgram_sideband=1;
  for( int loop=0; loop<10; loop++)
    {
      r_sideband = histo_TotBkg_sideband_varbin->Fit("M1Bkg_sideband","WLS","",minX_mass,maxX_mass);      
      r_sideband = histo_TotBkg_sideband_varbin->Fit("M1Bkg_sideband","WLS","",minX_mass,maxX_mass);      

      Int_t fitStatus = r_sideband;
      if(fitStatus==0)
	{
	  stopProgram_sideband=0;
	  r_sideband->Print("V");  
	  break;
	}
    }

  if(stopProgram_sideband==1)
    {
      cout << "###############################" << endl;
      cout << "###############################" << endl;
      cout << "ERROR : Fit Sideband failed!!!!" << endl;
      cout << "###############################" << endl;
      cout << "###############################" << endl;
      break;
    }

  Canvas0->cd(2);

  TFitResultPtr r_signal;
  int stopProgram_signal=1;
  for( int loop=0; loop<10; loop++)
    {
      r_signal = histo_TotBkg_signal_varbin->Fit("M1Bkg_signal","WLS","",minX_mass,maxX_mass);      
      r_signal = histo_TotBkg_signal_varbin->Fit("M1Bkg_signal","WLS","",minX_mass,maxX_mass);      

      Int_t fitStatus = r_signal;
      if(fitStatus==0)
	{
	  stopProgram_signal=0;
	  r_signal->Print("V");  
	  break;
	}
    }

  if(stopProgram_signal==1)
    {
      cout << "#############################" << endl;
      cout << "#############################" << endl;
      cout << "ERROR : Fit Signal failed!!!!" << endl;
      cout << "#############################" << endl;
      cout << "#############################" << endl;
      break;
    }

  //--- get alfa

  Canvas0->cd(3);
  Canvas0_3->SetGridx();
  Canvas0_3->SetGridy();
  histo_alfa->Draw();

  TF1 *fit_alfa;
  fit_alfa = new TF1("fit_alfa","M1Bkg_signal/M1Bkg_sideband",minX_mass,maxX_mass);
  fit_alfa->Draw("same");
  fit_alfa->SetLineColor(kRed);

  //## Get background prediction in signal region  

  //-- From alfa fit
  TH1D* histo_Data_signalPred_fit_varbin = new TH1D("histo_Data_signalPred_fit_varbin"
						    ,"histo_Data_signalPred_fit_varbin",
						    number_of_variableWidth_bins,massBins);
  histo_Data_signalPred_fit_varbin->Sumw2();
  histo_Data_signalPred_fit_varbin->SetLineColor(kRed);
  histo_Data_signalPred_fit_varbin->SetMarkerColor(kRed);
  histo_Data_signalPred_fit_varbin->SetMarkerStyle(7);

  for(int bin=1; bin<number_of_variableWidth_bins; bin++)
    {
      if( histo_Data_sideband_varbin->GetXaxis()->GetBinLowEdge(bin)>=minX_mass 
 	  && histo_Data_sideband_varbin->GetXaxis()->GetBinUpEdge(bin)<=maxX_mass )
 	{	  
	  double data = histo_Data_sideband_varbin->GetBinContent(bin);
	  double err_data = histo_Data_sideband_varbin->GetBinError(bin);
 	  double alfa = fit_alfa->Integral(histo_Data_sideband_varbin->GetXaxis()->GetBinLowEdge(bin) 
					   , histo_Data_sideband_varbin->GetXaxis()->GetBinUpEdge(bin) ); 
	  alfa = alfa / ( histo_Data_sideband_varbin->GetBinWidth(bin) );
	  double predicted_bkg = data * alfa;
	  double err_predicted_bkg = err_data * alfa;

	  histo_Data_signalPred_fit_varbin->SetBinContent(bin,predicted_bkg);
	  histo_Data_signalPred_fit_varbin->SetBinError(bin,err_predicted_bkg);
	}
    }

  //-- From histogram
  TH1D* histo_Data_signalPred_histo_varbin = new TH1D("histo_Data_signalPred_histo_varbin"
						    ,"histo_Data_signalPred_histo_varbin",
						      number_of_variableWidth_bins,massBins);
  histo_Data_signalPred_histo_varbin->Sumw2();
  histo_Data_signalPred_histo_varbin->SetLineColor(kBlue);
  histo_Data_signalPred_histo_varbin->SetMarkerColor(kBlue);
  histo_Data_signalPred_histo_varbin->SetMarkerStyle(7);

  histo_Data_signalPred_histo_varbin->Multiply(histo_Data_sideband_varbin,histo_alfa);

  //-- Draw (all predictions compared to original data)
  TCanvas *Canvas1 = new TCanvas("Canvas1","Canvas1",11,51,700,600);
  Canvas1->SetLogy();
  histo_Data_sideband_varbin->Draw();
  histo_Data_signalPred_fit_varbin->Draw("sames");
  histo_Data_signalPred_histo_varbin->Draw("sames");

  //-- Do the final fits to background prediction
  TF1 *M2Bkg_signal_alfaHist;
  TF1 *M2Bkg_signal_alfaFit;
  if( FunctionTypeBkg == 2 )
    {
      M2Bkg_signal_alfaHist = new TF1("M2Bkg_signal_alfaHist","exp([0]+x/[1])",minX_mass_final,maxX_mass_final);
      M2Bkg_signal_alfaHist->SetParameter(0,5);
      M2Bkg_signal_alfaHist->SetParameter(1,-300);
      M2Bkg_signal_alfaHist->SetLineColor(kBlue);

      M2Bkg_signal_alfaFit = new TF1("M2Bkg_signal_alfaFit","exp([0]+x/[1])",minX_mass_final,maxX_mass_final);
      M2Bkg_signal_alfaFit->SetParameter(0,5);
      M2Bkg_signal_alfaFit->SetParameter(1,-300);
      M2Bkg_signal_alfaFit->SetLineColor(kRed);
    }
  
  TFitResultPtr r_signal_alfaHist;
  int stopProgram_signal_alfaHist=1;
  for( int loop=0; loop<10; loop++)
    {
      r_signal_alfaHist = histo_Data_signalPred_histo_varbin->Fit("M2Bkg_signal_alfaHist","WLS0","",minX_mass_final,maxX_mass_final);      
      r_signal_alfaHist = histo_Data_signalPred_histo_varbin->Fit("M2Bkg_signal_alfaHist","WLS0","",minX_mass_final,maxX_mass_final);      

      Int_t fitStatus = r_signal_alfaHist;
      if(fitStatus==0)
	{
	  stopProgram_signal_alfaHist=0;
	  r_signal_alfaHist->Print("V");  
	  break;
	}
    }

  if(stopProgram_signal_alfaHist==1)
    {
      cout << "#############################" << endl;
      cout << "#############################" << endl;
      cout << "ERROR : Fit Signal failed!!!!" << endl;
      cout << "#############################" << endl;
      cout << "#############################" << endl;
      break;
    }

  TFitResultPtr r_signal_alfaFit;
  int stopProgram_signal_alfaFit=1;
  for( int loop=0; loop<10; loop++)
    {
      r_signal_alfaFit = histo_Data_signalPred_fit_varbin->Fit("M2Bkg_signal_alfaFit","WLS0","",minX_mass_final,maxX_mass_final);      
      r_signal_alfaFit = histo_Data_signalPred_fit_varbin->Fit("M2Bkg_signal_alfaFit","WLS0","",minX_mass_final,maxX_mass_final);      

      Int_t fitStatus = r_signal_alfaFit;
      if(fitStatus==0)
	{
	  stopProgram_signal_alfaFit=0;
	  r_signal_alfaFit->Print("V");  
	  break;
	}
    }

  if(stopProgram_signal_alfaFit==1)
    {
      cout << "#############################" << endl;
      cout << "#############################" << endl;
      cout << "ERROR : Fit Signal failed!!!!" << endl;
      cout << "#############################" << endl;
      cout << "#############################" << endl;
      break;
    }

  //-- fit residuals and chi2
  TH1D* hist_fit_residual_vsMass_alfaHist = new TH1D("hist_fit_residual_vsMass_alfaHist"
						     ,"hist_fit_residual_vsMass_alfaHist",
						     number_of_variableWidth_bins,massBins);
  TH1D* hist_fit_residual_vsMass_alfaFit = new TH1D("hist_fit_residual_vsMass_alfaFit"
						    ,"hist_fit_residual_vsMass_alfaFit",
						    number_of_variableWidth_bins,massBins);  
 
  if(unblind==true)
    {
      for(int bin=1; bin<number_of_variableWidth_bins+1; bin++)
	{
	  if( histo_Data_signal_varbin->GetXaxis()->GetBinLowEdge(bin)>=minX_mass_final 
	      && histo_Data_signal_varbin->GetXaxis()->GetBinUpEdge(bin)<=maxX_mass_final )
	    {	  
	      //cout << bin << " " << data << " " <<  histo_Data_signal_varbin->GetXaxis()->GetBinLowEdge(bin) << endl;
	      double data = histo_Data_signal_varbin->GetBinContent(bin);
	      double err_data = histo_Data_signal_varbin->GetBinError(bin);
	      double fit_alfaHist = M2Bkg_signal_alfaHist->Integral(histo_Data_signal_varbin->GetXaxis()->GetBinLowEdge(bin) 
								    , histo_Data_signal_varbin->GetXaxis()->GetBinUpEdge(bin) ); 
	      double fit_alfaFit = M2Bkg_signal_alfaFit->Integral(histo_Data_signal_varbin->GetXaxis()->GetBinLowEdge(bin) 
								  , histo_Data_signal_varbin->GetXaxis()->GetBinUpEdge(bin) ); 
	      fit_alfaHist = fit_alfaHist / ( histo_Data_signal_varbin->GetBinWidth(bin) );
	      fit_alfaFit = fit_alfaFit / ( histo_Data_signal_varbin->GetBinWidth(bin) );
	      
	      double err_tot = err_data;	  
	      double fit_residual_alfaHist = (data - fit_alfaHist) / err_tot;
	      double fit_residual_alfaFit = (data - fit_alfaFit) / err_tot;
	      double err_fit_residual = 1;
	      if (data == 0)
		{
		  fit_residual_alfaHist = (data - fit_alfaHist) / 1.29;
		  fit_residual_alfaFit = (data - fit_alfaFit) / 1.29;
		}
	      
	      hist_fit_residual_vsMass_alfaHist->SetBinContent(bin,fit_residual_alfaHist);
	      hist_fit_residual_vsMass_alfaHist->SetBinError(bin,err_fit_residual);

	      hist_fit_residual_vsMass_alfaFit->SetBinContent(bin,fit_residual_alfaFit);
	      hist_fit_residual_vsMass_alfaFit->SetBinError(bin,err_fit_residual);
	    }
	}      
    }

  
  //-- Plot them together with the data in the signal region 
  TCanvas *Canvas2 = new TCanvas("Canvas2","Canvas2",11,51,700,600);
  TPad* fPads1 = NULL;
  TPad* fPads2 = NULL;
  TPad* fPads3 = NULL;

  TLine* lineAtZero = NULL; 
  TLine* lineAtPlusTwo = NULL; 
  TLine* lineAtMinusTwo = NULL; 

  if(unblind==true)
    {
      fPads1 = new TPad("pad1", "", 0.00, 0.40, 0.99, 0.99);
      fPads2 = new TPad("pad2", "", 0.00, 0.20, 0.99, 0.40);
      fPads3 = new TPad("pad3", "", 0.00, 0.00, 0.99, 0.20);
      fPads1->SetFillColor(0);
      fPads1->SetLineColor(0);
      fPads2->SetFillColor(0);
      fPads2->SetLineColor(0);
      fPads3->SetFillColor(0);
      fPads3->SetLineColor(0);
      fPads1->Draw();
      fPads2->Draw();
      fPads3->Draw();
    }

  if(unblind==true)
    {
      fPads1->cd();
      fPads1->SetLogy();
    }
  if(unblind==false)
    {
      Canvas2->SetLogy();
    }

  histo_Data_signalPred_histo_varbin->Draw();
  histo_Data_signalPred_fit_varbin->Draw("sames");
  if(unblind==true)
    {
      histo_Data_signal_varbin->Draw("sames");
    }
  histo_Data_signalPred_histo_varbin->GetYaxis()->SetRangeUser(0.001,10);
  histo_Data_signalPred_histo_varbin->GetXaxis()->SetRangeUser(minX_mass_final,maxX_mass_final);
  M2Bkg_signal_alfaHist->Draw("samel");
  M2Bkg_signal_alfaFit->Draw("samel");

  if(unblind==true)
    {
      lineAtZero = new TLine(minX_mass_final,0,maxX_mass_final,0);
      lineAtZero->SetLineColor(2);
      lineAtPlusTwo = new TLine(minX_mass_final,2,maxX_mass_final,2);
      lineAtPlusTwo->SetLineColor(2);
      lineAtPlusTwo->SetLineStyle(2);
      lineAtMinusTwo = new TLine(minX_mass_final,-2,maxX_mass_final,-2);
      lineAtMinusTwo->SetLineColor(2);
      lineAtMinusTwo->SetLineStyle(2);
      
      fPads2->cd();
      fPads2->SetGridx();
      fPads2->SetGridy();
      hist_fit_residual_vsMass_alfaHist->SetLineColor(kBlue);
      hist_fit_residual_vsMass_alfaHist->SetMarkerColor(kBlue);
      hist_fit_residual_vsMass_alfaHist->SetMarkerStyle(20);
      hist_fit_residual_vsMass_alfaHist->SetTitle("");
      hist_fit_residual_vsMass_alfaHist->SetStats(0);
      hist_fit_residual_vsMass_alfaHist->GetYaxis()->SetRangeUser(-4,4);
      hist_fit_residual_vsMass_alfaHist->GetXaxis()->SetRangeUser(minX_mass_final,maxX_mass_final);
      hist_fit_residual_vsMass_alfaHist->Draw();

      fPads2->Update();
      lineAtZero->Draw();
      lineAtPlusTwo->Draw();
      lineAtMinusTwo->Draw();

      fPads3->cd();
      fPads3->SetGridx();
      fPads3->SetGridy();
      hist_fit_residual_vsMass_alfaFit->SetLineColor(kRed);
      hist_fit_residual_vsMass_alfaFit->SetMarkerColor(kRed);
      hist_fit_residual_vsMass_alfaFit->SetMarkerStyle(20);
      hist_fit_residual_vsMass_alfaFit->SetTitle("");
      hist_fit_residual_vsMass_alfaFit->SetStats(0);
      hist_fit_residual_vsMass_alfaFit->GetYaxis()->SetRangeUser(-4,4);
      hist_fit_residual_vsMass_alfaFit->GetXaxis()->SetRangeUser(minX_mass_final,maxX_mass_final);
      hist_fit_residual_vsMass_alfaFit->Draw();

      fPads3->Update();
      lineAtZero->Draw();
      lineAtPlusTwo->Draw();
      lineAtMinusTwo->Draw();
    }
}
