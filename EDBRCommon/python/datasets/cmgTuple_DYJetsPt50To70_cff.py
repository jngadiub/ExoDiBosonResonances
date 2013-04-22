import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_0.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_1.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_10.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_11.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_12.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_13.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_14.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_15.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_16.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_17.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_18.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_19.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_2.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_20.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_21.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_3.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_4.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_5.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_6.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_7.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_8.root',
    '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/Summer12//DYJetsPt50To70/cmgTuple_9.root',
    ])
