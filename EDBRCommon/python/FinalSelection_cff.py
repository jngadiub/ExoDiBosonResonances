import FWCore.ParameterSet.Config as cms


### NOTE IMPORTANT: this cff uses only the ele collections
### the muon channel is dealt in the main steering cfg by
### replacing the labels




################
## Add VBF tag to two collections (single- and double-jet)
vbfString = cms.string("vbfptr.isAvailable")
kineString1Jet=cms.string("mass > 180.0 && leg2.pt()>80.0")
kineString2Jet=cms.string("mass > 180.0 && leg2.pt()>80.0")
### unused, in just for reference
#LDString0 = cms.string("userFloat(\"LD\") > 0.00025 * userFloat(\"primaryMass\") + 0.55") 
#bString0  = cms.string("getSelection(\"cuts_btags_btag0\")") 


edbrtags =  cms.PSet( vbfDiJet = cms.PSet( vbf = vbfString,
                                                                 kine = kineString2Jet
#                                                                LD  = LDString0,
#                                                                btag= bString0
                                                                ),
                                            vbfSingleJet = cms.PSet( vbf = vbfString,
                                                                     kine = kineString1Jet
                                                                ),
                                            DiJet = cms.PSet( kine = kineString2Jet
                                                              ),
                                            SingleJet = cms.PSet(  kine = kineString1Jet
                                                                   )
                                            )

DiJetVBFTagger = cms.EDProducer("DiElectronDiJetEDBRTagger",
                           src=cms.InputTag("cmgEDBRSelKinFitEle"),
                           cuts = edbrtags,
                           basename=cms.string("tag")
                           )

SingleJetVBFTagger =  cms.EDProducer("DiElectronSingleJetEDBRTagger",
                           src=cms.InputTag("cmgEDBRMergedSelEle"),
                           cuts = edbrtags,
                           basename=cms.string("tag")
                           )




# take VBF-tagged EDBR collections with one and two jets and
# pass them to the BestCandidateSelector for choosing one candidate per event


# BestSelectorKinFit=cms.EDProducer("DiElectronDiJetHiggsBestCandidateSelector",
#                                           src               =cms.InputTag("MergedSignal"),
#                                           secondary         =cms.InputTag("MergedSignalSecondary"),
#                                           btagpriority      =cms.bool(True),
#                                           btagSelectionList =cms.vstring("cuts_btags_btag2","cuts_btags_btag1","cuts_btags_btag0")#highest priority to lowest priority
#                                           )
# FinalFilter = cms.EDFilter("CandViewCountFilter",
#                                    src = cms.InputTag("BestSelectorKinFit","primary"),
#                                    minNumber = cms.uint32(1)
#                                    )


cmgSeq = cms.Sequence( DiJetVBFTagger+SingleJetVBFTagger
#                       BestSelectorKinFit + FinalFilter
                       )
