void CosTheta_ZZ(TString infilename = "Bulk_ZZ_genlevel_1000.root")
{
	TChain * chain = new TChain("demo/ExTree");
	chain->Add(infilename);


	vector<const reco::Candidate *> *phig;
	vector<const reco::Candidate *> *pmu;
	vector<const reco::Candidate *> *pele;
	vector<const reco::Candidate *> *pjet;
	vector<const reco::Candidate *> *pz0;
	chain-> SetBranchAddress("phig", &phig);
	chain-> SetBranchAddress("pmu", &pmu);
	chain-> SetBranchAddress("pele", &pele);
	chain-> SetBranchAddress("pjet", &pjet);
	chain-> SetBranchAddress("pz0", &pz0);

	TH1F * ct = new TH1F("costheta","costheta",10,-1.2,1.2);


	TLorentzVector z;
	TLorentzVector lep1;
	TLorentzVector lep2;
	double theta;

	cout<<chain->GetEntries()<<endl;
	for(int i=0; i<chain->GetEntries(); i++)
	{
		//if(i>5000)break;//for test
		if(i%5000==0)cout<<"Processing the events: "<<i<<endl;
		chain->GetEntry(i);
		if(!pmu->empty())
		{
			lep1.SetPtEtaPhiE(pmu->at(0)->pt(), pmu->at(0)->eta(),pmu->at(0)->phi(),pmu->at(0)->energy());
			lep2.SetPtEtaPhiE(pmu->at(1)->pt(), pmu->at(1)->eta(),pmu->at(1)->phi(),pmu->at(1)->energy());
		}

		else if(!pele->empty())
		{
			lep1.SetPtEtaPhiE(pele->at(0)->pt(), pele->at(0)->eta(),pele->at(0)->phi(),pele->at(0)->energy());
			lep2.SetPtEtaPhiE(pele->at(1)->pt(), pele->at(1)->eta(),pele->at(1)->phi(),pele->at(1)->energy());
		}
		else {
			cout<<"skip event: "<<i<<"  no lepton here"<<endl;
			continue;
		}
		if(pjet->size()!=2)
		{
			cout<<"skip event: "<<i<<" jet sise != 2 "<<endl;
			continue;
		}
		z = lep1 + lep2;
		TVector3 v1 = z.Vect();
		//lep1
		TLorentzVector lep_in_z(lep1);
		TVector3 zboost = -(z.BoostVector());
		lep_in_z.Boost(zboost);
		TVector3 v2 = lep_in_z.Vect();
		theta = v1.Angle(v2);
		ct->Fill(cos(theta));
		//lep2
        TLorentzVector lep_in_z2(lep2);
        lep_in_z2.Boost(zboost);
        TVector3 v3 = lep_in_z2.Vect();
        theta = v1.Angle(v3);
        ct->Fill(cos(theta));
	}
	TCanvas *c1 = new TCanvas();
	ct->Draw();
	c1->Print("costheta_Bulk.png","png");

}
