#define NMAX 100

void draw(TTree* , TChain*, TString);
const float normal = 1;

void MakePlots()
{
	bool testlvlv = 1;

	TChain * cen = new TChain("demo/ExTree");
	cen->Add("central.root");
	TChain * pri = new TChain("demo/ExTree");
	pri->Add("private.root");

	draw(cen, pri, "nPV1");
	draw(cen, pri, "nPV2");
	draw(cen, pri, "wpgenpt");
	draw(cen, pri, "wpgeneta");
	draw(cen, pri, "wpgenphi");
	draw(cen, pri, "wmgenpt");
	draw(cen, pri, "wmgeneta");
	draw(cen, pri, "wmgenphi");
	draw(cen, pri, "mww");
	draw(cen, pri, "ptww");
	draw(cen, pri, "phiww");
	draw(cen, pri, "gram");
	draw(cen, pri, "graphi");
	draw(cen, pri, "grapt");
	draw(cen, pri, "mupt[0]");
	draw(cen, pri, "mueta[0]");
	draw(cen, pri, "muphi[0]");
	draw(cen, pri, "jetpt[0]");
	draw(cen, pri, "jeteta[0]");
	draw(cen, pri, "jetphi[0]");
	draw(cen, pri, "elept[0]");
	draw(cen, pri, "eleeta[0]");
	draw(cen, pri, "elephi[0]");
	string FABS = "TMath::Abs";
	if(testlvlv)
	{
		draw(cen, pri, "lep2m");
		draw(cen, pri, Form("%s(lep_p_phi-lep_m_phi)>3.14?%s(lep_p_phi-lep_m_phi)-3.14:%s(lep_p_phi-lep_m_phi)",FABS,FABS,FABS));
		draw(cen, pri, "lep_m_pt");
		draw(cen, pri, "lep_m_eta");
		draw(cen, pri, "lep_p_pt");
		draw(cen, pri, "lep_p_eta");
	}

}

void draw(TTree* cen , TChain* pri, TString branch)
{

	const float entries_cen= (float)cen->GetEntries();
	cout<<entries_cen<<endl;
	const float entries_pri= (float)pri->GetEntries();
	cout<<entries_pri<<endl;
	TCanvas *c1 = new TCanvas();
	c1->SetFillColor(0);

	//set histogram binning
	if(branch.Contains("pt"))
	{
		TH1F * h_cen = new TH1F("h_cen","h_cen",50,0,1000);
		TH1F * h_pri = new TH1F("h_pri","h_pri",50,0,1000);
		h_cen->GetXaxis()->SetTitle("PT/GeV");
	}

	else if(branch.Contains("eta"))
	{   
		TH1F * h_cen = new TH1F("h_cen","h_cen",50,-5,5);
		TH1F * h_pri = new TH1F("h_pri","h_pri",50,-5,5);
		h_cen->GetXaxis()->SetTitle("Eta");
	}
	
	else if(branch.Contains("phi"))
	{
		TH1F * h_cen = new TH1F("h_cen","h_cen",30,-4,4);
		TH1F * h_pri = new TH1F("h_pri","h_pri",30,-4,4);
		h_cen->GetXaxis()->SetTitle("Eta");
	}


	else if(branch.Contains("mww")||branch.Contains("gram")||branch.Contains("lep2m"))
	{   
		TH1F * h_cen = new TH1F("h_cen","h_cen",100,0,1500);
		TH1F * h_pri = new TH1F("h_pri","h_pri",100,0,1500);
		h_cen->GetXaxis()->SetTitle("M/GeV");
	}

	else if(branch.Contains("nPV"))
	{
		TH1F * h_cen = new TH1F("h_cen","h_cen",50,0,50);
		TH1F * h_pri = new TH1F("h_pri","h_pri",50,0,50);
		h_cen->GetXaxis()->SetTitle("nPV");
	}

	else 
	{
		cout<<"branch not found : "<<branch<<endl;
	}

	h_cen->Sumw2();
	h_pri->Sumw2();

	cen->Draw(branch+">>h_cen",Form("%f",normal/entries_cen));
	pri->Draw(branch+">>h_pri",Form("%f",normal/entries_pri));

	h_cen->SetLineColor(kBlue);
	h_pri->SetLineColor(kRed);

	h_cen->SetStats(0);
	h_cen->SetTitle(branch);
	h_cen->Draw();
	h_pri->Draw("same");
	TLegend *my =new TLegend(0.7,0.9,1.0,1.0,NULL,"brNDC");
	my->SetTextSize(0.05);
	my->AddEntry(h_cen,"official","l");
	my->AddEntry(h_pri,"private","l");
	my->Draw();
	c1->Print(branch+".png","png");


	delete c1;
	delete h_cen;
	delete h_pri;


}
