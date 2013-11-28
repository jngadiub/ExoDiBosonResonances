{

  gStyle->SetPaintTextFormat(".2f");
  gStyle->SetOptStat(0);

  //==> denominator
  TFile *FileEffBulk = new TFile("/afs/cern.ch/work/s/santanas/Releases/CMSSW_5_3_9_CMGrel_V5_15_0_ExoDiBosonResonances_GIT_production/CMSSW_5_3_9/src/ExoDiBosonResonances/EDBRCommon/test/eff/plotsEff_BulkG_c0p2_plus_wideRes_final_05_11_2013/efficiency_WW_forClosure.root","READ"); //bulk
  //TFile *FileEffBulk = new TFile("/afs/cern.ch/work/s/santanas/Releases/CMSSW_5_3_9_CMGrel_V5_15_0_ExoDiBosonResonances_GIT_production/CMSSW_5_3_9/src/ExoDiBosonResonances/EDBRCommon/test/eff/plotsEff_RSG_Madgraph_final_05_11_2013/efficiency_WW_forClosure.root","READ"); //rs madgraph
  
  //==> numerator
  //TFile *FileEffRS = new TFile("/afs/cern.ch/work/s/santanas/Releases/CMSSW_5_3_9_CMGrel_V5_15_0_ExoDiBosonResonances_GIT_production/CMSSW_5_3_9/src/ExoDiBosonResonances/EDBRCommon/test/eff/plotsEff_RSG_c0p2_final_05_11_2013/efficiency_WW_forClosure.root","READ"); //rs pythia
  TFile *FileEffRS = new TFile("/afs/cern.ch/work/s/santanas/Releases/CMSSW_5_3_9_CMGrel_V5_15_0_ExoDiBosonResonances_GIT_production/CMSSW_5_3_9/src/ExoDiBosonResonances/EDBRCommon/test/eff/plotsEff_RSG_Madgraph_final_05_11_2013/efficiency_WW_forClosure.root","READ"); //rs madgraph
  



  TH2D *eff_ele_Bulk = (TH2D*) FileEffBulk.Get("eff_ele");
  TH2D *eff_mu_Bulk = (TH2D*) FileEffBulk.Get("eff_mu");
  TH2D *eff_jet_Bulk = (TH2D*) FileEffBulk.Get("eff_jet");

  TH2D *eff_ele_RS = (TH2D*) FileEffRS.Get("eff_ele");
  TH2D *eff_mu_RS = (TH2D*) FileEffRS.Get("eff_mu");
  TH2D *eff_jet_RS = (TH2D*) FileEffRS.Get("eff_jet");  

  eff_ele_RS->GetZaxis()->SetRangeUser(0.5,1.1);
  eff_mu_RS->GetZaxis()->SetRangeUser(0.5,1.1);
  eff_jet_RS->GetZaxis()->SetRangeUser(0.5,1.1);

  eff_ele_RS->SetMarkerSize(1.2);
  eff_mu_RS->SetMarkerSize(1.2);
  eff_jet_RS->SetMarkerSize(1.2);

  //Ratios
  eff_ele_RS->Divide(eff_ele_Bulk);
  eff_mu_RS->Divide(eff_mu_Bulk);
  eff_jet_RS->Divide(eff_jet_Bulk);

  TCanvas c1("c1","c1",600,600);
  eff_ele_RS->Draw("colz,TEXT90");
  c1.SaveAs("eff_ele_RSOverBulk.eps");

  TCanvas c2("c2","c2",600,600);
  eff_mu_RS->Draw("colz,TEXT90");
  c2.SaveAs("eff_mu_RSOverBulk.eps");

  TCanvas c3("c3","c3",600,600);
  eff_jet_RS->Draw("colz,TEXT90");
  c3.SaveAs("eff_jet_RSOverBulk.eps");  

}
