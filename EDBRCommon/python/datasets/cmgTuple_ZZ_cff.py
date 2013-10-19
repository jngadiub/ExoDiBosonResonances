import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_0.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_1.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_10.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_11.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_12.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_13.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_14.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_15.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_16.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_17.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_18.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_19.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_2.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_20.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_21.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_22.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_3.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_4.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_5.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_6.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_7.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_8.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//ZZ/cmgTuple_9.root',
    ])
