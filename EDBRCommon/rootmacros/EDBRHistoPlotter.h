#include <string>
#include <vector>
#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "CMSLabels.h"

class EDBRHistoPlotter {
public:
  
  EDBRHistoPlotter(std::string nameInDir,
		   std::vector<std::string> nameFileDATA,
		   std::vector<std::string> nameFileMC, 
		   double targetLumi,
		   double kFactor,
		   int wantNXJets,
		   int flavour,
		   bool scaleToData)
  {
    nameInDir_    = nameInDir;
    fileNamesMC   = nameFileMC;
    fileNamesDATA = nameFileDATA;
    kFactor_      = kFactor;
    targetLumi_   = targetLumi;
    wantNXJets_   = wantNXJets;
    flavour_      = flavour;
    scaleToData_  = scaleToData;
    debug_        = false;
    EDBRColors.resize(20,kWhite);
    labels.resize(0);
    makeLabels();
    printf("Target lumi is %g pb-1\n",targetLumi);
    printf("k factor is %g\n",kFactor);
  }//end constructor
  
  virtual ~EDBRHistoPlotter(){
  }//end destructor
  
  /// Members
  std::vector<std::string> fileNamesMC;
  std::vector<std::string> fileNamesDATA;
  std::vector<std::string> labels;
  std::vector<TFile*>      filesMC;
  std::vector<TFile*>      filesDATA;
  std::vector<TH1D*>       histosMC;
  std::vector<TH1D*>       histosDATA;
  std::vector<int>         EDBRColors;

  std::string nameInDir_;
  std::string nameOutDir_;
  double dataIntegral_;
  double targetLumi_;
  double kFactor_;
  int    wantNXJets_;
  int    flavour_;
  bool   scaleToData_;
  bool   debug_;

  /// Functions
  void cleanupMC();
  void cleanupDATA();
  void makeLabels();
  void makeStackPlots(std::string histoName);
  void setOutDir(std::string outDirNew);

  /// set debug mode
  void setDebug(bool debug){debug_=debug;}
  
  /// get reasonable colors for stacks.
  int getFillColor(int index){
    if(index < 20)return EDBRColors[index];
    return kWhite;
  }
  
  /// set reasonable colors for stacks.
  void setFillColor(std::vector<int> colorList){
    unsigned int ind=0;
    while (ind < 20 && ind < colorList.size()){
      EDBRColors.at(ind)=colorList.at(ind);
      ind++;
    }
  }

};

void EDBRHistoPlotter::cleanupMC() {
  for(size_t i=0; i!= filesMC.size(); ++i) {
    filesMC.at(i)->Close();
  }
  filesMC.clear();  

  for(size_t i=0; i!= histosMC.size(); ++i) {
    histosMC.at(i)->Delete();
  }
  histosMC.clear();
}

void EDBRHistoPlotter::cleanupDATA() {
  for(size_t i=0; i!= filesDATA.size(); ++i) {
    filesDATA.at(i)->Close();
  }
  filesDATA.clear();  

  for(size_t i=0; i!= histosDATA.size(); ++i) {
    histosDATA.at(i)->Delete();
  }
  histosDATA.clear();
}

void EDBRHistoPlotter::makeLabels() {
  for(size_t i = 0; i != fileNamesMC.size(); i++){
    TString s1 = fileNamesMC.at(i);
    TString s2 = "_.";
    TObjArray* tokens = s1.Tokenize(s2);
    std::string aLabel = ((TObjString*)(tokens->At(1)))->String().Data();
    labels.push_back(aLabel);
  }
}

///set output directories for plots.
void EDBRHistoPlotter::setOutDir(std::string outDirNew){
  char buffer[256];
  nameOutDir_=outDirNew;
  
  sprintf(buffer,"%s/pdf",nameOutDir_.c_str());
  printf("%s\n",buffer);
  gSystem->mkdir(buffer,true);
  
  sprintf(buffer,"%s/root",nameOutDir_.c_str());
  printf("%s\n",buffer);
  gSystem->mkdir(buffer,true);
  
  sprintf(buffer,"%s/png",nameOutDir_.c_str());
  printf("%s\n",buffer);
  gSystem->mkdir(buffer,true);
}

void EDBRHistoPlotter::makeStackPlots(std::string histoName) {
  
  cleanupMC();
  cleanupDATA();
  
  //printf("Making histo %s\n",histoName.c_str());
  std::cout<<"\rMaking histo "<<histoName.c_str() << std::endl;

  TCanvas* cv = new TCanvas(("cv_"+histoName).c_str(),("cv_"+histoName).c_str(),600,600);

  ///--------------------
  /// Make the DATA stack
  ///--------------------

  TH1D* sumDATA = NULL;

  for(size_t i=0; i!= fileNamesDATA.size(); ++i) {
    filesDATA.push_back(TFile::Open((nameInDir_+
				     fileNamesDATA.at(i)).c_str()));
  }
  
  for(size_t i=0; i!= filesDATA.size(); ++i) {
    TH1D* histo = (TH1D*)(filesDATA.at(i)->Get(histoName.c_str())->Clone("clone"));
    histo->SetDirectory(0);
    histosDATA.push_back(histo);
  }
  
  if(histosDATA.size() !=0) {
    sumDATA = (TH1D*)(histosDATA.at(0)->Clone("master"));
    sumDATA->Reset();
    sumDATA->SetDirectory(0);
  }

  for(size_t i=0; i!= histosDATA.size(); ++i) {
    sumDATA->Add(histosDATA.at(i));
  }
  
  double sumDataIntegral = sumDATA->Integral();

  if(debug_) {
    printf("sumDataIntegral = %g\n",sumDataIntegral);
  }
  ///------------------
  /// Make the MC stack
  ///------------------

  TH1D* sumMC = NULL;
  double sumBkgAtTargetLumi = 0;
  
  for(size_t i=0; i!= fileNamesMC.size(); ++i) {
    filesMC.push_back(TFile::Open((nameInDir_+
				   fileNamesMC.at(i)).c_str()));
  }
  
  for(size_t i=0; i!= filesMC.size(); ++i) {
    TH1D* histo = (TH1D*)(filesMC.at(i)->Get(histoName.c_str())->Clone(labels.at(i).c_str()));
    histo->SetDirectory(0);
    histo->SetFillColor(getFillColor(i));

    /// This is important. If the user has given a k-factor, it means
    /// they INCONDITIONALLY want to multiply all MC histograms by
    /// this number. So we just do it here.
    /// Although, it shold be a different kFactor for EACH bacground.
    /// TODO: implement different kFactors for each MC.
    histo->Scale(kFactor_);
    
    if(debug_) {
      histo->Print();
    }
    sumBkgAtTargetLumi += (histo->Integral() * targetLumi_);
    histosMC.push_back(histo);
  }
  
  if(debug_) {
    printf("sumBkgAtTargetLumi = %g\n",sumBkgAtTargetLumi);
    printf("sumDataIntegral = %g\n",sumDataIntegral);
    printf("Scale factor = %g\n",sumDataIntegral/sumBkgAtTargetLumi);
  }
  
  if(histosMC.size() !=0) {
    sumMC = (TH1D*)(histosMC.at(0)->Clone("master"));
    sumMC->Reset();
    sumMC->SetDirectory(0);
  }

  /// Do we normalize to data or to lumi?
  /// NOTICE THAT THIS DEPENDS ON THE HISTOGRAMS HAVING BEING
  /// CORRECTLY FILLED WITH PUweight*LumiWeight*GenWeight
  for(size_t is=0; is!=histosMC.size(); is++){
    if(debug_)
      printf("This histogram has integral %g\n",histosMC.at(is)->Integral());
    if(scaleToData_) {
      histosMC.at(is)->Scale(targetLumi_*
			     sumDataIntegral/sumBkgAtTargetLumi);
    }
    else { 
      histosMC.at(is)->Scale(targetLumi_);
    }
    if(debug_)
      printf("After scaling this histogram has integral %g\n",histosMC.at(is)->Integral());
  }
  
  THStack* hs = new THStack("hs","");

  // Make a histogram just for the sum
  for(size_t i=0; i!= histosMC.size(); ++i) {
    sumMC->Add(histosMC.at(i));
    hs->Add(histosMC.at(i));
  }

  if(debug_) {
    printf("After scaling, sum of backgrounds = %g\n",sumMC->Integral());
    printf("Sum of data is still %g\n",sumDATA->Integral());
  }

  
  sumMC->SetFillStyle(0);
  sumMC->SetLineColor(kBlack);
  sumMC->SetLineWidth(1);
  
  ///-----------------------------------
  /// Draw both MC and DATA in the stack
  ///-----------------------------------
  hs->Draw("HIST");
  hs->GetXaxis()->SetTitle(histoName.c_str());
  hs->GetYaxis()->SetTitle("Number of Events");
  hs->GetYaxis()->SetTitleOffset(1.15);
  hs->GetYaxis()->CenterTitle();
  double maximumMC = 1.15*sumMC->GetMaximum();
  double maximumDATA = 1.15*sumDATA->GetMaximum();
  double maximumForStack = (maximumMC>maximumDATA?maximumMC:maximumDATA);
  hs->SetMaximum(maximumForStack);
  hs->SetMinimum(0.1);
  hs->Draw("HIST");
  sumMC->Draw("HISTO SAME");    
  sumDATA->Draw("SAME E1");

  
  // For the legend, we have to tokenize the name "histos_XXX.root"
  TLegend* leg = new TLegend(0.7,0.7,0.9,0.9);
  if( histosDATA.size() > 0)
    leg->AddEntry(sumDATA,"Data","p");
  for(size_t i = 0; i!= histosMC.size(); ++i)
    leg->AddEntry(histosMC.at(i),labels.at(i).c_str(),"f");

  leg->SetFillColor(kWhite);
  leg->Draw();

  // Nice labels
  TLatex* l = makeCMSPreliminaryTop(8,0.10,0.935);
  l->Draw();
  l = makeCMSLumi(19.6,0.5,0.935);
  l->Draw();
  l = makeChannelLabel(wantNXJets_,flavour_);
  l->Draw();

  // Save the picture
  char buffer[256];
  cv->SetLogy(false);
  sprintf(buffer,"%s/pdf/can_%s.pdf",nameOutDir_.c_str(),histoName.c_str());
  cv->SaveAs(buffer);
  sprintf(buffer,"%s/png/can_%s.png",nameOutDir_.c_str(),histoName.c_str());
  cv->SaveAs(buffer);
  sprintf(buffer,"%s/root/can_%s.root",nameOutDir_.c_str(),histoName.c_str());
  cv->SaveAs(buffer);
  cv->SetLogy(true);
  sprintf(buffer,"%s/pdf/LOG_can_%s.pdf",nameOutDir_.c_str(),histoName.c_str());
  cv->SaveAs(buffer);
  sprintf(buffer,"%s/png/LOG_can_%s.png",nameOutDir_.c_str(),histoName.c_str());
  cv->SaveAs(buffer);
  sprintf(buffer,"%s/root/LOG_can_%s.root",nameOutDir_.c_str(),histoName.c_str());
  cv->SaveAs(buffer);

  if(debug_) {
    printf("***********************\n");
  }
}
