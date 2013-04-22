import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Run2012//DoublePhotonHighPt_Run2012C_24Aug2012/cmgTuple_0.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Run2012//DoublePhotonHighPt_Run2012C_24Aug2012/cmgTuple_1.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Run2012//DoublePhotonHighPt_Run2012C_24Aug2012/cmgTuple_2.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Run2012//DoublePhotonHighPt_Run2012C_24Aug2012/cmgTuple_3.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Run2012//DoublePhotonHighPt_Run2012C_24Aug2012/cmgTuple_4.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Run2012//DoublePhotonHighPt_Run2012C_24Aug2012/cmgTuple_5.root',
    ])
