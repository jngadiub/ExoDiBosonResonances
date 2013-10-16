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
	TString InTreeLocation="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv12/";

	int treetype=1; //1 for fullrange, 0 for ttbarcontrol
	
	const double A1Low=40.0;
	const double A1High=55.0;

	const double A2Low=55.0;
	const double A2High=65.0;

	const double BLow=100;
	const double BHigh=115;

	const double SigLow=65;
	const double SigHigh=105;


	//#########################
	TString inTree, outSigTree , outSBTree;	
	
	if(treetype==1)
	{
		inTree=InTreeLocation+"/fullrange/";
		outSigTree=InTreeLocation+"/AnaSigTree/";
		outSBTree=InTreeLocation+"/AnaSBTree/";
	}
    if(treetype==0)    
	{   inTree=InTreeLocation+"/ttbarcontrol/";
        outSigTree=InTreeLocation+"/AnaSigTree_forTT/";
        outSBTree=InTreeLocation+"/AnaSBTree_forTT/";
    }


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
		double mt[99];
		int    nXjets[99];
		int nLooseEle;
		int nLooseMu;
		double region[99];
		double nsubj21[99];
		//per event weight
		double HLTweight=-1;
		double PUweight=-1;
		double LumiWeight=-1;
		double GenWeight=-1;
		double BTagWeight=-1;
		double VTagWeight=-1;
		//for ele 
		double eleid_passConversionVeto[99];
		double eleid_numberOfLostHits[99];
		//fit variables
		double mZZ[99],mJJ[99],mLL[99],prMJ[99],mJJNoKinFit[99];		
		//mc match
		double MCmatch[99];


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
		tIn->SetBranchAddress("mt", mt);
		tIn->SetBranchAddress("region", region);
		tIn->SetBranchAddress("nsubj21", nsubj21);
		tIn->SetBranchAddress("HLTweight", &HLTweight);
		tIn->SetBranchAddress("PUweight", &PUweight);
		tIn->SetBranchAddress("LumiWeight", &LumiWeight);
		tIn->SetBranchAddress("GenWeight", &GenWeight);
		tIn->SetBranchAddress("BTagWeight", &BTagWeight);
		tIn->SetBranchAddress("VTagWeight", &VTagWeight);
		tIn->SetBranchAddress("mZZ_type2_ptUncorrected", mZZ);
		tIn->SetBranchAddress("mJJ", mJJ);
		tIn->SetBranchAddress("mLL", mLL);
		tIn->SetBranchAddress("prunedmass", prMJ);
		tIn->SetBranchAddress("mJJNoKinFit", mJJNoKinFit);
		tIn->SetBranchAddress("eleid_passConversionVeto", eleid_passConversionVeto);
		tIn->SetBranchAddress("eleid_numberOfLostHits", eleid_numberOfLostHits);
		tIn->SetBranchAddress("MCmatch",MCmatch);

		//set branch for out tree
		TFile *fOutSig=new TFile(outSigTree+"/"+file,"RECREATE");
		TFile *fOutSB=new TFile(outSBTree+"/"+file,"RECREATE");
		fOutSig->cd();
		TTree *tAnaSig=new TTree("SelectedCandidates","SelectedCandidates");
		TTree *tAnaSigPlain=new TTree("SelectedCandidatesPlain","SelectedCandidatesPlain");

		double regionAnaSig[99],regionA1A2Sig[99],regionABSig[99]; 
		double regionAnaSB[99],regionA1A2SB[99],regionABSB[99];    
		int    nCandsAna;
		unsigned int neventAnaSig, runAna;
		double lepAna[99],mZZAna[99],mJJAna[99];
		double mLLAna[99],nsubj21Ana[99],prMJAna[99],mJJNoKinFitAnaSig[99];
		double  vTagPurityAna[99];
		int nXjetsAna[99];
		double HLTweightAnaSig, PUweightAnaSig, LumiWeightAnaSig,GenWeightAnaSig,BTagWeightAnaSig,VTagWeightAnaSig,weightAnaSig;
		int categories[99];
		double MCmatchAna[99];
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
		tAnaSig->Branch("HLTweight"        ,&HLTweightAnaSig            ,"HLTweight/D" );
		tAnaSig->Branch("PUweight"        ,&PUweightAnaSig            ,"PUweight/D" );
		tAnaSig->Branch("LumiWeight"      ,&LumiWeightAnaSig         ,"LumiWeight/D"  );
		tAnaSig->Branch("GenWeight"      ,&GenWeightAnaSig         ,"GenWeight/D"  );
		tAnaSig->Branch("BTagWeight"      ,&BTagWeightAnaSig         ,"BTagWeight/D"  );
		tAnaSig->Branch("VTagWeight"      ,&VTagWeightAnaSig         ,"VTagWeight/D"  );
		tAnaSig->Branch("weight"          ,&weightAnaSig             ,"weight/D");
		tAnaSig->Branch("categories"          ,&categories             ,"categories[nCands]/I");
		tAnaSig->Branch("MCmatch"          ,&MCmatchAna             ,"MCmatch[nCands]/D");

		double regionAnaSigPlain,regionA1A2SigPlain,regionABSigPlain;
		double regionAnaSBPlain,regionA1A2SBPlain,regionABSBPlain;
		int nCandsAnaPlain;
		unsigned int neventAnaSigPlain, runAnaPlain;
		double lepAnaPlain,mZZAnaPlain,mJJAnaPlain;
		double mLLAnaPlain,nsubj21AnaPlain,prMJAnaPlain,mJJNoKinFitAnaSigPlain;
		double  vTagPurityAnaPlain;
		int nXjetsAnaPlain;
		double HLTweightAnaSigPlain, PUweightAnaSigPlain, LumiWeightAnaSigPlain,GenWeightAnaSigPlain,BTagWeightAnaSigPlain, VTagWeightAnaSigPlain, weightAnaSigPlain;
		int categoriesPlain;
		double MCmatchAnaPlain;
		tAnaSigPlain->Branch("nCands" ,         &nCandsAnaPlain ,      "nCands/I");
		tAnaSigPlain->Branch("event"           ,&neventAnaSigPlain        ,"event/i");
		tAnaSigPlain->Branch("run"             ,&runAnaPlain           ,"run/i");
		tAnaSigPlain->Branch("lep"             ,&lepAnaPlain           ,"lep/D"   );  
		tAnaSigPlain->Branch("nXjets"          ,&nXjetsAnaPlain        ,"nXjets/I");
		tAnaSigPlain->Branch("vTagPurity"          ,&vTagPurityAnaPlain        ,"vTagPurity/D");
		tAnaSigPlain->Branch("mZZ"             ,&mZZAnaPlain           ,"mZZ/D"   );  
		tAnaSigPlain->Branch("mLL"             ,&mLLAnaPlain           ,"mLL/D"  );  
		tAnaSigPlain->Branch("mJJ"             ,&mJJAnaPlain           ,"mJJ/D");
		tAnaSigPlain->Branch("mJJNoKinFit"             ,&mJJNoKinFitAnaSigPlain           ,"mJJNoKinFit/D");
		tAnaSigPlain->Branch("prunedmass"      , &prMJAnaPlain, "prunedmass/D");
		tAnaSigPlain->Branch("nsubj21"       ,&nsubj21AnaPlain    ,"nsubj21/D");
		tAnaSigPlain->Branch("HLTweight"        ,&HLTweightAnaSigPlain            ,"HLTweight/D" );
		tAnaSigPlain->Branch("PUweight"        ,&PUweightAnaSigPlain            ,"PUweight/D" );
		tAnaSigPlain->Branch("LumiWeight"      ,&LumiWeightAnaSigPlain         ,"LumiWeight/D"  );  
		tAnaSigPlain->Branch("GenWeight"      ,&GenWeightAnaSigPlain         ,"GenWeight/D"  );  
		tAnaSigPlain->Branch("BTagWeight"      ,&BTagWeightAnaSigPlain         ,"BTagWeight/D"  );  
		tAnaSigPlain->Branch("VTagWeight"      ,&VTagWeightAnaSigPlain         ,"VTagWeight/D"  );
		tAnaSigPlain->Branch("weight"          ,&weightAnaSigPlain             ,"weight/D");
		tAnaSigPlain->Branch("categories"          ,&categoriesPlain             ,"categories/I");		
		tAnaSigPlain->Branch("MCmatch"          ,&MCmatchAnaPlain             ,"MCmatch/D");

		TTree *tA1A2Sig = tAnaSig->CloneTree(0);				
		tA1A2Sig->SetName("SelectedCandidatesA1A2");
		TTree *tABSig = tAnaSig->CloneTree(0);			
		tABSig->SetName("SelectedCandidatesAB");

        TTree *tA1A2SigPlain = tAnaSigPlain->CloneTree(0);    
        tA1A2SigPlain->SetName("SelectedCandidatesA1A2Plain");
        TTree *tABSigPlain = tAnaSigPlain->CloneTree(0);    
        tABSigPlain->SetName("SelectedCandidatesABPlain");		
		
		fOutSB->cd();
		TTree *tAnaSB = tAnaSig->CloneTree(0);
		TTree *tA1A2SB = tA1A2Sig->CloneTree(0);
		TTree *tABSB = tABSig->CloneTree(0);	
		TTree *tAnaSBPlain = tAnaSigPlain->CloneTree(0);
		TTree *tA1A2SBPlain = tA1A2SigPlain->CloneTree(0);
		TTree *tABSBPlain = tABSigPlain->CloneTree(0);	
		tAnaSB->Branch("region"             ,&regionAnaSB           ,"region[nCands]/D"   );
		tA1A2SB->Branch("region"             ,&regionA1A2SB           ,"region[nCands]/D"   );
		tABSB->Branch("region"             ,&regionABSB           ,"region[nCands]/D"   );
		tAnaSBPlain->Branch("region"             ,&regionAnaSBPlain           ,"region/D"   );
		tA1A2SBPlain->Branch("region"             ,&regionA1A2SBPlain           ,"region/D"   );
		tABSBPlain->Branch("region"             ,&regionABSBPlain           ,"region/D"   );
		tAnaSig->Branch("region"             ,&regionAnaSig           ,"region[nCands]/D"   );  
		tA1A2Sig->Branch("region"             ,&regionA1A2Sig           ,"region[nCands]/D"   );  
		tABSig->Branch("region"             ,&regionABSig           ,"region[nCands]/D"   );
		tAnaSigPlain->Branch("region"             ,&regionAnaSigPlain           ,"region/D"   );
		tA1A2SigPlain->Branch("region"             ,&regionA1A2SigPlain           ,"region/D"   );
		tABSigPlain->Branch("region"             ,&regionABSigPlain           ,"region/D"   );


		for(int i=0;i<tIn->GetEntries();i++){
			tIn->GetEntry(i);

			//set variables for new trees
			nCandsAna=nCands;
			neventAnaSig=nevent;
			runAna=run;
			HLTweightAnaSig=HLTweight;
			PUweightAnaSig=PUweight;
			LumiWeightAnaSig=LumiWeight;
			//if(file.Contains("BulkG_WW_lvjj_c0p2_M1600"))LumiWeightAnaSig=1.5771e-05/45994;
			GenWeightAnaSig=GenWeight;
			BTagWeightAnaSig=BTagWeight;
			VTagWeightAnaSig=VTagWeight;
			//if(file.Contains("WJetsPt"))GenWeightAnaSig=GenWeightAnaSig*1.3;//for wjets, add addtionnal 1.3 factor
			weightAnaSig=HLTweightAnaSig*PUweightAnaSig*LumiWeightAnaSig*GenWeightAnaSig*BTagWeightAnaSig*VTagWeightAnaSig;

			bool goodevent=false;
			for(int ivec =0; ivec<nCands; ivec++)
			{
				//make selections
				double deltaR_LJ = deltaR(etalep1[ivec],philep1[ivec],etajet1[ivec],phijet1[ivec]);
				double deltaPhi_JMET = deltaPhi(phijet1[ivec],philep2[ivec]);
				double deltaPhi_JWL  = deltaPhi(phijet1[ivec],phiZll[ivec]);

				if( nLooseEle+nLooseMu==1 );//loose lepton veto selection
				else continue;

				//if(ptZll[ivec]>200);//cut on WL pt
				//else continue;

				//cut from fermilab
				if((deltaR_LJ>1.57 && deltaPhi_JMET>2. && deltaPhi_JWL>2.&&treetype==1)||treetype==0);
				else continue;

				//b tag veto
				if(nbtagsM[ivec]==0&&treetype==1);
				//b control region
				else if(nbtagscleanM[ivec]>=1&&treetype==0);
				else continue;

				//if(ptlep1[ivec]>90&&met>80);//this is a test for muon channel: make the same cuts as electron, ad see closure test A->B
				//else continue;

				if(lep[ivec]==0){//cut on met in electron channel
					if(met>80); 
					else continue;
					//conversion veto
					//if(eleid_passConversionVeto[ivec]==1);
					//else continue;
					//if(eleid_numberOfLostHits[ivec]==0);
					//else continue;
				}


				goodevent =true;


				if(nsubj21[ivec]<0.5) vTagPurityAna[ivec]=1;
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
				MCmatchAna[ivec]=MCmatch[ivec];

				//define 6 categories: 
				if(lepAna[ivec]==0&&vTagPurityAna[ivec]==0&&nXjetsAna[ivec]==1)categories[ivec]=0;//0 ele LP 1J
				else if(lepAna[ivec]==0&&vTagPurityAna[ivec]==1&&nXjetsAna[ivec]==1)categories[ivec]=1;//1 ele HP 1J
				else if(lepAna[ivec]==1&&vTagPurityAna[ivec]==0&&nXjetsAna[ivec]==1)categories[ivec]=2;//2 mu LP 1J
				else if(lepAna[ivec]==1&&vTagPurityAna[ivec]==1&&nXjetsAna[ivec]==1)categories[ivec]=3;//3 mu HP 1J
				else if(lepAna[ivec]==0&&nXjetsAna[ivec]==2)categories[ivec]=4;//4 ele 2J
				else if(lepAna[ivec]==1&&nXjetsAna[ivec]==2)categories[ivec]=5;//5 mu 2J
				else categories[ivec]=-1;

				//signal SigLow SigHigh, sideband A
				//if(  (mJJNoKinFit[ivec]<A2High&&mJJNoKinFit[ivec]>A1Low)  || (mJJNoKinFit[ivec]<BHigh &&mJJNoKinFit[ivec]>BLow)  )regionAnaSB[ivec]=0;  else regionAnaSB[ivec]=-1;
				if(mJJNoKinFit[ivec]<A2High&&mJJNoKinFit[ivec]>A1Low)regionAnaSB[ivec]=0;  else regionAnaSB[ivec]=-1;
				if(mJJNoKinFit[ivec]<SigHigh&&mJJNoKinFit[ivec]>SigLow)regionAnaSig[ivec]=1; else regionAnaSig[ivec]=-1;
				// sideband A1, signal A2
				if(mJJNoKinFit[ivec]<A1High&&mJJNoKinFit[ivec]>A1Low)regionA1A2SB[ivec]=0;  else regionA1A2SB[ivec]=-1;
				if(mJJNoKinFit[ivec]<A2High&&mJJNoKinFit[ivec]>A2Low)regionA1A2Sig[ivec]=1; else regionA1A2Sig[ivec]=-1;
				// sideband A, signal B
				if(mJJNoKinFit[ivec]<A2High&&mJJNoKinFit[ivec]>A1Low)regionABSB[ivec]=0;  else regionABSB[ivec]=-1;
				if(mJJNoKinFit[ivec]<BHigh &&mJJNoKinFit[ivec]>BLow )regionABSig[ivec]=1;  else regionABSig[ivec]=-1;
			}//end of loop over candidates		

			//fill plain tree
			regionAnaSigPlain=regionAnaSig[0];
			regionA1A2SigPlain=regionA1A2Sig[0];
			regionABSigPlain=regionABSig[0];
			regionAnaSBPlain=regionAnaSB[0];
			regionA1A2SBPlain=regionA1A2SB[0];
			regionABSBPlain=regionABSB[0];

			nCandsAnaPlain=nCandsAna;
			neventAnaSigPlain=neventAnaSig;
			runAnaPlain=runAna;
			lepAnaPlain=lepAna[0];
			mZZAnaPlain=mZZAna[0];
			mJJAnaPlain=mJJAna[0];
			mLLAnaPlain=mLLAna[0];
			nsubj21AnaPlain=nsubj21Ana[0];
			prMJAnaPlain=prMJAna[0];
			mJJNoKinFitAnaSigPlain=mJJNoKinFitAnaSig[0];
			vTagPurityAnaPlain=vTagPurityAna[0];
			nXjetsAnaPlain=nXjetsAna[0];
			HLTweightAnaSigPlain=HLTweightAnaSig;
			PUweightAnaSigPlain=PUweightAnaSig;
			LumiWeightAnaSigPlain=LumiWeightAnaSig;
			GenWeightAnaSigPlain=GenWeightAnaSig;
			BTagWeightAnaSigPlain=BTagWeightAnaSig;
			VTagWeightAnaSigPlain=VTagWeightAnaSig;
			weightAnaSigPlain=weightAnaSig;
			categoriesPlain=categories[0];
			MCmatchAnaPlain=MCmatchAna[0];				


			if(goodevent)
			{
				tAnaSig->Fill();//signal and sideband as usual
				tAnaSB->Fill();//signal and sideband as usual
				tA1A2Sig->Fill();// sideband A1, signal A2
				tA1A2SB->Fill();// sideband A1, signal A2
				tABSig->Fill();// sideband A, signal B
				tABSB->Fill();// sideband A, signal B

				tAnaSigPlain->Fill();//signal and sideband as usual
				tAnaSBPlain->Fill();//signal and sideband as usual
                tA1A2SigPlain->Fill();// sideband A1, signal A2
                tA1A2SBPlain->Fill();// sideband A1, signal A2
                tABSigPlain->Fill();// sideband A, signal B
                tABSBPlain->Fill();// sideband A, signal B

			}
		}//end of loop over input tree
		fIn->Close();
		delete fIn;

		//save the trees
		fOutSig->cd();
		tAnaSig->Write();
		tA1A2Sig->Write();
		tABSig->Write();
		tAnaSigPlain->Write();
        tA1A2SigPlain->Write();
        tABSigPlain->Write();
		fOutSig->Close();
		delete fOutSig;


		fOutSB->cd();
		tAnaSB->Write();
		tA1A2SB->Write();
		tABSB->Write();
		tAnaSBPlain->Write();
        tA1A2SBPlain->Write();
        tABSBPlain->Write();
		fOutSB->Close();
		delete fOutSB;

	}//endl of loop over samples

	return 0;
}












