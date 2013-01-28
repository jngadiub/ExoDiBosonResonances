import FWCore.ParameterSet.Config as cms

AnalyzerXZZ = cms.EDAnalyzer("AnalyzerEDBR",
                             EventCategory=cms.string(""),
                             ###input collections for separated jet topology
                             EDBREEJJColl=cms.InputTag("cmgEDBRSelKinFitEleFullE"),
                             EDBREEJJNoKinFitColl=cms.InputTag("cmgEDBRSelEleFullE"),
                             EDBREEJJLDValueMap=cms.InputTag("dummyForNow"),
                             EDBRMMJJColl=cms.InputTag("cmgEDBRSelKinFitMuFullM"),
                             EDBRMMJJNoKinFitColl=cms.InputTag("cmgEDBRSelMuFullM"),
                             EDBRMMJJLDValueMap=cms.InputTag("dummyForNow"),
                             ###input collections for merged jet topology
                             EDBREEJColl=cms.InputTag("cmgEDBRMergedSelEle"),
                             EDBREEJLDValueMap=cms.InputTag("dummyForNow"),
                             EDBRMMJColl=cms.InputTag("cmgEDBRMergedSelMu"),
                             EDBRMMJLDValueMap=cms.InputTag("dummyForNow"),
                             ### other steering configurations
                             EDBRQGValueMap=cms.InputTag("dummyforNow2"),
                             outFileName=cms.string("tree_TEST.root"),
                             debug=cms.bool(False),
                             isMC=cms.bool(True),
                             treatVBFAsMultiple=cms.bool(True),
                             Ngen=cms.uint32(1000),
                             xsec=cms.double(1.0),
                             triggerNames=cms.vstring()
                             )
