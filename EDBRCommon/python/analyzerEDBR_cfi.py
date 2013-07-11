import FWCore.ParameterSet.Config as cms

AnalyzerXZZ = cms.EDAnalyzer("AnalyzerEDBR",
                             EventCategory=cms.string(""),
                             VType=cms.string("Z"),
                             ###input collections for separated jet topology
                             EDBREEJJColl=cms.InputTag("cmgEDBRSelKinFitEle"),
                             EDBREEJJLDValueMap=cms.InputTag("dummyForNow"),
                             EDBRMMJJColl=cms.InputTag("cmgEDBRSelKinFitMu"),
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
                             treatVBFAsMultiple=cms.bool(False),
                             saveVBFTaggedCands=cms.bool(False),
                             Ngen=cms.uint32(1000),
                             xsec=cms.double(1.0),
                             triggerNames=cms.vstring(),
                             FillGenLevelCode=cms.uint32(0), #0=nothing, 1=Zll,2=Zqq,3=Zll+Zqq, 4=XVV,5=Zqq+XVV, 6=Zll+XVV, 7=XVV+Zll+Zqq
                             VTaggingScaleFactor=cms.double(1.0)
                             )
