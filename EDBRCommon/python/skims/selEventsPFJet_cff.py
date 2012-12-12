import FWCore.ParameterSet.Config as cms

jetIDJet = cms.EDFilter(
#    "CmgBaseJetSelector",
    "CmgPFJetSelector",
    src = cms.InputTag("cmgJet"),
    cut = cms.string( " getSelection(\"cuts_jetKinematics\") && getSelection(\"cuts_looseJetId\")")
    )

selectedJetCandFilter = cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag('jetIDJet'),
   minNumber = cms.uint32(2)
 )

vbfJet = cms.EDFilter("CmgPFJetSelector",
                      src = cms.InputTag("cmgJet"),
                      cut = cms.string("getSelection(\"cuts_looseJetId\") && getSelection(\"cuts_jetKinematics_pt\")")
                      )



selectedJetSequence = cms.Sequence(vbfJet + jetIDJet + selectedJetCandFilter)
#selectedJetSequence = cms.Sequence(jetIDJet) #for PU studies I want all the jets


######## for Merged topology
vbfJetMerged = cms.EDFilter("CmgVJetSelector",
                      src = cms.InputTag("cmgJetStructured"),
                      cut = cms.string("getSelection(\"cuts_looseJetId\") && getSelection(\"cuts_jetKinematics_pt\")")
                      )


jetIDMerged = cms.EDFilter(
    "CmgVJetSelector",
    src = cms.InputTag("cmgJetStructured"),
    cut = cms.string( " getSelection(\"cuts_mergedJetKinematics\") && getSelection(\"cuts_mergedJetVTagging\")") ### && getSelection(\"cuts_signalBoostedZ\")
    )

selectedVJetCandFilter = cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag('jetIDMerged'),
   minNumber = cms.uint32(1)
 )

selectedMergedJetSequence = cms.Sequence(vbfJetMerged + jetIDMerged + selectedVJetCandFilter)
