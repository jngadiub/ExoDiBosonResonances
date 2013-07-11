// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TString.h"

using namespace edm;
using namespace std;
//
// class declaration
//

class EcalLaserCorrBadListFilter : public edm::EDFilter {
	public:
		explicit EcalLaserCorrBadListFilter(const edm::ParameterSet&);
		~EcalLaserCorrBadListFilter();

	private:
		virtual void beginJob() ;
		virtual bool filter(edm::Event&, const edm::EventSetup&);
		virtual void endJob() ;

		// ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
EcalLaserCorrBadListFilter::EcalLaserCorrBadListFilter(const edm::ParameterSet& iConfig)
{
	//now do what ever initialization is needed

}


EcalLaserCorrBadListFilter::~EcalLaserCorrBadListFilter()
{

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
	bool
EcalLaserCorrBadListFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	//bad list
	const int nbad = 94;

	TString badlist[nbad]= {"194704:406:363194565", "195378:627:773034826", "196452:922:1216434952", "194825:164:185390303", "196199:458:510043964", "196199:181:217709363", "196452:789:1068316045", "196199:247:291034722", "194699:208:247687743", "195113:551:642099932", "194778:129:148048665", "191202:31:48308626", "191202:68:113342471", "191271:213:224012074", "195163:331:485149875", "193999:19:19437537", "196197:245:364010217", "194912:1462:1899406025", "196199:128:155577710", "194778:92:82205412", "195937:27:22562610", "196453:54:52627889", "195552:1676:1832362113", "194778:82:65282132", "196452:244:319840770", "196249:88:75950345", "196047:60:105825075", "196453:530:472124232", "195915:719:1022844846", "194912:683:1082838756", "195915:286:478991762", "194429:927:800368815", "194479:262:313519795", "194479:29:37261904", "195948:83:162042453", "195399:158:130399398", "196239:195:200392827", "196452:106:70590267", "191247:587:794449601", "196495:116:23077590", "190733:372:413977736", "195552:1588:1754682493", "194912:230:379565335", "194224:292:443072497", "195649:178:261564697", "196019:60:44076207", "195014:105:130743800", "191721:6:1845056", "194644:129:138990138", "194631:96:151675200", "195397:694:919827670", "195378:396:502007907", "194644:303:312214545", "195950:355:363811854", "194778:170:216080819", "193621:287:263928771", "194896:58:99651133", "194912:345:582491182", "195163:278:402579027", "191277:540:490944674", "194050:105:78231766", "195398:872:732443681", "194912:1381:1829566434", "193621:1165:948318230", "196250:172:301443456", "195552:804:1059517746", "194052:32:30254155", "194644:199:210744387", "196239:254:256879618", "196218:598:869249382", "194115:328:307625405", "191247:313:454396823", "191046:102:137793113", "196239:529:500986178", "195163:304:444642160", "194429:601:547051034", "196197:505:752267023", "194050:1049:984993637", "196453:1259:1113581054", "195398:1203:967134828", "196452:1022:1320759013", "196364:1085:1001814140", "194704:375:332892872", "194643:262:328990789", "196197:207:299577561", "193621:760:667000786", "194912:712:1120558172", "193541:268:160529451", "191830:168:230226742", "194050:1299:1191093630", "194304:8:12283861", "194712:466:329036011", "196197:306:463935159", "193621:1306:1035757438"};

	//run ls event
	Int_t run    = iEvent.eventAuxiliary().run();
	Int_t ls     = iEvent.eventAuxiliary().luminosityBlock();
	Int_t event  = iEvent.eventAuxiliary().event();
	TString eventInfo  = Form("%d:%d:%d",run,ls,event);
	//cout<<eventInfo<<endl;

	//check if it is a bad event
	for(int i=0; i<nbad; i++)
	{
		if(eventInfo==badlist[i])return false;
	}

	return true;
}

// ------------ method called once each job just before starting event loop  ------------
	void 
EcalLaserCorrBadListFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
EcalLaserCorrBadListFilter::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(EcalLaserCorrBadListFilter);
