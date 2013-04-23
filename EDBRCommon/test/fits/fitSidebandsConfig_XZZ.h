
const std::string myOutDir="FitSidebandsMJJ_ZZ_20130423/";
const string inDirSIG="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1e/fullsig/";
const string inDirSB ="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1e/fullsb/";

unsigned int jetCats = 2;//1 for only 1 jet case, 2 for both
bool isZZChannel=true;//this will change only the file list
string leptStr="ALL";//"MU" //"ELE"//"ALL"
const std::string InTreeName="SelectedCandidates";
const bool useAlphaVV=false;


const int nxjCut=-1;//if negative: no cut
const std::string tmpTreeName="SelectedCandidatesV2";
