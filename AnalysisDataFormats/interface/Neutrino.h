#ifndef _AnalysisDataFormats_ExoDiBosonResonances_Neutrino_H_
#define _AnalysisDataFormats_ExoDiBosonResonances_Neutrino_H_

#include "DataFormats/Math/interface/LorentzVector.h"

#include "CMGTools/External/interface/PileupJetIdentifier.h"
#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"



#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/Lepton.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"


#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/PatCandidates/interface/MET.h"


#include <vector>

namespace cmg {

	//forward def needed
	class Neutrino;

	/** Class use cmg::Electron or cmg:Muon to build neutrino
	 */

	class Neutrino : public AbstractPhysicsObject {
		public:

	                //dummy constructor to set tne dummyPtr_
	                //If we try to persist this pointer, the world ends.
	                Neutrino(){pat::PATPtr(NULL,0);}

			virtual ~Neutrino(){}

			//return pointer to the Lepton FIXME
			// THIAGO: added dummy implementation.
			void setnlep(int nlep_){nlep=nlep_;}
			void setleppt(double lep_pt_){lep_pt=lep_pt_;}
			void setlepeta(double lep_eta_){lep_eta=lep_eta_;}
			void setlepphi(double lep_phi_){lep_phi=lep_phi_;}
			int getnlep() const{return nlep;}
			double getleppt() const{return lep_pt;}
			double getlepeta() const{return lep_eta;}
			double getlepphi() const{return lep_phi;}
			// Dummy implementation.
			pat::PATPtr const* sourcePtr() const{return &dummyPtr_;}

			//some functions just to sync with lepton, so we dont need to change the tree code
			double relIso()   const{return 0;}
			double mvaTrigV0()const{return 0;}

			friend class NeutrinoFactory;

		private:
			int nlep;
			double lep_pt;
			double lep_eta;
			double lep_phi;
			pat::PATPtr dummyPtr_;

	};
}


#endif
