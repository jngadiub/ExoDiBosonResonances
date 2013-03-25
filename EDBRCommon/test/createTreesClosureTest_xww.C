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
	TString outAnaTree="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_eleid/AnaTree";

	const double A1Low=50.0;
	const double A1High=60.0;

	const double A2Low=60.0;
	const double A2High=70.0;

	const double BLow=100;
	const double BHigh=130;

	//#########################


	system("mkdir -p "+outAnaTree);

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

		//set branch for out tree
		TFile *fOut=new TFile(outAnaTree+"/"+file,"RECREATE");
		TTree *tAna=new TTree("SelectedCandidates","SelectedCandidates");

		double regionAna[99],regionA1A2[99],regionAB[99];
		int    nCandsAna;
		unsigned int neventAna, runAna;
		double lepAna[99],mZZAna[99],mJJAna[99];
		double mLLAna[99],nsubj21Ana[99],prMJAna[99],mJJNoKinFitAna[99];
		double  vTagPurityAna[99];
		int nXjetsAna[99];
		double PUweightAna, LumiWeightAna,GenWeightAna,weightAna;
		tAna->Branch("nCands" ,         &nCandsAna ,      "nCands/I");
		tAna->Branch("event"           ,&neventAna        ,"event/i");
		tAna->Branch("run"             ,&runAna           ,"run/i");
		tAna->Branch("lep"             ,&lepAna           ,"lep[nCands]/D"   );
		tAna->Branch("nXjets"          ,&nXjetsAna        ,"nXjets[nCands]/I");
		tAna->Branch("vTagPurity"          ,&vTagPurityAna        ,"vTagPurity[nCands]/D");
		tAna->Branch("mZZ"             ,&mZZAna           ,"mZZ[nCands]/D"   );
		tAna->Branch("mLL"             ,&mLLAna           ,"mLL[nCands]/D"  );
		tAna->Branch("mJJ"             ,&mJJAna           ,"mJJ[nCands]/D");
		tAna->Branch("mJJNoKinFit"             ,&mJJNoKinFitAna           ,"mJJNoKinFit[nCands]/D");
		tAna->Branch("prunedmass"      , &prMJAna, "prunedmass[nCands]/D");
		tAna->Branch("nsubj21"       ,&nsubj21Ana    ,"nsubj21[nCands]/D");
		tAna->Branch("PUweight"        ,&PUweightAna            ,"PUweight/D" );
		tAna->Branch("LumiWeight"      ,&LumiWeightAna         ,"LumiWeight/D"  );
		tAna->Branch("GenWeight"      ,&GenWeightAna         ,"GenWeight/D"  );
		tAna->Branch("weight"          ,&weightAna             ,"weight/D");


		TTree *tA1A2 = tAna->CloneTree(0);				
		tA1A2->SetName("SelectedCandidatesA1A2");
		TTree *tAB = tAna->CloneTree(0);			
		tAB->SetName("SelectedCandidatesAB");	
		tAna->Branch("region"             ,&regionAna           ,"region[nCands]/D"   );
		tA1A2->Branch("region"             ,&regionA1A2           ,"region[nCands]/D"   );
		tAB->Branch("region"             ,&regionAB           ,"region[nCands]/D"   );

		for(int i=0;i<tIn->GetEntries();i++){
			tIn->GetEntry(i);

			//set variables for new trees
			nCandsAna=nCands;
			neventAna=nevent;
			runAna=run;
			PUweightAna=PUweight;
			LumiWeightAna=LumiWeight;
			GenWeightAna=GenWeight;
			weightAna=PUweight*LumiWeight*GenWeight;

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
					if(met>100); 
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
				mJJNoKinFitAna[ivec]=mJJNoKinFit[ivec];
				//signal and sideband as usual
				regionAna[ivec]=region[ivec];
				// sideband A1, signal A2
				if(mJJNoKinFit[ivec]<A1High&&mJJNoKinFit[ivec]>A1Low)regionA1A2[ivec]=0;
				else if(mJJNoKinFit[ivec]<A2High&&mJJNoKinFit[ivec]>A2Low)regionA1A2[ivec]=1;
				else regionA1A2[ivec]=-1;
				// sideband A, signal B
				if(mJJNoKinFit[ivec]<A2High&&mJJNoKinFit[ivec]>A1Low)regionAB[ivec]=0;
				else if(mJJNoKinFit[ivec]<BHigh &&mJJNoKinFit[ivec]>BLow )regionAB[ivec]=1;
				else regionAB[ivec]=-1;
			}			
			
			if(goodevent)
			{
				tAna->Fill();//signal and sideband as usual
				tA1A2->Fill();// sideband A1, signal A2
				tAB->Fill();// sideband A, signal B
			}
		}

		//save the trees
		fOut->cd();
		tAna->Write();
		tA1A2->Write();
		tAB->Write();

		//delte objects
		delete tAna;
		delete tA1A2;
		delete tAB;
		fOut->Close();
		delete fOut;
		delete tIn;
		fIn->Close();
		delete fIn;

	}//endl of loop over samples

	return 0;
}












