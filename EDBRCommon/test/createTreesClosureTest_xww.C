#include "TString.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"
#include <vector>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
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



//int main()
int createTreesClosureTest_xww()
{
	//########EDIT THIS PART###########
	TString inTree="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_eleid/fullallrange";
	TString outSigTree="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_eleid/AnaSigTree_elemet80";
	TString outSBTree="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_eleid/AnaSBTree_elemet80";

	const double A1Low=50.0;
	const double A1High=60.0;

	const double A2Low=60.0;
	const double A2High=70.0;

	const double BLow=100;
	const double BHigh=130;

	//#########################


	system("mkdir -p "+outSigTree);
	system("mkdir -p "+outSBTree);

	//make file list
	FILE *fp;
	TString command = "ls "+inTree+" | grep root";
	fp = popen(command,"r");
	char tempname[255];
	vector<TString> filelist;
	while(!feof(fp))
	{  
		fscanf(fp,"%s\n",tempname);
		//cout<<tempname<<endl;
		TString temp = tempname;
		filelist.push_back(temp);
	}  
	/////////end of making list//////////
	for(unsigned int fi=0; fi <filelist.size(); fi++)
	{
		TString file = filelist.at(fi);
		cout<<"Processing "<<file<<endl;
		TFile *fIn=new TFile(inTree+"/"+file,"READ");
		TTree *tIn=(TTree*)fIn->Get("SelectedCandidates");

		//common
		int    nCands;
		unsigned int nevent, run;
		//btags
		double nbtagsL[99];
		double nbtagsM[99];
		double nbtagsT[99];
		double nbtagscleanL[99];
		double nbtagscleanM[99];
		double nbtagscleanT[99];
		//to make cuts
		double ptlep1[99];
		double philep1[99];
		double etalep1[99];
		double met;
		double philep2[99];
		double etajet1[99];
		double phijet1[99];
		double ptZll[99];
		double phiZll[99];
		double ptZjj[99];
		double lep[99];
		int    nXjets[99];
		int nLooseEle;
		int nLooseMu;
		double region[99];
		double nsubj21[99];
		//per event weight
		double PUweight=-1;
		double LumiWeight=-1;
		double GenWeight=-1;
		//for ele 
		double eleid_passConversionVeto[99];
		double eleid_numberOfLostHits[99];
		//fit variables
		double mZZ[99],mJJ[99],mLL[99],prMJ[99],mJJNoKinFit[99];		

		tIn->SetBranchAddress("nCands", &nCands);
		tIn->SetBranchAddress("event", &nevent);
		tIn->SetBranchAddress("run", &run);
		tIn->SetBranchAddress("nLooseMu", &nLooseMu);
		tIn->SetBranchAddress("nLooseEle",&nLooseEle);
		tIn->SetBranchAddress("nbtagsL",nbtagsL);
		tIn->SetBranchAddress("nbtagsM",nbtagsM);
		tIn->SetBranchAddress("nbtagsT",nbtagsT);
		tIn->SetBranchAddress("nbtagscleanL",nbtagscleanL);
		tIn->SetBranchAddress("nbtagscleanM",nbtagscleanM);
		tIn->SetBranchAddress("nbtagscleanT",nbtagscleanT);
		tIn->SetBranchAddress("ptlep1", ptlep1);
		tIn->SetBranchAddress("etalep1", etalep1);
		tIn->SetBranchAddress("philep1", philep1);
		tIn->SetBranchAddress("philep2", philep2);
		tIn->SetBranchAddress("phijet1", phijet1);
		tIn->SetBranchAddress("etajet1", etajet1);
		tIn->SetBranchAddress("ptZll", ptZll);
		tIn->SetBranchAddress("phiZll", phiZll);
		tIn->SetBranchAddress("ptZjj", ptZjj);
		tIn->SetBranchAddress("met", &met);
		tIn->SetBranchAddress("nXjets", nXjets);
		tIn->SetBranchAddress("lep", lep);
		tIn->SetBranchAddress("region", region);
		tIn->SetBranchAddress("nsubj21", nsubj21);
		tIn->SetBranchAddress("PUweight", &PUweight);
		tIn->SetBranchAddress("LumiWeight", &LumiWeight);
		tIn->SetBranchAddress("GenWeight", &GenWeight);
		tIn->SetBranchAddress("mZZ", mZZ);
		tIn->SetBranchAddress("mJJ", mJJ);
		tIn->SetBranchAddress("mLL", mLL);
		tIn->SetBranchAddress("prunedmass", prMJ);
		tIn->SetBranchAddress("mJJNoKinFit", mJJNoKinFit);
		tIn->SetBranchAddress("eleid_passConversionVeto", eleid_passConversionVeto);
		tIn->SetBranchAddress("eleid_numberOfLostHits", eleid_numberOfLostHits);

		//set branch for out tree
		TFile *fOutSig=new TFile(outSigTree+"/"+file,"RECREATE");
		TFile *fOutSB=new TFile(outSBTree+"/"+file,"RECREATE");
		fOutSig->cd();
		TTree *tAnaSig=new TTree("SelectedCandidates","SelectedCandidates");

		double regionAnaSig[99],regionA1A2Sig[99],regionABSig[99];
		double regionAnaSB[99],regionA1A2SB[99],regionABSB[99];
		int    nCandsAna;
		unsigned int neventAnaSig, runAna;
		double lepAna[99],mZZAna[99],mJJAna[99];
		double mLLAna[99],nsubj21Ana[99],prMJAna[99],mJJNoKinFitAnaSig[99];
		double  vTagPurityAna[99];
		int nXjetsAna[99];
		double PUweightAnaSig, LumiWeightAnaSig,GenWeightAnaSig,weightAnaSig;
		tAnaSig->Branch("nCands" ,         &nCandsAna ,      "nCands/I");
		tAnaSig->Branch("event"           ,&neventAnaSig        ,"event/i");
		tAnaSig->Branch("run"             ,&runAna           ,"run/i");
		tAnaSig->Branch("lep"             ,&lepAna           ,"lep[nCands]/D"   );
		tAnaSig->Branch("nXjets"          ,&nXjetsAna        ,"nXjets[nCands]/I");
		tAnaSig->Branch("vTagPurity"          ,&vTagPurityAna        ,"vTagPurity[nCands]/D");
		tAnaSig->Branch("mZZ"             ,&mZZAna           ,"mZZ[nCands]/D"   );
		tAnaSig->Branch("mLL"             ,&mLLAna           ,"mLL[nCands]/D"  );
		tAnaSig->Branch("mJJ"             ,&mJJAna           ,"mJJ[nCands]/D");
		tAnaSig->Branch("mJJNoKinFit"             ,&mJJNoKinFitAnaSig           ,"mJJNoKinFit[nCands]/D");
		tAnaSig->Branch("prunedmass"      , &prMJAna, "prunedmass[nCands]/D");
		tAnaSig->Branch("nsubj21"       ,&nsubj21Ana    ,"nsubj21[nCands]/D");
		tAnaSig->Branch("PUweight"        ,&PUweightAnaSig            ,"PUweight/D" );
		tAnaSig->Branch("LumiWeight"      ,&LumiWeightAnaSig         ,"LumiWeight/D"  );
		tAnaSig->Branch("GenWeight"      ,&GenWeightAnaSig         ,"GenWeight/D"  );
		tAnaSig->Branch("weight"          ,&weightAnaSig             ,"weight/D");


		TTree *tA1A2Sig = tAnaSig->CloneTree(0);				
		tA1A2Sig->SetName("SelectedCandidatesA1A2");
		TTree *tABSig = tAnaSig->CloneTree(0);			
		tABSig->SetName("SelectedCandidatesAB");

		fOutSB->cd();
		TTree *tAnaSB = tAnaSig->CloneTree(0);
		TTree *tA1A2SB = tA1A2Sig->CloneTree(0);
		TTree *tABSB = tABSig->CloneTree(0);	
		tAnaSB->Branch("region"             ,&regionAnaSB           ,"region[nCands]/D"   );
		tA1A2SB->Branch("region"             ,&regionA1A2SB           ,"region[nCands]/D"   );
		tABSB->Branch("region"             ,&regionABSB           ,"region[nCands]/D"   );
		tAnaSig->Branch("region"             ,&regionAnaSig           ,"region[nCands]/D"   );  
		tA1A2Sig->Branch("region"             ,&regionA1A2Sig           ,"region[nCands]/D"   );  
		tABSig->Branch("region"             ,&regionABSig           ,"region[nCands]/D"   );

		for(int i=0;i<tIn->GetEntries();i++){
			tIn->GetEntry(i);

			//set variables for new trees
			nCandsAna=nCands;
			neventAnaSig=nevent;
			runAna=run;
			PUweightAnaSig=PUweight;
			LumiWeightAnaSig=LumiWeight;
			GenWeightAnaSig=GenWeight;
			if(file.Contains("WJetsPt"))GenWeightAnaSig=GenWeightAnaSig*1.3;//for wjets, add addtionnal 1.3 factor
			weightAnaSig=PUweight*LumiWeight*GenWeightAnaSig;

			bool goodevent=false;
			for(int ivec =0; ivec<nCands; ivec++)
			{
				//make selections
				double deltaR_LJ = deltaR(etalep1[ivec],philep1[ivec],etajet1[ivec],phijet1[ivec]);
				double deltaPhi_JMET = deltaPhi(phijet1[ivec],philep2[ivec]);
				double deltaPhi_JWL  = deltaPhi(phijet1[ivec],phiZll[ivec]);

				if( nLooseEle+nLooseMu==1 );//loose lepton veto selection
				else continue;

				if(ptZll[ivec]>200);//cut on WL pt
				else continue;

				//cut from fermilab
				if(deltaR_LJ>1.57 && deltaPhi_JMET>2. && deltaPhi_JWL>2.);
				else continue;

				//b tag veto
				if(nbtagsM[ivec]==0);
				else continue;

				if(lep[ivec]==0){//cut on met in electron channel
					if(met>80); 
					else continue;
					//conversion veto
					if(eleid_passConversionVeto[ivec]==1);
					else continue;
					if(eleid_numberOfLostHits[ivec]==0);
					else continue;
				}


				goodevent =true;


				if(nsubj21[ivec]<0.45) vTagPurityAna[ivec]=1;
				else if(nsubj21[ivec]<0.75) vTagPurityAna[ivec]=0;
				else  vTagPurityAna[ivec]=-1;

				lepAna[ivec]=lep[ivec];
				nXjetsAna[ivec]=nXjets[ivec];
				mZZAna[ivec]=mZZ[ivec];
				mLLAna[ivec]=mLL[ivec];
				mJJAna[ivec]=mJJ[ivec];
				prMJAna[ivec]=prMJ[ivec];
				nsubj21Ana[ivec]=nsubj21[ivec];	
				mJJNoKinFitAnaSig[ivec]=mJJNoKinFit[ivec];
				//signal and sideband as usual
				if(region[ivec]==0)regionAnaSB[ivec]=0;else regionAnaSB[ivec]=-1;
				if(region[ivec]==1)regionAnaSig[ivec]=1;else regionAnaSig[ivec]=-1;
				// sideband A1, signal A2
				if(mJJNoKinFit[ivec]<A1High&&mJJNoKinFit[ivec]>A1Low)regionA1A2SB[ivec]=0;  else regionA1A2SB[ivec]=-1;
				if(mJJNoKinFit[ivec]<A2High&&mJJNoKinFit[ivec]>A2Low)regionA1A2Sig[ivec]=1; else regionA1A2Sig[ivec]=-1;
				// sideband A, signal B
				if(mJJNoKinFit[ivec]<A2High&&mJJNoKinFit[ivec]>A1Low)regionABSB[ivec]=0;  else regionABSB[ivec]=-1;
				if(mJJNoKinFit[ivec]<BHigh &&mJJNoKinFit[ivec]>BLow )regionABSig[ivec]=1;  else regionABSig[ivec]=-1;
			}			

			if(goodevent)
			{
				tAnaSig->Fill();//signal and sideband as usual
				tAnaSB->Fill();//signal and sideband as usual
				tA1A2Sig->Fill();// sideband A1, signal A2
				tA1A2SB->Fill();// sideband A1, signal A2
				tABSig->Fill();// sideband A, signal B
				tABSB->Fill();// sideband A, signal B

			}
		}
		fIn->Close();
		delete fIn;

		//save the trees
		fOutSig->cd();
		tAnaSig->Write();
		tA1A2Sig->Write();
		tABSig->Write();
		fOutSig->Close();
		delete fOutSig;


		fOutSB->cd();
		tAnaSB->Write();
		tA1A2SB->Write();
		tABSB->Write();
		fOutSB->Close();
		delete fOutSB;

	}//endl of loop over samples

	return 0;
}












