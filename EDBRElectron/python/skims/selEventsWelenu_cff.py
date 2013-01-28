import FWCore.ParameterSet.Config as cms 

WelenuCand = cms.EDFilter(
    "CmgWelenuSelector",
    src = cms.InputTag("cmgWelenuEDBR"),
####    cut = cms.string( "getSelection(\"cuts_Welenu_kinematics\") && getSelection(\"cuts_Welenu_quality\") && getSelection(\"cuts_Welenu_validation\")"   )
        cut = cms.string( "getSelection(\"cuts_Welenu_kinematics\") && getSelection(\"cuts_Welenu_validation\")"   )
    )   

selectedWelenuCandFilter = cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag('WelenuCand'),
   minNumber = cms.uint32(1)
 )

selectedWelenuSequence = cms.Sequence(WelenuCand+selectedWelenuCandFilter)
