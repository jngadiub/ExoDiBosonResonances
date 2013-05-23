import FWCore.ParameterSet.Config as cms

readFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                                                noEventSort = cms.untracked.bool(True),
                                                duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                                                fileNames = readFiles
                                                )
readFiles.extend([
##    '/store/cmst3/user/bonato//patTuple/2012/EXOVVtest/newPatTuple_ZZ_1000_c1.root'
#        '/store/cmst3/user/bonato//patTuple/2012/EXOVVtest/patExoWW_M600_10_1_KPf.root'
    #        '/store/cmst3/user/bonato//patTuple/2012/EXOVVtest/patZZ_M1000_5k_20121212.root'
#'file:/afs/cern.ch/user/b/bonato/scratch0/PhysAnalysis/EXOVV_2012/CMGTools/CMSSW_5_3_9/src/ExoDiBosonResonances/PATtupleProduction/python/patTuple.v2.root'
    'file:/afs/cern.ch/user/b/bonato/scratch0/PhysAnalysis/EXOVV_2012/CMGTools/CMSSW_5_3_9/src/ExoDiBosonResonances/PATtupleProduction/python/patTuple_XWW.root'
    ])
