vector<int> masspoint;
vector<double> ourcut;
vector<double> punzivalue;
vector<double> punzi45;
vector<double> punziRef;
vector<double> sigeff;
vector<double> sigeff45;
vector<double> sigeffRef;

double masswindow = 0.15;
double refPunzi=0.50;

TH1D* getNSubjHisto(TFile* targetFile, double mass){
	char buffer[256];
	sprintf(buffer,"nsubj21_m%i",(int)mass);
	TH2D* histogram = targetFile->Get("histogram");
	int nMassBins = histogram->GetNbinsY();
	int nSubjBins = histogram->GetNbinsX();
	double widthMassBin = histogram->GetYaxis()->GetBinWidth(1);
	int mainBin = (int)mass/widthMassBin;
	double binsHalfRange = mass*masswindow/widthMassBin/2;
	//double binsHalfRange = 200/widthMassBin/2;//for check with before
	TH1D* projection = histogram->ProjectionX(buffer,mainBin-binsHalfRange-1,mainBin+binsHalfRange);
	//cout<<histogram->GetYaxis()->GetBinLowEdge(mainBin)<<endl;
	projection->SetDirectory(0);
	return projection;
}

/// For this macro to work, you should have the file with the
/// signal and background histograms for nsubjetiness open.
/// Put the names of the histograms in the first two lines
/// of the macro.
TCanvas* compareHistos(TH1* signalHisto, TH1* backgroundHisto){
	TCanvas* cv = new TCanvas("cv","cv",1200,800);
	cv->Divide(3,2);

	int rebinFactor = 2;
	signalHisto->Rebin(rebinFactor);
	backgroundHisto->Rebin(rebinFactor);

	signalHisto->SetLineColor(kRed);
	signalHisto->SetMarkerColor(kRed);
	signalHisto->SetFillColor(kRed);
	signalHisto->SetFillStyle(3001);

	double bkgIntegral = backgroundHisto->Integral();
	signalHisto->SetTitle("");
	double sgnIntegral = signalHisto->Integral();

	double nBins = backgroundHisto->GetNbinsX();

	TGraph* ROC     = new TGraph(nBins);
	TGraph* S       = new TGraph(nBins);
	TGraph* B       = new TGraph(nBins);
	TGraph* SoverB  = new TGraph(nBins);
	TGraph* SoverSB = new TGraph(nBins); 
	TGraph* punzi   = new TGraph(nBins);

	ROC->SetTitle("");
	S->SetTitle("");
	B->SetTitle("");
	SoverB->SetTitle("");
	SoverSB->SetTitle("");
	punzi->SetTitle("");

	S->SetLineColor(kRed);
	S->SetMarkerColor(kRed);


	double bestcut=0;
	double maxPunzi=0;
	double bestsigeff=0;

	for(size_t i=1; i!=(nBins+1); ++i) {
		double thisB = backgroundHisto->Integral(1,i);
		double thisS = signalHisto->Integral(1,i);
		double thisSOverB = (thisB != 0) ? (thisS/thisB) : 0;
		double thisSOverSB = (thisB != 0) ? (thisS/sqrt(thisS+thisB)) : 0;
		double effSgn = thisS/sgnIntegral;
		double effBkg = thisB/bkgIntegral;
		double thisPunzi = effSgn/(1.0+sqrt(thisB));
		double binLowEdge = backgroundHisto->GetBinLowEdge(i);
		double binHighEdge = binLowEdge+backgroundHisto->GetBinWidth(i);
		cout<<"cut: "<<binHighEdge<<" thisPunzi: "<<thisPunzi<<endl;
		if(binHighEdge==refPunzi){punziRef.push_back(thisPunzi);sigeffRef.push_back(effSgn);}
		if(binHighEdge==0.45){punzi45.push_back(thisPunzi);sigeff45.push_back(effSgn);}
		if(thisPunzi>maxPunzi)
		{
			maxPunzi=thisPunzi;
			bestcut=binHighEdge;
			bestsigeff=effSgn;
		}
		//printf("binHighEdge = %g, eff Bkg = %g, eff Sgn = %g\n",binHighEdge,effBkg,effSgn);
		ROC->SetPoint(i-1,effSgn,1.0-effBkg);
		S->SetPoint(i-1,binHighEdge,effSgn);
		B->SetPoint(i-1,binHighEdge,effBkg);
		SoverB->SetPoint(i-1,binHighEdge,thisSOverB);
		SoverSB->SetPoint(i-1,binHighEdge,thisSOverSB);
		punzi->SetPoint(i-1,binHighEdge,thisPunzi);
	}
	
	cout<<"bestcut: "<<bestcut<<" maxPunzi: "<<maxPunzi<<endl;
	ourcut.push_back(bestcut);
	punzivalue.push_back(maxPunzi);
	sigeff.push_back(bestsigeff);

	cv->cd(1);
	gPad->SetGridx();
	gPad->SetGridy();
	signalHisto->Scale(1.0/sgnIntegral);
	backgroundHisto->Scale(1.0/bkgIntegral);
	signalHisto->Draw("HISTO");
	signalHisto->GetYaxis()->SetRangeUser(0,0.25);
	signalHisto->GetYaxis()->SetTitle("Normalized Distributions");
	backgroundHisto->Draw("HISTO SAMES");
	//cv->SaveAs("tau21.png");

	cv->cd(2);
	gPad->SetGridx();
	gPad->SetGridy();
	ROC->GetXaxis()->SetTitle("SIG efficiency");
	ROC->GetYaxis()->SetTitle("1.0 - BKG  efficiency");
	ROC->Draw("ALP");
	//cv->SaveAs("ROCcurve.png");

	cv->cd(3);
	gPad->SetGridx();
	gPad->SetGridy();
	S->Draw("ALP");
	B->Draw("LP");
	S->GetXaxis()->SetTitle("#tau_{21} upper threshold");
	S->GetYaxis()->SetTitle("Efficiency");
	//cv->SaveAs("efficiency.png");

	cv->cd(4);
	gPad->SetGridx();
	gPad->SetGridy();
	SoverB->Draw("ALP");
	SoverB->GetXaxis()->SetTitle("#tau_{21} upper threshold");
	SoverB->GetYaxis()->SetTitle("S / B");
	//cv->SaveAs("SoverB.png");

	cv->cd(5);
	gPad->SetGridx();
	gPad->SetGridy();
	SoverSB->Draw("ALP");
	SoverSB->GetXaxis()->SetTitle("#tau_{21} upper threshold");
	SoverSB->GetYaxis()->SetTitle("S / sqrt(S+B)");

	cv->cd(6);
	gPad->SetGridx();
	gPad->SetGridy();
	punzi->Draw("ALP");
	punzi->GetXaxis()->SetTitle("#tau_{21} upper threshold");
	punzi->GetYaxis()->SetTitle("#epsilon_{S} / (1.0+sqrt(B))");
	//cv->SaveAs("punzi.png");

	return cv;
}

void makeROCcurves(int massValue) {
	char buffer[256];
	masspoint.push_back(massValue);
	sprintf(buffer,"optimization_tau21/signal_%i.root",massValue);
	TFile* sgnFile = TFile::Open(buffer); 
	TFile* bkgFile = TFile::Open("optimization_tau21/allBackgrounds.root"); 

	TH1* signalHisto = getNSubjHisto(sgnFile, massValue);
	TH1* backgroundHisto = getNSubjHisto(bkgFile, massValue);
	TCanvas* thisCanvas = compareHistos(signalHisto,backgroundHisto);
	bkgFile->Close();
	sgnFile->Close();
	sprintf(buffer,"optimization_%i.png",massValue);
	thisCanvas->SaveAs(buffer);
}

void ROCcurves(){
	makeROCcurves(600);
	makeROCcurves(700);
	makeROCcurves(800);
	makeROCcurves(900);
	makeROCcurves(1000);
	makeROCcurves(1100);
	makeROCcurves(1200);
	makeROCcurves(1300);
	makeROCcurves(1400);
	makeROCcurves(1500);
	makeROCcurves(1600);
	makeROCcurves(1700);
	makeROCcurves(1800);
	makeROCcurves(1900);
	makeROCcurves(2000);
	makeROCcurves(2100);
	makeROCcurves(2200);
	makeROCcurves(2300);
	makeROCcurves(2400);
	makeROCcurves(2500);

	TGraph* cutVSmass     = new TGraph(masspoint.size());
	TGraph* punziRefVSmass     = new TGraph(masspoint.size());
	TGraph* punzi45VSmass     = new TGraph(masspoint.size());
	TGraph* bestpunziVSmass     = new TGraph(masspoint.size());
	TGraph* sigeffVSmass     = new TGraph(masspoint.size());
	TGraph* sigeffRefVSmass     = new TGraph(masspoint.size());
	TGraph* sigeff45VSmass     = new TGraph(masspoint.size());
	TCanvas* c1 = new TCanvas();

	for(int i =0; i< masspoint.size(); i++)
	{
		cout<<"mass "<<masspoint.at(i)<<" cut "<<ourcut.at(i)<<" punzi "<<punzivalue.at(i)<<" punziRef "<<punziRef.at(i)<<endl;
		cutVSmass->SetPoint(i,masspoint.at(i),ourcut.at(i));
		punziRefVSmass->SetPoint(i,masspoint.at(i),punziRef.at(i));
		punzi45VSmass->SetPoint(i,masspoint.at(i),punzi45.at(i));
		bestpunziVSmass->SetPoint(i,masspoint.at(i),punzivalue.at(i));
		sigeffVSmass->SetPoint(i,masspoint.at(i),sigeff.at(i));
		sigeffRefVSmass->SetPoint(i,masspoint.at(i),sigeffRef.at(i));
		sigeff45VSmass->SetPoint(i,masspoint.at(i),sigeff45.at(i));

	}
    c1->SetGridx();
    c1->SetGridy();
    c1->cd();
    cutVSmass->Draw("ALP");
    cutVSmass->SetTitle("");
    cutVSmass->GetXaxis()->SetTitle("signal mass");
    cutVSmass->GetYaxis()->SetTitle("best cut on tau21");
    cutVSmass->GetYaxis()->SetRangeUser(0,1);
    cutVSmass->SetMarkerStyle(20);
    cutVSmass->SetMarkerSize(1);
    c1->SaveAs("cutVSmass.png");
    
    bestpunziVSmass->SetTitle("");
    bestpunziVSmass->GetXaxis()->SetTitle("signal mass");
    bestpunziVSmass->GetYaxis()->SetTitle("#epsilon_{S} / (1.0+sqrt(B))");
    bestpunziVSmass->SetMarkerStyle(20);
    bestpunziVSmass->SetMarkerSize(1.25);
    bestpunziVSmass->SetMarkerColor(kRed);
    bestpunziVSmass->Draw("ALP");
    punziRefVSmass->SetMarkerStyle(29);
    punziRefVSmass->SetMarkerSize(1.6);
    punziRefVSmass->Draw("LPsame");
    punzi45VSmass->SetMarkerStyle(28);
    punzi45VSmass->SetMarkerSize(1.25);
    punzi45VSmass->SetMarkerColor(9);
    //punzi45VSmass->Draw("LPsame");
    char legEntry2[64];
    sprintf(legEntry2,"%d Punzi",refPunzi);    
    TLegend * leg = new TLegend (0.15, 0.5, 0.45, 0.85, NULL, "brNDC") ;
    leg->SetFillColor(kWhite);
    leg->AddEntry (bestpunziVSmass, "Punzi Signif (Best Cut)" ,"p") ;
    leg->AddEntry (punziRefVSmass, "Punzi Signif (tau21<0.50)" ,"p") ;
    //leg->AddEntry (punzi45VSmass, "Punzi Signif (tau21<0.45)" ,"p") ;
    leg->Draw();
    c1->SaveAs("punziVSmass.png");


    sigeffVSmass->SetTitle("");
    sigeffVSmass->GetXaxis()->SetTitle("signal mass");
    sigeffVSmass->GetYaxis()->SetTitle("#epsilon_{S}");
    sigeffVSmass->GetYaxis()->SetRangeUser(0.6,1);
    sigeffVSmass->SetMarkerStyle(20);
    sigeffVSmass->SetMarkerSize(1);
    sigeffVSmass->SetMarkerColor(kRed);
    sigeffVSmass->Draw("ALP");
    sigeff45VSmass->SetMarkerStyle(20);
    sigeff45VSmass->SetMarkerSize(1);
    //sigeff45VSmass->Draw("LPsame");
    sprintf(legEntry2,"Cut %.3f sigeff",refPunzi);
    TLegend * leg2 = new TLegend (0.15, 0.7, 0.45, 0.9, NULL, "brNDC") ;
    leg2->SetFillColor(kWhite);
    leg2->AddEntry (sigeffVSmass, "Best Cut sigeff" ,"p") ;
    //leg2->AddEntry (sigeff45VSmass, legEntry2 ,"p") ;
    leg2->Draw();
    c1->SaveAs("sigeffVSmass.png");

}


