import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/cmst3/group/exovv/tomei/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_Herwig_20130618_081716/tomei/BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig__tomei-BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_1_1_Yhq.root',
    '/store/cmst3/group/exovv/tomei/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_Herwig_20130618_081716/tomei/BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig__tomei-BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_2_1_r37.root',
    '/store/cmst3/group/exovv/tomei/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_Herwig_20130618_081716/tomei/BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig__tomei-BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_3_1_oEv.root',
    '/store/cmst3/group/exovv/tomei/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_Herwig_20130618_081716/tomei/BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig__tomei-BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_4_1_Df7.root',
    '/store/cmst3/group/exovv/tomei/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_Herwig_20130618_081716/tomei/BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig__tomei-BulkG_ZZ_llnunu_c0p2_M1100-JHU-herwig-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_5_1_3FC.root',
    ])
