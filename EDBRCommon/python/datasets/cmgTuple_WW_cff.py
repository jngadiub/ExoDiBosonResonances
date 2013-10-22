import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_0.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_1.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_10.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_11.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_12.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_13.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_14.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_15.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_16.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_17.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_18.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_19.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_2.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_20.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_21.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_22.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_3.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_4.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_5.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_6.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_7.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_8.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WW/cmgTuple_9.root',
    ])
