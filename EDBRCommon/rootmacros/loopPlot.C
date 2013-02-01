#include <Riostream.h>
#include <vector>
#include <string>

#include "TROOT.h"
#include "TFile.h"
#include "TList.h"
#include "TIter.h"
//#include "EDBRHistoMaker.h"
//#include "EDBRHistoPlotter.h"

void loopPlot(){

  gROOT->ProcessLine(".L makeHisto.C+");
  gROOT->ProcessLine(".L plotHisto.C+");

  std::string pathToTrees="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1/";

  const int nDATA=3;//set to zero if you don't want to plot
  std::string dataLabels[nDATA]={"DataDoubleMu1","DataDoubleMu2","DataDoubleEle7"};//examples for now
  std::vector<std::string> fData;
  for(int ii=0;ii<nDATA;ii++){
    fData.push_back(pathToTrees+"treeEDBR_"+dataLabels[ii]+".root");
  }

  const int nMC=4;//set to zero if you don't want to plot
  std::string mcLabels[nMC]={"TTBAR","DYJetsPt50To70","DYJetsPt70To100","DYJetsPt100"};//examples for now
  std::vector<std::string> fMC;
  for(int ii=0;ii<nMC;ii++){
    fMC.push_back(pathToTrees+"treeEDBR_"+mcLabels[ii]+".root");
  }

  //loop over data files and make histograms individually for each of them
  for(int i=0;i<nDATA;i+){
    TTree *tdata=(TTree*)fData->at(i)->Get("SelectedCandidates");
    EDBRHistoMaker* maker = new EDBRHistoMaker(tdata);
    maker->Loop(("histos_"+dataLabels[i]+".root").c_str());
    delete maker;
  }//end loop on data files

  //loop over MC files and make histograms individually for each of them
  for(int i=0;i<nMC;i+){
    TTree *tmc=(TTree*)fMC->at(i)->Get("SelectedCandidates");
    EDBRHistoMaker* maker = new EDBRHistoMaker(tmc);
    maker->Loop(("histos_"+mcLabels[i]+".root").c_str());
    delete maker;
  }//end loop on data files


  //make nice plots
  EDBRHistoPlotter *plotter=new EDBRHistoPlotter("./",fData,fMC,19.6,false);
  plotter->setOutDir("./test_outplots");

  if(nMC>0){
    //loop over all the histograms inheriting from TH1 contained in the file 
    TIter next(fMC->at(0)->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)next())) {
      TClass *cl = gROOT->GetClass(key->GetClassName());
      if (!cl->InheritsFrom("TH1")) continue;
      TH1 *hTMP = (TH1*)key->ReadObj();
      std::string hName=hTMP->GetName();
      std::cout<<"Plotting histo "<<hName.c_str()<<std::endl;
      plotter->makeStackPlots(hName);      
    }//end while loop
  }//end if fmc size >0

 
  delete plotter;


}//end main
