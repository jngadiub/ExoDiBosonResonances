#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TSystem.h"

#include <assert.h>
#include <vector>
#include <string>
#include <sstream>
#include <Riostream.h>
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
    TFile* aFile=NULL;

    nDATASamples_=int(nameFileData.size());
    for(int i=0;i<nDATASamples_;i++){
      sprintf(buffer,"%s%s",inDir_.c_str(),(nameFileData[i]).c_str());
      printf("Opening file: %s\n",buffer);
      aFile = TFile::Open(buffer);
      printf("This file is at pointer %i\n",aFile);
      fdata.push_back(aFile);
      
    }

    nMCSamples_=int(nameFileMC.size());
    printf("nMCSamples is %i\n",nMCSamples_);
    fmc.reserve(8);
    for(int i=0;i<nMCSamples_;i++){
     sprintf(buffer,"%s%s",inDir_.c_str(),(nameFileMC[i]).c_str());
     printf("Opening file: %s\n",buffer);
     aFile = TFile::Open(buffer);
     aFile->Print();
     printf("This file is at pointer %i\n",aFile);
     fmc.push_back(aFile);

     
     //TH1D* FUCKINGHISTO = (TH1D*)fmc.at(i)->Get("h_ptZll");
     //printf("Got histo %s\n",FUCKINGHISTO->GetName());
     //printf("It has entries %i\n",(int)FUCKINGHISTO->GetEntries());
     //printf("It has Mean %g\n",FUCKINGHISTO->GetMean());
     //printf("It has RMS %g\n",FUCKINGHISTO->GetRMS());
    }
    
    /// UGLY HACK
    TFile* nullFile = NULL;
    fmc.push_back(nullFile);
    
    targetLumi_ = targetLumi;
    scaleToData_ = scaleToData;

    /// Long live C++!!!
    EDBRColors.resize(20,kWhite);
    EDBRColors.at(0)= kOrange-9;
    EDBRColors.at(1)= kRed-7;
    EDBRColors.at(2)=kGray+2;
    EDBRColors.at(3)=kBlue-9;
    EDBRColors.at(4)= kMagenta-9;
    EDBRColors.at(5)= kGreen-3;  
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
    if(index<7)return EDBRColors[index];
    return kWhite;
  }

  void setFillColor(std::vector<int> colorList){
    unsigned int ind=0;
    while (ind<20 && ind < colorList.size()){//max n color hard-coded down there
      EDBRColors.at(ind)=colorList.at(ind);
      ind++;
    }
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

  std::vector<int> EDBRColors;
  
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
  double nnn=((TH1D*)fmc.at(nMC-1)->Get("h_nVtx"))->GetEntries();

  for(int is=0;is<nMC;is++){
    
    tempFile = fmc.at(is);

    stringstream ss1;
    ss1<<is;
    string strNewName=histoName+"_"+ss1.str();
    tempHisto = (TH1D*)fmc.at(is)->Get(histoName.c_str())->Clone();//strNewName.c_str());
    tempHisto->SetDirectory(0);
    
    std::string tmp1 = "h_nVtx";
    std::string tmp2 = histoName.c_str();
    std::string tmp3 = "h_ptZll";

    if(tmp2 == tmp3) {
      printf("Trying to get histogram %s\n",histoName.c_str());
      printf("Histogram coming from file %s\n",tempFile->GetName());
      printf("tempFile at pointer %i\n",tempFile);
      printf("Got histo %s\n",tempHisto->GetName());
      printf("It has entries %i\n",(int)tempHisto->GetEntries());
      printf("It has Mean %g\n",tempHisto->GetMean());
      printf("It has RMS %g\n",tempHisto->GetRMS());
      printf("************************************************\n");
    }

    if(tmp1==tmp2) 
      std::cout<<fmc[is]->GetName()<<" : the histogram with the # of vertices has "
	       <<(int)tempHisto->GetEntries() <<" entries  ; "<<tempHisto->GetName()<<std::endl;

    h_mc.push_back(tempHisto);
    h_mc[is]->SetFillColor(getFillColor(is));
    
    if(is==0) {
      hSumMC = ((TH1D*)fmc[is]->Get(histoName.c_str())->Clone("RegularClone"));
    }
    else {
      hSumMC->Add((TH1D*)h_mc[is]->Clone("HatefulClone"));
    }
    
  }//end loop on MC samples

  dataIntegral_ =1.0;
  const int nDATA=nDATASamples_;

  for(int is=0;is<nDATA;is++){
    tempFile = fdata.at(is);
    tempHisto = ((TH1D*)fdata.at(is)->Get(histoName.c_str())->Clone());
    tempHisto->SetDirectory(0);
    h_data.push_back(tempHisto);
    
    if(is==0) {
      hSumDATA = ((TH1D*)fdata[is]->Get(histoName.c_str())->Clone());
    }
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
    string tmpstr1=h_mc[i]->GetName();
    if(tmpstr1=="h_ptZll")
      std::cout<<"Adding to THStack "<<tmpstr1.c_str()<<"  integral is "
	       <<h_mc[i]->Integral()<<"  Nentries="
	       <<h_mc[i]->GetEntries()<<std::endl;
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
