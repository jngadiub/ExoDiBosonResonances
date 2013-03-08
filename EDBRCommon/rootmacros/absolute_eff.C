#include <vector>
#include "TH1.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include "TCanvas.h"
#include "TStyle.h"

void absolute_eff()
{
	TString inputpath = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv6/goodbtag/AK7/fullsig/";
	TString cut = "absolute_efficiency";
	vector<TString> dataSamples;
	vector<TString> bkgSamples;
	vector<TString> sigSamples;

	bool weightedeff = true;

	double lumi = 19538.85;
/*
	dataSamples.push_back("SingleMu_Run2012A_13Jul2012_xww");
	dataSamples.push_back("SingleMu_Run2012A_recover_xww");
	dataSamples.push_back("SingleMu_Run2012B_13Jul2012_xww");
	dataSamples.push_back("SingleMu_Run2012C_24Aug2012_xww");
	dataSamples.push_back("SingleMu_Run2012C_PromptReco_xww");
	dataSamples.push_back("SingleMu_Run2012D_PromptReco_xww");
*/

	bkgSamples.push_back("TTBAR_xww");
	bkgSamples.push_back("SingleTopBarTWchannel_xww");
	bkgSamples.push_back("SingleTopTWchannel_xww");
	bkgSamples.push_back("SingleTopBarSchannel_xww" );
	bkgSamples.push_back("SingleTopSchannel_xww");
	bkgSamples.push_back("SingleTopBarTchannel_xww");
	bkgSamples.push_back("SingleTopTchannel_xww");			     
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

	TH1F * h1 = new TH1F (cut,cut,ndata+nbkg+nsig,0,ndata+nbkg+nsig);
	//h1->GetYaxis()->SetRangeUser(0,1);
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

		double pass=0;
		double total=0;
		double crossSection=0;
		//gen events and crossSection
		int Ngen=0;
		double xsec=0;
		//btags
		double nbtagsL[99];
		double nbtagsM[99];
		double nbtagsT[99];
		double nbtagscleanL[99];
		double nbtagscleanM[99];
		double nbtagscleanT[99];
		//to make cuts
		double met;
		double ptlep1[99];
		double etalep1[99];
		double ptZll[99];
		double ptZjj[99];
		double lep[99];
		int    nCands;
		int    nXjets[99];
		int nLooseEle;
		int nLooseMu;
		//per event weight
		double PUweight=-1;
		double LumiWeight=-1;
		double GenWeight=-1;
		double region[99];

		tree->SetBranchAddress("Ngen", &Ngen);
		tree->SetBranchAddress("xsec", &xsec);
		tree->SetBranchAddress("nLooseMu", &nLooseMu);
		tree->SetBranchAddress("nLooseEle",&nLooseEle); 
		tree->SetBranchAddress("nbtagsL",nbtagsL); 
		tree->SetBranchAddress("nbtagsM",nbtagsM); 
		tree->SetBranchAddress("nbtagsT",nbtagsT); 
		tree->SetBranchAddress("nbtagscleanL",nbtagscleanL); 
		tree->SetBranchAddress("nbtagscleanM",nbtagscleanM); 
		tree->SetBranchAddress("nbtagscleanT",nbtagscleanT);

		tree->SetBranchAddress("ptlep1", ptlep1); 
		tree->SetBranchAddress("etalep1", etalep1); 
		tree->SetBranchAddress("ptZll", ptZll);
		tree->SetBranchAddress("ptZjj", ptZjj);
		tree->SetBranchAddress("met", &met);
		tree->SetBranchAddress("nXjets", nXjets);
		tree->SetBranchAddress("lep", lep);
		tree->SetBranchAddress("nCands", &nCands);

		tree->SetBranchAddress("PUweight", &PUweight);
		tree->SetBranchAddress("LumiWeight", &LumiWeight);
		tree->SetBranchAddress("GenWeight", &GenWeight);
		tree->SetBranchAddress("region", region);
		
	
		bool filled = 0;

		for(int j=0;j<entries;j++)
		{
			tree->GetEntry(j);
			
			if(j==0)
			{
				total = Ngen;
				crossSection = xsec;
			}
				
			//per event weight
			double actualWeight = PUweight*LumiWeight*GenWeight*lumi;
			if(i<ndata) actualWeight =1; //for data
			//cout<<actualWeight<<endl;

			if(weightedeff==false)actualWeight=1;

			filled = 0;
			//if(nCands==0)continue;
			for(int ivec=0;ivec<nCands;ivec++){

				if(filled==0)//cout the event as pass if any candidate pass all the cuts
				{
					//make cuts
					if(lep[ivec]!=1.)continue;// 1 is for mu
					if(region[ivec]!=1)continue;// 1 is mjj signal region
					if((nLooseEle+nLooseMu)!=1)continue;
					if(met<40)continue;
					if(ptlep1[ivec]<50)continue;
					if(etalep1[ivec]<-2.1)continue;
					if(etalep1[ivec]>2.1)continue;
					if(ptZll[ivec]<200)continue;
					if(ptZjj[ivec]<200)continue;
					if(nXjets[ivec]!=2)continue;// 1 jet candidate
					if(nbtagsM[ivec]!=0)continue;

					pass=pass+actualWeight;

					filled =1;
				}
			}//end of candidate loop

		}//end of tree loop

		double eff=0;
		
		if(weightedeff==true)total = crossSection*lumi;

		if(total!=0)
		{
			eff = (double)pass/(double)total;
		}

		cout<<"crossSection: "<<crossSection<<" total: "<<total<<" pass: "<<pass<<" eff: "<<eff<<endl;

		
		h1->Fill(samplename,eff);	

	}//end of sample loop

	c1->SetGridy(1);

	h1->SetTitle(cut);
	h1->Draw();
	h1->Draw("TEXT0same");
	c1->SaveAs(cut+".png");

}
