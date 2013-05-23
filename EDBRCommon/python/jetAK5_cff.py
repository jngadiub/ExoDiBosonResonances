import FWCore.ParameterSet.Config as cms



from ExoDiBosonResonances.EDBRCommon.factories.cmgJet_cfi import cmgJet as cmgJetDummyName
cmgJetRawAK5 = cmgJetDummyName.clone()
cmgJetRawAK5.cfg.PFJetFactory.inputCollection=cms.InputTag('patJetsWithVarCHS')#"selectedPatJetsCHS")
cmgJetRawAK5.cfg.PFJetFactory.puVariables=cms.InputTag("puJetIdCHS")
cmgJetRawAK5.cfg.PFJetFactory.puMvas=cms.VInputTag(
                                      "puJetMvaAK5CHS:cutbasedDiscriminant",
                                      "puJetMvaAK5CHS:simpleDiscriminant", #puJetMvaAK5NoPUSub
                                      "puJetMvaAK5CHS:fullDiscriminant"
                                      )
cmgJetRawAK5.cfg.PFJetFactory.puIds=cms.VInputTag(
                                      "puJetMvaAK5CHS:cutbasedId",
                                      "puJetMvaAK5CHS:simpleId",
                                      "puJetMvaAK5CHS:fullId"                          
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


jetAK5Sequence = cms.Sequence(cmgJetRawAK5+cmgJetAK5Clean+jetAK5 )
