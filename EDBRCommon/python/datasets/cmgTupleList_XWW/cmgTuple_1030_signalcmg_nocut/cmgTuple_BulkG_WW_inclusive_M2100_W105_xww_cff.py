import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/cmst3/group/exovv/CMGtuple/shuai/production1101/Summer12/CA8//BulkG_WW_inclusive_M2100_W105_xww/cmgTuple_0.root',
    '/store/cmst3/group/exovv/CMGtuple/shuai/production1101/Summer12/CA8//BulkG_WW_inclusive_M2100_W105_xww/cmgTuple_1.root',
    '/store/cmst3/group/exovv/CMGtuple/shuai/production1101/Summer12/CA8//BulkG_WW_inclusive_M2100_W105_xww/cmgTuple_10.root',
    '/store/cmst3/group/exovv/CMGtuple/shuai/production1101/Summer12/CA8//BulkG_WW_inclusive_M2100_W105_xww/cmgTuple_2.root',
    '/store/cmst3/group/exovv/CMGtuple/shuai/production1101/Summer12/CA8//BulkG_WW_inclusive_M2100_W105_xww/cmgTuple_3.root',
    '/store/cmst3/group/exovv/CMGtuple/shuai/production1101/Summer12/CA8//BulkG_WW_inclusive_M2100_W105_xww/cmgTuple_4.root',
    '/store/cmst3/group/exovv/CMGtuple/shuai/production1101/Summer12/CA8//BulkG_WW_inclusive_M2100_W105_xww/cmgTuple_5.root',
    '/store/cmst3/group/exovv/CMGtuple/shuai/production1101/Summer12/CA8//BulkG_WW_inclusive_M2100_W105_xww/cmgTuple_6.root',
    '/store/cmst3/group/exovv/CMGtuple/shuai/production1101/Summer12/CA8//BulkG_WW_inclusive_M2100_W105_xww/cmgTuple_7.root',
    '/store/cmst3/group/exovv/CMGtuple/shuai/production1101/Summer12/CA8//BulkG_WW_inclusive_M2100_W105_xww/cmgTuple_8.root',
    '/store/cmst3/group/exovv/CMGtuple/shuai/production1101/Summer12/CA8//BulkG_WW_inclusive_M2100_W105_xww/cmgTuple_9.root',
    ])
