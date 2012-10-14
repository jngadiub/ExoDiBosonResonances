void CosTheta(TString infilename = "Graviton_genlevel_1000.root")
{
	TChain * chain = new TChain("demo/ExTree");
	chain->Add(infilename);


	vector<const reco::Candidate *> *phig;
	vector<const reco::Candidate *> *pmu;
	vector<const reco::Candidate *> *pele;
	vector<const reco::Candidate *> *pjet;
	vector<const reco::Candidate *> *pnu;
	vector<const reco::Candidate *> *pwplus;
	vector<const reco::Candidate *> *pwminus;
	chain-> SetBranchAddress("phig", &phig);
	chain-> SetBranchAddress("pmu", &pmu);
	chain-> SetBranchAddress("pele", &pele);
	chain-> SetBranchAddress("pjet", &pjet);
	chain-> SetBranchAddress("pnu", &pnu);
	chain-> SetBranchAddress("pwplus", &pwplus);
	chain-> SetBranchAddress("pwminus", &pwminus);

	TH1F * ct = new TH1F("costheta","costheta",50,-1.2,1.2);


	TLorentzVector w;
	TLorentzVector lep;
	TLorentzVector neu;

	cout<<chain->GetEntries()<<endl;
	for(int i=0; i<chain->GetEntries(); i++)
	{
		//if(i>5000)break;//for test
		if(i%5000==0)cout<<"Processing the events: "<<i<<endl;
		chain->GetEntry(i);
		if(!pmu->empty())lep.SetPtEtaPhiE(pmu->at(0)->pt(), pmu->at(0)->eta(),pmu->at(0)->phi(),pmu->at(0)->energy());
		else if(!pele->empty())lep.SetPtEtaPhiE(pele->at(0)->pt(), pele->at(0)->eta(),pele->at(0)->phi(),pele->at(0)->energy());
		else {
			cout<<"skip event: "<<i<<"  no lepton here"<<endl;
			continue;
		}
		neu.SetPxPyPzE(pnu->at(0)->px(), pnu->at(0)->py(),pnu->at(0)->pz(),pnu->at(0)->energy());
		w = lep + neu;
		//step 1
		TVector3 v1 = w.Vect();
		//step 2
		TLorentzVector lep_in_w(lep);
		TVector3 wboost = -(w.BoostVector());
		lep_in_w.Boost(wboost);
		//step 3
		TVector3 v2 = lep_in_w.Vect();
		//step 4
		double theta = v1.Angle(v2);
		//cout<<theta<<" "<<cos(theta)<<endl;
		//ct->Fill(theta);
		ct->Fill(cos(theta));
	}
	TCanvas *c1 = new TCanvas();
	ct->Draw();
	c1->Print("costheta.png","png");

}
