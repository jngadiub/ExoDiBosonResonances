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
#include "TGraphErrors.h"
#include "TLegend.h"
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

  //	TString inputpath = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv12/fullrange/";
  TString inputpath = "/afs/cern.ch/user/b/bonato/scratch0/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv2i/fullsig/";
  TString cut = "absolute_efficiency";
  vector<TString> dataSamples;
  vector<TString> bkgSamples;
  vector<TString> sigSamples;


  bool weightedeff = true;
  bool isZZChannel = true;
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
  system("mkdir -p SignalEffPlots_RS1PYTHIA");
  ofstream outFile("SignalEffPlots_RS1PYTHIA/efficiencies_MCSig.txt");
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
//      bkgSamples.push_back("TTBARpowheg");  
 //     bkgSamples.push_back("WW");
   //   bkgSamples.push_back("WZ");
     // bkgSamples.push_back("ZZ");
      // bkgSamples.push_back("DYJetsPt50To70");
      //		bkgSamples.push_back("DYJetsPt70To100");
      //bkgSamples.push_back("DYJetsPt100");


      /*
      sigSamples.push_back("WprimeWZ_inclusive_M750_PYTHIA6");
      sigSamples.push_back("WprimeWZ_inclusive_M1000_PYTHIA6");
      sigSamples.push_back("WprimeWZ_inclusive_M1500_PYTHIA6");
      sigSamples.push_back("WprimeWZ_inclusive_M2000_PYTHIA6");
      sigSamples.push_back("WprimeWZ_inclusive_M2200_PYTHIA6");
      sigSamples.push_back("WprimeWZ_inclusive_M2500_PYTHIA6");
      */
     
      
      sigSamples.push_back("RSG_ZZ_lljj_c0p05_M750_PYTHIA6");
      sigSamples.push_back("RSG_ZZ_lljj_c0p05_M1000_PYTHIA6");
      sigSamples.push_back("RSG_ZZ_lljj_c0p05_M1250_PYTHIA6");
      sigSamples.push_back("RSG_ZZ_lljj_c0p05_M1500_PYTHIA6");
      sigSamples.push_back("RSG_ZZ_lljj_c0p05_M1750_PYTHIA6");
      sigSamples.push_back("RSG_ZZ_lljj_c0p05_M2000_PYTHIA6");
     	 sigSamples.push_back("RSG_ZZ_lljj_c0p05_M1000");
      sigSamples.push_back("RSG_ZZ_lljj_c0p2_M1000");
      sigSamples.push_back("RSG_ZZ_lljj_c0p2_M1500");
      

      /*
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
      sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M2100");
      sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M2200");
      sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M2300");
      sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M2400");
      sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M2500");
      

      sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M700");
      sigSamples.push_back("BulkG_ZZ_lljj_M700_G40");
      sigSamples.push_back("BulkG_ZZ_lljj_M700_G80");
      sigSamples.push_back("BulkG_ZZ_lljj_M700_G120");
      sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M1000");
      sigSamples.push_back("BulkG_ZZ_lljj_M1000_G40");
      sigSamples.push_back("BulkG_ZZ_lljj_M1000_G80");
      sigSamples.push_back("BulkG_ZZ_lljj_M1000_G120");
      sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M1500");
      sigSamples.push_back("BulkG_ZZ_lljj_M1500_G40");
      sigSamples.push_back("BulkG_ZZ_lljj_M1500_G80");
      sigSamples.push_back("BulkG_ZZ_lljj_M1500_G120");
      sigSamples.push_back("BulkG_ZZ_lljj_c0p2_M2000");
      sigSamples.push_back("BulkG_ZZ_lljj_M2000_G40");
      sigSamples.push_back("BulkG_ZZ_lljj_M2000_G80");
      sigSamples.push_back("BulkG_ZZ_lljj_M2000_G120");
      sigSamples.push_back("BulkG_ZZ_lljj_M2000_G160");
      sigSamples.push_back("BulkG_ZZ_lljj_M2000_G200");
      */

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
	  	
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M600_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M700_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M800_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M900_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M1000_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M1100_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M1200_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M1300_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M1400_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M1500_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M1600_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M1700_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M1800_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M1900_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M2000_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M2100_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M2200_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M2300_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M2400_xww");
      sigSamples.push_back("BulkG_WW_inclusive_c0p2_M2500_xww");
	
      /*
	  sigSamples.push_back("WprimeWZ_inclusive_M750_PYTHIA6_xww");
	  sigSamples.push_back("WprimeWZ_inclusive_M1000_PYTHIA6_xww");
      sigSamples.push_back("WprimeWZ_inclusive_M1500_PYTHIA6_xww");
      sigSamples.push_back("WprimeWZ_inclusive_M1800_PYTHIA6_xww");
      sigSamples.push_back("WprimeWZ_inclusive_M2000_PYTHIA6_xww");
      sigSamples.push_back("WprimeWZ_inclusive_M2200_PYTHIA6_xww");
      sigSamples.push_back("WprimeWZ_inclusive_M2500_PYTHIA6_xww");
      sigSamples.push_back("WprimeWZ_inclusive_M3000_PYTHIA6_xww");
      sigSamples.push_back("WprimeWZ_inclusive_M4000_PYTHIA6_xww");

	  sigSamples.push_back("RSG_WW_lvjj_c0p05_M750_PYTHIA6_xww");
      sigSamples.push_back("RSG_WW_lvjj_c0p05_M1000_PYTHIA6_xww");
      sigSamples.push_back("RSG_WW_lvjj_c0p05_M1250_PYTHIA6_xww");
      sigSamples.push_back("RSG_WW_lvjj_c0p05_M1500_PYTHIA6_xww");
      sigSamples.push_back("RSG_WW_lvjj_c0p05_M1750_PYTHIA6_xww");
      sigSamples.push_back("RSG_WW_lvjj_c0p05_M2000_PYTHIA6_xww");
	  */
 
    }
  const int nCat=6;//EE1JHP, MM1JHP, EE1JLP, MM1JLP, EE2J, MM2J
  const int ndata = dataSamples.size();
  const int nbkg  = bkgSamples.size();
  const int nsig  = sigSamples.size();

  double SigEff[nsig][nCat];
  double SigNgen[nsig][nCat];
  int isig=0;
  int iCat=0;

  for(int i=0;i<nsig;i++){
    for(int j=0;j<nCat;j++){
      SigNgen[i][j]=0.0;
    }
  }

  //order of categories:EE1JHP, MM1JHP, EE1JLP, MM1JLP, EE2J, MM2J
  for(int iNJ=1;iNJ<=2;iNJ++){
    double nxjCut=double(iNJ);

    for(int iPur=1;iPur>=0;iPur--){
      if(iNJ==2&&iPur==0)continue;//for 2 jet only do the first loop
      double purCut=double(iPur);
      for(int ilep=0;ilep<2;ilep++){
	double lepCut=double(ilep);


	std::vector<double> sig_effs, sig_masses;
	cout<<"NJets="<<iNJ<<"   "<<(ilep==0? "ELE" : "MU")<<"   "<<(iPur==0 ? "LP" : "HP")<<endl;
	cout<<"data samples "<<ndata<<"  bkg samples "<<nbkg<<"  signal samples "<<nsig<<endl;	

	TH1F * h1 = new TH1F (cut,cut,ndata+nbkg+nsig,0,ndata+nbkg+nsig);
	TH1F * h1b = new TH1F ("Efficiency","Efficiency",nsig,550,550+100*nsig);
	//h1->GetYaxis()->SetRangeUser(0,1);
	h1->SetBit(TH1::kCanRebin);
	h1->SetStats(0);
	gStyle->SetPaintTextFormat("1.4f");
	//h1->Sumw2();

	TCanvas * c1 = new TCanvas();	

	int indM=0;
	double startM=750.0; double step=250.0;
	if(!isZZChannel){
		startM=600.0; 
		step=100.0;
	}
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
		samplename = sigSamples.at(i-ndata-nbkg);//RSG_WW_inclusive_c0p2_M1500_xww,BulkG_WW_inclusive_c1p0_M1500_xww
		if(!isZZChannel){
		  //this removes "_xww"
		  samplename.Remove(samplename.Length()-4,4);//RSG_WW_inclusive_c0p2_M1500,BulkG_WW_inclusive_c1p0_M1500
		  //this removes "WW_inclusive_"
		  if(samplename.Contains("RSG"))samplename.Remove(4,13);//RSG_c0p2_M1500
		  if(samplename.Contains("BulkG"))samplename.Remove(6,13);//BulkG_c1p0_M1500
		}
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
	    double vtag[99];
	    int    nCands;
	    int    nXjets[99];
	    int nLooseEle;
	    int nLooseMu;
	    //per event weight
	    double TOTweight=-1;
	    double HLTweight=-1;
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
	    tree->SetBranchAddress("vTagPurity", vtag);
	    tree->SetBranchAddress("nCands", &nCands);

	    tree->SetBranchAddress("weight", &TOTweight);
	    tree->SetBranchAddress("HLTweight", &HLTweight);
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

		if(total==0){//something wrong with this sample
		  cout<<samplename<<"  has "<<Ngen<<" generated events. Skipping sample"<<endl;
		  break;
		}


		//per event weight
		double actualWeight = TOTweight; //HLTweight*PUweight*LumiWeight*GenWeight*lumi;
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
		      if(isZZChannel)
			{
			  if(vtag[ivec]!=purCut&&iNJ!=2)continue;
			}
		      if(!isZZChannel)
			{
			  //for nsubjtiness
			  if(iPur==1&&nsubj21[ivec]<0.5);
			  else if(iPur==0&&(nsubj21[ivec]>0.5&&nsubj21[ivec]<0.75));
			  else if(nxjCut==2);
			  else continue;
			  ////
			  if((nLooseEle+nLooseMu)!=1)continue;
			  //if(ptZll[ivec]<200)continue;
			  if(nbtagsM[ivec]!=0)continue; //b-tag veto
			  if(deltaR_LJ<1.57 || deltaPhi_JMET<2. || deltaPhi_JWL<2.)continue;

			  if(lepCut==0)
			    {
			      if(met<80)continue;//for electron, use this cut
			      //if(eleid_passConversionVeto[ivec]!=1)continue;
			      //if(eleid_numberOfLostHits[ivec]!=0)continue;
			    }


			}
		      pass+=actualWeight;

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

	    double BR_sf=1.0;

	    if(samplename.Contains("BulkG") || samplename.Contains("RSG") || samplename.Contains("WprimeWZ") ){
	      int binToFill=h1b->FindBin(indM*step+startM);
	      cout<<"Filling IndM="<<indM<<"  M="<<indM*step+startM<<"  Bin#"<<binToFill<<"  Eff="<<eff<<endl;
	    
	      if(samplename.Contains("lljj") ||samplename.Contains("lvjj") ){
		BR_sf=1.0;
		if(samplename.Contains("RSG")&&samplename.Contains("PYTHIA")){//taus are also in !
		  BR_sf=2.0/3.0;
		}
	      }//end if samplename.Contains("lljj")
	      else if(samplename.Contains("inclusive")){
		if(isZZChannel){
		  BR_sf=0.0941;
		  if(samplename.Contains("WprimeWZ")) BR_sf=0.0452;
		}
		else{//WW ->lv qq branching ratios
		  BR_sf=0.2877;
		  if(samplename.Contains("WprimeWZ")) BR_sf=0.1386; // BR(W->lv) * BR(Z->qq) = (0.105+0.105) * 0.66 
		}
	      }//end if (samplename.Contains("inclusive")){
	      else {
		cout<<"\nWARNING !!! Impossible to identify the process of the signal from the sample name: "<<samplename<<" -> assuming full chain llqq with l=e or mu ."<<endl;
		BR_sf=1.0;
	      }

	      h1b->SetBinContent(binToFill,eff/BR_sf);
	      sig_masses.push_back(indM*step+startM);
	      sig_effs.push_back(eff/BR_sf);
	      indM++;

	      cout<<"Array indexes: "<<isig<<"  "<<iCat<<endl;
	      SigEff[isig][iCat]=eff/BR_sf;
	      SigNgen[isig][iCat]=Ngen;//last entry will be used, should be ok because always the same 
	      isig++;
	      if(isig==nsig){//finished last sample
		iCat++;
	      }
	    }//end if sample name is a signal sample name

	    h1->Fill(samplename,eff);	
	  }//end of sample loop

	c1->SetGridy(1);

	/// Here you choose the functional form of the efficiency.
	/// You have to be careful with the initial values of the parameters...
	bool fitEff=false;
	TF1 *fitFunc = 0;
	if(fitEff){
	  fitFunc = new TF1("fitPoly2","tanh([0]*(x-[1]))*([2]+[3]*x)",580,2550);
	  fitFunc->SetLineColor(kRed);
	  fitFunc->SetLineWidth(2);
	  fitFunc->SetParameter(0,-0.002);
	  fitFunc->SetParameter(1,300);
	  fitFunc->SetParameter(2,-0.3);
	  fitFunc->SetParameter(3,3E-5);
	  fitFunc->SetLineColor(kRed);
	  h1b->Fit(fitFunc,"R");
	  
	  cout<<"\n\nEff extrapolated at 2000 GeV: "<<fitFunc->Eval(2000.0)<<endl;
	}

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
	h1b->SetMinimum(0.0);
	h1b->SetMaximum(0.25);
	h1b->Draw("P");
	if(fitEff)fitFunc->Draw("Lsame");
	c2->SaveAs("SignalEffPlots_RS1PYTHIA/efficiencyAndFit"+SaveName+".png");
	c2->SaveAs("SignalEffPlots_RS1PYTHIA/efficiencyAndFit"+SaveName+".pdf");

	cout<<"data samples "<<ndata<<"  bkg samples "<<nbkg<<"  signal samples "<<nsig<<endl;	
	cout<<"\nEfficiencies for Lep="<<lepCut<<"  nXjets="<<nxjCut<<" : "<<endl;
	for(unsigned int im=0;im<sig_effs.size();im++){
	  cout<<"M="<<sig_masses.at(im)<<"  Eff="<<sig_effs.at(im)<<endl;
	}
      }//end of lepCut loop
    }//end of purity loop
  }//end of nxjCut loop

  /*
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
  */


  /*
  for(int is =0; is<nsig; is++)
    {
      double MAss = 600+100*is;
      if(is<4)outFile<<"700";
      else if(is<8)outFile<<"1000";
      else if(is<12)outFile<<"1500";
      else outFile<<"2000"; 
      for(int ic=0;ic<nCat; ic++)
	{
	  outFile<<"		"<<SigEff[is][ic];
	}
      outFile<<endl;

    }//end loop on samples


  TGraphErrors *grEff[4];
  for(int ic=0;ic<nCat; ic++){

    const  int nPmax=6;
    double width[nPmax]={0.0,40.0,80,120.0,160.0,200.0};
    double eff[4][nPmax], effErr[4][nPmax];
    
    string catLab="dummy";
    if(ic==0)catLab="EEHP";
    else if(ic==1)catLab="MMHP";
    else if(ic==2)catLab="EELP";
    else if(ic==3)catLab="MMLP";
    else catLab="dummyDUMMY";

    //    int iP=0;
    for(int is =0; is<nsig; is++)
      {
	
	double effTMP=SigEff[is][ic];
	if(is==2)effTMP*=(10698.0/8024.0);//M1500 G80 has less processed events
	if(is==10)effTMP*=(10692.0/5346.0);//M1500 G80 has less processed events
	double errTMP=sqrt((effTMP*(1.0-effTMP))/SigNgen[is][ic]);
	if(is<4){
	  eff[0][is]=effTMP;
	  effErr[0][is]=errTMP;
	}
	else if(is<8){
	  eff[1][is-4]=effTMP;
	  effErr[1][is-4]=errTMP;
	}
	else if(is<12){
	  eff[2][is-8]=effTMP;
	  effErr[2][is-8]=errTMP;
	}
	else {
	  eff[3][is-12]=effTMP;
	  effErr[3][is-12]=errTMP;
	}

      }//end loop on samples



    grEff[0]=new TGraphErrors(4,width,eff[0],0,effErr[0]);
    grEff[0]->SetName(("grEff_vs_Width_M700_"+catLab).c_str());
    grEff[0]->SetMarkerSize(1.5);
    grEff[0]->SetMarkerColor(kBlack);
    grEff[0]->SetMarkerStyle(20);
    grEff[0]->GetXaxis()->SetTitle("Natural Width [GeV]");
    grEff[0]->GetYaxis()->SetTitle("Efficiency");

    grEff[1]=new TGraphErrors(4,width,eff[1],0,effErr[1]);
    grEff[1]->SetName(("grEff_vs_Width_M1000_"+catLab).c_str());
    grEff[1]->SetMarkerSize(1.5);
    grEff[1]->SetMarkerColor(kRed);
    grEff[1]->SetMarkerStyle(21);
    grEff[1]->GetXaxis()->SetTitle("Natural Width [GeV]");
    grEff[1]->GetYaxis()->SetTitle("Efficiency");
    
    grEff[2]=new TGraphErrors(4,width,eff[2],0,effErr[2]);
    grEff[2]->SetName(("grEff_vs_Width_M1500_"+catLab).c_str());
    grEff[2]->SetMarkerSize(1.8);
    grEff[2]->SetMarkerColor(kBlue);
    grEff[2]->SetMarkerStyle(22);
    grEff[2]->GetXaxis()->SetTitle("Natural Width [GeV]");
    grEff[2]->GetYaxis()->SetTitle("Efficiency");
    
    grEff[3]=new TGraphErrors(4,width,eff[3],0,effErr[3]);
    grEff[3]->SetName(("grEff_vs_Width_M2000_"+catLab).c_str());
    grEff[3]->SetMarkerSize(1.5);
    grEff[3]->SetMarkerColor(kGreen+3);
    grEff[3]->SetMarkerStyle(24);
    grEff[3]->GetXaxis()->SetTitle("Natural Width [GeV]");
    grEff[3]->GetYaxis()->SetTitle("Efficiency");

    grEff[0]->SetMaximum(0.25);
    grEff[1]->SetMaximum(0.25);
    grEff[2]->SetMaximum(0.25);
    grEff[3]->SetMaximum(0.25);
    grEff[0]->SetMinimum(0.0);
    grEff[1]->SetMinimum(0.0);
    grEff[2]->SetMinimum(0.0);
    grEff[3]->SetMinimum(0.0);

    TCanvas *cc=new TCanvas("canEffWidth",("CANVAS - EFF vs WIDTH - "+catLab).c_str(),900,900);
    grEff[3]->Draw("AP");
    grEff[0]->Draw("P");
    grEff[1]->Draw("P");
    grEff[2]->Draw("P");

    TLegend *l1;
    if(ic>1)l1=new TLegend(0.13,0.7,0.43,0.93);
    else l1=new TLegend(0.13,0.25,0.43,0.45);
    l1->SetFillColor(kWhite);
    l1->SetHeader(catLab.c_str());
    l1->AddEntry(grEff[0],"M=700 GeV","P");
    l1->AddEntry(grEff[1],"M=1000 GeV","P");
    l1->AddEntry(grEff[2],"M=1500 GeV","P");
    l1->AddEntry(grEff[3],"M=2000 GeV","P");
    l1->Draw();

    cc->SaveAs(("can_eff_vs_width_"+catLab+".root").c_str());
    cc->SaveAs(("can_eff_vs_width_"+catLab+".png").c_str());
    delete cc;
    delete l1;
    //  delete[] grEff;
  }//end lopp on categories

  */

  
  for(int is =0; is<nsig; is++)
    {
      double MAss = 750+250*is;
      if(is<6)outFile<<MAss;
      else if(is==6)outFile<<"1000 (MG, c=0.05)";
      else if(is==7)outFile<<"1000 (MG, c=0.20)";
      else if(is==8)outFile<<"1500 (MG, c=0.20)";
      else outFile<<"?????";

      for(int ic=0;ic<nCat; ic++)
	{
	  outFile<<"		"<<SigEff[is][ic];
	}
      outFile<<endl;
    }
  


}//end main
