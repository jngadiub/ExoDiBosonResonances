import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_0.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_1.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_10.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_11.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_12.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_13.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_14.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_15.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_16.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_17.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_18.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_19.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_2.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_20.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_21.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_22.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_23.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_24.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_25.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_26.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_27.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_28.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_29.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_3.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_30.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_31.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_32.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_33.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_34.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_35.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_36.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_37.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_38.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_39.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_4.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_40.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_41.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_42.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_43.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_44.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_45.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_5.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_6.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_7.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_8.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2a/Run2012/DoubleMuParked_Run2012C_22Jan2013/cmgTuple_9.root',
    ])
