#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TSystem.h"

#include <assert.h>
#include <vector>
#include <string>

#include "CMSLabels.h"

class EDBRHistoPlotter {
public:
  
  EDBRHistoPlotter(std::string nameInDir,
		   std::vector<std::string> nameFileData,
		   std::vector<std::string> nameFileMC,
		   double targetLumi,
		   bool scaleToData
		   ) 
  {

    init(nameInDir,nameFileData,nameFileMC,targetLumi,scaleToData);

  }//end constructor
    
  virtual ~EDBRHistoPlotter(){
  }//end destructor
  
  /// Functions
  
  ///init() function to be called from constructor and 
  ///set members to reasonable values.
  void init(std::string nameInDir,
	    std::vector<std::string> nameFileData,
	    std::vector<std::string> nameFileMC,
	    double targetLumi,
	    bool scaleToData
	    ){   
    
    inDir_=nameInDir;
    char buffer[256];

    nDATASamples_=int(nameFileData.size());
    for(int i=0;i<nDATASamples_;i++){
      sprintf(buffer,"%s%s",inDir_.c_str(),(nameFileData[i]).c_str());
      printf("Opening file: %s\n",buffer);
      fdata.push_back(TFile::Open(buffer));
    }

    nMCSamples_=int(nameFileMC.size());
    for(int i=0;i<nMCSamples_;i++){
     sprintf(buffer,"%s%s",inDir_.c_str(),(nameFileMC[i]).c_str());
     printf("Opening file: %s\n",buffer);
     fmc.push_back(TFile::Open(buffer));
    }

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

  ///make histogram stacks. See below.
  void makeStackPlots(std::string histoName);
  
  ///set output directories for plots.
  void setOutDir(std::string outDirNew){
    char buffer[256];
    outDir_=outDirNew;
    
    sprintf(buffer,"%s/pdf",outDir_.c_str());
    printf("%s\n",buffer);
    gSystem->mkdir(buffer,true);

    sprintf(buffer,"%s/root",outDir_.c_str());
    printf("%s\n",buffer);
    gSystem->mkdir(buffer,true);

    sprintf(buffer,"%s/png",outDir_.c_str());
    printf("%s\n",buffer);
    gSystem->mkdir(buffer,true);

  }

  ///get reasonable colors for stacks.
  int getFillColor(int index){
    if(index<6)return EDBRColors[index];
    return kWhite;
  }

  /// Members

private:
  std::string inDir_;
  std::string outDir_;

  std::vector<TFile*> fmc;
  std::vector<TFile*> fdata;
  int nMCSamples_;
  int nDATASamples_;

  double dataIntegral_;

  double targetLumi_;

  int EDBRColors[6];
  
  bool scaleToData_;
};




///----------------------------------------------------
/// Makes a stack of all histograms with name histoName
/// available in the internal array of files.
///----------------------------------------------------
void EDBRHistoPlotter::makeStackPlots(std::string histoName) {
  
  // Always useful
  char buffer[256];

  TCanvas* cv = new TCanvas(("cv_"+histoName).c_str(),("cv_"+histoName).c_str(),600,600);

  TFile* tempFile=NULL;
  TH1D*  tempHisto=NULL;

  /// Fill a vector of histograms to stack
  std::vector<TH1D*> h_mc;
  std::vector<TH1D*> h_data;
  TH1D* hSumMC=NULL;
  TH1D* hSumDATA=NULL;

  // Loop over files, clone histograms and add them to the vectors.
  const int nMC=nMCSamples_;

  for(int is=0;is<nMC;is++){
    tempFile = fmc.at(is);
    //the EDBRHistoMaker prepares MC histos normalized all to the same lumi of 1 /fb
    // isn't it picobarn???
    tempHisto = ((TH1D*)fmc.at(is)->Get(histoName.c_str())->Clone());
    tempHisto->SetDirectory(0);
    std::string tmp1 = "h_nVtx";
    std::string tmp2 = histoName.c_str();
   if(tmp1==tmp2)
     printf("This number of vertices histogram has %i entries\n",(int)tempHisto->GetEntries());
    h_mc.push_back(tempHisto);
    h_mc[is]->SetFillColor(getFillColor(is));
    
    if(is==0)
      hSumMC = ((TH1D*)fmc[is]->Get(histoName.c_str())->Clone());
    else {
      hSumMC->Add(h_mc[is] );
    }
    
  }//end loop on MC samples

  dataIntegral_ =1.0;
  const int nDATA=nDATASamples_;

  for(int is=0;is<nDATA;is++){
    tempFile = fdata.at(is);
    tempHisto = ((TH1D*)fdata.at(is)->Get(histoName.c_str())->Clone());
    tempHisto->SetDirectory(0);
    h_data.push_back(tempHisto);
    
    if(is==0)
      hSumDATA = ((TH1D*)fdata[is]->Get(histoName.c_str())->Clone());
    else {
      hSumDATA->Add(h_data[is] );
    }

  }
  //end loop on data samples

  /// Nice style for data
  if(nDATA>0){
    hSumDATA->SetMarkerSize(0.7);
    hSumDATA->SetMarkerStyle(21);  
    dataIntegral_ = hSumDATA->Integral();
  }

  /// Scale to data or to lumi?
  if(scaleToData_) {    
    double totalBackgroundIntegral = hSumMC->Integral();
    for(int is=0;is<nMC;is++){
      h_mc[is]->Scale(dataIntegral_/totalBackgroundIntegral);
    }
  }
  else {
    for(int is=0;is<nMC;is++){
      h_mc[is]->Scale(targetLumi_);
    }
  }

  //make a THStack of the background
  //for the titles get them from the first MC histo, it is good enough.
  //or no title, if you prefer.
  sprintf(buffer,"%s;%s;%s;",
	  "", 
	  //h_mc[0]->GetTitle(),
	  h_mc[0]->GetXaxis()->GetTitle(),
	  h_mc[0]->GetYaxis()->GetTitle()
	  );

  THStack *hsbkgd=new THStack("allBkgd",buffer);
  for(int i=0;i<nMC;i++){
    hsbkgd->Add(h_mc[i]);
  }

  hsbkgd->Draw("HIST");
  if(nDATA>0) hSumDATA->Draw("SAME E1");

  // Let's make such that the histogram has some white space on top.
  // This needs a bit of magic...
  double maxMC = hSumMC->GetMaximum();
  double maxData = hSumDATA->GetMaximum();
  double histoMax = (maxMC > maxData? maxMC : maxData);
  double newHistoMax = histoMax*1.1;
  hsbkgd->GetHistogram()->GetYaxis()->SetRangeUser(0,newHistoMax);
  hsbkgd->SetMaximum(newHistoMax);

  // For the legend, we have to tokenize the name "histos_XXX.root"
  // to get the XXX... yuck.
  TLegend* leg = new TLegend(0.7,0.7,0.9,0.9);
  if(nDATA > 0) leg->AddEntry(hSumDATA,"Data","p");
  for(int i=0;i<nMC;i++){
    TString s1 = fmc[i]->GetName();
    TString s2 = "_.";
    TObjArray* tokens = s1.Tokenize(s2);
    leg->AddEntry(h_mc[i],((TObjString*)(tokens->At(1)))->String().Data(),"f");
  }
  leg->SetFillColor(kWhite);
  
  cv->Draw();
  leg->Draw();
  
  // Nice labels
  TLatex* l = makeCMSPreliminaryTop(8);
  l->Draw();
  
  // Save the file as a PDF
  sprintf(buffer,"%s/pdf/can_%s.pdf",outDir_.c_str(),histoName.c_str());
  cv->SaveAs(buffer);
  sprintf(buffer,"%s/png/can_%s.png",outDir_.c_str(),histoName.c_str());
  cv->SaveAs(buffer);
  sprintf(buffer,"%s/root/can_%s.root",outDir_.c_str(),histoName.c_str());
  cv->SaveAs(buffer);

  delete leg;
  delete hsbkgd;
  delete hSumMC; 
  delete hSumDATA;
  for(int is=0;is<nMC;is++) delete h_mc[is];
 
  delete cv;
}
