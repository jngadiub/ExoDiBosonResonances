import FWCore.ParameterSet.Config as cms 


cmgEDBRSel = cms.EDFilter(
    "CmgWelenuDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRWeighted" ),
   # cut = cms.string( "getSelection(\"cuts_fullSel_noOverlap\") && getSelection(\"cuts_fullSel_kinematics\")" )
    cut = cms.string( "getSelection(\"cuts_preSel\")" )
    )   
cmgEDBRSelKinFit = cms.EDFilter(
    "CmgWelenuDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRExtra" ),
#    cut = cms.string( "getSelection(\"cuts_fullSel_noOverlap\") && getSelection(\"cuts_fullSel_kinematics\")" )
   cut = cms.string( "getSelection(\"cuts_preSel\")" )   
    )   

cmgEDBRMergedSel = cms.EDFilter(
    "CmgWelenuSingleJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRMergedExtra" ),
 ##   cut = cms.string( "getSelection(\"cuts_preSel_noOverlap\")") ### && getSelection(\"cuts_fullSel_kinematics\")" )
   cut = cms.string( "getSelection(\"cuts_preSel\")" )
    )
