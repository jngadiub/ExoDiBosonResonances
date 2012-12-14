

AnalyzerXZZ = cms.EDAnalyzer("AnalyzerEDBR",
                             EventCategory=cms.string(""),
                             ###input collections for separated jet topology
                             EDBREEJJColl=cms.InputTag("cmgEDBRSelKinFitEle"),
                             EDBREEJJNoKinFitColl=cms.InputTag("cmgEDBRSelEle"),
                             EDBREEJJLDValueMap=cms.InputTag("dummyForNow"),
                             EDBRMMJJColl=cms.InputTag("cmgEDBRSelKinFitMu"),
                             EDBRMMJJNoKinFitColl=cms.InputTag("cmgEDBRSelMu"),
                             EDBRMMJJLDValueMap=cms.InputTag("dummyForNow"),
                             ###input collections for merged jet topology
                             EDBREEJColl=cms.InputTag("cmgEDBRMergedSelKinFitEle"),
                             EDBREEJLDValueMap=cms.InputTag("dummyForNow"),
                             EDBRMMJColl=cms.InputTag("cmgEDBRMergedSelKinFitMu"),
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
