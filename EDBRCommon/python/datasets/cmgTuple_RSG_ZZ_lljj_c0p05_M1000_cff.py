import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/cmst3/group/exovv/CMGtuple/productionv2i/Summer12//RSG_ZZ_lljj_c0p05_M1000/cmgTuple_0.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2i/Summer12//RSG_ZZ_lljj_c0p05_M1000/cmgTuple_1.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2i/Summer12//RSG_ZZ_lljj_c0p05_M1000/cmgTuple_2.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2i/Summer12//RSG_ZZ_lljj_c0p05_M1000/cmgTuple_3.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2i/Summer12//RSG_ZZ_lljj_c0p05_M1000/cmgTuple_4.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2i/Summer12//RSG_ZZ_lljj_c0p05_M1000/cmgTuple_5.root',
    ])
