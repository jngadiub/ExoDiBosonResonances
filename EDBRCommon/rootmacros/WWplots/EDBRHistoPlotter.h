#include <string>
#include <vector>
#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "CMSLabels.h"
#include "TVectorD.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegendEntry.h"
#include "TGaxis.h"

class EDBRHistoPlotter {
	public:

		EDBRHistoPlotter(std::string nameInDir,
				std::vector<std::string> nameFileDATA,
				std::vector<std::string> nameFileMC, 
				std::vector<std::string> nameFileMCSig, 
				double targetLumi,
				int wantNXJets,
				int wantPurity,
				bool isTTcontrol,
				int flavour,
				bool isZZchannel,
				bool scaleToData,
				bool scaleOnlyWJets,
				bool makeRatio,
				bool isSignalStackOnBkg,
				std::vector<double> kFactorsMC,
				std::vector<double> kFactorsMCSig)
		{
			nameInDir_     = nameInDir;
			fileNamesMC    = nameFileMC;
			fileNamesMCSig = nameFileMCSig;
			fileNamesDATA  = nameFileDATA;
			kFactorsMC_    = kFactorsMC;
			kFactorsSig_    = kFactorsMCSig;
			targetLumi_    = targetLumi;
			wantNXJets_    = wantNXJets;
			wantPurity_    = wantPurity;
			isTTcontrol_   = isTTcontrol;
			flavour_       = flavour;
			isZZchannel_   = isZZchannel;
			scaleToData_   = scaleToData;
			scaleOnlyWJets_ = scaleOnlyWJets;
			makeRatio_     = makeRatio;
			isSignalStackOnBkg_ = isSignalStackOnBkg;
			debug_         = true;
			if(fileNamesDATA.size() != 0)
				isDataPresent_ = true;
			else
				isDataPresent_ = false;
			EDBRColors.resize(20,kWhite);
			EDBRLineColors.resize(20,kWhite);
			labels.resize(0);
			labelsSig.resize(0);
			makeLabels();
			if(fileNamesMCSig.size()!=kFactorsSig_.size()){
				cout<<"======> Mismatch in size of input MC Sig arrays !!! "<<fileNamesMCSig.size()<<"  "<<kFactorsSig_.size()<<endl;
			}

			printf("Target lumi is %g pb-1\n",targetLumi);
			std::cout << "k factors for MC backgrounds are: " << std::endl;    
			int myKindex=0;
			for (std::vector<double>::iterator it = kFactorsMC_.begin(); it != kFactorsMC_.end(); ++it)
			{
				std::cout << *it << " for " << fileNamesMC.at(myKindex) << std::endl ; 
				myKindex++;
			}
			myKindex=0;
			for (std::vector<double>::iterator it = kFactorsSig_.begin(); it != kFactorsSig_.end(); ++it)
			{
				std::cout << *it << " for " << fileNamesMCSig.at(myKindex) << std::endl ; 
				myKindex++;
			}

			std::cout << std::endl;





		}//end constructor

		virtual ~EDBRHistoPlotter(){
		}//end destructor

		/// Members
		std::vector<std::string> fileNamesMC;
		std::vector<std::string> fileNamesMCSig;
		std::vector<std::string> fileNamesDATA;
		std::vector<std::string> labels;
		std::vector<std::string> labelsSig;
		std::vector<TFile*>      filesMC;
		std::vector<TFile*>      filesMCSig;
		std::vector<TFile*>      filesDATA;
		std::vector<TH1D*>       histosMC;
		std::vector<TH1D*>       histosMCSig;
		std::vector<TH1D*>       histosMCSigOrig;
		std::vector<TH1D*>       histosDATA;
		std::vector<int>         EDBRColors;
		std::vector<int>         EDBRLineColors;
		std::vector<double>      kFactorsMC_;
		std::vector<double>      kFactorsSig_;

		std::string nameInDir_;
		std::string nameOutDir_;
		double dataIntegral_;
		double targetLumi_;
		int    wantNXJets_;
		int    wantPurity_;
		bool   isTTcontrol_;
		int    flavour_;
		bool   isZZchannel_;
		bool   scaleToData_;
		bool   scaleOnlyWJets_;
		bool   makeRatio_;
		bool   isSignalStackOnBkg_;
		bool   isDataPresent_;
		bool   debug_;

		/// Functions
		void cleanupMC();
		void cleanupMCSig();
		void cleanupDATA();
		void makeLabels();
		void makeStackPlots(std::string histoName);
		void setOutDir(std::string outDirNew);

		/// set debug mode
		void setDebug(bool debug){debug_=debug;}

		/// get reasonable colors for stacks.
		int getFillColor(int index){
			if(index < 20)return EDBRColors[index];
			return kWhite;
		}

		/// set reasonable colors for stacks.
		void setFillColor(std::vector<int> colorList){
			unsigned int ind=0;
			while (ind < 20 && ind < colorList.size()){
				EDBRColors.at(ind)=colorList.at(ind);
				ind++;
			}
		}

		/// get reasonable colors for stacks.
		int getLineColor(int index){
			if(index < 20)return EDBRLineColors[index];
			return kWhite;
		}

		/// set reasonable colors for stacks.
		void setLineColor(std::vector<int> colorList){
			unsigned int ind=0;
			while (ind < 20 && ind < colorList.size()){
				EDBRLineColors.at(ind)=colorList.at(ind);
				ind++;
			}
		}

};

void EDBRHistoPlotter::cleanupMC() {
	for(size_t i=0; i!= filesMC.size(); ++i) {
		filesMC.at(i)->Close();
	}
	filesMC.clear();  

	for(size_t i=0; i!= histosMC.size(); ++i) {
		histosMC.at(i)->Delete();
	}
	histosMC.clear();
}

void EDBRHistoPlotter::cleanupMCSig() {
	for(size_t i=0; i!= filesMCSig.size(); ++i) {
		filesMCSig.at(i)->Close();
	}
	filesMCSig.clear();  

	for(size_t i=0; i!= histosMCSig.size(); ++i) {
		histosMCSig.at(i)->Delete();
		histosMCSigOrig.at(i)->Delete();
	}
	histosMCSig.clear();
	histosMCSigOrig.clear();
}

void EDBRHistoPlotter::cleanupDATA() {
	for(size_t i=0; i!= filesDATA.size(); ++i) {
		filesDATA.at(i)->Close();
	}
	filesDATA.clear();  

	for(size_t i=0; i!= histosDATA.size(); ++i) {
		histosDATA.at(i)->Delete();
	}
	histosDATA.clear();
}

void EDBRHistoPlotter::makeLabels() {
	for(size_t i = 0; i != fileNamesMC.size(); i++){
		TString s1 = fileNamesMC.at(i);
		TString s2 = "_.";
		TObjArray* tokens = s1.Tokenize(s2);
		std::string aLabel = ((TObjString*)(tokens->At(1)))->String().Data();
		labels.push_back(aLabel);
	}
	for(size_t i = 0; i != fileNamesMCSig.size(); i++){
		TString s1 = fileNamesMCSig.at(i);
		TString s2 = "_.";
		TObjArray* tokens = s1.Tokenize(s2);
		std::string aLabelType = ((TObjString*)(tokens->At(1)))->String().Data();
		std::string aLabelCoupling = ((TObjString*)(tokens->At(4)))->String().Data();
		std::string aLabelMass = ((TObjString*)(tokens->At(5)))->String().Data();
		std::string aLabel = aLabelType + aLabelCoupling + aLabelMass; 
		labelsSig.push_back(aLabel);
	}
}

///set output directories for plots.
void EDBRHistoPlotter::setOutDir(std::string outDirNew){
	char buffer[256];
	nameOutDir_=outDirNew;

	sprintf(buffer,"%s/pdf",nameOutDir_.c_str());
	printf("%s\n",buffer);
	gSystem->mkdir(buffer,true);

	sprintf(buffer,"%s/root",nameOutDir_.c_str());
	printf("%s\n",buffer);
	gSystem->mkdir(buffer,true);

	sprintf(buffer,"%s/png",nameOutDir_.c_str());
	printf("%s\n",buffer);
	gSystem->mkdir(buffer,true);
}

void EDBRHistoPlotter::makeStackPlots(std::string histoName) {

	cleanupMC();
	cleanupMCSig();
	cleanupDATA();

	//	TGaxis::SetMaxDigits(3);

	//printf("Making histo %s\n",histoName.c_str());
	std::cout<<"\rMaking histo "<<histoName.c_str() << std::endl;

	TCanvas* cv = new TCanvas(("cv_"+histoName).c_str(),("cv_"+histoName).c_str(),600,600);

	//create 3 pads in the canvas
	TPad* fPads1 = NULL;
	TPad* fPads2 = NULL;
	TPad* fPads3 = NULL;

	if(makeRatio_ && isDataPresent_)
	{
		fPads1 = new TPad("pad1", "", 0.00, 0.24, 0.99, 1);
		fPads2 = new TPad("pad2", "", 0.00, 0.13, 1, 0.26);//not used
		fPads3 = new TPad("pad3", "", 0.00, 0.00, 0.99, 0.24);
		fPads1->SetFillColor(0);
		fPads1->SetLineColor(0);
		fPads2->SetFillColor(0);
		fPads2->SetLineColor(0);
		fPads3->SetFillColor(0);
		fPads3->SetLineColor(0);
		fPads1->Draw();
		//fPads2->Draw();
		fPads3->Draw();
	}

	//============ Data vs MC plots ==============

	if(makeRatio_ && isDataPresent_)
	{      
		fPads1->cd();
	}

	///--------------------
	/// Make the DATA stack
	///--------------------

	TH1D* sumDATA = NULL;

	for(size_t i=0; i!= fileNamesDATA.size(); ++i) {
		filesDATA.push_back(TFile::Open((nameInDir_+
						fileNamesDATA.at(i)).c_str()));
	}

	for(size_t i=0; i!= filesDATA.size(); ++i) {
		TH1D* histo = (TH1D*)(filesDATA.at(i)->Get(histoName.c_str())->Clone("clone"));
		histo->SetDirectory(0);
		histosDATA.push_back(histo);
	}

	if(histosDATA.size() !=0) {
		sumDATA = (TH1D*)(histosDATA.at(0)->Clone("masterDATA"));
		sumDATA->Reset();
		sumDATA->SetDirectory(0);
	}

	for(size_t i=0; i!= histosDATA.size(); ++i) {
		sumDATA->Add(histosDATA.at(i));
	}

	double sumDataIntegral = 0;
	if(isDataPresent_)
		sumDataIntegral = sumDATA->Integral();

	if(debug_) {
		printf("sumDataIntegral = %g\n",sumDataIntegral);
	}

	///------------------
	/// Make the MC stack
	///------------------

	TH1D* sumMC = NULL;
	double sumBkgAtTargetLumi = 0;

	for(size_t i=0; i!= fileNamesMC.size(); ++i) {
		filesMC.push_back(TFile::Open((nameInDir_+
						fileNamesMC.at(i)).c_str()));
	}

	double sumBkgOther=0.;
	double sumWJets=0.;
	for(size_t i=0; i!= filesMC.size(); ++i) {
		TH1D* histo = (TH1D*)(filesMC.at(i)->Get(histoName.c_str())->Clone(labels.at(i).c_str()));

		histo->Scale(kFactorsMC_.at(i));

		if(debug_) {
			histo->Print();
		}
		sumBkgAtTargetLumi += (histo->Integral() * targetLumi_);


		if(debug_)cout<<"filesMC.at(i)->GetName()   "<<filesMC.at(i)->GetName()<<endl;
		TString filename = filesMC.at(i)->GetName();
		if(filename.Contains("WZJets")) sumWJets+=(histo->Integral() * targetLumi_);
		else sumBkgOther+=(histo->Integral() * targetLumi_);
	}
	if(debug_)cout<<sumBkgAtTargetLumi<<" "<<sumWJets<<" "<<sumBkgOther<<" "<<sumWJets+sumBkgOther<<endl;
	double WJetsScaleFactor = 1.;
	if(scaleOnlyWJets_){
		WJetsScaleFactor = (sumDataIntegral - sumBkgOther)/sumWJets;
		cout<<"WJetsScaleFactor "<<WJetsScaleFactor<<endl;
	}

	for(size_t i=0; i!= filesMC.size(); ++i) {
		TH1D* histo = (TH1D*)(filesMC.at(i)->Get(histoName.c_str())->Clone(labels.at(i).c_str()));
		histo->SetDirectory(0);
		histo->SetFillColor(getFillColor(i));

		TString filename = filesMC.at(i)->GetName();
		histo->Scale(kFactorsMC_.at(i));
		if(filename.Contains("WZJets"))histo->Scale(WJetsScaleFactor);
		histosMC.push_back(histo);
	}



	if(debug_) {
		printf("sumBkgAtTargetLumi = %g\n",sumBkgAtTargetLumi);
		printf("sumDataIntegral = %g\n",sumDataIntegral);
		printf("Scale factor = %g\n",sumDataIntegral/sumBkgAtTargetLumi);
	}

	if(histosMC.size() !=0) {
		sumMC = (TH1D*)(histosMC.at(0)->Clone("masterMC"));
		sumMC->Reset();
		sumMC->SetDirectory(0);
	}

	/// Do we normalize to data or to lumi?
	/// NOTICE THAT THIS DEPENDS ON THE HISTOGRAMS HAVING BEING
	/// CORRECTLY FILLED WITH PUweight*LumiWeight*GenWeight
	for(size_t is=0; is!=histosMC.size(); is++){
		if(debug_)
			printf("This histogram has integral %g\n",histosMC.at(is)->Integral());
		if(scaleToData_ && isDataPresent_) {
			histosMC.at(is)->Scale(targetLumi_*
					sumDataIntegral/sumBkgAtTargetLumi);
		}
		else { 
			histosMC.at(is)->Scale(targetLumi_);
		}
		if(debug_)
			printf("After scaling this histogram has integral %g\n",histosMC.at(is)->Integral());
	}

	THStack* hs = new THStack("hs","");

	// Make a histogram just for the sum
	for(size_t i=0; i!= histosMC.size(); ++i) {
		sumMC->Add(histosMC.at(i));
		hs->Add(histosMC.at(i));
	}

	if(debug_) {
		printf("After scaling, sum of backgrounds = %g\n",sumMC->Integral());
		printf("Sum of data is still %g\n",sumDATA->Integral());
	}


	sumMC->SetFillStyle(0);
	sumMC->SetLineColor(kBlack);
	sumMC->SetLineWidth(2);

	if(scaleToData_ && isDataPresent_) {
		std::cout << "===> Residual DATA/MC Scale Factor is: " << sumDataIntegral/sumBkgAtTargetLumi << std::endl; 
	}

	///-------------------------------
	/// Add the MC signal to the stack
	///-------------------------------

	for(size_t i=0; i!= fileNamesMCSig.size(); ++i) {
		filesMCSig.push_back(TFile::Open((nameInDir_+
						fileNamesMCSig.at(i)).c_str()));
	}

	for(size_t i=0; i!= filesMCSig.size(); ++i) {
		TH1D* histo = (TH1D*)(filesMCSig.at(i)->Get(histoName.c_str())->Clone(labelsSig.at(i).c_str()));
		TH1D* histoOrig = (TH1D*)(filesMCSig.at(i)->Get(histoName.c_str())->Clone(labelsSig.at(i).c_str()));
		histo->SetDirectory(0);
		histo->SetLineColor(getLineColor(i)); 
		histoOrig->SetDirectory(0);
		histoOrig->SetLineColor(getLineColor(i)); 
		histoOrig->SetFillColor(getLineColor(i));
		if(i%2==0)histoOrig->SetFillStyle(3004);
		else histoOrig->SetFillStyle(3005);
		//histo->Scale(kFactor_); //============= SCALA FACTORS FOR SIGNAL? ==== FIXME

		if(debug_) {
			histo->Print();
		}
		histosMCSig.push_back(histo);
		histosMCSigOrig.push_back(histoOrig);
	}

	//scale the MC signal histogram
	if(histosMCSig.size()!=kFactorsSig_.size())cout<<"+++++++++++++++++ Mismatch in size of input MC Sig arrays !!!"<<endl;
	for(size_t is=0; is!=histosMCSig.size(); is++){
		if(debug_)printf("This histogram has integral %g\n",histosMCSig.at(is)->Integral());

		histosMCSig.at(is)->Scale(targetLumi_*kFactorsSig_.at(is));
		histosMCSigOrig.at(is)->Scale(targetLumi_*kFactorsSig_.at(is));

		if(debug_)
			printf("After scaling this histogram has integral %g\n",histosMCSig.at(is)->Integral());

		//add the signal to the total background
		histosMCSig.at(is)->Add(sumMC);
	}

	///-----------------------------------
	/// Draw both MC and DATA in the stack
	///-----------------------------------

	hs->Draw("HIST");
	hs->GetXaxis()->SetTitle(histoName.c_str());
	hs->GetYaxis()->SetTitle("Number of Entries");
	hs->GetYaxis()->SetTitleOffset(1.15);
	hs->GetYaxis()->CenterTitle();
	double maximumMC = 1.15*sumMC->GetMaximum();
	double maximumDATA = -100;
	if(isDataPresent_)
		maximumDATA = 1.15*sumDATA->GetMaximum();
	double maximumForStack = -100;
	if(isDataPresent_)
		maximumForStack = (maximumMC>maximumDATA?maximumMC:maximumDATA);
	else
		maximumForStack = maximumMC;
	hs->SetMaximum(maximumForStack);
	hs->SetMaximum(maximumForStack*1.4);
	// Some hacks for better aestetics
	// Extra vertical space in eta plots
	if(histoName.find("eta")!=std::string::npos) {
		//	hs->SetMaximum(maximumForStack*1.25);
	}
	// Extra vertical space in eta plots
	if(histoName.find("mJJNoKinFit")!=std::string::npos) {
		//	hs->SetMaximum(maximumForStack*1.25);
	}

	if(histoName.find("ptWj")!=std::string::npos) 
	{
		hs->GetXaxis()->SetTitle("p_{T}W_{had} (GeV)");
		hs->GetXaxis()->SetTitleFont(132);
		hs->GetYaxis()->SetTitle("Events / (10 GeV)");
	}
	if(histoName.find("nsubj21")!=std::string::npos) 
	{
		hs->GetXaxis()->SetTitle("#tau_{21}");//#tau2/#tau1
		hs->GetYaxis()->SetTitle("Events / 0.03");
		//hs->SetMaximum(maximumForStack*1.15);
	}
	if(histoName.find("prunedmass")!=std::string::npos)
	{
		hs->GetXaxis()->SetTitle("Pruned jet mass (GeV)");
		hs->GetYaxis()->SetTitle("Events / (5 GeV)");
	}


	hs->GetXaxis()->SetTitleFont(42);
	hs->GetXaxis()->SetLabelFont(42);
	hs->GetYaxis()->SetTitleFont(42);
	hs->GetYaxis()->SetLabelFont(42);

	hs->SetMinimum(0.1);
	hs->Draw("HIST");
	sumMC->Draw("HISTO SAME");
	if(isDataPresent_)
		sumDATA->Draw("SAME E1");
	for(size_t is=0; is!=histosMCSig.size(); is++){
		if( isSignalStackOnBkg_ == true )
			histosMCSig.at(is)->Draw("HISTO SAME");
		else
			histosMCSigOrig.at(is)->Draw("HISTO SAME");
	}
	sumMC->Draw("HISTO SAME");      

	if(histoName.find("mZZ")!=std::string::npos){
		double limInt0=600.0,limInt1=1400.0,limInt2=2400.0;
		int binInt0=sumMC->FindBin(limInt0);
		int binInt1=sumMC->FindBin(limInt1);
		int binInt2=sumMC->FindBin(limInt2);
		double errInt=0.0;
		double mcInt=sumMC->IntegralAndError(binInt0,binInt2,errInt);
		cout<<"Integral of total MC in range ["<<limInt0<<","<< limInt2<< "] = "<<mcInt<<" +/- "<<errInt<<endl;
		if(isDataPresent_){
			double dataInt=sumDATA->Integral(binInt0,binInt2);
			cout<<"Integral of total DATA in range ["<<limInt0<<","<< limInt2<< "] = "<<dataInt<<" +/- "<<sqrt(dataInt)<<endl;
		}
	}

	// For the legend, we have to tokenize the name "histos_XXX.root"
	/*
	   TLegend *leg = new TLegend(0.54,0.66,0.91,0.93,NULL,"brNDC");
	   TLegendEntry *entry=leg->AddEntry(histosMC.at(3),"W+jets","F");
	   entry=leg->AddEntry(histosMC.at(2),"WW/WZ/ZZ","F");
	   entry=leg->AddEntry(histosMC.at(0),"t#bar{t}","F");
	   entry=leg->AddEntry(histosMC.at(1),"Single Top","F");
	   entry=leg->AddEntry(histosMCSig.at(0),"Bulk G, M=1.5 TeV ( #times 100)","L");
	   entry=leg->AddEntry(sumDATA,"data","PE");
	 */
	/*
	   TLegend * leg= new TLegend(0.65, 0.57, 0.92, 0.87, "", "NDC");
	   leg->SetName("leg->nd"); leg->SetBorderSize(0); leg->SetLineColor(0); leg->SetFillColor(0);
	   leg->SetFillStyle(0); leg->SetLineWidth(0); leg->SetLineStyle(0); leg->SetTextFont(42);
	   leg->SetTextSize(.045);

	   leg->AddEntry(histosMC.at(3), "WJets","F");
	   leg->AddEntry(histosMC.at(0), "TTbar","F");
	   leg->AddEntry(histosMC.at(1), "Single-T","F");
	   leg->AddEntry(histosMC.at(2), "VV","F");
	   leg->AddEntry(histosMCSig.at(0), "Bulk G, M=1.5 TeV ( #times 100)","L");
	   leg->AddEntry(sumDATA, "data","ep");
	   leg->SetY1NDC(0.9 - 0.05*6 - 0.005);
	   leg->SetY1(leg->GetY1NDC());
	   leg->Draw();
	 */
	/*
	   TFile * fg2 = new TFile("~/www/plots/EXODiBosonResonances/Plots_0722/pasplots/plot4Pas/plot4Pas/mlvj_sb_el_HP.root");	
	   TCanvas * cg2 = (TCanvas*) fg2->Get("cMassFit");
	   TLegend * leg   = (TLegend*) cg2->FindObject("TPave");
	   cv->cd();
	 */

	TString times ;
	if(isTTcontrol_==false)
	{
		if(kFactorsSig_.at(0)==60000)times="#times6#times10^{4}";
		if(kFactorsSig_.at(0)==10000)times="#times10^{4}";
		if(kFactorsSig_.at(0)==30000)times="#times3#times10^{4}";
		if(kFactorsSig_.at(0)==300000)times="#times3#times10^{5}";
	}

	//TString legendfile;
	//if(isTTcontrol_==false)legendfile="legend.root";
	//if(isTTcontrol_==true)legendfile="legend_tt.root";

	TFile * fl2 = new TFile("legend.root");
	TLegend * leg   = (TLegend*) fl2->Get("TPave");
	if(isTTcontrol_==false)leg->AddEntry("","","");
	if(isTTcontrol_==false)leg->AddEntry(histosMCSig.at(0),"Bulk Graviton M=1.5TeV #tilde{k}=0.2 ("+times+")","L");
	leg->Draw();


	// Nice labels
	/*
	   TLatex* l = makeCMSPreliminaryTop(8,0.10,0.935);
	   l->Draw();
	   l = makeCMSLumi(19.6,0.5,0.935);
	   l->Draw();
	   l = makeChannelLabel(wantNXJets_,flavour_,isZZchannel_);
	   l->Draw();
	 */
	TString LepTs;
	if(flavour_==11)LepTs = "e";
	else if (flavour_==13) LepTs = "#mu"; 

	TString sPur;
	if(wantPurity_==0) sPur = " LP";
	if(wantPurity_==1) sPur = " HP";
	if(wantPurity_==-1) sPur = "";//ALLP


	TLatex *   tex = new TLatex(0.22651,0.928322,"CMS Preliminary, 19.5 fb^{-1} at #sqrt{s}=8TeV, W#rightarrow " + LepTs +"#nu"+sPur);
	tex->SetNDC();
	if(makeRatio_==false)tex->SetTextSize(0.044);//for ratio, 0.045. for pas, 0.038
	if(makeRatio_==true)tex->SetTextSize(0.048);//for ratio, 0.045. for pas, 0.038
	tex->SetTextFont(42);
	if(makeRatio_==false)tex->SetX(0.096);
	if(makeRatio_==true)tex->SetX(0.194902);
	tex->SetY(0.9406);
	tex->SetLineWidth(2);
	tex->Draw();

	/*
	//============ Save the full background histogram ============

	//printf("%s\n",histoName.c_str());
	if(histoName == "h_nsubj21") {
	printf("Saving the full background histogram...\n");
	TFile* fullBkg = TFile::Open("forOptimization.root","RECREATE");
	sumMC->Write();
	histosMCSigOrig.at(0)->Write();
	fullBkg->Close();    
	}
	 */

	//============ Data/MC ratio ==============

	TLine* lineAtOne = NULL; 
	if(makeRatio_ && isDataPresent_)
	{
		fPads2->cd();

		fPads2->SetGridx();
		fPads2->SetGridy();

		TH1D* histoRatio = (TH1D*)sumDATA->Clone("ratio");  
		histoRatio->SetDirectory(0);
		histoRatio->SetTitle("");
		histoRatio->Divide(sumMC);
		histoRatio->GetYaxis()->SetRangeUser(0.5,1.5);
		histoRatio->GetYaxis()->SetTitle("#frac{Data}{Bkg}");
		histoRatio->GetYaxis()->SetTitleOffset(0.2);//
		histoRatio->GetYaxis()->SetTitleSize(0.3);///////////////////////////////////
		histoRatio->GetYaxis()->SetLabelSize(0.13);//0.07
		histoRatio->GetXaxis()->SetTitle("");
		histoRatio->GetXaxis()->SetTitleOffset(0.01);
		histoRatio->GetXaxis()->SetLabelSize(0.09);
		histoRatio->SetMarkerStyle(1);
		histoRatio->Draw("p");

		lineAtOne = new TLine (histoRatio->GetXaxis()->GetXmin(),1,histoRatio->GetXaxis()->GetXmax(),1);
		lineAtOne->SetLineColor(2);
		lineAtOne->Draw();
	}

	//============ Data-MC/Error ==============

	TLine* lineAtZero = NULL; 
	TLine* lineAtPlusTwo = NULL; 
	TLine* lineAtMinusTwo = NULL; 
	if(makeRatio_ && isDataPresent_)
	{      
		fPads3->cd();

		fPads3->SetGridx();
		fPads3->SetGridy();

		double thisYmin=-5;
		double thisYmax=5;

		TVectorD nsigma_x( sumDATA->GetNbinsX() ); 
		TVectorD nsigma_y( sumDATA->GetNbinsX() ); 
		TVectorD nsigma_ex( sumDATA->GetNbinsX() ); 
		TVectorD nsigma_ey( sumDATA->GetNbinsX() ); 

		for(int ibin = 0; ibin!= sumDATA->GetNbinsX(); ++ibin)
		{

			double Data = sumDATA->GetBinContent(ibin+1);
			double Bkg = sumMC->GetBinContent(ibin+1);
			double eData = sumDATA->GetBinError(ibin+1);
			double eBkg = sumMC->GetBinError(ibin+1);
			double x = sumDATA->GetBinCenter(ibin+1);

			double diff = Data - Bkg;
			double sigma = sqrt( (eData * eData) + (eBkg * eBkg) );

			if( sigma != 0.0 && Data != 0.0 )
			{
				nsigma_x[ibin] = x;
				nsigma_y[ibin] = diff / sigma;
			}	
			else
			{
				nsigma_x[ibin] = +999999;
				nsigma_y[ibin] = 0;
			}		
			nsigma_ex[ibin] = 0;
			nsigma_ey[ibin] = 1;
		}


		if( nsigma_x.GetNoElements() != 0 )
		{
			TGraphErrors *nsigmaGraph = new TGraphErrors(nsigma_x,nsigma_y,nsigma_ex,nsigma_ey);
			nsigmaGraph->SetTitle("");
			nsigmaGraph->GetYaxis()->SetRangeUser(thisYmin,thisYmax);
			nsigmaGraph->GetYaxis()->SetTitle("#frac{Data-Bkg}{#sigma}");
			nsigmaGraph->GetYaxis()->SetTitleOffset(0.35);
			nsigmaGraph->GetYaxis()->SetTitleSize(0.17);//////////////////////////////
			nsigmaGraph->GetYaxis()->SetNdivisions(205);
			nsigmaGraph->GetYaxis()->SetLabelSize(0.16);
			nsigmaGraph->GetXaxis()->SetTitle("");
			nsigmaGraph->GetXaxis()->SetLimits( sumMC->GetXaxis()->GetXmin() , sumMC->GetXaxis()->GetXmax() );
			nsigmaGraph->GetXaxis()->SetRangeUser( sumMC->GetXaxis()->GetXmin() , sumMC->GetXaxis()->GetXmax() );
			nsigmaGraph->GetXaxis()->SetTitleOffset(0.01);
			nsigmaGraph->GetXaxis()->SetLabelSize(0.16);
			nsigmaGraph->SetMarkerStyle(8);
			nsigmaGraph->SetMarkerSize(0.8);
			nsigmaGraph->Draw("ap");
		}

		fPads3->Update();

		lineAtZero = new TLine(sumMC->GetXaxis()->GetXmin(),0,sumMC->GetXaxis()->GetXmax(),0);
		lineAtZero->SetLineColor(2);
		lineAtZero->Draw();
		lineAtPlusTwo = new TLine(sumMC->GetXaxis()->GetXmin(),2,sumMC->GetXaxis()->GetXmax(),2);
		lineAtPlusTwo->SetLineColor(2);
		lineAtPlusTwo->SetLineStyle(2);
		lineAtPlusTwo->Draw();
		lineAtMinusTwo = new TLine(sumMC->GetXaxis()->GetXmin(),-2,sumMC->GetXaxis()->GetXmax(),-2);
		lineAtMinusTwo->SetLineColor(2);
		lineAtMinusTwo->SetLineStyle(2);
		lineAtMinusTwo->Draw();
	}

	// Save the picture
	char buffer[256];
	cv->SetLogy(false);
	sprintf(buffer,"%s/pdf/can_%s.pdf",nameOutDir_.c_str(),histoName.c_str());
	cv->SaveAs(buffer);
	sprintf(buffer,"%s/png/can_%s.png",nameOutDir_.c_str(),histoName.c_str());
	cv->SaveAs(buffer);
	sprintf(buffer,"%s/root/can_%s.root",nameOutDir_.c_str(),histoName.c_str());
	cv->SaveAs(buffer);
	if(makeRatio_ && isDataPresent_)
	{      
		fPads1->cd();
		fPads1->SetLogy(true);
	}
	else
	{
		cv->SetLogy(true);
	}
	//-- resize y axis --
	hs->SetMaximum(10*maximumForStack);
    if(histoName.find("mZZ")!=std::string::npos)
    {   
        hs->SetMaximum(maximumForStack*1000);
    }
	//
	sprintf(buffer,"%s/pdf/LOG_can_%s.pdf",nameOutDir_.c_str(),histoName.c_str());
	cv->SaveAs(buffer);
	sprintf(buffer,"%s/png/LOG_can_%s.png",nameOutDir_.c_str(),histoName.c_str());
	cv->SaveAs(buffer);
	sprintf(buffer,"%s/root/LOG_can_%s.root",nameOutDir_.c_str(),histoName.c_str());
	cv->SaveAs(buffer);

	if(debug_) {
		printf("***********************\n");
	}
}
