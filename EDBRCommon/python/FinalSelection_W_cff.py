import FWCore.ParameterSet.Config as cms


### NOTE IMPORTANT: this cff uses only the ele collections
### the muon channel is dealt in the main steering cfg by
### replacing the labels


################
## Add VBF tag to two collections (single- and double-jet)
vbfString = cms.string("vbfptr.isAvailable")
#nXJets was added as userfloat by KineVarsAdder
kineString1Jet=cms.string("mass > 180.0 && leg1.pt()>200.0 && leg2.pt()>200.0 && userFloat(\"nXJets\") == 1.0") 
kineString2Jet=cms.string("mass > 180.0 && leg1.pt()>80.0 && userFloat(\"nXJets\") == 2.0") # && leg2.pt()>80.0

kineString1JetTTBar=cms.string("mass > 180.0 && leg1.pt()>160.0 && leg2.pt()>200.0 && userFloat(\"nXJets\") == 1.0") 
kineString2JetTTBar=cms.string("mass > 180.0 && leg1.pt()>80.0 && userFloat(\"nXJets\") == 2.0")

sigreg=cms.string("leg2.getSelection(\"cuts_isWSignal\")")
sbreg=cms.string("leg2.getSelection(\"cuts_isWSideband\")")
fullreg=cms.string("leg2.getSelection(\"cuts_isFullRange\")")


### unused, in just for reference
#LDString0 = cms.string("userFloat(\"LD\") > 0.00025 * userFloat(\"primaryMass\") + 0.55") 
#bString0  = cms.string("getSelection(\"cuts_btags_btag0\")") 


edbrtags =  cms.PSet( vbfDoubleJet = cms.PSet( vbf = vbfString,
                                                    kine = kineString2Jet,
                                               sigRegion = sigreg
                                               # LD  = LDString0,
                                               #btag= bString0
                                               ),
                           DoubleJet = cms.PSet( kine = kineString2Jet,
                                                 sigRegion = sigreg
                                                 ),
                           ##### similar but select only in the sideband region
                           vbfDoubleJetSB = cms.PSet( vbf = vbfString,
                                                      kine = kineString2Jet,
                                                      sbRegion = sbreg
                                                      # LD  = LDString0,
                                                 #btag= bString0
                                                      ),
                           DoubleJetSB = cms.PSet( kine = kineString2Jet,
                                                   sbRegion = sbreg
                                              ),
                           ####
                           vbfSingleJet = cms.PSet( vbf = vbfString,
                                                    kine = kineString1Jet,
                                                    sigRegion = sigreg
                                                    ),
                           SingleJet = cms.PSet( kine = kineString1Jet,
                                                 sigRegion = sigreg
                                                 ),
                           ##### similar but select only in the sideband region
                           vbfSingleJetSB = cms.PSet( vbf = vbfString,
                                                      kine = kineString1Jet,
                                                      sbRegion = sbreg
                                                      ),
                           SingleJetSB = cms.PSet( kine = kineString1Jet,
                                                   sbRegion = sbreg
                                                   ) ,  
						   #############################full range for ttbar control
                           SingleJetFull  = cms.PSet( kine = kineString1JetTTBar,
                                                   fullRegion = fullreg
                                                   ),
                           DoubleJetFull  = cms.PSet( kine = kineString2JetTTBar,
                                                   fullRegion = fullreg
                                                   )                        
                           )#end edbrtags





DiJetVBFTagger = cms.EDProducer("WelenuDiJetEDBRTagger",
                           src=cms.InputTag("cmgEDBRSelKinFitEle"),
                           cuts = edbrtags,
                           basename=cms.string("tag")
                           )

SingleJetVBFTagger =  cms.EDProducer("WelenuSingleJetEDBRTagger",
                           src=cms.InputTag("cmgEDBRMergedSelEle"),
                           cuts = edbrtags,
                           basename=cms.string("tag")
                           )




# take VBF-tagged EDBR collections with one and two jets and
# pass them to the BestCandidateSelector for choosing one candidate per event


BestCandSelector=cms.EDProducer("WelenuNJetEDBRBestCandidateSelector",
                                  srcSingleJet     =cms.InputTag("SingleJetVBFTagger"),
                                  srcDoubleJet     =cms.InputTag("DiJetVBFTagger"),
                                  tagSelectionList =cms.vstring("tag_SingleJet","tag_DoubleJet"),#highest priority to lowest priority
                                  VMass            =cms.double(80.4)
                                           )

BestSidebandSelector=cms.EDProducer("WelenuNJetEDBRBestCandidateSelector",
                                    srcSingleJet     =cms.InputTag("SingleJetVBFTagger"),
                                    srcDoubleJet     =cms.InputTag("DiJetVBFTagger"),
                                    tagSelectionList =cms.vstring("tag_SingleJetSB","tag_DoubleJetSB"),#highest priority to lowest priority
                                    VMass            =cms.double(80.4)
                                    )

BestFullRangeSelector=cms.EDProducer("WelenuNJetEDBRBestCandidateSelector",
                                    srcSingleJet     =cms.InputTag("SingleJetVBFTagger"),
                                    srcDoubleJet     =cms.InputTag("DiJetVBFTagger"),
                                    tagSelectionList =cms.vstring("tag_SingleJetFull","tag_DoubleJetFull"),#highest priority to lowest priority
                                    VMass            =cms.double(80.4)
									)
                                    

allSelectedEDBR = cms.EDProducer("CandViewMerger",
       src = cms.VInputTag( "BestCandSelector:singleJet", "BestCandSelector:doubleJet", "BestSidebandSelector:singleJet", "BestSidebandSelector:doubleJet","BestFullRangeSelector:singleJet", "BestFullRangeSelector:doubleJet")
  )


FinalFilter = cms.EDFilter("CandViewCountFilter",
                           src = cms.InputTag("allSelectedEDBR"),
                           minNumber = cms.uint32(1)
                           )


cmgSeq = cms.Sequence( DiJetVBFTagger+SingleJetVBFTagger+
                       BestCandSelector + BestSidebandSelector +  BestFullRangeSelector +
                       allSelectedEDBR + FinalFilter
                       )
