
const std::string myOutDir="FitSidebandsMJJ_CA8_WW_V16_AB/";
const string inDirSIG="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/AnaSigTree_from40_noConv/";
const string inDirSB ="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/AnaSBTree_from40_noConv/";

unsigned int jetCats = 1;//1 for only 1 jet case, 2 for both
bool isZZChannel=false;//this will change only the file list
string leptStr="MU";//"MU" //"ELE"//"ALL"
const std::string InTreeName="SelectedCandidatesAB";
const bool useAlphaVV=false;
const unsigned int nToys = 500;

const int nxjCut=-1;//if negative: no cut
const std::string tmpTreeName="SelectedCandidatesV2";


