import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_10_1_rsX.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_11_1_1b7.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_12_1_i1S.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_13_1_wnO.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_14_1_gla.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_15_1_wyu.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_16_1_P15.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_17_1_Rph.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_18_1_6YH.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_19_1_Wnv.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_1_1_i8F.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_20_1_cJD.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_21_1_na5.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_2_1_Eu7.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_3_1_uPg.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_4_1_jxO.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_5_1_9ut.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_6_1_wd5.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_7_1_iS8.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_8_1_i63.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20131015_200317/santanas/BulkG_WW_jjjj_c0p2_M1500-JHU/EDBR_PATtuple_edbr_zz_20130605/9e8031288ea905f4456666a2f5084e75/BulkG_WW_jjjj_c0p2_M1500-JHU__qili-BulkG_WW_jjjj_c0p2_M1500-JHU-AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_9_1_IRf.root',
    ])
