import FWCore.ParameterSet.Config as cms


cmgEDBRSelMu = cms.EDFilter(
    "CmgDiMuonDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRMuWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_overlap\")&&getSelection(\"cuts_kinematics\")" )
    )
cmgEDBRSelKinFitMu = cms.EDFilter(
    "CmgDiMuonDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRKinFitMuWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_overlap\")&&getSelection(\"cuts_kinematics\")" )
    )


cmgEDBRMergedSel = cms.EDFilter(
    "CmgDiMuonSingleJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRMergedWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_overlap\")&&getSelection(\"cuts_kinematics\")" )
    )
