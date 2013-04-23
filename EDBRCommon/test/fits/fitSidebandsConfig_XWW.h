
const std::string myOutDir="FitSidebandsMJJ_CA8_WW_V15_AB/";
const string inDirSIG="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/AnaSigTree_from50_noConv/";
const string inDirSB ="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/AnaSBTree_from50_noConv/";

unsigned int jetCats = 2;//1 for only 1 jet case, 2 for both
bool isZZChannel=true;//this will change only the file list
string leptStr="ALL";//"MU" //"ELE"//"ALL"
const std::string InTreeName="SelectedCandidates";
const bool useAlphaVV=false;


const int nxjCut=-1;//if negative: no cut
const std::string tmpTreeName="SelectedCandidatesV2";


