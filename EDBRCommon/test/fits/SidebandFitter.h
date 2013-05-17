#ifndef SidebandFitter_h
#define SidebandFitter_h

#include <string>
#include "TTree.h"
#include "TH1D.h"
#include "TRandom3.h"

#include "RooFitResult.h"
#include "RooDataSet.h"
#include "RooProdPdf.h"
#include "RooCBShape.h"
#include "RooWorkspace.h"

#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"

//#include "binningFits_XWW.h"
#include "binningFits_XZZ.h"

class SidebandFitter {

 public:

  SidebandFitter(  const std::string& PUType );
  ~SidebandFitter() {};

  RooWorkspace* getAlphaFit( TTree* treeMC,int nxjCategory,const std::string& leptType_str, int purCut, bool withRooFit );

  
  RooFitResult* fitSidebands( TTree* treeMC, TTree* treeDATA, int nxjCategory, const std::string& leptType, TH1D* h1_alpha );

  std::string get_fitResultsName( int nxj, const std::string& init );
  std::string get_fitResultsRootFileName( int nxjCategory, const std::string& leptType );

  std::string get_outdir();
  void setOutDir(string new_outdir);
  void setOutFile(string new_outfile);
  void setCanvasLabel(string new_label);
  TH1D* shuffle( TH1D* inhist, TRandom3* random, char *histName, TH1D* inhist2 = 0 );
  TH1D* shuffle( TH1D* inhist, TRandom3* random, char *histName, TH1D* madgr , TH1D* sherpa );
  TH1D* shufflePoisson( TH1D* inhist, TRandom3* random, char *histName);
  TH1D* dummyAlphaHist( float alpha , TH1D* inhist , char* histName );
  void alphaFit( TH1D* alpha_hist , std::vector<double> & fitpars, std::vector<double> & fiterrs);
  std::string getFitFunc(std::string modifier){return fitfuncName_+modifier;}
  void bkgdFitIterative(TTree *treeMC, RooRealVar& mZZ, int nxjCategory, const std::string& leptType_str);
//   TTree* correctTreeWithAlpha( TTree* tree, TH1D* h1_alpha, int btagCategory, const std::string& name );



//   RooPlot* ContourPlot(RooRealVar* var1,RooRealVar* var2, RooFitResult* r);

//   // this method return only rate:
//   Double_t get_backgroundNormalization( int nbtags, const std::string& leptType, const std::string& data_mc, float mZZmin=-1., float mZZmax=-1. );
//   // this one return both rate (first) and error on rate (second):
//   std::pair<Double_t, Double_t> get_backgroundNormalizationAndError( int nbtags, const std::string& leptType, const std::string& data_mc, float mZZmin=-1., float mZZmax=-1.);

//   RooDataSet* get_observedDataset( RooRealVar* CMS_hzz2l2q_mZZ, const std::string& leptType_str, int nbtags );

//   static int convert_leptType( const std::string& leptType );

//   void fitPseudo( TTree* treeMC, TTree* treeDATA, int btagCategory, const std::string& leptType, TH1D* h1_alpha, int seed );
//   void pseudoMassge(int ntoys, int btagCategory , const std::string& leptType, RooFitResult* r_nominal);


 private:

  int smoothAlphaHist( TH1D *horig);
  TH1D* DivideAndSmoothAlphaHist( TH1D hnum, TH1D hden,TH1D &halpha);
  int smoothHist(TH1 &h, bool forceCorrZero=true,int smoothLevel=1);
  //data members
  std::string dataset_;
  std::string wType_;
  //  std::string init_;
  // std::string flags_;

  std::string outdir_;
  std::string outfile_;
  std::string fitfuncName_;
  std::string canvas_label_;
  float mZZmin_;
  float mZZmax_;
  float rangecut_;

  float turnOnShift_;

  RooRealVar* CMS_hzz2l2q_mZZ_;
  
  RooRealVar* beta_;
  RooRealVar* cutOff_;
  RooRealVar* m_;
  RooRealVar* n_;
  RooRealVar* wdth_;
  RooRealVar* alpha_;

  RooFermi* fermi_;
  RooCBShape* CBShape_;
 

  RooProdPdf* background_;
   
  RooAbsPdf* background_decorr_;

  RooWorkspace* fitWorkspace;



};


#endif
