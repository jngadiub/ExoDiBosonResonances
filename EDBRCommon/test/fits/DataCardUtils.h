#ifndef DataCardUtils_h
#define DataCardUtils_h

#include <string>

#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"


class  DataCardUtils{

public:
  DataCardUtils(){
    //   wsDirName_="./";
  };
  ~DataCardUtils(){};
  
  static int convert_leptType( const std::string& leptType );
  
  static std::string leptType_datacards( const std::string& leptType_str );
  
  static std::string get_fitResultsRootFileName( const int nxjCategory, const std::string& leptType );
  static std::string get_fitResultsRootFileName( const int nxjCategory, const std::string& purType ,const std::string& leptType , std::string myDir);
  static std::string get_fitResultsRootFileName(double mass, int nxjCategory, const std::string& purType ,const std::string& leptType, std::string myDir) ;
  static RooDataSet* get_observedDataset(  RooWorkspace* bgws  ,  std::string leptType_str,  int nxj, int purity,std::string datasetName="" );
  static double get_backgroundNormalization(  RooWorkspace* bgws  ,  std::string leptType_str,  int nxj ,int purity ,std::string BkgDatasetName="");
  static double get_backgroundNormalization(  RooWorkspace* bgws ,  std::string leptType_str);
  static double sidebandNorm(  RooWorkspace* bgws  ,  std::string leptType_str,  int nxj );
  static double sidebandNorm( RooWorkspace* bgws  , std::string SidebandDsName="" );
};
#endif
