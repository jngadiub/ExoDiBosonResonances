import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_0.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_1.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_10.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_11.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_12.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_13.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_14.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_15.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_16.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_17.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_18.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_19.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_2.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_20.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_21.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_22.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_3.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_4.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_5.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_6.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_7.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_8.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2h/Summer12//WZ/cmgTuple_9.root',
    ])
