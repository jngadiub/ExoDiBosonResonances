
const std::string myOutDir="FitSidebandsMJJ_CA8_WW_V16/";
const string inDirSIG="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/AnaSigTree_from40_noConv/";
const string inDirSB ="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/AnaSBTree_from40_noConv/";

unsigned int jetCats = 1;//1 for only 1 jet case, 2 for both
bool isZZChannel=false;//this will change only the file list
string leptType="ALL";//"MU" //"ELE"//"ALL"
const std::string InTreeName="SelectedCandidates";
const bool useAlphaVV=false;
const unsigned int nToys = 500;
const float lumi =19538.85;


const bool doPseudoExp=false; //if true, for for different psuedo-alpha 
const bool unblind=false;//default is not to plot the data in signal region
const bool decorrLevExpo=true;
const bool plotDecorrLevExpoMain=false;//choose what to plot as main fit function
std::string bkgd_decorr_name="levexp_dcr";


const std::string datacardDir("DataCards_XWW_V16/");
float mZZmax_=3000;

