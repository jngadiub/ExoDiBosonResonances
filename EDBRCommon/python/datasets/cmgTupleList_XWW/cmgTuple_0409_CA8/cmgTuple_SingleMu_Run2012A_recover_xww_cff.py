import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/user/shuai/ExoDiBosonResonances/CMGtuple/production0409/Run2012/CA8//SingleMu_Run2012A_recover_xww/cmgTuple_0.root',
    '/store/user/shuai/ExoDiBosonResonances/CMGtuple/production0409/Run2012/CA8//SingleMu_Run2012A_recover_xww/cmgTuple_1.root',
    '/store/user/shuai/ExoDiBosonResonances/CMGtuple/production0409/Run2012/CA8//SingleMu_Run2012A_recover_xww/cmgTuple_2.root',
    '/store/user/shuai/ExoDiBosonResonances/CMGtuple/production0409/Run2012/CA8//SingleMu_Run2012A_recover_xww/cmgTuple_3.root',
    '/store/user/shuai/ExoDiBosonResonances/CMGtuple/production0409/Run2012/CA8//SingleMu_Run2012A_recover_xww/cmgTuple_4.root',
    '/store/user/shuai/ExoDiBosonResonances/CMGtuple/production0409/Run2012/CA8//SingleMu_Run2012A_recover_xww/cmgTuple_5.root',
    ])
