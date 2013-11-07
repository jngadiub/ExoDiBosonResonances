
const std::string myOutDir="FitSidebandsMJJ_ZZ_20131007_prodv2g_HighPtJet_ALL/";//_Unblinded
const string inDirSIG="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv2h/fullsig/";
const string inDirSB ="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv2h/fullsb/";
const bool unrollTrees=true;

const unsigned int jetCats = 1;//1 for only 1 jet case, 2 for both
const bool isZZChannel=true;//this will change only the file list
const string leptType="ALL";//"MU" //"ELE"//"ALL"
const std::string InTreeName="SelectedCandidates";
const bool useAlphaVV=false;//include VV into alpha (true) or take it directly from MC (false)
const bool useMCHM=true;
const double DATAMC_HMSF[2]={1.0,0.5};//1JLP ; 1JHP
const unsigned int nToys = 500;
const float lumi =19710.0;
const bool useVJetsNormFromMJFit=true;

const bool doPseudoExp=true;//true; //if true, for for different psuedo-alpha 
const bool unblind=true;//default is not to plot the data in signal region
const bool decorrLevExpo=true;
const bool plotDecorrLevExpoMain=true;//choose what to plot as main fit function

const bool alphaPoisson=true;//use Poisson errors for alpha uncertainties
const bool plotFitPull=false;//plot pull plot with the fit result
const bool plotFixedBinning=false;//plot fit result with fixed binning
const double FixedBinWidth =50;
std::string bkgd_decorr_name="levexp_dcr_xzz";
float mZZmax_=2800;

//used by create_datacards
std::string signalProcessName="Bulk";
std::string channel_marker="xzz";
std::string dims = "1d";
const std::string datacardDir("DataCards_XZZ_prodv2h_TESTNEWSYST");


//********** Externally provided Normalizations *********
//
// From MJ sidebands fits; 
//first index is lepton flavor (==0 -> ELE, ==1 ->MU), 
//second index is the purity category (==0 -> LP, ==1 -> HP)
//LP from fit in mZZ [650, 2800], HP in [500, 2800]
const double extNorm_1J[2][2]={{198.278, 323.593}, {328.374, 562.107} };
const double extNorm_1J_err[2][2]={{16.5032, 21.8785}, {21.1646, 28.1272}};
const double extNorm_2J[2][1]={{0.0}, {0.0}};
const double extNorm_2J_err[2][1]={{0.0}, {0.0}};
/*
//using BestMass arbitration algo
const double extNorm_1J[2][2]={{198.801, 325.129}, {331.899, 563.395} };
const double extNorm_1J_err[2][2]={{16.5968, 22.0264}, {21.4401, 28.2718}};
const double extNorm_2J[2][1]={{0.0}, {0.0}};
const double extNorm_2J_err[2][1]={{0.0}, {0.0}};
*/
//**********systematics***********

////0) Lepton trigger and id
const double CMS_trigger_e = 1.01; 
const double CMS_eff_e = 1.03;//from T.Williams

const double CMS_trigger_m = 1.03; //it is 3% at very large eta, otherwise less
const double CMS_eff_m = 1.04; //2% eff from T&P, + 2% conservative for boosted topology

////1) Jet energy scale and resoluation
//signal efficiency
const double CMS_scale_j_up= 1.01;
const double CMS_scale_j_down= 0.99;
//signal shape: p1 for mean and p2 for sigma
const double CMS_sig1J_p1_jes = 0.005;
const double CMS_sig1J_p2_jes = 0.02;
const double CMS_sig1J_p1_jer = 0.00;
const double CMS_sig1J_p2_jer = 0.02;

////2) Electron energy scale and resoluation
//signal efficiency
const double CMS_scale_e = 1.00;
//signal shape: p1 for mean and p2 for sigma
const double CMS_sig1Je_p1_scale = 0.005 ;
const double CMS_sig1Je_p2_scale = 0.0004 ;

////3) Mu energy scale and resoluation
//signal efficiency
const double CMS_scale_m = 1.01;
//signal shape: p1 for mean and p2 for sigma
const double CMS_sig1Jm_p1_scale = 0.006;
const double CMS_sig1Jm_p2_scale = 0.018;

////4) V-tagging systematics
const double CMS_eff_vtag_tau21_sf_LP=0.30;
const double CMS_eff_vtag_tau21_sf_HP=0.08;

