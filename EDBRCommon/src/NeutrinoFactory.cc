#include "ExoDiBosonResonances/EDBRCommon/interface/NeutrinoFactory.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <iostream>

using namespace std;

cmg::NeutrinoFactory::NeutrinoFactory(const edm::ParameterSet& ps)
{

	//read in labels
	isEleNeutrino_ = ps.getParameter<bool>("isEleNeutrino");
	leptonLabel_ = ps.getParameter<edm::InputTag>("leptonLabel");
	metLabel_    = ps.getParameter<edm::InputTag>("metLabel");
	MW_          =  ps.getParameter<double>("massW");
}

cmg::NeutrinoFactory::event_ptr cmg::NeutrinoFactory::create(const edm::Event& iEvent, 
		const edm::EventSetup& iSetup){

	std::vector<TLorentzVector> p4_inv;
	std::vector<int> nlep_;
	std::vector<TLorentzVector> p4_lep;
	if(isEleNeutrino_)setEleNeutrino(iEvent,iSetup, p4_inv,nlep_,p4_lep);
	else setMuNeutrino(iEvent,iSetup, p4_inv,nlep_,p4_lep);

	const unsigned int nNeu=p4_inv.size();
	cmg::NeutrinoFactory::event_ptr result(new cmg::NeutrinoFactory::collection);
	for(unsigned int iNeu=0;iNeu<nNeu;++iNeu){
		cmg::Neutrino *neu=new Neutrino();
		math::XYZTLorentzVector neu_(p4_inv.at(iNeu).Px(),p4_inv.at(iNeu).Py(),p4_inv.at(iNeu).Pz(),p4_inv.at(iNeu).Energy());
		neu->setP4(neu_);
		neu->setnlep(nlep_.at(iNeu));
		neu->setleppt(p4_lep.at(iNeu).Pt());
		neu->setlepeta(p4_lep.at(iNeu).Eta());
		neu->setlepphi(p4_lep.at(iNeu).Phi());
		result->push_back(*neu);
	}

	return result;

}


void cmg::NeutrinoFactory::setEleNeutrino(const edm::Event& iEvent, 
		const edm::EventSetup& iSetup, std::vector<TLorentzVector> &outP4, std::vector<int> &nlep_, std::vector<TLorentzVector> &lepP4){

	edm::Handle<pat::METCollection> metHandle;
	iEvent.getByLabel(metLabel_,metHandle);

	pat::METCollection::const_iterator met=metHandle->begin();

	double metpx = met->px();
	double metpy = met->py();
	double metpz = 0.0;//unknown at this stage
	double metE  = met->energy();

	TLorentzVector *p4_met=new TLorentzVector(metpx,metpy,metpz,metE);


	edm::Handle<std::vector<cmg::Electron> > leptHandle;
	iEvent.getByLabel(leptonLabel_,leptHandle);


	for(unsigned int ilep=0; ilep < leptHandle->size(); ++ilep){
		//cmg::Electron *lep = leptHandle->refAt(ilep);
		const cmg::Electron *lep = &( leptHandle->at(ilep));
		double leppx = lep->px();
		double leppy = lep->py();
		double leppz = lep->pz();
		double lepE  = lep->energy();

		TLorentzVector *p4_lep=new TLorentzVector(leppx,leppy,leppz,lepE);

		outP4.push_back(neutrinoP4(p4_met,p4_lep) );	  
		nlep_.push_back(ilep);
		lepP4.push_back(*p4_lep);

	}//end loop on leptons


}//setEleNeutrino



void cmg::NeutrinoFactory::setMuNeutrino(const edm::Event& iEvent, 
		const edm::EventSetup& iSetup, std::vector<TLorentzVector> &outP4, std::vector<int> &nlep_, std::vector<TLorentzVector> &lepP4){



	edm::Handle<pat::METCollection> metHandle;
	iEvent.getByLabel(metLabel_,metHandle);

	pat::METCollection::const_iterator met=metHandle->begin();

	double metpx = met->px();
	double metpy = met->py();
	double metpz = 0.0;//unknown at this stage
	double metE  = met->energy();

	TLorentzVector *p4_met=new TLorentzVector(metpx,metpy,metpz,metE);


	edm::Handle<std::vector<cmg::Muon> > leptHandle;
	iEvent.getByLabel(leptonLabel_,leptHandle);


	for(unsigned int ilep=0; ilep < leptHandle->size(); ++ilep){
		//cmg::Muon *lep = leptHandle->refAt(ilep);
		const cmg::Muon *lep = &( leptHandle->at(ilep));
		double leppx = lep->px();
		double leppy = lep->py();
		double leppz = lep->pz();
		double lepE  = lep->energy();

		TLorentzVector *p4_lep=new TLorentzVector(leppx,leppy,leppz,lepE);

		outP4.push_back(neutrinoP4(p4_met,p4_lep) );	 
		nlep_.push_back(ilep);
		lepP4.push_back(*p4_lep); 
	}//end loop on leptons


}//setMuNeutrino


TLorentzVector cmg::NeutrinoFactory::neutrinoP4(TLorentzVector* met, TLorentzVector* lep){

	double leppt = lep->Pt();
	double lepphi = lep->Phi();
	double lepeta = lep->Eta();
	double lepenergy = lep->Energy();

	double metpt = met->Pt();
	double metphi = met->Phi();


	double	px = metpt*cos(metphi);
	double	py = metpt*sin(metphi);
	double  pz = 0;
	double	pxl= leppt*cos(lepphi);
	double	pyl= leppt*sin(lepphi);
	double	pzl= leppt*sinh(lepeta);
	double	El = lepenergy;
	double	a = pow(MW_,2) + pow(px+pxl,2) + pow(py+pyl,2) - px*px - py*py - El*El + pzl*pzl;
	double	b = 2.*pzl;   
	double	A = b*b -4.*El*El;
	double	B = 2.*a*b;
	double	C = a*a-4.*(px*px+py*py)*El*El;

	double tmp1;
	double tmp2;

	if(B*B<4.*A*C)pz=-B/2./A;
	else { 
		tmp1 = (-B + sqrt(B*B-4.*A*C))/2./A;
		tmp2 = (-B - sqrt(B*B-4.*A*C))/2./A;
		pz = (fabs(tmp1)>=fabs(tmp2))?tmp1:tmp2;
	}


	TLorentzVector outP4(px,py,pz,sqrt(px*px+py*py+pz*pz));

	return outP4;
}//end neutrinoP4
