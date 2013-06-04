void ttbar_scale()
{
	TString SigDir = "/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv8/AnaSigTree_forTT/";	

    //for which category?
    TString lepType = "mu";
    TString purity  = "HP";

	double lumi =0. ;
	if(lepType == "mu")lumi= 19538.85;//mu
	if(lepType == "ele")lumi = 19531.85;//ele

	TChain * chainMC = new TChain("SelectedCandidatesPlain");
	TChain * chainData = new TChain("SelectedCandidatesPlain");

	//TChain * chainMC = new TChain("SelectedCandidatesABPlain");
	//TChain * chainData = new TChain("SelectedCandidatesABPlain");

	chainMC->Add(SigDir+"/treeEDBR_DYJets_xww.root");
	chainMC->Add(SigDir+"/treeEDBR_WJetsPt100_xww.root");
	chainMC->Add(SigDir+"/treeEDBR_VV_xww.root");
	chainMC->Add(SigDir+"/treeEDBR_TTBARpowheg_xww.root");
	chainMC->Add(SigDir+"/treeEDBR_SingleTop_xww.root");

	chainData->Add(SigDir+"/treeEDBR_data_xww.root");

/*
	for(int i =0;i<chainMC->GetEntries();i++)
	{
		chainMC->GetEntry(i);
		//TString filename = chainMC->GetFile()->GetEndpointUrl()->GetFile();
		TString filename = chainMC->GetFile()->GetEndpointUrl()->GetUrl();
		cout<<filename<<endl;
	}
*/
	//for which category?
	//
	TString lep="1";
	if(lepType == "mu")lep = "(lep==1)";//0 for ele, 1 for mu
	if(lepType == "ele")lep = "(lep==0)";//0 for ele, 1 for mu
	TString vTagPurity="1";
	if(purity == "HP")vTagPurity = "(vTagPurity==1)";//1 for HP, 0 for LP
	if(purity == "LP")vTagPurity = "(vTagPurity==0)";//1 for HP, 0 for LP
	//
	TString region = "(region==1)";//1 for sig, 0 for sideband
	TString nXjets = "(nXjets==1)";

	TString Cut = region+"*"+lep+"*"+nXjets+"*"+vTagPurity;
	TString totalWeight = Form("weight*%f",lumi);	

	TH1D * mJJSBMC = new TH1D ("mJJSBMC","mJJSBMC",28,0,140);
	TH1D * mJJSBData = new TH1D ("mJJSBData","mJJSBData",28,0,140);
	mJJSBMC->Sumw2();
	mJJSBData->Sumw2();
	chainMC->Draw("prunedmass>>mJJSBMC",Cut+"*"+totalWeight);
	chainData->Draw("prunedmass>>mJJSBData",Cut);
	
	TCanvas * c0 = new TCanvas();
	mJJSBMC->SetLineColor(kGreen);
	mJJSBMC->SetStats(0);
	mJJSBData->SetStats(0);
	mJJSBMC->SetTitle("ttbar_scale_"+lepType+"_"+purity);
	mJJSBData->SetTitle("ttbar_scale_"+lepType+"_"+purity);
	mJJSBMC->Draw();
	mJJSBData->Draw("same");
	TLegend * leg = new TLegend (0.7, 0.8, 0.9, 0.9, NULL, "brNDC") ;
	leg->AddEntry(mJJSBMC,"MC","l");
	leg->AddEntry(mJJSBData,"Data","l");
	leg->Draw();


	double totalMC =0;
	double errorMC =0;

	double totalData=0;
	double errorData=0;

	totalMC=mJJSBMC->IntegralAndError(1,25,errorMC);
	totalData=mJJSBData->IntegralAndError(1,25,errorData);


	double scale  = totalData/totalMC ;
	double error = totalData/totalMC * sqrt (  pow(errorMC/totalMC,2) + pow(errorData/totalData,2)   );	

	cout<<Cut<<endl;
	cout<<"scale factor is : "<<scale<<" +- "<<error<<endl;


	TPaveText * tp = new TPaveText(0.1,0.76,0.5,0.9,"brNDC");
	TString text = Form("scale factor is : %f +- %f",scale,error);
	tp->AddText(text);
	tp->SetBorderSize(0) ;
	tp->Draw();
	c0->SaveAs("ttbar_scale_"+lepType+"_"+purity+".png");

}
