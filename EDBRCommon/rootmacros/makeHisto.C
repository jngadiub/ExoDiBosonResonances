#include <string>
#include <sstream>
#include <math.h>

#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TAxis.h"

int makeHisto(){

	gROOT->Reset();

	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);

	TFile *inFile = new TFile("tree_ANGELO.root");
	TFile *outFile = new TFile("Histograms.root", "recreate");

	TTree *t_ = (TTree*)inFile->Get("SelectedCandidates");

	int nVars = 77;
//	int nVars = 7;
	
	const string vars[] = {"nCands", "cosThetaStar", "cosTheta1", "cosTheta2", "phi", "phiStar1", "ptlep1",
				"ptlep2", "ptjet1", "ptjet2", "ptZll", "ptZjj", "yZll", "yZjj",
				"phiZll", "phiZjj", "etalep1", "etalep2", "etajet1", "etajet2", "philep1",
				"philep2", "phijet1", "phijet2", "lep", "region", "mZZ", "mZZNoKinFit",
				"ptmzz", "ptmzzNoKinFit", "mLL", "mJJ", "mJJNoKinFit", "met", "metSign",
				"nBTags", "deltaREDBR", "deltaRleplep", "deltaRjetjet", "qgProduct", "qgjet1", "qgjet2",
				"betajet1", "betajet2", "puMvajet1", "puMvajet2", "nXjets", "prunedmass", "mdrop",
				"nsubj12", "nsubj23", "tau1", "tau2", "qjet", "isolep1", "isolep2",
				"eleMVAId1", "eleMVAId2", "LD", "q1fl", "q2fl", "MCmatch", "nVtx",
				"nJets", "nPU", "HLTweight", "PUweight", "PUweight2012A", "PUweight2012B", "LumiWeight",
				"GenWeight", "weight", "weight2012A", "weight2012B", "event", "run", "ls"};

	// phijet1, phijet2, region and nPU have problems in the original tree
	int nBins[] = {13, 100, 100, 100, 100, 100, 100,
			100, 100, 100, 100, 100, 100, 100,
			100, 100, 100, 100, 100, 100, 100,
			100, 100, 100, 100, 100, 100, 100,
			100, 100, 100, 100, 100, 100, 100,
			100, 100, 100, 100, 100, 100, 100,
			100, 100, 100, 100, 3, 100, 100,
			100, 100, 100, 100, 100, 100, 100,
			100, 100, 100, 4, 4, 100, 43,
			7, 2, 100, 100, 100, 100, 100,
			100, 100, 100, 100, 102, 3, 102};

	double minBin[] = {0.0,-1.15, -1.15, -84., -3.7, -3.7, 34.,
			15., 0., -120., 44., 0., -2.8, -2.7,
			-3.7, -3.7, -2.8, -2.8, -2.8, -108., -3.7,
			-3.7, 0., 0., 0., 0., 100., 100.,
			0., 0., 66.5, 47., 43., 0., -2.2,
			-2.2, -1.2, 0.96, 0., -100.2, -100.2, -100.2,
			0., -1.2, -10800., -10800., 0., -115., -1080.,
			-1080., -1080., -1080., -1080., -1080., 0., 0.,
			-1.15, -1.15, -100.2, -101., -101., -1.2, 0.,
			1., 0., 0.99, 0., 0., 0., -1.15,
			0., 0., 0., 0., 2490000., 0., 6280.};

	double maxBin[] = {13.0,1.15, 1.15, 8., 3.7, 3.7, 131.,
			81., 365., 140., 154., 370., 2.8, 2.8,
			3.7, 3.7, 2.9, 2.9, 2.8, 12., 3.7,
			3.7, 1., 1., 1.1, 1.0, 1180., 1160.,
			320., 330., 113.0, 96., 137., 102., 0.,
			0., 1.2, 3.34, 4.35, -97.8, -97.8, -97.8,
			1.1, 1.2, 1000., 1000., 3., 110., 100.,
			100., 100., 100., 100., 100., 1.4, 5.4,
			1.15, 1.15, -97.8, -97., -97., 1.2, 43.,
			8., 1., 1., 16., 140., 16., 1.2,
			2.2, 0.02, 0.14, 0.02, 54510000., 3., 181720.};

	TH1D* histoVars[];
	TCanvas* canvas[];

	string histoName;
	string drawToHisto;
	string canvasName;
	string XaxisName;

	for(int i = 0; i < nVars; i++){

		if ( (vars[i] == "phijet1") || (vars[i] == "phijet2") || (vars[i] == "region") || (vars[i] == "nPU") )
			continue;

//		string histoName;
		histoName = "h_" + vars[i];
		histoVars[i] = new TH1D( histoName.c_str(), "", nBins[i], minBin[i], maxBin[i]);

//		string drawToHisto;
		drawToHisto = vars[i] + ">>" + histoName.c_str();
		t_->Draw( drawToHisto.c_str() );

//		drawHisto(canvas, histoVars, vars, i);

//		string canvasName;
		canvasName = "canvas_" + vars[i];

		canvas[i] = new TCanvas( canvasName.c_str(), canvasName.c_str(), 600, 600);
		canvas[i]->cd();

		histoVars[i]->GetYaxis()->SetTitle("# Entries");
		histoVars[i]->GetYaxis()->SetTitleOffset(1.30);

//		string XaxisName;
		XaxisName = vars[i];
		histoVars[i]->GetXaxis()->SetTitle( XaxisName.c_str() );
		histoVars[i]->GetXaxis()->SetTitleOffset(1.15);

		histoVars[i]->Draw();

//		canvas[i]->Modified();
//		canvas[i]->Update();
		canvas[i]->Write();
		canvas[i]->Close();

		histoName.clear();
		drawToHisto.clear();
		canvasName.clear();
		XaxisName.clear();
	}

	for (int i = 0; i < nVars; i++)
		delete histoVars[i];
	
	outFile->Write();
	return 0;
}
