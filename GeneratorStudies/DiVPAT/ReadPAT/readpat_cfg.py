import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_100_1_V5y.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_10_1_umt.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_11_1_8hR.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_12_1_UFh.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_13_1_cGe.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_14_1_3xe.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_15_1_Qtn.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_16_1_uWw.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_17_1_dBI.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_18_1_KKF.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_19_1_8Qm.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_1_1_N6S.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_20_1_oAI.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_21_1_nXl.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_22_1_nsg.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_23_1_M6B.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_24_1_TJg.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_25_1_ySN.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_26_1_NoC.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_27_1_8oW.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_28_1_HHI.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_29_1_XiD.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_2_1_u2D.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_30_1_qe2.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_31_1_dsi.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_32_1_0Mz.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_33_1_6Wa.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_34_1_G9I.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_35_1_jjc.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_36_1_r2k.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_37_1_9ax.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_38_1_61C.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_39_1_E64.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_3_1_ce5.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_40_1_DkC.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_41_1_EAD.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_42_1_epH.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_43_1_MKh.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_44_1_PqK.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_45_1_19p.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_46_1_kkD.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_47_1_5oD.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_48_1_hsQ.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_49_1_4fS.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_4_1_RN4.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_50_1_MC9.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_51_1_bbB.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_52_1_D51.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_53_1_xou.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_54_1_poj.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_55_1_kUZ.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_56_1_hNC.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_57_1_kAn.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_58_1_4T3.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_59_1_Ya6.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_5_1_LBN.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_60_1_WtX.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_61_1_DA9.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_62_1_mHO.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_63_1_Egv.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_64_1_pHN.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_65_1_2KG.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_66_1_div.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_67_1_cyB.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_68_1_etx.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_69_1_ha6.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_6_1_fqN.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_70_1_vYE.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_71_1_dOL.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_72_1_LCb.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_73_1_ZET.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_74_1_6PJ.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_75_1_h0o.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_76_1_0UQ.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_77_1_7Wp.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_78_1_l0L.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_79_1_e0G.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_7_1_icL.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_80_1_Eer.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_81_1_DuH.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_82_1_nOO.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_83_1_szp.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_84_1_w6R.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_85_1_3xU.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_86_1_7hv.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_87_1_Nvr.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_88_1_AfI.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_89_1_dEu.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_8_1_qm5.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_90_1_vOT.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_91_1_3TF.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_92_1_MRe.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_93_1_eYC.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_94_1_fpn.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_95_1_AmW.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_96_1_XDV.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_97_1_qSx.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_98_1_NXt.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_99_1_gPU.root',
'/store/user/qliphy/WJetsToLNu_PtW-100_TuneZ2star_8TeV-madgraph/ExoVV-WJetsPTW100/7573e76e6dbb48048324a9a9890f9eb4/patTuple_9_1_4wO.root',

))
process.demo = cms.EDAnalyzer('ReadPAT'
)


OUTPUT_FILE_NAME = "readpat.root"
process.TFileService = cms.Service(
    "TFileService", fileName = cms.string( OUTPUT_FILE_NAME ),
    closeFileFast = cms.untracked.bool(True)
)

process.options = cms.untracked.PSet(
SkipEvent = cms.untracked.vstring('ProductNotFound')
)


process.p = cms.Path(process.demo)
