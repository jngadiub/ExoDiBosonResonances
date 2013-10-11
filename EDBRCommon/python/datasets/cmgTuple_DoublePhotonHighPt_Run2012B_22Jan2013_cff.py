import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_0.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_1.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_10.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_11.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_12.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_13.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_14.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_15.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_16.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_2.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_3.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_4.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_5.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_6.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_7.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_8.root',
    '/store/cmst3/group/exovv/CMGtuple/productionv2g_HPTjet/Run2012/DoublePhotonHighPt_Run2012B_22Jan2013/cmgTuple_9.root',
    ])
