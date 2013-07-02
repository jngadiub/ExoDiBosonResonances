
const std::string myOutDir="FitSidebandsMJJ_ZZ_20130701_prodv2c_ALL/";
const string inDirSIG="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv2c/fullsig/";
const string inDirSB ="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv2c/fullsb/";

unsigned int jetCats = 2;//1 for only 1 jet case, 2 for both
bool isZZChannel=true;//this will change only the file list
string leptType="ALL";//"MU" //"ELE"//"ALL"
const std::string InTreeName="SelectedCandidates";
const bool useAlphaVV=false;
const unsigned int nToys = 500;
const float lumi =19538.85;


const bool doPseudoExp=true;//true; //if true, for for different psuedo-alpha 
const bool unblind=true;//default is not to plot the data in signal region
const bool decorrLevExpo=true;
const bool plotDecorrLevExpoMain=false;//choose what to plot as main fit function

const bool alphaPoisson=true;//use Poisson errors for alpha uncertainties

std::string channel_marker="xzz";

std::string bkgd_decorr_name="levexp_dcr_xzz";


const std::string datacardDir("DataCards_XZZ_20130701/");
float mZZmax_=2600;

