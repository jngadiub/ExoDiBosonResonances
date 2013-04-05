#include <vector>
#include "TH1D.h"
#include "TH1F.h"
#include "TF1.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <fstream>
#include "TCanvas.h"
#include "TStyle.h"
using namespace std;

double deltaPhi(const double& phi1, const double& phi2)
{   
	double deltaphi = fabs(phi1 - phi2);
	if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi;
	return deltaphi;
}

//  ------------------------------------------------------------

double deltaEta(const double& eta1, const double& eta2)
{
	double deltaeta = fabs(eta1 - eta2);
	return deltaeta;
}

//  ------------------------------------------------------------

double deltaR(const double& eta1, const double& phi1,
		const double& eta2, const double& phi2)
{   
	double deltaphi = deltaPhi(phi1, phi2);
	double deltaeta = deltaEta(eta1, eta2);
	double deltar = sqrt(deltaphi*deltaphi + deltaeta*deltaeta);
	return deltar;
}

void absolute_eff()
{
	//TString inputpath = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_eleid/AnaSigTree";
	TString inputpath = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_eleid/fullallrange";
	//TString inputpath = "/afs/cern.ch/user/t/tomei/work/public/EXOVV_2012/analyzer_trees/productionv5/fullsigCA8/";
	TString cut = "absolute_efficiency";
	vector<TString> dataSamples;
	vector<TString> bkgSamples;
	vector<TString> sigSamples;


	bool weightedeff = true;
	bool isZZChannel = false;
	/*
	   double lepCut=0.0;   // 0->ele 1->mu
	   double nxjCut=1.0;  // -1 means you take both single and double jet
	   double nsubjCut=1.0; // The cut is nsubj21 < than this 
	 */

	//for eff lumi doesnt matter
	//double lumi = 19538.85;// for SingleMu2012
	//double lumi = 19531.85;// for singleEle2012
	double lumi=1;

	//system("rm -rf SignalEffPlots");   
	system("mkdir SignalEffPlots");
	ofstream outFile("SignalEffPlots/efficiencies_MCSig.txt");
	//outFile<<"Mass	EE1JHP	MM1JHP	EE1JLP	MM1JLP	EE2J	MM2J"<<endl;
	/*
	   dataSamples.push_back("SingleMu_Run2012A_13Jul2012_xww");
	   dataSamples.push_back("SingleMu_Run2012A_recover_xww");
	   dataSamples.push_back("SingleMu_Run2012B_13Jul2012_xww");
	   dataSamples.push_back("SingleMu_Run2012C_24Aug2012_xww");
	   dataSamples.push_back("SingleMu_Run2012C_PromptReco_xww");
	   dataSamples.push_back("SingleMu_Run2012D_PromptReco_xww");
	 */
	if(isZZChannel)
	{
		bkgSamples.push_back("TTBAR");  
		bkgSamples.push_back("WW");
		bkgSamples.push_back("WZ");
		bkgSamples.push_back("ZZ");
		bkgSamples.push_back("DYJetsPt50To70");
		bkgSamples.push_back("DYJetsPt70To100");
		bkgSamples.push_back("DYJetsPt100");

		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M600");
		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M700");
		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M800");
		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M900");
		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M1000");
		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M1100");
		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M1200");
		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M1300");
		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M1400");
		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M1500");
		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M1600");
		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M1700");
		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M1800");
		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M1900");
		sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M2000");
	}

	if(!isZZChannel)
	{
		//bkgSamples.push_back("TTBARpowheg_xww");
		//     bkgSamples.push_back("SingleTopBarTWchannel_xww");
		//     bkgSamples.push_back("SingleTopTWchannel_xww");
		//     bkgSamples.push_back("SingleTopBarSchannel_xww" );
		//     bkgSamples.push_back("SingleTopSchannel_xww");
		//     bkgSamples.push_back("SingleTopBarTchannel_xww");
		//     bkgSamples.push_back("SingleTopTchannel_xww");                
		//     bkgSamples.push_back("WW_xww");
		//     bkgSamples.push_back("WZ_xww");
		//     bkgSamples.push_back("ZZ_xww");
		//     bkgSamples.push_back("WJetsPt50To70_xww");
		//     bkgSamples.push_back("WJetsPt70To100_xww");
		//bkgSamples.push_back("WJetsPt100_xww");
		//     bkgSamples.push_back("DYJetsPt50To70_xww");
		//     bkgSamples.push_back("DYJetsPt70To100_xww");
		//     bkgSamples.push_back("DYJetsPt100_xww");

		//sigSamples.push_back("RSG_WW_lvjj_c0p2_M600_xww");
		//sigSamples.push_back("RSG_WW_lvjj_c0p2_M1000_xww");
		//sigSamples.push_back("RSG_WW_lvjj_c0p2_M1500_xww"); 
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M600_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M700_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M800_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M900_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M1000_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M1100_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M1200_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M1300_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M1400_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M1500_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M1600_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M1700_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M1800_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M1900_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M2000_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M2100_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M2200_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M2300_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M2400_xww");
		sigSamples.push_back("BulkG_WW_lvjj_c0p2_M2500_xww");
	}
	const int nCat=6;//EE1JHP, MM1JHP, EE1JLP, MM1JLP, EE2J, MM2J
	const int ndata = dataSamples.size();
	const int nbkg  = bkgSamples.size();
	const int nsig  = sigSamples.size();

	double SigEff[nsig][nCat];
	int isig=0;
	int iCat=0;

	//order of categories:EE1JHP, MM1JHP, EE1JLP, MM1JLP, EE2J, MM2J
	for(int iNJ=1;iNJ<=2;iNJ++){
		double nxjCut=double(iNJ);

		for(int iPur=1;iPur>=0;iPur--){
			if(iNJ==2&&iPur==0)break;//for 2 jet only do the first loop
			for(int ilep=0;ilep<2;ilep++){
				double lepCut=double(ilep);


				std::vector<double> sig_effs, sig_masses;

				cout<<"data samples "<<ndata<<"  bkg samples "<<nbkg<<"  signal samples "<<nsig<<endl;	

				TH1F * h1 = new TH1F (cut,cut,ndata+nbkg+nsig,0,ndata+nbkg+nsig);
				TH1F * h1b = new TH1F ("Efficiency","Efficiency",17,550,2650);
				//h1->GetYaxis()->SetRangeUser(0,1);
				h1->SetBit(TH1::kCanRebin);
				h1->SetStats(0);
				gStyle->SetPaintTextFormat("1.4f");
				//h1->Sumw2();

				TCanvas * c1 = new TCanvas();	

				int indM=0;
				double startM=600.0; double step=100.0;
				isig=0;
				for(int i =0; i<ndata+nbkg+nsig; i++   )
				{
					TString filename;
					TString samplename;
					if(i<ndata) 
					{
						filename = "treeEDBR_"+dataSamples.at(i)+".root";
						samplename = dataSamples.at(i);//SingleMu_Run2012A_13Jul2012_xww
						if(!isZZChannel)samplename.Remove(samplename.Length()-4,4);//SingleMu_Run2012A_13Jul2012
						samplename.Remove(0,12);//2012A_13Jul2012
					}
					else if(i>=ndata&&i<(ndata+nbkg)) 
					{
						filename = "treeEDBR_"+bkgSamples.at(i-ndata)+".root";
						samplename = bkgSamples.at(i-ndata);//TTBAR_xww
						if(!isZZChannel)samplename.Remove(samplename.Length()-4,4);//TTBAR
					}
					else if(i>=(ndata+nbkg)) 
					{
						filename = "treeEDBR_"+sigSamples.at(i-ndata-nbkg)+".root";
						samplename = sigSamples.at(i-ndata-nbkg);//RSG_WW_lvjj_c0p2_M1500_xww,BulkG_WW_lvjj_c1p0_M1500_xww
						if(!isZZChannel)samplename.Remove(samplename.Length()-4,4);//RSG_WW_lvjj_c0p2_M1500,BulkG_WW_lvjj_c1p0_M1500
						if(samplename.Contains("RSG"))samplename.Remove(4,8);//RSG_c0p2_M1500
						if(samplename.Contains("BulkG"))samplename.Remove(6,8);//BulkG_c1p0_M1500
					}
					cout<<"\n\n----------------------"<<endl;
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
					double philep1[99];
					double etalep1[99];
					double philep2[99];
					double etajet1[99];
					double phijet1[99];

					double ptZll[99];
					double phiZll[99];
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
					double nsubj21[99];
					//cuts for electron, ww 
					double eleid_passConversionVeto[99];
					double eleid_numberOfLostHits[99];


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
					tree->SetBranchAddress("philep1", philep1);
					tree->SetBranchAddress("philep2", philep2);
					tree->SetBranchAddress("phijet1", phijet1);
					tree->SetBranchAddress("etajet1", etajet1);
					tree->SetBranchAddress("ptZll", ptZll);
					tree->SetBranchAddress("phiZll", phiZll);
					tree->SetBranchAddress("ptZjj", ptZjj);
					tree->SetBranchAddress("met", &met);
					tree->SetBranchAddress("nXjets", nXjets);
					tree->SetBranchAddress("lep", lep);
					tree->SetBranchAddress("nCands", &nCands);

					tree->SetBranchAddress("PUweight", &PUweight);
					tree->SetBranchAddress("LumiWeight", &LumiWeight);
					tree->SetBranchAddress("GenWeight", &GenWeight);
					tree->SetBranchAddress("region", region);
					tree->SetBranchAddress("nsubj21", nsubj21);

					tree->SetBranchAddress("eleid_passConversionVeto", eleid_passConversionVeto);
					tree->SetBranchAddress("eleid_numberOfLostHits", eleid_numberOfLostHits);

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

							double deltaR_LJ = deltaR(etalep1[ivec],philep1[ivec],etajet1[ivec],phijet1[ivec]);
							double deltaPhi_JMET = deltaPhi(phijet1[ivec],philep2[ivec]);
							double deltaPhi_JWL  = deltaPhi(phijet1[ivec],phiZll[ivec]);

							if(filled==0)//cout the event as pass if any candidate pass all the cuts
							{
								//make cuts
								if(lep[ivec]!=lepCut)continue;// 1 is for mu
								if(region[ivec]!=1)continue;// 1 is mjj signal region
								if(nXjets[ivec]!=nxjCut)continue;// 1 jet candidate
								//for nsubjtiness
								if(iPur==1&&nsubj21[ivec]<0.45);
								else if(iPur==0&&(nsubj21[ivec]>0.45&&nsubj21[ivec]<0.75));
								else if(nxjCut==2);
								else continue;
								////
								if(!isZZChannel)
								{
									if((nLooseEle+nLooseMu)!=1)continue;
									if(ptZll[ivec]<200)continue;
									if(nbtagsM[ivec]!=0)continue; //b-tag veto
									if(deltaR_LJ<1.57 || deltaPhi_JMET<2. || deltaPhi_JWL<2.)continue;
									if(lepCut==0)
									{
										if(met<80)continue;//for electron, use this cut
										if(eleid_passConversionVeto[ivec]!=1)continue;
										if(eleid_numberOfLostHits[ivec]!=0)continue;
									}
								}
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

					if(samplename.Contains("BulkG")){
						int binToFill=h1b->FindBin(indM*step+startM);
						cout<<"Filling IndM="<<indM<<"  M="<<indM*step+startM<<"  Bin#"<<binToFill<<"  Eff="<<eff<<endl;
						h1b->SetBinContent(binToFill,eff);
						sig_masses.push_back(indM*step+startM);
						sig_effs.push_back(eff);
						indM++;
					}
					SigEff[isig][iCat]=eff;
					isig++;
					if(isig==nsig)//finished last sample
						iCat++;
				}//end of sample loop

				c1->SetGridy(1);

				/// Here you choose the functional form of the efficiency.
				/// You have to be careful with the initial values of the parameters...
				TF1 *fitFunc = new TF1("fitPoly2","tanh([0]*(x-[1]))*([2]+[3]*x)",580,2550);
				fitFunc->SetLineColor(kRed);
				fitFunc->SetLineWidth(2);
				fitFunc->SetParameter(0,-0.002);
				fitFunc->SetParameter(1,300);
				fitFunc->SetParameter(2,-0.3);
				fitFunc->SetParameter(3,3E-5);
				fitFunc->SetLineColor(kRed);
				h1b->Fit(fitFunc,"R");

				cout<<"\n\nEff extrapolated at 2000 GeV: "<<fitFunc->Eval(2000.0)<<endl;

				TString lepStr;
				if(lepCut==1)lepStr="MU";
				if(lepCut==0)lepStr="ELE";
				TString PurStr;
				if(iPur==1)PurStr="HP";
				if(iPur==0)PurStr="LP";
				if(nxjCut==2)PurStr="";

				TString SaveName="_"+lepStr+"_"+PurStr+"_";
				SaveName+=iNJ;
				SaveName+="J";
				//outFile<<SaveName<<endl;

				/// The original plot with efficiencies for both signals and backgrounds
				h1->SetTitle(cut);
				h1->Draw();
				h1->Draw("TEXT0same");
				c1->SaveAs("SignalEffPlots/"+cut+SaveName+".png");

				/// The plot with signal efficiency and the fit.
				TCanvas *c2=new TCanvas("cFit","cFit",800,800);
				c2->cd();
				h1b->SetMarkerStyle(20);
				h1b->Draw("P");
				fitFunc->Draw("Lsame");
				c2->SaveAs("SignalEffPlots/efficiencyAndFit"+SaveName+".png");

				cout<<"data samples "<<ndata<<"  bkg samples "<<nbkg<<"  signal samples "<<nsig<<endl;	
				cout<<"\nEfficiencies for Lep="<<lepCut<<"  nXjets="<<nxjCut<<" : "<<endl;
				for(unsigned int im=0;im<sig_effs.size();im++){
					cout<<"M="<<sig_masses.at(im)<<"  Eff="<<sig_effs.at(im)<<endl;
				}
			}//end of lepCut loop
		}//end of purity loop
	}//end of nxjCut loop

	for(int is =0; is<nsig; is++)
	{
		double MAss = 600+100*is;
		outFile<<MAss;
		for(int ic=0;ic<nCat; ic++)
		{
			outFile<<"		"<<SigEff[is][ic];
		}
		outFile<<endl;
	}
}
