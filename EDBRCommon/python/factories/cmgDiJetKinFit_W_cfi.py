import FWCore.ParameterSet.Config as cms

cmgDiJetKinFit = cms.EDFilter(
    "DiPFJetKinFitPOProducer",
    cfg = cms.PSet( inputs = cms.InputTag("ZjjCand"),
                    kinFit=cms.PSet( mass=cms.double(80.4),
                                     merr=cms.double(0.0)
                                     )
                    ),
    cuts = cms.PSet(),
    verbose = cms.untracked.bool( False )
    )
