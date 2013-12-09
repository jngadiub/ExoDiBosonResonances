import FWCore.ParameterSet.Config as cms

##MU-TAUH CHANNEL
ak5PFJetsNoMu = cms.EDProducer(
    "CleanJetsProducer"
    )
CleanJetsSequence = cms.Sequence(ak5PFJetsNoMu)


##ELE-TAUH CHANNEL
ak5PFJetsNoEle = cms.EDProducer(
    "CleanJetsETauProducer"
    )
CleanJetsETauSequence = cms.Sequence(ak5PFJetsNoEle)
