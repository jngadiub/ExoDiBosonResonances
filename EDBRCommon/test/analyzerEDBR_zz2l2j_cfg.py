import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import *
process = cms.Process("EDBR")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000))
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string("START53_V7A::All")
process.load("Configuration.StandardSequences.MagneticField_cff")

readFiles = cms.untracked.vstring()
process.source = cms.Source("PoolSource",
                                                noEventSort = cms.untracked.bool(True),
                                                duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                                                fileNames = readFiles
                                                )
readFiles.extend([
    'file:/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/CMGTools/CMSSW_5_3_3_patch3/src/ExoDiBosonResonances/EDBRCommon/prod/test.root'
    ])


process.ANEDBR = cms.EDAnalyzer("AnalyzerEDBR",
                                EventCategory=cms.string(""),
                                EDBREECollection=cms.InputTag("cmgEDBRSelKinFitEle"),
                                EDBREENoKinFitCollection=cms.InputTag("cmgEDBRSelEle"),
                                EDBREELDValueMap=cms.InputTag("dummyForNow"),
                                EDBRMMCollection=cms.InputTag("cmgEDBRSelKinFitMu"),
                                EDBRMMNoKinFitCollection=cms.InputTag("cmgEDBRSelMu"),
                                EDBRMMLDValueMap=cms.InputTag("dummyForNow"),
                                EDBRQGValueMap=cms.InputTag("dummyforNow2"),
                                outFileName=cms.string("tree_TEST.root"),
                                debug=cms.bool(True),
                                isMC=cms.bool(True),
                                treatVBFAsMultiple=cms.bool(True),
                                Ngen=cms.uint32(1000),
                                xsec=cms.double(1.0),
                                triggerNames=cms.vstring()
                                )

process.p=cms.Path(process.ANEDBR)
