import FWCore.ParameterSet.Config as cms
maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
source = cms.Source ('PoolSource',fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend([
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_0.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_1.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_10.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_11.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_12.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_13.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_14.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_15.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_16.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_17.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_18.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_19.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_2.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_3.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_4.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_5.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_6.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_7.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_8.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WW/cmgTuple_9.root',
]);
