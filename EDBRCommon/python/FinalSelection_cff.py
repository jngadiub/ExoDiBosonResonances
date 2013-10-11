import FWCore.ParameterSet.Config as cms


### NOTE IMPORTANT: this cff uses only the ele collections
### the muon channel is dealt in the main steering cfg by
### replacing the labels


################
## Add VBF tag to two collections (single- and double-jet)
vbfString = cms.string("vbfptr.isAvailable")
#nXJets was added as userfloat by KineVarsAdder
kineString1Jet=cms.string("mass > 180.0 && leg1.pt()>80.0 && leg2.pt()>80.0 && userFloat(\"nXJets\") == 1.0") # && leg2.pt()>80.0
kineString2Jet=cms.string("mass > 180.0 && leg1.pt()>80.0 && leg2.pt()>80.0 && userFloat(\"nXJets\") == 2.0") # && leg2.pt()>80.0
sigregZ=cms.string("leg2.getSelection(\"cuts_isZSignal\")")
vtagHP=cms.string("leg2.ntau21()<0.50")
sbregZ=cms.string("leg2.getSelection(\"cuts_isZSideband\")")
vtagLP=cms.string("leg2.ntau21()>0.50&& leg2.ntau21()<0.75")



### unused, in just for reference
#LDString0 = cms.string("userFloat(\"LD\") > 0.00025 * userFloat(\"primaryMass\") + 0.55") 
#bString0  = cms.string("getSelection(\"cuts_btags_btag0\")") 


edbrtags1J =  cms.PSet(
    ###double jet tags will be dummy for 1J, but we need them for having only one set
    ###of flags and avoiding a flood of annoying error messages when running
    ### the BestCandidateSelector
                       vbfDoubleJet = cms.PSet(kine = kineString2Jet),
                       DoubleJet = cms.PSet( kine = kineString2Jet),
                       vbfDoubleJetSB = cms.PSet(kine = kineString2Jet),
                       DoubleJetSB = cms.PSet( kine = kineString2Jet),
                       ########### 1J part
                       vbfSingleJetHP = cms.PSet( vbf = vbfString,
                                               kine = kineString1Jet,
                                               sigRegion = sigregZ,
                                               vtag = vtagHP
                                               ),
                      SingleJetHP = cms.PSet( kine = kineString1Jet,
                                            sigRegion = sigregZ
                                            ),
                      vbfSingleJetLP = cms.PSet( vbf = vbfString,
                                               kine = kineString1Jet,
                                               sigRegion = sigregZ,
                                               vtag=vtagLP
                                               ),
                      SingleJetLP = cms.PSet( kine = kineString1Jet,
                                              sigRegion = sigregZ,
                                              vtag=vtagLP
                                            ),
                      ##### similar but select only in the sideband region
                      vbfSingleJetSBHP = cms.PSet( vbf = vbfString,
                                               kine = kineString1Jet,
                                               sbRegion = sbregZ,
                                               vtag = vtagHP
                                               ),
                      SingleJetSBHP = cms.PSet( kine = kineString1Jet,
                                            sbRegion = sbregZ
                                            ),
                      vbfSingleJetSBLP = cms.PSet( vbf = vbfString,
                                               kine = kineString1Jet,
                                               sbRegion = sbregZ,
                                               vtag=vtagLP
                                               ),
                      SingleJetSBLP = cms.PSet( kine = kineString1Jet,
                                              sbRegion = sbregZ,
                                              vtag=vtagLP
                                            )                           
                      )#end edbrtags


edbrtags2J =  cms.PSet( vbfDoubleJet = cms.PSet( vbf = vbfString,
                                               kine = kineString2Jet,
                                               sigRegion = sigregZ
                                               # LD  = LDString0,
                                               #btag= bString0
                                               ),
                      DoubleJet = cms.PSet( kine = kineString2Jet,
                                            sigRegion = sigregZ
                                            ),
                      ##### similar but select only in the sideband region
                      vbfDoubleJetSB = cms.PSet( vbf = vbfString,
                                                 kine = kineString2Jet,
                                                 sbRegion = sbregZ
                                                 # LD  = LDString0,
                                                 #btag= bString0
                                                 ),
                      DoubleJetSB = cms.PSet( kine = kineString2Jet,
                                              sbRegion = sbregZ
                                              ) ,
                        ###same as above: 1J flags are dummy (will always be false) for the 2J case,
                        ### the important thing is that they are there
                        vbfSingleJetHP = cms.PSet( kine = kineString1Jet),
                        SingleJetHP = cms.PSet( kine = kineString1Jet),
                        vbfSingleJetLP = cms.PSet( kine = kineString1Jet),
                        SingleJetLP = cms.PSet( kine = kineString1Jet),
                        vbfSingleJetSBHP = cms.PSet( kine = kineString1Jet),
                        SingleJetSBHP = cms.PSet( kine = kineString1Jet),
                        vbfSingleJetSBLP = cms.PSet( kine = kineString1Jet),
                        SingleJetSBLP = cms.PSet( kine = kineString1Jet)
                      )#end edbrtags





DiJetVBFTagger = cms.EDProducer("DiElectronDiJetEDBRTagger",
                           src=cms.InputTag("cmgEDBRSelKinFitEle"),
                           cuts = edbrtags2J,
                           basename=cms.string("tag")
                           )

SingleJetVBFTagger =  cms.EDProducer("DiElectronSingleJetEDBRTagger",
                           src=cms.InputTag("cmgEDBRMergedSelEle"),
                           cuts = edbrtags1J,
                           basename=cms.string("tag")
                           )




# take VBF-tagged EDBR collections with one and two jets and
# pass them to the BestCandidateSelector for choosing one candidate per event


BestCandSelector=cms.EDProducer("DiElectronNJetEDBRBestCandidateSelector",
                                srcSingleJet     =cms.InputTag("SingleJetVBFTagger"),
                                srcDoubleJet     =cms.InputTag("DiJetVBFTagger"),
                                tagSelectionList =cms.vstring("tag_SingleJetHP","tag_SingleJetLP","tag_DoubleJet"),#highest priority to lowest priority
                                VMass            =cms.double(91.1876),
                                Algo             =cms.string("UseJetPt")##could be UseVMass or UseJetPt
                                )

BestSidebandSelector=cms.EDProducer("DiElectronNJetEDBRBestCandidateSelector",
                                    srcSingleJet     =cms.InputTag("SingleJetVBFTagger"),
                                    srcDoubleJet     =cms.InputTag("DiJetVBFTagger"),
                                    tagSelectionList =cms.vstring("tag_SingleJetSBHP","tag_SingleJetSBLP","tag_DoubleJetSB"),#highest priority to lowest priority
                                    VMass            =cms.double(91.1876),
                                    Algo             =cms.string("UseJetPt")##could be UseVMass or UseJetPt
                                    )

allSelectedEDBR = cms.EDProducer("CandViewMerger",
       src = cms.VInputTag( "BestCandSelector:singleJet", "BestCandSelector:doubleJet", "BestSidebandSelector:singleJet", "BestSidebandSelector:doubleJet")
  ) 


FinalFilter = cms.EDFilter("CandViewCountFilter",
                           src = cms.InputTag("allSelectedEDBR"),
                           minNumber = cms.uint32(1)
                           )


cmgSeq = cms.Sequence( DiJetVBFTagger+SingleJetVBFTagger+
                       BestCandSelector + BestSidebandSelector +
                       allSelectedEDBR + FinalFilter
                       )
