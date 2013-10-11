import FWCore.ParameterSet.Config as cms

cmgFiles = cms.untracked.vstring()
source = cms.Source("PoolSource",
                    noEventSort = cms.untracked.bool(True),
                    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                    fileNames = cmgFiles
                   )

cmgFiles.extend([
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_10_1_Ntf.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_11_1_aTk.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_12_1_Jx8.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_13_1_fq6.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_14_1_Uh3.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_15_1_pxX.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_16_1_HGS.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_17_1_yX4.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_18_1_QE4.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_19_1_tl2.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_1_1_NTJ.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_20_1_zpM.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_21_1_60g.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_2_1_zAx.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_3_1_JN7.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_4_1_eCz.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_5_2_7x5.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_6_1_IIj.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_7_1_QRn.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_8_1_eaI.root',
    '/store/cmst3/group/exovv/santanas/EDBR_PATtuple_edbr_zz_20130605_Summer12MC_BulkGravitonsWW_20130916_175629/santanas/BulkG_WW_inclusive_M2100_W315_GENSIM/EDBR_PATtuple_edbr_zz_20130605/1b325ddfb984c14533be7920e22baeef/BulkG_WW_inclusive_M2100_W315_GENSIM__shuai-BulkG_WW_inclusive_M2100_W315_AODSIM-2c74483358b1f8805e5601fc325d256c__USER_9_1_WjU.root',
    ])