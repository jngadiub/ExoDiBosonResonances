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
#include "TStyle.h"

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
#include "RooExponential.h"
#include "RooGenericPdf.h"
#include "RooBinning.h"
#include "RooEllipse.h"
#include "RooAddPdf.h"
#include "RooHist.h"

#include "PdfDiagonalizer.h"
#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"

//#include "Config_XWW.h" 
//#include "binningFits_XWW.h"
#include "Config_XZZ.h"
#include "binningFits_XZZ.h"

using namespace std ;
using namespace RooFit ;


//ROOT::gErrorIgnoreLevel=kWarning;

RooPlot* ContourPlot(RooRealVar* var1,RooRealVar* var2, RooFitResult* r);
TTree* weightTree(TTree* tree, TH1D* h1_alpha,const std::string& name ,bool verbose=false,double corrFactor=1.0);
void fitPseudoOnePar( RooDataSet& ModSideband, RooWorkspace& ws,int seed,char* initialvalues, int nxj,std::string inPurStr,const std::string& leptType);
void fitPseudoTwoPars( RooDataSet& ModSideband, RooWorkspace& ws,int seed,char* initialvalues, int nxj, int pur,const std::string& leptType);
void pseudoMassgeOnePar(int nxj ,std::string inPurStr,const std::string& leptType, RooFitResult* r_nominal, RooWorkspace& ws,char* initialvalues, double NormRelErr, RooRealVar &errV1);
void pseudoMassgeTwoPars(int nxj, int pur ,const std::string& leptType, RooFitResult* r_nominal, RooWorkspace& ws,char* initialvalues, double NormRelErr, RooRealVar &errV1, RooRealVar &errV2);
void CopyTreeVecToPlain(TTree *t1, std::string wType, std::string f2Name,std::string t2Name,int nxjCut=-1);
void CreateUnrolledTreesDATA(std::string inDir,std::string tmpFileName,std::string tmpTreeName,std::string weighting,int nxjCut,ostream &outlog);
void CreateUnrolledTreesDYMCSIG(std::string inDirSIG,std::string tmpFileName,std::string tmpTreeName,std::string weighting,int nxjCut,ostream &outlog);
void CreateUnrolledTreesVVMCSIG(std::string inDirSIG,std::string tmpFileName,std::string tmpTreeName,std::string weighting,int nxjCut,ostream &outlog);
double getExtNormalization(int purityCut,double &extNorm, double &extNormELE, double &extNormMU);

/*****************
 *
 * All configurations now are in the header file fitBackgroundConfig_XZZ.h
 *
 *****************/
int main(){
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING) ;
  //DEBUG=0, INFO=1, PROGRESS=2, WARNING=3, ERROR=4, FATAL=5
  ofstream logf((myOutDir+"./log_fitBackground_"+leptType+".log").c_str(),ios::out);

  //write in a plain tree because RooFit does not like trees with branches storing vectors 
  const int nxjCut=-1;//if negative: no cut
  const std::string tmpTreeName="SelectedCandidatesV2";
  char foutn[64];
  if(nxjCut>=0)  sprintf(foutn,"EXOVVTree_DATASB_%d.root",nxjCut);
  else   sprintf(foutn,"EXOVVTree_DATASB_NOcut.root");
  std::string tmpFileName=foutn;
  std::string weighting = "weight";
  if(unrollTrees)CreateUnrolledTreesDATA(inDirSB,tmpFileName, tmpTreeName,weighting,nxjCut,logf);

  const std::string tmpSigTreeName="SelectedCandidatesSIG";
  char foutSig[64];
  if(nxjCut>=0)  sprintf(foutSig,"EXOVVTree_DATASIG_%d.root",nxjCut);
  else   sprintf(foutSig,"EXOVVTree_DATASIG_NOcut.root");
  std::string tmpSigFileName=foutSig;
  if(unrollTrees)CreateUnrolledTreesDATA(inDirSIG,tmpSigFileName, tmpSigTreeName,weighting,nxjCut,logf);
 
  TFile *ftree=new TFile(foutn,"READ");
  TTree *treeDATA_tmp=(TTree*)ftree->Get(tmpTreeName.c_str());

  TFile *ftreeSig=new TFile(foutSig,"READ");
  TTree *treeDATA_sig=(TTree*)ftreeSig->Get(tmpSigTreeName.c_str());

  TFile *ftreeVV=0;
  TTree *treeVV_sig=0;

  if(!useAlphaVV){//load VV MC in SIG region already created by fitSidebands
    char foutVV[64];
    if(nxjCut>=0)  sprintf(foutVV,"EXOVVTree_MCVV_SIG_%dJ.root",nxjCut);
    else   sprintf(foutVV,"EXOVVTree_MCVV_SIG_NOcut.root");
    //	  std::string tmpVVFileName=foutVV;
    ftreeVV=new TFile(foutVV,"READ");
    treeVV_sig=(TTree*)ftreeVV->Get(tmpTreeName.c_str());

  }//end if not useAlphaVV

  // TFile *ftreeHM=0;
  TChain *treeHM_sig=0;
  if(useMCHM){

    //create unrolled tree for DY MC in signal region
    char foutDYMC[64];
    char foutVV[64];
    if(nxjCut>=0)  sprintf(foutDYMC,"EXOVVTree_DYMC_SIG_%dJ.root",nxjCut);
    else   sprintf(foutDYMC,"EXOVVTree_DYMC_SIG_NOcut.root");
    tmpFileName=foutDYMC;

    if(nxjCut>=0)  sprintf(foutVV,"EXOVVTree_MCVV_SIG_%dJ.root",nxjCut);
    else   sprintf(foutVV,"EXOVVTree_MCVV_SIG_NOcut.root");
  
    if(unrollTrees){
      CreateUnrolledTreesDYMCSIG(inDirSIG,tmpFileName,tmpTreeName,weighting, nxjCut,logf);
      
      if(useAlphaVV){//we include VV in the alpha calculation, hence we didn't create the unrolled trees yet
	tmpFileName=foutVV;
	CreateUnrolledTreesVVMCSIG(inDirSIG,tmpFileName,tmpTreeName,weighting, nxjCut,logf);
      }
    }
    
    treeHM_sig=new TChain(tmpTreeName.c_str());
    treeHM_sig->Add(foutDYMC);
    treeHM_sig->Add(foutVV);
  
  }//end if not use MC at High Masses

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

  for( inxj=1;inxj<=int(jetCats);inxj++){

    int nPurities=1;
    if(inxj==1)nPurities=2;

    double purityCut=-1;
    for(int iP=0;iP<nPurities;iP++){//loop over purity categories
      if(inxj==1)purityCut=iP;//for 2J category, no cut on Purity
      //if(inxj!=1||iP!=1)continue;
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


      string alphaFileName=myOutDir+"/Workspaces_alpha_"+ssnxj.str()+"J_"+pur_str+"_"+leptType+".root";
      logf<<"\n\n\n\n****** NEW NXJ = "<<inxj<<" "<< pur_str.c_str()<<" ---> "<<(alphaFileName).c_str() <<std::endl; 
      std::cout<<"\n\n\n\n****** NEW NXJ = "<<inxj<<" "<< pur_str.c_str()<<"  ---> "<<(alphaFileName).c_str() <<std::endl; 
      TFile *falpha=new TFile( (alphaFileName).c_str() );
      char alphahname[50];
      //    sprintf(alphahname,"nominal_alpha_%dnxj",inxj);//histo with fit to alpha


      //sprintf(alphahname,"h_alpha_smoothened");
      sprintf(alphahname,"h_alpha_smoothened_Final");//alpha with R0


      TTree* weightedData = weightTree(treeDATA_tmp , (TH1D*)falpha->Get(alphahname)  ,"alphaWeightedTreetmp" );

      //stat uncertainty on alpha normalization

      double area_MCsig=( (TH1D*)falpha->Get("mX_signalRegion"))->Integral();
      double area_MCsb=( (TH1D*)falpha->Get("mX_sidebands"))->Integral();
      double nent_MCsig=( (TH1D*)falpha->Get("mX_signalRegion"))->GetEntries();
      double nent_MCsb=( (TH1D*)falpha->Get("mX_sidebands"))->GetEntries();
      double totAlpha=area_MCsig/area_MCsb;

      //this is if you have another alpha coming from a different model
      //(for example, Sherpa instead of MAdgraph)
      //    TFile *falphaAlt=new TFile( (myOutDir+"/Workspaces_alpha_"+ssnxj.str()+"J_"+leptType+".root").c_str() );
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

      RooWorkspace *ws = (RooWorkspace*)falpha->Get( ("ws_alpha_"+ssnxj.str()+"J_"+pur_str+"_"+leptType).c_str() );
      ws->Print();

      //TF1 *alpha_func=(TF1*)falpha->Get("alpha_fitfunc");
      // RooPolynomial *ws_alpha_pol=(RooPolynomial*)ws->pdf("pol0_func");
      //RooRealVar *c0=(RooRealVar*)ws->var("c0");
      //RooPolynomial *pol0_new=new RooPolynomial("newpol0_func","Polynomial 0th (constant) for fit",*mZZ,*c0,0);
      //RooFormulaVar *alpha_formula=new RooFormulaVar("alpha_formula","alpha_formula","@0",RooArgList(*c0));

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
      string cutSB= "nXjets=="+ssnxj.str()+" &&region==0.0 &&mZZ>800&&mZZ<3000"+lepCutStr+vtagcutstr;
      string cutSIG="nXjets=="+ssnxj.str()+" &&region==1.0 &&mZZ>800&&mZZ<3000"+lepCutStr+vtagcutstr;

      if(isZZChannel)
	{
	  cutSB= "nXjets=="+ssnxj.str()+" &&region==0.0 &&mZZ>"+strmcut.str()+lepCutStr+vtagcutstr;
	  cutSIG="nXjets=="+ssnxj.str()+" &&region==1.0 &&mZZ>"+ strmcut.str()+lepCutStr+vtagcutstr;	
	}

      string cutDum="nXjets=="+ssnxj.str()+" && mZZ>"+ strmcut.str()+lepCutStr+vtagcutstr;


      logf<<"Applying the following cuts (SB): "<<cutSB.c_str()<<std::endl;
      logf<<"Applying the following cuts (SIG): "<<cutSIG.c_str()<<std::endl;
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

      std::string name_datasb2="dsDataSB2tmp";//"_"+ssnxj.str()+"J"+pur_str;
      RooDataSet *dsDataSB2tmp=new RooDataSet(name_datasb2.c_str(),name_datasb2.c_str(),weightedData,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep,*vTagPurity,*alphaWeight),cutSB.c_str(),"alphaWeight") ;

      //define fit ranges       
      //if 1JLP of ZZ channel, start at 600 GeV     
      double minFitRange=((inxj==1&&purityCut==0&&isZZChannel) ? 650.0 : startFit);
      double maxFitRange=(inxj==1 ? mZZmax_ : bins[nBins-1]);
      mZZ->setRange("fitRange",minFitRange,maxFitRange) ;
      mZZ->setRange("plotRange",minMZZ,maxFitRange) ;



      double normalizationOLD=(dsDataSB2tmp->reduce(CutRange("fitRange")))->sumEntries();
      //for ZZ, 1JLP, define correction for normalization usign only fit range
      if(isZZChannel&&purityCut==0){
	normalizationOLD=(dsDataSB2tmp->reduce(CutRange("fitRange")))->sumEntries();
      }


      delete weightedData;delete dsDataSB2tmp;

      // assign wjets normalization from external info (for example: fit to MJ sidebands)
      // do it only if you are taking the other bkgds from MC (this is an assumption
      // done when calculating the new V+jets normalization); make sure that the fits
      // to the MJ sidebands were done in the same mVV range used here

      double normalizationNEW=normalizationOLD;
      double normalizationNEWELE=0.0,normalizationNEWMU=0.0;

      if(inxj==1&&!useAlphaVV){
	if(getExtNormalization(purityCut, normalizationNEW, normalizationNEWELE, normalizationNEWMU)==-1.0){
	  logf<<"WARNING !!! Requested to normalize V+jets bkgd to externally provided values, but tree name does not match. TreeName: "<<InTreeName.c_str()<<std::endl;
	}
      }
      //      normalizationNEW=normalizationOLD;

      double normCorrection=normalizationNEW/normalizationOLD;  
      logf<<" normalizationNEW "<<normalizationNEW<<" normalizationOLD  "<<normalizationOLD<<endl;

      name_datasb2="dsDataSB2";
      TTree* weightedData2 = weightTree(treeDATA_tmp , (TH1D*)falpha->Get(alphahname)  ,"alphaWeightedTree2",false,normCorrection );
      RooDataSet *dsDataSB2=new RooDataSet(name_datasb2.c_str(),name_datasb2.c_str(),weightedData2,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep,*vTagPurity,*alphaWeight),cutSB.c_str(),"alphaWeight") ;
      std::string name_datasig="dsDataSIG";//"_"+ssnxj.str()+"J"+pur_str;
      RooDataSet *dsDataSIG=new RooDataSet(name_datasig.c_str(),name_datasig.c_str(),(TTree*)treeDATA_sig,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep,*vTagPurity),cutSIG.c_str()) ;//real data in signal region; cuts on mjj and nXjets
      logf<<"Number of events in OBSERVED datasets:"<<std::endl;
      logf<<dsDataSB->GetName()<<"  -> "<<dsDataSB->numEntries()<<"  "<<dsDataSB->sumEntries()<<std::endl;
      logf<<dsDataSB2->GetName()<<"  -> "<<dsDataSB2->numEntries()<<"  "<<dsDataSB2->sumEntries()<<std::endl;
      dsDataSB2->Print("v");
      if(unblind)logf<<dsDataSIG->GetName()<<"  -> "<<dsDataSIG->numEntries()<<"  "<<dsDataSIG->sumEntries()<<std::endl;

      //add the MC VV from MC if requested
      RooDataSet *VVDataSetNoWeight = 0, *VVDataSetWeight = 0;
      if(!useAlphaVV){

	logf<<"Adding the VV-MC events to the sideband control region. SumEntries of dsDATASB2 before adding VV-MC: "<<dsDataSB2->sumEntries()<<"  "<<flush;
	char vvweightstring[100];
	sprintf(vvweightstring,"weight*%f",lumi);
	RooFormulaVar weightVV("vvWeight",vvweightstring,*mcweight) ;
	std::string cutSIGVV=cutSIG+"&&mZZ<2200";
	VVDataSetNoWeight=new RooDataSet("VVDS","VVDS",treeVV_sig,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep,*vTagPurity,*mcweight),cutSIGVV.c_str()) ;

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
	canX->SaveAs((myOutDir+"/mccontrol_"+channel_marker.c_str()+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+".png").c_str());
	mccontrol->SetMinimum(0.00006);
	gPad->SetLogy();
	mccontrol->Draw();
	canX->SaveAs((myOutDir+"/mccontrol_"+channel_marker.c_str()+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_log.png").c_str());
	delete mccontrol;
	delete canX;
	logf<<"; after addition of VV-MC: "<<dsDataSB2->sumEntries()<<endl;
	//update normalizations
	double normMCVVELE=0.0,normMCVVMU=0.0; 
	normMCVVELE=(VVDataSetWeight->reduce(CutRange("fitRange"))->reduce(Cut("lep==0")))->sumEntries();
	normMCVVMU=(VVDataSetWeight->reduce(CutRange("fitRange"))->reduce(Cut("lep==1")))->sumEntries();
	normalizationNEWELE+=normMCVVELE;
	normalizationNEWMU+=normMCVVMU;
	logf<<"Norm of VV MC in mVV fit Range -> ELE= "<<normMCVVELE<<"  MU= "<<normMCVVMU<<endl;
	cout<<"Finished to add the VV MC to the extrapolated SB."<<endl;
      }//end if not usealphavv

      delete VVDataSetNoWeight;
  

      // dsDataSB->Print();

      if(dsDataSB2->numEntries()<5){
	cout<<"ERROR! Not enough stat in data sidebands ("<<dsDataSB2->numEntries()<<" entries). Quitting."<<endl;
	continue;
      }

      //at high masses, add the MC DY+VV from MC if requested (can help if no data in SB to constrain fit)
      RooDataSet *HMDataSetNoWeight = 0, *HMDataSetWeight = 0;//HM->High Mass
      RooRealVar *HM_Norm=new RooRealVar("HighMassBkgMCNorm","Integral of DY+VV MC in [2200, 2800] divided by 600",0.0,0.0,10.0);
      if(useMCHM){

	logf<<"Adding MC events at high mass (sig region) to the sideband control region. SumEntries of dsDATASB2 before adding HM-MC: "<<dsDataSB2->sumEntries()<<"  "<<flush;
	char hmweightstring[100];
	sprintf(hmweightstring,"weight*%f*%f",lumi,1.0/DATAMC_HMSF[iP]);
	RooFormulaVar weightHM("hmWeight",hmweightstring,*mcweight) ;
	std::string cutSIGHM=cutSIG+"&&mZZ>2200&&mZZ<2800";
	HMDataSetNoWeight=new RooDataSet("HighMassDS","HighMassDS",treeVV_sig,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep,*vTagPurity,*mcweight),cutSIGHM.c_str()) ;

	RooRealVar* wHM = (RooRealVar*)HMDataSetNoWeight->addColumn(weightHM);			
	HMDataSetWeight = new RooDataSet("VVDSW","VVDS",HMDataSetNoWeight,*HMDataSetNoWeight->get(),0,wHM->GetName());
	HM_Norm->setVal(HMDataSetWeight->sumEntries()/600.0);
	HM_Norm->setConstant(kTRUE);
	//VVDataSetWeight->Print("v");
	dsDataSB2->append(*HMDataSetWeight);
	//dsDataSB2->Print("v");
	logf<<"; after addition of HM-MC: "<<dsDataSB2->sumEntries()<<"  MC added has density of "<<HM_Norm->getVal() <<endl;
	//update normalizations
	double normHMMCELE=0.0,normHMMCMU=0.0; 
	normHMMCELE=(HMDataSetWeight->reduce(CutRange("fitRange"))->reduce(Cut("lep==0")))->sumEntries();
	normHMMCMU=(HMDataSetWeight->reduce(CutRange("fitRange"))->reduce(Cut("lep==1")))->sumEntries();
	normalizationNEWELE+=normHMMCELE;
	normalizationNEWMU+=normHMMCMU;

	logf<<"Norm of HM-MC in mVV fit Range -> ELE= "<<normHMMCELE<<"  MU= "<<normHMMCMU<<endl;
	cout<<"Finished to add the HM MC to the extrapolated SB."<<endl;
      }//end if not usealphavv

      delete HMDataSetNoWeight;
  

      // dsDataSB->Print();

      if(dsDataSB2->numEntries()<5){
	cout<<"ERROR! Not enough stat in data sidebands ("<<dsDataSB2->numEntries()<<" entries). Quitting."<<endl;
	continue;
      }
      //end if add the pure MC at high masses when you run out of data in SB



      //fit the weighted dataset with a custom function
      std::cout<<"STARTING TO FIT WITH CUSTOM FUNCTION in range [ "<<minFitRange<<" , "<<maxFitRange <<" ]"<<std::endl;
      logf<<"STARTING TO FIT WITH CUSTOM FUNCTION in range [ "<<minFitRange<<" , "<<maxFitRange <<" ]"<<std::endl;
      double inislope;
      if(inxj==1)inislope=-0.25;
      if(inxj==2)inislope=-0.25;
      //	char slopePar_name[32];
      //	sprintf(slopePar_name,"a0_%dJ",inxj);
      std::string slopePar_name="expoFit_"+channel_marker+"_"+leptType+"_"+ssnxj.str()+"J"+pur_str+"_p0";
      //logf<<slopePar_name.c_str()<<endl;

      RooRealVar *a0=new RooRealVar(slopePar_name.c_str(),slopePar_name.c_str(),inislope,-10.0,0.0);
      RooExponential *expo_fit=new RooExponential(("exp_fit_"+channel_marker).c_str(),"exp_fit",*mZZ,*a0);
      //  RooRealVar *exp_norm=new RooRealVar("exp_N","exp_N",0.0,10000.0);

      //  RooFitResult* r_sig = expo_fit->fitTo(*dsDataSB,Save(kTRUE),Range("fitRange")) ;
      RooFitResult* r_sig2 = expo_fit->fitTo(*dsDataSB2,Save(kTRUE),SumW2Error(kTRUE),RooFit::PrintLevel(-1),Range("fitRange")) ;
      char fitResultName_expo[200];
      sprintf( fitResultName_expo, "resultsExpoFit_%s_%dJ_%s_%s",channel_marker.c_str(),inxj ,pur_str.c_str(), leptType.c_str() );
      r_sig2->SetName(fitResultName_expo); 
      sprintf( fitResultName_expo, "%s/resultsExpoFit_%s_%dJ_%s_%s.txt",myOutDir.c_str(),channel_marker.c_str(),inxj ,pur_str.c_str(), leptType.c_str() );
      RooArgSet expofitvars( *a0 );
      expofitvars.writeToFile(fitResultName_expo);


      logf<<"\n\n\t---> Check entries: DatainSB (no alpha)="<<dsDataSB->sumEntries()<<"   DatainSB (rescaled with alpha)="<<dsDataSB2->sumEntries()<<endl<<endl;
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
      //normalizations in fit range
      //for ZZ: normalization in [500, 2800] always, also for LP
      //reason is that one must create workspaces for HiggsComb tool
      //using same RooRealVar otherwise mess with RooStats when 
      //combining channels
      RooRealVar *NbkgRange=new RooRealVar("bkgdNormalizationFitRange","Background normalization in range of fit (ELE+MU)",dsDataSB2->reduce(CutRange("fitRange"))->sumEntries(),0.0,10000.0);
      RooRealVar *NbkgRangeELE=new RooRealVar("bkgdNormalizationFitRangeELE","Background normalization in range of fit (ELE)",dsDataSB2->reduce(CutRange("fitRange"))->reduce("lep==0")->sumEntries(),0.0,10000.0);
      RooRealVar *NbkgRangeMU=new RooRealVar("bkgdNormalizationFitRangeMU","Background normalization in range of fit (MU)",dsDataSB2->reduce(CutRange("fitRange"))->reduce("lep==1")->sumEntries(),0.0,10000.0);
      RooRealVar *Nbkg500=new RooRealVar("bkgdNormalization500","Background normalization starting at M_VV=500 GeV",dsDataSB2->reduce(Cut("mZZ>500.0"))->sumEntries(),0.0,10000.0);
      RooRealVar *Nbkg500ELE=new RooRealVar("bkgdNormalization500ELE","Background normalization  starting at M_VV=500 GeV (ELE)",dsDataSB2->reduce(Cut("mZZ>500.0&&lep==0"))->sumEntries(),0.0,10000.0);
      RooRealVar *Nbkg500MU=new RooRealVar("bkgdNormalization500MU","Background normalization  starting at M_VV=500 GeV (MU)",dsDataSB2->reduce(Cut("mZZ>500.0&&lep==1"))->sumEntries(),0.0,10000.0);

      if(useVJetsNormFromMJFit){
	double ELEfrac_MJFit=normalizationNEWELE/ (normalizationNEWELE+normalizationNEWMU);
	Nbkg500ELE->setVal(Nbkg500->getVal()*ELEfrac_MJFit);
	Nbkg500MU->setVal(Nbkg500->getVal()*(1.0-ELEfrac_MJFit));
      }

      NbkgRange->setConstant(kTRUE);

      //for High mass (>2000)
      RooRealVar *Nbkg2000=new RooRealVar("bkgdNormalization2000","Background normalization starting at M_VV=500 GeV",dsDataSB2->reduce(Cut("mZZ>2000.0"))->sumEntries(),0.0,10000.0);
      RooRealVar *Nbkg2000ELE=new RooRealVar("bkgdNormalization2000ELE","Background normalization  starting at M_VV=500 GeV (ELE)",dsDataSB2->reduce(Cut("mZZ>2000.0&&lep==0"))->sumEntries(),0.0,10000.0);
      RooRealVar *Nbkg2000MU=new RooRealVar("bkgdNormalization2000MU","Background normalization  starting at M_VV=500 GeV (MU)",dsDataSB2->reduce(Cut("mZZ>2000.0&&lep==1"))->sumEntries(),0.0,10000.0);
      RooRealVar *SFDYHM=new RooRealVar("sfMCHighMass","Data/MC scale factor applied to DY+VV MC at high mass (applied only if requested)",DATAMC_HMSF[iP],0.0,10.0);
      SFDYHM->setConstant(kTRUE);
      // a0->setConstant(kTRUE);
      logf<<"Normalization in full range : ELE="<<NbkgELE->getVal()<<"  MU="<<NbkgMU->getVal()<<"  ALL="<<Nbkg->getVal()<<endl;
      logf<<"Normalization errors: Nent="<<Nent->getVal()<<" NormErr="<<NormErr->getVal()<<"  Nerr="<<Nerr->getVal()<<std::endl;
      logf<<"Norm In Range ALL = "<<NbkgRange->getVal()<<"   Expo-fit Slope="<<a0->getVal()<<std::endl;
      logf<<"Norm In Range (using ELE/MU ratio from extrapolated sidebands) ELE = "<<NbkgRangeELE->getVal()<<"  MU = "<<NbkgRangeMU->getVal()<<endl; 
      logf<<"Norm In Range (using V+jets from fit) ELE = "<<normalizationNEWELE<<"  MU = "<<normalizationNEWMU<<endl; 
      logf<<"Norm In [500, INF] = "<<Nbkg500->getVal()<<"   Expo-fit Slope="<<a0->getVal()<<std::endl;
      logf<<"Norm In [500, INF] ELE = "<<Nbkg500ELE->getVal()<<"  MU = "<<Nbkg500MU->getVal()<<endl; 
      logf<<"Norm In [2000, INF] ELE = "<<Nbkg2000ELE->getVal()<<"  MU = "<<Nbkg2000MU->getVal()<<endl; 
      if(unblind){
	logf<<dsDataSIG->GetName()<<"  -> ALL: "<<dsDataSIG->numEntries()<<" ELE: "<<dsDataSIG->reduce("lep==0")->numEntries()<<" MU: "<<dsDataSIG->reduce("lep==1")->numEntries()<<endl;

	logf<<dsDataSIG->GetName()<<" in [500, 2200]  -> ALL: "<<dsDataSIG->reduce("mZZ>500.0&&mZZ<2200.0")->numEntries()<<" ELE: "<<dsDataSIG->reduce("lep==0&&mZZ>500.0&&mZZ<2200.0")->numEntries()<<" MU: "<<dsDataSIG->reduce("lep==1&&mZZ>500.0&&mZZ<2200.0")->numEntries()<<endl;
	logf<<dsDataSIG->GetName()<<" in [650, 2800]  -> ALL: "<<dsDataSIG->reduce("mZZ>650.0&&mZZ<2800.0")->numEntries()<<" ELE: "<<dsDataSIG->reduce("lep==0&&mZZ>650.0&&mZZ<2800.0")->numEntries()<<" MU: "<<dsDataSIG->reduce("lep==1&&mZZ>650.0&&mZZ<2800.0")->numEntries()<<endl;
	logf<<dsDataSIG->GetName()<<" in [2000, INF]  -> ALL: "<<dsDataSIG->reduce("mZZ>2000.0")->numEntries()<<" ELE: "<<dsDataSIG->reduce("lep==0&&mZZ>2000.0")->numEntries()<<" MU: "<<dsDataSIG->reduce("lep==1&&mZZ>2000.0")->numEntries()<<endl;
      }
      r_sig2->printMultiline(logf,99,true);
      logf<<"\nNow fitting with Leveled Expo:"<<endl;
      //for levelled exponential
      double initf0=90.0;
      if(inxj==1)initf0=200.0;
      if(inxj==2)initf0=200.0;
      double initf1=0.0;
      if(inxj==1)initf1=0.010;
      if(inxj==2)initf1=0.050;
      double initf1b=0.0;
      if(inxj==1)initf1b=0.0;
      if(inxj==2)initf1b=0.0;
      double initm=480.0;
      if(inxj==2)initm=400.0;
      RooRealVar *f0=new RooRealVar("f0","sigma",initf0,0.0,300.0);
      RooRealVar *f1=new RooRealVar("f1","alpha",initf1,-0.050,0.5);
      RooRealVar *f1b=new RooRealVar("f1b","beta",initf1b,-0.1,2.0);
      RooRealVar *f2=new RooRealVar("f2","m",initm,200.0,500.0);
      RooRealVar *f3=new RooRealVar("f3","theta",0.0);
      RooRealVar *f4=new RooRealVar("f4","tail",HM_Norm->getVal(),0.0,10.0);
      f4->setError(HM_Norm->getVal()*1.0);
      f2->setConstant(kTRUE);
      f3->setConstant(kTRUE);
      f4->setConstant(kTRUE);
      f1b->setConstant(kTRUE);//if both f1 and f1b set constant to zero, levelled expo becomes a normal exponential
         
      RooLevelledExp2 *expLev_fit=new RooLevelledExp2("levelled_exp_fit","levelled_exp_fit",*mZZ,*f0,*f1,*f1b,*f2,*f3);
      //RooLevExpFlatTail *expLev_fit=new RooLevExpFlatTail("levelled_exp_fit","levelled_exp_fit",*mZZ,*f0,*f1,*f1b,*f2,*f3,*f4);
      RooFitResult* r_sig_expLev = expLev_fit->fitTo(*dsDataSB2,Save(kTRUE),SumW2Error(kTRUE),RooFit::PrintLevel(-1),Range("fitRange")) ;//,Range("fitRange"),SumW2Error(kTRUE)

      logf<<"LevExpo fit done: Sigma = "<<f0->getVal()<<"   alpha="<<f1->getVal()<<"   beta="<<f1b->getVal()<<"   m="<<f2->getVal()<<"  Theta="<<f3->getVal()<<std::endl;
      r_sig_expLev->printMultiline(logf,99,true);
      logf<<"LEVELLED EXPO FIT PERFORMED ! DECORRELATING PARAMETERS."<<std::endl;

      //now decorrelate parameters:
      RooWorkspace *wstmp=new RooWorkspace("tempWorkSpace","tempWorkSpac");
      RooAbsPdf  *background_decorr_=0;
      RooFitResult *r_sig_expLev_decorr =0;
      char fitResultName_eig[200];
      sprintf( fitResultName_eig, "DUMMYExpLevelledFit_%s_%dJ_%s_decorr",channel_marker.c_str(),inxj , leptType.c_str() );

      if(decorrLevExpo){
	char diagonalizerName[200];
	sprintf( diagonalizerName, "expLev_%s_%s_%dJ%s",channel_marker.c_str(),leptType.c_str(), inxj,pur_str.c_str());
	PdfDiagonalizer diago(diagonalizerName, wstmp, *r_sig_expLev );
	background_decorr_ =diago.diagonalize(*expLev_fit);//RooAbsPdf
	background_decorr_->SetName(bkgd_decorr_name.c_str());
	char var1[96],var2[96];
	//the first part of the name of these vairables var1 and var2 MUST 
	//match the name of diagonalizerName
	sprintf(var1,"expLev_%s_%s_%dJ%s_eig0",channel_marker.c_str(),leptType.c_str(),inxj,pur_str.c_str());
	sprintf(var2,"expLev_%s_%s_%dJ%s_eig1",channel_marker.c_str(),leptType.c_str(),inxj,pur_str.c_str());

	RooRealVar *f0rot=(RooRealVar*)wstmp->var(var1);
	RooRealVar *f1rot=(RooRealVar*)wstmp->var(var2);
	logf<<"Decorr vars: "<<f0rot->getVal()<<"  "<<f1rot->getVal()<<endl;
	//if only one free par in roolevexpo
	//RooRealVar *f1rot=0;
	// f1rot=new RooRealVar(*f1); f1rot->SetName(var2);


	//refit to get a new covariance matrix to check that this has worked
	r_sig_expLev_decorr = background_decorr_->fitTo(*dsDataSB2, Save(),SumW2Error(kTRUE),RooFit::PrintLevel(-1),Range("fitRange"));
	//    char fitResultName_eig[200];
	sprintf( fitResultName_eig, "resultsExpLevelledFit_%s_%dJ_%s_%s_decorr",channel_marker.c_str(),inxj ,pur_str.c_str(), leptType.c_str() );
	r_sig_expLev_decorr->SetName(fitResultName_eig); 
	logf<<"Sigma= "<<f0->getVal()<<" -> SigmaDecorr="<<f0rot->getVal()<<std::endl;
	logf<<"Alphaa= "<<f1->getVal()<<" -> AlphaDecorr="<<f1rot->getVal()<<std::endl;
	logf<<"Beta="<<f1b->getVal() <<" m= "<<f2->getVal()<<" Theta="<<f3->getVal()<<std::endl;
	sprintf( fitResultName_eig, "%s/resultsExpLevelledFit_%s_%dJ_%s_%s_decorr.txt",myOutDir.c_str(),channel_marker.c_str(),inxj ,pur_str.c_str(), leptType.c_str() );
	RooArgSet rotvars( *f0rot,*f1rot );
	rotvars.writeToFile(fitResultName_eig);

	//plot the correlated/decorrelated uncertainties
	bool plotDecorr=true;
	if(plotDecorr){
	  TCanvas* c_rot = new TCanvas("canvas_rot", "CANVAS PARS ROT", 800, 800);
	  c_rot->cd();
	  double f0val=f0->getVal();
	  double f1val=f1->getVal();
	  double f0err=f0->getError();
	  double f1err=f1->getError();
	  Double_t rho= r_sig_expLev->correlation(f0->GetName(),f1->GetName());
	  RooEllipse *contour= new RooEllipse("contour",f0val,f1val,f0err,f1err,rho,500);
	  contour->SetLineWidth(2) ;
	  RooPlot *plot = new RooPlot(*f0,*f1,f0val-2*f0err,f0val+2*f0err,f1val-2*f1err,f1val+2*f1err);
	  plot->addPlotable(contour);
	  plot->Draw();
	  c_rot->SaveAs((myOutDir+"/checkRotation_"+channel_marker+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+".eps").c_str());
	  c_rot->SaveAs((myOutDir+"/checkRotation_"+channel_marker+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+".png").c_str());

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
	  c_rot->SaveAs((myOutDir+"/checkRotation_"+channel_marker+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_decorrelated.eps").c_str());
	  c_rot->SaveAs((myOutDir+"/checkRotation_"+channel_marker+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_decorrelated.png").c_str());

	  delete contour;
	  delete c_rot;
	}//end plot Decorr

      }//edn if decorrLevExpo

      //save everything in a RooWorkspace
      TFile *fout=new TFile((myOutDir+"/workspace_"+channel_marker+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_new.root").c_str(),"RECREATE");
      RooWorkspace *wsnew=new RooWorkspace(*ws);
      wsnew->SetName(("ws_alpha_"+channel_marker+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType).c_str());
      logf<<"\n\nName of new WS: "<<wsnew->GetName()<<std::endl;
      wsnew->import(*Nbkg);
      wsnew->import(*NbkgELE);
      wsnew->import(*NbkgMU);
      wsnew->import(*NbkgRange);
      wsnew->import(*NbkgRangeELE);
      wsnew->import(*NbkgRangeMU);
      wsnew->import(*Nbkg500);
      wsnew->import(*Nbkg500ELE);
      wsnew->import(*Nbkg500MU);
      wsnew->import(*Nbkg2000);
      wsnew->import(*Nbkg2000ELE);
      wsnew->import(*Nbkg2000MU);
      wsnew->import(*Nent);
      wsnew->import(*alphaErr);
      wsnew->import(*SFDYHM);
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
      weightedData2->Write();


      if(doPseudoExp){
	char tmpTname[32];
	char fitResultNamePseudo[128];
	bool doPseudoTwoPars=true;//use lev expo 
	//if(isZZChannel)doPseudoTwoPars=false;
	//if(purityCut==1&&isZZChannel){//XZZ-1JHP : simple expo with only one param
	//  doPseudoTwoPars=false;
	//	}
	if(doPseudoTwoPars) sprintf(fitResultNamePseudo,"%s", fitResultName_expo);
	else sprintf(fitResultNamePseudo,"%s", fitResultName_eig);
	//do pseudo-experiments for alpha-error
	for( unsigned n=0 ;n<nToys;n++){
	  //get alternative weights
	  if(n%50==0)std::cout<<"Throwing toy #"<<n<<"/"<<nToys<<" "<<std::endl;
	  sprintf(alphahname,"tmp_%d",n);
	  sprintf(tmpTname,"pseudoTree_%s",alphahname);    
	  TTree* weightedPseudo = weightTree(treeDATA_tmp ,(TH1D*)falpha->Get(alphahname), tmpTname, false, normCorrection);
	  RooDataSet pseudoSB2("pseudoSB2","pseudoSB2",weightedPseudo,RooArgSet(*mZZ,*nXjets,*region,*mJJ,*lep,*alphaWeight,*vTagPurity),cutSB.c_str(),"alphaWeight") ;
	  pseudoSB2.append(*VVDataSetWeight);
	  //std::cout << "XXX " <<doPseudoTwoPars << "  " <<decorrLevExpo <<std::endl;
	  if(doPseudoTwoPars && decorrLevExpo){//actually do it only if you already decorrelated the two par func
	    fitPseudoTwoPars(pseudoSB2,*wsnew,n,fitResultNamePseudo,inxj,purityCut,leptType);
	  }
	  else{
	    fitPseudoOnePar(pseudoSB2,*wsnew,n,fitResultNamePseudo,inxj,pur_str,leptType);
	  }
	  //      cout<<"Deleting tree #"<<n<<"  "<<weightedPseudo->GetName()<<endl;
	  delete weightedPseudo;
	}
	//std::cout << "XXX" << std::endl;
	char var1errA[50];
	sprintf(var1errA,"expoFit_%s_%s_%dJ%s_p0_alphaErr",channel_marker.c_str(),leptType.c_str(),inxj,pur_str.c_str());

	char var2errA[50];
	sprintf(var2errA,"DUMMY_%dJ_alphaErr",inxj);
	if(doPseudoTwoPars &&decorrLevExpo){
	  sprintf(var1errA,"expLev_%s_%s_%dJ%s_eig0_alphaErr",channel_marker.c_str(),leptType.c_str(),inxj,pur_str.c_str());
	  sprintf(var2errA,"expLev_%s_%s_%dJ%s_eig1_alphaErr",channel_marker.c_str(),leptType.c_str(),inxj,pur_str.c_str());

	}

	RooRealVar errV1(var1errA,var1errA,0.0);
	RooRealVar errV2(var2errA,var2errA,0.0);
	std::cout<<"Starting pseudoMassge"<<std::endl;
	if(doPseudoTwoPars && decorrLevExpo) pseudoMassgeTwoPars( inxj ,purityCut,leptType,r_sig_expLev,*wsnew,fitResultName_eig, Nerr->getVal(), errV1, errV2);
	else  pseudoMassgeOnePar( inxj,pur_str, leptType  ,r_sig2,*wsnew,fitResultName_expo, Nerr->getVal(), errV1);

	wsnew->import(errV1);
	logf<<"Saving in the ws the error on shape param due to alpha stat: errPar1="<<errV1.getVal()<<std::flush;
	if(doPseudoTwoPars && decorrLevExpo){
	  logf<<"  errPar2="<<errV2.getVal()<<std::endl;
	  wsnew->import(errV2);
	}
	else logf<<std::endl;
      }//end if doPseudoExp

      fout->cd();
      wsnew->Write();


      logf<<"N evts in dsDataSIG in [1600, 2500] GeV = "<<dsDataSIG->reduce("mZZ>1600.0&&mZZ<2500.0")->sumEntries()<<endl;
      logf<<"N evts in dsDataSB2 in [1600, 2500] GeV = "<<dsDataSB2->reduce("mZZ>1600.0&&mZZ<2500.0")->sumEntries()<<endl;
      mZZ->setRange("HighMassTail",1600,2500.0);
      double fit_1parIntFitR=expo_fit->createIntegral(*mZZ,RooFit::Range("fitRange"))->getVal();
      double fit_2parIntFitR=background_decorr_->createIntegral(*mZZ,RooFit::Range("fitRange"))->getVal();
      double Fit1pFrac=expo_fit->createIntegral(*mZZ,RooFit::Range("HighMassTail"))->getVal() / fit_1parIntFitR;
      double Fit2pFrac=background_decorr_->createIntegral(*mZZ,RooFit::Range("HighMassTail"))->getVal() / fit_2parIntFitR;
      logf<<"Yield in [1600, 2500] GeV predicted by SimpleExpo fit = "<<Fit1pFrac*(NbkgRange->getVal())<<endl;
      logf<<"Yield in [1600, 2500] GeV predicted by LevExpo fit = "<<Fit2pFrac*(NbkgRange->getVal())<<endl;



      //draw and save plots
      std::cout<<"Drawing"<<std::endl;

      TCanvas *can1=new TCanvas("canvas1", "can1",800,800);
      can1->cd();
      RooPlot *xf;
      if(!plotFixedBinning)xf=mZZ->frame(minMZZ,maxFitRange);
      else xf=mZZ->frame((bins[nBins-1]-bins[0])/FixedBinWidth);//definition of FixedBinWidth in Config_XWW.h and Config_XZZ.h
      string frameTitle="Bkgd Estimation from Data Sidebands ("+ssnxj.str()+"J "+pur_str +" - "+leptType+" lept flav)";
      xf->SetTitle(frameTitle.c_str());
      //DO NOT CHANGE THE ORDER !!!!!!! DATA AS FIRST ONES !!!!
      if(!plotFixedBinning)
	{
	  dsDataSB2->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(21),MarkerColor(kRed),RooFit::Range("plotRange"));
	  if(unblind)dsDataSIG->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(20),MarkerColor(kBlack),RooFit::Range("fitRange"),RooFit::Range("plotRange"));
	  if(useMCHM)HMDataSetWeight->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(21),MarkerColor(kBlue-7),RooFit::Range("plotRange"));
	}
      else
	{
	  dsDataSB2->plotOn(xf,MarkerStyle(21),MarkerColor(kRed));
	  if(unblind)dsDataSIG->plotOn(xf,MarkerStyle(20),MarkerColor(kBlack));
	  if(useMCHM)HMDataSetWeight->plotOn(xf,MarkerStyle(21),MarkerColor(kBlue-7));
	}

      if(plotDecorrLevExpoMain){
	//plot error bands of fit			  
	background_decorr_->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent), LineColor(kViolet-2),VisualizeError(*r_sig_expLev_decorr,2.0,kFALSE),FillColor(kYellow),RooFit::NormRange("fitRange"),RooFit::Range("plotRange"));//
	background_decorr_->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent), LineColor(kViolet-2),VisualizeError(*r_sig_expLev_decorr,1.0,kFALSE),FillColor(kGreen),RooFit::NormRange("fitRange"),RooFit::Range("plotRange"));

	//plot normalization unc of fits		
	background_decorr_->plotOn(xf, Normalization(NbkgRange->getVal()+Nerr->getVal()*NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), LineColor(kViolet-2), LineStyle(kDashed),RooFit::Range("plotRange"));
	background_decorr_->plotOn(xf, Normalization(NbkgRange->getVal()-Nerr->getVal()*NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), LineColor(kViolet-2), LineStyle(kDashed),RooFit::Range("plotRange"));
	//plot fits		
	expo_fit->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), LineColor(kBlue), LineStyle(kDashed));
	background_decorr_->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), LineColor(kViolet-2),RooFit::Range("plotRange"));//,RooFit::Range("plotRange")

      }
      else{
	expo_fit->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent), LineColor(kViolet-2),VisualizeError(*r_sig2,2.0,kFALSE),FillColor(kYellow),RooFit::NormRange("fitRange"),RooFit::Range("plotRange"));
	expo_fit->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent), LineColor(kViolet-2),VisualizeError(*r_sig2,1.0,kFALSE),FillColor(kGreen),RooFit::NormRange("fitRange"),RooFit::Range("plotRange"));

	//
	background_decorr_->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"),RooFit::Range("plotRange"), LineColor(kViolet-2)); 
	expo_fit->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), LineColor(kBlue), LineStyle(kSolid));

	//
	expo_fit->plotOn(xf, Normalization(NbkgRange->getVal()+Nerr->getVal()*NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), LineColor(kBlue), LineStyle(kDashed));
	expo_fit->plotOn(xf, Normalization(NbkgRange->getVal()-Nerr->getVal()*NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), LineColor(kBlue), LineStyle(kDashed));
	background_decorr_->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"),RooFit::Range("plotRange"), LineColor(kViolet-2));
	expo_fit->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), LineColor(kBlue), LineStyle(kSolid));      }

      //	expLev_fit->plotOn(xf, Normalization(NbkgRange->getVal(),RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"), LineColor(kOrange+5), LineStyle(kDashed));//RooAbsPdf::NumEvent

      if(!plotFixedBinning)
	{
	  dsDataSB2->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(21),MarkerColor(kRed),RooFit::Range("plotRange"));//,Normalization(dsDataSB2->numEntries(),RooAbsPdf::NumEvent)
	  if(useMCHM)HMDataSetWeight->plotOn(xf,MarkerStyle(21),MarkerColor(kBlue-7),Binning(RooBinning(nBins-1,bins)),RooFit::Range("plotRange"));
	  if(unblind)dsDataSIG->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(20),MarkerColor(kBlack),RooFit::Range("plotRange"));

	}
      else
	{

	  dsDataSB2->plotOn(xf,MarkerStyle(21),MarkerColor(kRed));
	  if(useMCHM)HMDataSetWeight->plotOn(xf,MarkerStyle(21),MarkerColor(kBlue-7));
	  if(unblind)dsDataSIG->plotOn(xf,MarkerStyle(20),MarkerColor(kBlack));

	}

      logf<<"Check nromalization: NumEntries of dsDataSIG= "<<dsDataSIG->numEntries() <<"("<<dsDataSIG->sumEntries() <<")    SumEntries of dsDataSB2="<<dsDataSB2->sumEntries()<<"   numEntries="<<dsDataSB2->numEntries()<<"    Nbkg (NORM)="<<NbkgRange->getVal()<<"   Nent="<<Nent->getVal()<<"     Nerr="<<Nerr->getVal() <<std::endl;
      if(!isZZChannel)xf->SetXTitle("mWW");
      TPad * fPads1 = NULL;
      TPad * fPads2 = NULL;
      if(unblind&&plotFitPull)
	{
	  fPads1 = new TPad("pad1", "", 0.00, 0.30, 0.99, 0.99);
	  fPads2 = new TPad("pad2", "", 0.00, 0.00, 0.99, 0.30);
	  fPads1->SetFillColor(0);
	  fPads1->SetLineColor(0);
	  fPads2->SetFillColor(0);
	  fPads2->SetLineColor(0);
	  fPads1->Draw();
	  fPads2->Draw();


	  fPads1->cd();
	  RooHist* hpull = xf->pullHist();
	  xf->Draw();
	  fPads1->Update();

	  RooPlot* xf2 = mZZ->frame(Title("Pull Distribution")) ;
	  //set error to 1 by hand
	  for(Int_t i=0;i<hpull->GetN();i++){
	    hpull->SetPointError(i,0.,0.,1.,1.);
	  }
	  xf2->addPlotable(hpull,"P") ;
	  xf2->GetYaxis()->SetRangeUser(-4,4);
	  fPads2->cd();	
	  xf2->Draw();
	  //draw lines 0 -2, 2
	  TLine* lineAtZero = new TLine(xf2->GetXaxis()->GetXmin(),0,xf2->GetXaxis()->GetXmax(),0);
	  lineAtZero->SetLineColor(2);
	  lineAtZero->Draw();
	  TLine* lineAtPlusTwo = new TLine(xf2->GetXaxis()->GetXmin(),2,xf2->GetXaxis()->GetXmax(),2);
	  lineAtPlusTwo->SetLineColor(2);
	  lineAtPlusTwo->SetLineStyle(2);
	  lineAtPlusTwo->Draw();
	  TLine* lineAtMinusTwo = new TLine(xf2->GetXaxis()->GetXmin(),-2,xf2->GetXaxis()->GetXmax(),-2);
	  lineAtMinusTwo->SetLineColor(2);
	  lineAtMinusTwo->SetLineStyle(2);
	  lineAtMinusTwo->Draw();
	  fPads2->Update();

	}
      else {
	fPads1 = new TPad("pad1", "", 0.00, 0.00, 0.99, 0.99);
	fPads1->SetFillColor(0);
	fPads1->SetLineColor(0);
	fPads1->Draw();
	fPads1->cd();
	xf->Draw();
	fPads1->Update();
      }

//       dsDataSB2->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(21),MarkerColor(kRed));//,Normalization(dsDataSB2->numEntries(),RooAbsPdf::NumEvent)
//       if(unblind)dsDataSIG->plotOn(xf,Binning(RooBinning(nBins-1,bins)),MarkerStyle(20),MarkerColor(kBlack));
//       logf<<"Check nromalization: NumEntries of dsDataSIG= "<<dsDataSIG->numEntries() <<"("<<dsDataSIG->sumEntries() <<")    SumEntries of dsDataSB2="<<dsDataSB2->sumEntries()<<"   numEntries="<<dsDataSB2->numEntries()<<"    Nbkg (NORM)="<<NbkgRange->getVal()<<"   Nent="<<Nent->getVal()<<"     Nerr="<<Nerr->getVal() <<std::endl;
//      xf->Draw();

      can1->SaveAs((myOutDir+"/fitPlot_"+channel_marker+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+".root").c_str());
      can1->SaveAs((myOutDir+"/fitPlot_"+channel_marker+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+".eps").c_str());
      can1->SaveAs((myOutDir+"/fitPlot_"+channel_marker+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+".png").c_str());
      can1->SaveAs((myOutDir+"/fitPlot_"+channel_marker+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+".pdf").c_str());
      xf->SetMinimum(0.008);
      fPads1->SetLogy();
      can1->Update();

      can1->SaveAs((myOutDir+"/fitPlot_"+channel_marker+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_log.root").c_str());
      can1->SaveAs((myOutDir+"/fitPlot_"+channel_marker+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_log.eps").c_str());
      can1->SaveAs((myOutDir+"/fitPlot_"+channel_marker+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_log.png").c_str());
      can1->SaveAs((myOutDir+"/fitPlot_"+channel_marker+"_"+ssnxj.str()+"J_"+pur_str.c_str()+"_"+leptType+"_log.pdf").c_str());
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

TTree* weightTree(TTree* tree, TH1D* h1_alpha,const std::string& name ,bool verbose,double corrFactor){
  gROOT->cd(); //magic!


  Double_t mZZ;
  tree->SetBranchAddress( "mZZ", &mZZ );

  TTree* newTree = tree->CloneTree(0);
  newTree->SetName(name.c_str());

  Double_t alpha;
  newTree->Branch( "alphaWeight", &alpha, "alphaWeight/D" );

  unsigned nentries = tree->GetEntries();
  //cout<<"Starting to loop over tree inside weightTree. Alpha histo -> "<<h1_alpha->GetNbinsX()<<" bins"<<endl;
  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    tree->GetEntry( iEntry );
    if( (iEntry % 10000)==0 && verbose) std::cout << "weightTree: Entry " << iEntry << "/" << nentries << std::endl;

    int alphabin = h1_alpha->FindBin( mZZ );
    if(alphabin==0 ||alphabin> h1_alpha->GetNbinsX())
      alpha=1.;
    else
      alpha = h1_alpha->GetBinContent( alphabin );

    //	alpha=999.0;
    //alpha=alpha*184.302/72.429;
    alpha=alpha*corrFactor;//205.437/111.167;
    //ofstream logT("Test.txt");
    if(verbose)std::cout<<"weightTree corrFactor = "<<corrFactor<<endl;
    newTree->Fill();

  }
  //	cout<<"Finished to llop over entries insied  weightTree."<<endl;
  //delete tree;
  return newTree;

}
void fitPseudoOnePar( RooDataSet& ModSideband, RooWorkspace& ws ,int seed,char* initialvalues , int nxj,std::string inPurStr, const std::string& leptType){
  //reset parameters
  char var1[50];
  sprintf(var1,"expoFit_%s_%s_%dJ%s_p0",channel_marker.c_str(),leptType.c_str(),nxj,inPurStr.c_str());
  char argname[100];
  sprintf(argname,"%s",var1);
  ws.argSet(argname).readFromFile(initialvalues);
  RooFitResult* r_pseudo = ws.pdf(("exp_fit_"+channel_marker).c_str())->fitTo(ModSideband, SumW2Error(kTRUE), Save(),RooFit::PrintLevel(-1),Range("fitRange") );

  char indexstring[200];
  sprintf(indexstring,"TMPResult_%s_%s_%dJ%s_%d",channel_marker.c_str(),leptType.c_str(),nxj,inPurStr.c_str(),seed);
  RooArgSet tmpset(r_pseudo->floatParsFinal());
  tmpset.writeToFile(indexstring);

}


void fitPseudoTwoPars( RooDataSet& ModSideband, RooWorkspace& ws ,int seed,char* initialvalues , int nxj, int pur , const std::string& leptType ) {
  std::string pur_str="";
  if(pur==0)pur_str="LP";
  if(pur==1)pur_str="HP";
  //reset parameters
  char var1[50];
  char var2[50];
  sprintf(var1,"expLev_%s_%s_%dJ%s_eig0",channel_marker.c_str(),leptType.c_str(),nxj,pur_str.c_str());
  sprintf(var2,"expLev_%s_%s_%dJ%s_eig1",channel_marker.c_str(),leptType.c_str(),nxj,pur_str.c_str());
  char argname[100];
  //  if(nxj==2) sprintf(argname,"%s",var1);
  // else sprintf(argname,"%s,%s",var1,var2);
  sprintf(argname,"%s,%s",var1,var2);
  ws.argSet(argname).readFromFile(initialvalues);

  RooFitResult* r_pseudo = ws.pdf(bkgd_decorr_name.c_str())->fitTo(ModSideband, SumW2Error(kTRUE), Save(),RooFit::PrintLevel(-1),Range("fitRange") );
  // RooFitResult* r_pseudo2 =ws.pdf("levelled_exp_fit")->fitTo(ModSideband, SumW2Error(kTRUE), Save(),RooFit::PrintLevel(-1),Range("fitRange") );
  char indexstring[200];
  sprintf(indexstring,"TMPResult_%s_%s_%dJ%s_%d",channel_marker.c_str(),leptType.c_str(),nxj,pur_str.c_str(),seed);
  RooArgSet tmpset(r_pseudo->floatParsFinal());
  tmpset.writeToFile(indexstring);


  //plot results of fit to pseudo data
  bool plotSinglePseudoFit=false;
  if(seed%20==1 && plotSinglePseudoFit){
    cout<<"Plotting tmp fit for toy #"<<seed<<endl;
    TCanvas *canX=new TCanvas("canvasX", "canX",800,800);
    canX->cd();
    //r_pseudo->printMultiline(cout,99,true);
    //std::cout<<"*** this is the result using a non-decorrelated leveled expo ***"<<std::endl;
    //r_pseudo2->printMultiline(cout,99,true);
    RooPlot *tmpfitfr=ws.var("mZZ")->frame();//mZZtmp->frame();
    tmpfitfr->SetMinimum(0.1);
    //    ModSideband.plotOn(tmpfitfr,MarkerStyle(20),MarkerColor(kBlue));
    double normTMP=ModSideband.reduce(CutRange("fitRange"))->sumEntries();
    //    ModSideband.Print("v");
    ModSideband.reduce(CutRange("fitRange"))->plotOn(tmpfitfr,MarkerStyle(20),MarkerColor(kBlue));
    cout<<"CP4 "<<normTMP<<"  "<<flush;
    ws.pdf(bkgd_decorr_name.c_str())->plotOn(tmpfitfr,Normalization(normTMP,RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"),RooFit::Range("plotRange"), LineColor(kOrange+3));
    //ws.pdf("levelled_exp_fit")->plotOn(tmpfitfr,Normalization(normTMP,RooAbsPdf::NumEvent),RooFit::NormRange("fitRange"),RooFit::Range("plotRange"), LineColor(kRed));

    ModSideband.reduce(CutRange("fitRange"))->plotOn(tmpfitfr,MarkerStyle(20),MarkerColor(kBlue));
    tmpfitfr->GetYaxis()->SetRangeUser(0.01,500.0);
    gPad->SetLogy();
    tmpfitfr->Draw();

    cout<<"CP6 "<<endl;
    char canName1[128];
    sprintf(canName1,"TMPFit_%s_%s_%dJ%s_%d.png",channel_marker.c_str(),leptType.c_str(),nxj,pur_str.c_str(),seed);
    canX->SaveAs(canName1);
    // delete mZZtmp;
    delete tmpfitfr;
    delete canX;
    cout<<"Finished to make tmp fit plot"<<endl;
  }//end if seed %20==1


}

void pseudoMassgeOnePar(int nxj ,std::string inPurStr, const std::string & leptType, RooFitResult* r_nominal, RooWorkspace& ws,char* initialvalues, double NormRelErr, RooRealVar &errV1){
  char var1[50];

  sprintf(var1,"expoFit_%s_%s_%dJ%s_p0",channel_marker.c_str(),leptType.c_str(),nxj,inPurStr.c_str());//this must be equal to what is in fitPseudo
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

  bool plotEnvelope=false;

  cout<<"pseudoMassgeOnePar starts to loop over "<<nToys<<" toys with initial value "<<x1<<"+/-"<<e1<<endl;

  for(unsigned i =0 ; i < nToys ; i++){
    sprintf(indexstring,"TMPResult_%s_%s_%dJ%s_%d",channel_marker.c_str(),leptType.c_str(),nxj,inPurStr.c_str(),i);
    //  ifstream testFile(indexstring,ios::in);
    // if(! testFile.good())continue;
    ws.argSet(argname).readFromFile(indexstring);
    if(fabs(ws.var(var1)->getVal())>4.0 ){
      cout<<"Toy #"<<i<<" did not have successful fit "<< ws.var(var1)->getVal()<<endl;
      continue;
    }
    vals1.push_back(ws.var(var1)->getVal());

    if(plotEnvelope ){
      cout<<"(in pseudoMassgeOnePar) Plot expo #"<<i<<" with "<<var1<<"="<<ws.var(var1)->getVal()<<endl;
      ws.pdf(("exp_fit_"+channel_marker).c_str())->plotOn(plot_MCbkg,LineWidth(1),LineColor(1));
      cout<<"Finished to Plot expo fit #"<<i<<endl;
      // char nameTmpFit[64];
      // sprintf(nameTmpFit,"%s_%d",bkgd_decorr_name.c_str(),i);
      // ws.pdf(bkgd_decorr_name.c_str())->plotOn(plot_MCbkg,LineWidth(1),LineColor(1));//,Name(nameTmpFit));
      // ws.pdf(nameTmpFit)->plotOn(plot_MCbkg,LineWidth(1),LineColor(1),Name(nameTmpFit));      
    }//end if plotenvelope

    std::string mkdir_command("rm ");  
    mkdir_command+= indexstring;
    system(mkdir_command.c_str());
    if(i%100==0)    std::cout << mkdir_command << std::endl;
  }
  cout<<"pseudoMassgeOnePar finished to loop over "<<nToys<<" toys"<<endl;


  char canvasName[400];
  sprintf( canvasName, "%s/mZZ_fitSidebandData_alphaFromToys_%s_%s_%dJ%s_%s", myOutDir.c_str(),leptType.c_str(),channel_marker.c_str(), nxj,inPurStr.c_str(), "ALL");
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
    ws.pdf(("exp_fit_"+channel_marker).c_str())->plotOn(plot_MCbkg,VisualizeError(*r_nominal,2.0,kFALSE),FillColor(kYellow));
    ws.pdf(("exp_fit_"+channel_marker).c_str())->plotOn(plot_MCbkg,VisualizeError(*r_nominal,1.0,kFALSE),FillColor(kGreen));
    ws.pdf(("exp_fit_"+channel_marker).c_str())->plotOn(plot_MCbkg);
    ws.pdf(("exp_fit_"+channel_marker).c_str())->plotOn(plot_MCbkg, LineColor(kViolet-2), LineStyle(kDashed), Normalization(1.0+NormRelErr,RooAbsReal::Relative));
    ws.pdf(("exp_fit_"+channel_marker).c_str())->plotOn(plot_MCbkg, LineColor(kViolet-2), LineStyle(kDashed), Normalization(1.0-NormRelErr,RooAbsReal::Relative));
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
    sprintf(dumname,"%s/rooCurves_%s_%s_%dJ%s_ALL.root",myOutDir.c_str(),leptType.c_str(),channel_marker.c_str(),nxj,inPurStr.c_str());
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

  // plotting value distributions; remember that in the case of one param we did not decorrelated it
  //this means that the value of the new param is not centered at zero with err=1
  std::sort(vals1.begin(),vals1.end());
  //	float lowAlphaRange=((vals1[0]<-3.0)?vals1[0]:-3.0) ;
  //float hiAlphaRange=((vals1[nToys-1]>+3.0)?vals1[nToys-1] : +3.0);
  float lowAlphaRange=vals1[0];
  float hiAlphaRange=vals1[nToys-1];
  TH1F* histo1= new TH1F("test1","test1",100,lowAlphaRange,hiAlphaRange);
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
  gStyle->SetOptFit(111);
  histo1->Draw();
  line->SetLineColor(2);
  line->DrawLine(x1,histo1->GetMinimum(),x1,histo1->GetMaximum());
  line->SetLineColor(4);
  line->DrawLine(x1+e1,histo1->GetMinimum(),x1+e1,histo1->GetMaximum());
  line->DrawLine(x1-e1,histo1->GetMinimum(),x1-e1,histo1->GetMaximum());


  sprintf( canvasName, "%s/alphaSystErr_par1_%s_%s_%dJ%s_%s", myOutDir.c_str(),leptType.c_str(),channel_marker.c_str(), nxj,inPurStr.c_str(), "ALL");
  canvasName_str = canvasName;
  canvasName_eps = canvasName_str + ".eps";
  canvasName_png = canvasName_str + ".png";
  c1->SaveAs(canvasName_eps.c_str());
  c1->SaveAs(canvasName_png.c_str());

  errV1.setVal(s1);
  errV1.setConstant(kTRUE);
  gStyle->SetOptFit(0);
  std::cout<<"Finished pseudoMassgeOnePar ! Error value is "<<s1<<std::endl;
}//end pseudoMassgeOnePar


void pseudoMassgeTwoPars(int nxj ,int pur, const std::string& leptType , RooFitResult* r_nominal, RooWorkspace& ws,char* initialvalues, double NormRelErr, RooRealVar &errV1, RooRealVar &errV2){
  std::string pur_str="";
  if(pur==0)pur_str="LP";
  if(pur==1)pur_str="HP";

  char var1[50];
  char var2[50];
  sprintf(var1,"expLev_%s_%s_%dJ%s_eig0",channel_marker.c_str(),leptType.c_str(),nxj,pur_str.c_str());//this must be equal to what is in fitPseudo
  sprintf(var2,"expLev_%s_%s_%dJ%s_eig1",channel_marker.c_str(),leptType.c_str(),nxj,pur_str.c_str());//this must be equal to what is in fitPseudo
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

  bool plotEnvelope=false;

  cout<<"pseudoMassgeTwoPars starts to loop over "<<nToys<<" toys"<<endl;
  for(unsigned i =0 ; i < nToys ; i++){
    sprintf(indexstring,"TMPResult_%s_%s_%dJ%s_%d",channel_marker.c_str(),leptType.c_str(),nxj,pur_str.c_str(),i);
    //    sprintf(indexstring,"TMPResult_%dtag_%d",nxj,i);
    //  ifstream testFile(indexstring,ios::in);
    // if(! testFile.good())continue;
    ws.argSet(argname).readFromFile(indexstring);
    if(fabs(ws.var(var1)->getVal())>4.0 || fabs(ws.var(var2)->getVal())>4.0 ){
      cout<<"Toy #"<<i<<" did not have successful fit "<< ws.var(var1)->getVal()<<"  "<<ws.var(var2)->getVal()<<endl;
      continue;
    }
    vals1.push_back(ws.var(var1)->getVal());
    vals2.push_back(ws.var(var2)->getVal());
    if(plotEnvelope&&(i%20==1) ){
      cout<<"Plot levexp_dcr #"<<i<<" with "<<var1<<"="<<ws.var(var1)->getVal()<<"  "<<var2<<"="<<ws.var(var2)->getVal()<<endl;
      ws.pdf(bkgd_decorr_name.c_str())->plotOn(plot_MCbkg,LineWidth(1),LineColor(1));
      //RooAbsPdf *tmpBkgPdf= ws.pdf(bkgd_decorr_name.c_str());
      // char pdfNewName[128];
      //sprintf(pdfNewName,"%s_%dJ%s_%d",bkgd_decorr_name.c_str(),nxj,pur_str.c_str(),i);
      //tmpBkgPdf->SetName(pdfNewName);
      //tmpBkgPdf->plotOn(plot_MCbkg,LineWidth(1),LineColor(1));

      // char nameTmpFit[64];
      // sprintf(nameTmpFit,"%s_%d",bkgd_decorr_name.c_str(),i);
      // ws.pdf(bkgd_decorr_name.c_str())->plotOn(plot_MCbkg,LineWidth(1),LineColor(1));//,Name(nameTmpFit));
      // ws.pdf(nameTmpFit)->plotOn(plot_MCbkg,LineWidth(1),LineColor(1),Name(nameTmpFit));      


      cout<<"Finished to Plot levexp_dcr #"<<i<<endl;
    }//end if plotenvelope

    std::string mkdir_command("rm ");  
    mkdir_command+= indexstring;
    system(mkdir_command.c_str());
    if(i%100==0)    std::cout << mkdir_command << std::endl;
  }
  cout<<"pseudoMassgeTwoPars finished to loop over "<<nToys<<" toys"<<endl;


  char canvasName[400];
  sprintf( canvasName, "%s/mZZ_fitSidebandData_alphaFromToys_%s_%s_%dJ%s_%s", myOutDir.c_str(),channel_marker.c_str(),leptType.c_str(), nxj,pur_str.c_str(), "ALL");
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
    ws.pdf(bkgd_decorr_name.c_str())->plotOn(plot_MCbkg,VisualizeError(*r_nominal,2.0,kFALSE),FillColor(kYellow));
    ws.pdf(bkgd_decorr_name.c_str())->plotOn(plot_MCbkg,VisualizeError(*r_nominal,1.0,kFALSE),FillColor(kGreen));
    ws.pdf(bkgd_decorr_name.c_str())->plotOn(plot_MCbkg);
    ws.pdf(bkgd_decorr_name.c_str())->plotOn(plot_MCbkg, LineColor(kViolet-2), LineStyle(kDashed), Normalization(1.0+NormRelErr,RooAbsReal::Relative));
    ws.pdf(bkgd_decorr_name.c_str())->plotOn(plot_MCbkg, LineColor(kViolet-2), LineStyle(kDashed), Normalization(1.0-NormRelErr,RooAbsReal::Relative));
    plot_MCbkg->addPlotable(lower);
    plot_MCbkg->addPlotable(upper);


    /*
      double x0,up0,low0;
      upper->GetPoint(0,x0,up0);  
      lower->GetPoint(0,x0,low0);  
      RooRealVar *r0=new RooRealVar("mZZ","mZZ",x0);
      RooArgSet *tmparg=new RooArgSet(*r0);
      double central0=1.0;//ws.pdf(bkgd_decorr_name.c_str())->getValV();
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
    sprintf(dumname,"%s/rooCurves_%s_%s_%dJ%s_ALL.root",myOutDir.c_str(),channel_marker.c_str(),leptType.c_str(),nxj,pur_str.c_str());
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
  float lowAlphaRange=((vals1[0]<-3.0)?vals1[0]:-3.0) ;
  float hiAlphaRange=((vals1[nToys-1]>+3.0)?vals1[nToys-1] : +3.0);
  TH1F* histo1= new TH1F("test1","Distribution of par1 in pseudo-exp",100,lowAlphaRange,hiAlphaRange);
  char tit[20];
  sprintf(tit,"#alpha_{%d}",nxj);
  histo1->GetXaxis()->SetTitle(tit);
  histo1->GetYaxis()->SetTitle("Nr trials");

  std::sort(vals2.begin(),vals2.end());
  lowAlphaRange=((vals2[0]<-3.0)?vals2[0]:-3.0) ;
  hiAlphaRange=((vals2[nToys-1]>+3.0)?vals2[nToys-1] : +3.0);
  TH1F* histo2= new TH1F("test2","Distribution of par2 in pseudo-exp",100,lowAlphaRange,hiAlphaRange);
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
  gStyle->SetOptFit(111);
  histo1->Draw();
  line->SetLineColor(2);
  line->DrawLine(x1,histo1->GetMinimum(),x1,histo1->GetMaximum());
  line->SetLineColor(4);
  line->DrawLine(x1+e1,histo1->GetMinimum(),x1+e1,histo1->GetMaximum());
  line->DrawLine(x1-e1,histo1->GetMinimum(),x1-e1,histo1->GetMaximum());


  sprintf( canvasName, "%s/alphaSystErr_par1_%s_%s_%dJ%s_%s", myOutDir.c_str(),channel_marker.c_str(),leptType.c_str(), nxj,pur_str.c_str(), "ALL");
  canvasName_str = canvasName;
  canvasName_eps = canvasName_str + ".eps";
  canvasName_png = canvasName_str + ".png";
  c1->SaveAs(canvasName_eps.c_str());
  c1->SaveAs(canvasName_png.c_str());

  gStyle->SetOptFit(111);
  histo2->Draw();
  line->SetLineColor(2);
  line->DrawLine(x2,histo2->GetMinimum(),x2,histo2->GetMaximum());
  line->SetLineColor(4);
  line->DrawLine(x2+e2,histo2->GetMinimum(),x2+e2,histo2->GetMaximum());
  line->DrawLine(x2-e2,histo2->GetMinimum(),x2-e2,histo2->GetMaximum());

  sprintf( canvasName, "%s/alphaSystErr_par2_%s_%s_%dJ%s_%s", myOutDir.c_str(),channel_marker.c_str(),leptType.c_str(), nxj,pur_str.c_str(), "ALL");
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
  gStyle->SetOptFit(0);
  std::cout<<"Finished pseudoMassge !"<<std::endl;
}//end pseudoMassgeTwoPars



void CopyTreeVecToPlain(TTree *t1, std::string wType, std::string f2Name,std::string t2Name,int nxjCut){

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

      // if(isZZChannel&&(mynxj_2==1&&mZqq_2>110.0)){
      //std::cout<<"Excluding evt "<<nevt_2<<" because in upper SB -> MJ="<<mZqq_2<<std::endl;
      //	region_2=2;
      // }

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


void CreateUnrolledTreesDATA(std::string inDir,std::string tmpFileName,std::string tmpTreeName,std::string weighting,int nxjCut,ostream &outlog){
  outlog<<"Creating unrolled tree for DATA in "<<inDirSIG.c_str()<<endl;

  TChain* chainData = new TChain(InTreeName.c_str());
  if(isZZChannel)
    {
      chainData->Add( (inDir+"treeEDBR_DoubleMuParked_Run2012*.root").c_str()  );
      chainData->Add( (inDir+"treeEDBR_DoubleMu_Run2012A*.root").c_str()  );
      chainData->Add( (inDir+"treeEDBR_DoublePhotonHighPt_Run2012*.root").c_str()  );
      chainData->Add( (inDir+"treeEDBR_Photon_Run2012A*.root").c_str()  );
    }
  else
    {
      chainData->Add( (inDirSIG+"treeEDBR_data_xww.root").c_str()  );
    }
  outlog<<"In the data chain there are "<<chainData->GetEntries()<<" events"<<endl;
  CopyTreeVecToPlain(chainData,weighting,tmpFileName,tmpTreeName,nxjCut);//(TTree*)
  delete chainData;


}//end CreateUnrolledTreesDATA()




void CreateUnrolledTreesDYMCSIG(std::string inDirSIG,std::string tmpFileName,std::string tmpTreeName,std::string weighting,int nxjCut,ostream &outlog){

  outlog<<"Creating unrolled tree for DY MC"<<endl;
  TChain* chainDYSig = new TChain(InTreeName.c_str());
 
  if(isZZChannel){
    chainDYSig->Add( (inDirSIG+"treeEDBR_DYJetsPt70To100.root").c_str()  );
    chainDYSig->Add( (inDirSIG+"treeEDBR_DYJetsPt100.root").c_str()  );
  }//end if isZZChannel


  CopyTreeVecToPlain(chainDYSig,weighting,tmpFileName,tmpTreeName,nxjCut);
  delete chainDYSig;
 

}//end CreateUnrolledTreesDYMC()


void CreateUnrolledTreesVVMCSIG(std::string inDirSIG,std::string tmpFileName,std::string tmpTreeName,std::string weighting,int nxjCut,ostream &outlog){

  outlog<<"Creating unrolled tree for VV MC (signal region)"<<endl;
  TChain* chainVVSig = new TChain(InTreeName.c_str());
  if(isZZChannel){
    chainVVSig->Add( (inDirSIG+"treeEDBR_WW.root").c_str()  );
    chainVVSig->Add( (inDirSIG+"treeEDBR_WZ.root").c_str()  );
    chainVVSig->Add( (inDirSIG+"treeEDBR_ZZ.root").c_str()  );
  }//end if isZZChannel
  else{
    chainVVSig->Add( (inDirSIG+"treeEDBR_TTBARpowheg_xww.root").c_str());
    chainVVSig->Add( (inDirSIG+"treeEDBR_SingleTop_xww.root").c_str());
    chainVVSig->Add( (inDirSIG+"treeEDBR_VV_xww.root").c_str());
  }

  CopyTreeVecToPlain(chainVVSig,weighting,tmpFileName,tmpTreeName,nxjCut);
  delete chainVVSig;


}//end CreateUnrolledTreesVVMCSIG()


double getExtNormalization(int purityCut,double &extNorm, double &extNormELE, double &extNormMU){

  if(InTreeName=="SelectedCandidatesAB"){ // for closure test A->B
    if(!isZZChannel){//WW
      if(leptType=="ELE"){
	if(purityCut==0)extNormELE=91.7039;
	if(purityCut==1)extNormELE=121.074;
	extNorm=extNormELE;
      }
      if(leptType=="MU"){
	if(purityCut==0)extNormMU=175.271;
	if(purityCut==1)extNormMU=198.2;
	extNorm=extNormMU;
      }
    }
  }
  else if(InTreeName=="SelectedCandidates"){
    if(isZZChannel){
	      //LP norm calculated in [650, 2800]
      if(leptType=="ELE"||leptType=="ALL"){
	if(purityCut<2)extNormELE=extNorm_1J[0][purityCut];
	else{
	  cout<<"\n\nERROR !!! Purity Category out of range !!! Purity=="<<purityCut<<endl;
	  extNormELE= -99.0; //uhi uhi, something bad's gonna happen...
	}
	extNorm= extNormELE;
      }
      if(leptType=="MU"||leptType=="ALL"){
	if(purityCut<2)extNormMU=extNorm_1J[1][purityCut];
	else{
	  cout<<"\n\nERROR !!! Purity Category out of range !!! Purity=="<<purityCut<<endl;
	  extNormMU=-99.0;//uhi uhi, something bad's gonna happen...
	}
	extNorm= extNormMU;
      }
      if(leptType=="ALL"){
	extNorm= extNormMU+extNormELE;
      }
    }
    else{//WW analysis; for Ana , wjets 180
      if(leptType=="ELE"||leptType=="ALL"){
	if(purityCut<2)extNormELE=extNorm_1J[0][purityCut];
	else{
	  cout<<"\n\nERROR !!! Purity Category out of range !!! Purity=="<<purityCut<<endl;
	  extNormELE=-99.0;//uhi uhi, something bad's gonna happen...
	}
	extNorm= extNormELE;
      }
      if(leptType=="MU"||leptType=="ALL"){
	if(purityCut<2)extNormMU=extNorm_1J[1][purityCut];
	else{
	  cout<<"\n\nERROR !!! Purity Category out of range !!! Purity=="<<purityCut<<endl;
	  extNormMU=-99.0;//uhi uhi, something bad's gonna happen...
	}
	extNorm= extNormMU;
      }
      if(leptType=="ALL"){
	extNorm= extNormMU+extNormELE;
      }
    }
  }
  else{
    cout<<"WARNING !!! Requested to normalize V+jets bkgd to externally provided values, but tree name does not match. TreeName: "<<InTreeName.c_str()<<std::endl;
    extNormMU=-99.0;
    extNormELE=-99.0;
    extNorm=extNormMU+extNormELE;
    return -1.0;
  }
  
  return extNorm;

}
