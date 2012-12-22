import FWCore.ParameterSet.Config as cms

ZjjCand = cms.EDFilter(
     "DiJetSelector",  #created
     src = cms.InputTag("cmgDiJet"),
     cut = cms.string( "getSelection(\"cuts_zjj\")" ) #created
     )

selectedZjjCandFilter = cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag('ZjjCand'),
   minNumber = cms.uint32(1)
 )

selectedDiJetKinFitCandFilter = cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag('cmgDiJetKinFit'),
   minNumber = cms.uint32(1)
 )


selectedZjjSequence = cms.Sequence(ZjjCand+selectedZjjCandFilter)
