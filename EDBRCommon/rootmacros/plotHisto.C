#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "THStack.h"

class EDBRHistoPlotter {
public:
  TFile* fileData_;
  TFile* fileWjets_;
  TFile* fileDYjets_;
  TFile* filettbar_;
  TFile* fileWWjets_;
  TFile* fileWZjets_;
  TFile* fileZZjets_;

  double dataIntegral_;
  double WjetsIntegral_;
  double DYjetsIntegral_;
  double ttbarIntegral_;
  double WWjetsIntegral_;
  double WZjetsIntegral_;
  double ZZjetsIntegral_;

  double targetLumi_;

  int EDBRColors[6];
  
  bool scaleToData_;
  
  EDBRHistoPlotter(char* nameFileData,
		   char* nameFileWjets,
		   char* nameFileDYjets,
		   char* nameFilettbar,
		   char* nameFileWWjets,
		   char* nameFileWZjets,
		   char* nameFileZZjets,
		   double targetLumi,
		   bool scaleToData
		   ) 
  {
    fileData_ = TFile::Open(nameFileData);
    fileWjets_ = TFile::Open(nameFileWjets);
    fileDYjets_ = TFile::Open(nameFileDYjets);
    filettbar_ = TFile::Open(nameFilettbar);
    fileWWjets_ = TFile::Open(nameFileWWjets);
    fileWZjets_ = TFile::Open(nameFileWZjets);
    fileZZjets_ = TFile::Open(nameFileZZjets);
    targetLumi_ = targetLumi;
    scaleToData_ = scaleToData;

    /// Long live C++!!!
    EDBRColors[0] = kOrange-9;
    EDBRColors[1] = kRed-7;
    EDBRColors[2] = kGray+2;
    EDBRColors[3] = kBlue-9;
    EDBRColors[4] = kMagenta-9;
    EDBRColors[5] = kGreen-3;  
  }

  /// Functions
  void makeStackPlots(std::string histoName);
};

void EDBRHistoPlotter::makeStackPlots(std::string histoName) {

  TCanvas* cv = new TCanvas("cv","cv",600,600);

  TH1D* h_DYjets = (TH1D*)fileData_->Get(histoName.c_str())->Clone(); 
  TH1D* h_Wjets =  (TH1D*)fileWjets_->Get(histoName.c_str())->Clone(); 
  TH1D* h_ttbar =  (TH1D*)fileData_->Get(histoName.c_str())->Clone(); 
  TH1D* h_WWjets = (TH1D*)fileData_->Get(histoName.c_str())->Clone(); 
  TH1D* h_WZjets = (TH1D*)fileData_->Get(histoName.c_str())->Clone(); 
  TH1D* h_ZZjets = (TH1D*)fileData_->Get(histoName.c_str())->Clone(); 

  TH1D* h_data =  (TH1D*)fileData_->Get(histoName.c_str())->Clone();
  
  dataIntegral_ = h_data->Integral();
  DYjetsIntegral_ = h_DYjets->Integral();
  WjetsIntegral_ = h_Wjets->Integral();
  ttbarIntegral_ = h_ttbar->Integral();
  WWjetsIntegral_ = h_WWjets->Integral();
  WZjetsIntegral_ = h_WZjets->Integral();
  ZZjetsIntegral_ = h_ZZjets->Integral();

  TH1D* h_sum = (TH1D*)h_DYjets->Clone("h_sum");
  h_sum->Reset();
  h_sum->Add(h_DYjets);
  h_sum->Add(h_Wjets);
  h_sum->Add(h_ttbar);
  h_sum->Add(h_WWjets);
  h_sum->Add(h_WZjets);
  h_sum->Add(h_ZZjets);
  
  h_DYjets->SetFillColor(EDBRColors[0]);
  h_Wjets->SetFillColor(EDBRColors[1]);
  h_ttbar->SetFillColor(EDBRColors[2]);
  h_WWjets->SetFillColor(EDBRColors[3]);
  h_WZjets->SetFillColor(EDBRColors[4]);
  h_ZZjets->SetFillColor(EDBRColors[5]);

  h_data->SetMarkerSize(0.7);
  h_data->SetMarkerStyle(21);
  
  if(scaleToData_) {    
    double totalBackgroundIntegral = h_sum->Integral();
    h_DYjets->Scale(dataIntegral_/totalBackgroundIntegral);
    h_Wjets->Scale(dataIntegral_/totalBackgroundIntegral);
    h_ttbar->Scale(dataIntegral_/totalBackgroundIntegral);
    h_WWjets->Scale(dataIntegral_/totalBackgroundIntegral);
    h_WZjets->Scale(dataIntegral_/totalBackgroundIntegral);
    h_ZZjets->Scale(dataIntegral_/totalBackgroundIntegral);
    h_sum->Scale(dataIntegral_/totalBackgroundIntegral);
  }
  else {
    h_DYjets->Scale(targetLumi_);
    h_Wjets->Scale(targetLumi_);
    h_ttbar->Scale(targetLumi_);
    h_WWjets->Scale(targetLumi_);
    h_WZjets->Scale(targetLumi_);
    h_ZZjets->Scale(targetLumi_);
    h_sum->Scale(targetLumi_);
  }
  
  THStack* hs = new THStack("hs","");
  hs->Add(h_DYjets);
  hs->Add(h_Wjets);
  hs->Add(h_ttbar);
  hs->Add(h_WWjets);
  hs->Add(h_WZjets);
  hs->Add(h_ZZjets);
  
  hs->Draw("HIST");
  h_data->Draw("SAME E1");
  
  cv->Draw();

  // Save the file as a PDF
  char buffer[256];
  sprintf(buffer,"%s.pdf",histoName.c_str());
  cv->SaveAs(buffer);

  hs->Delete();
  h_sum->Delete();
  cv->Delete();
}
