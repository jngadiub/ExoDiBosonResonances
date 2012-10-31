import FWCore.ParameterSet.Config as cms

# do not really on the default cuts implemented here,
# as they are subject to change.
# you should override these cuts in your analysis.

cmgDiElectronSelEDBR = cms.EDFilter(
    "CmgDiElectronSelector",
    src = cms.InputTag( "cmgDiElectronEDBR" ),
    cut = cms.string( "getSelection(\"cuts_zee_kinematics\") && getSelection(\"cuts_zee_quality\") " )
    )
