import FWCore.ParameterSet.Config as cms


cmgEDBRSel = cms.EDFilter(
    "CmgDiElectronDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_fullSel_overlap\") && getSelection(\"cuts_fullSel_kinematics\")" )
   # cut = cms.string( "" )
    )
cmgEDBRSelKinFit = cms.EDFilter(
    "CmgDiElectronDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRKinFitWeighted" ),

    cut = cms.string( "!getSelection(\"cuts_fullSel_overlap\") && getSelection(\"cuts_fullSel_kinematics\")" )
#   cut = cms.string( "" )
    )

cmgEDBRMergedSel = cms.EDFilter(
    "CmgDiElectronSingleJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRMergedWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_fullSel_overlap\") && getSelection(\"cuts_fullSel_kinematics\")" )
 #   cut = cms.string( "" )
    )
