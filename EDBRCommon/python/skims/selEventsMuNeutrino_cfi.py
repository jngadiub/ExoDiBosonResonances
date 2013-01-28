import FWCore.ParameterSet.Config as cms

MuNeutrinoPresel = cms.EDFilter(
  "CmgNeutrinoSelector",
    src = cms.InputTag("cmgMuNeutrino"),
    cut = cms.string( "getSelection(\"cuts_neuKine\") "  )
    )

selectedMuNeutrinoSequence = cms.Sequence(MuNeutrinoPresel)
