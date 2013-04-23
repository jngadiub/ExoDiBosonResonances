void ttbar_scale()
{
	TString SigDir = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/AnaSigTree_from50_noConv_BcontrolCT";	
	double lumi = 19538.85;

	TChain * chainMC = new TChain("SelectedCandidatesPlain");
	TChain * chainData = new TChain("SelectedCandidatesPlain");

	chainMC->Add(SigDir+"/treeEDBR_DYJets_xww.root");
	chainMC->Add(SigDir+"/treeEDBR_WJetsPt100_xww.root");
	chainMC->Add(SigDir+"/treeEDBR_VV_xww.root");
	chainMC->Add(SigDir+"/treeEDBR_TTBARpowheg_xww.root");
	chainMC->Add(SigDir+"/treeEDBR_SingleTop_xww.root");

	chainData->Add(SigDir+"/treeEDBR_data_xww.root");

	TString region = "(region==1)";
	TString lep = "(lep==1)";	
	TString nXjets = "(nXjets==1)";
	TString vTagPurity = "(vTagPurity==1)";

	TString Cut = region+"*"+lep+"*"+nXjets+"*"+vTagPurity;
	TString totalWeight = Form("weight*%f",lumi);
	

	TH1D * mJJSBMC = new TH1D ("mJJSBMC","mJJSBMC",20,50,130);
	TH1D * mJJSBData = new TH1D ("mJJSBData","mJJSBData",20,50,130);
	mJJSBMC->Sumw2();
	mJJSBData->Sumw2();
	chainMC->Draw("prunedmass>>mJJSBMC",Cut+"*"+totalWeight);
	chainData->Draw("prunedmass>>mJJSBData",Cut);
	
	TCanvas * c0 = new TCanvas();
	mJJSBMC->SetLineColor(kGreen);
	mJJSBMC->Draw();
	mJJSBData->Draw("same");

	double totalMC =0;
	double errorMC =0;

	double totalData=0;
	double errorData=0;

	totalMC=mJJSBMC->IntegralAndError(1,20,errorMC);
	totalData=mJJSBData->IntegralAndError(1,20,errorData);


	double scale  = totalData/totalMC ;
	double error = totalData/totalMC * sqrt (  pow(errorMC/totalMC,2) + pow(errorData/totalData,2)   );	

	cout<<Cut<<endl;
	cout<<"scale factor is : "<<scale<<" +- "<<error<<endl;


}
