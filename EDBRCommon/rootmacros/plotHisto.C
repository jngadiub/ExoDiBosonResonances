#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "THStack.h"
#include <assert.h>
#include <vector>
#include <string>
#include <sys/stat.h>

class EDBRHistoPlotter {
public:
  
  EDBRHistoPlotter(std::string nameInDir="./",
		   std::vector<std::string> nameFileData,
		   std::vector<std::string> nameFileMC,
		   double targetLumi,
		   bool scaleToData
		   ) 
  {

    init(nameInDir,nameFileData,nameFileMC,,targetLumi,scaleToData);

    setOutDir("./");
    
  }


  virtual void ~EDBRHistoPlotter(){

    delete fileData_;
    delete  fileWjets_;
    delete  fileDYjets_;
    delete  filettbar_;
    delete  fileWWjets_;
    delete  fileWZjets_;
    delete  fileZZjets_;



  }//end destructor

  /// Functions

  void init(std::string nameInDir="./",
	    std::vector<std::string> nameFileData,
	    std::vector<std::string> nameFileMC,
	    double targetLumi,
	    bool scaleToData
	    ){   
    inDir_=nameInDir;
    nDATASamples_=int(nameFileData.size());
    for(int i=0;i<nDATASamples_;i++){
      fdata[i] = TFile::Open((inDir_+nameFileData[i]));
    }

    nMCSamples_=int(nameFileMC.size());
    for(int i=0;i<nMCSamples_;i++){
      fmc[i] = TFile::Open((inDir_+nameFileMC[i]));
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

  void makeStackPlots(std::string histoName);
  //  void setInDir(std::string inDirNew){inDir_=inDirNew;}//not really needed, passed through init()
  void setOutDir(std::string outDirNew){
    outDir_=outDirNew;
    mkdir(outDir_.c_str());
    assert( (mkdir((outDir_+"/pdf/").c_str()) !=0) );//if output dir exists already, do not overwrite and stop
    assert( (mkdir((outDir_+"/root/").c_str()) !=0) );
    assert( (mkdir((outDir_+"/png/").c_str()) !=0) );
  }

  int getFillColor(int index){
    if(index<6)return EDBRColors[index];
    return kWhite;
  }


private:
  std::string inDir_,outDir_;

  TFile* fmc[99],fdata[99];
  int nMCSamples_,nDATASamples_;

  double dataIntegral_;


  double targetLumi_;

  int EDBRColors[6];
  
  bool scaleToData_;


};

void EDBRHistoPlotter::makeStackPlots(std::string histoName) {

  TCanvas* cv = new TCanvas(("cv_"+histoName).c_str(),("cv_"+histoName).c_str(),600,600);


  const int nMC=nMCSamples_;
  TH1D *h_mc[nMC];
  TH1D *h_sum=NULL;
  for(int is=0;is<nMC;is++){
    //the EDBRHistoMaker prepares MC histos normalized all to the same lumi of 1 /fb
    if(is==0)h_mcsum= (TH1D*)fmc[is]->Get(histoName.c_str())->Clone() ;
    else{
      h_mc[is]= (TH1D*)fmc[is]->Get(histoName.c_str())->Clone()
      h_sum->Add(h_mc[is] );
      h_mc[is]->SetFillColor(getFillColor(is));
    }
  }//end loop on MC samples

  dataIntegral_ =1.0;
  const int nDATA=nDATASamples_;
  TH1D *h_data; 
  for(int is=0;is<nDATA;is++){
    if(is==0)h_data= (TH1D*)fdata[is]->Get(histoName.c_str())->Clone() ;
    else h_data->Add( (TH1D*)fdata[is]->Get(histoName.c_str())->Clone() );
  }

  if(nDATA>0){
    h_data->SetMarkerSize(0.7);
    h_data->SetMarkerStyle(21);  
    dataIntegral_ = h_data->Integral();
  }

  if(scaleToData_) {    
    double totalBackgroundIntegral = h_sum->Integral();
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
  THStack *hsbkgd=new THStack("allBkgd",plname.c_str());
  for(int i=0;i<nMC;i++){
    hsbkgd->Add(h_mc[i]);
  }
  
  hsbkgd->Draw("HIST");
  if(nDATA>0) h_data->Draw("SAME E1");
  
  cv->Draw();

  // Save the file as a PDF
  cv->SaveAs( (outDir_+"/pdf/can_"+histoName+".pdf").c_str()  );
  cv->SaveAs( (outDir_+"/png/can_"+histoName+".png").c_str()  );
  cv->SaveAs( (outDir_+"/root/can_"+histoName+".root").c_str()  );

  delete hsbkgd;
  delete h_sum; delete h_data;
  for(int is=0;is<nMC;is++) delete h_mc[is];
 
  delete cv;
}
