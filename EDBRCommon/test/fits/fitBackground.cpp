#include <string>
#include <sstream>
#include <fstream>
#include <Riostream.h>

#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "TChain.h"
#include "TH1D.h"
#include "TString.h"
#include "TROOT.h"
#include "TF1.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TSystem.h"

#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooPlot.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooProdPdf.h"
#include "RooFitResult.h"
#include "RooPolynomial.h"
#include "RooWorkspace.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooChi2Var.h"
#include "RooMinuit.h"
#include "RooExponential.h"
#include "RooFitResult.h"
#include "RooGenericPdf.h"
#include "RooExtendPdf.h"
#include "RooBinning.h"
#include "RooEllipse.h"
#include "RooAddPdf.h"

#include "PdfDiagonalizer.h"
#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"

using namespace std ;
using namespace RooFit ;

//ROOT::gErrorIgnoreLevel=kWarning;

RooPlot* ContourPlot(RooRealVar* var1,RooRealVar* var2, RooFitResult* r);
TTree* weightTree(TTree* tree, TH1D* h1_alpha,const std::string& name ,bool verbose=false);
void fitPseudoOnePar( RooDataSet& ModSideband, RooWorkspace& ws,int seed,char* initialvalues, int nxj,std::string inPurStr);
void fitPseudoTwoPars( RooDataSet& ModSideband, RooWorkspace& ws,int seed,char* initialvalues, int nxj);
void pseudoMassgeOnePar(int nxj ,std::string inPurStr, RooFitResult* r_nominal, RooWorkspace& ws,char* initialvalues, double NormRelErr, RooRealVar &errV1);
void pseudoMassgeTwoPars(int nxj , RooFitResult* r_nominal, RooWorkspace& ws,char* initialvalues, double NormRelErr, RooRealVar &errV1, RooRealVar &errV2);
void CopyTreeVecToPlain(TTree *t1, std::string wType, std::string f2Name,std::string t2Name,int nxjCut=-1);

//#############EDIT THIS PART#########################
/*
const string inDirSig="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1d/fullsig/";
const string inDir="/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1d/fullsb/";
<<<<<<< fitBackground.cpp
const string outDir="FitSidebandsMJJ_ZZ_20130418_TEST2/";
=======
const string outDir="FitSidebandsMJJ_ZZ_20130418_VVMC/";
*/

const std::string outDir="FitSidebandsMJJ_CA8_WW_V11/";
const string inDirSig="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/AnaSigTree_from50_noConv/";
const string inDir ="/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/AnaSBTree_from50_noConv/";
>>>>>>> 1.25

bool isZZChannel=false;//only changes the file list
string InTreeName = "SelectedCandidates";
double startFit = 1000.0;
int jetCats =1; // 1 for only 1jet case and 2 for both
const string leptType="ALL";//"ALL" //"MU" //"ELE"
const bool doPseudoExp=false; //if true, for for different psuedo-alpha 
const unsigned int nToys = 500;
const bool unblind=false;//default is not to plot the data in signal region
const bool decorrLevExpo=true;
const float lumi =19538.85;
const bool useAlphaVV=false;//to sync with fitSidebands
//binning for merged Jet topology 

const int nBins1=22;
const double bins1[nBins1]={480,500,520,560,600,640,680,720,760,800,840,920,
	1000,1100,1200,1300,1500,1700,1900,2100,2300,2600};

//binning for double Jet topology 
const int nBins2=16;
const double bins2[nBins2]={480,500,520,560,600,640,680,720,760,800,840,920,
	1000,1100,1250,1400};//,1700,2000,2300,2600};



//#######################################################################################


ofstream TestfitBkgLog((outDir+"/TestfitBkgLog.txt").c_str());

int main(){
	RooMsgService::instance().setGlobalKillBelow(RooFit::INFO) ;
	//DEBUG=0, INFO=1, PROGRESS=2, WARNING=3, ERROR=4, FATAL=5
	ofstream logf((outDir+"./log_fitBackground_"+leptType+".log").c_str(),ios::out);

	TChain* chainData = new TChain(InTreeName.c_str());
	if(isZZChannel)
	{
		chainData->Add( (inDir+"treeEDBR_DoubleMu_Run2012*.root").c_str()  );
		chainData->Add( (inDir+"treeEDBR_DoublePhoton*_Run2012*.root").c_str()  );
		chainData->Add( (inDir+"treeEDBR_Photon_Run2012*.root").c_str()  );
	}
	else
	{
		chainData->Add( (inDir+"treeEDBR_data_xww.root").c_str()  );
	}

	logf<<"In the data chain there are "<<chainData->GetEntries()<<" events"<<endl;

	TChain* chainDataSig = new TChain(InTreeName.c_str());
	if(isZZChannel)
	{
		chainDataSig->Add( (inDirSig+"treeEDBR_DoubleMu_Run2012*.root").c_str()  );
		chainDataSig->Add( (inDirSig+"treeEDBR_DoublePhoton*_Run2012*.root").c_str()  );
		chainDataSig->Add( (inDirSig+"treeEDBR_Photon_Run2012*.root").c_str()  );
	}
	else
	{
		chainDataSig->Add( (inDirSig+"treeEDBR_data_xww.root").c_str()  );
	}


	//write in a plain tree because RooFit does not like trees with branches storing vectors 
	const int nxjCut=-1;//if negative: no cut
	const std::string tmpTreeName="SelectedCandidatesV2";
	char foutn[64];
	if(nxjCut>=0)  sprintf(foutn,"EXOVVTree_DATASB_%d.root",nxjCut);
	else   sprintf(foutn,"EXOVVTree_DATASB_NOcut.root");
	std::string tmpFileName=foutn;
	std::string weighting = "weight";
	CopyTreeVecToPlain(chainData,weighting,tmpFileName,tmpTreeName,nxjCut);//(TTree*)
	delete chainData;

	const std::string tmpSigTreeName="SelectedCandidatesSIG";
	char foutSig[64];
	if(nxjCut>=0)  sprintf(foutSig,"EXOVVTree_DATASIG_%d.root",nxjCut);
	else   sprintf(foutSig,"EXOVVTree_DATASIG_NOcut.root");
	std::string tmpSigFileName=foutSig;
	CopyTreeVecToPlain(chainDataSig,weighting,tmpSigFileName,tmpSigTreeName,nxjCut);//(TTree*)
	delete chainDataSig;

	TFile *ftree=new TFile(foutn,"READ");
	TTree *treeDATA_tmp=(TTree*)ftree->Get(tmpTreeName.c_str());

	TFile *ftreeSig=new TFile(foutSig,"READ");
	TTree *treeDATA_sig=(TTree*)ftreeSig->Get(tmpSigTreeName.c_str());

	//get SBVV 
	TFile *ftreeSBVV=new TFile("EXOVVTree_MCVV_SB_NOcut.root","READ");
	TTree *treeSBVV=(TTree*)ftreeSBVV->Get(tmpTreeName.c_str());

	TFile *ftreeVV=0;
	TTree *treeVV_sig=0;

	if(!useAlphaVV){
		char foutVV[64];
		if(nxjCut>=0)  sprintf(foutVV,"EXOVVTree_MCVV_SIG_%dJ.root",nxjCut);
		else   sprintf(foutVV,"EXOVVTree_MCVV_SIG_NOcut.root");
		//	  std::string tmpVVFileName=foutVV;
		ftreeVV=new TFile(foutVV,"READ");
		treeVV_sig=(TTree*)ftreeVV->Get(tmpTreeName.c_str());

	}//end if not useAlphaVV

	// gROOT->cd(); //magic!

	const double minMZZ=bins1[0];
	const double maxMZZ=bins1[nBins1-1];
	int inxj=0;
	RooRealVar* mZZ = new RooRealVar("mZZ","mZZ",minMZZ,maxMZZ);//bins[nBins-1]);
	RooRealVar *nXjets=new RooRealVar("nXjets","nXjets",-0.1,2.1);
	RooRealVar *mJJ=new RooRealVar("mJJ","mJJ",50.0,150.0);
	RooRealVar *lep=new RooRealVar("lep","lep",0.0,1.0);
	RooRealVar *region=new RooRealVar("region","region",-0.1,1.1);
	RooRealVar *vTagPurity=new RooRealVar("vTagPurity","vTagPurity",-5.0,5.0);
	RooRealVar *alphaWeight = new RooRealVar("alphaWeight","alphaWeight",1.,0.,100000.);//(RooRealVar*) dsDataSB->addColumn(*alpha_formula) ;
	RooRealVar *mcweight    = new RooRealVar("weight","mcWeight",1.,0.,100000.);//(RooRealVar*) dsDataSB->addColu	//add roorealvar for purity

	for( inxj=1;inxj<=jetCats;inxj++){

		int nPurities=1;
		if(inxj==1)nPurities=2;

		double purityCut=-1;
		for(int iP=0;iP<nPurities;iP++){//loop over purity categories
			if(inxj==1)purityCut=iP;//for 2J category, no cut on Purity

			int nBins;
			const double* bins=0;

			if(inxj==2){
				nBins = nBins2;
				bins = bins2;
				mZZ->setMin(bins[0]);
				mZZ->setMax(bins[nBins-1]);
			}
			else if(inxj==1){
				nBins = nBins1;
				bins = bins1;
			}
			else{
				std::cout<<"Value of nxj not allowed: "<<inxj<<" . Skipping to the next."<<std::endl;
				continue;
			}

			std::stringstream ssnxj;
			ssnxj << inxj;

			std::string pur_str="";
			if(purityCut==0)pur_str="LP";
			if(purityCut==1)pur_str="HP";

			//now trying to get R0
			double lepCut=-1;
			if(leptType=="ELE")lepCut=0;
			if(leptType=="MU")lepCut=1;
			if(leptType=="ALL")lepCut=-1; 

			TString lepTS = Form ("(lep==%.0f)",lepCut );
			if(lepCut==-1)lepTS="1";
			TString nXjetsTS = Form ("(nXjets==%d)",inxj );
			TString vTagPurityTS = Form ("(vTagPurity==%.0f)",purityCut );
			if(purityCut==-1) vTagPurityTS="1";
			TString totalWeight = Form("weight*%f",lumi);
			TString Cut = lepTS+"*"+nXjetsTS+"*"+vTagPurityTS;
			TestfitBkgLog<<Cut<<endl;

			TCanvas * cr0 = new TCanvas();
			TH1D * mZZSBVV = new TH1D ("mZZSBVV","mZZSBVV",nBins-1,bins);
			TH1D * mZZSBData = new TH1D ("mZZSBData","mZZSBData",nBins-1,bins);
			treeSBVV->Draw("mZZ>>mZZSBVV",Cut+"*"+totalWeight);
			treeDATA_tmp->Draw("mZZ>>mZZSBData",Cut);
			TH1D * R0 = (TH1D *) mZZSBVV->Clone("R0");
			R0->Reset();
			R0->Divide(mZZSBVV,mZZSBData);
			R0->SetTitle("R0");
			R0->Draw();
			cr0->SaveAs((outDir+"/R0_"+ssnxj.str()+"J_"+pur_str+"_"+leptType+".png").c_str());
			cr0->SaveAs((outDir+"/R0_"+ssnxj.str()+"J_"+pur_str+"_"+leptType+".root").c_str());
			//got R0

			string alphaFileName=outDir+"/Workspaces_alpha_"+ssnxj.str()+"J_"+pur_str+"_"+leptType+".root";
			logf<<"\n\n\n\n****** NEW NXJ = "<<inxj<<" "<< pur_str.c_str()<<" ---> "<<(alphaFileName).c_str() <<std::endl; 
			std::cout<<"\n\n\n\n****** NEW NXJ = "<<inxj<<" "<< pur_str.c_str()<<"  ---> "<<(alphaFileName).c_str() <<std::endl; 
			TFile *falpha=new TFile( (alphaFileName).c_str() );
			char alphahname[50];
			//    sprintf(alphahname,"nominal_alpha_%dnxj",inxj);//histo with fit to alpha
			sprintf(alphahname,"h_alpha_smoothened");
<<<<<<< fitBackground.cpp
			//read original alpha
			TH1D * halphaORIG = (TH1D*)falpha->Get(alphahname);
			//create histo with (1-r0)
			//multuply bin-by-bin the two
		       	halphaORIG->Reset();
			TTree* weightedData = weightTree(treeDATA_tmp ,halphaORIG,"alphaWeightedTree" );
=======

			TH1D * alpha_ORI = (TH1D*)falpha->Get(alphahname);	
			TH1D * alpha_Final = (TH1D*)alpha_ORI->Clone("h_alpha_smoothened_Final");

			if(!useAlphaVV)
			{
				alpha_Final->Reset();

				// alpha_Final = (1-R0) * alpha_ORI

				for (int iBin = 1 ; iBin <= alpha_Final->GetNbinsX() ; ++iBin)
				{
					double alpha_ORI_c = alpha_ORI->GetBinContent(iBin);
					double alpha_ORI_e = alpha_ORI->GetBinError(iBin);
					double R0_c = R0->GetBinContent(iBin);
					double R0_e = R0->GetBinError(iBin);
					double alpha_Final_c = (1-R0_c)*alpha_ORI_c;
					double alpha_Final_e = sqrt(alpha_ORI_c*alpha_ORI_c*R0_e*R0_e+(1-R0_c)*(1-R0_c)*alpha_ORI_e*alpha_ORI_e);
					alpha_Final->SetBinContent(iBin,alpha_Final_c);
					alpha_Final->SetBinError(iBin,alpha_Final_e);
				}
			}

			TTree* weightedData = weightTree(treeDATA_tmp , alpha_Final  ,"alphaWeightedTree" );
>>>>>>> 1.25

			//stat uncertainty on alpha normalization

			double area_MCsig=( (TH1D*)falpha->Get("mX_signalRegion"))->Integral();
			double area_MCsb=( (TH1D*)falpha->Get("mX_sidebands"))->Integral();
			double nent_MCsig=( (TH1D*)falpha->Get("mX_signalRegion"))->GetEntries();
			double nent_MCsb=( (TH1D*)falpha->Get("mX_sidebands"))->GetEntries();
			double totAlpha=area_MCsig/area_MCsb;

			//this is if you have another alpha coming from a different model
			//(for example, Sherpa instead of MAdgraph)
			//    TFile *falphaAlt=new TFile( (outDir+"/Workspaces_alpha_"+ssnxj.str()+"J_"+leptType+".root").c_str() );
			// double area_MCsigAlt=( (TH1D*)falphaAlt->Get("mX_signalRegion"))->Integral();
			// double area_MCsbAlt=( (TH1D*)falphaAlt->Get("mX_sidebands"))->Integral();
			//double totAlphaAlt=area_MCsigAlt/area_MCsbAlt;
			// double alphaErrMod= (totAlpha-totAlphaAlt);
			double alphaErrMod= 0.0;
			double alphaErrtmp= totAlpha*sqrt(1.0/nent_MCsig +1.0/nent_MCsb) ;  
			double alphaErrTot= sqrt(alphaErrtmp*alphaErrtmp + alphaErrMod*alphaErrMod);//absolute error
			double alphaFitErr= 0.0;//((TF1*)( (TH1D*)falpha->Get(alphahname))->GetFunction("alpha_fitfunc"))->GetParError(0);//wow !
			logf<<"MC total statistics: SIG="<<nent_MCsig<<"  SB="<<nent_MCsb<<"  alphaERR="<<alphaErrtmp<<" (from fit: "<<alphaFitErr<<")"<<endl;
			RooRealVar* alphaErr=new RooRealVar("alphaNormErr","alphaNormErr (relative)", alphaErrTot/totAlpha);

			////////    double minMZZ=bins[0];
			stringstream strmcut;
			strmcut<<minMZZ;
			//read in the file with the alpha fit function
			TF1 *alpha_func=(TF1*)falpha->Get("alpha_fitfunc");
			RooWorkspace *ws = (RooWorkspace*)falpha->Get( ("ws_alpha_"+ssnxj.str()+"J_"+pur_str+"_"+leptType).c_str() );
			ws->Print();

			RooPolynomial *ws_alpha_pol=(RooPolynomial*)ws->pdf("pol0_func");
			RooRealVar *c0=(RooRealVar*)ws->var("c0");
			RooPolynomial *pol0_new=new RooPolynomial("newpol0_func","Polynomial 0th (constant) for fit",*mZZ,*c0,0);
			RooFormulaVar *alpha_formula=new RooFormulaVar("alpha_formula","alpha_formula","@0",RooArgList(*c0));

			string lepCutStr="";
			if(leptType=="ELE")lepCutStr=" &&lep==0";
			if(leptType=="MU")lepCutStr=" &&lep==1";

			stringstream strpurcut;
			strpurcut<<purityCut;
			std::string vtagcutstr;
			if(purityCut<0)vtagcutstr="";
			else vtagcutstr=" &&vTagPurity=="+strpurcut.str();

			//select the data in the sidebands, convert it in a RooDataSet
			//weight events by the alpha function
			string cutSB= "nXjets=="+ssnxj.str()+" &&region==0.0 &&mZZ>"+strmcut.str()+lepCutStr+vtagcutstr;
			string cutSIG="nXjets=="+ssnxj.str()+" &&region==1.0 &&mZZ>"+ strmcut.str()+lepCutStr+vtagcutstr;
			string cutDum="nXjets=="+ssnxj.str()+" && mZZ>"+ strmcut.str()+lepCutStr+vtagcutstr;


			logf<<"Applying the following cuts: "<<cutSIG.c_str()<<std::endl;
			RooDataSet *dsDataSB=new RooDataSet("dsDataSB","dsDataSB",(TTree*)treeDATA_tmp,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep,*vTagPurity),cutSB.c_str()) ;
			//real data; weight each event by the alpha factor in order to scale to signal region

			logf<<"\n================================="<<std::endl;
			mZZ->setVal(600.0);
			logf<<"===> MZZ="<<mZZ->getVal()<<"    ALPHA (fromFIT)="<<alphaWeight->getVal()<<std::endl;
			mZZ->setVal(1200.0);
			logf<<"===> MZZ="<<mZZ->getVal()<<"    ALPHA (fromFIT)="<<alphaWeight->getVal()<<std::endl;
			mZZ->setVal(1900.0);
			logf<<"===> MZZ="<<mZZ->getVal()<<"    ALPHA (fromFIT)="<<alphaWeight->getVal()<<std::endl;
			logf<<"=================================\n"<<std::endl;

			std::string name_datasb2="dsDataSB2";//"_"+ssnxj.str()+"J"+pur_str;
			RooDataSet *dsDataSB2=new RooDataSet(name_datasb2.c_str(),name_datasb2.c_str(),weightedData,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep,*vTagPurity,*alphaWeight),cutSB.c_str(),"alphaWeight") ;
			std::string name_datasig="dsDataSIG";//"_"+ssnxj.str()+"J"+pur_str;
			RooDataSet *dsDataSIG=new RooDataSet(name_datasig.c_str(),name_datasig.c_str(),(TTree*)treeDATA_sig,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep,*vTagPurity),cutSIG.c_str()) ;//real data in signal region; cuts on mjj and nXjets
			logf<<"Number of events in OBSERVED datasets:"<<std::endl;
			logf<<dsDataSB->GetName()<<"  -> "<<dsDataSB->numEntries()<<"  "<<dsDataSB->sumEntries()<<std::endl;
			logf<<dsDataSB2->GetName()<<"  -> "<<dsDataSB2->numEntries()<<"  "<<dsDataSB2->sumEntries()<<std::endl;
			dsDataSB2->Print("v");

			RooDataSet *VVDataSetNoWeight = 0, *VVDataSetWeight = 0;
			if(!useAlphaVV){
				char vvweightstring[100];
				sprintf(vvweightstring,"weight*%f",lumi);
				RooFormulaVar weightVV("vvWeight",vvweightstring,*mcweight) ;

				VVDataSetNoWeight=new RooDataSet("VVDS","VVDS",treeVV_sig,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep,*vTagPurity,*mcweight),cutSIG.c_str()) ;

				RooRealVar* wVV = (RooRealVar*)VVDataSetNoWeight->addColumn(weightVV);			
				// VVDataSetNoWeight->Print("v");
				VVDataSetWeight = new RooDataSet("VVDSW","VVDS",VVDataSetNoWeight,*VVDataSetNoWeight->get(),0,wVV->GetName());
				//VVDataSetWeight->Print("v");
				dsDataSB2->append(*VVDataSetWeight);
				//dsDataSB2->Print("v");

				std::cout << "We just added the VV-MC to the extrapolated SB. Now snaity check plot." << std::endl;
				TCanvas *canX=new TCanvas("canvasX", "canX",800,800);
				canX->cd();

				RooPlot *mccontrol=mZZ->frame();
				dsDataSB2->plotOn(mccontrol,MarkerStyle(20),MarkerColor(kBlack));
				VVDataSetWeight->plotOn(mccontrol,MarkerStyle(20),MarkerColor(kRed));
				mccontrol->Draw();
				canX->SaveAs((outDir+"/mccontrol_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+".png").c_str());
				mccontrol->SetMinimum(0.00006);
				gPad->SetLogy();
				mccontrol->Draw();
				canX->SaveAs((outDir+"/mccontrol_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_log.png").c_str());
				delete mccontrol;
				delete canX;
				cout<<"Finished to add the VV MC to the extrapolated SB."<<endl;
			}//end if not usealphavv

			delete VVDataSetNoWeight;
			if(unblind)logf<<dsDataSIG->GetName()<<"  -> "<<dsDataSIG->numEntries()<<"  "<<dsDataSIG->sumEntries()<<std::endl;

			// dsDataSB->Print();

			if(dsDataSB2->numEntries()<5){
				cout<<"ERROR! Not enough stat in data sidebands ("<<dsDataSB2->numEntries()<<" entries). Quitting."<<endl;
				continue;
			}

			//fit the weighted dataset with a custom function
			double minFitRange=(inxj==1 ? startFit : 600);//minMZZ
			double maxFitRange=(inxj==1 ? maxMZZ : bins[nBins-1]);
			std::cout<<"STARTING TO FIT WITH CUSTOM FUNCTION in range [ "<<minFitRange<<" , "<<maxFitRange <<" ]"<<std::endl;
			logf<<"STARTING TO FIT WITH CUSTOM FUNCTION in range [ "<<minFitRange<<" , "<<maxFitRange <<" ]"<<std::endl;
			mZZ->setRange("fitRange",minFitRange,maxFitRange) ;

			double inislope;
			if(inxj==1)inislope=-0.25;
			if(inxj==2)inislope=-0.25;
			//	char slopePar_name[32];
			//	sprintf(slopePar_name,"a0_%dJ",inxj);
			std::string slopePar_name="a0_"+ssnxj.str()+"J"+pur_str;
			RooRealVar *a0=new RooRealVar(slopePar_name.c_str(),slopePar_name.c_str(),inislope,-10.0,0.0);
			RooExponential *expo_fit=new RooExponential("exp_fit","exp_fit",*mZZ,*a0);
			//  RooRealVar *exp_norm=new RooRealVar("exp_N","exp_N",0.0,10000.0);

			//  RooFitResult* r_sig = expo_fit->fitTo(*dsDataSB,Save(kTRUE),Range("fitRange")) ;
			RooFitResult* r_sig2 = expo_fit->fitTo(*dsDataSB2,Save(kTRUE),SumW2Error(kTRUE),RooFit::PrintLevel(-1),Range("fitRange")) ;
			char fitResultName_expo[200];
			sprintf( fitResultName_expo, "resultsExpoFit_%dJ_%s_%s",inxj ,pur_str.c_str(), leptType.c_str() );
			r_sig2->SetName(fitResultName_expo); 
			logf<<"\n\n\n\n\n#################\nCheck entries: "<<dsDataSB->sumEntries()<<"   "<<dsDataSB2->sumEntries()
				<<"##############\n\n\n\n\n"<<endl;
			RooRealVar *Nent=new RooRealVar("sidebandNormalization","Integral of data in sidebands before signal-rescaling",dsDataSB->numEntries(),0.0,50000.0);
			RooRealVar *NormErr=new RooRealVar("errNormDataSB","Statistical uncertainty on bkgd norm (relative)",1.0/sqrt(Nent->getVal()),0.0,50000.0);
			RooRealVar *Nerr=new RooRealVar("errNormalization","Total uncertainty on bkgd norm (relative)",sqrt(NormErr->getVal()*NormErr->getVal()+alphaErr->getVal()*alphaErr->getVal()) ,0.0,15.0);
			alphaErr->setConstant(kTRUE);
			NormErr->setConstant(kTRUE);
			Nerr->setConstant(kTRUE);
			RooRealVar *Nbkg=new RooRealVar("bkgdNormalizationFullRange",("Background normalization in ["+strmcut.str()+", maxMZZ]  (ELE+MU)").c_str(),dsDataSB2->sumEntries(),0.0,10000.0);
			RooRealVar *NbkgELE=new RooRealVar("bkgdNormalizationFullRangeELE",("Background normalization in ["+strmcut.str()+", maxMZZ]  (ELE)").c_str(),dsDataSB2->reduce("lep==0")->sumEntries(),0.0,10000.0);
			RooRealVar *NbkgMU=new RooRealVar("bkgdNormalizationFullRangeMU",("Background normalization in ["+strmcut.str()+", maxMZZ]  (MU)").c_str(),dsDataSB2->reduce("lep==1")->sumEntries(),0.0,10000.0);
			Nbkg->setConstant(kTRUE);
			RooRealVar *NbkgRange=new RooRealVar("bkgdNormalizationFitRange","Background normalization in range of fit (ELE+MU)",dsDataSB2->reduce(CutRange("fitRange"))->sumEntries(),0.0,10000.0);
			RooRealVar *NbkgRangeELE=new RooRealVar("bkgdNormalizationELE","Background normalization in range of fit (ELE)",dsDataSB2->reduce(CutRange("fitRange"))->reduce("lep==0")->sumEntries(),0.0,10000.0);
			RooRealVar *NbkgRangeMU=new RooRealVar("bkgdNormalizationMU","Background normalization in range of fit (MU)",dsDataSB2->reduce(CutRange("fitRange"))->reduce("lep==1")->sumEntries(),0.0,10000.0);
			NbkgRange->setConstant(kTRUE);
			// a0->setConstant(kTRUE);
			logf<<"Normalization in full range : ELE="<<NbkgELE->getVal()<<"  MU="<<NbkgMU->getVal()<<"  ALL="<<Nbkg->getVal()<<endl;
			logf<<"Normalization errors: Nent="<<Nent->getVal()<<" NormErr="<<NormErr->getVal()<<"  Nerr="<<Nerr->getVal()<<std::endl;
			logf<<"Exp fit done: Norm In Range = "<<NbkgRange->getVal()<<"   Slope="<<a0->getVal()<<std::endl;
			r_sig2->printMultiline(logf,99,true);
			logf<<"\nNow fitting with Leveled Expo:"<<endl;
			//for levelled exponential
			double initf0=90.0;
			if(inxj==1)initf0=200.0;
			if(inxj==2)initf0=200.0;
			double initf1=0.0;
			if(inxj==1)initf1=0.0;
			if(inxj==2)initf1=0.0;
			double initm=560.0;
			if(inxj==2)initm=400.0;
			RooRealVar *f0=new RooRealVar("f0","sigma",initf0,0.0,300.0);
			RooRealVar *f1=new RooRealVar("f1","alpha",initf1,-0.5,2.0);
			RooRealVar *f1b=new RooRealVar("f1b","beta",initf1,-0.5,2.0);
			RooRealVar *f2=new RooRealVar("f2","m",initm,200.0,500.0);
			RooRealVar *f3=new RooRealVar("f3","theta",0.0);
			f2->setConstant(kTRUE);
			f3->setConstant(kTRUE);
			f1->setConstant(kTRUE);//if both f1 and f1b set constant to zero, levelled expo becomes a normal exponential
			//  RooGenericPdf *expLev_fit=new RooGenericPdf("levelled_exp_fit","exp(-1.0*(mZZ-f2)/(f0 + f1*(mZZ-f2)))",RooArgList(*mZZ,*f0,*f1,*f2));
			RooLevelledExp2 *expLev_fit=new RooLevelledExp2("levelled_exp_fit","levelled_exp_fit",*mZZ,*f0,*f1,*f1b,*f2,*f3);
			RooFitResult* r_sig_expLev = expLev_fit->fitTo(*dsDataSB2,Save(kTRUE),SumW2Error(kTRUE),RooFit::PrintLevel(-1),Range("fitRange")) ;//,Range("fitRange"),SumW2Error(kTRUE)

			logf<<"LevExpo fit done: Sigma = "<<f0->getVal()<<"   alpha="<<f1->getVal()<<"   beta="<<f1b->getVal()<<"   m="<<f2->getVal()<<"  Theta="<<f3->getVal()<<std::endl;
			logf<<"LEVELLED EXPO FIT PERFORMED ! DECORRELATING PARAMETERS."<<std::endl;

			//now decorrelate parameters:
			RooWorkspace *wstmp=new RooWorkspace("tempWorkSpace","tempWorkSpac");
			RooAbsPdf  *background_decorr_=0;
			RooFitResult *r_sig_expLev_decorr =0;
			char fitResultName_eig[200];
			sprintf( fitResultName_eig, "DUMMYExpLevelledFit_%dJ_%s_decorr",inxj , leptType.c_str() );

			if(decorrLevExpo){
				char diagonalizerName[200];
				sprintf( diagonalizerName, "expLev_%db", inxj);
				PdfDiagonalizer diago(diagonalizerName, wstmp, *r_sig_expLev );
				background_decorr_ =diago.diagonalize(*expLev_fit);//RooAbsPdf
				background_decorr_->SetName("levexp_dcr");
				char var1[50],var2[50];
				sprintf(var1,"expLev_%db_eig0",inxj);
				sprintf(var2,"expLev_%db_eig1",inxj);
				RooRealVar *f0rot=(RooRealVar*)wstmp->var(var1);
				RooRealVar *f1rot=(RooRealVar*)wstmp->var(var2);
				//if only one free par in roolevexpo
				//RooRealVar *f1rot=0;
				// f1rot=new RooRealVar(*f1); f1rot->SetName(var2);


				//refit to get a new covariance matrix to check that this has worked
				r_sig_expLev_decorr = background_decorr_->fitTo(*dsDataSB2, Save(),SumW2Error(kTRUE),RooFit::PrintLevel(-1),Range("fitRange"));
				//    char fitResultName_eig[200];
				sprintf( fitResultName_eig, "resultsExpLevelledFit_%dJ_%s_%s_decorr",inxj ,pur_str.c_str(), leptType.c_str() );
				r_sig_expLev_decorr->SetName(fitResultName_eig); 
				logf<<"Sigma= "<<f0->getVal()<<" -> SigmaDecorr="<<f0rot->getVal()<<std::endl;
				logf<<"Alphaa= "<<f1b->getVal()<<" -> AlphaDecorr="<<f1rot->getVal()<<std::endl;
				logf<<"m= "<<f2->getVal()<<" Theta="<<f3->getVal()<<std::endl;
				sprintf( fitResultName_eig, "%s/resultsExpLevelledFit_%dJ_%s_%s_decorr.txt",outDir.c_str(),inxj ,pur_str.c_str(), leptType.c_str() );
				RooArgSet rotvars( *f0rot,*f1rot );
				rotvars.writeToFile(fitResultName_eig);

				//plot the correlated/decorrelated uncertainties
				bool plotDecorr=true;
				if(plotDecorr){
					TCanvas* c_rot = new TCanvas("canvas_rot", "CANVAS PARS ROT", 800, 800);
					c_rot->cd();
					double f0val=f0->getVal();
					double f1val=f1b->getVal();
					double f0err=f0->getError();
					double f1err=f1b->getError();
					Double_t rho= r_sig_expLev->correlation(f0->GetName(),f1b->GetName());
					RooEllipse *contour= new RooEllipse("contour",f0val,f1val,f0err,f1err,rho,500);
					contour->SetLineWidth(2) ;
					RooPlot *plot = new RooPlot(*f0,*f1b,f0val-2*f0err,f0val+2*f0err,f1val-2*f1err,f1val+2*f1err);
					plot->addPlotable(contour);
					plot->Draw();
					c_rot->SaveAs((outDir+"/checkRotation_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+".eps").c_str());
					c_rot->SaveAs((outDir+"/checkRotation_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+".png").c_str());

					f0val=f0rot->getVal();
					f1val=f1rot->getVal();
					f0err=f0rot->getError();
					f1err=f1rot->getError();
					rho= r_sig_expLev_decorr->correlation(f0rot->GetName(),f1rot->GetName());
					RooEllipse *contourRot= new RooEllipse("contourRot",f0val,f1val,f0err,f1err,rho,500);
					contourRot->SetLineWidth(2) ;
					contourRot->SetLineColor(kRed) ;
					RooPlot *plotRot = new RooPlot(*f0rot,*f1rot,f0val-2*f0err,f0val+2*f0err,f1val-2*f1err,f1val+2*f1err);
					plotRot->addPlotable(contourRot);
					plotRot->Draw();
					// ContourPlot(f0,f1,r_sig_expLev);
					c_rot->SaveAs((outDir+"/checkRotation_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_decorrelated.eps").c_str());
					c_rot->SaveAs((outDir+"/checkRotation_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_decorrelated.png").c_str());

					delete contour;
					delete c_rot;
				}//end plot Decorr

			}//edn if decorrLevExpo

			//save everything in a RooWorkspace
			TFile *fout=new TFile((outDir+"/workspace_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_new.root").c_str(),"RECREATE");
			RooWorkspace *wsnew=new RooWorkspace(*ws);
			wsnew->SetName(("ws_alpha_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType).c_str());
			logf<<"\n\nName of new WS: "<<wsnew->GetName()<<std::endl;
			wsnew->import(*Nbkg);
			wsnew->import(*NbkgELE);
			wsnew->import(*NbkgMU);
			wsnew->import(*NbkgRange);
			wsnew->import(*NbkgRangeELE);
			wsnew->import(*NbkgRangeMU);
			wsnew->import(*Nent);
			wsnew->import(*alphaErr);
			wsnew->import(*NormErr);
			wsnew->import(*Nerr);
			wsnew->import(*expo_fit);
			wsnew->import(*expLev_fit);
			wsnew->import(*r_sig_expLev);
			wsnew->import(*r_sig2);
			wsnew->import(*dsDataSB);
			wsnew->import(*dsDataSB2);
			wsnew->import(*dsDataSIG);
			if(decorrLevExpo){
				wsnew->import(*background_decorr_, RooFit::RecycleConflictNodes()); 
				wsnew->import(*r_sig_expLev_decorr);
				r_sig_expLev_decorr->Write();
			}
			logf<<"Everything imported. Saving."<<std::endl;
			r_sig2->Write();
			weightedData->Write();


			if(doPseudoExp){
				char tmpTname[32];
				//do pseudo-experiments for alpha-error
				for( unsigned n=0 ;n<nToys;n++){
					//get alternative weights
					if(n%50==0)std::cout<<"Throwing toy #"<<n<<"/"<<nToys<<" "<<std::endl;
					sprintf(alphahname,"tmp_%d",n);
					sprintf(tmpTname,"pseudoTree_%s",alphahname);     
					TTree* weightedPseudo = weightTree(treeDATA_tmp ,(TH1D*)falpha->Get(alphahname), tmpTname);
					RooDataSet pseudoSB2("pseudoSB2","pseudoSB2",weightedPseudo,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep,*alphaWeight,*vTagPurity),cutSB.c_str(),"alphaWeight") ;
					pseudoSB2.append(*VVDataSetWeight);
					if(decorrLevExpo) fitPseudoTwoPars(pseudoSB2,*wsnew,n,fitResultName_expo,inxj);
					else      fitPseudoOnePar(pseudoSB2,*wsnew,n,fitResultName_expo,inxj,pur_str);
					//      cout<<"Deleting tree #"<<n<<"  "<<weightedPseudo->GetName()<<endl;
					delete weightedPseudo;
				}
				char var1errA[50];
				sprintf(var1errA,"expoFit_%dJ_alphaErr",inxj);
				char var2errA[50];
				sprintf(var2errA,"DUMMY_%dJ_alphaErr",inxj);
				if(decorrLevExpo){
					sprintf(var1errA,"expLev_%db_eig0_alphaErr",inxj);
					sprintf(var2errA,"expLev_%db_eig1_alphaErr",inxj);
				}

				RooRealVar errV1(var1errA,var1errA,0.0);
				RooRealVar errV2(var2errA,var2errA,0.0);
				std::cout<<"Starting pseudoMassge"<<std::endl;
				if(decorrLevExpo) pseudoMassgeTwoPars( inxj ,r_sig_expLev,*wsnew,fitResultName_eig, Nerr->getVal(), errV1, errV2);
				else  pseudoMassgeOnePar( inxj,pur_str  ,r_sig2,*wsnew,fitResultName_expo, Nerr->getVal(), errV1);

				wsnew->import(errV1);
				if(decorrLevExpo) wsnew->import(errV2);
			}//end if doPseudoExp

			fout->cd();
			wsnew->Write();

			//draw and save plots
			std::cout<<"Drawing"<<std::endl;

			TCanvas *can1=new TCanvas("canvas1", "can1",800,800);
			can1->cd();
			RooPlot *xf=mZZ->frame();
			string frameTitle="Bkgd Estimation from Data Sidebands ("+ssnxj.str()+"J "+pur_str +" - "+leptType+" lept flav)";
			xf->SetTitle(frameTitle.c_str());
			//DO NOT CHANGE THE ORDER !!!!!!! DATA AS FIRST ONES !!!!
			dsDataSB2->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(21),MarkerColor(kRed));
			if(unblind)dsDataSIG->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(20),MarkerColor(kBlack));

			//plot error bands of fit
			// background_decorr_->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"),RooFit::Range("fitRange"), LineColor(kRed));
			//background_decorr_->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent), LineColor(kViolet-2),VisualizeError(*r_sig_expLev_decorr,2.0,kFALSE),FillColor(kYellow),RooFit::NormRange("fitRange"),RooFit::Range("fitRange"));
			// background_decorr_->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent), LineColor(kViolet-2),VisualizeError(*r_sig_expLev_decorr,1.0,kFALSE),FillColor(kGreen),RooFit::NormRange("fitRange"),RooFit::Range("fitRange"));
			expo_fit->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent), LineColor(kViolet-2),VisualizeError(*r_sig2,2.0,kFALSE),FillColor(kYellow),RooFit::NormRange("fitRange"),RooFit::Range("fitRange"));
			expo_fit->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent), LineColor(kViolet-2),VisualizeError(*r_sig2,1.0,kFALSE),FillColor(kGreen),RooFit::NormRange("fitRange"),RooFit::Range("fitRange"));

			//plot fits
			expLev_fit->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), LineColor(kOrange+5), LineStyle(kDashed));//RooAbsPdf::NumEvent

			expo_fit->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), LineColor(kBlue), LineStyle(kSolid));
			expo_fit->plotOn(xf, Normalization(NbkgRange->getVal()+Nerr->getVal()*NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), LineColor(kBlue), LineStyle(kDashed));
			expo_fit->plotOn(xf, Normalization(NbkgRange->getVal()-Nerr->getVal()*NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), LineColor(kBlue), LineStyle(kDashed));


			//plot fit with decorrelated parameters (should superimpose the previous)
			// background_decorr_->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"),RooFit::Range("fitRange"), LineColor(kViolet-2), LineStyle(kDashed));
			//    background_decorr_->plotOn(xf, Normalization((NbkgRange->getVal()+Nerr->getVal()*NbkgRange->getVal()),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"),RooFit::Range("fitRange"), LineColor(kViolet-2), LineStyle(kDashed));
			//  background_decorr_->plotOn(xf, Normalization((NbkgRange->getVal()-Nerr->getVal()*NbkgRange->getVal()),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"),RooFit::Range("fitRange"), LineColor(kViolet-2), LineStyle(kDashed));

			dsDataSB2->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(21),MarkerColor(kRed));//,Normalization(dsDataSB2->numEntries(),RooAbsPdf::NumEvent)
			if(unblind)dsDataSIG->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(20),MarkerColor(kBlack));
			logf<<"Check nromalization: NumEntries of dsDataSIG= "<<dsDataSIG->numEntries() <<"("<<dsDataSIG->sumEntries() <<")    SumEntries of dsDataSB2="<<dsDataSB2->sumEntries()<<"   numEntries="<<dsDataSB2->numEntries()<<"    Nbkg (NORM)="<<NbkgRange->getVal()<<"   Nent="<<Nent->getVal()<<"     Nerr="<<Nerr->getVal() <<std::endl;

			xf->Draw();
			can1->SaveAs((outDir+"/fitPlot_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+".root").c_str());
			can1->SaveAs((outDir+"/fitPlot_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+".eps").c_str());
			can1->SaveAs((outDir+"/fitPlot_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+".png").c_str());
			can1->SaveAs((outDir+"/fitPlot_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+".pdf").c_str());
			xf->SetMinimum(0.06);
			gPad->SetLogy();
			xf->Draw();
			can1->SaveAs((outDir+"/fitPlot_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_log.root").c_str());
			can1->SaveAs((outDir+"/fitPlot_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_log.eps").c_str());
			can1->SaveAs((outDir+"/fitPlot_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_log.png").c_str());
			can1->SaveAs((outDir+"/fitPlot_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_log.pdf").c_str());
			delete xf;

			//don't change this order, for God's sake !
			delete expLev_fit;delete r_sig_expLev;delete r_sig_expLev_decorr;

			delete dsDataSB;delete dsDataSB2;delete dsDataSIG;
			//delete VVDataSetNoWeight;
			delete VVDataSetWeight;
			delete wsnew;// delete wstmp;
			delete ws;
			delete fout;
			// if(inxj==2) delete falpha;

		}//end loop on purity categories

	}//end loop on nxj
	logf.close();



}//end main



RooPlot* ContourPlot(RooRealVar* var1,RooRealVar* var2, RooFitResult* r){

	Double_t x1= var1->getVal();
	Double_t x2= var2->getVal();
	Double_t s1= var1->getError();
	Double_t s2= var2->getError();
	Double_t rho= r->correlation(var1->GetName(),var2->GetName());

	RooEllipse *contour= new RooEllipse("contour",x1,x2,s1,s2,rho,500);
	contour->SetLineWidth(2) ;


	RooPlot *plot = new RooPlot(*var1,*var2,x1-2*s1,x1+2*s1,x2-2*s2,x2+2*s2);
	plot->addPlotable(contour);

	r->plotOn(plot,*var1,*var2,"M12");

	plot->Draw();
	return plot;

}

TTree* weightTree(TTree* tree, TH1D* h1_alpha,const std::string& name ,bool verbose){
	gROOT->cd(); //magic!


	Double_t mZZ;
	tree->SetBranchAddress( "mZZ", &mZZ );

	TTree* newTree = tree->CloneTree(0);
	newTree->SetName(name.c_str());

	Double_t alpha;
	newTree->Branch( "alphaWeight", &alpha, "alphaWeight/D" );

	unsigned nentries = tree->GetEntries();
	cout<<"Starting to loop over tree inside weightTree. The alpha histo has "<<h1_alpha->GetNbinsX()<<" bins"<<endl;
	for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

		tree->GetEntry( iEntry );
		if( (iEntry % 10000)==0 && verbose) std::cout << "weightTree: Entry " << iEntry << "/" << nentries << std::endl;

		int alphabin = h1_alpha->FindBin( mZZ );
		if(alphabin==0 ||alphabin> h1_alpha->GetNbinsX())
			alpha=1.;
		else
			alpha = h1_alpha->GetBinContent( alphabin );

		//	alpha=999.0;
		newTree->Fill();

	}
	cout<<"Finished to llop over entries insied  weightTree."<<endl;
	//delete tree;
	return newTree;

}
void fitPseudoOnePar( RooDataSet& ModSideband, RooWorkspace& ws ,int seed,char* initialvalues , int nxj,std::string inPurStr) {
	//reset parameters
	char var1[50];
	sprintf(var1,"a0_%dJ%s",nxj,inPurStr.c_str());
	char argname[100];
	sprintf(argname,"%s",var1);
	ws.argSet(argname).readFromFile(initialvalues);
	RooFitResult* r_pseudo = ws.pdf("exp_fit")->fitTo(ModSideband, SumW2Error(kTRUE), Save(),RooFit::PrintLevel(-1),Range("fitRange") );

	char indexstring[200];
	sprintf(indexstring,"TMPResult_%dtag_%d",nxj,seed);
	RooArgSet tmpset(r_pseudo->floatParsFinal());
	tmpset.writeToFile(indexstring);

}


void fitPseudoTwoPars( RooDataSet& ModSideband, RooWorkspace& ws ,int seed,char* initialvalues , int nxj) {
	//reset parameters
	char var1[50];
	char var2[50];
	sprintf(var1,"expLev_%db_eig0",nxj);
	sprintf(var2,"expLev_%db_eig1",nxj);
	char argname[100];
	//  if(nxj==2) sprintf(argname,"%s",var1);
	// else sprintf(argname,"%s,%s",var1,var2);
	sprintf(argname,"%s,%s",var1,var2);
	ws.argSet(argname).readFromFile(initialvalues);
	RooFitResult* r_pseudo = ws.pdf("levexp_dcr")->fitTo(ModSideband, SumW2Error(kTRUE), Save(),RooFit::PrintLevel(-1),Range("fitRange") );

	char indexstring[200];
	sprintf(indexstring,"TMPResult_%dtag_%d",nxj,seed);
	RooArgSet tmpset(r_pseudo->floatParsFinal());
	tmpset.writeToFile(indexstring);

}

void pseudoMassgeOnePar(int nxj ,std::string inPurStr, RooFitResult* r_nominal, RooWorkspace& ws,char* initialvalues, double NormRelErr, RooRealVar &errV1){
	char var1[50];

	sprintf(var1,"a0_%dJ%s",nxj,inPurStr.c_str());//this must be equal to what is in fitPseudo
	char argname[100];
	sprintf(argname,"%s",var1);

	ws.argSet(argname).readFromFile(initialvalues);// read nominal best fit value
	double x1= ws.var(var1)->getVal();
	double e1= ws.var(var1)->getError();

	RooPlot *plot_MCbkg = ws.var("mZZ")->frame();

	std::vector<float> vals;
	std::vector<float> vals1;
	vals.reserve(nToys);
	vals1.reserve(nToys);


	char indexstring[200];

	bool plotEnvelope=true;

	cout<<"pseudoMassgeOnePar starts to loop over "<<nToys<<" toys"<<endl;

	for(unsigned i =0 ; i < nToys ; i++){
		sprintf(indexstring,"TMPResult_%dtag_%d",nxj,i);
		//  ifstream testFile(indexstring,ios::in);
		// if(! testFile.good())continue;
		ws.argSet(argname).readFromFile(indexstring);
		if(fabs(ws.var(var1)->getVal())>4.0 ){
			cout<<"Toy #"<<i<<" did not have successful fit "<< ws.var(var1)->getVal()<<endl;
			continue;
		}
		vals1.push_back(ws.var(var1)->getVal());

		if(plotEnvelope ){
			cout<<"Plot levexp_dcr #"<<i<<" with "<<var1<<"="<<ws.var(var1)->getVal()<<endl;
			ws.pdf("exp_fit")->plotOn(plot_MCbkg,LineWidth(1),LineColor(1));
			cout<<"Finished to Plot expo fit #"<<i<<endl;
			// char nameTmpFit[64];
			// sprintf(nameTmpFit,"levexp_dcr_%d",i);
			// ws.pdf("levexp_dcr")->plotOn(plot_MCbkg,LineWidth(1),LineColor(1));//,Name(nameTmpFit));
			// ws.pdf(nameTmpFit)->plotOn(plot_MCbkg,LineWidth(1),LineColor(1),Name(nameTmpFit));      
		}//end if plotenvelope

		std::string mkdir_command("rm ");  
		mkdir_command+= indexstring;
		//  system(mkdir_command.c_str());
		if(i%100==0)    std::cout << mkdir_command << std::endl;
	}
	cout<<"pseudoMassgeOnePar finished to loop over "<<nToys<<" toys"<<endl;


	char canvasName[400];
	sprintf( canvasName, "%s/mZZ_sidebandData_alphaVarWithToys_%dJ_%s", outDir.c_str(), nxj, "ALL");
	std::string canvasName_str(canvasName);
	std::string canvasName_eps = canvasName_str + ".eps";
	std::string canvasName_png = canvasName_str + ".png";
	TCanvas* c1 = new TCanvas("c1", "", 600, 600);
	c1->cd();


	if(plotEnvelope){
		RooCurve* upper = new RooCurve();
		RooCurve* lower = new RooCurve();
		upper->SetName("upper_staterr_alpha");
		lower->SetName("lower_staterr_alpha");
		float min = plot_MCbkg->GetXaxis()->GetXmin();
		float max = plot_MCbkg->GetXaxis()->GetXmax();
		float range=max-min;
		cout<<"Looping over points for envelope calculation: "<<flush;
		for(unsigned int x =0 ; x < 200 ; x++  ){
			float xval = min +x*range/200.;
			if(x%20==0)cout<<"Point #"<<x<<flush;
			for(unsigned i =0 ; i < nToys ; i++){
				vals.push_back(((RooCurve*)(plot_MCbkg->getObject(i)))->interpolate(xval));
				//   vals.push_back(0.0);
			}

			std::sort(vals.begin(),vals.end());
			lower->addPoint(xval,vals[floor(0.166*nToys)]);
			upper->addPoint(xval,vals[floor(0.832*nToys)]);
			vals.clear();
		}
		cout<<" Done."<<endl;


		lower->SetLineWidth(2);
		lower->SetLineColor(2);
		upper->SetLineWidth(2);
		upper->SetLineColor(2);

		cout<<"Plotting with pseudo experiments for "<<nxj<<"-J topology "<<endl;
		ws.pdf("exp_fit")->plotOn(plot_MCbkg,VisualizeError(*r_nominal,2.0,kFALSE),FillColor(kYellow));
		ws.pdf("exp_fit")->plotOn(plot_MCbkg,VisualizeError(*r_nominal,1.0,kFALSE),FillColor(kGreen));
		ws.pdf("exp_fit")->plotOn(plot_MCbkg);
		ws.pdf("exp_fit")->plotOn(plot_MCbkg, LineColor(kViolet-2), LineStyle(kDashed), Normalization(1.0+NormRelErr,RooAbsReal::Relative));
		ws.pdf("exp_fit")->plotOn(plot_MCbkg, LineColor(kViolet-2), LineStyle(kDashed), Normalization(1.0-NormRelErr,RooAbsReal::Relative));
		plot_MCbkg->addPlotable(lower);
		plot_MCbkg->addPlotable(upper);


		/*
		   double x0,up0,low0;
		   upper->GetPoint(0,x0,up0);  
		   lower->GetPoint(0,x0,low0);  
		   RooRealVar *r0=new RooRealVar("mZZ","mZZ",x0);
		   RooArgSet *tmparg=new RooArgSet(*r0);
		   double central0=1.0;//ws.pdf("exp_fit")->getValV();
		   RooRealVar normUp("norm_upper_staterr_alpha","norm_upper_staterr_alpha",up0/central0);
		   RooRealVar normDown("norm_lower_staterr_alpha","norm_lower_staterr_alpha",low0/central0);
		   std::cout<<"###Norm for UP/LOW: "<<x0<<" - "<< r0->getVal()<<" - "<<up0<<" - "<<low0<<" - "<<central0<<" - "<<normUp.getVal()<<std::endl;
		 */


		cout<<"Drawing plot_MCbkg"<<endl;
		plot_MCbkg->Draw(); 
		c1->SaveAs(canvasName_eps.c_str());
		c1->SaveAs(canvasName_png.c_str());

		plot_MCbkg->SetMinimum(0.0001);
		plot_MCbkg->SetMaximum(1.0);
		cout<<"Drawing plot_MCbkg (log-scale)"<<endl;
		plot_MCbkg->Draw();
		c1->SetLogy();

		canvasName_str += "_log";
		canvasName_eps = canvasName_str + ".eps";
		canvasName_png = canvasName_str + ".png";
		c1->SaveAs(canvasName_eps.c_str());
		c1->SaveAs(canvasName_png.c_str());

		char dumname[200];
		sprintf(dumname,"%s/rooCurves_%dJ_ALL.root",outDir.c_str(),nxj);
		cout<<"Drawing "<<dumname<<endl;
		TFile *fdumout=new TFile(dumname ,"RECREATE"); 
		char upname[50];
		char lowname[50];
		sprintf(upname,"roocurve_%dJ_UP",nxj);
		sprintf(lowname,"roocurve_%dJ_LOW",nxj);
		//sprintf(var2errA,"%s_alphaErr",var2);
		upper->Write();
		lower->Write();
		//   normUp.Write();
		// normDown.Write();
		//fdumout->Close();
		delete fdumout;

		delete plot_MCbkg;


	}//end if plotEnvelope


	c1->Clear();
	c1->SetLogy(false);

	// plotting value distributions
	std::sort(vals1.begin(),vals1.end());
	TH1F* histo1= new TH1F("test1","test1",100,vals1[0],vals1[nToys-1]);
	char tit[20];
	sprintf(tit,"#alpha_{%d}",nxj);
	histo1->GetXaxis()->SetTitle(tit);
	histo1->GetYaxis()->SetTitle("Nr trials");

	TLine* line = new TLine();
	line->SetLineColor(2);
	line->SetLineWidth(2);
	for(unsigned i =0 ; i <  nToys; i++){
		histo1->Fill(vals1[i]);
	}

	//   std::cout << "XXXX before fits" << std::endl;

	//   histo1->Print("v");
	//   histo2->Print("all");


	histo1->Fit("gaus");
	double s1 = histo1->GetFunction("gaus")->GetParameter("Sigma"); // we need this to update the errors 
	histo1->Draw();
	line->SetLineColor(2);
	line->DrawLine(x1,histo1->GetMinimum(),x1,histo1->GetMaximum());
	line->SetLineColor(4);
	line->DrawLine(x1+e1,histo1->GetMinimum(),x1+e1,histo1->GetMaximum());
	line->DrawLine(x1-e1,histo1->GetMinimum(),x1-e1,histo1->GetMaximum());


	sprintf( canvasName, "%s/alphaVar_par1_%dJ_%s", outDir.c_str(), nxj, "ALL");
	canvasName_str = canvasName;
	canvasName_eps = canvasName_str + ".eps";
	canvasName_png = canvasName_str + ".png";
	c1->SaveAs(canvasName_eps.c_str());
	c1->SaveAs(canvasName_png.c_str());

	errV1.setVal(s1);
	errV1.setConstant(kTRUE);

	std::cout<<"Finished pseudoMassgeOnePar ! Error value is "<<s1<<std::endl;
}//end pseudoMassgeOnePar


void pseudoMassgeTwoPars(int nxj , RooFitResult* r_nominal, RooWorkspace& ws,char* initialvalues, double NormRelErr, RooRealVar &errV1, RooRealVar &errV2){
	char var1[50];
	char var2[50];
	sprintf(var1,"expLev_%db_eig0",nxj);//this must be equal to what is in fitPseudo
	sprintf(var2,"expLev_%db_eig1",nxj);//this must be equal to what is in fitPseudo
	char argname[100];
	//  if(nxj==2) sprintf(argname,"%s",var1);
	// else sprintf(argname,"%s,%s",var1,var2);
	sprintf(argname,"%s,%s",var1,var2);

	ws.argSet(argname).readFromFile(initialvalues);// read nominal best fit value
	double x1= ws.var(var1)->getVal();
	double x2=ws.var(var2)->getVal(); //x2= 0.0;

	double e1= ws.var(var1)->getError();
	double e2=ws.var(var2)->getError(); //e2= 0.0;

	RooPlot *plot_MCbkg = ws.var("mZZ")->frame();

	std::vector<float> vals;
	std::vector<float> vals1;
	std::vector<float> vals2;
	vals.reserve(nToys);
	vals1.reserve(nToys);
	vals2.reserve(nToys);

	char indexstring[200];

	bool plotEnvelope=true;

	cout<<"pseudoMassge starts to loop over "<<nToys<<" toys"<<endl;
	for(unsigned i =0 ; i < nToys ; i++){
		sprintf(indexstring,"TMPResult_%dtag_%d",nxj,i);
		//  ifstream testFile(indexstring,ios::in);
		// if(! testFile.good())continue;
		ws.argSet(argname).readFromFile(indexstring);
		if(fabs(ws.var(var1)->getVal())>4.0 || fabs(ws.var(var2)->getVal())>4.0 ){
			cout<<"Toy #"<<i<<" did not have successful fit "<< ws.var(var1)->getVal()<<"  "<<ws.var(var2)->getVal()<<endl;
			continue;
		}
		vals1.push_back(ws.var(var1)->getVal());
		vals2.push_back(ws.var(var2)->getVal());
		if(plotEnvelope ){
			cout<<"Plot levexp_dcr #"<<i<<" with "<<var1<<"="<<ws.var(var1)->getVal()<<"  "<<var2<<"="<<ws.var(var2)->getVal()<<endl;
			ws.pdf("levexp_dcr")->plotOn(plot_MCbkg,LineWidth(1),LineColor(1));
			cout<<"Finished to Plot levexp_dcr #"<<i<<endl;
			// char nameTmpFit[64];
			// sprintf(nameTmpFit,"levexp_dcr_%d",i);
			// ws.pdf("levexp_dcr")->plotOn(plot_MCbkg,LineWidth(1),LineColor(1));//,Name(nameTmpFit));
			// ws.pdf(nameTmpFit)->plotOn(plot_MCbkg,LineWidth(1),LineColor(1),Name(nameTmpFit));      
		}//end if plotenvelope

		std::string mkdir_command("rm ");  
		mkdir_command+= indexstring;
		//  system(mkdir_command.c_str());
		if(i%100==0)    std::cout << mkdir_command << std::endl;
	}
	cout<<"pseudoMassge finished to loop over "<<nToys<<" toys"<<endl;


	char canvasName[400];
	sprintf( canvasName, "%s/mZZ_sidebandData_alphaVarWithToys_%dJ_%s", outDir.c_str(), nxj, "ALL");
	std::string canvasName_str(canvasName);
	std::string canvasName_eps = canvasName_str + ".eps";
	std::string canvasName_png = canvasName_str + ".png";
	TCanvas* c1 = new TCanvas("c1", "", 600, 600);
	c1->cd();


	if(plotEnvelope){
		RooCurve* upper = new RooCurve();
		RooCurve* lower = new RooCurve();
		upper->SetName("upper_staterr_alpha");
		lower->SetName("lower_staterr_alpha");
		float min = plot_MCbkg->GetXaxis()->GetXmin();
		float max = plot_MCbkg->GetXaxis()->GetXmax();
		float range=max-min;
		cout<<"Looping over points for envelope calculation: "<<flush;
		for(unsigned int x =0 ; x < 200 ; x++  ){
			float xval = min +x*range/200.;
			if(x%20==0)cout<<"Point #"<<x<<flush;
			for(unsigned i =0 ; i < nToys ; i++){
				vals.push_back(((RooCurve*)(plot_MCbkg->getObject(i)))->interpolate(xval));
				//   vals.push_back(0.0);
			}

			std::sort(vals.begin(),vals.end());
			lower->addPoint(xval,vals[floor(0.166*nToys)]);
			upper->addPoint(xval,vals[floor(0.832*nToys)]);
			vals.clear();
		}
		cout<<" Done."<<endl;


		lower->SetLineWidth(2);
		lower->SetLineColor(2);
		upper->SetLineWidth(2);
		upper->SetLineColor(2);

		cout<<"Plotting with pseudo experiments for "<<nxj<<"-J topology "<<endl;
		ws.pdf("levexp_dcr")->plotOn(plot_MCbkg,VisualizeError(*r_nominal,2.0,kFALSE),FillColor(kYellow));
		ws.pdf("levexp_dcr")->plotOn(plot_MCbkg,VisualizeError(*r_nominal,1.0,kFALSE),FillColor(kGreen));
		ws.pdf("levexp_dcr")->plotOn(plot_MCbkg);
		ws.pdf("levexp_dcr")->plotOn(plot_MCbkg, LineColor(kViolet-2), LineStyle(kDashed), Normalization(1.0+NormRelErr,RooAbsReal::Relative));
		ws.pdf("levexp_dcr")->plotOn(plot_MCbkg, LineColor(kViolet-2), LineStyle(kDashed), Normalization(1.0-NormRelErr,RooAbsReal::Relative));
		plot_MCbkg->addPlotable(lower);
		plot_MCbkg->addPlotable(upper);


		/*
		   double x0,up0,low0;
		   upper->GetPoint(0,x0,up0);  
		   lower->GetPoint(0,x0,low0);  
		   RooRealVar *r0=new RooRealVar("mZZ","mZZ",x0);
		   RooArgSet *tmparg=new RooArgSet(*r0);
		   double central0=1.0;//ws.pdf("levexp_dcr")->getValV();
		   RooRealVar normUp("norm_upper_staterr_alpha","norm_upper_staterr_alpha",up0/central0);
		   RooRealVar normDown("norm_lower_staterr_alpha","norm_lower_staterr_alpha",low0/central0);
		   std::cout<<"###Norm for UP/LOW: "<<x0<<" - "<< r0->getVal()<<" - "<<up0<<" - "<<low0<<" - "<<central0<<" - "<<normUp.getVal()<<std::endl;
		 */


		cout<<"Drawing plot_MCbkg"<<endl;
		plot_MCbkg->Draw(); 
		c1->SaveAs(canvasName_eps.c_str());
		c1->SaveAs(canvasName_png.c_str());

		plot_MCbkg->SetMinimum(0.0001);
		plot_MCbkg->SetMaximum(1.0);
		cout<<"Drawing plot_MCbkg (log-scale)"<<endl;
		plot_MCbkg->Draw();
		c1->SetLogy();

		canvasName_str += "_log";
		canvasName_eps = canvasName_str + ".eps";
		canvasName_png = canvasName_str + ".png";
		c1->SaveAs(canvasName_eps.c_str());
		c1->SaveAs(canvasName_png.c_str());

		char dumname[200];
		sprintf(dumname,"%s/rooCurves_%dJ_ALL.root",outDir.c_str(),nxj);
		cout<<"Drawing "<<dumname<<endl;
		TFile *fdumout=new TFile(dumname ,"RECREATE"); 
		char upname[50];
		char lowname[50];
		sprintf(upname,"roocurve_%dJ_UP",nxj);
		sprintf(lowname,"roocurve_%dJ_LOW",nxj);
		//sprintf(var2errA,"%s_alphaErr",var2);
		upper->Write();
		lower->Write();
		//   normUp.Write();
		// normDown.Write();
		//fdumout->Close();
		delete fdumout;

		delete plot_MCbkg;


	}//end if plotEnvelope


	c1->Clear();
	c1->SetLogy(false);

	// plotting value distributions
	std::sort(vals1.begin(),vals1.end());
	TH1F* histo1= new TH1F("test1","test1",100,vals1[0],vals1[nToys-1]);
	char tit[20];
	sprintf(tit,"#alpha_{%d}",nxj);
	histo1->GetXaxis()->SetTitle(tit);
	histo1->GetYaxis()->SetTitle("Nr trials");

	std::sort(vals2.begin(),vals2.end());
	TH1F* histo2= new TH1F("test2","test2",100,vals2[0],vals2[nToys-1]); 
	//  if(nxj!=2)histo2 = new TH1F("test2","test2",100,vals2[0],vals2[nToys-1]);  
	//  else histo2 = (TH1F*)histo1->Clone("test2");
	sprintf(tit,"#beta_{%d}",nxj);
	histo2->GetYaxis()->SetTitle("Nr trials");
	histo2->GetXaxis()->SetTitle(tit);


	TLine* line = new TLine();
	line->SetLineColor(2);
	line->SetLineWidth(2);
	for(unsigned i =0 ; i <  nToys; i++){
		histo1->Fill(vals1[i]);
		//    if(nxj!=2) histo2->Fill(vals2[i]);
		//    else histo2->Fill(0.0);
		histo2->Fill(vals2[i]);
	}

	//   std::cout << "XXXX before fits" << std::endl;

	//   histo1->Print("v");
	//   histo2->Print("all");


	histo1->Fit("gaus");
	double s1 = histo1->GetFunction("gaus")->GetParameter("Sigma"); // we need this to update the errors 
	histo2->Fit("gaus");
	//  double s2 = 0.0;
	double s2 = histo2->GetFunction("gaus")->GetParameter("Sigma"); // we need this to update the errors
	histo1->Draw();
	line->SetLineColor(2);
	line->DrawLine(x1,histo1->GetMinimum(),x1,histo1->GetMaximum());
	line->SetLineColor(4);
	line->DrawLine(x1+e1,histo1->GetMinimum(),x1+e1,histo1->GetMaximum());
	line->DrawLine(x1-e1,histo1->GetMinimum(),x1-e1,histo1->GetMaximum());


	sprintf( canvasName, "%s/alphaVar_par1_%dJ_%s", outDir.c_str(), nxj, "ALL");
	canvasName_str = canvasName;
	canvasName_eps = canvasName_str + ".eps";
	canvasName_png = canvasName_str + ".png";
	c1->SaveAs(canvasName_eps.c_str());
	c1->SaveAs(canvasName_png.c_str());

	histo2->Draw();
	line->SetLineColor(2);
	line->DrawLine(x2,histo2->GetMinimum(),x2,histo2->GetMaximum());
	line->SetLineColor(4);
	line->DrawLine(x2+e2,histo2->GetMinimum(),x2+e2,histo2->GetMaximum());
	line->DrawLine(x2-e2,histo2->GetMinimum(),x2-e2,histo2->GetMaximum());

	sprintf( canvasName, "%s/alphaVar_par2_%dJ_%s", outDir.c_str(), nxj, "ALL");
	canvasName_str = canvasName;
	canvasName_eps = canvasName_str + ".eps";
	canvasName_png = canvasName_str + ".png";
	c1->SaveAs(canvasName_eps.c_str());
	c1->SaveAs(canvasName_png.c_str());


	// ws->import(upper);//not supported by RooFit
	// ws->import(lower);

	//  char var1errA[50];
	// char var2errA[50];
	// sprintf(var1errA,"%s_alphaErr",var1);
	//sprintf(var2errA,"%s_alphaErr",var2);
	// RooRealVar errV1(var1errA,var1errA,s1);
	//RooRealVar errV2(var2errA,var2errA,s2);
	errV1.setVal(s1);
	errV2.setVal(s2);
	errV1.setConstant(kTRUE);
	errV2.setConstant(kTRUE);
	// ws.import(errV1);
	// ws.import(errV2);
	//   RooArgSet *res=new RooArgSet(errV1,errV2);
	//   cout<<"Exiting."<<endl;

	std::cout<<"Finished pseudoMassge !"<<std::endl;
}//end pseudoMassgeTwoPars



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
/*
   TH1D * makeVVR0(TTree* treeSBVV , TTree* treeSBData, double lepCut,int inxj, double purityCut)
   {
   TCanvas * c0 = new TCanvas();

   int nBins;
   const double* bins=0;
   if(inxj==2){
   nBins = nBins2;
   bins = bins2;
   }    
   else if(inxj==1){
   nBins = nBins1;
   bins = bins1;
   }    
   string ssnxj;
   if(inxj==1) ssnxj="1";
   if(inxj==2) ssnxj="2";
   string pur_str;
   if(purityCut==1) pur_str="HP";
   if(purityCut==0) pur_str="LP";
   if(purityCut==-1) pur_str="";
   string leptType;
   if(lepCut==-1) leptType="ALL";
   if(lepCut==1) leptType="MU";
   if(lepCut==0) leptType="ELE";


   TH1D * mZZSBVV = new TH1D ("mZZSBVV","mZZSBVV",nBins-1,bins);
   TH1D * mZZSBData = new TH1D ("mZZSBData","mZZSBData",nBins-1,bins);
   treeSBVV->Draw("mZZ>>mZZSBVV");
   treeSBData->Draw("mZZ>>mZZSBData");

   TH1D * R0 = new TH1D ("R0","R0",nBins-1,bins);
   R0->Reset();
   R0->Divide(mZZSBVV,mZZSBData);
   R0->Draw();

   c0->SaveAs((outDir+"/R0_"+ssnxj.c_str()+"J_"+pur_str+"_"+leptType+".png").c_str());

   delete mZZSBVV;
   delete mZZSBData;

   return R0;
   }
 */
