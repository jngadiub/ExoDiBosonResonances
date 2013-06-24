import FWCore.ParameterSet.Config as cms
maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
source = cms.Source ('PoolSource',fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend([
       '/store/cmst3/group/exovv/caber/EDBR_PATtuple_edbr_zz_20130607_Summer12MC_BulkGravitonsZZ_20130607_195908/caber/BulkG_ZZ_lljj_M1500_G40-JHU/EDBR_PATtuple_edbr_zz_20130607/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_lljj_M1500_G40-JHU__tomei-BulkG_ZZ_lljj_M1500_G40-JHU-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_1_5_WDP.root',
       '/store/cmst3/group/exovv/caber/EDBR_PATtuple_edbr_zz_20130607_Summer12MC_BulkGravitonsZZ_20130607_195908/caber/BulkG_ZZ_lljj_M1500_G40-JHU/EDBR_PATtuple_edbr_zz_20130607/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_lljj_M1500_G40-JHU__tomei-BulkG_ZZ_lljj_M1500_G40-JHU-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_2_4_G6l.root',
       '/store/cmst3/group/exovv/caber/EDBR_PATtuple_edbr_zz_20130607_Summer12MC_BulkGravitonsZZ_20130607_195908/caber/BulkG_ZZ_lljj_M1500_G40-JHU/EDBR_PATtuple_edbr_zz_20130607/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_lljj_M1500_G40-JHU__tomei-BulkG_ZZ_lljj_M1500_G40-JHU-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_3_4_VK6.root',
       '/store/cmst3/group/exovv/caber/EDBR_PATtuple_edbr_zz_20130607_Summer12MC_BulkGravitonsZZ_20130607_195908/caber/BulkG_ZZ_lljj_M1500_G40-JHU/EDBR_PATtuple_edbr_zz_20130607/1b325ddfb984c14533be7920e22baeef/BulkG_ZZ_lljj_M1500_G40-JHU__tomei-BulkG_ZZ_lljj_M1500_G40-JHU-c8f8ed334db8a7d6f56c62266b1dfa5b__USER_4_4_2bu.root',
]);
