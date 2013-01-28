import FWCore.ParameterSet.Config as cms


cmgEDBRSel = cms.EDFilter(
    "CmgWelenuDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_fullSel_overlap\") && getSelection(\"cuts_fullSel_kinematics\")" )
    )
cmgEDBRSelKinFit = cms.EDFilter(
    "CmgWelenuDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRKinFitWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_fullSel_overlap\") && getSelection(\"cuts_fullSel_kinematics\")" )
    )

cmgEDBRMergedSel = cms.EDFilter(
    "CmgWelenuSingleJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRMergedWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_fullSel_overlap\") && getSelection(\"cuts_fullSel_kinematics\")" )
    )
