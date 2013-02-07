import FWCore.ParameterSet.Config as cms
maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
source = cms.Source ('PoolSource',fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend([
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomeiTEST/Run2012/presel/DoubleMu_Run2012A_13Jul2012/cmgTuple_0.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomeiTEST/Run2012/presel/DoubleMu_Run2012A_13Jul2012/cmgTuple_1.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomeiTEST/Run2012/presel/DoubleMu_Run2012A_13Jul2012/cmgTuple_10.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomeiTEST/Run2012/presel/DoubleMu_Run2012A_13Jul2012/cmgTuple_11.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomeiTEST/Run2012/presel/DoubleMu_Run2012A_13Jul2012/cmgTuple_12.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomeiTEST/Run2012/presel/DoubleMu_Run2012A_13Jul2012/cmgTuple_13.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomeiTEST/Run2012/presel/DoubleMu_Run2012A_13Jul2012/cmgTuple_2.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomeiTEST/Run2012/presel/DoubleMu_Run2012A_13Jul2012/cmgTuple_3.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomeiTEST/Run2012/presel/DoubleMu_Run2012A_13Jul2012/cmgTuple_4.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomeiTEST/Run2012/presel/DoubleMu_Run2012A_13Jul2012/cmgTuple_5.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomeiTEST/Run2012/presel/DoubleMu_Run2012A_13Jul2012/cmgTuple_6.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomeiTEST/Run2012/presel/DoubleMu_Run2012A_13Jul2012/cmgTuple_7.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomeiTEST/Run2012/presel/DoubleMu_Run2012A_13Jul2012/cmgTuple_8.root',
'/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomeiTEST/Run2012/presel/DoubleMu_Run2012A_13Jul2012/cmgTuple_9.root',
]);
