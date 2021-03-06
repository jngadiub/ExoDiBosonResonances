import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/cmst3/group/exovv/CMGtuple/shuai/production1101/Summer12/CA8//WprimeWZ_inclusive_M2500_PYTHIA6_xww/cmgTuple_0.root',
    '/store/cmst3/group/exovv/CMGtuple/shuai/production1101/Summer12/CA8//WprimeWZ_inclusive_M2500_PYTHIA6_xww/cmgTuple_1.root',
    '/store/cmst3/group/exovv/CMGtuple/shuai/production1101/Summer12/CA8//WprimeWZ_inclusive_M2500_PYTHIA6_xww/cmgTuple_2.root',
    ])
