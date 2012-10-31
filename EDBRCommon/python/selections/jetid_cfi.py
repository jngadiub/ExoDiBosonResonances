import FWCore.ParameterSet.Config as cms

# see for details https://twiki.cern.ch/twiki/bin/view/CMS/JetID
# why are we doing it by hand and not accessing the bit embedded in PAT-jet ?
looseJetId = cms.PSet(
    gammaFraction = cms.string('sourcePtr.get.photonEnergyFraction < 0.99'),
    h0Fraction = cms.string('sourcePtr.get.neutralHadronEnergyFraction < 0.99'),
    nConstituents = cms.string('sourcePtr.get.nConstituents > 1'),
    #these only valid for abs(eta) < 2.4
    hFraction = cms.string('sourcePtr.get.chargedHadronEnergyFraction > 0 || abs(eta) > 2.4'),
    hChargedMultiplicity = cms.string('sourcePtr.get.chargedHadronMultiplicity > 0 || abs(eta)> 2.4'),
    eFraction = cms.string('sourcePtr.get.chargedEmEnergyFraction < 0.99 || abs(eta) > 2.4'),
    muFraction = cms.string('sourcePtr.get.muonEnergyFraction < 0.99 || abs(eta) > 2.4'),
    )


## mediumJetId = looseJetId.clone
## mediumJetId.gammaFraction  = cms.string('component(4).fraction < 0.95')
## mediumJetId.h0Fraction  = cms.string('component(5).fraction < 0.95')

## tightJetId = looseJetId.clone
## tightJetId.gammaFraction  = cms.string('component(4).fraction < 0.90')
## tightJetId.h0Fraction  = cms.string('component(5).fraction < 0.90')

