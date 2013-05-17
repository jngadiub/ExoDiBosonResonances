import FWCore.ParameterSet.Config as cms


from  CMGTools.External.pujetidsequence_cff import puJetId, puJetMva
from CMGTools.External.pujetidproducer_cfi import  stdalgos_4x, stdalgos_5x, stdalgos, cutbased, chsalgos_4x, chsalgos_5x, chsalgos
puJetMvaAK5= puJetMva.clone(
    jetids = cms.InputTag("puJetIdCHS"),
    jets ='selectedPatJetsCHS',
    algos =  chsalgos
    )
##

from ExoDiBosonResonances.EDBRCommon.factories.cmgJet_cfi import cmgJet as cmgJetDummyName
cmgJetRawAK5 = cmgJetDummyName.clone()
cmgJetRawAK5.cfg.inputCollection=cms.InputTag('selectedPatJetsCHS')#"selectedPatJets")
cmgJetRawAK5.cfg.puVariables=cms.InputTag("puJetIdCHS")
cmgJetRawAK5.cfg.puMvas=cms.VInputTag(
                                      "puJetMvaAK5:cutbasedDiscriminant",
                                      "puJetMvaAK5:simpleDiscriminant", #puJetMvaAK5NoPUSub
                                      "puJetMvaAK5:fullDiscriminant"
                                      )
cmgJetRawAK5.cfg.puIds=cms.VInputTag(
                                      "puJetMvaAK5:cutbasedId",
                                      "puJetMvaAK5:simpleId",
                                      "puJetMvaAK5:fullId"                          
                                      )


cmgJetAK5Clean = cms.EDProducer("cmgPFJetCleaner",
                          src = cms.InputTag("cmgJetRawAK5"),
                          preselection = cms.string(''),
                          checkOverlaps = cms.PSet( muonIso= cms.PSet( src = cms.InputTag("muonPreselLoose"),
                                                                           preselection        = cms.string(""),  
                                                                           deltaR              = cms.double(0.3),
                                                                           checkRecoComponents = cms.bool(False), 
                                                                           pairCut             = cms.string(""),
                                                                           requireNoOverlaps = cms.bool(True)
                                                                       ),
                                                    
                                                    eleIso= cms.PSet( src = cms.InputTag("electronPreselLoose"),
                                                                           preselection        = cms.string(""),  
                                                                           deltaR              = cms.double(0.3),
                                                                           checkRecoComponents = cms.bool(False), 
                                                                           pairCut             = cms.string(""),
                                                                           requireNoOverlaps = cms.bool(True)
                                                                      )
                                                    ),# end checkOverlaps
                                finalCut = cms.string('')
                                )


jetAK5 = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgJetAK5Clean"),
    cut = cms.string( "getSelection(\"cuts_jetKinematics\") && getSelection(\"cuts_looseJetId\") ")##  &&  !getSelection(\"cuts_muonIso\") && !getSelection(\"cuts_eleIso\")  " )  
    )


jetAK5Sequence = cms.Sequence(puJetMvaAK5+cmgJetRawAK5+cmgJetAK5Clean+jetAK5 )
