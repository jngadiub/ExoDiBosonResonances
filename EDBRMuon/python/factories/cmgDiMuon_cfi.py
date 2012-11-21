import FWCore.ParameterSet.Config as cms
#from ExoDiBosonResonances.EDBRMuon.factories.cmgMuon_cfi import cmgMuon

dimuonFactory = cms.PSet(
       leg1Collection = cms.InputTag("muonPresel"),
       leg2Collection = cms.InputTag("muonPresel"),
       metCollection = cms.InputTag("")
)
from ExoDiBosonResonances.EDBRMuon.selections.zmumu_cfi import zmumu

cmgDiMuon = cms.EDFilter(
    "DiMuonPOProducer",
    cfg = dimuonFactory.clone(),
    cuts = cms.PSet(
      zmumu = zmumu.clone(),
      charge = cms.PSet( charge = cms.string("leg1.charge != leg2.charge") ),#XXX
      genP = cms.PSet(genP = cms.string("leg1.sourcePtr.get.hasOverlaps(\"genLeptons\") && leg2.sourcePtr.get.hasOverlaps(\"genLeptons\")"))
      ),
    )


