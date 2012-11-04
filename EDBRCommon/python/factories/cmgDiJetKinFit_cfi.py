import FWCore.ParameterSet.Config as cms

cmgDiJetKinFit = cms.EDFilter(
    "DiPFJetKinFitPOProducer",
    cfg = cms.PSet( inputs = cms.InputTag("ZjjCand"),
                    kinFit=cms.PSet( mass=cms.double(91.1876),
                                     merr=cms.double(0.0)
                                     )
                    ),
    cuts = cms.PSet(),
    verbose = cms.untracked.bool( False )
    )
