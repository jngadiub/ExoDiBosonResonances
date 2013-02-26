#include <vector>
#include "TH1.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include "TCanvas.h"
#include "TStyle.h"

void cuteff_weighted()
{
	TString inputpath = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv5/fullsig/";
	TString cut = "bveto";
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

	//TH1F * h1 = new TH1F (cut,cut,ndata+nbkg+nsig+1,0,ndata+nbkg+nsig+1);
	TH1F * h1 = new TH1F (cut,cut,6,0,6);
	//h1->GetYaxis()->SetRangeUser(0,1);
	h1->SetBit(TH1::kCanRebin);
	h1->SetStats(0);
	gStyle->SetPaintTextFormat("1.4f");
	//h1->Sumw2();
	TH1F *h2 = (TH1F*)h1->Clone(cut+"_2");
	TH1F *h3 = (TH1F*)h1->Clone(cut+"_3");
	TH1F *h4 = (TH1F*)h1->Clone(cut+"_4");
	TH1F *h5 = (TH1F*)h1->Clone(cut+"_5");
	TH1F *h6 = (TH1F*)h1->Clone(cut+"_6");
	TH1F *h7 = (TH1F*)h1->Clone(cut+"_7");

	TCanvas * c1 = new TCanvas();	

	double totalB1=0;
	double totalB2=0;
	double totalB3=0;
	double totalB4=0;
	double totalB5=0;
	double totalB6=0;
	double totalB=0;//before veto
	double sigeff1[10];
	double sigeff2[10];
	double sigeff3[10];
	double sigeff4[10];
	double sigeff5[10];
	double sigeff6[10];

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

		double pass1=0;
		double pass2=0;
		double pass3=0;
		double pass4=0;
		double pass5=0;
		double pass6=0;
		double total=0;
		int nLooseEle;
		int nLooseMu;
		double nbtagsL[99];
		double nbtagsM[99];
		double nbtagsT[99];
		double nbtagscleanL[99];
		double nbtagscleanM[99];
		double nbtagscleanT[99];

		//to make cuts
		double met;
		double ptlep1[99];
		double ptZll[99];
		double lep[99];
		int    nCands;
		int    nXjets[99];

		//per event weight
		double PUweight=-1;
		double LumiWeight=-1;
		double GenWeight=-1;


		tree->SetBranchAddress("nLooseMu", &nLooseMu);
		tree->SetBranchAddress("nLooseEle",&nLooseEle); 
		tree->SetBranchAddress("nbtagsL",nbtagsL); 
		tree->SetBranchAddress("nbtagsM",nbtagsM); 
		tree->SetBranchAddress("nbtagsT",nbtagsT); 
		tree->SetBranchAddress("nbtagscleanL",nbtagscleanL); 
		tree->SetBranchAddress("nbtagscleanM",nbtagscleanM); 
		tree->SetBranchAddress("nbtagscleanT",nbtagscleanT);

		tree->SetBranchAddress("ptlep1", ptlep1); 
		tree->SetBranchAddress("ptZll", ptZll);
		tree->SetBranchAddress("met", &met);
		tree->SetBranchAddress("nXjets", nXjets);
		tree->SetBranchAddress("lep", lep);
		tree->SetBranchAddress("nCands", &nCands);

		tree->SetBranchAddress("PUweight", &PUweight);
		tree->SetBranchAddress("LumiWeight", &LumiWeight);
		tree->SetBranchAddress("GenWeight", &GenWeight);

		bool filled = 0;

		for(int j=0;j<entries;j++)
		{
			tree->GetEntry(j);

			//per event weight
			double actualWeight = PUweight*LumiWeight*GenWeight*19538.85;
			if(i<ndata) actualWeight =1; //for data
			//cout<<actualWeight<<endl;

			filled = 0;
			//if(nCands==0)continue;
			for(int ivec=0;ivec<nCands;ivec++){

				if(filled==0)
				{
					//cout<<nXjets[ivec]<<endl;
					//make cuts
					if((nLooseEle+nLooseMu)!=1)continue;
					if(met<40)continue;
					if(lep[ivec]!=1.)continue;//mu
					if(ptlep1[ivec]<50)continue;
					if(ptZll[ivec]<200)continue;
					if(nXjets[ivec]!=1)continue;
					total = total + actualWeight;

					if(nbtagsL[ivec]==0)pass1=pass1+actualWeight;
					if(nbtagsM[ivec]==0)pass2=pass2+actualWeight;
					if(nbtagsT[ivec]==0)pass3=pass3+actualWeight;
					if(nbtagscleanL[ivec]==0)pass4=pass4+actualWeight;
					if(nbtagscleanM[ivec]==0)pass5=pass5+actualWeight;
					if(nbtagscleanT[ivec]==0)pass6=pass6+actualWeight;

					filled =1;
				}
			}//end of tree loop

		}

		double eff1=0;
		double eff2=0;
		double eff3=0;
		double eff4=0;
		double eff5=0;
		double eff6=0;
		if(total!=0)
		{
			eff1 = (double)pass1/(double)total;
			eff2 = (double)pass2/(double)total;
			eff3 = (double)pass3/(double)total;
			eff4 = (double)pass4/(double)total;
			eff5 = (double)pass5/(double)total;
			eff6 = (double)pass6/(double)total;
		}
		cout<<"total: "<<total<<" pass1: "<<pass1<<" eff: "<<eff1<<endl;
		cout<<"total: "<<total<<" pass2: "<<pass2<<" eff: "<<eff2<<endl;
		cout<<"total: "<<total<<" pass3: "<<pass3<<" eff: "<<eff3<<endl;
		cout<<"total: "<<total<<" pass4: "<<pass4<<" eff: "<<eff4<<endl;
		cout<<"total: "<<total<<" pass5: "<<pass5<<" eff: "<<eff5<<endl;
		cout<<"total: "<<total<<" pass6: "<<pass6<<" eff: "<<eff6<<endl;

		/*
		h1->Fill(samplename,eff1);	
		h2->Fill(samplename,eff2);	
		h3->Fill(samplename,eff3);	
		h4->Fill(samplename,eff4);	
		h5->Fill(samplename,eff5);	
		h6->Fill(samplename,eff6);
		*/
		/*
        h1->Fill(samplename,pass1);  
        h2->Fill(samplename,pass2);  
        h3->Fill(samplename,pass3);  
        h4->Fill(samplename,pass4);  
        h5->Fill(samplename,pass5);  
        h6->Fill(samplename,pass6);
        h7->Fill(samplename,total);
		*/

		if(i>=ndata&&i<(ndata+nbkg))//for bkg, sum all passed
		{
			totalB1=totalB1+pass1;
			totalB2=totalB2+pass2;
			totalB3=totalB3+pass3;
			totalB4=totalB4+pass4;
			totalB5=totalB5+pass5;
			totalB6=totalB6+pass6;
			totalB =totalB+ total;
		}
		if(i>=(ndata+nbkg))//for signal, save the efficiency
		{
			sigeff1[i-ndata-nbkg]=eff1;
			sigeff2[i-ndata-nbkg]=eff2;
			sigeff3[i-ndata-nbkg]=eff3;
			sigeff4[i-ndata-nbkg]=eff4;
			sigeff5[i-ndata-nbkg]=eff5;
			sigeff6[i-ndata-nbkg]=eff6;
		}


	}//end of sample loop

	/*
        h1->Fill("total",totalB1);
        h2->Fill("total",totalB2);
        h3->Fill("total",totalB3);  
        h4->Fill("total",totalB4);  
        h5->Fill("total",totalB5);  
        h6->Fill("total",totalB6);
		h7->Fill("total",totalB);
	*/

	cout<<"total B before veto is "<<totalB<<endl;
	c1->SetGridy(1);

	///*
	for(int i =0; i<nsig; i++   )
	{
		TString filename;
		TString samplename;
		filename = "treeEDBR_"+sigSamples.at(i)+".root";
		samplename = sigSamples.at(i);//RSG_WW_lvjj_c0p2_M1500_xww,BulkG_WW_lvjj_c1p0_M1500_xww
		samplename.Remove(samplename.Length()-4,4);//RSG_WW_lvjj_c0p2_M1500,BulkG_WW_lvjj_c1p0_M1500
		if(samplename.Contains("RSG"))samplename.Remove(4,8);//RSG_c0p2_M1500
		if(samplename.Contains("BulkG"))samplename.Remove(6,8);//BulkG_c1p0_M1500
		if(i==0)
		{
			h1->Fill("nbtagsL",sigeff1[i]/(1+sqrt(totalB1)));	
			h1->Fill("nbtagsM",sigeff2[i]/(1+sqrt(totalB2)));	
			h1->Fill("nbtagsT",sigeff3[i]/(1+sqrt(totalB3)));	
			h1->Fill("nbtagscleanL",sigeff4[i]/(1+sqrt(totalB4)));	
			h1->Fill("nbtagscleanM",sigeff5[i]/(1+sqrt(totalB5)));	
			h1->Fill("nbtagscleanT",sigeff6[i]/(1+sqrt(totalB6)));	
			h1->SetTitle(cut+"_significance_"+samplename);
			h1->Draw();
			h1->Draw("TEXT0same");
			c1->SaveAs(cut+"_significance_"+samplename+".png");
		}
		if(i==1)
		{   
			h2->Fill("nbtagsL",sigeff1[i]/(1+sqrt(totalB1)));   
			h2->Fill("nbtagsM",sigeff2[i]/(1+sqrt(totalB2)));   
			h2->Fill("nbtagsT",sigeff3[i]/(1+sqrt(totalB3)));   
			h2->Fill("nbtagscleanL",sigeff4[i]/(1+sqrt(totalB4)));  
			h2->Fill("nbtagscleanM",sigeff5[i]/(1+sqrt(totalB5)));  
			h2->Fill("nbtagscleanT",sigeff6[i]/(1+sqrt(totalB6)));
			h2->SetTitle(cut+"_significance_"+samplename);
			h2->Draw();
			h2->Draw("TEXT0same");
			c1->SaveAs(cut+"_significance_"+samplename+".png");  
		} 
		if(i==2)
		{   
			h3->Fill("nbtagsL",sigeff1[i]/(1+sqrt(totalB1)));   
			h3->Fill("nbtagsM",sigeff2[i]/(1+sqrt(totalB2)));   
			h3->Fill("nbtagsT",sigeff3[i]/(1+sqrt(totalB3)));   
			h3->Fill("nbtagscleanL",sigeff4[i]/(1+sqrt(totalB4)));  
			h3->Fill("nbtagscleanM",sigeff5[i]/(1+sqrt(totalB5)));  
			h3->Fill("nbtagscleanT",sigeff6[i]/(1+sqrt(totalB6)));  
			h3->SetTitle(cut+"_significance_"+samplename);
			h3->Draw();
			h3->Draw("TEXT0same");
			c1->SaveAs(cut+"_significance_"+samplename+".png");
		} 
		if(i==3)
		{   
			h4->Fill("nbtagsL",sigeff1[i]/(1+sqrt(totalB1)));   
			h4->Fill("nbtagsM",sigeff2[i]/(1+sqrt(totalB2)));   
			h4->Fill("nbtagsT",sigeff3[i]/(1+sqrt(totalB3)));   
			h4->Fill("nbtagscleanL",sigeff4[i]/(1+sqrt(totalB4)));  
			h4->Fill("nbtagscleanM",sigeff5[i]/(1+sqrt(totalB5)));  
			h4->Fill("nbtagscleanT",sigeff6[i]/(1+sqrt(totalB6)));  
			h4->SetTitle(cut+"_significance_"+samplename);
			h4->Draw();
			h4->Draw("TEXT0same");
			c1->SaveAs(cut+"_significance_"+samplename+".png");
		} 
		if(i==4)
		{   
			h5->Fill("nbtagsL",sigeff1[i]/(1+sqrt(totalB1)));   
			h5->Fill("nbtagsM",sigeff2[i]/(1+sqrt(totalB2)));   
			h5->Fill("nbtagsT",sigeff3[i]/(1+sqrt(totalB3)));   
			h5->Fill("nbtagscleanL",sigeff4[i]/(1+sqrt(totalB4)));  
			h5->Fill("nbtagscleanM",sigeff5[i]/(1+sqrt(totalB5)));  
			h5->Fill("nbtagscleanT",sigeff6[i]/(1+sqrt(totalB6)));  
			h5->SetTitle(cut+"_significance_"+samplename);
			h5->Draw();
			h5->Draw("TEXT0same");
			c1->SaveAs(cut+"_significance_"+samplename+".png");
		} 
		if(i==5)
		{   
			h6->Fill("nbtagsL",sigeff1[i]/(1+sqrt(totalB1)));   
			h6->Fill("nbtagsM",sigeff2[i]/(1+sqrt(totalB2)));   
			h6->Fill("nbtagsT",sigeff3[i]/(1+sqrt(totalB3)));   
			h6->Fill("nbtagscleanL",sigeff4[i]/(1+sqrt(totalB4)));  
			h6->Fill("nbtagscleanM",sigeff5[i]/(1+sqrt(totalB5)));  
			h6->Fill("nbtagscleanT",sigeff6[i]/(1+sqrt(totalB6)));  
			h6->SetTitle(cut+"_significance_"+samplename);
			h6->Draw();
			h6->Draw("TEXT0same");
			c1->SaveAs(cut+"_significance_"+samplename+".png");
		} 
	}
		
	//*/	

	/*
	h1->SetTitle(cut+"_nbtagsL");
	h1->Draw();
	h1->Draw("TEXT0same");
	c1->SaveAs(cut+"_nbtagsL.png");

	h2->SetTitle(cut+"_nbtagsM");
	h2->Draw();
	h2->Draw("TEXT0same");
	c1->SaveAs(cut+"_nbtagsM.png");

	h3->SetTitle(cut+"_nbtagsT");
	h3->Draw();
	h3->Draw("TEXT0same");
	c1->SaveAs(cut+"_nbtagsT.png");

	h4->SetTitle(cut+"_nbtagscleanL");
	h4->Draw();
	h4->Draw("TEXT0same");
	c1->SaveAs(cut+"_nbtagscleanL.png");

	h5->SetTitle(cut+"_nbtagscleanM");
	h5->Draw();
	h5->Draw("TEXT0same");
	c1->SaveAs(cut+"_nbtagscleanM.png");

	h6->SetTitle(cut+"_nbtagscleanT");
	h6->Draw();
	h6->Draw("TEXT0same");
	c1->SaveAs(cut+"_nbtagscleanT.png");	

    h7->SetTitle(cut+"_beforeveto");
    h7->Draw();
    h7->Draw("TEXT0same");
    c1->SaveAs(cut+"_beforeveto.png");  
	*/

}
