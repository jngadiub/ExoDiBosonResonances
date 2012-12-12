import FWCore.ParameterSet.Config as cms


cmgEDBRSel = cms.EDFilter(
    "CmgDiMuonDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_overlap\")" )
    )
cmgEDBRSelKinFit = cms.EDFilter(
    "CmgDiMuonDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRKinFitWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_overlap\")" )
    )


cmgEDBRMergedSel = cms.EDFilter(
    "CmgDiMuonSingleJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRMergedWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_overlap\")" )
    )
