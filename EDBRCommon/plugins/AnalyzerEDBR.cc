#include "ExoDiBosonResonances/EDBRCommon/plugins/AnalyzerEDBR.h"


AnalyzerEDBR::AnalyzerEDBR(const edm::ParameterSet &ps){

	debug_    = ps.getParameter<bool>("debug");
	if(debug_)    cout << "AnalyzerEDBR constructor 555..." << endl;
	cout<<"pippo1"<<endl;
	isMC_       = ps.getParameter<bool>("isMC");
	treatVBFAsMultiple_    = ps.getParameter<bool>("treatVBFAsMultiple");
	saveVBFCands_   = ps.getParameter<bool>("saveVBFTaggedCands");
	Ngen_     = ps.getParameter<unsigned int>("Ngen");
	xsec_     = ps.getParameter<double>("xsec"); // in fb
	cout<<"pippo2"<<endl;
	cat_             = ps.getParameter<std::string>("EventCategory");
	XEEColl_         = ps.getParameter<edm::InputTag>("EDBREEJJColl");
	XEELDMap_        = ps.getParameter<edm::InputTag>("EDBREEJJLDValueMap");
	XMMColl_         = ps.getParameter<edm::InputTag>("EDBRMMJJColl");
	XMMLDMap_        = ps.getParameter<edm::InputTag>("EDBRMMJJLDValueMap");
	XEEJColl_        = ps.getParameter<edm::InputTag>("EDBREEJColl");
	XEEJLDMap_       = ps.getParameter<edm::InputTag>("EDBREEJLDValueMap");
	XMMJColl_        = ps.getParameter<edm::InputTag>("EDBRMMJColl");
	XMMJLDMap_       = ps.getParameter<edm::InputTag>("EDBRMMJLDValueMap");
	XQGMap_          = ps.getParameter<edm::InputTag>("EDBRQGValueMap");
	VType_           = ps.getParameter<std::string>("VType");
	fillGen_         = ps.getParameter<unsigned int>("FillGenLevelCode");
	//XEENoKinFitLDMap_=ps.getParameter<edm::InputTag>("EDBREENoKinFitLDValueMap");
	//XMMNoKinFitLDMap_=ps.getParameter<edm::InputTag>("EDBRMMNoKinFitLDValueMap");

	if(VType_=="Z"){
		cmgEDBRMu_="cmgEDBRZZMu";
		cmgEDBREle_="cmgEDBRZZEle";
		VpdgId_=23;
		VMass_= 91.1876;
	}
	else if(VType_=="W"){
		cmgEDBRMu_="cmgEDBRWWMu";
		cmgEDBREle_="cmgEDBRWWEle";
		VpdgId_=24;
		VMass_= 80.4;
	} 
	else{
		throw cms::Exception("Wrong parameter")<<"Unrecognized VType paramter: "<<VType_.c_str()<<" . Allowed options are : W ; Z ."<<std::endl;

	}

	if(XEELDMap_.label()=="" ||XMMLDMap_.label()=="" ) readLDFromUserFloat_=true;
	else readLDFromUserFloat_=false;

	if(XQGMap_.label()=="") readQGFromUserFloat_=true;
	else readQGFromUserFloat_=false;


	outFileName_ = ps.getParameter<string>("outFileName");
	outFile_ = new TFile(outFileName_.c_str(),"recreate");
	if(debug_)cout<<"Storing output TTree in "<<outFileName_.c_str()<<endl;

	triggerNames_ = ps.getParameter< vector<string> >("triggerNames");

	if(cat_!=""){
		if( cat_!="mmjj" &&cat_!="eejj" && cat_!="mmj" && cat_!="eej"){
			throw cms::Exception("AnalyzerEDBR: Wrong event category passed as input. Possibilities are: mmjj, eejj, mmj, eej");
		}
	}

	if(debug_)cout<<"Initializing"<<endl;

	init();
	if(debug_)cout<<"Initialization is over"<<endl;

}//end constructor


void AnalyzerEDBR::analyze(edm::Event const& iEvent, edm::EventSetup const& eventSetup){

	//use these for X->ZZ analysis
	/*		
	typedef  cmg::DiElectronSingleJetEDBR cmgEleSingleJetEDBR ;
	typedef  cmg::DiMuonSingleJetEDBR     cmgMuSingleJetEDBR  ;
	typedef  cmg::DiElectronDiJetEDBR     cmgEleDiJetEDBR  ;
	typedef  cmg::DiMuonDiJetEDBR     cmgMuDiJetEDBR  ;
	*/
	//use these for X->WW analysis
			
	typedef  cmg::WelenuSingleJetEDBR cmgEleSingleJetEDBR ;
	typedef  cmg::WmunuSingleJetEDBR     cmgMuSingleJetEDBR  ; 
	typedef  cmg::WelenuDiJetEDBR     cmgEleDiJetEDBR  ;
	typedef  cmg::WmunuDiJetEDBR     cmgMuDiJetEDBR  ;

	
	nEvt++;

	if(debug_) cout<<"\n\nAnalyzing event"<<endl;
	initDataMembers();

	nevent = iEvent.eventAuxiliary().event();
	run    = iEvent.eventAuxiliary().run();
	ls     = iEvent.eventAuxiliary().luminosityBlock();
	if(debug_) cout<<endl<<" Run "<<run<<"  Event "<<nevent<<endl;


	// GET VERTICES
	edm::Handle<reco::VertexCollection> vertexCollection;
	if(iEvent.getByLabel("offlinePrimaryVertices",vertexCollection))  nvtx=vertexCollection->size();
	else{
		if(debug_)cout<<"WARNING: VertexCollection called \'offlinePrimaryVertices\' NOT FOUND !"<<endl;
		nvtx=0;
	}
	npu=-1;

	//total number of pre-selected jets in the event
	edm::Handle<std::vector<cmg::PFJet> > allJets;
	iEvent.getByLabel("jetIDJet", allJets);
	njets = allJets->size();

	// GET MISSING ET
	edm::Handle<edm::View<pat::MET> > metHandle;
	iEvent.getByLabel("patMETs", metHandle);
	met     = metHandle->at(0).pt();
	metSign = metHandle->at(0).significance(); 

	analyzeTrigger(iEvent, eventSetup);

	///  bool eleEvent   = elePath_; //&&(cat_=="eejj" || cat_=="eej")
	///  bool muEvent    = muPath_;
	bool goodKinFit = true;
	bool singleJetEvent=false;
	bool doubleJetEvent=false;

	if(isMC_) analyzeGenLevel(iEvent, eventSetup);


	// edm::Handle<edm::ValueMap<float> > qgmap;
	// if(anyPath_){
	//  if(!readQGFromUserFloat_)iEvent.getByLabel(HiggsQGMap_, qgmap);
	// }


	//edm::Handle<std::vector< cmg::DiPFJet > >   dijets;
	//  edm::Handle<std::vector< cmg::DiObject<cmg::PFJet,cmg::PFJet> > >   dijetskinfit;
	// edm::Handle<std::vector< cmg::DiObject<cmg::PFJet,cmg::PFJet> > >  zjjs;
	//if( iEvent.getByLabel(  "cmgDiJet"  , dijets  ) && iEvent.getByLabel(  "cmgDiJetKinFit"  , dijetskinfit  ) && iEvent.getByLabel(  "ZjjCand"  , zjjs  ) ){                 
	// cout<<"Size of Dijet Collections---> cmgDiJet: "<<dijets->size()<<"   cmgDijetKinFit: "<<dijetskinfit->size()<<"  Zjj: "<<zjjs->size()<<endl;
	// }

	if(muPath_){


		if(singleJetPath_){

			edm::Handle<edm::View< cmgMuSingleJetEDBR > > finalEDBRcand;
			iEvent.getByLabel(XMMJColl_        , finalEDBRcand  );  // With kinfit


			int nCollCandidates=finalEDBRcand->size();
			if ( nCollCandidates+nCands > nMaxCand ) nCollCandidates = nMaxCand - nCands;
			if(debug_)cout<<"read from MUON event, there are "<<nCollCandidates<<" X->ZZ->2L1J cands"<<endl;
			int ih = nCands;
			bool vbfFound=false;
			for(int icand=0;icand<nCollCandidates;icand++){

				edm::RefToBase<cmgMuSingleJetEDBR> edbrM =finalEDBRcand->refAt(icand);

				//	  if(edbrM->nJets()!=1){
				//throw cms::Exception("Mismatched param") <<"Event in SingleJet Path has "<<edbrM->nJets()
				//				     <<" jets"<<std::endl;  
				//}

				bool keepThisVBFCand=true;
				int vbfTest=checkVBFTag(edbrM,ih,vbfFound,keepThisVBFCand);
				if(vbfTest>0)vbfFound=true;	  
				if(vbfTest>0 && !saveVBFCands_)continue; //skip all vbf-tagged cands	 	 
				if(vbfTest>0 &&!keepThisVBFCand) continue; 

				VBFTag[ih] = vbfTest;

				analyzeGeneric(edbrM, ih);
				analyzeSingleJet(edbrM,ih);        
				analyzeMuon(edbrM,ih);
				analyzeVBF(edbrM,ih, vbfTest);

				ih++;
			}//end loop on candidates
			// if(debug_)cout<<"Adding "<<ih<<" muCands"<<endl;
			nCands = ih;
			singleJetEvent=true;
		}//end if singleJetPath

		if(doubleJetPath_){
			edm::Handle<edm::View< cmgMuDiJetEDBR > > finalEDBRcand;
			iEvent.getByLabel(XMMColl_        , finalEDBRcand  );  // With kinfit


			int nCollCandidates=finalEDBRcand->size();

			if (nCollCandidates+nCands> nMaxCand) nCollCandidates = nMaxCand-nCands;

			if(debug_)cout<<"read from MUON event, there are "<<nCollCandidates<<" X->ZZ->2L2J cands"<<endl;
			int ih = nCands;
			bool vbfFound=false;
			for(int icand=0;icand<nCollCandidates;icand++){

				edm::RefToBase<cmgMuDiJetEDBR> edbrM =finalEDBRcand->refAt(icand);


				//  if(edbrM->nJets()!=2){
				//  throw cms::Exception("Mismatched param") <<"Event in DoubleJet Path has "<<edbrM->nJets()
				//						     <<" jets"<<std::endl;  
				// }

				bool keepThisVBFCand=true;
				int vbfTest=checkVBFTag(edbrM,ih,vbfFound,keepThisVBFCand);
				if(vbfTest>0)vbfFound=true;	  
				if(vbfTest>0 && !saveVBFCands_)continue; //skip all vbf-tagged cands	 	 
				if(vbfTest>0 &&!keepThisVBFCand) continue; 

				VBFTag[ih] = vbfTest;

				analyzeGeneric(edbrM,ih);
				analyzeDoubleJet(edbrM,ih,goodKinFit);        
				analyzeMuon(edbrM,ih);
				analyzeVBF(edbrM,ih, vbfTest);

				ih++;
			}//end loop on candidates

			nCands = ih;
			doubleJetEvent=true;
		}//end if doubleJetPath

	}//end if mmjj


	//  if(cat_=="eejj"){
	if(elePath_){


		if(singleJetPath_){
			edm::Handle<edm::View< cmgEleSingleJetEDBR > > finalEDBRcand;
			iEvent.getByLabel(XEEJColl_        , finalEDBRcand  );  // With kinfit


			int nCollCandidates=finalEDBRcand->size();
			if (nCollCandidates+nCands > nMaxCand) nCollCandidates = nMaxCand-nCands;
			if(debug_)cout<<"read from ELECTRON event, there are "<<nCollCandidates<<" X->ZZ->2L1J cands"<<endl;
			int ih = nCands;
			bool vbfFound=false;
			for(int icand=0;icand<nCollCandidates;icand++){

				edm::RefToBase< cmgEleSingleJetEDBR > edbrE =finalEDBRcand->refAt(icand);

				//  if(edbrE->nJets()!=1){
				//  throw cms::Exception("Mismatched param") <<"Event in SingleJet Path has "<<edbrE->nJets()
				//						     <<" jets"<<std::endl;  
				// }

				bool keepThisVBFCand=true;
				int vbfTest=checkVBFTag(edbrE,ih,vbfFound,keepThisVBFCand);
				if(vbfTest>0)vbfFound=true;	  
				if(vbfTest>0 && !saveVBFCands_)continue; //skip all vbf-tagged cands	 	 
				if(vbfTest>0 &&!keepThisVBFCand) continue; 

				VBFTag[ih] = vbfTest;

				analyzeGeneric(edbrE, ih);
				analyzeSingleJet(edbrE,ih);        
				analyzeElectron(edbrE,ih);
				analyzeVBF(edbrE,ih, vbfTest);

				ih++;
			}//end loop on candidates
			// if(debug_)cout<<"Adding "<<ih<<" muCands"<<endl;
			nCands = ih;

		}//end if singleJetPath



		if(doubleJetPath_){
			edm::Handle<edm::View< cmgEleDiJetEDBR > > finalEDBRcand;
			iEvent.getByLabel(XEEColl_        , finalEDBRcand  );  // With kinfit

			int nCollCandidates=finalEDBRcand->size();
			if (nCollCandidates+nCands > nMaxCand) nCollCandidates = nMaxCand-nCands;

			if(debug_){
				cout<<"read from ELECTRON event, there are "<<nCollCandidates<<" X->ZZ->2L2J cands"<<endl;


				for(int icand=0;icand<nCollCandidates;icand++){
					cout<<"Loop on ELE cand ih="<<icand<<"  Mass="<<finalEDBRcand->refAt(icand)->mass() <<" Mll="<<finalEDBRcand->refAt(icand)->leg1().mass() <<" Mjj="<<finalEDBRcand->refAt(icand)->leg2().mass() <<" Etajj="<<finalEDBRcand->refAt(icand)->leg2().eta()<<std::flush;
					if(finalEDBRcand->refAt(icand)->vbfptr().isAvailable())cout<<" Mass_vbf="<<finalEDBRcand->refAt(icand)->vbfptr()->mass()<<std::endl;
					else cout<<" Mass_vbf= n.a."<<std::endl;
				}


			}//end if debug

			int ih = nCands;
			bool vbfFound=false;
			for(int icand=0;icand<nCollCandidates;icand++){
				//  cout<<"Loop on ELE cand ih="<<icand<<std::flush;
				edm::RefToBase<cmgEleDiJetEDBR> edbrE =finalEDBRcand->refAt(icand);


				// if(edbrE->nJets()!=2){
				//  throw cms::Exception("Mismatched param") <<"Event in DoubleJet Path has "<<edbrE->nJets()
				//						     <<" jets"<<std::endl;  
				// }

				bool keepThisVBFCand=true;
				int vbfTest=checkVBFTag(edbrE,ih,vbfFound,keepThisVBFCand);
				if(vbfTest>0)vbfFound=true;	  
				if(vbfTest>0 && !saveVBFCands_)continue; //skip all vbf-tagged cands	 	 
				if(vbfTest>0 &&!keepThisVBFCand) continue; 

				VBFTag[ih] = vbfTest;

				analyzeGeneric(edbrE,ih);
				analyzeDoubleJet(edbrE, ih,goodKinFit);        
				analyzeElectron(edbrE,ih);
				analyzeVBF(edbrE,ih, vbfTest);

				ih++;
			}//end loop on candidates
			// if(debug_)cout<<"Adding "<<ih<<" muCands"<<endl;
			nCands = ih;
		}//end if doubleJetPath
	}//end if electron channel


	//EVENT WEIGHTS
	if(isMC_){
		// Ngen=Ngen_;
		lumiw = xsec_/Ngen_;
		edm::Handle<GenEventInfoProduct> hGenEvtInfo;
		// cout<<"Am I crashing ?  "<<flush;
		if(iEvent.getByLabel("generator",hGenEvtInfo)){
			genw=hGenEvtInfo->weights()[0];
			// cout<<"  No ! "<<genw<<endl;
		}
	}
	if(debug_)  cout<<"lumi weight="<<lumiw<<"  PU weight="<<PU<<endl;
	w  = PU *HLTSF*genw*lumiw;
	wA = PUA*HLTSF*genw*lumiw;
	wB = PUB*HLTSF*genw*lumiw;


	bool passCuts=true;
	bool storeEvt=goodKinFit && (muPath_ || elePath_)  && passCuts;
	//  if(debug_&& singleJetEvent&&doubleJetEvent)cout<<" Run "<<run<<"  Event "<<nevent<<"\tThis muon-event has both single and double jet topology."<<endl;
	if( singleJetEvent&&doubleJetEvent &&debug_)cout<<" Run "<<run<<"  Event "<<nevent<<"\tThis muon-event has both single and double jet topology."<<endl;
	if(storeEvt){
		//  if(debug_)cout<<"Filling the tree ("<<nCands<<endl;//" cands -> pTlep1="<< ptlep1[nCands-1]<<")"<<endl;
		outTree_->Fill(); 
		//  if(debug_)cout<<" filled."<<endl;
	}

	/// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

	if(debug_)cout<<"Finished AnalyzerEDBR::analyze for Run "<<run<<"  Event "<<nevent<<endl;
}//
//end AnalyzerEDBR::analyze



void AnalyzerEDBR::init(){

	initTree();
	nEvt=0;



}//end init()


void AnalyzerEDBR::initTree(){

	if(debug_)cout<<"creating the output TTree"<<endl;
	outTree_ = new TTree("SelectedCandidates","angles etc.");
	outTree_->Branch("nEvt"            ,&nEvt          ,"nEvt/I"                 );
	outTree_->Branch("nCands"          ,&nCands        ,"nCands/I"               );
	outTree_->Branch("event"           ,&nevent        ,"event/i"                );
	outTree_->Branch("run"             ,&run           ,"run/i"                  );
	outTree_->Branch("ls"              ,&ls            ,"ls/i"                   );
	outTree_->Branch("cosThetaStar"    ,&hs            ,"cosThetaStar[nCands]/D" );
	outTree_->Branch("cosTheta1"       ,&h1            ,"cosTheta1[nCands]/D"    );
	outTree_->Branch("cosTheta2"       ,&h2            ,"cosTheta2[nCands]/D"    );
	outTree_->Branch("phi"             ,&phi           ,"phi[nCands]/D"          );
	outTree_->Branch("phiStar1"        ,&phiS1         ,"phiStar1[nCands]/D"     );
	outTree_->Branch("ptlep1"          ,&ptlep1        ,"ptlep1[nCands]/D"       );
	outTree_->Branch("ptlep2"          ,&ptlep2        ,"ptlep2[nCands]/D"       );
	outTree_->Branch("ptjet1"          ,&ptjet1        ,"ptjet1[nCands]/D"       );
	outTree_->Branch("ptjet2"          ,&ptjet2        ,"ptjet2[nCands]/D"       );
	outTree_->Branch("ptZll"           ,&ptZll         ,"ptZll[nCands]/D"        );
	outTree_->Branch("ptZjj"           ,&ptZjj         ,"ptZjj[nCands]/D"        );
	outTree_->Branch("yZll"            ,&yZll          ,"yZll[nCands]/D"         );
	outTree_->Branch("yZjj"            ,&yZjj          ,"yZjj[nCands]/D"         );
	outTree_->Branch("phiZll"          ,&phiZll        ,"phiZll[nCands]/D"       );
	outTree_->Branch("phiZjj"          ,&phiZjj        ,"phiZjj[nCands]/D"       );
	outTree_->Branch("etalep1"         ,&etalep1       ,"etalep1[nCands]/D"      );
	outTree_->Branch("etalep2"         ,&etalep2       ,"etalep2[nCands]/D"      );
	outTree_->Branch("etajet1"         ,&etajet1       ,"etajet1[nCands]/D"      );
	outTree_->Branch("etajet2"         ,&etajet2       ,"etajet2[nCands]/D"      );
	outTree_->Branch("philep1"         ,&philep1       ,"philep1[nCands]/D"      );
	outTree_->Branch("philep2"         ,&philep2       ,"philep2[nCands]/D"      );
	outTree_->Branch("phijet1"         ,&phijet1       ,"phijet1[nCands]/D"      );
	outTree_->Branch("phijet2"         ,&phijet2       ,"phijet2[nCands]/D"      );
	outTree_->Branch("lep"             ,&lep           ,"lep[nCands]/D"          );
	outTree_->Branch("mt"              ,&mt            ,"mt[nCands]/D"           );
	outTree_->Branch("region"          ,&reg           ,"region[nCands]/D"       );
	outTree_->Branch("nXjets"          ,&nXjets        ,"nXjets[nCands]/I"    ); 
	outTree_->Branch("mZZ"             ,&mzz           ,"mZZ[nCands]/D"          );
	outTree_->Branch("mZZNoKinFit"     ,&mzzNoKinFit   ,"mZZNoKinFit[nCands]/D"  );
	outTree_->Branch("ptmzz"           ,&ptmzz         ,"ptmzz[nCands]/D"        );
	outTree_->Branch("ptmzzNoKinFit"   ,&ptmzzNoKinFit ,"ptmzzNoKinFit[nCands]/D");
	outTree_->Branch("mLL"             ,&mll           ,"mLL[nCands]/D"          );
	outTree_->Branch("mJJ"             ,&mjj           ,"mJJ[nCands]/D"          );
	outTree_->Branch("mJJNoKinFit"     ,&mjjNoKinFit   ,"mJJNoKinFit[nCands]/D"  );
	outTree_->Branch("met"             ,&met           ,"met/D"                  );
	outTree_->Branch("metSign"         ,&metSign       ,"metSign/D"              );
	//outTree_->Branch("nBTags"          ,&btag          ,"nBTags[nCands]/D"       );
	outTree_->Branch("deltaREDBR"     ,&deltaREDBR   ,"deltaREDBR[nCands]/D"  );
	outTree_->Branch("deltaRleplep"    ,&deltaRleplep  ,"deltaRleplep[nCands]/D" );
	outTree_->Branch("deltaRjetjet"    ,&deltaRjetjet  ,"deltaRjetjet[nCands]/D" );
	outTree_->Branch("qgProduct"       ,&qgProduct     ,"qgProduct[nCands]/D"    );
	outTree_->Branch("qgjet1"          ,&qgjet1        ,"qgjet1[nCands]/D"       );
	outTree_->Branch("qgjet2"          ,&qgjet2        ,"qgjet2[nCands]/D"       );
	outTree_->Branch("betajet1"        ,&betajet1      ,"betajet1[nCands]/D"     ); 
	outTree_->Branch("betajet2"        ,&betajet2      ,"betajet2[nCands]/D"     ); 
	outTree_->Branch("puMvajet1"       ,&puMvajet1     ,"puMvajet1[nCands]/D"    ); 
	outTree_->Branch("puMvajet2"       ,&puMvajet2     ,"puMvajet2[nCands]/D"    ); 
	outTree_->Branch("prunedmass"       ,&prunedmass    ,"prunedmass[nCands]/D"    ); 
	outTree_->Branch("mdrop"       ,&mdrop     ,"mdrop[nCands]/D"    ); 
	outTree_->Branch("nsubj12"       ,&nsubj12    ,"nsubj12[nCands]/D"    ); 
	outTree_->Branch("nsubj23"       ,&nsubj23     ,"nsubj23[nCands]/D"    ); 
	outTree_->Branch("tau1"       ,&tau1     ,"tau1[nCands]/D"    ); 
	outTree_->Branch("tau2"       ,&tau2    ,"tau2[nCands]/D"    ); 
	outTree_->Branch("qjet"       ,&qjet    ,"qjet[nCands]/D"    ); 
	outTree_->Branch("isolep1"         ,&isolep1       ,"isolep1[nCands]/D"      ); 
	outTree_->Branch("isolep2"         ,&isolep2       ,"isolep2[nCands]/D"      ); 
	outTree_->Branch("isomu1mod"         ,&isomu1mod       ,"isomu1mod[nCands]/D"      ); 
	outTree_->Branch("isomu2mod"         ,&isomu2mod       ,"isomu2mod[nCands]/D"      ); 
	outTree_->Branch("isoele1trk"        ,&isoele1trk      ,"isoele1trk[nCands]/D"      ); 
	outTree_->Branch("isoele2trk"        ,&isoele2trk      ,"isoele2trk[nCands]/D"      ); 
	outTree_->Branch("isoele1calo"       ,&isoele1calo     ,"isoele1calo[nCands]/D"      ); 
	outTree_->Branch("isoele2calo"       ,&isoele2calo     ,"isoele2calo[nCands]/D"      ); 
	outTree_->Branch("eleMVAId1"       ,&eleMVAId1     ,"eleMVAId1[nCands]/D"    );
	outTree_->Branch("eleMVAId2"       ,&eleMVAId2     ,"eleMVAId2[nCands]/D"    );
	outTree_->Branch("LD"              ,&LD            ,"LD[nCands]/D"           );
	//outTree_->Branch("aplanarity"      ,&aplanarity    ,"aplanarity[nCands]/D"   );
	// outTree_->Branch("sphericity"      ,&sphericity    ,"sphericity[nCands]/D"   );
	// outTree_->Branch("centrality"      ,&centrality    ,"centrality[nCands]/D"   );
	outTree_->Branch("q1fl"            ,&q1fl          ,"q1fl[nCands]/I"         );
	outTree_->Branch("q2fl"            ,&q2fl          ,"q2fl[nCands]/I"         );
	//outTree_->Branch("jjfl"            ,&jjfl          ,"jjfl[nCands]/I"         );
	outTree_->Branch("MCmatch"         ,&MCmatch       ,"MCmatch[nCands]/D"      );
	outTree_->Branch("nVtx"            ,&nvtx          ,"nVtx/i"                 );
	outTree_->Branch("nJets"           ,&njets         ,"nJets/i"                );
	outTree_->Branch("nPU"             ,&npu           ,"nPU/i"                  );
	outTree_->Branch("HLTweight"       ,&HLTSF         ,"HLTweight/D"            ); 
	outTree_->Branch("PUweight"        ,&PU            ,"PUweight/D"             ); 
	outTree_->Branch("PUweight2012A"   ,&PUA           ,"PUweight2012A/D"        ); 
	outTree_->Branch("PUweight2012B"   ,&PUB           ,"PUweight2012B/D"        );
	outTree_->Branch("LumiWeight"      ,&lumiw          ,"LumiWeight/D"           );  // For correct lumi scaling
	outTree_->Branch("GenWeight"       ,&genw          ,"GenWeight/D"            );  // Gen level MC weights
	outTree_->Branch("weight"          ,&w             ,"weight/D"               );  // Product of PU and lumi weights
	outTree_->Branch("weight2012A"     ,&wA            ,"weight2012A/D"          );
	outTree_->Branch("weight2012B"     ,&wB            ,"weight2012B/D"          );
	outTree_->Branch("VBFTag"          ,&VBFTag     ,"VBFTag[nCands]/I"    ); 
	outTree_->Branch("VBFmJJ"          ,&VBFmJJ     ,"VBFmJJ[nCands]/D"    ); 
	outTree_->Branch("VBFdeltaEta"     ,&VBFdeltaEta     ,"VBFdeltaEta[nCands]/D"    ); 
	outTree_->Branch("VBFptjet1"       ,&VBFptjet1     ,"VBFptjet1[nCands]/D"    ); 
	outTree_->Branch("VBFptjet2"       ,&VBFptjet2     ,"VBFptjet2[nCands]/D"    ); 
	outTree_->Branch("VBFetajet1"       ,&VBFetajet1     ,"VBFetajet1[nCands]/D"    ); 
	outTree_->Branch("VBFetajet2"       ,&VBFetajet2     ,"VBFetajet2[nCands]/D"    ); 
	outTree_->Branch("VBFphijet1"       ,&VBFphijet1     ,"VBFphijet1[nCands]/D"    ); 
	outTree_->Branch("VBFphijet2"       ,&VBFphijet2     ,"VBFphijet2[nCands]/D"    ); 
	outTree_->Branch("massGenX"        ,&massGenX      ,"massGenX/d"             );
	outTree_->Branch("ptGenX"          ,&ptGenX        ,"ptGenX/d"               );
	outTree_->Branch("yGenX"           ,&yGenX         ,"yGenX/d"                );
	outTree_->Branch("phiGenX"         ,&phiGenX       ,"phiGenX/d"              );
	outTree_->Branch("pdgIdGenX"       ,&pdgIdGenX     ,"pdgIdGenX/I"            );
	outTree_->Branch("massGenVll"      ,&massGenZll    ,"massGenVll/d"           );
	outTree_->Branch("ptGenVll"        ,&ptGenZll      ,"ptGenVll/d"             );
	outTree_->Branch("yGenVll"         ,&yGenZll       ,"yGenVll/d"              );
	outTree_->Branch("phiGenVll"       ,&phiGenZll     ,"phiGenVll/d"            );
	outTree_->Branch("massGenVqq"      ,&massGenZqq    ,"massGenVqq/d"           );
	outTree_->Branch("ptGenVqq"        ,&ptGenZqq      ,"ptGenVqq/d"             );
	outTree_->Branch("yGenVqq"         ,&yGenZqq       ,"yGenVqq/d"              );
	outTree_->Branch("phiGenVqq"       ,&phiGenZqq     ,"phiGenVqq/d"            );
	outTree_->Branch("nLooseMu"        ,&nLooseMu      ,"nLooseMu/I"             );
	outTree_->Branch("nLooseEle"       ,&nLooseEle     ,"nLooseEle/I"            );
	outTree_->Branch("nbtagsL"          ,&nbtagsL        ,"nbtagsL[nCands]/D"       );
	outTree_->Branch("nbtagsM"          ,&nbtagsM        ,"nbtagsM[nCands]/D"       );
	outTree_->Branch("nbtagsT"          ,&nbtagsT        ,"nbtagsT[nCands]/D"       );
	outTree_->Branch("nbtagscleanL"     ,&nbtagscleanL   ,"nbtagscleanL[nCands]/D"  );
	outTree_->Branch("nbtagscleanM"     ,&nbtagscleanM   ,"nbtagscleanM[nCands]/D"  );
	outTree_->Branch("nbtagscleanT"     ,&nbtagscleanT   ,"nbtagscleanT[nCands]/D"  );
	
	outTree_->Branch("Ngen"            ,&Ngen_         ,"Ngen/I"                 );
	outTree_->Branch("xsec"            ,&xsec_         ,"xsec/D"                 );

	if(triggerNames_.size()>0){
		if(debug_)cout<<"Adding branches with trigger names"<<endl;
		//flags for telling if the event passed a certain trig path
		char triggerNamePiuI[200];
		for(unsigned int iTrig=0;iTrig<triggerNames_.size();iTrig++) {
			sprintf(triggerNamePiuI,"%s/I",(triggerNames_.at(iTrig)).c_str());
			outTree_->Branch((triggerNames_.at(iTrig)).c_str(),        &triggerRes[iTrig],       triggerNamePiuI);
		}

	}

}//end initTree()

void AnalyzerEDBR::initDataMembers(){

	lumiw  =   1.0;
	PU    =   1.0;
	PUA   =   1.0;
	PUB   =   11.0;
	genw  =   1.0;
	w     =   0.0;
	wA    =   1.0;
	wB    =   1.0;
	HLTSF =   1.0;
	nCands=0;
	nLooseEle=-1;
	nLooseMu=-1;

	met=0; metSign=0;            // MET and its significance
	//reset arrays
	for(int i =0 ; i < nMaxCand ; i++){

		hs[i]=-99.; h1[i]=-99.; h2[i]=-99.; phi[i]=-99.; phiS1[i]=-99.; LD[i]=-99.; 
		mzz[i]=-99.; mzzNoKinFit[i]=-99.; mll[i]=-99.; mjj[i]=-99.; mjjNoKinFit[i]=-99.;           
		ptmzz[i]=-99.; ptmzzNoKinFit[i]=-99.;
		ptlep1[i]=-99.; ptlep2[i]=-99.; etalep1[i]=-99.; etalep2[i]=-99.; philep1[i]=-99.; philep2[i]=-99.;  
		ptjet1[i]=-99.; ptjet2[i]=-99.; etajet1[i]=-99.; etajet2[i]=-99.; phijet1[i]=-99.; phijet2[i]=-99.;  
		deltaREDBR[i]=-99.;
		ptZll[i]=-99.; ptZjj[i]=-99.; yZll[i]=-99.; yZjj[i]=-99.; deltaRleplep[i]=-99.; deltaRjetjet[i]=-99.;
		phiZll[i]=-99.;
		phiZjj[i]=-99.;
		btag[i]=-99.; lep[i]=-99.; reg[i]=-99.;  
		qgjet1[i]=-99.; qgjet2[i]=-99.; qgProduct[i]=-99.;   
		betajet1[i]=-99.;betajet2[i]=-99.;puMvajet1[i]=-99.;puMvajet2[i]=-99.;
		isolep1[i]=-99.; isolep2[i]=-99.; eleMVAId1[i]=-99.; eleMVAId2[i]=-99.;
		MCmatch[i]=-99.;           
		qjet[i]=-99.;tau1[i]=-99.;tau2[i]=-99.;nsubj12[i]=-99.;nsubj23[i]=-99.;
		mdrop[i]=-99.;prunedmass[i]=-99.;
		VBFTag[i]=-999;
		VBFmJJ[i]=-999.0; VBFdeltaEta[i]=-999.0; VBFptjet1[i]=-999.0; VBFptjet2[i]=-999.0; VBFetajet1[i]=-999.0; VBFetajet2[i]=-999.0; VBFphijet1[i]=-999.0; VBFphijet2[i]=-999.0;
		mt[i]=-99.;
		nbtagsL[i]=-99.; nbtagscleanL[i]=-99.;
		nbtagsM[i]=-99.; nbtagscleanM[i]=-99.;
		nbtagsT[i]=-99.; nbtagscleanT[i]=-99.;
	}


	massGenX=-999.0; ptGenX=-999.0;  yGenX=-999.0; phiGenX=-999.0; 
	pdgIdGenX=-999;
	massGenZll=-999.0; ptGenZll=-999.0; yGenZll=-999.0; phiGenZll=-999.0;
	massGenZqq=-999.0; ptGenZqq=-999.0; yGenZqq=-999.0; phiGenZqq=-999.0;
	ptGenq1=-999.0; etaGenq1=-999.0;  phiGenq1=-999.0; 
	ptGenq2=-999.0; etaGenq2=-999.0;  phiGenq2 =-999.0; 
	flavGenq1=-999; flavGenq2=-999;
	ptGenl1=-999.0; etaGenl1=-999.0;  phiGenl1=-999.0; 
	ptGenl2=-999.0; etaGenl2=-999.0;  phiGenl2 =-999.0; 
	flavGenl1=-999; flavGenl2=-999;


}//end AnalyzeEDBR::initDataMembers()

void AnalyzerEDBR::analyzeTrigger(edm::Event const& iEvent, edm::EventSetup const& eventSetup){

	edm::Handle<edm::TriggerResults> HLTR;
	iEvent.getByLabel(edm::InputTag("TriggerResults","","HLT"), HLTR);

	for(unsigned int iTrig=0;iTrig<triggerNames_.size();iTrig++) {
		int triggerIndex = (int)hltConfig.triggerIndex((triggerNames_.at(iTrig)).c_str()) ;
		triggerRes[iTrig] = 0;
		// triggerIndex must be less than the size of HLTR or you get a CMSException: _M_range_check
		if (triggerIndex < (int)HLTR->size()) triggerRes[iTrig] = (int)HLTR->accept(triggerIndex);
	}

	//check which analysis paths the event passed.
	//this matters as we can ask for some specific things according to the 
	//path (for example electron specific quantitites for cands passing the eejj path)

	preselM_=iEvent.triggerResultsByName("CMG").accept("preselMuPath");
	finalM_= iEvent.triggerResultsByName("CMG").accept(cmgEDBRMu_);
	sbM_=false;//iEvent.triggerResultsByName("CMG").triggerIndex("cmgXZZMMSideband")!=iEvent.triggerResultsByName("CMG").size() &&     iEvent.triggerResultsByName("CMG").accept("cmgXZZMMSideband");
	preselE_=iEvent.triggerResultsByName("CMG").accept("preselElePath");
	finalE_= iEvent.triggerResultsByName("CMG").accept(cmgEDBREle_);
	sbE_=false;//iEvent.triggerResultsByName("CMG").triggerIndex("cmgXZZEESideband")!=iEvent.triggerResultsByName("CMG").size() &&  iEvent.triggerResultsByName("CMG").accept("cmgXZZEESideband");

	preselM1J_=iEvent.triggerResultsByName("CMG").accept("preselMuMergedPath");
	finalM1J_=false;// iEvent.triggerResultsByName("CMG").accept("cmgXZZMMJ");
	sbM1J_=false;//iEvent.triggerResultsByName("CMG").triggerIndex("cmgXZZMMJSideband")!=iEvent.triggerResultsByName("CMG").size() &&     iEvent.triggerResultsByName("CMG").accept("cmgXZZMMJSideband");
	preselE1J_=iEvent.triggerResultsByName("CMG").accept("preselEleMergedPath");
	finalE1J_=false;// iEvent.triggerResultsByName("CMG").accept("cmgXZZEEJ");
	sbE1J_=false;//iEvent.triggerResultsByName("CMG").triggerIndex("cmgXZZEEJSideband")!=iEvent.triggerResultsByName("CMG").size() &&  iEvent.triggerResultsByName("CMG").accept("cmgXZZEEJSideband");



	muPath_=false;
	elePath_=false;
	anyPath_=false;
	singleJetPath_=false;
	doubleJetPath_=false;
	if(preselM_ || finalM_ || sbM_||preselM1J_ || finalM1J_ || sbM1J_)muPath_=true;
	if(preselE_ || finalE_ || sbE_||preselE1J_ || finalE1J_ || sbE1J_)elePath_=true;
	if(preselE1J_ || finalE1J_ || sbE1J_|| preselM1J_ || finalM1J_ || sbM1J_ )singleJetPath_=true;
	if(preselE_ || finalE_ || sbE_|| preselM_ || finalM_ || sbM_ )doubleJetPath_=true;
	if(muPath_ || elePath_) anyPath_=true;


	//number of loose muons/electrons
	edm::Handle<std::vector<cmg::Electron> > EleHandle;
	iEvent.getByLabel("electronPreselLoose",EleHandle);
	nLooseEle = EleHandle -> size();	

	edm::Handle<std::vector<cmg::Muon> > MuHandle;
	iEvent.getByLabel("muonPreselLoose",MuHandle);	
	nLooseMu = MuHandle->size();

}//end  AnalyzeEDBR::analyzeTrigger()


void AnalyzerEDBR::analyzeGenLevel(edm::Event const& iEvent, edm::EventSetup const& eventSetup){

	//choose what to do according to user input
	bool searchX= (fillGen_>=4);  /// || fillGen_==3 || fillGen_==4);
	bool searchZll= (fillGen_==1 || fillGen_==3 || fillGen_==6 ||fillGen_==7 );
	bool searchZqq= (fillGen_==2 || fillGen_==3 || fillGen_==5 ||fillGen_==7);

	//collection of gen particles
	edm::Handle<edm::View< reco::GenParticle > > genPColl;
	iEvent.getByLabel( "genParticles"       , genPColl  );

	bool foundX=false, foundZll=false,foundZqq=false;
	//  bool foundq1=false,foundq2=false,foundl1=false,foundl2=false;

	unsigned int i = 0;
	for( i=0;i<genPColl->size();i++){

		if( (foundX==searchX) && (foundZll==searchZll) && (foundZqq==searchZqq) )break ; //no need to continue looping
		edm::RefToBase< reco::GenParticle > genP =genPColl->refAt(i);

		int pdgId=genP->pdgId();
		int status = genP->status();
		int ndau=genP->numberOfDaughters();
		int pdgId_1=9999;
		if(ndau>0)pdgId_1=genP->daughter(0)->pdgId();
		int pdgId_2=9999;
		if(ndau>1)pdgId_2=genP->daughter(1)->pdgId();


		if(ndau>1  && abs(pdgId_1)==VpdgId_&& abs(pdgId_2)==VpdgId_ &&status==3){//found the X->ZZ
			foundX=true;
			massGenX=genP->mass();
			ptGenX=genP->pt();
			yGenX=genP->rapidity();
			phiGenX=genP->phi();
			pdgIdGenX=pdgId;
		}

		if(abs(pdgId)==VpdgId_ &&ndau>1 && abs(pdgId_2)<9 &&status==3){//found the V->qq
			foundZqq=true;
			massGenZqq=genP->mass();
			ptGenZqq=genP->pt();
			yGenZqq=genP->rapidity();
			phiGenZqq=genP->phi();
		}

		if(abs(pdgId)==VpdgId_ &&ndau>1 && status==3){//found the V->ll

			if  (abs(pdgId_2)>=11&&abs(pdgId_2)<=14) { 
				foundZll=true;   
				massGenZll=genP->mass();
				ptGenZll=genP->pt();
				yGenZll=genP->rapidity();
				phiGenZll=genP->phi();
			}
			else if(abs(pdgId_2)==15 || abs(pdgId_2)==16){// Z->tautau or W->taunu
				foundZll=true;  
			}
		}



	}//end loop on genParticles


	if( searchX && (!foundX) ) std::cout<<"WARNING from AnalyzerEDBR::analyzeGenLevel!!! Loop on genLevel particles ended without having found X->VV "<<std::endl;
	if( searchZqq && (!foundZqq) ) std::cout<<"WARNING from AnalyzerEDBR::analyzeGenLevel!!! Loop on genLevel particles ended without having found V->qq "<<std::endl;
	if( searchZll && (!foundZll) ) std::cout<<"WARNING from AnalyzerEDBR::analyzeGenLevel!!! Loop on genLevel particles ended without having found V->leplep "<<std::endl;


}//end AnalyzerEDBR::analyzeGenLevel

double AnalyzerEDBR::deltaR(reco::LeafCandidate p1,reco::LeafCandidate p2){
	double deltaEta = fabs(p1.eta()-p2.eta());
	double deltaPhi = (p1.phi()-p2.phi());
	while (deltaPhi > 3.14) deltaPhi -= 2*3.14;
	while (deltaPhi <= -3.14) deltaPhi += 2*3.14;     
	return sqrt(deltaPhi*deltaPhi + deltaEta*deltaEta);
}
double AnalyzerEDBR::deltaR(double phi1,double eta1,double phi2,double eta2){
	double deltaEta = fabs(eta1-eta2);
	double deltaPhi = (phi1-phi2);
	while (deltaPhi > 3.14) deltaPhi -= 2*3.14;
	while (deltaPhi <= -3.14) deltaPhi += 2*3.14;     
	return sqrt(deltaPhi*deltaPhi + deltaEta*deltaEta);
}



#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(AnalyzerEDBR);
