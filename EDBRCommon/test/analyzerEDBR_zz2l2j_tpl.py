import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import *
process = cms.Process("EDBR")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string("START53_V7A::All")
process.load("Configuration.StandardSequences.MagneticField_cff")


### input cmgTuples
process.load("ExoDiBosonResonances.EDBRCommon.datasets.cmgTuple_<SAMPLE>_cff")
#CA8
#process.load("ExoDiBosonResonances.EDBRCommon.datasets.cmgTupleList_XWW.cmgTuple_08032013_CA8.cmgTuple_<SAMPLE>_cff")
#AK7
#process.load("ExoDiBosonResonances.EDBRCommon.datasets.cmgTupleList_XWW.cmgTuple_08032013_AK7.cmgTuple_<SAMPLE>_cff")
#
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

## process.source = cms.Source("PoolSource",
##                     noEventSort = cms.untracked.bool(True),
##                     duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
##                     fileNames = cms.untracked.vstring(
##     '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1/Summer12/presel/TTBAR/test_0.root')
##                     )

### it's useful to have the summary
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
    )

from ExoDiBosonResonances.EDBRCommon.analyzerEDBR_cfi import AnalyzerXZZ
process.ANEDBR = AnalyzerXZZ.clone(
    debug=cms.bool(False),
    outFileName=cms.string("treeEDBR_<SAMPLE>.root"),
    VType=cms.string("Z"),
    Ngen=cms.uint32(1),
    xsec=cms.double(1.0) ###in pb
    )



### if false, use the default collections
### in ExoDiBosonResonances.EDBRCommon.analyzerEDBR_cfi
### (i.e. all the cands passing pre-selection cuts)
processFullSel=False

if processFullSel :
       process.ANEDBR.EDBREEJJColl=cms.InputTag("BestSidebandSelectorEle:doubleJet")
       process.ANEDBR.EDBRMMJJColl=cms.InputTag("BestSidebandSelectorMu:doubleJet")
       process.ANEDBR.EDBREEJColl=cms.InputTag("BestSidebandSelectorEle:singleJet")
       process.ANEDBR.EDBRMMJColl=cms.InputTag("BestSidebandSelectorMu:singleJet")
       #process.ANEDBR.EDBREEJJColl=cms.InputTag("BestCandSelectorEle:doubleJet")
       #process.ANEDBR.EDBRMMJJColl=cms.InputTag("BestCandSelectorMu:doubleJet")
       #process.ANEDBR.EDBREEJColl=cms.InputTag("BestCandSelectorEle:singleJet")
       #process.ANEDBR.EDBRMMJColl=cms.InputTag("BestCandSelectorMu:singleJet")

#    process.ANEDBR.EDBREEJJColl=cms.InputTag("BestFullRangeSelectorEle:doubleJet")
#    process.ANEDBR.EDBRMMJJColl=cms.InputTag("BestFullRangeSelectorMu:doubleJet")
#    process.ANEDBR.EDBREEJColl=cms.InputTag("BestFullRangeSelectorEle:singleJet")
#    process.ANEDBR.EDBRMMJColl=cms.InputTag("BestFullRangeSelectorMu:singleJet")


    #process.ANEDBR.EDBREEJJColl=cms.InputTag("BestTTBarSelectorEle:doubleJet")
    #process.ANEDBR.EDBRMMJJColl=cms.InputTag("BestTTBarSelectorMu:doubleJet")
    #process.ANEDBR.EDBREEJColl=cms.InputTag("BestTTBarSelectorEle:singleJet")
    #process.ANEDBR.EDBRMMJColl=cms.InputTag("BestTTBarSelectorMu:singleJet")

##### set Ngen and xsect values ofr MC samples; xsect in pb !!! 
if "TTBAR"=="<SAMPLE>":
    process.ANEDBR.Ngen=cms.uint32(6540800)
    process.ANEDBR.xsec=cms.double(225.197)
elif "TTBAR_xww"=="<SAMPLE>":
    process.ANEDBR.Ngen=cms.uint32(6540800)
    process.ANEDBR.xsec=cms.double(225.197)
elif "TTBARpowheg"=="<SAMPLE>":
    process.ANEDBR.Ngen=cms.uint32(21675970)
    process.ANEDBR.xsec=cms.double(225.197)
elif "TTBARpowheg_xww"=="<SAMPLE>":
    process.ANEDBR.Ngen=cms.uint32(21675970)
    process.ANEDBR.xsec=cms.double(225.197)
elif "DYJetsPt50To70" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(4823000)
    process.ANEDBR.xsec=cms.double(105.7) # 105.7 = 89.0 (from PREP) times 1.188
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)
elif "DYJetsPt70To100" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11764538)
    process.ANEDBR.xsec=cms.double(62.9) # 62.9 = 53.0 (from PREP) times 1.187
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)
elif "DYJetsPt100" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(12511326)
    process.ANEDBR.xsec=cms.double(39.1) # 39.1 = 32.9 (from PREP) times 1.188
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)
elif "WW"=="<SAMPLE>":
    process.ANEDBR.Ngen=cms.uint32(10000431)
    process.ANEDBR.xsec=cms.double(57.1097)
elif "WZ"=="<SAMPLE>":
    process.ANEDBR.Ngen=cms.uint32(9955839)
    process.ANEDBR.xsec=cms.double(33.21)
elif "ZZ"=="<SAMPLE>":
    process.ANEDBR.Ngen=cms.uint32(9799908)
    process.ANEDBR.xsec=cms.double(8.059)
    process.ANEDBR.FillGenLevelCode=cms.uint32(3)
elif "WW_xww"=="<SAMPLE>":
    process.ANEDBR.Ngen=cms.uint32(3870000)
    process.ANEDBR.xsec=cms.double(57.1097)
elif "WZ_xww"=="<SAMPLE>":
    process.ANEDBR.Ngen=cms.uint32(1910000)
    process.ANEDBR.xsec=cms.double(33.21)
elif "ZZ_xww"=="<SAMPLE>":
    process.ANEDBR.Ngen=cms.uint32(485716)
    process.ANEDBR.xsec=cms.double(8.059)
    process.ANEDBR.FillGenLevelCode=cms.uint32(3)
elif "WJetsPt50To70" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(24950166)
    process.ANEDBR.xsec=cms.double(1001.0)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)
elif "WJetsPt70To100" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(20916010)
    process.ANEDBR.xsec=cms.double(529.3)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)
elif "WJetsPt100" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(12106534)
    process.ANEDBR.xsec=cms.double(282.5)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)
elif "WJetsPt180" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9732165)
    process.ANEDBR.xsec=cms.double(29.00)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)
elif "SingleTopBarSchannel" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(139974)
    process.ANEDBR.xsec=cms.double(1.76)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)
elif "SingleTopBarTWchannel" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(493460)
    process.ANEDBR.xsec=cms.double(11.1)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)
elif "SingleTopBarTchannel" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(1904234)
    process.ANEDBR.xsec=cms.double(30.7)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)    
elif "SingleTopSchannel" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(209237)
    process.ANEDBR.xsec=cms.double(3.79)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)    
elif "SingleTopTWchannel" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(497658)
    process.ANEDBR.xsec=cms.double(11.1)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)    
elif "SingleTopTchannel" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(3750740)
    process.ANEDBR.xsec=cms.double(56.4)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)    
### MC signal
###### Bulk Graviton, ZZ, c = 1.0
elif "BulkG_ZZ_lljj_c1p0_M600" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(41965)
    process.ANEDBR.xsec=cms.double(0.0627) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c1p0_M1000" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(40964)
    process.ANEDBR.xsec=cms.double(0.00206) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c1p0_M1500" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(40938)
    process.ANEDBR.xsec=cms.double(0.000106) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
###### Bulk Graviton, ZZ, c = 0.2
elif "BulkG_ZZ_lljj_c0p2_M600" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(40789)
    process.ANEDBR.xsec=cms.double(0.00258) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M700" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10709)
    process.ANEDBR.xsec=cms.double(0.000927) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M800" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9359)
    process.ANEDBR.xsec=cms.double(0.000384) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M900" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9707)
    process.ANEDBR.xsec=cms.double(0.000174) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M1000" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(41997-4199) # Subtracting 4199 because one file isn't there
    process.ANEDBR.xsec=cms.double(8.51e-05) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M1100" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9358)
    process.ANEDBR.xsec=cms.double(4.40e-05) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M1200" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(40040)
    process.ANEDBR.xsec=cms.double(2.35e-05) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M1300" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(8360)
    process.ANEDBR.xsec=cms.double(1.31e-05) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M1400" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10709)
    process.ANEDBR.xsec=cms.double(7.43e-06) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M1500" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9356)
    process.ANEDBR.xsec=cms.double(4.35e-06) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M1600" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(40040)
    process.ANEDBR.xsec=cms.double(2.59e-06) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M1700" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(8009)
    process.ANEDBR.xsec=cms.double(1.57e-06) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M1800" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9358)
    process.ANEDBR.xsec=cms.double(9.65e-07) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M1900" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9355)
    process.ANEDBR.xsec=cms.double(6.03e-07) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M2000" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(40040)
    process.ANEDBR.xsec=cms.double(3.79e-07) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
###### RS Graviton, ZZ, c = 0.05
elif "RSG_ZZ_lljj_c0p05_M1000" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(38022)
    process.ANEDBR.xsec=cms.double(5.07e-03) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
###### RS Graviton, ZZ, c = 0.02
elif "RSG_ZZ_lljj_c0p2_M1000" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(37410)
    process.ANEDBR.xsec=cms.double(7.86e-02) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "RSG_ZZ_lljj_c0p2_M1500" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(24996)
    process.ANEDBR.xsec=cms.double(6.08e-3)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7) ### Checked
    
#ww
##RSG WW, c=0.2
elif "RSG_WW_lvjj_c0p2_M600" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(31785)
    process.ANEDBR.xsec=cms.double(8.42e+00) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "RSG_WW_lvjj_c0p2_M1000" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(34541)
    process.ANEDBR.xsec=cms.double(4.81e-01) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "RSG_WW_lvjj_c0p2_M1500" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(23879)
    process.ANEDBR.xsec=cms.double(3.69e-02) ### Checked
elif "RSG_WW_lvjj_c0p05_M1000" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(35421)
    process.ANEDBR.xsec=cms.double(3.11e-02) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
##Bulk WW, c=1.0
elif "BulkG_WW_lvjj_c1p0_M600" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(40371) ### Checked
    process.ANEDBR.xsec=cms.double(0.3675)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c1p0_M1000" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(40761) ### Checked
    process.ANEDBR.xsec=cms.double(0.01236)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c1p0_M1500" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(41136) ### Checked
    process.ANEDBR.xsec=cms.double(0.000646)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
##Bulk WW, c=0.2
elif "BulkG_WW_lvjj_c0p2_M600" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(48949) ### Checked
    process.ANEDBR.xsec=cms.double(0.015)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M700" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9832) ### Checked
    process.ANEDBR.xsec=cms.double(0.005478)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M800" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9506) ### Checked
    process.ANEDBR.xsec=cms.double(0.00228)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M900" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9506) ### Checked
    process.ANEDBR.xsec=cms.double(0.001048)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M1000" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(48796) ### Checked
    process.ANEDBR.xsec=cms.double(0.0005114)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M1100" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9833) ### Checked
    process.ANEDBR.xsec=cms.double(0.0002646)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M1200" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(44952) ### Checked
    process.ANEDBR.xsec=cms.double(0.00014120)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M1300" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9767) ### Checked
    process.ANEDBR.xsec=cms.double(7.9031e-05)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M1400" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9831) ### Checked
    process.ANEDBR.xsec=cms.double(4.5246e-05)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M1500" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(49996) ### Checked
    process.ANEDBR.xsec=cms.double(2.654e-05)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M1600" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(45994) ### Checked
    process.ANEDBR.xsec=cms.double(1.5771e-05)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M1700" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9899) ### Checked
    process.ANEDBR.xsec=cms.double(9.5695e-06)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M1800" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9833) ### Checked
    process.ANEDBR.xsec=cms.double(5.871e-06)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M1900" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9897) ### Checked
    process.ANEDBR.xsec=cms.double(3.6674e-06)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M2000" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(49360) ### Checked
    process.ANEDBR.xsec=cms.double(2.3073e-06)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M2100" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9900) ### Checked
    process.ANEDBR.xsec=cms.double(1.458e-06)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M2200" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9702) ### Checked
    process.ANEDBR.xsec=cms.double(9.399e-07)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M2300" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9899) ### Checked
    process.ANEDBR.xsec=cms.double(6.0353e-07)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M2400" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9899) ### Checked
    process.ANEDBR.xsec=cms.double(3.91e-07)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_lvjj_c0p2_M2500" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9898) ### Checked
    process.ANEDBR.xsec=cms.double(2.5515e-07)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)

### Data
#zz--doublemu
elif "<SAMPLE>"=="DoubleMu_Run2012A_13Jul2012" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="DoubleMu_Run2012A_recover" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="DoubleMu_Run2012B_13Jul2012" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="DoubleMu_Run2012C_24Aug2012" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="DoubleMu_Run2012C_PRv2" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="DoubleMu_Run2012D_PRv1" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
##zz--doubleEle
elif "<SAMPLE>"=="Photon_Run2012A_13Jul2012" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="Photon_Run2012A_recover" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="DoublePhotonHighPt_Run2012B_13Jul2012" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="DoublePhotonHighPt_Run2012C_24Aug2012" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="DoublePhotonHighPt_Run2012C_PRv2" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="DoublePhotonHighPt_Run2012D_PRv1" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
#ww--singlemu
elif "<SAMPLE>"=="SingleMu_Run2012A_13Jul2012_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleMu_Run2012A_recover_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleMu_Run2012B_13Jul2012_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleMu_Run2012C_24Aug2012_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleMu_Run2012C_PromptReco_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleMu_Run2012C_EcalRecove_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleMu_Run2012D_PromptReco_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
#ww---singleEle
elif "<SAMPLE>"=="SingleElectron_Run2012A_13Jul2012_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)  
elif "<SAMPLE>"=="SingleElectron_Run2012A_recover_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)   
elif "<SAMPLE>"=="SingleElectron_Run2012B_13Jul2012_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleElectron_Run2012C_24Aug2012_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleElectron_Run2012C_PromptReco_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleElectron_Run2012C_EcalRecove_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleElectron_Run2012D_PromptReco_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
   
    
else :
    print 'ERROR !!! Sample named <SAMPLE> was not recognized !'

print '---> Ngen=',process.ANEDBR.Ngen,'  Xsect=',process.ANEDBR.xsec



process.filterFinalSelPath = cms.EDFilter("HLTHighLevel",
                                       TriggerResultsTag = cms.InputTag("TriggerResults","","CMG"),
                                       #HLTPaths = cms.vstring("cmgEDBRWWEle","cmgEDBRWWMu"),
                                       HLTPaths = cms.vstring("cmgEDBRZZEle","cmgEDBRZZMu"),
                                       eventSetupPathsKey = cms.string(''),
                                       andOr = cms.bool(True),  # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
                                       throw = cms.bool(True)    # throw exception on unknown path names
                                       )

if processFullSel :
    process.p=cms.Path(process.filterFinalSelPath+process.ANEDBR)
else :
    process.p=cms.Path(process.ANEDBR)
