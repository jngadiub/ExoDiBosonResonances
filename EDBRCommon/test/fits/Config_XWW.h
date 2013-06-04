
const std::string myOutDir="FitSidebandsMJJ_CA8_WW_V21/";
const string inDirSIG="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv8/AnaSigTree/";
const string inDirSB ="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv8/AnaSBTree/";

unsigned int jetCats = 1;//1 for only 1 jet case, 2 for both
bool isZZChannel=false;//this will change only the file list
string leptType="MU";//"MU" //"ELE"//"ALL"
const std::string InTreeName="SelectedCandidates";
const bool useAlphaVV=false;
const unsigned int nToys = 500;
//const float lumi =19531.85;//for ele
const float lumi =19538.85;//for mu


const bool doPseudoExp=false; //if true, for for different psuedo-alpha 
const bool unblind=true;//default is not to plot the data in signal region
const bool decorrLevExpo=true;
const bool plotDecorrLevExpoMain=true;//choose what to plot as main fit function
std::string channel_marker="xww";

std::string bkgd_decorr_name="levexp_dcr_xww";


const std::string datacardDir("DataCards_XWW_V21/");
float mZZmax_=2800;

