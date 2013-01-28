import FWCore.ParameterSet.Config as cms

EleNeutrinoPresel = cms.EDFilter(
  "CmgNeutrinoSelector",
    src = cms.InputTag("cmgEleNeutrino"),
    cut = cms.string( "getSelection(\"cuts_neuKine\") "  )
    )

selectedEleNeutrinoSequence = cms.Sequence(EleNeutrinoPresel)
