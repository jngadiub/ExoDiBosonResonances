import FWCore.ParameterSet.Config as cms 


cmgEDBRSelMu = cms.EDFilter(
    "CmgWmunuDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRMuWeighted" ),
     cut = cms.string( "!getSelection(\"cuts_fullSel_overlap\") && getSelection(\"cuts_fullSel_kinematics\")" )
 #  cut = cms.string( "" )
  )
cmgEDBRSelKinFitMu = cms.EDFilter(
    "CmgWmunuDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRKinFitMuWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_fullSel_overlap\") && getSelection(\"cuts_fullSel_kinematics\")" )
#   cut = cms.string( "" )
    )   


cmgEDBRMergedSel = cms.EDFilter(
    "CmgWmunuSingleJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRMergedWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_fullSel_overlap\") && getSelection(\"cuts_fullSel_kinematics\")" )
#   cut = cms.string( "" )
    )
