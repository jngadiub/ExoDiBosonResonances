import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
       '/store/cmst3/group/exovv/bonato/EDBR_PATtuple_BulkZZ_HERWIGpp_20131102_20131102_141556/bonato/BulkG_ZZ_lljj_M2500-JHU/EDBR_PATtuple_edbr_BulkZZ_HERWIGpp_20131102/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_lljj_M2500-JHU__tomei-BulkG_ZZ_lljj_M2500-JHU-HERWIG-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_10_1_fuR.root',
              '/store/cmst3/group/exovv/bonato/EDBR_PATtuple_BulkZZ_HERWIGpp_20131102_20131102_141556/bonato/BulkG_ZZ_lljj_M2500-JHU/EDBR_PATtuple_edbr_BulkZZ_HERWIGpp_20131102/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_lljj_M2500-JHU__tomei-BulkG_ZZ_lljj_M2500-JHU-HERWIG-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_1_1_rzS.root',
              '/store/cmst3/group/exovv/bonato/EDBR_PATtuple_BulkZZ_HERWIGpp_20131102_20131102_141556/bonato/BulkG_ZZ_lljj_M2500-JHU/EDBR_PATtuple_edbr_BulkZZ_HERWIGpp_20131102/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_lljj_M2500-JHU__tomei-BulkG_ZZ_lljj_M2500-JHU-HERWIG-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_2_1_CPJ.root',
              '/store/cmst3/group/exovv/bonato/EDBR_PATtuple_BulkZZ_HERWIGpp_20131102_20131102_141556/bonato/BulkG_ZZ_lljj_M2500-JHU/EDBR_PATtuple_edbr_BulkZZ_HERWIGpp_20131102/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_lljj_M2500-JHU__tomei-BulkG_ZZ_lljj_M2500-JHU-HERWIG-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_3_1_y4C.root',
              '/store/cmst3/group/exovv/bonato/EDBR_PATtuple_BulkZZ_HERWIGpp_20131102_20131102_141556/bonato/BulkG_ZZ_lljj_M2500-JHU/EDBR_PATtuple_edbr_BulkZZ_HERWIGpp_20131102/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_lljj_M2500-JHU__tomei-BulkG_ZZ_lljj_M2500-JHU-HERWIG-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_4_1_LiO.root',
              '/store/cmst3/group/exovv/bonato/EDBR_PATtuple_BulkZZ_HERWIGpp_20131102_20131102_141556/bonato/BulkG_ZZ_lljj_M2500-JHU/EDBR_PATtuple_edbr_BulkZZ_HERWIGpp_20131102/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_lljj_M2500-JHU__tomei-BulkG_ZZ_lljj_M2500-JHU-HERWIG-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_5_1_D5k.root',
              '/store/cmst3/group/exovv/bonato/EDBR_PATtuple_BulkZZ_HERWIGpp_20131102_20131102_141556/bonato/BulkG_ZZ_lljj_M2500-JHU/EDBR_PATtuple_edbr_BulkZZ_HERWIGpp_20131102/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_lljj_M2500-JHU__tomei-BulkG_ZZ_lljj_M2500-JHU-HERWIG-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_6_1_S1x.root',
              '/store/cmst3/group/exovv/bonato/EDBR_PATtuple_BulkZZ_HERWIGpp_20131102_20131102_141556/bonato/BulkG_ZZ_lljj_M2500-JHU/EDBR_PATtuple_edbr_BulkZZ_HERWIGpp_20131102/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_lljj_M2500-JHU__tomei-BulkG_ZZ_lljj_M2500-JHU-HERWIG-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_7_1_1HB.root',
              '/store/cmst3/group/exovv/bonato/EDBR_PATtuple_BulkZZ_HERWIGpp_20131102_20131102_141556/bonato/BulkG_ZZ_lljj_M2500-JHU/EDBR_PATtuple_edbr_BulkZZ_HERWIGpp_20131102/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_lljj_M2500-JHU__tomei-BulkG_ZZ_lljj_M2500-JHU-HERWIG-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_8_1_HNt.root',
              '/store/cmst3/group/exovv/bonato/EDBR_PATtuple_BulkZZ_HERWIGpp_20131102_20131102_141556/bonato/BulkG_ZZ_lljj_M2500-JHU/EDBR_PATtuple_edbr_BulkZZ_HERWIGpp_20131102/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_lljj_M2500-JHU__tomei-BulkG_ZZ_lljj_M2500-JHU-HERWIG-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_9_1_yuH.root'
    ])
