import FWCore.ParameterSet.Config as cms


selectedEDBRCandFilter = cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag('cmgEDBRSel'),
   minNumber = cms.uint32(1)
 )

selectedEDBRMergedCandFilter = cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag('cmgEDBRMergedSel'),
   minNumber = cms.uint32(1)
 )

