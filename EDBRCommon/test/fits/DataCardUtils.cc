#include "DataCardUtils.h"
#include <sstream>

int DataCardUtils::convert_leptType( const std::string& leptType ) {
  
  if( leptType!="ELE" && leptType!="MU" ) {
    std::cout << "WARNING!!! LeptType '" << leptType << "' is NOT supported!!! Returning -1." << std::endl;
    return -1;
  }
  
  int leptType_int = (leptType=="MU" ) ? 1 : 0;

  return leptType_int;
  
}


std::string DataCardUtils::leptType_datacards( const std::string& leptType_str ) {
  
  std::string returnString="";

  if( leptType_str=="MU" ) returnString = "m";
  if( leptType_str=="ELE" ) returnString = "e";
  if( leptType_str=="ALL" ) returnString = "em";
  
  return returnString;

}

std::string DataCardUtils::get_fitResultsRootFileName( int nxjCategory, const std::string& leptType , const std::string&  channel_marker) {
  
  char fitResultsFileName[500];              
  std::string lt = leptType;
  if(lt=="SIMPLIFIED")
    lt="ALL";
  
  sprintf( fitResultsFileName, "FitSidebands_MJJ/workspace_%s_%dJ%s_new.root",channel_marker.c_str(), nxjCategory, lt.c_str() );
  
  std::string fitResultsFileName_str(fitResultsFileName);
  
  return fitResultsFileName_str;
  
}

std::string DataCardUtils::get_fitResultsRootFileName( int nxjCategory,const std::string& purType , const std::string& leptType , std::string myDir ,const std::string&  channel_marker) {
  
  char fitResultsFileName[500];              
  std::string lt = leptType;
  if(lt=="SIMPLIFIED")
    lt="ALL";

  sprintf( fitResultsFileName, "/workspace_%s_%dJ_%s_%s_new.root",channel_marker.c_str(), nxjCategory,purType.c_str(), lt.c_str() );
  
  std::string fitResultsFileName_str(myDir+fitResultsFileName);
  
  return fitResultsFileName_str;
  
}

std::string DataCardUtils::get_fitResultsRootFileName(double mass, int nxjCategory,const std::string& purType , const std::string& leptType , std::string myDir ,const std::string&  channel_marker) {
  
  char fitResultsFileName[500];              
  std::string lt = leptType;
  if(lt=="SIMPLIFIED")
    lt="ALL";

  sprintf( fitResultsFileName, "/workspace_%s_M%d_%dJ_%s_%s_new.root",channel_marker.c_str(), int(mass), nxjCategory,purType.c_str(), lt.c_str() );
  
  std::string fitResultsFileName_str(myDir+fitResultsFileName);
  
  return fitResultsFileName_str;
  
}

RooDataSet* DataCardUtils::get_observedDataset( RooWorkspace* bgws  , std::string leptType_str, int nxj, int purity, std::string datasetName ){
  string dName="dsDataSIGfull";
  if(datasetName!="")dName=datasetName;
  char cutstring[200];

  std::stringstream cutstream;
  cutstream << "nXjets=="<<nxj;
  if (leptType_str=="MU" || leptType_str=="ELE")
    cutstream << "&& lep=="<<convert_leptType(leptType_str);
  if(purity >= 0)
    cutstream <<"&&vTagPurity=="<<purity;

  //std::cout << cutstream.str() << std::endl;
  //exit(0);


  return dynamic_cast<RooDataSet*>(bgws->data(dName.c_str())->reduce(cutstream.str().c_str()));
}


double DataCardUtils::get_backgroundNormalization( RooWorkspace* bgws  , std::string leptType_str, int nxj ,int purity,std::string BkgDatasetName){
  string dName="dsDataSB2";
  if(BkgDatasetName!="")dName=BkgDatasetName;
  char cutstring[200];

  std::stringstream cutstream;
  cutstream << "nXjets=="<<nxj;
  if (leptType_str=="MU" || leptType_str=="ELE")
    cutstream << "&& lep=="<<convert_leptType(leptType_str);
  if(purity >= 0)
    cutstream <<"&&vTagPurity=="<<purity;

  //std::cout << cutstream.str() << std::endl;
  //exit(0);

  std::cout<<"DataCardUtils::get_backgroundNormalization , dSet: "<<dName.c_str()<<"  "<< bgws->data(dName.c_str())->reduce(cutstream.str().c_str())->numEntries()<<" ("<<  bgws->data(dName.c_str())->reduce(cutstream.str().c_str())->sumEntries()<<std::endl;
 
  return bgws->data(dName.c_str())->reduce(cutstream.str().c_str())->sumEntries();
}

double DataCardUtils::get_backgroundNormalization( RooWorkspace* bgws, std::string leptType_str  ){

  //  RooRealVar *N=bgws->var("bkgdNormalization"); 
  std::string varName="bkgdNormalization";
  if(!(leptType_str=="" || leptType_str=="ALL" ||leptType_str=="SIMPLIFIED"   )){//if it is ELE or MU
    varName=varName+"500"+leptType_str ;
  }
  else varName=varName+"500";//"FitRange";
  // else varName=varName+leptType_str ;
  return bgws->var(varName.c_str())->getVal();
}

double DataCardUtils::sidebandNorm( RooWorkspace* bgws  , std::string leptType_str, int nbtags ){

  double extrapolation= bgws->var("sidebandExtrapolation")->getVal();
  double totBkgd=get_backgroundNormalization(  bgws, leptType_str  );
  return totBkgd*extrapolation;
  //  double totSB=get_backgroundNormalization(  bgws, leptType_str ,nbtags,"dsDataSB");
  // std::cout<<"DataCardUtils::sidebandNorm: "<<totSB<<" / "<<extrapolation<<" = "<<totSB/extrapolation<<std::endl;
  // return totSB;
}

double DataCardUtils::sidebandNorm( RooWorkspace* bgws  ,std::string SidebandDsName ){

  string sbName="dsDataSB";
  if(SidebandDsName!="")sbName=SidebandDsName;
  return bgws->data(sbName.c_str())->numEntries();
}
