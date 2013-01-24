#include "DataFormats/Common/interface/Wrapper.h"

#include "AnalysisDataFormats/ExoDiBosonResonances/interface/CompoundTypesHZZLL.h"
#include "AnalysisDataFormats/ExoDiBosonResonances/interface/VJet.h"
#include "AnalysisDataFormats/ExoDiBosonResonances/interface/Neutrino.h"

#include <vector>

namespace {
  struct EDBR_DataFormats {

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

    cmg::DiMuonSingleJet dz1jm_;
    std::vector<cmg::DiMuonSingleJet> dz1jmv;
    edm::Wrapper<cmg::DiMuonSingleJet> edz1jm;
    edm::Wrapper<std::vector<cmg::DiMuonSingleJet> > edz1jmv;

    cmg::DiElectronSingleJet dz1je_;
    std::vector<cmg::DiElectronSingleJet> dz1jev;
    edm::Wrapper<cmg::DiElectronSingleJet> edz1j;
    edm::Wrapper<std::vector<cmg::DiElectronSingleJet> > edz1jev;

    cmg::DiMuonDiJetEDBR dzh_;
    std::vector<cmg::DiMuonDiJetEDBR> dzvh;
    edm::Wrapper<cmg::DiMuonDiJetEDBR> edzh;
    edm::Wrapper<std::vector<cmg::DiMuonDiJetEDBR> > edzvh;

    cmg::DiElectronDiJetEDBR dzeh_;
    std::vector<cmg::DiElectronDiJetEDBR> dzevh;
    edm::Wrapper<cmg::DiElectronDiJetEDBR> edezh;
    edm::Wrapper<std::vector<cmg::DiElectronDiJetEDBR> > edzevh;

    cmg::DiGenParticleDiGenParticleEDBR dzggh_;
    std::vector<cmg::DiGenParticleDiGenParticleEDBR> dzggvh;
    edm::Wrapper<cmg::DiGenParticleDiGenParticleEDBR> edzggh;
    edm::Wrapper<std::vector<cmg::DiGenParticleDiGenParticleEDBR> > edzggvh;

    cmg::DiMuonSingleJetEDBR dz1jmh_;
    std::vector<cmg::DiMuonSingleJetEDBR> dz1jmvh;
    edm::Wrapper<cmg::DiMuonSingleJetEDBR> edz1jmh;
    edm::Wrapper<std::vector<cmg::DiMuonSingleJetEDBR> > edz1jmvh;

    cmg::DiElectronSingleJetEDBR dz1jeh_;
    std::vector<cmg::DiElectronSingleJetEDBR> dz1jevh;
    edm::Wrapper<cmg::DiElectronSingleJetEDBR> edz1jeh;
    edm::Wrapper<std::vector<cmg::DiElectronSingleJetEDBR> > edz1jevh;

    cmg::VJet vjet_;
    std::vector<cmg::VJet> vjetv;
    edm::Wrapper<cmg::VJet> evjet;
    edm::Wrapper<std::vector<cmg::VJet> > vevjet;


    cmg::Neutrino neutrino_;
    std::vector<cmg::Neutrino> neutrinov;
    edm::Wrapper<cmg::Neutrino> eneutrino;
    edm::Wrapper<std::vector<cmg::Neutrino> > veneutrino;


    cmg::GenParticlePtr gpptr_;

    edm::Ptr<cmg::DiObject<cmg::BaseJet,cmg::BaseJet> > vbtfptr_;
    edm::Ptr<cmg::DiObject<cmg::PFJet,cmg::PFJet> > vbtfptr2_;

    cmg::Wmunu wm_;
    std::vector<cmg::Wmunu> wmv;
    edm::Wrapper<cmg::Wmunu> ewm;
    edm::Wrapper<std::vector<cmg::Wmunu> > vewm;

    cmg::Welenu we_;
    std::vector<cmg::Welenu> wev;
    edm::Wrapper<cmg::Welenu> ewe;
    edm::Wrapper<std::vector<cmg::Welenu> > vewe;

    cmg::WmunuEDBR wmh_;
    std::vector<cmg::WmunuEDBR> wmvh;
    edm::Wrapper<cmg::WmunuEDBR> ewmv;
    edm::Wrapper<std::vector<cmg::WmunuEDBR> > vewmh;

    cmg::WelenuEDBR weh_;
    std::vector<cmg::WelenuEDBR> wevh;
    edm::Wrapper<cmg::WelenuEDBR> ewev;
    edm::Wrapper<std::vector<cmg::WelenuEDBR> > veweh;


    cmg::WelenuDiJet welejj_;
    std::vector<cmg::WelenuDiJet> welejjv;
    edm::Wrapper<cmg::WelenuDiJet> ewelejj;
    edm::Wrapper<std::vector<cmg::WelenuDiJet> > ewelejjv;
    
    cmg::WmunuDiJet wmujj_;
    std::vector<cmg::WmunuDiJet> wmujjv;
    edm::Wrapper<cmg::WmunuDiJet> ewmujj;
    edm::Wrapper<std::vector<cmg::WmunuDiJet> > ewmujjv;

    cmg::WelenuSingleJet welej_;
    std::vector<cmg::WelenuSingleJet> welejv;
    edm::Wrapper<cmg::WelenuSingleJet> ewelej;
    edm::Wrapper<std::vector<cmg::WelenuSingleJet> > ewelejv;

    cmg::WmunuSingleJet muj_;
    std::vector<cmg::WmunuSingleJet> mujv;
    edm::Wrapper<cmg::WmunuSingleJet> emuj;
    edm::Wrapper<std::vector<cmg::WmunuSingleJet> > emujv;


    cmg::WelenuDiJetEDBR welejjh_;
    std::vector<cmg::WelenuDiJetEDBR> welejjvh;
    edm::Wrapper<cmg::WelenuDiJetEDBR> ewelejjh;
    edm::Wrapper<std::vector<cmg::WelenuDiJetEDBR> > ewelejjvh;

    cmg::WmunuDiJetEDBR wmujjh_;
    std::vector<cmg::WmunuDiJetEDBR> wmujjvh;
    edm::Wrapper<cmg::WmunuDiJetEDBR> ewmujjh;
    edm::Wrapper<std::vector<cmg::WmunuDiJetEDBR> > ewmujjvh;

    cmg::WelenuSingleJetEDBR welejh_;
    std::vector<cmg::WelenuSingleJetEDBR> welejvh;
    edm::Wrapper<cmg::WelenuSingleJetEDBR> ewelejh;
    edm::Wrapper<std::vector<cmg::WelenuSingleJetEDBR> > ewelejvh;

    cmg::WmunuSingleJetEDBR mujh_;
    std::vector<cmg::WmunuSingleJetEDBR> mujvh;
    edm::Wrapper<cmg::WmunuSingleJetEDBR> emujh;
    edm::Wrapper<std::vector<cmg::WmunuSingleJetEDBR> > emujvh;

  };
  

}
