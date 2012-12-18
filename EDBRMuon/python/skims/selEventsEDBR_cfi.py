import FWCore.ParameterSet.Config as cms


selectedEDBRMuCandFilter = cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag('cmgEDBRSelMu'),
   minNumber = cms.uint32(0)
 )


selectedEDBRKinFitMuCandFilter = cms.EDFilter("CandViewCountFilter",
                                      src = cms.InputTag('cmgEDBRSelKinFitMu'),
                                      minNumber = cms.uint32(1)
                                      )

selectedEDBRMergedCandFilter = cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag('cmgEDBRMergedSel'),
   minNumber = cms.uint32(0)
 )

