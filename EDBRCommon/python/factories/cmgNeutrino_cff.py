import FWCore.ParameterSet.Config as cms

neutrinoFactory = cms.PSet(
    isEleNeutrino = cms.bool(True),
    leptonLabel=cms.InputTag("electronPresel"),
    metLabel = cms.InputTag("patMETs"),
    massW = cms.double(80.4)
    )

from ExoDiBosonResonances.EDBRCommon.selections.neutrinoKinematics_cfi import neuKinematics

cmgEleNeutrino = cms.EDFilter(
    "NeutrinoPOProducer",
    cfg = neutrinoFactory.clone(
	),
    cuts = cms.PSet(
                   neuKine = neuKinematics.clone()
                   ),
    verbose =  cms.untracked.bool( False )
)

cmgMuNeutrino = cms.EDFilter(
    "NeutrinoPOProducer",
    cfg = neutrinoFactory.clone(
                isEleNeutrino = cms.bool(False),
                leptonLabel=cms.InputTag("muonPreselNoIso"),

    ),  
    cuts = cms.PSet(
                   neuKine = neuKinematics.clone()
                   ),  
    verbose =  cms.untracked.bool( False )

)
