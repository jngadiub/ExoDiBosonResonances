#include "DataFormats/Common/interface/Wrapper.h"

#include "AnalysisDataFormats/HiggsAna/interface/CompoundTypesHZZLL.h"

#include <vector>

namespace {
  struct HiggsAna_DataFormats {

    cmg::DiGenParticle dgg_;
    std::vector<cmg::DiGenParticle> dggv;
    edm::Wrapper<cmg::DiGenParticle> edgg;
    edm::Wrapper<std::vector<cmg::DiGenParticle> > edggv;

    cmg::DiMuonDiJet dz_;
    std::vector<cmg::DiMuonDiJet> dzv;
    edm::Wrapper<cmg::DiMuonDiJet> edz;
    edm::Wrapper<std::vector<cmg::DiMuonDiJet> > edzv;

    cmg::DiElectronDiJet dze_;
    std::vector<cmg::DiElectronDiJet> dzev;
    edm::Wrapper<cmg::DiElectronDiJet> edez;
    edm::Wrapper<std::vector<cmg::DiElectronDiJet> > edzev;

    cmg::DiGenParticleDiGenParticle dzgg_;
    std::vector<cmg::DiGenParticleDiGenParticle> dzggv;
    edm::Wrapper<cmg::DiGenParticleDiGenParticle> edzgg;
    edm::Wrapper<std::vector<cmg::DiGenParticleDiGenParticle> > edzggv;

    cmg::DiMuonDiJetHiggs dzh_;
    std::vector<cmg::DiMuonDiJetHiggs> dzvh;
    edm::Wrapper<cmg::DiMuonDiJetHiggs> edzh;
    edm::Wrapper<std::vector<cmg::DiMuonDiJetHiggs> > edzvh;

    cmg::DiElectronDiJetHiggs dzeh_;
    std::vector<cmg::DiElectronDiJetHiggs> dzevh;
    edm::Wrapper<cmg::DiElectronDiJetHiggs> edezh;
    edm::Wrapper<std::vector<cmg::DiElectronDiJetHiggs> > edzevh;

    cmg::DiGenParticleDiGenParticleHiggs dzggh_;
    std::vector<cmg::DiGenParticleDiGenParticleHiggs> dzggvh;
    edm::Wrapper<cmg::DiGenParticleDiGenParticleHiggs> edzggh;
    edm::Wrapper<std::vector<cmg::DiGenParticleDiGenParticleHiggs> > edzggvh;

    cmg::GenParticlePtr gpptr_;

    edm::Ptr<cmg::DiObject<cmg::BaseJet,cmg::BaseJet> > vbtfptr_;
    edm::Ptr<cmg::DiObject<cmg::PFJet,cmg::PFJet> > vbtfptr2_;

  };
  

}
