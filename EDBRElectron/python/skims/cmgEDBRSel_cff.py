import FWCore.ParameterSet.Config as cms


cmgEDBRSel = cms.EDFilter(
    "CmgDiElectronDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRWeighted" ),
   # cut = cms.string( "getSelection(\"cuts_fullSel_noOverlap\") && getSelection(\"cuts_fullSel_kinematics\")" )
    cut = cms.string( "getSelection(\"cuts_preSel\")" )
    )
cmgEDBRSelKinFit = cms.EDFilter(
    "CmgDiElectronDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRExtra" ),
#    cut = cms.string( "getSelection(\"cuts_fullSel_noOverlap\") && getSelection(\"cuts_fullSel_kinematics\")" )
   cut = cms.string( "getSelection(\"cuts_preSel\")" )  
    )

cmgEDBRMergedSel = cms.EDFilter(
    "CmgDiElectronSingleJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRMergedExtra" ),
#    cut = cms.string( "getSelection(\"cuts_fullSel_noOverlap\") && getSelection(\"cuts_fullSel_kinematics\")" )
   cut = cms.string( "getSelection(\"cuts_preSel\")" )
    )
