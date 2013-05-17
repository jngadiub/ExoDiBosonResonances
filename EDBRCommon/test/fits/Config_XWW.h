
const std::string myOutDir="FitSidebandsMJJ_CA8_WW_V20_Test/";
const string inDirSIG="/afs/cern.ch/work/s/santanas/public/EXOVV_2012/ntuples/WW_02_05_2013_ForUnblinding/fullallrange/AnaSigTree_mWW_Type2_corrected/";
const string inDirSB ="/afs/cern.ch/work/s/santanas/public/EXOVV_2012/ntuples/WW_02_05_2013_ForUnblinding/fullallrange/AnaSBTree_mWW_Type2_corrected/";

unsigned int jetCats = 1;//1 for only 1 jet case, 2 for both
bool isZZChannel=false;//this will change only the file list
string leptType="ELE";//"MU" //"ELE"//"ALL"
const std::string InTreeName="SelectedCandidates";
const bool useAlphaVV=false;
const unsigned int nToys = 500;
const float lumi =19531.85;


const bool doPseudoExp=false; //if true, for for different psuedo-alpha 
const bool unblind=true;//default is not to plot the data in signal region
const bool decorrLevExpo=true;
const bool plotDecorrLevExpoMain=true;//choose what to plot as main fit function
std::string bkgd_decorr_name="levexp_dcr";


const std::string datacardDir("DataCards_XWW_V20_Test/");
float mZZmax_=2800;

