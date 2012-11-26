import FWCore.ParameterSet.Config as cms

ZeeCand = cms.EDFilter(
    "CmgDiElectronSelector",
    src = cms.InputTag("cmgDiElectronEDBR"),
    cut = cms.string( "getSelection(\"cuts_zee_kinematics\") && getSelection(\"cuts_zee_quality\") && getSelection(\"cuts_charge\")" )
    )

selectedZeeCandFilter = cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag('ZeeCand'),
   minNumber = cms.uint32(1)
 )

selectedZSequence = cms.Sequence(ZeeCand+selectedZeeCandFilter)

