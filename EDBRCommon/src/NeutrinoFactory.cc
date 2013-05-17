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

		outP4.push_back(neutrinoP4(p4_met,p4_lep,0) );	  
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

		outP4.push_back(neutrinoP4(p4_met,p4_lep,1) );	 
		nlep_.push_back(ilep);
		lepP4.push_back(*p4_lep); 
	}//end loop on leptons


}//setMuNeutrino


TLorentzVector cmg::NeutrinoFactory::neutrinoP4(TLorentzVector* met, TLorentzVector* lep, int lepType){//lepType 0 ele, 1 mu

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
	/*
	   double tmp1;
	   double tmp2;

	   if(B*B<4.*A*C)pz=-B/2./A;
	   else { 
	   tmp1 = (-B + sqrt(B*B-4.*A*C))/2./A;
	   tmp2 = (-B - sqrt(B*B-4.*A*C))/2./A;
	   pz = (fabs(tmp1)>=fabs(tmp2))?tmp1:tmp2;
	   }
	 */

	///////////////////////////pz for fnal
	double M_mu =  0; 
	if(lepType==1)M_mu=0.105658367;//mu
	if(lepType==0)M_mu=0.00051099891;//electron

	int type=2; // use the small abs real root

	double otherSol_ = 0.; 

	a = MW_*MW_ - M_mu*M_mu + 2.0*pxl*px + 2.0*pyl*py;
	A = 4.0*(El*El - pzl*pzl);
	B = -4.0*a*pzl;
	C = 4.0*El*El*(px*px + py*py) - a*a;

	double newPtneutrino1_=0;
	double newPtneutrino2_=0;

	double tmproot = B*B - 4.0*A*C;

	bool isComplex_;

	if (tmproot<0) {
		isComplex_= true;
		pz = - B/(2*A); // take real part of complex roots
		otherSol_ = pz;

		// recalculate the neutrino pT
		// solve quadratic eq. discriminator = 0 for pT of nu
		double pnu = metpt;
		double Delta = (MW_*MW_ - M_mu*M_mu);
		double alpha = (pxl*px/pnu + pyl*py/pnu);
		double ptmu = TMath::Sqrt( pxl*pxl + pyl*pyl);
		double ptnu = TMath::Sqrt( px*px + py*py); // old
		double AA = 4.*pzl*pzl - 4*El*El + 4*alpha*alpha;
		double BB = 4.*alpha*Delta;
		double CC = Delta*Delta;

		double tmpdisc = BB*BB - 4.0*AA*CC;
		double tmpsolpt1 = (-BB + TMath::Sqrt(tmpdisc))/(2.0*AA);
		double tmpsolpt2 = (-BB - TMath::Sqrt(tmpdisc))/(2.0*AA);

		//should rejetc negative root

		if ( tmpsolpt1 >0 ) { newPtneutrino1_ = tmpsolpt1; newPtneutrino2_ = tmpsolpt2;}
		else { newPtneutrino1_ = tmpsolpt2; newPtneutrino2_ = tmpsolpt1; }

	}
	else {
		isComplex_ = false;
		double tmpsol1 = (-B + TMath::Sqrt(tmproot))/(2.0*A);
		double tmpsol2 = (-B - TMath::Sqrt(tmproot))/(2.0*A);

		//std::cout << " Neutrino Solutions: " << tmpsol1 << ", " << tmpsol2 << std::endl;

		if (type == 0 ) {
			// two real roots, pick the one closest to pz of muon
			if (TMath::Abs(tmpsol2-pzl) < TMath::Abs(tmpsol1-pzl)) { pz = tmpsol2; otherSol_ = tmpsol1;}
			else { pz = tmpsol1; otherSol_ = tmpsol2; }
			// if pz is > 300 pick the most central root
			if ( abs(pz) > 300. ) {
				if (TMath::Abs(tmpsol1)<TMath::Abs(tmpsol2) ) { pz = tmpsol1; otherSol_ = tmpsol2; }
				else { pz = tmpsol2; otherSol_ = tmpsol1; }
			}
		}
		if (type == 1 ) {
			// two real roots, pick the one closest to pz of muon
			if (TMath::Abs(tmpsol2-pzl) < TMath::Abs(tmpsol1-pzl)) { pz = tmpsol2; otherSol_ = tmpsol1; }
			else {pz = tmpsol1; otherSol_ = tmpsol2; }
		}
		if (type == 2 ) {
			// pick the most central root.
			if (TMath::Abs(tmpsol1)<TMath::Abs(tmpsol2) ) { pz = tmpsol1; otherSol_ = tmpsol2; }
			else { pz = tmpsol2; otherSol_ = tmpsol1; }
		}
		if (type == 3 ) {
			// pick the largest value of the cosine
			TVector3 p3w, p3mu;
			p3w.SetXYZ(pxl+px, pyl+py, pzl+ tmpsol1);
			p3mu.SetXYZ(pxl, pyl, pzl );

			double sinthcm1 = 2.*(p3mu.Perp(p3w))/MW_;
			p3w.SetXYZ(pxl+px, pyl+py, pzl+ tmpsol2);
			double sinthcm2 = 2.*(p3mu.Perp(p3w))/MW_;

			double costhcm1 = TMath::Sqrt(1. - sinthcm1*sinthcm1);
			double costhcm2 = TMath::Sqrt(1. - sinthcm2*sinthcm2);

			if ( costhcm1 > costhcm2 ) { pz = tmpsol1; otherSol_ = tmpsol2; }
			else { pz = tmpsol2;otherSol_ = tmpsol1; }

		}//end of type3

	}//endl of if real root
/*
	if(isComplex_)//complex root, correct the pt
	{
		double nu_pt1=newPtneutrino1_;
		TLorentzVector tmpp1; 
		tmpp1.SetPxPyPzE(nu_pt1 * cos(metphi), nu_pt1 * sin(metphi), pz, sqrt(nu_pt1*nu_pt1 + pz*pz) );
		return tmpp1;
	}
	else
	{
		TLorentzVector outP4(px,py,pz,sqrt(px*px+py*py+pz*pz));
		return outP4;
	}
*/
	//dont correct pt neutrino	
	TLorentzVector outP4(px,py,pz,sqrt(px*px+py*py+pz*pz));
	return outP4;

}//end neutrinoP4
