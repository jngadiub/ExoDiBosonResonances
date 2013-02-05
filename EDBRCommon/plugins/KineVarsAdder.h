#ifndef ExoDiBosonResonances_EDBRCommon_KineVarsAdder_h
#define ExoDiBosonResonances_EDBRCommon_KineVarsAdder_h

#include <TF1.h>
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/deltaR.h"

template<class edbrtype>
class KineVarsAdder : public edm::EDProducer {
	public:
		explicit KineVarsAdder(const edm::ParameterSet& iConfig) :
			src_(iConfig.getParameter<edm::InputTag>("src")),
			nokinfitSrc_(iConfig.getParameter<edm::InputTag>("noKinFitSrc"))
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

		float nXJets=1.0;

		// also save the corresponding other candidate (need to read other LD, QG is the same)
		float mzzNKF=-999.0,ptNKF=-999.0,etaNKF=-999.0,phiNKF=-999.0;
		float mjjNKF=-999.0,ptjjNKF=-999.0,etajjNKF=-999.0,phijjNKF=-999.0;
		float isMJJSigReg=-99.0;


		////////////////////////////////////////////////////////////////////
	    float iso1=-99.0;;
	    float iso2=-99.0;
		bool isleg1GoodLep = ( abs(newCand.leg1().leg1().pdgId())== 11  || abs(newCand.leg1().leg1().pdgId())== 13 ) ; //check that lep1 is either an ele or a mu
		bool isleg2GoodLep = ( abs(newCand.leg1().leg2().pdgId())== 11  || abs(newCand.leg1().leg2().pdgId())== 13 ) ; //check that lep2 is either an ele or a mu

		if( isleg1GoodLep ){

			iso1 = (*(newCand.leg1().leg1().sourcePtr()))->trackIso() / newCand.leg1().leg1().pt();   
			if(  isleg2GoodLep ) { //correct iso1 from lep2 and iso2 from lep1
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
			}//end if lep2 is a good one
		}//end if lep1 is a good one
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

			if(nokinfitCand->leg2().getSelection("cuts_isSignal")){
				isMJJSigReg=1.0;
			}
			else if(nokinfitCand->leg2().getSelection("cuts_isSideband")) isMJJSigReg=0.0;
			else {
				throw cms::Exception("Value out of range")<<"Error, NoKinFit Z->jj is neither signal or sideband region. MJJ-NoKinFit="<<mjjNKF<<std::endl;
			}
		}

		newCand.addUserFloat("isolep1mod",iso1 );
		newCand.addUserFloat("isolep2mod",iso2 );
		newCand.addUserFloat("nXJets",nXJets );
		newCand.addUserFloat("nokinfitMZZ",mzzNKF );
		newCand.addUserFloat("nokinfitPT",ptNKF);
		newCand.addUserFloat("nokinfitEta",etaNKF);
		newCand.addUserFloat("nokinfitPhi",phiNKF);
		newCand.addUserFloat("nokinfitMJJ",mjjNKF);
		newCand.addUserFloat("nokinfitPTJJ",ptjjNKF);
		newCand.addUserFloat("nokinfitEtaJJ",etajjNKF);
		newCand.addUserFloat("nokinfitPhiJJ",phijjNKF);


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
