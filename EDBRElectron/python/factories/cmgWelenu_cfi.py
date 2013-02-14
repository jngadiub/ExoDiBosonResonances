import FWCore.ParameterSet.Config as cms 


WelenuFactory = cms.PSet(
       leg1Collection = cms.InputTag("electronPresel"),
       leg2Collection = cms.InputTag("EleNeutrinoPresel"),
	   metCollection = cms.InputTag("")
)
from ExoDiBosonResonances.EDBRElectron.selections.Welenu_cfi import Welenu_kinematics
from ExoDiBosonResonances.EDBRElectron.selections.Welenu_cfi import Welenu_quality
from ExoDiBosonResonances.EDBRElectron.selections.Welenu_cfi import Welenu_validation

cmgWelenuEDBR = cms.EDFilter(
    "WelenuPOProducer",
    cfg = WelenuFactory.clone(),
    cuts = cms.PSet(
       Welenu_kinematics = Welenu_kinematics.clone(),
       ##       Welenu_quality = Welenu_quality.clone(),
       Welenu_validation = Welenu_validation.clone(),
       genP = cms.PSet(genP = cms.string("leg1.sourcePtr.get.hasOverlaps(\"genLeptonsW\")")
       ) 
    )
)
