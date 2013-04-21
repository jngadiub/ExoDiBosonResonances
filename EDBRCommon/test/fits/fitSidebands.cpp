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

#include "SidebandFitter.h"

void CopyTreeVecToPlain(TTree *t1, std::string wType, std::string f2Name, std::string t2Name,int nxjCut=-1);
void doAlpha(TTree *chMC, std::string wType);

//##############EDIT THIS PART####################

const std::string myOutDir="FitSidebandsMJJ_CA8_WW_V11/";
const string inDirSIG="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/AnaSigTree_from50_noConv/";
const string inDirSB ="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/AnaSBTree_from50_noConv/";

/*
const std::string myOutDir="FitSidebandsMJJ_ZZ_20130418_VVMC/";
const string inDirSIG="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1d/fullsig/";
const string inDirSB ="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1d/fullsb/";
*/
unsigned int jetCats = 1;//1 for only 1 jet case, 2 for both
bool isZZChannel=false;//this will change only the file list
string leptStr="ALL";//"MU" //"ELE"//"ALL"
const std::string InTreeName="SelectedCandidates";
const bool useAlphaVV=false;
//################################################

const int nxjCut=-1;//if negative: no cut
const std::string tmpTreeName="SelectedCandidatesV2";
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
			chainMC->Add( (inDir+"treeEDBR_SingleTop_xww.root").c_str());		
			chainMC->Add( (inDir+"treeEDBR_WJetsPt100_xww.root").c_str());
			////chainMC->Add( (inDir+"treeEDBR_WJetsPt180_xww.root").c_str());
			if(useAlphaVV){
			  chainMC->Add( (inDir+"treeEDBR_TTBARpowheg_xww.root").c_str());
			  chainMC->Add( (inDir+"treeEDBR_VV_xww.root").c_str());
			}
			else{
			  chainMCVV = new TChain(InTreeName.c_str());
			  chainMCVV->Add( (inDir+"treeEDBR_TTBARpowheg_xww.root").c_str());
			  chainMCVV->Add( (inDir+"treeEDBR_VV_xww.root").c_str());
			}
		}
		gROOT->cd(); //magic!

		if(nxjCut>=0)  sprintf(foutn,"EXOVVTree_forAlpha_SIG_%dJ.root",nxjCut);
		else   sprintf(foutn,"EXOVVTree_forAlpha_SIG_NOcut.root");
		std::string tmpFileName=foutn;
		CopyTreeVecToPlain(chainMC,weighting,tmpFileName,tmpTreeName,nxjCut);//(TTree*)

		if(!useAlphaVV){
		  if(nxjCut>=0)  sprintf(foutn,"EXOVVTree_MCVV_SIG_%dJ.root",nxjCut);
		  else   sprintf(foutn,"EXOVVTree_MCVV_SIG_NOcut.root");
		  tmpFileName=foutn;
		  CopyTreeVecToPlain(chainMCVV,weighting,tmpFileName,tmpTreeName,nxjCut);//(TTree*)
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
            chainMC->Add( (inDir+"treeEDBR_SingleTop_xww.root").c_str());    
            chainMC->Add( (inDir+"treeEDBR_WJetsPt100_xww.root").c_str());
            ////chainMC->Add( (inDir+"treeEDBR_WJetsPt180_xww.root").c_str());
            if(useAlphaVV){
              chainMC->Add( (inDir+"treeEDBR_TTBARpowheg_xww.root").c_str());
              chainMC->Add( (inDir+"treeEDBR_VV_xww.root").c_str());
            }   
            else{
              chainMCVV = new TChain(InTreeName.c_str());
              chainMCVV->Add( (inDir+"treeEDBR_TTBARpowheg_xww.root").c_str());
              chainMCVV->Add( (inDir+"treeEDBR_VV_xww.root").c_str());
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




	//  TFile *ftree=new TFile(foutn,"READ");
	// TTree *tTmp=(TTree*)ftree->Get(tmpTreeName.c_str());

	//  sprintf(foutn,"EXOVVTree_forAlpha_%d.root",nxjCut);
	// TFile *ftree2=new TFile(foutn,"READ");

	TChain *tTmp=new TChain(tmpTreeName.c_str());
	if(nxjCut>=0) sprintf(foutn,"EXOVVTree_forAlpha_SIG_%dJ.root",nxjCut);
	else  sprintf(foutn,"EXOVVTree_forAlpha_SIG_NOcut.root");
	tTmp->Add(foutn);
	if(nxjCut>=0) sprintf(foutn,"EXOVVTree_forAlpha_SB_%dJ.root",nxjCut);
	else  sprintf(foutn,"EXOVVTree_forAlpha_SB_NOcut.root");
	tTmp->Add(foutn);
	doAlpha(tTmp,weighting);

	delete tTmp;
	return 0;

}//end main

void doAlpha(TTree *chMC, std::string wType){
	unsigned int nToys = 500;

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


			outFileName=myOutDir+"/Workspaces_alpha_"+ss.str()+"J_"+pur_str+"_"+leptStr+".root";
			sf->setOutFile(outFileName);
			sf->setCanvasLabel("_Madgraph");
			RooWorkspace* alpha_nxj = sf->getAlphaFit( treeMC_nxj , inxj,  leptStr, purityCut,true);



			// now estimate stat errors by throwing toys
			// 1: get the histos produced before and saved in the output file
			cout<<"\n*** Throwing toys for category "<<inxj<<"Jet "<< pur_str.c_str()<<" ***"<<endl<<endl;
			//  outFileName=myOutDir+"/Workspaces_alpha_"+ss.str()+"J_"+pur_str+"_"+leptStr+".root";
			TFile *fWS=new TFile(outFileName.c_str(),"UPDATE");
			// fWS->ls();
			TH1D *myalpha=(TH1D*)fWS->Get("h_alpha_smoothened");

			/*
			   outFileName=myOutDir+"/Workspaces_alpha_"+ss.str()+"btag_"+leptStr+".root";
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
			TCanvas *calphaAVG=new TCanvas("c_avg","CANVAS with Alpha histo",800,800);
			calphaAVG->cd();
			std::vector<double> avgpars;
			std::vector<double> avgerrs;
			sf->alphaFit( myalpha , avgpars,avgerrs);
			myalpha->SetMarkerStyle(20);
			myalpha->SetMarkerColor(kBlue);
			myalpha->SetXTitle("m_{ZZ} [GeV]");
			myalpha->SetYTitle("#alpha");
			myalpha->GetFunction("ratio_fit_expo")->SetBit(TF1::kNotDraw);
			myalpha->GetFunction("fitPolyRooFit")->SetBit(TF1::kNotDraw);
			myalpha->Draw("PE0");
			char canvasName[400];
			sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s.eps", myOutDir.c_str(), inxj,pur_str.c_str(), leptStr.c_str());
			calphaAVG->SaveAs( canvasName  );
			sprintf( canvasName, "%s/mZZ_alpha_%dJ%s_%s.pdf", myOutDir.c_str(), inxj,pur_str.c_str(), leptStr.c_str());
			calphaAVG->SaveAs( canvasName  );
			delete calphaAVG;

			cout<<"Generating toys"<<endl;
			//2: use it as template for generating toys
			TH1D *h_dist_p0=new TH1D("h_dist_p0","Distribution of par0 of fit to alpha for 500 toys",1000,-5.0,5.0);
			for(unsigned int i = 0 ; i <nToys ; i++) {

				char histotitle[50];
				sprintf(histotitle,"tmp_%d",i);
				TH1D* variedHisto = sf->shuffle(myalpha, randomGen ,histotitle, myalpha);

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

			float alpha =  ((TF1*)myalpha->GetFunction(sf->getFitFunc("_LowRange").c_str()))->Eval(400);
			std::cout << "alpha (M=400) : " << alpha << std::endl;    
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



void CopyTreeVecToPlain(TTree *t1, std::string wType, std::string f2Name,std::string t2Name,int nxjCut){

	int ncands; 
	double eventWeight;
	unsigned int nrun,nevt;
	double leptType[35];
	int mynxj[35];
	double mZZd[35],region[35],mZqq[35];
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
	t1->SetBranchAddress("region",region);
	t1->SetBranchAddress("vTagPurity",vTagPurity);
	t1->SetBranchAddress("nsubj21",nsubj21);

	TFile *fOut=new TFile(f2Name.c_str(),"RECREATE");
	fOut->cd();

	int ncands_2, mynxj_2;
	double eventWeight_2;
	unsigned int nrun_2,nevt_2;
	double leptType_2;
	double mZZd_2,region_2,mZqq_2, nsubj21_2;
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
			region_2=region[j];
			mZqq_2=mZqq[j];
			mynxj_2=int(mynxj[j]);
			nsubj21_2=nsubj21[j]; 
			vTagPurity_2=vTagPurity[j]; 

			//if(nsubj21_2>0.45)continue;

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
