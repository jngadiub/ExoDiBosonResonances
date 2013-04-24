

const string inDirSig="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1e/fullsig/";
const string inDir="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1e/fullsb/";
const string outDir="FitSidebandsMJJ_ZZ_20130424/";

bool isZZChannel=true;//only changes the file list
string InTreeName = "SelectedCandidates";
int jetCats =2; // 1 for only 1jet case and 2 for both
const string leptType="ALL";//"ALL" //"MU" //"ELE"
const bool doPseudoExp=false; //if true, for for different psuedo-alpha 
const unsigned int nToys = 500;
const bool unblind=false;//default is not to plot the data in signal region
const bool decorrLevExpo=true;
const bool plotDecorrLevExpoMain=true;//choose what to plot as main fit function
const float lumi =19538.85;
const bool useAlphaVV=false;//to sync with fitSidebands
std::string bkgd_decorr_name="levexp_dcr";
