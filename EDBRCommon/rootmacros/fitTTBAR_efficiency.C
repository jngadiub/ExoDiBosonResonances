{
  TFile *_file_Data = TFile::Open("powheg_inclusive_mu/output_fitTTBAR_Data.root");
  TFile *_file_Bkg = TFile::Open("powheg_inclusive_mu/output_fitTTBAR_Bkg.root");

//   TFile *_file_Data = TFile::Open("powheg_inclusive_ele/output_fitTTBAR_Data.root");
//   TFile *_file_Bkg = TFile::Open("powheg_inclusive_ele/output_fitTTBAR_Bkg.root");
  
  
  TH1D *histo_eff_nsub_Data = (TH1D*) _file_Data->Get("histo_efficiency_NsubjettinessCut"); 
  TH1D *histo_eff_nsub_Bkg = (TH1D*) _file_Bkg->Get("histo_efficiency_NsubjettinessCut"); 
  TH1D *histo_eff_mass_Data = (TH1D*) _file_Data->Get("histo_efficiency_massCut"); 
  TH1D *histo_eff_mass_Bkg = (TH1D*) _file_Bkg->Get("histo_efficiency_massCut"); 
  TH1D *histo_eff_tot_Data = (TH1D*) _file_Data->Get("histo_efficiency_tot"); 
  TH1D *histo_eff_tot_Bkg = (TH1D*) _file_Bkg->Get("histo_efficiency_tot"); 

  TH1D *histo_err_eff_nsub_Data = (TH1D*) _file_Data->Get("histo_err_efficiency_NsubjettinessCut"); 
  TH1D *histo_err_eff_nsub_Bkg = (TH1D*) _file_Bkg->Get("histo_err_efficiency_NsubjettinessCut"); 
  TH1D *histo_err_eff_mass_Data = (TH1D*) _file_Data->Get("histo_err_efficiency_massCut"); 
  TH1D *histo_err_eff_mass_Bkg = (TH1D*) _file_Bkg->Get("histo_err_efficiency_massCut"); 
  TH1D *histo_err_eff_tot_Data = (TH1D*) _file_Data->Get("histo_err_efficiency_tot"); 
  TH1D *histo_err_eff_tot_Bkg = (TH1D*) _file_Bkg->Get("histo_err_efficiency_tot"); 

  histo_eff_nsub_Data->SetLineColor(kBlue);
  histo_eff_mass_Data->SetLineColor(kBlue);
  histo_eff_tot_Data->SetLineColor(kBlue);
  histo_eff_nsub_Bkg->SetLineColor(kRed);
  histo_eff_mass_Bkg->SetLineColor(kRed);
  histo_eff_tot_Bkg->SetLineColor(kRed);

  histo_eff_nsub_Data->SetLineStyle(1);
  histo_eff_mass_Data->SetLineStyle(2);
  histo_eff_tot_Data->SetLineStyle(1);
  histo_eff_nsub_Bkg->SetLineStyle(1);
  histo_eff_mass_Bkg->SetLineStyle(2);
  histo_eff_tot_Bkg->SetLineStyle(1);

  histo_eff_tot_Data->SetFillColor(kBlue);
  histo_eff_tot_Data->SetFillStyle(3004);  
  histo_eff_tot_Bkg->SetFillColor(kRed);
  histo_eff_tot_Bkg->SetFillStyle(3004);

  /*
  TH1D *histo_efficiency_NsubjettinessCut = new TH1D("histo_efficiency_NsubjettinessCut","histo_efficiency_NsubjettinessCut",50,0.5,1);
  TH1D *histo_err_efficiency_NsubjettinessCut = new TH1D("histo_err_efficiency_NsubjettinessCut","histo_err_efficiency_NsubjettinessCut",50,0,0.05);

  TH1D *histo_efficiency_massCut = new TH1D("histo_efficiency_massCut","histo_efficiency_massCut",50,0.5,1);
  TH1D *histo_err_efficiency_massCut = new TH1D("histo_err_efficiency_massCut","histo_err_efficiency_massCut",50,0,0.05);

  TH1D *histo_efficiency_tot = new TH1D("histo_efficiency_tot","histo_efficiency_tot",50,0.5,1);
  TH1D *histo_err_efficiency_tot = new TH1D("histo_err_efficiency_tot","histo_err_efficiency_tot",50,0,0.05);
  */

  histo_eff_nsub_Bkg->GetXaxis()->SetTitle("Efficiency");
  histo_eff_nsub_Bkg->GetXaxis()->SetRangeUser(0.3,1.1);

  histo_eff_nsub_Bkg->Draw();
  histo_eff_mass_Bkg->Draw("sames");
  histo_eff_tot_Bkg->Draw("HISTsames");

  histo_eff_nsub_Data->Draw("sames");
  histo_eff_mass_Data->Draw("sames");
  histo_eff_tot_Data->Draw("HISTsames");

  TLegend *leg = new TLegend(0.123563,0.567797,0.422414,0.868644);
  leg->SetFillColor(0);
  leg->AddEntry(histo_eff_nsub_Data,"Data - Nsub cut","l"); 
  leg->AddEntry(histo_eff_nsub_Bkg,"Bkg - Nsub cut","l"); 
  leg->AddEntry(histo_eff_mass_Data,"Data - Mass cut","l"); 
  leg->AddEntry(histo_eff_mass_Bkg,"Bkg - Mass cut","l"); 
  leg->AddEntry(histo_eff_tot_Data,"Data - Tot","f"); 
  leg->AddEntry(histo_eff_tot_Bkg,"Bkg - Tot","f"); 
  leg->Draw();


  // Calculate scale factor Data/MC
  double eff_Data = histo_eff_tot_Data->GetMean();
  double syst_eff_Data = histo_eff_tot_Data->GetRMS();
  double stat_eff_Data = histo_err_eff_tot_Data->GetMean();
  double err_eff_Data = sqrt(stat_eff_Data*stat_eff_Data + syst_eff_Data*syst_eff_Data);

  double eff_Bkg = histo_eff_tot_Bkg->GetMean();
  double syst_eff_Bkg = histo_eff_tot_Bkg->GetRMS();
  double stat_eff_Bkg = histo_err_eff_tot_Bkg->GetMean();
  double err_eff_Bkg = sqrt(stat_eff_Bkg*stat_eff_Bkg + syst_eff_Bkg*syst_eff_Bkg);

  double scaleFactor_data_over_MC = eff_Data / eff_Bkg;
  double err_scaleFactor_data_over_MC = sqrt( ( 1 / pow(eff_Bkg,2) ) * ( pow(err_eff_Data,2) ) + ( (pow(eff_Data,2)) / pow(eff_Bkg,4) ) * pow(err_eff_Bkg,4) );
  
  cout << "Efficiency in Data : " << eff_Data << " +/- " << stat_eff_Data << " (stat.) +/- " <<  syst_eff_Data << " (syst.)" << endl;
  cout << "Efficiency in MC : " << eff_Bkg << " +/- " << stat_eff_Bkg << " (stat.) +/- " <<  syst_eff_Bkg << " (syst.)" << endl;

  cout << "Data/MC scale factor : " << scaleFactor_data_over_MC << " +/- " << err_scaleFactor_data_over_MC << endl;
}
