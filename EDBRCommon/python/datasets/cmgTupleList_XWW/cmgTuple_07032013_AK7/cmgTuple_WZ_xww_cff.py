import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                                                noEventSort = cms.untracked.bool(True),
                                                duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                                                fileNames = cmgFiles
                                                )

cmgFiles.extend([

       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_0.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_1.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_10.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_11.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_12.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_13.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_14.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_15.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_16.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_17.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_18.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_19.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_2.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_3.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_4.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_5.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_6.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_7.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_8.root',
       '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/AK7/WZ_xww/cmgTuple_9.root',
    ])
