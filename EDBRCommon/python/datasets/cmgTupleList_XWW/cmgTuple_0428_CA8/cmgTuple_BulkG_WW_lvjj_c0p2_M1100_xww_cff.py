import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/user/shuai/ExoDiBosonResonances/CMGtuple/production0428/Summer12/CA8//BulkG_WW_lvjj_c0p2_M1100_xww/cmgTuple_0.root',
    '/store/user/shuai/ExoDiBosonResonances/CMGtuple/production0428/Summer12/CA8//BulkG_WW_lvjj_c0p2_M1100_xww/cmgTuple_1.root',
    '/store/user/shuai/ExoDiBosonResonances/CMGtuple/production0428/Summer12/CA8//BulkG_WW_lvjj_c0p2_M1100_xww/cmgTuple_2.root',
    ])
