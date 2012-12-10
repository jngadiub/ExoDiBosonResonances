// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "SHarper/HEEPAnalyzer/interface/HEEPCutCodes.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

#include "TEfficiency.h"
#include "TH1F.h"

//
// class declaration
//

class HEEPElectronStudy : public edm::EDAnalyzer {
public:
    explicit HEEPElectronStudy(const edm::ParameterSet&);
    ~HEEPElectronStudy();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void endRun(edm::Run const&, edm::EventSetup const&);
    virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
    virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
    virtual void fillPassingBits(const pat::Electron&, double);
    virtual bool passedHEEPAndIsol(int, const pat::Electron&, double);
    virtual bool passedCaloIsol(const pat::Electron&, double, bool);
    virtual bool passedTrackIsol(const pat::Electron&, double, bool);

    edm::InputTag eleLabel_;
    //edm::InputTag eleRhoCorrLabel_;

    TEfficiency* h_oneLegEffElePt_barrel;
    TEfficiency* h_oneLegEffElePt_endcap;
    TEfficiency* h_oneLegEffEleEta;
    TEfficiency* h_oneLegEffNumVert;

    TEfficiency* h_twoLegEffLElePt;
    TEfficiency* h_twoLegEffLEleEta;
    TEfficiency* h_twoLegEffNumVert;
    TEfficiency* h_twoLegEffDeltaR;
    TEfficiency* h_twoLegEffZPt;

    TH1F* h_electronPt;
    TH1F* h_electronEta;
    TH1F* h_passingBits_barrel;
    TH1F* h_passingBits_endcap;
    TH1F* h_dR;
    TH1F* h_numElectrons;
    //TH1F* h_trackIsol;
    //TH1F* h_trackIsolOrig;
    //TH1F* h_caloIsol;
    //TH1F* h_caloIsolOrig;
    //TH1F* h_etaSc_barrelFail;
    //TH1F* h_etaSc_endcapFail;

    int electronsAnalyzed_barrel;
    int electronsAnalyzed_endcap;
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
HEEPElectronStudy::HEEPElectronStudy(const edm::ParameterSet& iConfig)
{
    eleLabel_ = iConfig.getParameter<edm::InputTag>("eleLabel");

    edm::Service<TFileService> fs;

    h_oneLegEffElePt_barrel    = fs->make<TEfficiency>("oneLegEffElePt_barrel", ";Electron p_{T} (GeV);Efficiency", 50, 0, 1000);
    h_oneLegEffElePt_endcap    = fs->make<TEfficiency>("oneLegEffElePt_endcap", ";Electron p_{T} (GeV);Efficiency", 50, 0, 1000);
    h_oneLegEffEleEta          = fs->make<TEfficiency>("oneLegEffEleEta", ";Electron #eta;Efficiency", 50, -2.5, 2.5);
    h_oneLegEffNumVert         = fs->make<TEfficiency>("oneLegEffNumVert", ";Number of Vertices;Efficiency", 50, -0.5, 49.5);

    h_twoLegEffLElePt          = fs->make<TEfficiency>("twoLegEffElePt", ";Electron p_{T} (GeV);Efficiency", 50, 0, 1000);
    h_twoLegEffLEleEta         = fs->make<TEfficiency>("twoLegEffEleEta", ";Electron #eta;Efficiency", 50, -2.5, 2.5);
    h_twoLegEffNumVert         = fs->make<TEfficiency>("twoLegEffNumVert", ";Number of Vertices;Efficiency", 50, -0.5, 49.5);
    h_twoLegEffDeltaR          = fs->make<TEfficiency>("twoLegEffDeltaR", ";#DeltaR;Efficiency", 60, 0, 1.5);
    h_twoLegEffZPt             = fs->make<TEfficiency>("twoLegEffZPt", ";Dielectron p_{T} (GeV);Efficiency", 50, 0, 1000);

    h_electronPt    = fs->make<TH1F>("electronPt", ";Electron p_{T} (GeV);", 50, 0, 1000);
    h_electronEta   = fs->make<TH1F>("electronEta", ";Electron #ta;", 50, -2.5, 2.5);
    h_dR            = fs->make<TH1F>("dR", ";DeltaR;", 150, 0, 1.5);
    h_numElectrons  = fs->make<TH1F>("numElectrons", ";numElectrons;", 10, -0.5, 9.5);

    h_passingBits_barrel    = fs->make<TH1F>("passingBits_barrel", "", 16, 0, 16);
    h_passingBits_endcap    = fs->make<TH1F>("passingBits_endcap", "", 16, 0, 16);

    h_passingBits_barrel->SetBit(TH1::kCanRebin);
    h_passingBits_barrel->Sumw2();
    electronsAnalyzed_barrel = 0;
    h_passingBits_endcap->SetBit(TH1::kCanRebin);
    h_passingBits_endcap->Sumw2();
    electronsAnalyzed_endcap = 0;
}


HEEPElectronStudy::~HEEPElectronStudy()
{

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
HEEPElectronStudy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    // Get the electrons
    edm::Handle<edm::View<pat::Electron> > eleHandle;
    iEvent.getByLabel(eleLabel_, eleHandle);
    const edm::View<pat::Electron>& eles = *(eleHandle.product());

    // Get the gen electrons
    edm::Handle<edm::View<reco::Candidate> > genEleHandle;
    iEvent.getByLabel("genElectrons", genEleHandle);
    const edm::View<reco::Candidate>& genEles = *(genEleHandle.product());

    //int numElectrons = eles.size();

    // Get rho
    edm::Handle<double> rhoHandle;
    iEvent.getByLabel(edm::InputTag("kt6PFJets", "rho"), rhoHandle);
    double rho = *rhoHandle;

    // Get number of vertices
    edm::Handle<edm::View<reco::Vertex> > vertexHandle;
    iEvent.getByLabel("offlinePrimaryVertices", vertexHandle);
    const edm::View<reco::Vertex>& vertices = *(vertexHandle.product());

    // Check that electrons have matches
    int numOfMatchedElectrons = 0;
    std::vector<size_t> wellRecoEles;

    for (size_t eleNr = 0; eleNr != eles.size(); ++eleNr) {

        const pat::Electron& ele = eles[eleNr];

        // Let's see if this guy has a match
        double minDeltaR = 10000;
        size_t matchedEle = 9999;
        for (size_t genEleNr = 0; genEleNr != genEles.size(); ++ genEleNr) {
            const reco::Candidate& genEle = genEles[genEleNr];
            double dR = deltaR(ele.eta(), ele.phi(), genEle.eta(), genEle.phi());
            if (dR < minDeltaR) {
                minDeltaR = dR;
                matchedEle = genEleNr;
            }
        }

        if (minDeltaR < 0.05) {
            ++numOfMatchedElectrons;
            wellRecoEles.push_back(matchedEle);
        }
    }

    h_numElectrons->Fill(numOfMatchedElectrons);

    // I want all electrons well reconstructed!
    if (numOfMatchedElectrons != 2)
        return;

    // Lets's put those electrons in the proper order
    const pat::Electron* firstElePtr = 0;
    const pat::Electron* secondElePtr = 0;
    // Artificial scope to make life easier
    {
        const pat::Electron& ele0 = eles[wellRecoEles[0]];
        const pat::Electron& ele1 = eles[wellRecoEles[1]];
        if (ele0.pt() > ele1.pt()) {
            firstElePtr = &ele0;
            secondElePtr = &ele1;
        } else {
            firstElePtr = &ele1;
            secondElePtr = &ele0;
        }
    }

    const pat::Electron& ele0 = *firstElePtr;
    const pat::Electron& ele1 = *secondElePtr;
    reco::CompositeCandidate dielectron;
    dielectron.addDaughter(ele0, "First electron");
    dielectron.addDaughter(ele1, "Second electron");
    AddFourMomenta addP4;
    addP4.set(dielectron);

    double Zpt = dielectron.pt();
    double dR = deltaR(ele0.eta(), ele0.phi(), ele1.eta(), ele1.phi());
    int numVertices = vertices.size();

    h_electronPt->Fill(ele0.pt());
    h_electronPt->Fill(ele1.pt());
    h_electronEta->Fill(ele0.eta());
    h_electronEta->Fill(ele1.eta());

    h_dR->Fill(dR);

    int ele0CutCode = ele1.userInt("HEEPId");
    int ele1CutCode = ele0.userInt("HEEPId");

    bool ele0Passed = passedHEEPAndIsol(ele0CutCode, ele0, rho);
    bool ele1Passed = passedHEEPAndIsol(ele1CutCode, ele1, rho);

    bool bothElesPassed = (ele0Passed and ele1Passed);

    h_twoLegEffLElePt->Fill(bothElesPassed, ele0.pt());
    h_twoLegEffLEleEta->Fill(bothElesPassed, ele0.eta());
    h_twoLegEffNumVert->Fill(bothElesPassed, numVertices);
    h_twoLegEffDeltaR->Fill(bothElesPassed, dR);
    h_twoLegEffZPt->Fill(bothElesPassed, Zpt);


    // Check all electrons
    for (size_t eleNr = 0; eleNr != wellRecoEles.size(); ++eleNr) {

        size_t matchedElectron = wellRecoEles[eleNr];
        const pat::Electron& ele = eles[matchedElectron];

        double et = ele.et();
        double eta = ele.eta();
        bool inBarrel = ele.isEB();
        bool inEndcap = ele.isEE();

        // Get the special isolations
        //double sumTrackPt = ele.userIso(0);
        //double ECALIsol = ele.userIso(1);
        //double HCALIsol = ele.userIso(2);
        //double sumCaloEt = ECALIsol + HCALIsol;

        // Get the HEEP bit, remember that it has NO isolation cuts.
        int eleCutCode = ele.userInt("HEEPId");
        //bool passedHEEPNoIso = (eleCutCode == 0x0);

        // Let 's make sure that th the HEEPId bit has indeed
        // been defined with no isolation cuts...
        assert((eleCutCode & heep::CutCodes::ISOLEMHADDEPTH1) == 0x0);
        assert((eleCutCode & heep::CutCodes::ISOLPTTRKS) == 0x0);

        fillPassingBits(ele, rho);
        bool passed = passedHEEPAndIsol(eleCutCode, ele, rho);

        h_oneLegEffEleEta->Fill(passed, eta);
        h_oneLegEffNumVert->Fill(passed, numVertices);

        if (inBarrel) {
            ++electronsAnalyzed_barrel;
            h_oneLegEffElePt_barrel->Fill(passed, et);
        } // Close in barrel
        if (inEndcap) {
            ++electronsAnalyzed_endcap;
            h_oneLegEffElePt_endcap->Fill(passed, et);
        } // Close in endcap

    } // Close loop over electrons

} // Close

void
HEEPElectronStudy::fillPassingBits(const pat::Electron& ele, double rho)
{

    bool inBarrel = ele.isEB();
    bool inEndcap = ele.isEE();

    int eleCutCode = ele.userInt("HEEPId");

    //BARREL
    if (inBarrel) {
        h_passingBits_barrel->Fill("A_NORM", 1);

        if ((eleCutCode & heep::CutCodes::ET) == 0x0)
            h_passingBits_barrel->Fill("ET", 1);
        if ((eleCutCode & heep::CutCodes::DETETA) == 0x0)
            h_passingBits_barrel->Fill("DETETA", 1);
        if ((eleCutCode & heep::CutCodes::ECALDRIVEN) == 0x0)
            h_passingBits_barrel->Fill("ECALDRIVEN", 1);
        if ((eleCutCode & heep::CutCodes::DETAIN) == 0x0)
            h_passingBits_barrel->Fill("DETAIN", 1);
        if ((eleCutCode & heep::CutCodes::DPHIIN) == 0x0)
            h_passingBits_barrel->Fill("DPHIIN", 1);
        if ((eleCutCode & heep::CutCodes::HADEM) == 0x0)
            h_passingBits_barrel->Fill("HADEM", 1);
        //if((eleCutCode&heep::CutCodes::SIGMAIETAIETA)==0x0)
        //h_passingBits_barrel->Fill("SIGMAIETAIETA",1);
        if ((eleCutCode & heep::CutCodes::E2X5OVER5X5) == 0x0)
            h_passingBits_barrel->Fill("E2X5OVER5X5", 1);

        bool isolCalo      = passedCaloIsol(ele, rho, false);
        bool isolTrack     = passedTrackIsol(ele, rho, false);
        bool origIsolCalo  = passedCaloIsol(ele, rho, true);
        bool origIsolTrack = passedTrackIsol(ele, rho, true);
        bool passed = ((eleCutCode == 0x0) and isolCalo and isolTrack);

        if (isolCalo)
            h_passingBits_barrel->Fill("ISOLEMHADDEPTH1", 1);
        if (origIsolCalo)
            h_passingBits_barrel->Fill("ISOLEMHADDEPTH1_ORIG", 1);

        if (isolTrack)
            h_passingBits_barrel->Fill("ISOLPTTRKS", 1);
        if (origIsolTrack)
            h_passingBits_barrel->Fill("ISOLPTTRKS_ORIG", 1);

        if ((eleCutCode & heep::CutCodes::NRMISSHITS) == 0x0)
            h_passingBits_barrel->Fill("NRMISSHITS", 1);
        if ((eleCutCode & heep::CutCodes::DXY) == 0x0)
            h_passingBits_barrel->Fill("DXY", 1);

        if (passed)
            h_passingBits_barrel->Fill("Z_TOTAL", 1);
    }

    // ENDCAP
    if (inEndcap) {

        h_passingBits_endcap->Fill("A_NORM", 1);

        if ((eleCutCode & heep::CutCodes::ET) == 0x0)
            h_passingBits_endcap->Fill("ET", 1);
        if ((eleCutCode & heep::CutCodes::DETETA) == 0x0)
            h_passingBits_endcap->Fill("DETETA", 1);
        if ((eleCutCode & heep::CutCodes::ECALDRIVEN) == 0x0)
            h_passingBits_endcap->Fill("ECALDRIVEN", 1);
        if ((eleCutCode & heep::CutCodes::DETAIN) == 0x0)
            h_passingBits_endcap->Fill("DETAIN", 1);
        if ((eleCutCode & heep::CutCodes::DPHIIN) == 0x0)
            h_passingBits_endcap->Fill("DPHIIN", 1);
        if ((eleCutCode & heep::CutCodes::HADEM) == 0x0)
            h_passingBits_endcap->Fill("HADEM", 1);
        if ((eleCutCode & heep::CutCodes::SIGMAIETAIETA) == 0x0)
            h_passingBits_endcap->Fill("SIGMAIETAIETA", 1);
        //if((eleCutCode&heep::CutCodes::E2X5OVER5X5)==0x0)
        //h_passingBits_endcap->Fill("E2X5OVER5X5",1);

        bool isolCalo      = passedCaloIsol(ele, rho, false);
        bool isolTrack     = passedTrackIsol(ele, rho, false);
        bool origIsolCalo  = passedCaloIsol(ele, rho, true);
        bool origIsolTrack = passedTrackIsol(ele, rho, true);
        bool passed = ((eleCutCode == 0x0) and isolCalo and isolTrack);

        if (isolCalo)
            h_passingBits_endcap->Fill("ISOLEMHADDEPTH1", 1);
        if (origIsolCalo)
            h_passingBits_endcap->Fill("ISOLEMHADDEPTH1_ORIG", 1);

        if (isolTrack)
            h_passingBits_endcap->Fill("ISOLPTTRKS", 1);
        if (origIsolTrack)
            h_passingBits_endcap->Fill("ISOLPTTRKS_ORIG", 1);

        if ((eleCutCode & heep::CutCodes::NRMISSHITS) == 0x0)
            h_passingBits_endcap->Fill("NRMISSHITS", 1);
        if ((eleCutCode & heep::CutCodes::DXY) == 0x0)
            h_passingBits_endcap->Fill("DXY", 1);

        if (passed)
            h_passingBits_endcap->Fill("Z_TOTAL", 1);
    }


}

bool
HEEPElectronStudy::passedCaloIsol(const pat::Electron& ele, double rho, bool orig)
{

    double et = ele.et();
    bool inBarrel = ele.isEB();
    bool inEndcap = ele.isEE();

    double sumCaloEt = 9999;

    if (!orig) {
        double ECALIsol = ele.userIso(1);
        double HCALIsol = ele.userIso(2);
        sumCaloEt = ECALIsol + HCALIsol;
    }
    if (orig) {
        sumCaloEt = ele.dr03EcalRecHitSumEt() + ele.dr03HcalDepth1TowerSumEt();
    }
    double sumCaloEtLimit = -1;

    if (inBarrel) {
        sumCaloEtLimit = 2 + 0.03 * et + 0.28 * rho;
    }
    if (inEndcap) {
        if (et < 50)
            sumCaloEtLimit = 2.5 + 0.28 * rho;
        else
            sumCaloEtLimit = 2.5 + 0.03 * (et - 50) + 0.28 * rho;
    }

    return (sumCaloEt < sumCaloEtLimit);
}

bool
HEEPElectronStudy::passedTrackIsol(const pat::Electron& ele, double rho, bool orig)
{

    double sumTrackPt = 9999;
    if (!orig) sumTrackPt = ele.userIso(0);
    if (orig)  sumTrackPt = ele.dr03TkSumPt();

    double sumTrackPtLimit = 5.0;
    return (sumTrackPt < sumTrackPtLimit);
}

bool
HEEPElectronStudy::passedHEEPAndIsol(int cutCode, const pat::Electron& ele, double rho)
{

    // If it doesn't pass HEEP, don't even care
    bool passedHEEP = (cutCode == 0x0);
    if (!passedHEEP)
        return false;

    bool passedCalo = passedCaloIsol(ele, rho, false);
    bool passedTrack = passedTrackIsol(ele, rho, false);

    return (passedHEEP and passedCalo and passedTrack);
}



// ------------ method called once each job just before starting event loop  ------------
void
HEEPElectronStudy::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
HEEPElectronStudy::endJob()
{
    printf("Analyzed %i electrons in barrel", electronsAnalyzed_barrel);
    printf("Analyzed %i electrons in endcap", electronsAnalyzed_endcap);
    h_passingBits_barrel->Scale(1.0 / electronsAnalyzed_barrel);
    h_passingBits_barrel->GetXaxis()->LabelsOption("a");
    h_passingBits_endcap->Scale(1.0 / electronsAnalyzed_endcap);
    h_passingBits_endcap->GetXaxis()->LabelsOption("a");
}
// ------------ method called when starting to processes a run  ------------
void
HEEPElectronStudy::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void
HEEPElectronStudy::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void
HEEPElectronStudy::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void
HEEPElectronStudy::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HEEPElectronStudy::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HEEPElectronStudy);
