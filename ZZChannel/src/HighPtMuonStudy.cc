// system include files
#include <memory>
#include <bitset>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "ExoDiBosonResonances/EDBRMuon/interface/HPTMMuonSelector.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"

#include "TEfficiency.h"
#include "TH1F.h"

//
// class declaration
//
class HighPtMuonStudy : public edm::EDAnalyzer {
public:
    explicit HighPtMuonStudy(const edm::ParameterSet&);
    ~HighPtMuonStudy();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
    virtual void fillPassingBits(const reco::Muon&, const hptm::MuonSelector::Point&);

    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void endRun(edm::Run const&, edm::EventSetup const&);
    virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
    virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

    // ----------member data ---------------------------
    edm::InputTag muons_;

    TEfficiency* h_twoLegEffZPtBarrel;
    TEfficiency* h_twoLegEffZPtEndcapNear;
    TEfficiency* h_twoLegEffZPtEndcapFar;

    TEfficiency* h_twoLegEffLMuPtBarrel;
    TEfficiency* h_twoLegEffLMuPtEndcapNear;
    TEfficiency* h_twoLegEffLMuPtEndcapFar;

    TEfficiency* h_twoLegEffLMuEta;
    TEfficiency* h_twoLegEffNumVert;
    TEfficiency* h_twoLegEffDeltaR;

    TEfficiency* h_oneLegEffPtBarrel;
    TEfficiency* h_oneLegEffPtEndcapNear;
    TEfficiency* h_oneLegEffPtEndcapFar;
    TEfficiency* h_oneLegEffEta;
    TEfficiency* h_oneLegEffNumVert;

    TH1F* h_dB_trackerMuon;
    TH1F* h_dZ_trackerMuon;
    TH1F* h_muonStations_trackerMuon;
    TH1F* h_pixelHits_trackerMuon;
    TH1F* h_muonHits_trackerMuon;

    TH1F* h_passingBits;

    TH1F* h_dR;
    TH1F* h_category;
    TH1F* h_muonPt;
    TH1F* h_muonEta;
    TH1F* h_deltaPtOverPt;
    TH1F* h_deltaPtOverPtTracker;

    int muonsAnalyzed;
    int trackerMuonsAnalyzed;

    /// Muon ID
    enum DimuonIDType {HPTHPT, HPTTRK};
    std::string smID;
    std::string dmID;
    hptm::MuonSelector muonSelector_;
    hptm::MuonIDType   singleMuonID_;
    DimuonIDType       dimuonID_;

};

HighPtMuonStudy::HighPtMuonStudy(const edm::ParameterSet& iConfig)
{
    muons_ = iConfig.getParameter<edm::InputTag>("muons");
    smID =  iConfig.getParameter<std::string>("singleMuonID");
    dmID =  iConfig.getParameter<std::string>("dimuonID");

    edm::Service<TFileService> fs;

    /// Bin array for endcap efficiencies
    double binLowEdgesEndcap[30] = {0.0, 20.0, 40.0, 60.0, 80.0, 100.0,
                                    120.0, 140.0, 160.0, 180.0, 200.0,
                                    220.0, 240.0, 260.0, 280.0, 300.0,
                                    320.0, 340.0, 360.0, 380.0, 400.0,
                                    450.0, 500.0, 550.0, 600.0, 650.0,
                                    700.0, 800.0, 900.0, 1000.0
                                   };

    /// Plots suggested by Francesco
    h_twoLegEffZPtBarrel     = fs->make<TEfficiency>("twoLegEffZPtBarrel", ";Z p_{T} (GeV); Efficiency", 50, 0, 1000);
    h_twoLegEffZPtEndcapNear = fs->make<TEfficiency>("twoLegEffZPtEndcapNear", ";Z p_{T} (GeV); Efficiency", 50, 0, 1000);
    h_twoLegEffZPtEndcapFar  = fs->make<TEfficiency>("twoLegEffZPtEndcapFar", ";Z p_{T} (GeV); Efficiency", 50, 0, 1000);

    h_twoLegEffLMuPtBarrel     = fs->make<TEfficiency>("twoLegEffLMuPtBarrel", ";Leading muon p_{T} (GeV); Efficiency", 50, 0, 1000);
    h_twoLegEffLMuPtEndcapNear = fs->make<TEfficiency>("twoLegEffLMuPtEndcapNear", ";Leading muon p_{T} (GeV); Efficiency", 50, 0, 1000);
    h_twoLegEffLMuPtEndcapFar  = fs->make<TEfficiency>("twoLegEffLMuPtEndcapFar", ";Leading muon p_{T} (GeV); Efficiency", 50, 0, 1000);

    h_twoLegEffLMuEta          = fs->make<TEfficiency>("twoLegEffLMuEta", ";Leading muon #eta; Efficiency", 48, -2.4, 2.4);
    h_twoLegEffNumVert         = fs->make<TEfficiency>("twoLegEffNumVert", ";Number of vertices; Efficiency", 50, -0.5, 49.5);
    h_twoLegEffDeltaR          = fs->make<TEfficiency>("twoLegEffDeltaR", ";#Delta R; Efficiency", 60, 0.0, 1.5);

    h_oneLegEffPtBarrel        = fs->make<TEfficiency>("oneLegEffPtBarrel", ";Muon p_{T}; Efficiency", 50, 0, 1000);
    h_oneLegEffPtEndcapNear    = fs->make<TEfficiency>("oneLegEffPtEndcapNear", ";Muon p_{T}; Efficiency", 29, binLowEdgesEndcap);
    h_oneLegEffPtEndcapFar     = fs->make<TEfficiency>("oneLegEffPtEndcapFar", ";Muon p_{T}; Efficiency", 29, binLowEdgesEndcap);
    h_oneLegEffEta             = fs->make<TEfficiency>("oneLegEffEta", ";Muon #eta; Efficiency", 48, -2.4, 2.4);
    h_oneLegEffNumVert         = fs->make<TEfficiency>("oneLegEffNumVert", ";Number of vertices; Efficiency", 50, -0.5, 49.5);

    h_dB_trackerMuon           = fs->make<TH1F>("dB_trackerMuon", ";dB;", 200, 0, 0.25);
    h_dZ_trackerMuon           = fs->make<TH1F>("dZ_trackerMuon", ";dZ;", 240, 0, 0.6);
    h_muonStations_trackerMuon = fs->make<TH1F>("muonStations_trackerMuon", ";Number of muon stations;", 50, -0.5, 49.5);
    h_pixelHits_trackerMuon    = fs->make<TH1F>("pixelHits_trackerMuon", ";Number of pixel hits;", 50, -0.5, 49.5);
    h_muonHits_trackerMuon     = fs->make<TH1F>("muonHits_trackerMuon", ";Number of muon hits;", 50, -0.5, 49.5);

    h_passingBits = fs->make<TH1F>("passingBits", "passingBits", 10, 0, 10);

    h_dR = fs->make<TH1F>("dR", ";DeltaR;", 150, 0, 1.5);
    h_category = fs->make<TH1F>("category", "category", 4, 0, 4);
    h_muonPt = fs->make<TH1F>("muonPt", ";Muon p_{T} (GeV);", 50, 0, 1000);
    h_muonEta = fs->make<TH1F>("muonEta", ";Muon #eta;", 48, -2.4, 2.4);
    h_deltaPtOverPt = fs->make<TH1F>("deltaPtOverPt", ";#Delta p_{T}/p_{T};", 100, -0.5, 0.5);
    h_deltaPtOverPtTracker = fs->make<TH1F>("deltaPtOverPtTracker", ";#Delta p_{T}/p_{T};", 100, -0.5, 0.5);

    h_passingBits->SetBit(TH1::kCanRebin);
    h_passingBits->Sumw2();
    h_category->SetBit(TH1::kCanRebin);
    h_category->Sumw2();
    muonsAnalyzed = 0;
    trackerMuonsAnalyzed = 0;
}


HighPtMuonStudy::~HighPtMuonStudy()
{

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
HighPtMuonStudy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

    edm::Handle<edm::View<pat::Muon> > muonHandle;
    iEvent.getByLabel(muons_, muonHandle);
    const edm::View<pat::Muon>& muons = *(muonHandle.product());

    edm::Handle<edm::View<reco::Vertex> > vertexHandle;
    iEvent.getByLabel("offlinePrimaryVertices", vertexHandle);
    const edm::View<reco::Vertex>& vertices = *(vertexHandle.product());
    const reco::Vertex& vertex = vertices[0];

    // Get the gen muons
    edm::Handle<edm::View<reco::Candidate> > genMuonHandle;
    iEvent.getByLabel("genMuons", genMuonHandle);
    const edm::View<reco::Candidate>& genMuons = *(genMuonHandle.product());
    //int numMuons = muons.size();
    //int numMatchedMuons = 0;
    //int numGoodMuons = 0;

    // Run over the muons and get the indices of RECO muons
    // which have a match to a GEN muon.
    std::vector<size_t> matchedMuons;
    std::vector<size_t> matchedGenMuons;

    for (size_t muonNr = 0; muonNr != muons.size(); ++muonNr) {

        const pat::Muon& recoMu = muons[muonNr];

        // First, let's see if this muon has a gen match
        double minDeltaR = 10000;
        size_t matchedGenMuon = 9999;
        for (size_t genMuonNr = 0; genMuonNr != genMuons.size(); ++genMuonNr) {
            const reco::Candidate& genMuon = genMuons[genMuonNr];
            double dR = deltaR(recoMu.eta(), recoMu.phi(), genMuon.eta(), genMuon.phi());
            if (dR < minDeltaR) {
                minDeltaR = dR;
                matchedGenMuon = genMuonNr;
            }
        }

        if (minDeltaR > 0.05) continue; // Don't care about non-matched muons.

        // If we got here the muon is matched
        matchedMuons.push_back(muonNr);
        matchedGenMuons.push_back(matchedGenMuon);
    }

    if (matchedMuons.size() != 2) return; // Don't care about corner cases where we managed to match different number of muons.

    muonsAnalyzed = (muonsAnalyzed + 2);

    // Make the Z candidate
    const pat::Muon& m0 = muons[matchedMuons[0]];
    const pat::Muon& m1 = muons[matchedMuons[1]];
    reco::CompositeCandidate dimuon;
    dimuon.addDaughter(m0, "First muon");
    dimuon.addDaughter(m1, "Second muon");
    AddFourMomenta addP4;
    addP4.set(dimuon);

    double ZPt = dimuon.pt();
    double Zeta = dimuon.eta();
    double lmuPt = m0.pt() > m1.pt() ? m0.pt() : m1.pt();
    double lmuEta = m0.pt() > m1.pt() ? m0.eta() : m1.eta();
    double dR = deltaR(m0.eta(), m0.phi(), m1.eta(), m1.phi());
    int numVertices = vertices.size();

    h_muonPt->Fill(m0.pt());
    h_muonPt->Fill(m1.pt());
    h_muonEta->Fill(m0.eta());
    h_muonEta->Fill(m1.eta());

    h_dR->Fill(dR);

    if (m0.isGlobalMuon())
        h_category->Fill("GLOBAL", 1);
    if (m0.isTrackerMuon())
        h_category->Fill("TRACKER", 1);
    if (m0.isStandAloneMuon())
        h_category->Fill("STANDALONE", 1);
    if (m0.isPFMuon())
        h_category->Fill("PFMUON", 1);

    if (m1.isGlobalMuon())
        h_category->Fill("GLOBAL", 1);
    if (m1.isTrackerMuon())
        h_category->Fill("TRACKER", 1);
    if (m1.isStandAloneMuon())
        h_category->Fill("STANDALONE", 1);
    if (m1.isPFMuon())
        h_category->Fill("PFMUON", 1);

    // Check to see which of the high-pt cuts the muons pass
    fillPassingBits(m0, vertex.position());
    fillPassingBits(m1, vertex.position());

    bool firstMuonPassedHPT  = muonSelector_.checkMuonID(m0, vertex.position(), hptm::HIGHPT_OLD);
    bool secondMuonPassedHPT = muonSelector_.checkMuonID(m1, vertex.position(), hptm::HIGHPT_OLD);
    bool firstMuonPassedTRK  = muonSelector_.checkMuonID(m0, vertex.position(), hptm::TRACKER);
    bool secondMuonPassedTRK = muonSelector_.checkMuonID(m1, vertex.position(), hptm::TRACKER);

    bool firstMuonPassed  = muonSelector_.checkMuonID(m0, vertex.position(), singleMuonID_);
    bool secondMuonPassed = muonSelector_.checkMuonID(m1, vertex.position(), singleMuonID_);

    bool passedHPTHPT = (firstMuonPassedHPT and secondMuonPassedHPT);
    bool passedHPTTRK = ((firstMuonPassedHPT and secondMuonPassedTRK) or (firstMuonPassedTRK and secondMuonPassedHPT));

    bool passed = false;

    if (dimuonID_ == HPTHPT)
        passed = passedHPTHPT;
    if (dimuonID_ == HPTTRK)
        passed = passedHPTTRK;

    // Fill the efficiency plots
    if (fabs(Zeta) < 1.1)
        h_twoLegEffZPtBarrel->Fill(passed, ZPt);
    if (fabs(Zeta) > 1.1 and fabs(Zeta) < 1.479)
        h_twoLegEffZPtEndcapNear->Fill(passed, ZPt);
    if (fabs(Zeta) > 1.479 and fabs(Zeta) < 2.4)
        h_twoLegEffZPtEndcapFar->Fill(passed, ZPt);

    if (fabs(lmuEta < 1.1))
        h_twoLegEffLMuPtBarrel->Fill(passed, lmuPt);
    if (fabs(lmuEta) > 1.1 and fabs(lmuEta) < 1.479)
        h_twoLegEffLMuPtEndcapNear->Fill(passed, lmuPt);
    if (fabs(lmuEta > 1.479) and fabs(lmuEta) < 2.4)
        h_twoLegEffLMuPtEndcapFar->Fill(passed, lmuPt);

    h_twoLegEffLMuEta->Fill(passed, lmuEta);
    h_twoLegEffNumVert->Fill(passed, numVertices);
    h_twoLegEffDeltaR->Fill(passed, dR);

    if (fabs(m0.eta()) < 1.1)
        h_oneLegEffPtBarrel->Fill(firstMuonPassed, m0.pt());
    if (fabs(m1.eta()) < 1.1)
        h_oneLegEffPtBarrel->Fill(secondMuonPassed, m1.pt());
    if (fabs(m0.eta()) > 1.1 and fabs(m0.eta()) < 1.479)
        h_oneLegEffPtEndcapNear->Fill(firstMuonPassed, m0.pt());
    if (fabs(m1.eta()) > 1.1 and fabs(m1.eta()) < 1.479)
        h_oneLegEffPtEndcapNear->Fill(secondMuonPassed, m1.pt());
    if (fabs(m0.eta()) > 1.479 and fabs(m0.eta()) < 2.4)
        h_oneLegEffPtEndcapFar->Fill(firstMuonPassed, m0.pt());
    if (fabs(m1.eta()) > 1.479 and fabs(m1.eta()) < 2.4)
        h_oneLegEffPtEndcapFar->Fill(secondMuonPassed, m1.pt());

    //if(passed)
    //printf("Event Passed\n");

    h_oneLegEffEta->Fill(firstMuonPassed, m0.eta());
    h_oneLegEffEta->Fill(secondMuonPassed, m1.eta());
    h_oneLegEffNumVert->Fill(firstMuonPassed, numVertices);
    h_oneLegEffNumVert->Fill(secondMuonPassed, numVertices);

    // Check the delta pt over pt for the HPT muon
    double deltaPtOverPt = 9999.9;
    if (firstMuonPassedHPT) {
        double genPt = genMuons[matchedGenMuons[0]].pt();
        deltaPtOverPt = (genPt - m0.pt()) / genPt;
    }
    if (secondMuonPassedHPT) {
        double genPt = genMuons[matchedGenMuons[1]].pt();
        deltaPtOverPt = (genPt - m1.pt()) / genPt;
    }
    h_deltaPtOverPt->Fill(deltaPtOverPt);

    // For the tracker muon which did not pass HPT -
    // which cuts does it pass?
    if ((firstMuonPassedHPT and secondMuonPassedTRK and !secondMuonPassedHPT) or
        (firstMuonPassedTRK and secondMuonPassedHPT and !firstMuonPassedHPT)) {
        // At least one passed HPT and the other didn't but it passed TRK

        std::bitset<8> trackerMuonBits;
        trackerMuonBits.reset(); // All to false

        const reco::Muon* theMuon = NULL;
        int whichMuon = -1;
        // First muon is tracker
        if (firstMuonPassedTRK and (!firstMuonPassedHPT)) {
            //printf("First muon passed TRK\n");
            trackerMuonBits = muonSelector_.muonBits(m0, vertex.position(), singleMuonID_);
            whichMuon = 0;
            theMuon = &m0;
        }
        // Second muon is tracker
        if (secondMuonPassedTRK and (!secondMuonPassedHPT)) {
            //printf("Second muon passed TRK\n");
            trackerMuonBits = muonSelector_.muonBits(m1, vertex.position(), singleMuonID_);
            whichMuon = 1;
            theMuon = &m1;
        }

        assert(theMuon != NULL);
        assert(trackerMuonBits[1] == true); // Be SURE that we're dealing with a tracker muon here.

        //printf("Okay, we are here\n");

        const reco::Muon& recoMu = *theMuon;
        //const reco::Candidate::Point& vertex = recoMu.vertex();
        const reco::TrackRef& innerTrackRef = recoMu.innerTrack();
        double dBValue = 9999.9;
        double dZValue = 9999.9;
        int numPixelHits = 9999;
        int numMuonHits = 9999;
        int numMuonStations = recoMu.numberOfMatchedStations();

        // Check the delta pt over pt for the TRK muon
        double deltaPtOverPt;
        double genPt = genMuons[matchedGenMuons[whichMuon]].pt();
        deltaPtOverPt = (genPt - recoMu.pt()) / genPt;
        h_deltaPtOverPtTracker->Fill(deltaPtOverPt);


        if (innerTrackRef.isNonnull()) {
            dBValue = fabs(innerTrackRef->dxy(vertex.position()));
            dZValue = fabs(innerTrackRef->dz(vertex.position()));
            numPixelHits = innerTrackRef->hitPattern().numberOfValidPixelHits();
            numMuonHits = innerTrackRef->hitPattern().numberOfValidMuonHits();
            //printf("Vertex is at %g, %g, %g\n", vertex.x(), vertex.y(), vertex.z());
            //printf("dBValue is %g\n",dBValue);
            //printf("dZValue is %g\n",dZValue);
        } else {
            dBValue = -1.0;
            dZValue = -1.0;
            numPixelHits = -1;
            numMuonHits = -1;
        }

        // Fill the histograms
        h_dB_trackerMuon->Fill(dBValue);
        h_dZ_trackerMuon->Fill(dZValue);
        h_muonStations_trackerMuon->Fill(numMuonStations);
        h_pixelHits_trackerMuon->Fill(numPixelHits);
        h_muonHits_trackerMuon->Fill(numMuonHits);

    } // Close if "one TRK and one HPT muon"
}

// ------------ method called once each job just before starting event loop  ------------
void
HighPtMuonStudy::fillPassingBits(const reco::Muon& recoMu, const hptm::MuonSelector::Point& vertex)
{
    std::bitset<8> muonBits;

    muonBits = muonSelector_.muonBits(recoMu, vertex, hptm::HIGHPT_OLD);

    bool isGlobal = muonBits[0];
    bool isTracker = muonBits[1];
    bool muonChamberHit  = muonBits[2];
    bool matchedStations = muonBits[3];
    bool dBCut = muonBits[4];
    bool longiCut = muonBits[5];
    bool pixelHit = muonBits[6];
    bool trackerLayers = muonBits[7];

    bool thisMuonPassed = (isGlobal and muonChamberHit and matchedStations and
                           dBCut and longiCut and pixelHit and trackerLayers);

    h_passingBits->Fill("A_NORM", 1);
    if (isGlobal)
        h_passingBits->Fill("GLOBAL", 1);
    if (isTracker)
        h_passingBits->Fill("TRACKER", 1);
    if (muonChamberHit)
        h_passingBits->Fill("MUONHITS", 1);
    if (matchedStations)
        h_passingBits->Fill("MATCHEDSTATIONS", 1);
    if (dBCut)
        h_passingBits->Fill("DBCUT", 1);
    if (longiCut)
        h_passingBits->Fill("DZCUT", 1);
    if (pixelHit)
        h_passingBits->Fill("PIXELHITS", 1);
    if (trackerLayers)
        h_passingBits->Fill("TRACKERLAYERS", 1);
    if (thisMuonPassed)
        h_passingBits->Fill("Z_HPT", 1);

}

void
HighPtMuonStudy::beginJob()
{
    if (smID.compare("HIGHPT_OLD") == 0)
        singleMuonID_ = hptm::HIGHPT_OLD;
    else if (smID.compare("TRACKER") == 0)
        singleMuonID_ = hptm::TRACKER;
    else
        throw cms::Exception("InvalidParameter")
                << "Muon ID " << smID << " is unknown\n";

    if (dmID.compare("HPTHPT") == 0)
        dimuonID_ = HPTHPT;
    else if (dmID.compare("HPTTRK") == 0)
        dimuonID_ = HPTTRK;
    else
        throw cms::Exception("InvalidParameter")
                << "Dimuon ID " << dmID << " is unknown\n";

    printf("Muon ID chosen: %s\n", smID.c_str());
    printf("Dimuon ID chosen: %s\n", dmID.c_str());

}

// ------------ method called once each job just after ending the event loop  ------------
void
HighPtMuonStudy::endJob()
{
    double total = (double)muonsAnalyzed;
    h_category->Scale(1.0 / total);
    h_passingBits->Scale(1.0 / total);

    h_category->GetXaxis()->LabelsOption("av");
    h_passingBits->GetXaxis()->LabelsOption("av");
}

// ------------ method called when starting to processes a run  ------------
void
HighPtMuonStudy::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void
HighPtMuonStudy::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void
HighPtMuonStudy::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void
HighPtMuonStudy::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HighPtMuonStudy::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HighPtMuonStudy);
