#ifndef ExoDiBosonResonances_EDBRCommon_KineVarsAdder_h
#define ExoDiBosonResonances_EDBRCommon_KineVarsAdder_h

#include <TF1.h>
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "ExoDiBosonResonances/EDBRCommon/interface/VJetFactory.h"

template<class edbrtype>
class KineVarsAdder : public edm::EDProducer {
	public:
		explicit KineVarsAdder(const edm::ParameterSet& iConfig) :
			src_(iConfig.getParameter<edm::InputTag>("src")),
			nokinfitSrc_(iConfig.getParameter<edm::InputTag>("noKinFitSrc")),
			bjetSrc_(iConfig.getParameter<edm::InputTag>("BTagJets")),
			jetSrc_(iConfig.getParameter<edm::InputTag>("BTagCleaningTarget"))
																			  //    METSrc_(iConfig.getParameter<edm::InputTag>("METsrc"))
	{
		produces<std::vector<edbrtype> >(); // the actual filtered collection
		if( nokinfitSrc_.encode().size()){//check that the Input Tag is not empty
			isDoubleJet_=true;
		}
		else{//enpty InputTag
			isDoubleJet_=false;
		}
	}

		virtual ~KineVarsAdder() {}

		void produce(edm::Event & iEvent, const edm::EventSetup & iSetup) ;

	private:
		edm::InputTag src_ ;
		edm::InputTag nokinfitSrc_ ;

		edm::InputTag bjetSrc_ ;
		edm::InputTag jetSrc_ ;
		//  edm::InputTag METSrc_;
		//  StringCutObjectSelector<pat::MET> METcut_;
		bool isDoubleJet_;
};

template <class edbrtype>
void KineVarsAdder<edbrtype>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

	edm::Handle<edm::View<edbrtype> > edbrcandidates;
	iEvent.getByLabel(src_, edbrcandidates);

	edm::Handle<edm::View<edbrtype> > nokinfitcandidates;
	if(isDoubleJet_){

		iEvent.getByLabel(nokinfitSrc_, nokinfitcandidates);
		unsigned int ncands= edbrcandidates->size();
		unsigned int ncandsNKF= nokinfitcandidates->size();
		if(ncands!=ncandsNKF){ //these really need to be the same
			throw cms::Exception("Mismatched collection size") <<
				"Event in DoubleJet Path has mismatched collections with and without KinematicFit. #cands WITH kin-fit: "<<
				ncands<<"  #cands NO kin-fit: "<<ncandsNKF<<std::endl;

		}
	}


	// prepare room for output
	std::vector<edbrtype> outCands;   


	for (unsigned int i=0 ; i<edbrcandidates->size() ; ++i ) {
		edm::RefToBase<edbrtype>  edbrCand = edbrcandidates->refAt(i);

		edbrtype newCand(*edbrCand);


		//for b tagging: get AK5 jet and CA8 jet
		float nbtagsL=0.;
		float nbtagsM=0.;
		float nbtagsT=0.;
		float nbtagscleanL=0.;
		float nbtagscleanM=0.;
		float nbtagscleanT=0.;
		bool isclean=false;

		if(bjetSrc_.encode().size()){//if input tag is not null
			edm::Handle<std::vector<cmg::PFJet> > ak5jetCands;
			iEvent.getByLabel(bjetSrc_,ak5jetCands);
			edm::Handle<std::vector<cmg::PFJet> > ca8jetCands;
			iEvent.getByLabel(jetSrc_,ca8jetCands);


			//	pat::JetCollection::const_iterator ak5;
			//	pat::JetCollection::const_iterator ca8;
			std::vector<cmg::PFJet>::const_iterator ak5;
			std::vector<cmg::PFJet>::const_iterator ca8;

			//std::cout<<"################################"<<std::endl;
			for(ak5 = ak5jetCands->begin(); ak5 != ak5jetCands->end(); ++ak5 ){
				double discCSV = ak5->bDiscriminator( "combinedSecondaryVertexBJetTags" );

				//double discJP  = ak5->bDiscriminator( "JetProbabilityBJetTags" );
				//double   discSSVHE=ak5->bDiscriminator( "SimpleSecondaryVertexHighEffBJetTags");
				//std::cout<<discCSV<<" "<<discSSVHE<<std::endl;
				//std::cout<<discCSV<<std::endl;
				if(discCSV>0.244) nbtagsL++;// loose working point
				if(discCSV>0.679) nbtagsM++;// medium working point
				if(discCSV>0.898) nbtagsT++;// tight working point

				//check if the btagged ak5 is overlapped with ca8(mass>50)
				isclean=1;
				/*
				   for(ca8 = ca8jetCands->begin(); ca8 != ca8jetCands->end(); ++ca8){
				//std::cout<<ca8->mass()<<" "<<deltaR(  ak5->eta(),ak5->phi(),ca8->eta(),ca8->phi()  )<<std::endl;
				if(ca8->mass()>50&&deltaR(  ak5->eta(),ak5->phi(),ca8->eta(),ca8->phi()  )<0.8) { isclean=0; break; }
				}
				 */
				//sychronize with group2: clean only away from the hardronic candidate
				if(deltaR(  ak5->eta(),ak5->phi(),newCand.leg1().eta(),newCand.leg1().phi()  )<0.8) { isclean=0; break; }

				if(discCSV>0.244&&isclean==1) nbtagscleanL++;
				if(discCSV>0.679&&isclean==1) nbtagscleanM++;
				if(discCSV>0.898&&isclean==1) nbtagscleanT++;
			}
		}//end if intput ag is not empty
		//std::cout<<nbtags<<" "<<nbtagsclean<<std::endl;
		//std::cout<<"################################"<<std::endl;





		float nXJets=1.0;

		// also save the corresponding other candidate (need to read other LD, QG is the same)
		float mzzNKF=-999.0,ptNKF=-999.0,etaNKF=-999.0,phiNKF=-999.0;
		float mjjNKF=-999.0,ptjjNKF=-999.0,etajjNKF=-999.0,phijjNKF=-999.0;
		float isMJJSigReg=-99.0;


		////////////////////////////////////////////////////////////////////

		/// Electron isolation:
		//    isHEEP = cms.string('sourcePtr().userInt("HEEPId") == 0'),
		//    isIsolTrk = cms.string('sourcePtr().userIso(0) < 5.0'),
		//    isIsolCalo ~ (ele.userIso(1) + ele.userIso(2))/ele.et(); (ECAL+HCAL)/et
		// Notice that the cut value for the last one is a VERY complicated formula - check: 
		//     EDBRElectron/python/skims/selEventsElectrons_cfi.py
		// It depends on the fact that electrons may be barrel or endcap, and their energies, etc.
		// To first approximation, the cut is isolCalo < A + B*et,
		// so we plot the variable isolCalo/et.
		float trkiso1=-99.0;
		float trkiso2=-99.0;
		float caloiso1=-99.0;
		float caloiso2=-99.0;
		bool isleg1GoodEle = ( abs(newCand.leg1().leg1().pdgId())== 11 ) ; //check that lep1 is an ele

		if(isleg1GoodEle) {
			trkiso1 = (*(*newCand.leg1().leg1().sourcePtr())).userIso(0);
			caloiso1 = ( (*(*newCand.leg1().leg1().sourcePtr())).userIso(1) + 
					(*(*newCand.leg1().leg1().sourcePtr())).userIso(2) ) / 
				newCand.leg1().leg1().pt();
		}

		/// This DOESN'T WORK. So just don't use the electron
		/// isolations of the second leg. If you do, the wrath of
		/// Maurizio will be unleashed upon thee.
		/*
		   bool isleg2GoodEle = ( abs(newCand.leg1().leg2().pdgId())== 11 ) ; //check that lep2 is an ele
		   if(isleg2GoodEle) {
		   trkiso2 = (*(*newCand.leg1().leg2().sourcePtr())).userIso(0);
		   caloiso2 = ( (*(*newCand.leg1().leg2().sourcePtr())).userIso(1) + 
		   (*(*newCand.leg1().leg2().sourcePtr())).userIso(2) ) / 
		   newCand.leg1().leg2().pt();
		   }
		 */

		/// Muon isolation:
		//  Is just the tracker based isolation minus the other muon pt if it is deltaR < 0.3.
		float iso1=-99.0;
		float iso2=-99.0;
		bool isleg1GoodMuon = ( abs(newCand.leg1().leg1().pdgId())== 13 ) ; //check that lep1 is a mu
		bool isleg2GoodMuon = ( abs(newCand.leg1().leg2().pdgId())== 13 ) ; //check that lep2 is a mu

		if( isleg1GoodMuon ){

			iso1 = (*(newCand.leg1().leg1().sourcePtr()))->trackIso() / newCand.leg1().leg1().pt();   
			if(  isleg2GoodMuon ) { //correct iso1 from lep2 and iso2 from lep1
				iso2 = (*(newCand.leg1().leg2().sourcePtr()))->trackIso() / newCand.leg1().leg2().pt();   

				/// Let's correct!
				double l1eta = newCand.leg1().leg1().eta();
				double l1phi = newCand.leg1().leg1().phi();
				double l2eta = newCand.leg1().leg2().eta();
				double l2phi = newCand.leg1().leg2().phi();
				double theDR = deltaR(l1eta,l1phi,l2eta,l2phi);
				if(theDR < 0.3) {
					iso1 = (iso1 - newCand.leg1().leg2().pt()/newCand.leg1().leg1().pt());
					iso2 = (iso2 - newCand.leg1().leg1().pt()/newCand.leg1().leg2().pt());
				}
			}//end if lep2 is a good muon
		}//end if lep1 is a good muon
		/////////////////////////////////////////////////

		if(isDoubleJet_){
			nXJets=2.0; 

			edm::RefToBase<edbrtype> nokinfitCand=nokinfitcandidates->refAt(i);
			mzzNKF  = nokinfitCand->mass();
			ptNKF  = nokinfitCand->pt();
			etaNKF  = nokinfitCand->eta();
			phiNKF  = nokinfitCand->phi();
			mjjNKF  = nokinfitCand->leg2().mass();
			ptjjNKF  = nokinfitCand->leg2().pt();
			etajjNKF  = nokinfitCand->leg2().eta();
			phijjNKF  = nokinfitCand->leg2().phi();

			if(nokinfitCand->leg2().getSelection("cuts_isWSignal") || nokinfitCand->leg2().getSelection("cuts_isZSignal")){
				isMJJSigReg=1.0;
			}
			else if(nokinfitCand->leg2().getSelection("cuts_isWSideband") || nokinfitCand->leg2().getSelection("cuts_isZSideband")) isMJJSigReg=0.0;
			else {
				throw cms::Exception("Value out of range")<<"Error, NoKinFit Z->jj is neither signal or sideband region. MJJ-NoKinFit="<<mjjNKF<<"  isMJJSig="<<isMJJSigReg<<std::endl;
			}			
		}

		newCand.addUserFloat("isomu1mod",iso1 );
		newCand.addUserFloat("isomu2mod",iso2 );
		newCand.addUserFloat("isoele1trk",trkiso1 );
		newCand.addUserFloat("isoele2trk",trkiso2 );
		newCand.addUserFloat("isoele1calo",caloiso1 );
		newCand.addUserFloat("isoele2calo",caloiso2 );
		newCand.addUserFloat("nXJets",nXJets );
		newCand.addUserFloat("nokinfitMZZ",mzzNKF );
		newCand.addUserFloat("nokinfitPT",ptNKF);
		newCand.addUserFloat("nokinfitEta",etaNKF);
		newCand.addUserFloat("nokinfitPhi",phiNKF);
		newCand.addUserFloat("nokinfitMJJ",mjjNKF);
		newCand.addUserFloat("nokinfitPTJJ",ptjjNKF);
		newCand.addUserFloat("nokinfitEtaJJ",etajjNKF);
		newCand.addUserFloat("nokinfitPhiJJ",phijjNKF);
		newCand.addUserFloat("nbtagsL",nbtagsL);
		newCand.addUserFloat("nbtagsM",nbtagsM);
		newCand.addUserFloat("nbtagsT",nbtagsT);
		newCand.addUserFloat("nbtagscleanL",nbtagscleanL);
		newCand.addUserFloat("nbtagscleanM",nbtagscleanM);
		newCand.addUserFloat("nbtagscleanT",nbtagscleanT);

		//LD calculation is de-activated for the moment...
		//adding the value of the LD discriminant:
		float LD=-999.0, HelPsig=-9.0, HelPbkg=9999.0;
		newCand.addUserFloat("LD", LD );
		newCand.addUserFloat("HelPsig", HelPsig );
		newCand.addUserFloat("HelPbkg", HelPbkg );


		outCands.push_back( newCand );
	}


	std::auto_ptr<std::vector<edbrtype> > out(new std::vector<edbrtype>(outCands));

	iEvent.put(out);


}

#endif
