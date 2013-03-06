#include <map>
#include <string>

#include "TH1D.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"

// Sadly, CINT will not allow these large arrays
// to be part of the class... possibly if I change
// them to be std::vectors, eventually?

const std::string vars[92] = 
{"nCands", "cosThetaStar", "cosTheta1", "cosTheta2", "phi", "phiStar1", "ptlep1",
	"ptlep2", "ptjet1", "ptjet2", "ptZll", "ptZjj", "yZll", "yZjj",
	"phiZll", "phiZjj", "etalep1", "etalep2", "etajet1", "etajet2", "philep1",
	"philep2", "phijet1", "phijet2", "lep", "region", "mZZ", "mZZNoKinFit",
	"ptmzz", "ptmzzNoKinFit", "mLL", "mJJ", "mJJNoKinFit", "met", "metSign",
	"nBTags", "deltaREDBR", "deltaRleplep", "deltaRjetjet", "qgProduct", "qgjet1", "qgjet2",
	"betajet1", "betajet2", "puMvajet1", "puMvajet2", "nXjets", "prunedmass", "mdrop",
	"nsubj21", "nsubj23", "tau1", "tau2", "qjet", "isomu1mod", "isomu2mod",
	"isoele1calo", "isoele2calo", "isoele1trk", "isoele2trk", "LD", "q1fl", "q2fl", "MCmatch", "nVtx",
	"nJets", "nPU", "HLTweight", "PUweight", "PUweight2012A", "PUweight2012B", "LumiWeight",
	"GenWeight", "weight", "weight2012A", "weight2012B", "event", "run", "ls",
	"nVL","VBFTag","VBFmJJ","VBFdeltaEta","nLooseEle","nLooseMu","mt",
	"nbtagsL","nbtagsM","nbtagsT","nbtagscleanL","nbtagscleanM","nbtagscleanT"};

const int nBins[92] = 
{30,  100, 100, 100, 100, 100, 100,
	100, 100, 100, 92,  100, 28,  28,
	100, 100, 26,  26,  26,  26,  100,
	100, 100, 100, 100, 100, 50,  50,
	35,  35,  50,  80,  75,  100, 20,
	100, 100, 100, 100, 100, 100, 100,
	100, 100, 100, 100, 4,   22,  100,
	40,  100, 100, 100, 100, 100, 100,
	100, 100, 100, 100, 100, 4,   4,   100, 43,
	10,  2,   100, 100, 100, 100, 100,
	100, 100, 100, 100, 100, 100, 100,
	10,  2,   100, 100, 10,  10,  80,
	10,  10,  10,  10,  10,  10};

const double minBin[92] = 
{0.0,   -1.15,  -1.15,  -1.15,  -3.7,   -3.7,    0.0,
	0.0,    0.0,    0.0,    80.0,   0.0,   -2.8,   -2.8,
	-3.7,   -3.7,   -2.6,   -2.6,   -2.6,   -2.6,   -3.7,
	-3.7,   -3.7,   -3.7,    0.0,    0.0,    0.0,    0.0,
	0.0,    0.0,    70.0,   40.0,   40.0,   0.0,    0.0,
	-2.2,    0.0,    0.0,    0.0,   -100.2, -100.2, -100.2,
	0.,    -1.2,   -1080., -1080.,  0.,     50.,   -1080.,
	0.,    -1080., -1080., -1080., -1080.,  0.,     0.,
	0.0,    0.0,    0.0,    0.0,   -101.,  -101.,  -101., -1.2, -0.5,
	0.5,    0.,     0.99,   0.,     0.,     0.,     0.,
	0.,     0.,     0.,     0.,     0.,     190000, 0,
	0.,     0.,     0.,     0.,     0.,     0.,     0.,
	0.,     0.,     0.,     0.,     0.,     0.};

const double maxBin[92] = 
{30.0,  1.15,  1.15, 1.15,   3.7,     3.7,   500.0,
	500.0, 500.0, 500.0, 1000.0, 1000.0,  2.8,   2.8,
	3.7,   3.7,   2.6,   2.6,    2.6,     2.6,   3.7,
	3.7,   3.7,   3.7,   1.0,    1.0,     2000,  2000,
	350.0, 350.0, 110.0, 140.0,  190.0,   500.0, 10.0,
	0.,    4.0,   4.0,   4.0,    -97.8,   -97.8, -97.8 ,
	1.1,   1.2,   1000., 1000.,  4.,      105.,  100.,
	1.,    100.,  100.,  100.,   100.,    0.20,  0.20,
	1.0,   1.0,   10.0,  10.0,   -97.,    -97.,  -97., 1.2, 42.5,
	10.5,  1.0,   10.0,  10.0,   10.,      10.,  0.1,
	10,    10,    10,    10,     1.0E9,   210000,10000,
	10,    2.0,   1000., 10.0,   10.0,    10.0,  130.0,
	10,    10,    10,    10,     10,      10};

/// EDBRHistoMaker is the class that analyzes the flat
/// TTree that comes out from the NTuple dumper module.
/// It doesn't make analysis; it just makes plots.
/// There are a few switches to do different plots:
/// SIGNAL - BACKGROUND,
/// MUON - ELECTRON, etc...

class EDBRHistoMaker {
	public:
		EDBRHistoMaker(TTree *tree=0, 
				bool wantElectrons=true,
				bool wantMuons=true,
				bool wantSideband=true, 
				bool wantSignal=false,
				int  wantNXJets=1,
				bool isZZchannel=1);
		virtual ~EDBRHistoMaker();

		/// This is the tree structure. This comes directly from MakeClass
	public :
		TTree          *fChain;   //!pointer to the analyzed TTree or TChain
		Int_t           fCurrent; //!current Tree number in a TChain

		// Declaration of leaf types
		Int_t           nCands;
		UInt_t          event;
		UInt_t          run;
		UInt_t          ls;
		Double_t        cosThetaStar[99];   //[nCands]
		Double_t        cosTheta1[99];   //[nCands]
		Double_t        cosTheta2[99];   //[nCands]
		Double_t        phi[99];   //[nCands]
		Double_t        phiStar1[99];   //[nCands]
		Double_t        ptlep1[99];   //[nCands]
		Double_t        ptlep2[99];   //[nCands]
		Double_t        ptjet1[99];   //[nCands]
		Double_t        ptjet2[99];   //[nCands]
		Double_t        ptZll[99];   //[nCands]
		Double_t        ptZjj[99];   //[nCands]
		Double_t        yZll[99];   //[nCands]
		Double_t        yZjj[99];   //[nCands]
		Double_t        phiZll[99];   //[nCands]
		Double_t        phiZjj[99];   //[nCands]
		Double_t        etalep1[99];   //[nCands]
		Double_t        etalep2[99];   //[nCands]
		Double_t        etajet1[99];   //[nCands]
		Double_t        etajet2[99];   //[nCands]
		Double_t        philep1[99];   //[nCands]
		Double_t        philep2[99];   //[nCands]
		Double_t        phijet1[99];   //[nCands]
		Double_t        phijet2[99];   //[nCands]
		Double_t        lep[99];       //[nCands]
		Double_t        region[99];   //[nCands]
		Int_t           nXjets[99];   //[nCands]
		Double_t        mZZ[99];   //[nCands]
		Double_t        mZZNoKinFit[99];   //[nCands]
		Double_t        ptmzz[99];   //[nCands]
		Double_t        ptmzzNoKinFit[99];   //[nCands]
		Double_t        mLL[99];   //[nCands]
		Double_t        mJJ[99];   //[nCands]
		Double_t        mJJNoKinFit[99];   //[nCands]
		Double_t        met;
		Double_t        metSign;
		Double_t        nBTags[99];   //[nCands]
		Double_t        deltaREDBR[99];   //[nCands]
		Double_t        deltaRleplep[99];   //[nCands]
		Double_t        deltaRjetjet[99];   //[nCands]
		Double_t        qgProduct[99];   //[nCands]
		Double_t        qgjet1[99];   //[nCands]
		Double_t        qgjet2[99];   //[nCands]
		Double_t        betajet1[99];   //[nCands]
		Double_t        betajet2[99];   //[nCands]
		Double_t        puMvajet1[99];   //[nCands]
		Double_t        puMvajet2[99];   //[nCands]
		Double_t        prunedmass[99];   //[nCands]
		Double_t        mdrop[99];   //[nCands]
		Double_t        nsubj12[99];   //[nCands]
		Double_t        nsubj23[99];   //[nCands]
		Double_t        tau1[99];   //[nCands]
		Double_t        tau2[99];   //[nCands]
		Double_t        qjet[99];   //[nCands]
		Double_t        isolep1[99];   //[nCands]
		Double_t        isolep2[99];   //[nCands]
		Double_t        isomu1mod[99];   //[nCands]
		Double_t        isomu2mod[99];   //[nCands]
		Double_t        isoele1trk[99];   //[nCands]
		Double_t        isoele2trk[99];   //[nCands]
		Double_t        isoele1calo[99];   //[nCands]
		Double_t        isoele2calo[99];   //[nCands]
		Double_t        eleMVAId1[99];   //[nCands]
		Double_t        eleMVAId2[99];   //[nCands]
		Double_t        LD[99];   //[nCands]
		Int_t           q1fl[99];   //[nCands]
		Int_t           q2fl[99];   //[nCands]
		Double_t        MCmatch[99];   //[nCands]
		UInt_t          nVtx;
		UInt_t          nJets;
		UInt_t          nPU;
		Double_t        HLTweight;
		Double_t        PUweight;
		Double_t        PUweight2012A;
		Double_t        PUweight2012B;
		Double_t        LumiWeight;
		Double_t        GenWeight;
		Double_t        weight;
		Double_t        weight2012A;
		Double_t        weight2012B;
		Int_t           VBFTag[99];   //[nCands]
		Double_t        VBFmJJ[99];   //[nCands]
		Double_t        VBFdeltaEta[99];   //[nCands]
		Double_t        VBFptjet1[99];   //[nCands]
		Double_t        VBFptjet2[99];   //[nCands]
		Double_t        VBFetajet1[99];   //[nCands]
		Double_t        VBFetajet2[99];   //[nCands]
		Double_t        VBFphijet1[99];   //[nCands]
		Double_t        VBFphijet2[99];   //[nCands]
		Double_t        massGenX;
		Double_t        ptGenX;
		Double_t        yGenX;
		Double_t        phiGenX;
		Int_t           pdgIdGenX;
		Double_t        massGenVll;
		Double_t        ptGenVll;
		Double_t        yGenVll;
		Double_t        phiGenVll;
		Double_t        massGenVqq;
		Double_t        ptGenVqq;
		Double_t        yGenVqq;
		Double_t        phiGenVqq;
		Int_t           nLooseMu;
		Int_t           nLooseEle;
		Double_t        mt[99];      //[nCands]
		Double_t        nbtagsL[99];  //[nCands]
		Double_t        nbtagsM[99];  //[nCands]
		Double_t        nbtagsT[99];  //[nCands]
		Double_t        nbtagscleanL[99];  //[nCands]
		Double_t        nbtagscleanM[99];  //[nCands]
		Double_t        nbtagscleanT[99];  //[nCands]


		// List of branches
		TBranch        *b_nCands;   //!
		TBranch        *b_event;   //!
		TBranch        *b_run;   //!
		TBranch        *b_ls;   //!
		TBranch        *b_cosThetaStar;   //!
		TBranch        *b_cosTheta1;   //!
		TBranch        *b_cosTheta2;   //!
		TBranch        *b_phi;   //!
		TBranch        *b_phiStar1;   //!
		TBranch        *b_ptlep1;   //!
		TBranch        *b_ptlep2;   //!
		TBranch        *b_ptjet1;   //!
		TBranch        *b_ptjet2;   //!
		TBranch        *b_ptZll;   //!
		TBranch        *b_ptZjj;   //!
		TBranch        *b_yZll;   //!
		TBranch        *b_yZjj;   //!
		TBranch        *b_phiZll;   //!
		TBranch        *b_phiZjj;   //!
		TBranch        *b_etalep1;   //!
		TBranch        *b_etalep2;   //!
		TBranch        *b_etajet1;   //!
		TBranch        *b_etajet2;   //!
		TBranch        *b_philep1;   //!
		TBranch        *b_philep2;   //!
		TBranch        *b_phijet1;   //!
		TBranch        *b_phijet2;   //!
		TBranch        *b_lep;   //!
		TBranch        *b_region;   //!
		TBranch        *b_nXjets;   //!
		TBranch        *b_mZZ;   //!
		TBranch        *b_mZZNoKinFit;   //!
		TBranch        *b_ptmzz;   //!
		TBranch        *b_ptmzzNoKinFit;   //!
		TBranch        *b_mLL;   //!
		TBranch        *b_mJJ;   //!
		TBranch        *b_mJJNoKinFit;   //!
		TBranch        *b_met;   //!
		TBranch        *b_metSign;   //!
		TBranch        *b_nBTags;   //!
		TBranch        *b_deltaREDBR;   //!
		TBranch        *b_deltaRleplep;   //!
		TBranch        *b_deltaRjetjet;   //!
		TBranch        *b_qgProduct;   //!
		TBranch        *b_qgjet1;   //!
		TBranch        *b_qgjet2;   //!
		TBranch        *b_betajet1;   //!
		TBranch        *b_betajet2;   //!
		TBranch        *b_puMvajet1;   //!
		TBranch        *b_puMvajet2;   //!
		TBranch        *b_prunedmass;   //!
		TBranch        *b_mdrop;   //!
		TBranch        *b_nsubj12;   //!
		TBranch        *b_nsubj23;   //!
		TBranch        *b_tau1;   //!
		TBranch        *b_tau2;   //!
		TBranch        *b_qjet;   //!
		TBranch        *b_isolep1;   //!
		TBranch        *b_isolep2;   //!
		TBranch        *b_isomu1mod;   //!
		TBranch        *b_isomu2mod;   //!
		TBranch        *b_isoele1trk;   //!
		TBranch        *b_isoele2trk;   //!
		TBranch        *b_isoele1calo;   //!
		TBranch        *b_isoele2calo;   //!
		TBranch        *b_eleMVAId1;   //!
		TBranch        *b_eleMVAId2;   //!
		TBranch        *b_LD;   //!
		TBranch        *b_q1fl;   //!
		TBranch        *b_q2fl;   //!
		TBranch        *b_MCmatch;   //!
		TBranch        *b_nVtx;   //!
		TBranch        *b_nJets;   //!
		TBranch        *b_nPU;   //!
		TBranch        *b_HLTweight;   //!
		TBranch        *b_PUweight;   //!
		TBranch        *b_PUweight2012A;   //!
		TBranch        *b_PUweight2012B;   //!
		TBranch        *b_LumiWeight;   //!
		TBranch        *b_GenWeight;   //!
		TBranch        *b_weight;   //!
		TBranch        *b_weight2012A;   //!
		TBranch        *b_weight2012B;   //!
		TBranch        *b_VBFTag;   //!
		TBranch        *b_VBFmJJ;   //!
		TBranch        *b_VBFdeltaEta;   //!
		TBranch        *b_VBFptjet1;   //!
		TBranch        *b_VBFptjet2;   //!
		TBranch        *b_VBFetajet1;   //!
		TBranch        *b_VBFetajet2;   //!
		TBranch        *b_VBFphijet1;   //!
		TBranch        *b_VBFphijet2;   //!
		TBranch        *b_massGenX;   //!
		TBranch        *b_ptGenX;   //!
		TBranch        *b_yGenX;   //!
		TBranch        *b_phiGenX;   //!
		TBranch        *b_pdgIdGenX;   //!
		TBranch        *b_massGenVll;   //!
		TBranch        *b_ptGenVll;   //!
		TBranch        *b_yGenVll;   //!
		TBranch        *b_phiGenVll;   //!
		TBranch        *b_massGenVqq;   //!
		TBranch        *b_ptGenVqq;   //!
		TBranch        *b_yGenVqq;   //!
		TBranch        *b_phiGenVqq;   //!
		TBranch        *b_nLooseMu;   //!
		TBranch        *b_nLooseEle;   //!
		TBranch        *b_mt;   //!

		// Basic functions directly from MakeClass
		Int_t    GetEntry(Long64_t entry);
		Long64_t LoadTree(Long64_t entry);
		void     Init(TTree *tree);
		void     Loop(std::string outFileName);
		double   FastLoop(double lumiValue, double kFactor, double nsubjetinessCut, double massPoint, double percentageWindow);

		// Our added functions
		void createAllHistos();
		void printAllHistos();
		void saveAllHistos(std::string outFileName);

		void setWantElectrons(bool doele=false){wantElectrons_=doele;}
		void setWantMuons(bool domu=false){wantMuons_=domu;}
		void setWantSideband(bool dosb=false){wantSideband_=dosb;}
		void setWantSignal(bool dosig=false){wantSignal_=dosig;}
		void setWantNXJets(int nxj=1){wantNXJets_=nxj;}
		void setUnitaryWeights(bool setuniw=false){setUnitaryWeights_=setuniw;}

		bool eventPassesFlavorCut(int i);
		bool eventPassesLeptonicZPtCut(int i, double ptZll_threshold);
		bool eventPassesLep1PtCut(int i, double ptlep1_threshold);
		bool eventInSidebandRegion(int i);
		bool eventInSignalRegion(int i);
		bool eventPassesRegionCut(int i);
		bool eventPassesNXJetCut(int i);  
		bool eventPassesCut(int i, double ptZll_threshold, double ptlep1_threshold );
		bool eventPassesVBFCut(int i);

		int check ( double pt, vector<double> * ptZ  )
		{
			int goodw=1;
			for(unsigned int i =0; i< ptZ->size(); i++)
			{   
				//printf("Comparing %g and %g\n",pt,ptZ->at(i));
				if(pt==ptZ->at(i)) { goodw=0; break;}
				//else {printf("I think they're different\n");}
			}   

			return goodw;
		}

		// Our added variables
		int nVars;
		bool wantElectrons_;
		bool wantMuons_;
		bool wantSideband_;
		bool wantSignal_;
		bool setUnitaryWeights_;
		bool debug_;
		int wantNXJets_;
		double sidebandVHMassLow_;
		double sidebandVHMassHigh_;
		double signalVHMassLow_;
		double signalVHMassHigh_;
		bool isZZchannel_;

		// The histograms
		std::map<std::string,TH1D*> theHistograms;
};

void EDBRHistoMaker::Init(TTree *tree)
{
	// The Init() function is called when the selector needs to initialize
	// a new tree or chain. Typically here the branch addresses and branch
	// pointers of the tree will be set.
	// It is normally not necessary to make changes to the generated
	// code, but the routine can be extended by the user if needed.
	// Init() will be called many times when running on PROOF
	// (once per file to be processed).

	// Set branch addresses and branch pointers
	if (!tree) return;
	fChain = tree;
	fCurrent = -1;
	fChain->SetMakeClass(1);

	fChain->SetBranchAddress("nCands", &nCands, &b_nCands);
	fChain->SetBranchAddress("event", &event, &b_event);
	fChain->SetBranchAddress("run", &run, &b_run);
	fChain->SetBranchAddress("ls", &ls, &b_ls);
	fChain->SetBranchAddress("cosThetaStar", cosThetaStar, &b_cosThetaStar);
	fChain->SetBranchAddress("cosTheta1", cosTheta1, &b_cosTheta1);
	fChain->SetBranchAddress("cosTheta2", cosTheta2, &b_cosTheta2);
	fChain->SetBranchAddress("phi", phi, &b_phi);
	fChain->SetBranchAddress("phiStar1", phiStar1, &b_phiStar1);
	fChain->SetBranchAddress("ptlep1", ptlep1, &b_ptlep1);
	fChain->SetBranchAddress("ptlep2", ptlep2, &b_ptlep2);
	fChain->SetBranchAddress("ptjet1", ptjet1, &b_ptjet1);
	fChain->SetBranchAddress("ptjet2", ptjet2, &b_ptjet2);
	fChain->SetBranchAddress("ptZll", ptZll, &b_ptZll);
	fChain->SetBranchAddress("ptZjj", ptZjj, &b_ptZjj);
	fChain->SetBranchAddress("yZll", yZll, &b_yZll);
	fChain->SetBranchAddress("yZjj", yZjj, &b_yZjj);
	fChain->SetBranchAddress("phiZll", phiZll, &b_phiZll);
	fChain->SetBranchAddress("phiZjj", phiZjj, &b_phiZjj);
	fChain->SetBranchAddress("etalep1", etalep1, &b_etalep1);
	fChain->SetBranchAddress("etalep2", etalep2, &b_etalep2);
	fChain->SetBranchAddress("etajet1", etajet1, &b_etajet1);
	fChain->SetBranchAddress("etajet2", etajet2, &b_etajet2);
	fChain->SetBranchAddress("philep1", philep1, &b_philep1);
	fChain->SetBranchAddress("philep2", philep2, &b_philep2);
	fChain->SetBranchAddress("phijet1", phijet1, &b_phijet1);
	fChain->SetBranchAddress("phijet2", phijet2, &b_phijet2);
	fChain->SetBranchAddress("lep", lep, &b_lep);
	fChain->SetBranchAddress("region", region, &b_region);
	fChain->SetBranchAddress("nXjets", nXjets, &b_nXjets);
	fChain->SetBranchAddress("mZZ", mZZ, &b_mZZ);
	fChain->SetBranchAddress("mZZNoKinFit", mZZNoKinFit, &b_mZZNoKinFit);
	fChain->SetBranchAddress("ptmzz", ptmzz, &b_ptmzz);
	fChain->SetBranchAddress("ptmzzNoKinFit", ptmzzNoKinFit, &b_ptmzzNoKinFit);
	fChain->SetBranchAddress("mLL", mLL, &b_mLL);
	fChain->SetBranchAddress("mJJ", mJJ, &b_mJJ);
	fChain->SetBranchAddress("mJJNoKinFit", mJJNoKinFit, &b_mJJNoKinFit);
	fChain->SetBranchAddress("met", &met, &b_met);
	fChain->SetBranchAddress("metSign", &metSign, &b_metSign);
	fChain->SetBranchAddress("nBTags", nBTags, &b_nBTags);
	fChain->SetBranchAddress("deltaREDBR", deltaREDBR, &b_deltaREDBR);
	fChain->SetBranchAddress("deltaRleplep", deltaRleplep, &b_deltaRleplep);
	fChain->SetBranchAddress("deltaRjetjet", deltaRjetjet, &b_deltaRjetjet);
	fChain->SetBranchAddress("qgProduct", qgProduct, &b_qgProduct);
	fChain->SetBranchAddress("qgjet1", qgjet1, &b_qgjet1);
	fChain->SetBranchAddress("qgjet2", qgjet2, &b_qgjet2);
	fChain->SetBranchAddress("betajet1", betajet1, &b_betajet1);
	fChain->SetBranchAddress("betajet2", betajet2, &b_betajet2);
	fChain->SetBranchAddress("puMvajet1", puMvajet1, &b_puMvajet1);
	fChain->SetBranchAddress("puMvajet2", puMvajet2, &b_puMvajet2);
	fChain->SetBranchAddress("prunedmass", prunedmass, &b_prunedmass);
	fChain->SetBranchAddress("mdrop", mdrop, &b_mdrop);
	fChain->SetBranchAddress("nsubj12", nsubj12, &b_nsubj12);
	fChain->SetBranchAddress("nsubj23", nsubj23, &b_nsubj23);
	fChain->SetBranchAddress("tau1", tau1, &b_tau1);
	fChain->SetBranchAddress("tau2", tau2, &b_tau2);
	fChain->SetBranchAddress("qjet", qjet, &b_qjet);
	fChain->SetBranchAddress("isolep1", isolep1, &b_isolep1);
	fChain->SetBranchAddress("isolep2", isolep2, &b_isolep2);
	fChain->SetBranchAddress("isomu1mod", isomu1mod, &b_isomu1mod);
	fChain->SetBranchAddress("isomu2mod", isomu2mod, &b_isomu2mod);
	fChain->SetBranchAddress("isoele1trk", isoele1trk, &b_isoele1trk);
	fChain->SetBranchAddress("isoele2trk", isoele2trk, &b_isoele2trk);
	fChain->SetBranchAddress("isoele1calo", isoele1calo, &b_isoele1calo);
	fChain->SetBranchAddress("isoele2calo", isoele2calo, &b_isoele2calo);
	fChain->SetBranchAddress("eleMVAId1", eleMVAId1, &b_eleMVAId1);
	fChain->SetBranchAddress("eleMVAId2", eleMVAId2, &b_eleMVAId2);
	fChain->SetBranchAddress("LD", LD, &b_LD);
	fChain->SetBranchAddress("q1fl", q1fl, &b_q1fl);
	fChain->SetBranchAddress("q2fl", q2fl, &b_q2fl);
	fChain->SetBranchAddress("MCmatch", MCmatch, &b_MCmatch);
	fChain->SetBranchAddress("nVtx", &nVtx, &b_nVtx);
	fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
	fChain->SetBranchAddress("nPU", &nPU, &b_nPU);
	fChain->SetBranchAddress("HLTweight", &HLTweight, &b_HLTweight);
	fChain->SetBranchAddress("PUweight", &PUweight, &b_PUweight);
	fChain->SetBranchAddress("PUweight2012A", &PUweight2012A, &b_PUweight2012A);
	fChain->SetBranchAddress("PUweight2012B", &PUweight2012B, &b_PUweight2012B);
	fChain->SetBranchAddress("LumiWeight", &LumiWeight, &b_LumiWeight);
	fChain->SetBranchAddress("GenWeight", &GenWeight, &b_GenWeight);
	fChain->SetBranchAddress("weight", &weight, &b_weight);
	fChain->SetBranchAddress("weight2012A", &weight2012A, &b_weight2012A);
	fChain->SetBranchAddress("weight2012B", &weight2012B, &b_weight2012B);
	fChain->SetBranchAddress("VBFTag", VBFTag, &b_VBFTag);
	fChain->SetBranchAddress("VBFmJJ", VBFmJJ, &b_VBFmJJ);
	fChain->SetBranchAddress("VBFdeltaEta", VBFdeltaEta, &b_VBFdeltaEta);
	fChain->SetBranchAddress("VBFptjet1", VBFptjet1, &b_VBFptjet1);
	fChain->SetBranchAddress("VBFptjet2", VBFptjet2, &b_VBFptjet2);
	fChain->SetBranchAddress("VBFetajet1", VBFetajet1, &b_VBFetajet1);
	fChain->SetBranchAddress("VBFetajet2", VBFetajet2, &b_VBFetajet2);
	fChain->SetBranchAddress("VBFphijet1", VBFphijet1, &b_VBFphijet1);
	fChain->SetBranchAddress("VBFphijet2", VBFphijet2, &b_VBFphijet2);
	fChain->SetBranchAddress("massGenX", &massGenX, &b_massGenX);
	fChain->SetBranchAddress("ptGenX", &ptGenX, &b_ptGenX);
	fChain->SetBranchAddress("yGenX", &yGenX, &b_yGenX);
	fChain->SetBranchAddress("phiGenX", &phiGenX, &b_phiGenX);
	fChain->SetBranchAddress("pdgIdGenX", &pdgIdGenX, &b_pdgIdGenX);
	fChain->SetBranchAddress("massGenVll", &massGenVll, &b_massGenVll);
	fChain->SetBranchAddress("ptGenVll", &ptGenVll, &b_ptGenVll);
	fChain->SetBranchAddress("yGenVll", &yGenVll, &b_yGenVll);
	fChain->SetBranchAddress("phiGenVll", &phiGenVll, &b_phiGenVll);
	fChain->SetBranchAddress("massGenVqq", &massGenVqq, &b_massGenVqq);
	fChain->SetBranchAddress("ptGenVqq", &ptGenVqq, &b_ptGenVqq);
	fChain->SetBranchAddress("yGenVqq", &yGenVqq, &b_yGenVqq);
	fChain->SetBranchAddress("phiGenVqq", &phiGenVqq, &b_phiGenVqq);
	fChain->SetBranchAddress("nLooseMu", &nLooseMu, &b_nLooseMu);
	fChain->SetBranchAddress("nLooseEle", &nLooseEle, &b_nLooseEle);
	fChain->SetBranchAddress("mt", mt, &b_mt);
	fChain->SetBranchAddress("nbtagsL",nbtagsL);
	fChain->SetBranchAddress("nbtagsM",nbtagsM);
	fChain->SetBranchAddress("nbtagsT",nbtagsT);
	fChain->SetBranchAddress("nbtagscleanL",nbtagscleanL);
	fChain->SetBranchAddress("nbtagscleanM",nbtagscleanM);
	fChain->SetBranchAddress("nbtagscleanT",nbtagscleanT);
}

EDBRHistoMaker::EDBRHistoMaker(TTree* tree, 
		bool wantElectrons,
		bool wantMuons,
		bool wantSideband,
		bool wantSignal,
		int  wantNXJets,
		bool isZZchannel){
	fChain = 0;
	nVars = 92;

	// Definition of regions
	sidebandVHMassLow_  =  0.0;  // GeV
	sidebandVHMassHigh_ =  70.0; // GeV
	signalVHMassLow_    =  70.0; // GeV
	signalVHMassHigh_   = 105.0; // GeV

	// Which category do we want to analyze?
	wantElectrons_ = wantElectrons;
	wantMuons_ = wantMuons;
	wantSideband_ = wantSideband;
	wantSignal_ = wantSignal;
	wantNXJets_ = wantNXJets;
	isZZchannel_ =isZZchannel;

	debug_ = true;
	Init(tree);
	createAllHistos();
	printAllHistos();
}

EDBRHistoMaker::~EDBRHistoMaker() {
	if (!fChain) return;
	delete fChain->GetCurrentFile();
}                  

Int_t EDBRHistoMaker::GetEntry(Long64_t entry) {
	// Read contents of entry.
	if (!fChain) return 0;
	return fChain->GetEntry(entry);
}

Long64_t EDBRHistoMaker::LoadTree(Long64_t entry) {
	// Set the environment to read one entry
	if (!fChain) return -5;
	Long64_t centry = fChain->LoadTree(entry);
	if (centry < 0) return centry;
	if (fChain->GetTreeNumber() != fCurrent) {
		fCurrent = fChain->GetTreeNumber();
	}
	return centry;
}

//-------------------------
// Infrastructure functions
//-------------------------

void EDBRHistoMaker::createAllHistos() {
	char buffer[256];
	char buffer2[256];

	for(int i = 0; i!= nVars; ++i) {
		sprintf(buffer,"h_%s",vars[i].c_str());
		sprintf(buffer2,"%s;%s;Number of events;",vars[i].c_str(),vars[i].c_str());
		TH1D* histogram = new TH1D(buffer,
				buffer2,
				nBins[i],
				minBin[i],
				maxBin[i]);
		histogram->SetDirectory(0);
		histogram->Sumw2();
		theHistograms[vars[i]] = histogram;
	}

}

void EDBRHistoMaker::printAllHistos() {
	printf("We have %i histograms \n",int(theHistograms.size()));
	typedef std::map<std::string, TH1D*>::iterator it_type;
	for(it_type iterator = theHistograms.begin(); iterator != theHistograms.end(); iterator++) {
		//iterator->second->Print();
		// Repeat if you also want to iterate through the second map.
	}
}

void EDBRHistoMaker::saveAllHistos(std::string outFileName) {

	TFile* outFile = TFile::Open(outFileName.c_str(),"RECREATE");

	for(int i = 0; i!=nVars; ++i) {
		std::string name = vars[i];
		const TH1D* thisHisto = this->theHistograms[name];
		thisHisto->Write();
	}
	outFile->Close();
}

//------------------
// Physics functions
//------------------

bool EDBRHistoMaker::eventPassesFlavorCut(int i){
	bool passesFlavour = ((lep[i] == 0 and wantElectrons_) or
			(lep[i] == 1 and wantMuons_));

	return passesFlavour;
}


bool EDBRHistoMaker::eventPassesLep1PtCut(int i, double ptlep1_threshold) {

	bool pass = false;

	pass = ( ptlep1 [i] > ptlep1_threshold);

	return pass;

}

bool EDBRHistoMaker::eventPassesLeptonicZPtCut(int i, double ptZll_threshold){

	bool passesLeptonicZPt = false;

	passesLeptonicZPt = (ptZll[i] > ptZll_threshold);

	return passesLeptonicZPt;
}

// These functions have to aware if we're in the one jet / 
// two jet case.
bool EDBRHistoMaker::eventInSidebandRegion(int i){

	bool isInSideband = false;

	isInSideband = (region[i] == 0);

	return isInSideband;
}

bool EDBRHistoMaker::eventInSignalRegion(int i){

	bool isInSignal = false;

	isInSignal = (region[i] == 1);

	return isInSignal;
}

bool EDBRHistoMaker::eventPassesRegionCut(int i){
	bool isInSideband = eventInSidebandRegion(i);
	bool isInSignal   = eventInSignalRegion(i);
	bool passesRegion = ((isInSideband and wantSideband_) or
			(isInSignal and wantSignal_));

	return passesRegion;
}

	bool  EDBRHistoMaker::eventPassesNXJetCut(int i){
		if(wantNXJets_==-1)
			return true;
		bool passesNXJ = (nXjets[i] == wantNXJets_);
		return passesNXJ;

	}

bool EDBRHistoMaker::eventPassesVBFCut(int i){

	bool vbfFlag = false;

	vbfFlag = (VBFTag[i] == 0);

	return vbfFlag;
}



bool EDBRHistoMaker::eventPassesCut(int i, double ptZll_threshold, double ptlep1_threshold ) {

	bool passesFlavour = eventPassesFlavorCut(i);
	bool passesRegion  = eventPassesRegionCut(i);
	bool passesNXJet   = eventPassesNXJetCut(i);
	bool passesLeptonicZPt = eventPassesLeptonicZPtCut(i, ptZll_threshold);
	bool passesLep1Pt  = eventPassesLep1PtCut(i, ptlep1_threshold);
	bool passesVBF     = eventPassesVBFCut(i);
	//printf("passesFlavour: %i\n",passesFlavour);
	//printf("passesRegion: %i\n",passesRegion);
	//printf("passesNXJet: %i\n",passesNXJet);
	//printf("passesLeptonicZPt: %i\n",passesLeptonicZPt);
	bool result = 
		passesFlavour and
		passesRegion and
		passesNXJet and 
		passesLep1Pt and
		passesLeptonicZPt;

	return result;
}

///----------------------------------------------------------------
/// This is the important function, the loop over all events.
/// Here we fill the histograms according to cuts, weights,
/// and can also filter out events on an individual basis.
///----------------------------------------------------------------
void EDBRHistoMaker::Loop(std::string outFileName){

	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();

	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		if(jentry==0){
			//      printf("Entry number %i...\n",(int)jentry);
			float genLumi=1.0/LumiWeight;
			if(genLumi==1.0)genLumi=-1.0;
			if(genLumi!=-1.0) std::cout<<"Lumi of this sample: "<<genLumi <<"  /pb"<<std::endl;
			else std::cout<<"Lumi of this sample: xxx  /pb (dummy for data)"<<std::endl;

		}
		// We calculate a weight here.
		//double actualWeight = weight;//*HLTweight*PUweight*LumiWeight*GenWeight;
		double actualWeight = PUweight*LumiWeight*GenWeight;
		if(setUnitaryWeights_) {
			if(jentry==0)printf("Unitary weights set!\n");
			actualWeight=1.0;
		}
		//printf("jentry is %i\n",(int)jentry);
		// We get the histogram from the map by string and fill it.
		// We could wrap all the fills in the this->eventPassesCut()
		// to fill histograms only for the events which pass a given
		// cut (Sideband / SignalRegion, Muon / Electron, 
		// Single / Double jet ...) 

		vector<double> ptZ;
		//printf("nCands == %i\n",nCands);
		for (int iptz=0;iptz<nCands;iptz++)
		{   
			//printf("Okay, going to call check()\n");
			if(check(ptZll[iptz],&ptZ)==1){
				ptZ.push_back(ptZll[iptz]);
			}   
		}

		int wnum = ptZ.size();;

		bool filled = 0;

		for(int ivec=0;ivec<nCands;ivec++){

			if(eventPassesCut(ivec, 80, 20)){

				if(isZZchannel_==0)//WW channel, veto second loose lepton
				{
					if((nLooseEle+nLooseMu==1)&&met>40);//global selection
					else continue;	

					if(eventPassesCut(ivec, 200, 50));
					else continue;
					
					//b veto cut
					if(nbtagsT[ivec]==0) ;
					else continue;

				}
				if(filled==0)
				{   
					(theHistograms["nVL"])->Fill(wnum,actualWeight);
					(theHistograms["nCands"])->Fill(nCands,actualWeight);

					(theHistograms["PUweight"])->Fill(PUweight);
					(theHistograms["LumiWeight"])->Fill(LumiWeight);
					(theHistograms["GenWeight"])->Fill(GenWeight);

					(theHistograms["nLooseEle"])->Fill(nLooseEle,actualWeight);
					(theHistograms["nLooseMu"])->Fill(nLooseMu,actualWeight);

					filled =1; 
				} 

				(theHistograms["ptlep1"])->Fill(ptlep1[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["ptlep2"])->Fill(ptlep2[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["ptjet1"])->Fill(ptjet1[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["ptjet2"])->Fill(ptjet2[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["ptZll"])->Fill(ptZll[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["ptZjj"])->Fill(ptZjj[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["yZll"])->Fill(yZll[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["yZjj"])->Fill(yZjj[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["mLL"])->Fill(mLL[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["mJJ"])->Fill(mJJ[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["mZZ"])->Fill(mZZ[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["prunedmass"])->Fill(prunedmass[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["mdrop"])->Fill(mdrop[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["mJJNoKinFit"])->Fill(mJJNoKinFit[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				
				/// Thiago: This is temporarily here for the optimization.
				/// Should probably be made better.
				double thisMZZ = mZZ[ivec];
				double minMass = 1500.0 * 0.85;
				double maxMass = 1500.0 * 1.15;
				
				if(thisMZZ > minMass and
				   thisMZZ < maxMass) {
				  (theHistograms["nsubj21"])->Fill(1.0/nsubj12[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				}
				
				(theHistograms["nVtx"])->Fill(nVtx,actualWeight);//printf("line number %i\n",__LINE__);

				(theHistograms["nXjets"])->Fill(nXjets[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["betajet1"])->Fill(betajet1[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["isomu1mod"])->Fill(isomu1mod[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["isomu2mod"])->Fill(isomu2mod[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["isoele1trk"])->Fill(isoele1trk[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["isoele2trk"])->Fill(isoele2trk[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["isoele1calo"])->Fill(isoele1calo[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["isoele2calo"])->Fill(isoele2calo[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["nJets"])->Fill(nJets,actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["met"])->Fill(met,actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["metSign"])->Fill(metSign,actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["etalep1"])->Fill(etalep1[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["etalep2"])->Fill(etalep2[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["etajet1"])->Fill(etajet1[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["etajet2"])->Fill(etajet2[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["deltaREDBR"])->Fill(deltaREDBR[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["deltaRleplep"])->Fill(deltaRleplep[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["deltaRjetjet"])->Fill(deltaRjetjet[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["VBFTag"])->Fill(VBFTag[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["VBFmJJ"])->Fill(VBFmJJ[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["VBFdeltaEta"])->Fill(VBFdeltaEta[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["mt"])->Fill(mt[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["lep"])->Fill(lep[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["nbtagsL"])->Fill(nbtagsL[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["nbtagsM"])->Fill(nbtagsM[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["nbtagsT"])->Fill(nbtagsT[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["nbtagscleanL"])->Fill(nbtagscleanL[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["nbtagscleanM"])->Fill(nbtagscleanM[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				(theHistograms["nbtagscleanT"])->Fill(nbtagscleanT[ivec],actualWeight);//printf("line number %i\n",__LINE__);

				// (theHistograms[""])->Fill([ivec],actualWeight);

			}//end if eventPassesCut
		}//end loop over nCands
	}//end loop over entries

	if(isZZchannel_==0)//WW channel, change the names, which will be the plot lable
	{
		(theHistograms["ptlep1"])->SetName("h_ptlepton");
		(theHistograms["ptlep2"])->SetName("h_ptneutrino");
		(theHistograms["ptZll"])->SetName("h_ptWL");
		(theHistograms["ptZjj"])->SetName("h_ptWjj");
		(theHistograms["yZll"])->SetName("h_yWL");
		(theHistograms["yZjj"])->SetName("h_yWjj");
		(theHistograms["mLL"])->SetName("h_mWL");
		(theHistograms["mZZ"])->SetName("h_mWW");
		(theHistograms["etalep1"])->SetName("h_etalep");
		(theHistograms["etalep2"])->SetName("h_etaneu");
	}

	std::cout<<"From makeHisto: the histo with #vtx has "<<(theHistograms["nVtx"])->GetEntries()<<" entries"<<std::endl;
	this->saveAllHistos(outFileName);
}

///
/// This function loops over the entries and
/// calculates the number of events passing the cut.
/// It doesn't fill histograms. Useful for calculating
/// figures of merit and optimization
///
double EDBRHistoMaker::FastLoop(double lumiValue, double kFactor, double nsubjetinessCut, double massPoint, double percentageWindow){

	if (fChain == 0) return -1;

	Long64_t nentries = fChain->GetEntriesFast();

	Long64_t nbytes = 0, nb = 0;

	double totalWeight = 0.0;

	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		if(jentry==0){
			float genLumi=1.0/LumiWeight;
			if(genLumi==1.0)genLumi=-1.0;
			if(genLumi!=-1.0) std::cout<<"Lumi of this sample: "<<genLumi <<"  /pb"<<std::endl;
			else std::cout<<"Lumi of this sample: xxx  /pb (dummy for data)"<<std::endl;

		}

		// We calculate a weight here.
		//double actualWeight = weight;//*HLTweight*PUweight*LumiWeight*GenWeight;
		double actualWeight = PUweight*LumiWeight*GenWeight;
		if(setUnitaryWeights_) {
			if(jentry==0)printf("Unitary weights set!\n");
			actualWeight=1.0;
		}
		//printf("jentry is %i\n",(int)jentry);

		for(int ivec=0;ivec<nCands;ivec++){
		
		  if(eventPassesCut(ivec, 80, 20)){
		    double nsubjetiness = 1.0/nsubj12[ivec];
		    double thisMZZ = mZZ[ivec];
		    double minMass = massPoint*(1.0-percentageWindow);
		    double maxMass = massPoint*(1.0+percentageWindow);
		    
		    if(nsubjetiness < nsubjetinessCut and
		       thisMZZ > minMass and
		       thisMZZ < maxMass) {
		      totalWeight += actualWeight;
		    }
		  }
		
		}//end loop over nCands
	}//end loop over entries

	return totalWeight*lumiValue*kFactor;
}
