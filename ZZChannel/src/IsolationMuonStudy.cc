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
#include "DataFormats/MuonReco/interface/MuonIsolation.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
#include "CommonTools/Utils/interface/StringToEnumValue.h"
#include "TEfficiency.h"
#include "TH1F.h"

//
// class declaration
//
class IsolationMuonStudy : public edm::EDAnalyzer {
public:
    explicit IsolationMuonStudy(const edm::ParameterSet&);
    ~IsolationMuonStudy();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void endRun(edm::Run const&, edm::EventSetup const&);
    virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
    virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

    // ----------member data ---------------------------
    edm::InputTag muons_;

    hptm::MuonSelector muonSelector_;

    TH1F* h_dR;
    TH1F* h_detectorIsol;
    TH1F* h_detectorIsolCorr;
    TH1F* h_pFlowIsol;
};

IsolationMuonStudy::IsolationMuonStudy(const edm::ParameterSet& iConfig)
{
    muons_ = iConfig.getParameter<edm::InputTag>("muons");

    edm::Service<TFileService> fs;

    h_dR               = fs->make<TH1F>("dR", ";DeltaR;", 150, 0, 1.5);
    h_detectorIsol     = fs->make<TH1F>("detectorIsol", ";Detector rel. isolation;", 300, -1.5, 1.5);
    h_detectorIsolCorr = fs->make<TH1F>("detectorIsolCorr", ";Detector rel. isolation;", 300, -1.5, 1.5);
    h_pFlowIsol        = fs->make<TH1F>("pFlowIsol", ";Particle Flow isolation;", 300, -1.5, 1.5);

}


IsolationMuonStudy::~IsolationMuonStudy()
{

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
IsolationMuonStudy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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

    h_dR->Fill(dR);

    /// Check HPT muons
    std::vector<size_t> muonsPassingHPT;
    for (size_t i = 0; i != matchedMuons.size(); ++i) {
        if (muonSelector_.checkMuonID(muons[matchedMuons[i]], vertex.position(), hptm::HIGHPT_OLD)) {
            muonsPassingHPT.push_back(matchedMuons[i]);
        }
    }

    /// Check TRK muons
    std::vector<size_t> muonsPassingTRK;
    for (size_t i = 0; i != matchedMuons.size(); ++i) {
        if (muonSelector_.checkMuonID(muons[matchedMuons[i]], vertex.position() , hptm::TRACKER)) {
            muonsPassingTRK.push_back(matchedMuons[i]);
        }
    }

    bool eventHasTwoHPTMuons = (muonsPassingHPT.size() == 2);
    // We want two different muons to pass HPT and TRK, respectively
    bool eventHasOneHPTOneTRKMuon =
        ((muonsPassingHPT.size() == 1)
         and (muonsPassingTRK.size() == 1)
         and (muonsPassingHPT[0] != muonsPassingTRK[0]));

    bool eventHasTwoGoodMuons = (eventHasTwoHPTMuons
                                 or eventHasOneHPTOneTRKMuon);

    if (!eventHasTwoGoodMuons) return;

    // Check the isolation
    for (size_t i = 0; i != matchedMuons.size(); ++i) {
        const reco::Muon& recoMu = muons[matchedMuons[i]];
        const reco::Muon& otherMu = (i == 0 ? muons[matchedMuons[1]] : muons[matchedMuons[0]]);

        // Detector-based isolation
        const reco::MuonIsolation& muonIso = recoMu.isolationR03();
        double detIsol = muonIso.sumPt / recoMu.pt();
        h_detectorIsol->Fill(detIsol);

        double correctionDetIsol = dR > 0.3 ? 0 : (-otherMu.innerTrack()->pt() / recoMu.pt());
        h_detectorIsolCorr->Fill(detIsol + correctionDetIsol);

        // PF-based isolation
        const reco::MuonPFIsolation& muonPfIso = recoMu.pfIsolationR04();
        double pfIsol =
            (muonPfIso.sumChargedHadronPt +
             std::max(0.,
                      muonPfIso.sumNeutralHadronEt +
                      muonPfIso.sumPhotonEt -
                      0.5 * muonPfIso.sumPUPt)) / recoMu.pt();
        h_pFlowIsol->Fill(pfIsol);
    }

    return;
}

// ------------ method called once each job just before starting event loop  ------------
void
IsolationMuonStudy::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
IsolationMuonStudy::endJob()
{
}

// ------------ method called when starting to processes a run  ------------
void
IsolationMuonStudy::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void
IsolationMuonStudy::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void
IsolationMuonStudy::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void
IsolationMuonStudy::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
IsolationMuonStudy::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(IsolationMuonStudy);
