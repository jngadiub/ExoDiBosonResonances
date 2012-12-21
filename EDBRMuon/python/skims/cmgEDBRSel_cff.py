import FWCore.ParameterSet.Config as cms


cmgEDBRSelMu = cms.EDFilter(
    "CmgDiMuonDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRMuWeighted" ),
     cut = cms.string( "!getSelection(\"cuts_fullSel_overlap\")" )
 #  cut = cms.string( "" )
  )
cmgEDBRSelKinFitMu = cms.EDFilter(
    "CmgDiMuonDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRKinFitMuWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_fullSel_overlap\")" )
#   cut = cms.string( "" )
    )


cmgEDBRMergedSel = cms.EDFilter(
    "CmgDiMuonSingleJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRMergedWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_fullSel_overlap\")" )
#   cut = cms.string( "" )
    )
