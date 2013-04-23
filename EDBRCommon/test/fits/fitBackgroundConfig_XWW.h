
const std::string outDir="FitSidebandsMJJ_CA8_WW_V14_AB/";
const string inDirSig="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/AnaSigTree_from50_noConv/";
const string inDir ="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/AnaSBTree_from50_noConv/";


bool isZZChannel=false;//only changes the file list
string InTreeName = "SelectedCandidatesAB";
int jetCats =1; // 1 for only 1jet case and 2 for both
const string leptType="ALL";//"ALL" //"MU" //"ELE"
const bool doPseudoExp=false; //if true, for for different psuedo-alpha 
const unsigned int nToys = 500;
const bool unblind=false;//default is not to plot the data in signal region
const bool decorrLevExpo=true;
const bool plotDecorrLevExpoMain=false;//choose what to plot as main fit function
const float lumi =19538.85;
const bool useAlphaVV=false;//to sync with fitSidebands

