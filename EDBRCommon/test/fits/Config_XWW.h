
const std::string myOutDir="FitSidebandsMJJ_CA8_WW_V24/";
const string inDirSIG="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv9/AnaSigTree/";
const string inDirSB ="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv9/AnaSBTree/";
const bool unrollTrees=true;

const unsigned int jetCats = 1;//1 for only 1 jet case, 2 for both
const bool isZZChannel=false;//this will change only the file list
const string leptType="ELE";//"MU" //"ELE"//"ALL"
const std::string InTreeName="SelectedCandidates";
const bool useAlphaVV=false;
const bool useMCHM=false;
const double DATAMC_HMSF[2]={1.0,1.0};//1JLP ; 1JHP
const unsigned int nToys = 500;
const float lumi =19531.85;//for ele
//const float lumi =19538.85;//for mu
const bool useVJetsNormFromMJFit=true;

const bool doPseudoExp=false; //if true, for for different psuedo-alpha 
const bool unblind=true;//default is not to plot the data in signal region
const bool decorrLevExpo=true;
const bool plotDecorrLevExpoMain=true;//choose what to plot as main fit function
std::string channel_marker="xww";

const bool alphaPoisson=false;//use Poisson errors for alpha uncertainties
const bool plotFitPull=true;//plot pull plot with the fit result
const bool plotFixedBinning=true;//plot fit result with fixed binning
const double FixedBinWidth =50;
std::string bkgd_decorr_name="levexp_dcr_xww";
float mZZmax_=3000;

//used by create_datacards
std::string signalProcessName="Bulk";
std::string dims = "1d";
const std::string datacardDir("DataCards_XWW_V24/");



//********** Externally provided Normalizations *********
//
// From MJ sidebands fits; 
//first index is lepton flavor (==0 -> ELE, ==1 ->MU), 
//second index is the purity category (==0 -> LP, ==1 -> HP)
const double extNorm_1J[2][2]={{546.608, 339.861}, {835.742, 536.198}};
const double extNorm_1J_err[2][2]={{0.0, 0.0}, {0.0, 0.0}};
const double extNorm_2J[2][1]={{0.0}, {0.0}};
const double extNorm_2J_err[2][1]={{0.0}, {0.0}};


//**********systematics***********
////0) Lepton trigger and id
const double CMS_trigger_e = 1.01; 
const double CMS_eff_e = 1.03;

const double CMS_trigger_m = 1.01; 
const double CMS_eff_m = 1.01; 

////1) Jet energy scale and resoluation
//signal efficiency------------This will not be used. In the code we will use a line from 1% to 3% from 600 to 2500.
const double CMS_scale_j_up= 1.03;
const double CMS_scale_j_down= 0.97;
//signal shape: p1 for mean and p2 for sigma
const double CMS_sig1J_p1_jes = 0.013;
const double CMS_sig1J_p2_jes = 0.04;//---------------This will not be used. In the code we will use a line from 2% to 3% from 600 to 2500.
const double CMS_sig1J_p1_jer = 0.001;
const double CMS_sig1J_p2_jer = 0.03;

////2) Electron energy scale and resoluation
//signal efficiency
const double CMS_scale_e = 1.002;
//signal shape: p1 for mean and p2 for sigma
const double CMS_sig1Je_p1_scale = 0.001 ;
const double CMS_sig1Je_p2_scale = 0.001 ;

////3) Mu energy scale and resoluation
//signal efficiency
const double CMS_scale_m = 1.007;
//signal shape: p1 for mean and p2 for sigma
const double CMS_sig1Jm_p1_scale = 0.001;
const double CMS_sig1Jm_p2_scale = 0.005;


////4) Btagging scale: negligible


////5) V-tagging systematics
const double CMS_eff_vtag_tau21_sf_LP=0.30;
const double CMS_eff_vtag_tau21_sf_HP=0.08;










