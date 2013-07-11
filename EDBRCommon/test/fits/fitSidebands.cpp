#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TH1D.h"
#include "TF1.h"
#include "TString.h"
#include "TROOT.h"
#include "TCanvas.h"
#include <iostream>

#include "SidebandFitter.h"
//#include "Config_XWW.h"
#include "Config_XZZ.h"
using namespace std;

void CopyTreeVecToPlain(TChain *t1, std::string wType, std::string f2Name, std::string t2Name,int nxjCut=-1,bool ScaleTTbar=0);
void doAlpha(TTree *chMC, std::string wType, vector<TH1D> R0_vector);
vector<TH1D> doR0(TTree* treeSBOther, TTree* treeSBData);

/*****************
 *
 * All configurations now are in the header file fitSidebandsConfig_XZZ.h and binningFits_XZZ.h
 *
 *****************/
const int nxjCut=-1;//if negative: no cut
const std::string tmpTreeName="SelectedCandidatesV2";
ofstream loga((myOutDir+"./log_fitSidebands_"+leptType+".log").c_str(),ios::out);
int main( int argc, char* argv[] ) {

	std::string weighting = "weight";
	bool unrollSIGTree=true;
	bool unrollSBTree=true;

	string inDir="dummy";
	char foutn[64];

	if(unrollSIGTree){
		inDir=inDirSIG;
		TChain* chainMC = new TChain(InTreeName.c_str());
		TChain* chainMCVV = 0;
		if(isZZChannel==true)
		{
			//	chainMC->Add( (inDir+"treeEDBR_DYJetsPt50To70.root").c_str());
			chainMC->Add( (inDir+"treeEDBR_DYJetsPt70To100.root").c_str());
			chainMC->Add( (inDir+"treeEDBR_DYJetsPt100.root").c_str());
			chainMC->Add( (inDir+"treeEDBR_TTBARpowheg.root").c_str());
			if(useAlphaVV){
				chainMC->Add( (inDir+"treeEDBR_WW.root").c_str());
				chainMC->Add( (inDir+"treeEDBR_WZ.root").c_str());
				chainMC->Add( (inDir+"treeEDBR_ZZ.root").c_str());
			}
			else{
				chainMCVV = new TChain(InTreeName.c_str());
				chainMCVV->Add( (inDir+"treeEDBR_WW.root").c_str());
				chainMCVV->Add( (inDir+"treeEDBR_WZ.root").c_str());
				chainMCVV->Add( (inDir+"treeEDBR_ZZ.root").c_str());
			}
		}
		if(isZZChannel==false)
		{
			chainMC->Add( (inDir+"treeEDBR_DYJets_xww.root").c_str());
			//chainMC->Add( (inDir+"treeEDBR_WJetsPt100_xww.root").c_str());
			chainMC->Add( (inDir+"treeEDBR_WJetsPt180_xww.root").c_str());
			if(useAlphaVV){
				chainMC->Add( (inDir+"treeEDBR_TTBARpowheg_xww.root").c_str());
				chainMC->Add( (inDir+"treeEDBR_VV_xww.root").c_str());
				chainMC->Add( (inDir+"treeEDBR_SingleTop_xww.root").c_str());
			}
			else{
				chainMCVV = new TChain(InTreeName.c_str());
				chainMCVV->Add( (inDir+"treeEDBR_TTBARpowheg_xww.root").c_str());
				chainMCVV->Add( (inDir+"treeEDBR_SingleTop_xww.root").c_str());
				chainMCVV->Add( (inDir+"treeEDBR_VV_xww.root").c_str());
			}
		}
		gROOT->cd(); //magic!


		if(nxjCut>=0)  sprintf(foutn,"EXOVVTree_forAlpha_SIG_%dJ.root",nxjCut);
		else   sprintf(foutn,"EXOVVTree_forAlpha_SIG_NOcut.root");
		std::string tmpFileName=foutn;
		bool ScaleTTbar=false;
		if(!isZZChannel)ScaleTTbar=true;//for ww case, apply ttbar scale factor
		CopyTreeVecToPlain(chainMC,weighting,tmpFileName,tmpTreeName,nxjCut,ScaleTTbar);//(TTree*)

		if(!useAlphaVV){
			if(nxjCut>=0)  sprintf(foutn,"EXOVVTree_MCVV_SIG_%dJ.root",nxjCut);
			else   sprintf(foutn,"EXOVVTree_MCVV_SIG_NOcut.root");
			tmpFileName=foutn;
			CopyTreeVecToPlain(chainMCVV,weighting,tmpFileName,tmpTreeName,nxjCut,ScaleTTbar);//(TTree*)
		}
		delete chainMC; //delete chainData;
		delete chainMCVV;
	}//end if unrollSIGTree

	if(unrollSBTree){
		inDir=inDirSB;
		TChain* chainMC = new TChain(InTreeName.c_str());
		TChain* chainMCVV = 0;
		if(isZZChannel==true)
		{   
			chainMC->Add( (inDir+"treeEDBR_DYJetsPt50To70.root").c_str());
			chainMC->Add( (inDir+"treeEDBR_DYJetsPt70To100.root").c_str());
			chainMC->Add( (inDir+"treeEDBR_DYJetsPt100.root").c_str());
			chainMC->Add( (inDir+"treeEDBR_TTBARpowheg.root").c_str());
			if(useAlphaVV){
				chainMC->Add( (inDir+"treeEDBR_WW.root").c_str());
				chainMC->Add( (inDir+"treeEDBR_WZ.root").c_str());
				chainMC->Add( (inDir+"treeEDBR_ZZ.root").c_str());
			}
			else{
				chainMCVV = new TChain(InTreeName.c_str());
				chainMCVV->Add( (inDir+"treeEDBR_WW.root").c_str());
				chainMCVV->Add( (inDir+"treeEDBR_WZ.root").c_str());
				chainMCVV->Add( (inDir+"treeEDBR_ZZ.root").c_str());
			}

		}   
		if(isZZChannel==false)
		{   
			chainMC->Add( (inDir+"treeEDBR_DYJets_xww.root").c_str());
			//chainMC->Add( (inDir+"treeEDBR_WJetsPt100_xww.root").c_str());
			chainMC->Add( (inDir+"treeEDBR_WJetsPt180_xww.root").c_str());
			if(useAlphaVV){
				chainMC->Add( (inDir+"treeEDBR_TTBARpowheg_xww.root").c_str());
				chainMC->Add( (inDir+"treeEDBR_VV_xww.root").c_str());
				chainMC->Add( (inDir+"treeEDBR_SingleTop_xww.root").c_str());
			}   
			else{
				chainMCVV = new TChain(InTreeName.c_str());
				chainMCVV->Add( (inDir+"treeEDBR_TTBARpowheg_xww.root").c_str());
				chainMCVV->Add( (inDir+"treeEDBR_VV_xww.root").c_str());
				chainMCVV->Add( (inDir+"treeEDBR_SingleTop_xww.root").c_str());
			}
		}
		gROOT->cd(); //magic!

		if(nxjCut>=0)  sprintf(foutn,"EXOVVTree_forAlpha_SB_%dJ.root",nxjCut);
		else   sprintf(foutn,"EXOVVTree_forAlpha_SB_NOcut.root");
		std::string tmpFileName=foutn;
		CopyTreeVecToPlain(chainMC,weighting,tmpFileName,tmpTreeName,nxjCut);//(TTree*)


		if(!useAlphaVV){
			if(nxjCut>=0)  sprintf(foutn,"EXOVVTree_MCVV_SB_%dJ.root",nxjCut);
			else   sprintf(foutn,"EXOVVTree_MCVV_SB_NOcut.root");
			tmpFileName=foutn;
			CopyTreeVecToPlain(chainMCVV,weighting,tmpFileName,tmpTreeName,nxjCut);//(TTree*)
		}

		delete chainMC; //delete chainData;
		delete chainMCVV;
	}//end if unrollSIGTree


	TChain *tTmp=new TChain(tmpTreeName.c_str());
	if(nxjCut>=0) sprintf(foutn,"EXOVVTree_forAlpha_SIG_%dJ.root",nxjCut);
	else  sprintf(foutn,"EXOVVTree_forAlpha_SIG_NOcut.root");
	tTmp->Add(foutn);
	if(nxjCut>=0) sprintf(foutn,"EXOVVTree_forAlpha_SB_%dJ.root",nxjCut);
	else  sprintf(foutn,"EXOVVTree_forAlpha_SB_NOcut.root");
	tTmp->Add(foutn);


	/////////////////do R0/////////////	
	//get sidebband Other
	TChain *treeSBOther = new TChain(tmpTreeName.c_str());
	treeSBOther->Add("EXOVVTree_MCVV_SB_NOcut.root");

	//get sideband data
	TChain * chainSBData = new TChain (InTreeName.c_str());
	if(isZZChannel)
	{    
		chainSBData->Add( (inDirSB+"treeEDBR_DoubleMu_Run2012A*.root").c_str()  );
		chainSBData->Add( (inDirSB+"treeEDBR_DoubleMuParked_Run2012*.root").c_str()  );
		chainSBData->Add( (inDirSB+"treeEDBR_DoublePhotonHighPt_Run2012*.root").c_str()  );
		chainSBData->Add( (inDirSB+"treeEDBR_Photon_Run2012A*.root").c_str()  );
	}    
	else 
	{    
		chainSBData->Add( (inDirSB+"treeEDBR_data_xww.root").c_str() );
	}	
	CopyTreeVecToPlain(chainSBData,weighting,"EXOVVTree_DaTa__SB_NOcut.root",tmpTreeName,nxjCut);
	delete chainSBData;

	TChain *treeSBData=new TChain(tmpTreeName.c_str());
	treeSBData->Add("EXOVVTree_DaTa__SB_NOcut.root");


	//calculate R0
	vector<TH1D> R0_vector  = doR0 (treeSBOther,treeSBData);
	doAlpha(tTmp,weighting,R0_vector);

	delete tTmp;

	return 0;

}//end main

void doAlpha(TTree *chMC, std::string wType, vector<TH1D> R0_vector){


	TRandom3* randomGen = new TRandom3(13);

	///loop on different topologies/categories: the code
	//will performa a separate bkg estimation for each of them
	for( unsigned inxj=1; inxj<=jetCats; ++inxj ) {

		int nPurities=1;
		if(inxj==1)nPurities=2;

		double purityCut=-1;
		for(int iP=0;iP<nPurities;iP++){//loop over purity categories
			if(inxj==1)purityCut=iP;//for 2J category, no cut on Purity

			SidebandFitter *sf = new SidebandFitter( wType);

			sf->setOutDir(myOutDir);

			int nxjCut_=inxj;
			int nentriesTOT=chMC->GetEntries();
			std::cout<<"Cutting nXjets=="<<nxjCut_<<" on a chain with "<< nentriesTOT<<" entries"<<std::endl;

			int nxjOld;
			chMC->SetBranchAddress("nXjets",&nxjOld);
			TTree* treeMC_nxj=(TTree*)chMC->CloneTree(0);
			for (Int_t iOld=0;iOld<nentriesTOT; iOld++) {
				chMC->GetEntry(iOld);
				if(nxjOld==nxjCut_)treeMC_nxj->Fill();
			}

			std::cout<<"Cut applied: "<<treeMC_nxj->GetEntries()<< " entries remain"<<std::endl;
			string outFileName;
			std::stringstream ss;
			ss << inxj;

			std::string pur_str="";
			if(purityCut==0)pur_str="LP";
			if(purityCut==1)pur_str="HP";


			outFileName=myOutDir+"/Workspaces_alpha_"+ss.str()+"J_"+pur_str+"_"+leptType+".root";
			sf->setOutFile(outFileName);
			sf->setCanvasLabel("_Madgraph");
			RooWorkspace* alpha_nxj = sf->getAlphaFit( treeMC_nxj , inxj,  leptType, purityCut,true);



			// now estimate stat errors by throwing toys
			// 1: get the histos produced before and saved in the output file
			cout<<"\n*** Throwing toys for category "<<inxj<<"Jet "<< pur_str.c_str()<<" ***"<<endl<<endl;
			//  outFileName=myOutDir+"/Workspaces_alpha_"+ss.str()+"J_"+pur_str+"_"+leptType+".root";
			TFile *fWS=new TFile(outFileName.c_str(),"UPDATE");
			// fWS->ls();
			//TH1D *myalpha=(TH1D*)fWS->Get("h_alpha_smoothened");
			TH1D *alpha_ORI=(TH1D*)fWS->Get("h_alpha_smoothened");

			//add R0 to alpha
			TH1D * alpha_Final = (TH1D*)alpha_ORI->Clone("h_alpha_smoothened_Final");
			TH1D * alpha_Ori = (TH1D*)alpha_ORI->Clone("h_alpha_smoothened_Ori");
			TH1D * R0=0;
			if(inxj==1&&iP==0)R0=&R0_vector.at(0);//1JLP
			else if(inxj==1&&iP==1)R0=&R0_vector.at(1);//1JHP
			else if(inxj==2)R0=&R0_vector.at(2);//2J

			/*
			//a test to see we get what they are
			TCanvas * cr1 = new TCanvas();
			R0->Draw();
			TString r0test = Form("test_%dJ_%d_R0_test.png",inxj,iP);
			cr1->SaveAs( r0test );
			 */

			//do alpha_Final = (1-R0) * alpha_ORI
			alpha_Final->Reset();
			alpha_Ori->Reset();
			for (int iBin = 1 ; iBin <= alpha_Final->GetNbinsX() ; ++iBin)
			{
				double alpha_ORI_c = alpha_ORI->GetBinContent(iBin);
				double alpha_ORI_e = alpha_ORI->GetBinError(iBin);
				if(!isZZChannel)alpha_ORI_e = alpha_ORI->GetBinError(iBin)*2;//multiply by 2 the uncertainties on alpha to account for differences in wjets parton showering

				double R0_c = R0->GetBinContent(iBin);
				double R0_e = R0->GetBinError(iBin);
				double alpha_Final_c = (1-R0_c)*alpha_ORI_c;
				double alpha_Final_e = sqrt(alpha_ORI_c*alpha_ORI_c*R0_e*R0_e+(1-R0_c)*(1-R0_c)*alpha_ORI_e*alpha_ORI_e);
				alpha_Final->SetBinContent(iBin,alpha_Final_c);
				alpha_Final->SetBinError(iBin,alpha_Final_e);
				alpha_Ori->SetBinContent(iBin,alpha_ORI_c);
				alpha_Ori->SetBinError(iBin,alpha_ORI_e);
			}

			TH1D *myalpha= alpha_Final;
			//TH1D *myalpha= alpha_ORI;

			//save R0 and alpha_Final in the same file
			R0->Write();
			alpha_Final->Write();	


			/*
			   outFileName=myOutDir+"/Workspaces_alpha_"+ss.str()+"btag_"+leptType+".root";
			   TFile *fWS=new TFile(outFileName.c_str(),"UPDATE");
			// fWS->ls();
			TH1D *myalpha=(TH1D*)fWS->Get("h_alpha_smoothened");
			cout<<"Fillin averaged histo"<<endl;
			for(int i=1 ; i <= halfdiff->GetNbinsX() ; i++) {
			double valSh=myalphaSh->GetBinContent(i);
			double errSh=myalphaSh->GetBinError(i);
			double valMd=myalphaMd->GetBinContent(i);
			double errMd=myalphaMd->GetBinError(i);
			double wNorm =1.0/(errSh*errSh) + 1.0/(errMd*errMd);
			double wSh=  1.0/(errSh*errSh)  ;
			double wMd=1.0/(errMd*errMd);
			halfdiff->SetBinContent(i,fabs(valSh-valMd)/2.);
			myalpha->SetBinContent(i,(wSh/wNorm)*valSh+(wMd/wNorm)*valMd);
			double err2=(wSh/wNorm)*(wSh/wNorm)*errSh*errSh + (wMd/wNorm)*(wMd/wNorm)*errMd*errMd;
			myalpha->SetBinError(i,sqrt(err2));
			}
			 */

            //draw alpha origin
			TCanvas *canAlphaOri=new TCanvas("canAlphaOri","canAlphaOri",800,800);
			canAlphaOri->cd();
			alpha_Ori->SetMarkerStyle(20);
			alpha_Ori->SetMarkerColor(kBlue);
			alpha_Ori->SetXTitle("m_{ZZ} [GeV]");
			if(!isZZChannel)alpha_Ori->SetXTitle("m_{WW} [GeV]");
			alpha_Ori->SetYTitle("#alpha");
			alpha_Ori->GetYaxis()->SetRangeUser(0,4);
			alpha_Ori->Draw("PE0");
			char canvasName[400];
			sprintf( canvasName, "%s/mZZ_alpha_Ori_%dJ%s_%s.png", myOutDir.c_str(), inxj,pur_str.c_str(), leptType.c_str());
			canAlphaOri->SaveAs( canvasName  );
			delete canAlphaOri;

			TCanvas *calphaAVG=new TCanvas("c_avg","CANVAS with Alpha histo",800,800);
			calphaAVG->cd();
			std::vector<double> avgpars;
			std::vector<double> avgerrs;
			sf->alphaFit( myalpha , avgpars,avgerrs);
			myalpha->SetMarkerStyle(20);
			myalpha->SetMarkerColor(kBlue);
			myalpha->SetXTitle("m_{ZZ} [GeV]");
			if(!isZZChannel)myalpha->SetXTitle("m_{WW} [GeV]");
			myalpha->SetYTitle("#alpha");
			//myalpha->GetFunction("ratio_fit_expo")->SetBit(TF1::kNotDraw);
			//myalpha->GetFunction("fitPolyRooFit")->SetBit(TF1::kNotDraw);
			myalpha->GetFunction("alpha_fitfunc")->SetBit(TF1::kNotDraw);
			myalpha->GetYaxis()->SetRangeUser(0,4);
			myalpha->Draw("PE0");
			sprintf( canvasName, "%s/mZZ_alpha_Final_%dJ%s_%s.eps", myOutDir.c_str(), inxj,pur_str.c_str(), leptType.c_str());
			calphaAVG->SaveAs( canvasName  );
			sprintf( canvasName, "%s/mZZ_alpha_Final_%dJ%s_%s.pdf", myOutDir.c_str(), inxj,pur_str.c_str(), leptType.c_str());
			calphaAVG->SaveAs( canvasName  );
			sprintf( canvasName, "%s/mZZ_alpha_Final_%dJ%s_%s.png", myOutDir.c_str(), inxj,pur_str.c_str(), leptType.c_str());
			calphaAVG->SaveAs( canvasName  );
			delete calphaAVG;
			cout<<"Generating toys"<<endl;
			//2: use it as template for generating toys
			TH1D *h_dist_p0=new TH1D("h_dist_p0","Distribution of par0 of fit to alpha for 500 toys",1000,-5.0,5.0);
			TH1D *h1_mZZ_signalRegion   =(TH1D*)fWS->Get("mX_signalRegion");
			TH1D *h1_mZZ_sidebands      =(TH1D*)fWS->Get("mX_sidebands");
			TH1D *h1_mZZ_signalRegion_nw=(TH1D*)fWS->Get("mX_signalRegion_noWeight");
			TH1D *h1_mZZ_sidebands_nw   =(TH1D*)fWS->Get("mX_sidebands_noWeight");
			for(unsigned int i = 0 ; i <nToys ; i++) {

				char histotitle[50];
				sprintf(histotitle,"tmp_%d",i);
				TH1D* variedHisto = 0 ;
				if(alphaPoisson)
					variedHisto = sf->realpharize(h1_mZZ_signalRegion ,h1_mZZ_sidebands , h1_mZZ_signalRegion_nw ,h1_mZZ_sidebands_nw  ,R0, randomGen ,histotitle);
				else
					variedHisto = sf->shuffle(myalpha, randomGen ,histotitle, NULL);


				variedHisto->Write();
				//3:for each toy make a fit and save the results in the output histo
				std::vector<double> tmppars;
				std::vector<double> tmperrs;
				sf->alphaFit( variedHisto , tmppars,tmperrs);
				h_dist_p0->Fill(tmppars.at(0));
				// sf->fitPseudo( treeMC_Xbtag, treeDATA_Xbtag, ibtag, "ALL", variedHisto,i);
				//delete variedHisto;
			}
			h_dist_p0->Write();

			float alpha =  ((TF1*)myalpha->GetFunction(sf->getFitFunc("_LowRange").c_str()))->Eval(800);
			std::cout << "alpha (M=800) : " << alpha << std::endl;    
			char hlphname[50];
			sprintf(hlphname,"nominal_alpha_%dJ",inxj);
			TH1D* dummy_alpha=sf->dummyAlphaHist(alpha,myalpha,hlphname);
			dummy_alpha->Write();
			TCanvas can("ctoy","ctoy",600,600);
			for(unsigned int i = 0 ; i <nToys ; i++) {
				char histotitle[50];
				sprintf(histotitle,"tmp_%d",i);
				TH1D* variedHisto = (TH1D*)fWS->Get(histotitle);
				if(i==0)
					variedHisto->Draw("HIST");
				else{
					variedHisto->Draw("HIST,same");
				}
			}
			myalpha->SetLineColor(2);
			myalpha->SetMarkerColor(2);
			myalpha->Draw("same");
			//    char canvasName[400];
			sprintf( canvasName, "%s/mZZ_alphaToys_%dJ%s_%s.eps", myOutDir.c_str(), inxj,pur_str.c_str(), "ALL");
			can.SaveAs(canvasName);
			sprintf( canvasName, "%s/mZZ_alphaToys_%dJ%s_%s.pdf", myOutDir.c_str(), inxj,pur_str.c_str(), "ALL");
			can.SaveAs(canvasName);


			myalpha->Write();
			fWS->Close();
			cout<<"Deleting "<<endl; 
			//delete h_dist_p0;
			delete fWS;
			delete alpha_nxj;
			delete sf;


			//     RooFitResult* fr = sf->fitSidebands( treeMC_Xbtag, treeDATA_Xbtag, inxj, "ALL", alpha_Xbtag );    
			//     for(int i = 0 ; i <nToys ; i++) {
			//       std::cout << std::endl << "[ " << inxj << " jets ]  Toy: " << i << "/" << nToys << std::endl;
			//       TH1D* variedHisto = sf->shuffle(alpha_Xbtag, randomNum ,"tmp");
			//       sf->fitPseudo( treeMC_Xbtag, treeDATA_Xbtag, ibtag, "ALL", variedHisto,i);
			//       delete variedHisto;
			//     }    
			//     if( nToys > 0 )
			//       sf->pseudoMassge(nToys, inxj,"ALL",fr);
			//     delete fr;    




		}//end loop over purities

	} //end loop on nXjets

}//end void doAlpha



void CopyTreeVecToPlain(TChain *t1, std::string wType, std::string f2Name,std::string t2Name,int nxjCut,bool ScaleTTbar){

	int ncands; 
	double eventWeight;
	unsigned int nrun,nevt;
	double leptType[35];
	int mynxj[35];
	double mZZd[35],region[35],mZqq[35],mZqqNoKF[35];
	double vTagPurity[35];
	double nsubj21[35];

	t1->SetBranchAddress("nCands",&ncands);
	t1->SetBranchAddress("run",&nrun);
	t1->SetBranchAddress("event",&nevt);
	t1->SetBranchAddress("lep",leptType);
	t1->SetBranchAddress(wType.c_str(),&eventWeight);
	t1->SetBranchAddress("mZZ",mZZd);
	t1->SetBranchAddress("nXjets",mynxj);
	t1->SetBranchAddress("mJJ",mZqq);
	t1->SetBranchAddress("mJJNoKinFit",mZqqNoKF);
	t1->SetBranchAddress("region",region);
	t1->SetBranchAddress("vTagPurity",vTagPurity);
	t1->SetBranchAddress("nsubj21",nsubj21);

	TFile *fOut=new TFile(f2Name.c_str(),"RECREATE");
	fOut->cd();

	int ncands_2, mynxj_2;
	double eventWeight_2;
	unsigned int nrun_2,nevt_2;
	double leptType_2;
	double mZZd_2,region_2,mZqq_2,mZqqNoKF_2, nsubj21_2;
	double vTagPurity_2;

	TTree *t2=new TTree(t2Name.c_str(),t2Name.c_str());

	t2->Branch("nCands",&ncands_2,"nCands/I");
	t2->Branch("run",&nrun_2,"run/i");
	t2->Branch("event",&nevt_2,"event/i");
	t2->Branch("weight",&eventWeight_2,"weight/D");
	t2->Branch("nXjets",&mynxj_2 , "nXjets/I");
	t2->Branch("lep",&leptType_2,"lep/D");
	t2->Branch("mZZ",&mZZd_2 , "mZZ/D");
	t2->Branch("mJJ",&mZqq_2 , "mJJ/D");
	t2->Branch("mJJNoKinFit",&mZqqNoKF_2, "mJJNoKinFit/D");
	t2->Branch("region",&region_2 , "region/D");
	t2->Branch("nsubj21",&nsubj21_2, "nsubj21/D");
	t2->Branch("vTagPurity",&vTagPurity_2, "vTagPurity/D");


	for(int i=0;i<t1->GetEntries();i++){

		t1->GetEntry(i);

		for(int j=0;j<ncands;j++){
			ncands_2=ncands;
			nrun_2=nrun;
			nevt_2=nevt;
			eventWeight_2=eventWeight;
			leptType_2=leptType[j];
			mZZd_2=mZZd[j];
			mZqq_2=mZqq[j];
			mZqqNoKF_2=mZqqNoKF[j];
			region_2=region[j];
			if(isZZChannel&&(mynxj_2==1&&mZqqNoKF_2>110.0))region_2=2;//upper sideband kept separated
			mynxj_2=int(mynxj[j]);
			nsubj21_2=nsubj21[j]; 
			vTagPurity_2=vTagPurity[j]; 

			//if(nsubj21_2>0.45)continue;


			//now add ttbar scale factor to t1, and only to ttbar 
			if(ScaleTTbar)
			{
				double ttbar_scale =1;//default no scale
				//double tttar_scale_error=0;

				//for the analysis
				if(InTreeName=="SelectedCandidates")
				{
					if(vTagPurity_2==0&&leptType_2==0)ttbar_scale = 1.297481;//eleLP
					if(vTagPurity_2==1&&leptType_2==0)ttbar_scale = 0.966417;//eleHP
					if(vTagPurity_2==0&&leptType_2==1)ttbar_scale = 1.226198;//muLP
					if(vTagPurity_2==1&&leptType_2==1)ttbar_scale = 0.958444;//muHP
				}
				else if (InTreeName=="SelectedCandidatesAB")
				{
					//for closure test A->B
					if(vTagPurity_2==0&&leptType_2==0)ttbar_scale = 1.296917;//eleLP
					if(vTagPurity_2==1&&leptType_2==0)ttbar_scale = 0.957699;//eleHP
					if(vTagPurity_2==0&&leptType_2==1)ttbar_scale = 1.208456;//muLP
					if(vTagPurity_2==1&&leptType_2==1)ttbar_scale = 1.046983;//muHP
				}

				TString filename = t1->GetFile()->GetEndpointUrl()->GetUrl();
				//cout<<filename<<endl;
				if(filename.Contains("TTBAR")||filename.Contains("SingleTop"))
				{
					//cout<<filename<<endl;
					eventWeight_2 =eventWeight_2*ttbar_scale;
				}
			}

			if(region[j]<0||mZZd_2>9999.0||mynxj_2>10||mZqq_2>999.0){
				//cout<<"Event="<<nevt<<" cand="<<j<<" has reg="<<region[j]<<" mZZ="<<mZZd_2<<endl;
				continue;
			}

			if(mynxj_2==nxjCut||nxjCut<0) {
				t2->Fill();
				//	if(i%1000==1)cout<<"Filled "<<nb<<" bytes"<<endl;
				//	if(nb<0)cout<<"====>  Event"<<nevt_2 <<"  Filled "<<nb<<" bytes"<<endl;
			}
		}//end loop on j -> nCands

	}//end loop on i -> entries of input tree

	cout<<"returning t2 with "<<t2->GetEntries()<<" entries"<<endl;




	//  cout<<"Writing unrolled tree"<<endl;
	// t2->Write();
	fOut->Write();
	delete t2;
	delete fOut;


	//  cout<<"returning"<<endl;
}


vector<TH1D> doR0(TTree* treeSBOther, TTree* treeSBData)
{
	//do R0 for 3 categories
	//0 1J LP; 1 1JHP; 2 2J ALLP
	vector<TH1D> R0_vector;
	for( unsigned int inxj=1; inxj<=jetCats; inxj++ ) 
	{
		double purityCut=-1;
		for(int iP=0;iP<=1;iP++)
		{	//loop over purity categories

			purityCut=(double)iP;
			if(inxj==2)
			{
				purityCut=-1;//do all purity
				if(iP==1)break;//for 2jet, do it once.
			}
			int nBins=0;
			const double* bins=0;
			if(inxj==2){
				nBins = nBins2;
				bins = bins2;
			}    
			else if(inxj==1){
				nBins = nBins1;
				bins = bins1;
			}
			loga<<inxj<<" "<<iP<<endl;
			TString pur_str="";
			if(purityCut==0)pur_str="LP";
			if(purityCut==1)pur_str="HP";
			if(purityCut==-1)pur_str="ALLP";

			double lepCut=-2;
			TString leptType_=leptType.c_str();
			if(leptType_=="ELE")lepCut=0;
			if(leptType_=="MU")lepCut=1;
			if(leptType_=="ALL")lepCut=-1;

			//make cut string
			TString lepTS = Form ("(lep==%.0f)",lepCut );
			if(lepCut==-1)lepTS="1";
			TString nXjetsTS = Form ("(nXjets==%d)",inxj );
			TString vTagPurityTS = Form ("(vTagPurity==%.0f)",purityCut );
			if(inxj==2)vTagPurityTS="1";//do not cut on purity in 2 jet case
			TString totalWeight = Form("weight*%f",lumi);
			TString Cut = lepTS+"*"+nXjetsTS+"*"+vTagPurityTS;			
			loga<<Cut<<endl;

			TH1D * mZZSBOther = new TH1D ("mZZSBOther","mZZSBOther",nBins-1,bins);
			TH1D * mZZSBData = new TH1D ("mZZSBData","mZZSBData",nBins-1,bins);
			mZZSBOther->Sumw2();
			mZZSBData->Sumw2();
			treeSBOther->Draw("mZZ>>mZZSBOther",Cut+"*"+totalWeight);
			treeSBData->Draw("mZZ>>mZZSBData",Cut);
			TH1D * R0 = (TH1D *) mZZSBOther->Clone("R0");
			R0->Reset();

			//now try to divide to get R0	 
			for(int ibin = 0; ibin < nBins-1; ibin++)	 
			{	 
				double SBOther_c = mZZSBOther->GetBinContent(ibin);	 
				double SBOther_e = mZZSBOther->GetBinError(ibin);	 
				double SBData_c  = mZZSBData->GetBinContent(ibin);	 
				double SBData_e  = mZZSBData->GetBinError(ibin);	 
				if(!isZZChannel) SBData_e  = 0; //R0 only error on others (not on data because it's double counted)	 
				double R0_c      = SBData_c==0?0:SBOther_c/SBData_c;	 
				double R0_e      = R0_c==0?0:R0_c*sqrt(   pow(SBOther_e/SBOther_c,2)   +   pow(SBData_e/SBData_c,2)  );	 
				R0->SetBinContent(ibin,R0_c);	 
				R0->SetBinError(ibin,R0_e);	 
			}	 
			//divide finished.	 
			//R0->Divide(mZZSBOther,mZZSBData);	 

			//now try to smooth R0	 
			SidebandFitter *sf = new SidebandFitter("weight");	 
			sf->smoothHist(*R0,true,2);


			R0->SetName(Form("R0_%dJ_"+pur_str+"_"+leptType_,inxj));
			R0->SetTitle(Form("R0_%dJ_"+pur_str+"_"+leptType_,inxj));

			TCanvas * cr0 = new TCanvas("cr0","cr0",600,800);
			cr0->Divide(1,3);
			cr0->cd(1);
			mZZSBOther->Draw();
			cr0->cd(2);
			mZZSBData->Draw();
			cr0->cd(3);
			R0->Draw();
			cr0->SaveAs(Form(myOutDir+"/R0_%dJ_"+pur_str+"_"+leptType+".png",inxj));
			cr0->SaveAs(Form(myOutDir+"/R0_%dJ_"+pur_str+"_"+leptType+".root",inxj));
			//got R0

			R0_vector.push_back(*R0);
			delete cr0;
			delete mZZSBOther;
			delete mZZSBData;
			delete R0;
		}
	}
	return R0_vector;
}
