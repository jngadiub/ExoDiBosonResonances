#define NMAX 90
double deltaR(const double& eta1, const double& phi1,const double& eta2, const double& phi2)
{ 
	double deltaphi = deltaPhi(phi1, phi2);
	double deltaeta = deltaEta(eta1, eta2);
	double deltar = sqrt(deltaphi*deltaphi + deltaeta*deltaeta);
	return deltar;
}

double deltaPhi(const double& phi1, const double& phi2)
{ 
	double deltaphi = fabs(phi1 - phi2);
	if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
	if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi;
	return deltaphi;
}

double deltaEta(const double& eta1, const double& eta2)
{ 
	double deltaeta = fabs(eta1 - eta2);
	return deltaeta;
}


void MakeHist(TString infilename = "Graviton_genlevel_3000.root")
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

	//---------------------------------get  hist-----------------------------------------------
	//const float normal = 1000;// number of entries normalize to 1000

	//eta bin
	float inix= -2.5;
	float finx= 2.5;
	float nbinx= 20.0; 
	//pt bin
	float iniy= 0.0;
	float finy= 100.0;
	float nbiny= 20.0; 


	TH2F *h1= new TH2F("h1","h1",nbinx,inix,finx,nbiny,iniy,finy);
	h1->Sumw2();


	int nbins_lvjj      = 300;
	int masswindow_low  = 200;
	int masswindow_high = 4000;

	//histograms for differeny methods
	TH1F * hlvjj0 = new TH1F ("hlvjj0","hlvjj",nbins_lvjj,masswindow_low,masswindow_high);
	TH1F * hlvjj1 = new TH1F ("hlvjj1","hlvjj",nbins_lvjj,masswindow_low,masswindow_high);
	TH1F * hlvjj2 = new TH1F ("hlvjj2","hlvjj",nbins_lvjj,masswindow_low,masswindow_high);
	TH1F * hlvjj2_1 = new TH1F ("hlvjj2_1","hlvjj",nbins_lvjj,masswindow_low,masswindow_high);
	TH1F * hlvjj3 = new TH1F ("hlvjj3","hlvjj",nbins_lvjj,masswindow_low,masswindow_high);
	TH1F * hlvjj4 = new TH1F ("hlvjj4","hlvjj",nbins_lvjj,masswindow_low,masswindow_high);
	TH1F * hlvjj5 = new TH1F ("hlvjj5","hlvjj",nbins_lvjj,masswindow_low,masswindow_high);
	TH1F * hlvjj5_1 = new TH1F ("hlvjj5_1","hlvjj",nbins_lvjj,masswindow_low,masswindow_high);
	TH1F * hlvjj5_2 = new TH1F ("hlvjj5_2","hlvjj",nbins_lvjj,masswindow_low,masswindow_high);
	TH1F * hlvjj6 = new TH1F ("hlvjj6","hlvjj",nbins_lvjj,masswindow_low,masswindow_high);

	TH2F * h2= new TH2F("h2","h2",100,0,200,100,0,200); // for wlv and wjj
	TH2F * h3= new TH2F("h3","h3",5,0,200,5,0,200); // for percentage

	TH2F * hjjpt = new TH2F("hjjpt","hjjpt",10,0,1000,10,0,1000);
	TH2F * hlvpt = new TH2F("hlvpt","hlvpt",10,0,1000,10,0,1000);

	TH1F * hwlv = new TH1F("hwlv","hwlv",100,0,200);

	TH1F * hleppz_all = new TH1F("hleppz_all","hleppz_all",100,0,80);
	TH1F * hleppz_low = new TH1F("hleppz_low","hleppz_low",100,0,80);

	TH2F * hlvpz = new TH2F("hlvpz","hlvpz",10,0,1000,10,0,1000);

	double xbins[4] = {0,65,95,200};
	double ybins[4] = {0,65,95,200};
	h3->GetXaxis()->Set(3,xbins);
	h3->GetYaxis()->Set(3,ybins);


	//comparison hists
	TH1F * pz_comparison1 = new TH1F("pz_comparison1","pz_comparison",100,-2,2);
	TH1F * pz_comparison2 = new TH1F("pz_comparison2","pz_comparison",100,-2,2);
	TH1F * pz_comparison2_1 = new TH1F("pz_comparison2_1","pz_comparison",100,-2,2);
	TH1F * pz_comparison3 = new TH1F("pz_comparison3","pz_comparison",100,-2,2);
	TH1F * pz_comparison4 = new TH1F("pz_comparison4","pz_comparison",100,-2,2);
	TH1F * pz_comparison5 = new TH1F("pz_comparison5","pz_comparison",100,-2,2);
	TH1F * pz_comparison5_2 = new TH1F("pz_comparison5_2","pz_comparison",100,-2,2);
	TH1F * pz_comparison5_1 = new TH1F("pz_comparison5_1","pz_comparison",100,-2,2);
	TH1F * pz_comparison6 = new TH1F("pz_comparison6","pz_comparison",100,-2,2);
	TH1F * lvjj_comparison1 = new TH1F("lvjj_comparison1","lvjj_comparison",100,-2,2);
	TH1F * lvjj_comparison2 = new TH1F("lvjj_comparison2","lvjj_comparison",100,-2,2);
	TH1F * lvjj_comparison2_1 = new TH1F("lvjj_comparison2_1","lvjj_comparison",100,-2,2);
	TH1F * lvjj_comparison3 = new TH1F("lvjj_comparison3","lvjj_comparison",100,-2,2);
	TH1F * lvjj_comparison4 = new TH1F("lvjj_comparison4","lvjj_comparison",100,-2,2);
	TH1F * lvjj_comparison5 = new TH1F("lvjj_comparison5","lvjj_comparison",100,-2,2);
	TH1F * lvjj_comparison5_1 = new TH1F("lvjj_comparison5_1","lvjj_comparison",100,-2,2);
	TH1F * lvjj_comparison5_2 = new TH1F("lvjj_comparison5_2","lvjj_comparison",100,-2,2);
	TH1F * lvjj_comparison6 = new TH1F("lvjj_comparison6","lvjj_comparison",100,-2,2); 

	TCanvas * c1 = new TCanvas();	

	TLorentzVector lep;
	TLorentzVector jet1;
	TLorentzVector jet2;
	TLorentzVector neu;
	TLorentzVector temp;

	double mlvjj;
	double lvjjmass_true;
	cout<<chain->GetEntries()<<endl;
	for(int i=0; i<chain->GetEntries(); i++)
	{
		//if(i>5000)break;//for test
		if(i%5000==0)cout<<"Processing the events: "<<i<<endl;
		chain->GetEntry(i);
		//higgs 2D
		//h1->Fill(phig->at(0)->eta(),phig->at(0)->pt(),normal/(float)chain->GetEntries());

		//------------------------------neutrino pz
		if(!pmu->empty())lep.SetPtEtaPhiE(pmu->at(0)->pt(), pmu->at(0)->eta(),pmu->at(0)->phi(),pmu->at(0)->energy());
		else if(!pele->empty())lep.SetPtEtaPhiE(pele->at(0)->pt(), pele->at(0)->eta(),pele->at(0)->phi(),pele->at(0)->energy());
		else {
				cout<<"skip event: "<<i<<"  no lepton here"<<endl;
				//cout<<phig->size()<<endl;
				continue;
			}
		if(pjet->size()!=2)
		{
			cout<<"skip event: "<<i<<" jet sise != 2 "<<endl;
			continue;
		}
		jet1.SetPtEtaPhiE(pjet->at(0)->pt(), pjet->at(0)->eta(),pjet->at(0)->phi(),pjet->at(0)->energy());
		jet2.SetPtEtaPhiE(pjet->at(1)->pt(), pjet->at(1)->eta(),pjet->at(1)->phi(),pjet->at(1)->energy());
		if(jet1.Pt()<jet2.Pt())
		{
			temp=jet1;
			jet1=jet2;
			jet2=temp;
		}
		neu.SetPxPyPzE(pnu->at(0)->px(), pnu->at(0)->py(),pnu->at(0)->pz(),pnu->at(0)->energy());//real neutrino
		double px = pnu->at(0)->px();
		double py = pnu->at(0)->py();
		double pz=0;


		// ------------------------------- make cuts----------------------------
		//mt
		//double mt = sqrt(2. * lep.Pt() * neu.Pt() * ( 1 - cos(deltaPhi(lep.Phi(), neu.Phi()) ) ) );
		//if(mt>20)continue;

		//lepton pt,eta
		if(lep.Pt()<50)continue;
		//if(!pmu->empty()&&pmu->at(0)->pt()<24)continue;
		//if(!pele->empty()&&pele->at(0)->pt()<27)continue;
		//if(lep.Eta()>2.5)continue;

		//jet pt,eta
		if(jet1.Pt()<50)continue;
		//if(jet1.Eta()>2.4)continue;
		if(jet2.Pt()<50)continue;
		//if(jet2.Eta()>2.4)continue;

		//met or neutrino pt
		//double met = sqrt(px*px+py*py);
		if(neu.Pt()<50)continue;

		//mjj
		//double mjj = (jet1+jet2).M();
		//if(mjj>95||mjj<65)continue; //wjj on
		//if(mjj<95&&mjj>65)continue;   //wjj off


		// 0---------direct method
		pz = pnu->at(0)->pz();
		temp.SetPxPyPzE(px,py,pz,sqrt(px*px+py*py+pz*pz));
		mlvjj = (lep + jet1 + jet2 + temp).M();
		if (mlvjj>masswindow_low&&mlvjj<masswindow_high)hlvjj0->Fill(mlvjj);
		lvjjmass_true = mlvjj;

/*
		// 1----------nu pz = 0
		pz = 0;
		temp.SetPxPyPzE(px,py,pz,sqrt(px*px+py*py+pz*pz));
		mlvjj = (lep + jet1 + jet2 + temp).M();
		if (mlvjj>masswindow_low&&mlvjj<masswindow_high)hlvjj1->Fill(mlvjj);
        pz_comparison1->Fill((pz-pnu->at(0)->pz())/pnu->at(0)->pz());
        lvjj_comparison1->Fill((mlvjj-lvjjmass_true)/lvjjmass_true);

		// 2----------nu pz = lep pz
		pz=lep.Pz();
		temp.SetPxPyPzE(px,py,pz,sqrt(px*px+py*py+pz*pz));
		mlvjj = (lep + jet1 + jet2 + temp).M();
		if (mlvjj>masswindow_low&&mlvjj<masswindow_high)hlvjj2->Fill(mlvjj);
        pz_comparison2->Fill((pz-pnu->at(0)->pz())/pnu->at(0)->pz());
        lvjj_comparison2->Fill((mlvjj-lvjjmass_true)/lvjjmass_true);

		//--check pz
		//hleppz_all->Fill(pz);
		//if(mlvjj>130)continue;
		//hleppz_low->Fill(pz);

		//2_1 ---------nu pz =lep pz -x 
		pz=lep.Pz();
		int pz_correct=20;
		if(pz>pz_correct)pz=pz-pz_correct;
		if(pz<-pz_correct)pz=pz+pz_correct;
		temp.SetPxPyPzE(px,py,pz,sqrt(px*px+py*py+pz*pz));
		mlvjj = (lep + jet1 + jet2 + temp).M();
		if (mlvjj>masswindow_low&&mlvjj<masswindow_high)hlvjj2_1->Fill(mlvjj);
        pz_comparison2_1->Fill((pz-pnu->at(0)->pz())/pnu->at(0)->pz());
        lvjj_comparison2_1->Fill((mlvjj-lvjjmass_true)/lvjjmass_true);



		// 3----------nu pz = far jet pz----------------use theta to choose
		double angle1 = lep.Vect().Angle(jet1.Vect());
		double angle2 = lep.Vect().Angle(jet2.Vect());
		if(angle1>angle2)pz=jet1.Pz();
		else pz = jet2.Pz();
		temp.SetPxPyPzE(px,py,pz,sqrt(px*px+py*py+pz*pz));
		mlvjj = (lep + jet1 + jet2 + temp).M();
		if (mlvjj>masswindow_low&&mlvjj<masswindow_high)hlvjj3->Fill(mlvjj);
        pz_comparison3->Fill((pz-pnu->at(0)->pz())/pnu->at(0)->pz());
        lvjj_comparison3->Fill((mlvjj-lvjjmass_true)/lvjjmass_true);



		//4 ---------nu pz = far jet pz--------------------use DetaR to choose
		double dR1 = deltaR(lep.Eta(),lep.Phi(),jet1.Eta(),jet1.Phi());
		double dR2 = deltaR(lep.Eta(),lep.Phi(),jet2.Eta(),jet2.Phi());
		if(dR1>dR2)pz=jet1.Pz();
		else pz = jet2.Pz();
		temp.SetPxPyPzE(px,py,pz,sqrt(px*px+py*py+pz*pz));
		mlvjj = (lep + jet1 + jet2 + temp).M();
		if (mlvjj>masswindow_low&&mlvjj<masswindow_high)hlvjj4->Fill(mlvjj);
        pz_comparison4->Fill((pz-pnu->at(0)->pz())/pnu->at(0)->pz());
        lvjj_comparison4->Fill((mlvjj-lvjjmass_true)/lvjjmass_true);

*/
		// 5 ---------------------slove the function ------------milano

		float alpha = lep.Px()*pnu->at(0)->px() + lep.Py()*pnu->at(0)->py();

		float delta = (alpha + 0.5*80.399*80.399)*(alpha + 0.5*80.399*80.399) - lep.Pt()*lep.Pt()*pnu->at(0)->pt()*pnu->at(0)->pt();
		if( delta < 0. ) {
			delta = 0.;
		}
			//double wlv = (lep+neu).M();
			//hwlv->Fill(wlv);
			//cout<<"event with imaginary roots :"<<i<<" mwlv is  "<<wlv<<endl;
		
		//else continue;//use only imaginary root

		float pz1 = ( lep.Pz()*(alpha + 0.5*80.399*80.399) + lep.Energy()*sqrt(delta) ) / lep.Pt() / lep.Pt();
		float pz2 = ( lep.Pz()*(alpha + 0.5*80.399*80.399) - lep.Energy()*sqrt(delta) ) / lep.Pt() / lep.Pt();

		if( delta >= 0. )//choose the larger pz
		{
			if(fabs(pz1)<fabs(pz2))pz=pz2;
			else pz=pz1;   
		}
		//else continue;//discard unreal pz
		temp.SetPxPyPzE(px,py,pz,sqrt(px*px+py*py+pz*pz));
		mlvjj = (lep + jet1 + jet2 + temp).M();
		if (mlvjj>masswindow_low&&mlvjj<masswindow_high)hlvjj5->Fill(mlvjj);
        pz_comparison5->Fill((pz-pnu->at(0)->pz())/pnu->at(0)->pz());
        lvjj_comparison5->Fill((mlvjj-lvjjmass_true)/lvjjmass_true);


		//4 
		if( delta >= 0. )//choose the smaller pz
        {   
            if(fabs(pz1)>fabs(pz2))pz=pz2;
            else pz=pz1;   
        }
        temp.SetPxPyPzE(px,py,pz,sqrt(px*px+py*py+pz*pz));
        mlvjj = (lep + jet1 + jet2 + temp).M();
        if (mlvjj>masswindow_low&&mlvjj<masswindow_high)hlvjj4->Fill(mlvjj);
        pz_comparison4->Fill((pz-pnu->at(0)->pz())/pnu->at(0)->pz());
        lvjj_comparison4->Fill((mlvjj-lvjjmass_true)/lvjjmass_true);


		
		// 5_1 ---------------- Sara---if you have real roots, take the solutions where neutrino and charged lepton are the closest in DR.
		if( delta >= 0. )	
		{
			pz=pz1;
			temp.SetPxPyPzE(px,py,pz,sqrt(px*px+py*py+pz*pz));
			//cout<<temp.Phi()<<" "<<neu.Phi()<<endl;
			double dR1 = deltaR(lep.Eta(),lep.Phi(),temp.Eta(),temp.Phi());
			pz=pz2;
			temp.SetPxPyPzE(px,py,pz,sqrt(px*px+py*py+pz*pz));
			double dR2 = deltaR(lep.Eta(),lep.Phi(),temp.Eta(),temp.Phi());
			if(dR1>dR2)pz=pz2;
			else pz=pz1;
		}
		//else continue;//discard unreal pz
		temp.SetPxPyPzE(px,py,pz,sqrt(px*px+py*py+pz*pz));
		mlvjj = (lep + jet1 + jet2 + temp).M();
		if (mlvjj>masswindow_low&&mlvjj<masswindow_high)hlvjj5_1->Fill(mlvjj);
		pz_comparison5_1->Fill((pz-pnu->at(0)->pz())/pnu->at(0)->pz());
		lvjj_comparison5_1->Fill((mlvjj-lvjjmass_true)/lvjjmass_true);


		//5_2 -------------------- another method to select right real root
			//use also delta = 0 in real root case
			//use +delta root
		pz=pz1;		
        temp.SetPxPyPzE(px,py,pz,sqrt(px*px+py*py+pz*pz));
        mlvjj = (lep + jet1 + jet2 + temp).M();
        if (mlvjj>masswindow_low&&mlvjj<masswindow_high)hlvjj5_2->Fill(mlvjj);
        pz_comparison5_2->Fill((pz-pnu->at(0)->pz())/pnu->at(0)->pz());
        lvjj_comparison5_2->Fill((mlvjj-lvjjmass_true)/lvjjmass_true);
		

		// 6 --------------------- Fracesco----boost case lepton and neutrino are like parallel
		pz = neu.Pt()/lep.Pt()*lep.Pz();
		temp.SetPxPyPzE(px,py,pz,sqrt(px*px+py*py+pz*pz));
		mlvjj = (lep + jet1 + jet2 + temp).M();
		if (mlvjj>masswindow_low&&mlvjj<masswindow_high)hlvjj6->Fill(mlvjj);
        pz_comparison6->Fill((pz-pnu->at(0)->pz())/pnu->at(0)->pz());
        lvjj_comparison6->Fill((mlvjj-lvjjmass_true)/lvjjmass_true);




		//------------------------------2D plots of Wjj and Wlv mass----------------------------------------
		//double wlv = (lep+neu).M();
		//double wjj = (jet1+jet2).M();

		//hwlv->Fill(wlv);

		//h2-> Fill(wlv,wjj);
		//h3-> Fill(wlv,wjj,1.0/(float)chain->GetEntries());
		//h3-> Fill(wlv,wjj);

		//hjjpt->Fill(jet1.Pt(),jet2.Pt());
		//hlvpt->Fill(lep.Pt(),neu.Pt());
		//hlvpz->Fill(abs(lep.Pz()),abs(neu.Pz()));

	}

/*
	h2-> GetXaxis()->SetTitle("wlv");
	h2-> GetYaxis()->SetTitle("wjj");

	h3-> GetXaxis()->SetTitle("wlv");
	h3-> GetYaxis()->SetTitle("wjj");
	h3->Draw("COLZ");
	h3->Draw("TEXTEsame");
	h3->SetStats(0);
	c1->Print("percent.png","png");
	//h1->Draw("SURF1");


	hjjpt-> GetXaxis()->SetTitle("leading jet pt");
	hjjpt-> GetYaxis()->SetTitle("jet2 pt");
	hlvpt-> GetXaxis()->SetTitle("lepton pt");
	hlvpt-> GetYaxis()->SetTitle("neotrino pt");
	hjjpt->Draw("COLZ");
	hjjpt->Draw("TEXTEsame");
	c1->Print("jjpt.png","png");
	hlvpt->Draw("COLZ");
	hlvpt->Draw("TEXTEsame");
	c1->Print("lvpt.png","png");


	hlvpz-> GetXaxis()->SetTitle("lepton pz");
	hlvpz-> GetYaxis()->SetTitle("neotrino pz");
	hlvpz->Draw("COLZ");
	hlvpz->Draw("TEXTEsame");
	c1->Print("lvpz.png","png");

*/

	hlvjj0->Draw();
	hlvjj0->SetTitle("lvjj_original");
	c1->Print("lvjj_original.png","png");

/*
	hlvjj1->Draw();
	hlvjj1->SetTitle("lvjj_pz0");
	c1->Print("lvjj_pz0.png","png");

	hlvjj2->Draw();
	hlvjj2->SetTitle("lvjj_leptonpz");
	c1->Print("lvjj_leptonpz.png","png");

	hlvjj2_1->Draw();
	hlvjj2_1->SetTitle("lvjj_leptonpz_improve");
	c1->Print("lvjj_leptonpz_improve.png","png");

	hlvjj3->Draw();
	hlvjj3->SetTitle("lvjj_jetpz_angle");
	c1->Print("lvjj_jetpz_angle.png","png");
*/
	hlvjj4->Draw();
	//hlvjj4->SetTitle("lvjj_jetpz_DeltaR");
	//c1->Print("lvjj_jetpz_DeltaR.png","png");
	hlvjj4->SetTitle("lvjj_function_smaller");
	c1->Print("lvjj_function_smaller.png","png");

	hlvjj5->Draw();
	hlvjj5->SetTitle("lvjj_function_larger");
	c1->Print("lvjj_function_larger.png","png");

	hlvjj5_1->Draw();
	hlvjj5_1->SetTitle("lvjj_function_sara");
	c1->Print("lvjj_function_sara.png","png");

    hlvjj5_2->Draw();
    hlvjj5_2->SetTitle("lvjj_function_pz1");
    c1->Print("lvjj_function_pz1.png","png");


	hlvjj6->Draw();
	hlvjj6->SetTitle("lvjj_Francesco");
	c1->Print("lvjj_Francesco.png","png");

	//hwlv->Draw();
	//hwlv->SetTitle("hwlv");
	//c1->Print("hwlv.png","png");

	//hleppz_all->Draw();
	//c1->Print("hleppz_all.png","png");
	//hleppz_low->Draw();
	//c1->Print("hleppz_low.png","png");


	TLegend *leg = new TLegend(0.65, 0.5, 1, 1, NULL, "brNDC");
	/*
	leg->AddEntry (pz_comparison1,"neu pz = 0","l") ;
	leg->AddEntry (pz_comparison2,"neu pz = lep pz","l") ;
	leg->AddEntry (pz_comparison2_1,"neu pz = lep pz - 20","l") ;
	leg->AddEntry (pz_comparison3,"neu pz = theta far jet pz","l") ;
	leg->AddEntry (pz_comparison4,"neu pz = dltaR far jet pz","l") ;
*/
	leg->AddEntry (pz_comparison4,"neu pz = slove function smaller","l");
	leg->AddEntry (pz_comparison5,"neu pz = slove function larger","l") ;
	leg->AddEntry (pz_comparison5_1,"neu pz = slove function sara","l") ;
	leg->AddEntry (pz_comparison5_2,"neu pz = slove function +delta","l") ;
	leg->AddEntry (pz_comparison6,"neu pz = Francesco","l") ;

/*
	pz_comparison1->SetLineColor(kYellow+3);
	pz_comparison1->Draw();
    pz_comparison2->SetLineColor(kOrange+7);
    pz_comparison2->Draw("same");
    pz_comparison2_1->SetLineColor(kGrey;
    pz_comparison2_1->Draw("same");
    pz_comparison3->SetLineColor(kMagenta+1);
    pz_comparison3->Draw("same");
*/
    pz_comparison4->SetLineColor(kMagenta+1);
    pz_comparison4->Draw();
    pz_comparison5->SetLineColor(kBlue);
    pz_comparison5->Draw("same");
	pz_comparison5_1->SetLineColor(kSpring-7);
	pz_comparison5_1->Draw("same");
	pz_comparison5_2->SetLineColor(kAzure+10);
	pz_comparison5_2->Draw("same");
    pz_comparison6->SetLineColor(kRed);
    pz_comparison6->Draw("same");
	leg->Draw();
	c1->Print("pz_comparison.png","png");

/*
    lvjj_comparison1->SetLineColor(kYellow+3);
    lvjj_comparison1->Draw();
    lvjj_comparison2->SetLineColor(kOrange+7);
    lvjj_comparison2->Draw("same");
    lvjj_comparison2_1->SetLineColor(kGrey);
    lvjj_comparison2_1->Draw("same");
    lvjj_comparison3->SetLineColor(kMagenta+1);
    lvjj_comparison3->Draw("same");
*/
    lvjj_comparison4->SetLineColor(kMagenta+1);
    lvjj_comparison4->Draw();
    lvjj_comparison5->SetLineColor(kBlue);
    lvjj_comparison5->Draw("same");
	lvjj_comparison5_1->SetLineColor(kSpring-7);
	lvjj_comparison5_1->Draw("same");
    lvjj_comparison5_2->SetLineColor(kAzure+10);
    lvjj_comparison5_2->Draw("same");
    lvjj_comparison6->SetLineColor(kRed);
    lvjj_comparison6->Draw("same");
	leg->Draw();
    c1->Print("lvjj_comparison.png","png");

	leg->AddEntry (hlvjj0,"neu pz = true","l") ;

	hlvjj0->SetLineColor(kBlack);
	hlvjj0->SetTitle("hlvjj_allin");
	hlvjj0->Draw();
    /*
	hlvjj1->SetLineColor(kYellow+3);
    hlvjj1->Draw("same");
    hlvjj2->SetLineColor(kOrange+7);
    hlvjj2->Draw("same");
    hlvjj2_1->SetLineColor(kGrey);
    hlvjj2_1->Draw("same");
    hlvjj3->SetLineColor(kMagenta+1);
    hlvjj3->Draw("same");
*/
    hlvjj4->SetLineColor(kMagenta+1);
    hlvjj4->Draw("same");
	hlvjj5->SetLineColor(kBlue);
    hlvjj5->Draw("same");
	hlvjj5_1->SetLineColor(kSpring-7);
	hlvjj5_1->Draw("same");
    hlvjj5_2->SetLineColor(kAzure+10);
    hlvjj5_2->Draw("same");
    hlvjj6->SetLineColor(kRed);
    hlvjj6->Draw("same");
	leg->Draw();
    c1->Print("hlvjj_allin.png","png");



}
