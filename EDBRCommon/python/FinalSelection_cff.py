import FWCore.ParameterSet.Config as cms


### NOTE IMPORTANT: this cff uses only the ele collections
### the muon channel is dealt in the main steering cfg by
### replacing the labels


################
## Add VBF tag to two collections (single- and double-jet)
vbfString = cms.string("vbfptr.isAvailable")
#nXJets was added as userfloat by KineVarsAdder
kineString1Jet=cms.string("mass > 180.0 && leg2.pt()>80.0 && userFloat(\"nXJets\") == 1.0")
kineString2Jet=cms.string("mass > 180.0 && leg2.pt()>80.0 && userFloat(\"nXJets\") == 2.0")
### unused, in just for reference
#LDString0 = cms.string("userFloat(\"LD\") > 0.00025 * userFloat(\"primaryMass\") + 0.55") 
#bString0  = cms.string("getSelection(\"cuts_btags_btag0\")") 


edbrtags =  cms.PSet( vbfDoubleJet = cms.PSet( vbf = vbfString,
                                          kine = kineString2Jet
                                          # LD  = LDString0,
                                          #btag= bString0
                                          ),
                      vbfSingleJet = cms.PSet( vbf = vbfString,
                                               kine = kineString1Jet
                                               ),
                      DoubleJet = cms.PSet( kine = kineString2Jet
                                        ),
                      SingleJet = cms.PSet( kine = kineString1Jet
                                             )
                      )#end edbrtags

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


BestSelectorKinFit=cms.EDProducer("DiElectronNJetEDBRBestCandidateSelector",
                                  srcSingleJet     =cms.InputTag("SingleJetVBFTagger"),
                                  srcDoubleJet     =cms.InputTag("DiJetVBFTagger"),
                                  tagSelectionList =cms.vstring("tag_SingleJet","tag_DoubleJet")#highest priority to lowest priority
                                           )

allSelectedEDBR = cms.EDProducer("CandViewMerger",
       src = cms.VInputTag( "BestSelectorKinFit:singleJet", "BestSelectorKinFit:doubleJet")
  ) 


FinalFilter = cms.EDFilter("CandViewCountFilter",
                           src = cms.InputTag("allSelectedEDBR"),
                           minNumber = cms.uint32(1)
                           )


cmgSeq = cms.Sequence( DiJetVBFTagger+SingleJetVBFTagger+
                       BestSelectorKinFit +
                       allSelectedEDBR + FinalFilter
                       )
