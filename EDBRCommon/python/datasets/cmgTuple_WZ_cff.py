import FWCore.ParameterSet.Config as cms
maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
source = cms.Source ('PoolSource',fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend([
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WZ/cmgTuple_0.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WZ/cmgTuple_1.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WZ/cmgTuple_2.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WZ/cmgTuple_3.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WZ/cmgTuple_4.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WZ/cmgTuple_5.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WZ/cmgTuple_6.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WZ/cmgTuple_7.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WZ/cmgTuple_8.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8//WZ/cmgTuple_9.root',
]);
