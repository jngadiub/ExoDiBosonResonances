import FWCore.ParameterSet.Config as cms


dieleFactory = cms.PSet(
       leg1Collection = cms.InputTag("electronPresel"),
       leg2Collection = cms.InputTag("electronPresel"),
       metCollection = cms.InputTag("")
)
from ExoDiBosonResonances.EDBRElectron.selections.zee_cfi import zee_kinematics
from ExoDiBosonResonances.EDBRElectron.selections.zee_cfi import zee_quality

cmgDiElectronEDBR = cms.EDFilter(
    "DiElectronPOProducer",
    cfg = dieleFactory.clone(),
    cuts = cms.PSet(
       zee_kinematics = zee_kinematics.clone(),
       #zee_quality = zee_quality.clone(),
       charge = cms.PSet( charge = cms.string("leg1.charge != leg2.charge") ),#XXX
       genP = cms.PSet(genP = cms.string("leg1.sourcePtr.get.hasOverlaps(\"genLeptons\") && leg2.sourcePtr.get.hasOverlaps(\"genLeptons\")"))
      ),
    )
