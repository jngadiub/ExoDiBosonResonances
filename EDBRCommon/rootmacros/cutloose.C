#include <vector>
#include "TH1.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include "TCanvas.h"
#include "TStyle.h"

void cutloose()
{
	TString inputpath = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv2/full";
	vector<TString> dataSamples;
	vector<TString> bkgSamples;
	vector<TString> sigSamples;

	dataSamples.push_back("SingleMu_Run2012A_13Jul2012_xww");
	dataSamples.push_back("SingleMu_Run2012A_recover_xww");
	dataSamples.push_back("SingleMu_Run2012B_13Jul2012_xww");
	dataSamples.push_back("SingleMu_Run2012C_24Aug2012_xww");
	dataSamples.push_back("SingleMu_Run2012C_PromptReco_xww");
	dataSamples.push_back("SingleMu_Run2012D_PromptReco_xww");

	bkgSamples.push_back("TTBAR_xww");
	bkgSamples.push_back("WW_xww");
	bkgSamples.push_back("WZ_xww");
	bkgSamples.push_back("ZZ_xww");
	bkgSamples.push_back("WJetsPt50To70_xww");
	bkgSamples.push_back("WJetsPt70To100_xww");
	bkgSamples.push_back("WJetsPt100_xww");
	bkgSamples.push_back("DYJetsPt50To70_xww");
	bkgSamples.push_back("DYJetsPt70To100_xww");
	bkgSamples.push_back("DYJetsPt100_xww");

	sigSamples.push_back("RSG_WW_lvjj_c0p2_M600_xww");
	sigSamples.push_back("RSG_WW_lvjj_c0p2_M1000_xww");
	sigSamples.push_back("RSG_WW_lvjj_c0p2_M1500_xww");
	sigSamples.push_back("BulkG_WW_lvjj_c1p0_M600_xww");
	sigSamples.push_back("BulkG_WW_lvjj_c1p0_M1000_xww");
	sigSamples.push_back("BulkG_WW_lvjj_c1p0_M1500_xww");

	int ndata = dataSamples.size();
	int nbkg  = bkgSamples.size();
	int nsig  = sigSamples.size();

	cout<<"data samples "<<ndata<<"  bkg samples "<<nbkg<<"  signal samples "<<nsig<<endl;	

	TH1F * h1 = new TH1F ("CutLooseEff","CutLooseEff",22,0,22);
	h1->SetBit(TH1::kCanRebin);
	h1->SetStats(0);
	gStyle->SetPaintTextFormat("1.4f");
	//h1->Sumw2();

	TCanvas * c1 = new TCanvas();	

	for(int i =0; i<ndata+nbkg+nsig; i++   )
	{
		TString filename;
		TString samplename;
		if(i<ndata) 
		{
			filename = "treeEDBR_"+dataSamples.at(i)+".root";
			samplename = dataSamples.at(i);//SingleMu_Run2012A_13Jul2012_xww
			samplename.Remove(samplename.Length()-4,4);//SingleMu_Run2012A_13Jul2012
			samplename.Remove(0,12);//2012A_13Jul2012
		}
		else if(i>=ndata&&i<(ndata+nbkg)) 
		{
			filename = "treeEDBR_"+bkgSamples.at(i-ndata)+".root";
			samplename = bkgSamples.at(i-ndata);//TTBAR_xww
			samplename.Remove(samplename.Length()-4,4);//TTBAR
		}
		else if(i>=(ndata+nbkg)) 
		{
			filename = "treeEDBR_"+sigSamples.at(i-ndata-nbkg)+".root";
			samplename = sigSamples.at(i-ndata-nbkg);//RSG_WW_lvjj_c0p2_M1500_xww,BulkG_WW_lvjj_c1p0_M1500_xww
			samplename.Remove(samplename.Length()-4,4);//RSG_WW_lvjj_c0p2_M1500,BulkG_WW_lvjj_c1p0_M1500
			if(samplename.Contains("RSG"))samplename.Remove(4,8);//RSG_c0p2_M1500
			if(samplename.Contains("BulkG"))samplename.Remove(6,8);//BulkG_c1p0_M1500
		}
		cout<<filename<<endl;
		cout<<samplename<<endl;


		TFile *fp = new TFile(inputpath+"/"+filename);
		TTree * tree = (TTree *) fp->Get("SelectedCandidates");
		int entries = tree->GetEntries();
		cout<<"entries: "<<entries<<endl;

		int pass=0;
		int nLooseEle;
		int nLooseMu;
		tree->SetBranchAddress("nLooseMu", &nLooseMu);
		tree->SetBranchAddress("nLooseEle",&nLooseEle); 

		for(int j=0;j<entries;j++)
		{
			tree->GetEntry(j);
			if(nLooseEle+nLooseMu==1)pass++;
		}
		
		double eff = (double)pass/(double)entries;
		cout<<"total: "<<entries<<" pass: "<<pass<<" eff: "<<eff<<endl;

		h1->Fill(samplename,eff);	

	}

	h1->Draw();
	h1->Draw("TEXT0same");
	c1->SetGridy(1);
	c1->SaveAs("LooseVetoEff.png");

}
