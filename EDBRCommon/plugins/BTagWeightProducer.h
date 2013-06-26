#ifndef ExoDiBosonResonances_EDBRCommon_BTagWeightProducer_h
#define ExoDiBosonResonances_EDBRCommon_BTagWeightProducer_h
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "ExoDiBosonResonances/EDBRCommon/interface/VJetFactory.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "TH2.h"
#include "TString.h"
#include "TFile.h"
using namespace std;

template <class restype>
class BTagWeightProducer : public edm::EDProducer {
	public:
		explicit BTagWeightProducer(const edm::ParameterSet& iConfig):
			src_(iConfig.getParameter<edm::InputTag>("src")),
			bjetSrc_(iConfig.getParameter<edm::InputTag>("BTagJets")),
			EffmapFilename_(iConfig.getParameter<edm::FileInPath>("EffmapFilename").fullPath()),
			scale_(iConfig.getParameter<double>("scale"))
	{
		//cout<<EffmapFilename_<<endl;
		produces<std::vector<restype> >();
	}
		~BTagWeightProducer(){}

	private:
		virtual void produce(edm::Event&, const edm::EventSetup&);
		void getEff(double pt, double eta, int flavor);
		void getSF(double pt, double eta, int flavor);

		edm::InputTag src_;
		edm::InputTag bjetSrc_;
		std::string EffmapFilename_;
		double scale_;

		vector<double> jetEff;
		vector<double> jetEff_e;
		vector<double> jetSF;
		vector<double> jetSF_e_up;
		vector<double> jetSF_e_down;
};

template <class restype>
void BTagWeightProducer<restype>::produce(edm::Event& iEvent, const edm::EventSetup&) {

	//std::cout<<EffmapFilename_.c_str()<<std::endl;
	edm::Handle<edm::View<restype> > edbrcandidates;
	iEvent.getByLabel(src_, edbrcandidates);

	// prepare room for output
	std::vector<restype> outEDBR;   outEDBR.reserve(edbrcandidates->size());

	for ( typename edm::View<restype>::const_iterator edbrIt = edbrcandidates->begin() ; edbrIt != edbrcandidates->end() ; ++edbrIt ) {
		restype newCand(*edbrIt);
		double weight=1.0;
		if(!iEvent.isRealData()){ // only look for MC
			if(bjetSrc_.encode().size()){//if input tag is not null
				//clean the vectors
				jetEff.clear();
				jetEff_e.clear();
				jetSF.clear();
				jetSF_e_up.clear();
				jetSF_e_down.clear();

				double nbtagsM=0.;
				double nbtagscleanM=0.;
				vector<double> btagsM;
				vector<double> btagscleanM;

				edm::Handle<std::vector<cmg::PFJet> > ak5jetCands;
				iEvent.getByLabel(bjetSrc_,ak5jetCands);
				std::vector<cmg::PFJet>::const_iterator ak5;	
				for(ak5 = ak5jetCands->begin(); ak5 != ak5jetCands->end(); ++ak5 ){
					//reading eff and SF for each jet and save into vector
					getEff(ak5->pt(),ak5->eta(),ak5->partonFlavour()) ;
					getSF(ak5->pt(),ak5->eta(),ak5->partonFlavour()) ; 

					//get btags
					double discCSV = ak5->bDiscriminator( "combinedSecondaryVertexBJetTags" );
					if(discCSV>0.679) {btagsM.push_back(1);nbtagsM++;}
					else btagsM.push_back(0);
					bool isclean = 1;
					if(deltaR(  ak5->eta(),ak5->phi(),newCand.leg2().eta(),newCand.leg2().phi()  )<0.8) isclean=0;
					if(discCSV>0.679&&isclean==1) {btagscleanM.push_back(1);nbtagscleanM++;}
					else btagscleanM.push_back(0);
				}


				/*
				   cout<<"------"<<endl;
				   cout<<ak5jetCands->size()<<endl;
				   cout<<jetEff.size()<<endl;
				   cout<<jetEff_e.size()<<endl;
				   cout<<jetSF.size()<<endl;
				   cout<<jetSF_e_up.size()<<endl;
				   cout<<jetSF_e_down.size()<<endl;
				 */

				vector<double> *btag;
				if(nbtagsM==0)btag=&btagsM;
				else if(nbtagscleanM>=1)btag=&btagscleanM;
				else btag=&btagsM;//This does not matter. These evenets will not be used.

				double PMC=1.;
				double PDATA=1.;

				for( unsigned int i=0;i<ak5jetCands->size();i++)
				{
					double temp_SF=1.;
					if(scale_>=0) temp_SF = jetSF.at(i) + jetSF_e_up.at(i) * scale_;
					if(scale_<0)  temp_SF = jetSF.at(i) - jetSF_e_down.at(i) * scale_;
					double temp_eff_mc = btag->at(i)==1? jetEff.at(i):1-jetEff.at(i);
					double temp_eff_data = btag->at(i)==1? jetEff.at(i)*temp_SF : 1-jetEff.at(i)*temp_SF ; 
					PMC=PMC*temp_eff_mc;
					PDATA=PDATA*temp_eff_data;
				}
				weight = PDATA/PMC;

			}//end if input tag is not null
		}//end if is realData

		newCand.addUserFloat("BTagWeight",weight);
		outEDBR.push_back(newCand);
	}

	std::auto_ptr<std::vector<restype> > out(new std::vector<restype>(outEDBR));
	iEvent.put(out);
}


template <class restype>
void BTagWeightProducer<restype>::getEff(double pt, double eta, int flavor){

	double eff=1.0;
	double eff_e=0.0;

	TString HistName;
	//cout<<"-------"<<endl;
	//cout<<flavor<<endl;
	flavor = abs(flavor);
	//cout<<flavor<<endl;
	if(flavor==1||flavor==2||flavor==3||flavor==21)HistName="efficiency_udsg";
	else if(flavor==4)HistName="efficiency_c";
	else if(flavor==5)HistName="efficiency_b";
	else
	{ 	//treat others all like udsg
		//cout<<"Unsupported flavor. Flavor="<<flavor<<endl;
		HistName="efficiency_udsg";
	}

	//cout<<HistName<<endl;

	TFile * file = new TFile(EffmapFilename_.c_str());
	TH2D * h2 = (TH2D *)file->Get(HistName);

	double binx = h2->GetXaxis()->FindBin(pt);
	double biny = h2->GetYaxis()->FindBin(eta);

	eff = h2->GetBinContent(binx,biny);
	eff_e = h2->GetBinError(binx,biny);
	//if(flavor==0)cout<<pt<<" "<<eta<<" "<<endl;
	//cout<<eff<<endl;
	//cout<<eff_e<<endl;

	jetEff.push_back(eff);
	jetEff_e.push_back(eff_e);

	delete file;
	delete h2;
	return ;
}

template <class restype>
void BTagWeightProducer<restype>::getSF(double pt, double eta, int flavor){
	double SF=1.0;
	double SF_e_up=0.0;
	double SF_e_down=0.0;
	const int nbins = 16;
	double ptmin[nbins] = {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600};
	double ptmax[nbins] = {30, 40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 600, 800};

	flavor=abs(flavor);
	if(flavor==4||flavor==5)
	{
		//Tagger: CSVM within 20 < pt < 800 GeV, abs(eta) < 2.4, x = pt
		if(abs(eta)>2.4)
		{
			jetSF.push_back(SF);
			jetSF_e_up.push_back(SF_e_up);
			jetSF_e_down.push_back(SF_e_down);
			return;
		}
		if(pt<20){
			SF=(0.938887+(0.00017124*20))+(-2.76366e-07*(20*20));
			SF_e_up=SF_e_down=0.0415707*2;
		}
		else if(pt>800){
			SF = (0.938887+(0.00017124*800))+(-2.76366e-07*(800*800));
			SF_e_up=SF_e_down=0.0596716*2;
		}
		else{//20 < pt < 800

			SF = (0.938887+(0.00017124*pt))+(-2.76366e-07*(pt*pt));
			double SFb_error[nbins] = {
				0.0415707,
				0.0204209,
				0.0223227,
				0.0206655,
				0.0199325,
				0.0174121,
				0.0202332,
				0.0182446,
				0.0159777,
				0.0218531,
				0.0204688,
				0.0265191,
				0.0313175,
				0.0415417,
				0.0740446,
				0.0596716 };
			for (int i=0;i<nbins;i++)
			{
				if(pt>=ptmin[i]&&pt<=ptmax[i])
				{
					SF_e_up=SF_e_down=SFb_error[i];
				}
			}
		}//end of 20 < pt < 800 GeV
		if (flavor==4) 
		{	//SFc = SFb with twice the quoted uncertainty
			SF_e_up = 2*SF_e_up;
			SF_e_down = 2*SF_e_down;
		}
	}//end of b,c

	else// light flavor
	{
		double SF_low;
		double SF_high;
		double ptmax;
		bool overmax=0;
		if( abs(eta)<0.8)
		{
			ptmax=1000;
			if(pt>ptmax){pt=ptmax;overmax=1;}
			SF_low = ((1.07541+(0.00231827*pt))+(-4.74249e-06*(pt*pt)))+(2.70862e-09*(pt*(pt*pt)));
			SF = ((0.964527+(0.00149055*pt))+(-2.78338e-06*(pt*pt)))+(1.51771e-09*(pt*(pt*pt)));
			SF_high = ((1.18638+(0.00314148*pt))+(-6.68993e-06*(pt*pt)))+(3.89288e-09*(pt*(pt*pt)));
		}
		else if(abs(eta)>0.8&&abs(eta)<1.6)
		{
			ptmax=1000;
			if(pt>ptmax){pt=ptmax;overmax=1;}
			SF_low = ((1.05613+(0.00114031*pt))+(-2.56066e-06*(pt*pt)))+(1.67792e-09*(pt*(pt*pt)));
			SF = ((0.946051+(0.000759584*pt))+(-1.52491e-06*(pt*pt)))+(9.65822e-10*(pt*(pt*pt)));
			SF_high = ((1.16624+(0.00151884*pt))+(-3.59041e-06*(pt*pt)))+(2.38681e-09*(pt*(pt*pt)));
		}
		else if( abs(eta)>1.6&&abs(eta)<2.4)
		{
			ptmax=850;
			if(pt>ptmax){pt=ptmax;overmax=1;}
			SF_low = ((1.05625+(0.000487231*pt))+(-2.22792e-06*(pt*pt)))+(1.70262e-09*(pt*(pt*pt)));
			SF = ((0.956736+(0.000280197*pt))+(-1.42739e-06*(pt*pt)))+(1.0085e-09*(pt*(pt*pt)));
			SF_high = ((1.15575+(0.000693344*pt))+(-3.02661e-06*(pt*pt)))+(2.39752e-09*(pt*(pt*pt)));
		}
		else {
			jetSF.push_back(SF);
			jetSF_e_up.push_back(SF_e_up);
			jetSF_e_down.push_back(SF_e_down);
			return;
		}
		SF_e_up = SF_high - SF;
		SF_e_down = SF - SF_low;

		if(overmax==1){SF_e_up=2*SF_e_up;SF_e_down=2*SF_e_down;}

	}//endl of light flavor

	jetSF.push_back(SF);
	jetSF_e_up.push_back(SF_e_up);
	jetSF_e_down.push_back(SF_e_down);

	return;
}


#endif
