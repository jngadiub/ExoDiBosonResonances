import FWCore.ParameterSet.Config as cms


cmgEDBRSelMu = cms.EDFilter(
    "CmgDiMuonDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRMuWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_overlap\")" )
    )
cmgEDBRSelKinFitMu = cms.EDFilter(
    "CmgDiMuonDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRKinFitMuWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_overlap\")" )
    )


cmgEDBRMergedSel = cms.EDFilter(
    "CmgDiMuonSingleJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRMergedWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_overlap\")" )
    )
