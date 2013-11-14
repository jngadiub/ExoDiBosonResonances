import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import *
process = cms.Process("EDBR")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string("START53_V23::All")
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
    VType=cms.string("W"),
    Ngen=cms.uint32(1),
    xsec=cms.double(1.0) ###in pb
    )


if "WW"=="<ANALYSIS>" :
    process.ANEDBR.VType = cms.string("W")
else :
    process.ANEDBR.VType = cms.string("Z")



### if false, use the default collections
### in ExoDiBosonResonances.EDBRCommon.analyzerEDBR_cfi
### (i.e. all the cands passing pre-selection cuts)
processFullSel='<PROCESS>'

if processFullSel == "fullsb" :
    process.ANEDBR.EDBREEJJColl=cms.InputTag("BestSidebandSelectorEle:doubleJet")
    process.ANEDBR.EDBRMMJJColl=cms.InputTag("BestSidebandSelectorMu:doubleJet")
    process.ANEDBR.EDBREEJColl=cms.InputTag("BestSidebandSelectorEle:singleJet")
    process.ANEDBR.EDBRMMJColl=cms.InputTag("BestSidebandSelectorMu:singleJet")
elif processFullSel == "fullsig" :
    process.ANEDBR.EDBREEJJColl=cms.InputTag("BestCandSelectorEle:doubleJet")
    process.ANEDBR.EDBRMMJJColl=cms.InputTag("BestCandSelectorMu:doubleJet")
    process.ANEDBR.EDBREEJColl=cms.InputTag("BestCandSelectorEle:singleJet")
    process.ANEDBR.EDBRMMJColl=cms.InputTag("BestCandSelectorMu:singleJet")
elif processFullSel == "fullrange" :
    process.ANEDBR.EDBREEJJColl=cms.InputTag("BestFullRangeSelectorEle:doubleJet")
    process.ANEDBR.EDBRMMJJColl=cms.InputTag("BestFullRangeSelectorMu:doubleJet")
    process.ANEDBR.EDBREEJColl=cms.InputTag("BestFullRangeSelectorEle:singleJet")
    process.ANEDBR.EDBRMMJColl=cms.InputTag("BestFullRangeSelectorMu:singleJet")  
elif processFullSel == "ttbarcontrol" :
    process.ANEDBR.EDBREEJJColl=cms.InputTag("BestTTBarSelectorEle:doubleJet")
    process.ANEDBR.EDBRMMJJColl=cms.InputTag("BestTTBarSelectorMu:doubleJet")
    process.ANEDBR.EDBREEJColl=cms.InputTag("BestTTBarSelectorEle:singleJet")
    process.ANEDBR.EDBRMMJColl=cms.InputTag("BestTTBarSelectorMu:singleJet")
else :
    print 'Processing preselected collections (default)'


##### set Ngen and xsect values ofr MC samples; xsect in pb !!! 
if "TTBAR"=="<SAMPLE>" or "TTBAR_xww"=="<SAMPLE>":
    process.ANEDBR.Ngen=cms.uint32(6540800)
    process.ANEDBR.xsec=cms.double(225.197)
elif "TTBARpowheg"=="<SAMPLE>" or "TTBARpowheg_xww"=="<SAMPLE>":
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
elif "WW"=="<SAMPLE>" or "WW_xww"=="<SAMPLE>":
    process.ANEDBR.Ngen=cms.uint32(10000431)
    process.ANEDBR.xsec=cms.double(57.1097)
elif "WZ"=="<SAMPLE>" or "WZ_xww"=="<SAMPLE>":
    process.ANEDBR.Ngen=cms.uint32(9955839)
    process.ANEDBR.xsec=cms.double(33.21)
elif "ZZ"=="<SAMPLE>" or "ZZ_xww"=="<SAMPLE>":
    process.ANEDBR.Ngen=cms.uint32(9799908)
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
    process.ANEDBR.Ngen=cms.uint32(12742382)
    process.ANEDBR.xsec=cms.double(282.5)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)
elif "WJetsPt180" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(9737030)
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
    process.ANEDBR.Ngen=cms.uint32(1935072)
    process.ANEDBR.xsec=cms.double(30.7)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)    
elif "SingleTopSchannel" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(259961)
    process.ANEDBR.xsec=cms.double(3.79)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)    
elif "SingleTopTWchannel" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(497658)
    process.ANEDBR.xsec=cms.double(11.1)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)    
elif "SingleTopTchannel" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(3758227)
    process.ANEDBR.xsec=cms.double(56.4)
    process.ANEDBR.FillGenLevelCode=cms.uint32(1)    
### MC signal
    ###### Special BulkG -> ZZ with different width or different Parton Shower
elif "<SAMPLE>" == "BulkG_ZZ_lljj_c0p2_M800-JHU-herwig" : 
    process.ANEDBR.Ngen=cms.uint32(8097)
    process.ANEDBR.xsec=cms.double(0.000384) 
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "<SAMPLE>" == "BulkG_ZZ_lljj_c0p2_M1100-JHU-herwig" :
    process.ANEDBR.Ngen=cms.uint32(7693)
    process.ANEDBR.xsec=cms.double(4.40e-05)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "<SAMPLE>" == "BulkG_ZZ_lljj_c0p2_M1400-JHU-herwig" :
    process.ANEDBR.Ngen=cms.uint32(8897)
    process.ANEDBR.xsec=cms.double(7.43e-06) 
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "<SAMPLE>" == "BulkG_ZZ_lljj_c0p2_M1800-JHU-herwig" :
    process.ANEDBR.Ngen=cms.uint32(9680)
    process.ANEDBR.xsec=cms.double(9.65e-07) 
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "<SAMPLE>" == "BulkG_ZZ_lljj_c0p2_M1000-JHU-HERWIGpp" : 
    process.ANEDBR.Ngen=cms.uint32(10500)
    process.ANEDBR.xsec=cms.double(8.51e-05) 
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "<SAMPLE>" == "BulkG_ZZ_lljj_c0p2_M1800-JHU-HERWIGpp" :
    process.ANEDBR.Ngen=cms.uint32(10210)
    process.ANEDBR.xsec=cms.double(9.65e-07)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "<SAMPLE>" == "BulkG_ZZ_lljj_c0p2_M2500-JHU-HERWIGpp" :
    process.ANEDBR.Ngen=cms.uint32(10710)
    process.ANEDBR.xsec=cms.double(4.1969e-08) 
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
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
elif "BulkG_ZZ_lljj_c0p2_M2100" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10499)
    process.ANEDBR.xsec=cms.double(2.396e-07) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M2200" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10706)
    process.ANEDBR.xsec=cms.double(1.542e-07) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M2300" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10710)
    process.ANEDBR.xsec=cms.double(9.891e-08) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_c0p2_M2400" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10709)
    process.ANEDBR.xsec=cms.double(6.4226e-08) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)    
elif "BulkG_ZZ_lljj_c0p2_M2500" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10078)
    process.ANEDBR.xsec=cms.double(4.1969e-08) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
###### Special BulkG -> ZZ with different width or different Parton Shower
elif "BulkG_ZZ_lljj_M700_G40" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10708)
    process.ANEDBR.xsec=cms.double(1) 
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_M700_G80" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10698)
    process.ANEDBR.xsec=cms.double(1) 
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_M700_G120" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10686)
    process.ANEDBR.xsec=cms.double(1) 
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_M1000_G40" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10700)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_M1000_G80" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(8022)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_M1000_G120" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10476)
    process.ANEDBR.xsec=cms.double(1) 
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)    
elif "BulkG_ZZ_lljj_M1500_G40" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10284)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_M1500_G80" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10692)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_M1500_G120" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(0)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_M2000_G40" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10701)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_M2000_G80" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10705)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_M2000_G120" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10690)
    process.ANEDBR.xsec=cms.double(1)
elif "BulkG_ZZ_lljj_M2000_G160" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10053)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_ZZ_lljj_M2000_G200" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10180)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)   
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)  
###### RS Graviton, PYTHIA 6, ZZ, c = 0.05
elif "<SAMPLE>"  == "RSG_ZZ_lljj_c0p05_M750_PYTHIA6"  :
    process.ANEDBR.Ngen=cms.uint32(60192)
    process.ANEDBR.xsec=cms.double(0.008968)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "<SAMPLE>"  == "RSG_ZZ_lljj_c0p05_M1000_PYTHIA6"  :
    process.ANEDBR.Ngen=cms.uint32(60192)
    process.ANEDBR.xsec=cms.double(0.002378)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "<SAMPLE>"  == "RSG_ZZ_lljj_c0p05_M1250_PYTHIA6"  :
    process.ANEDBR.Ngen=cms.uint32(60192)
    process.ANEDBR.xsec=cms.double(0.000778)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)    
elif "<SAMPLE>"  == "RSG_ZZ_lljj_c0p05_M1500_PYTHIA6"  :
    process.ANEDBR.Ngen=cms.uint32(60192)
    process.ANEDBR.xsec=cms.double(0.000276)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "<SAMPLE>"  == "RSG_ZZ_lljj_c0p05_M1750_PYTHIA6"  :
    process.ANEDBR.Ngen=cms.uint32(60192)
    process.ANEDBR.xsec=cms.double(0.000109)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "<SAMPLE>"  == "RSG_ZZ_lljj_c0p05_M2000_PYTHIA6"  :
    process.ANEDBR.Ngen=cms.uint32(60192)
    process.ANEDBR.xsec=cms.double(0.000044)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
###### RS Graviton, JHUgen, ZZ, c = 0.05
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
    
#ww (old, used for PAS)
##RSG WW, c=0.2
elif "RSG_WW_lvjj_c0p2_M600_xww"=="<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(27456)
    process.ANEDBR.xsec=cms.double(8.42e+00) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "RSG_WW_lvjj_c0p2_M1000_xww"=="<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(33186)
    process.ANEDBR.xsec=cms.double(4.81e-01) ### Checked
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "RSG_WW_lvjj_c0p2_M1500_xww"=="<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(23879)
    process.ANEDBR.xsec=cms.double(3.69e-02) ### Checked
elif "RSG_WW_lvjj_c0p05_M1000_xww"=="<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(27705)
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

#ww (new, used for PAPER)
# ww signal (exclusive W-->l v decays, despite of the name, since we use pattuples filtered at gen level) 
elif "BulkG_WW_inclusive_c0p2_M600_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11854) ### Checked
    process.ANEDBR.xsec=cms.double(2.292e-02)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M700_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(12164) ### Checked
    process.ANEDBR.xsec=cms.double(8.362e-03)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M800_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11998) ### Checked
    process.ANEDBR.xsec=cms.double(3.479e-03)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M900_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11940) ### Checked
    process.ANEDBR.xsec=cms.double(1.600e-03)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M1000_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11896) ### Checked
    process.ANEDBR.xsec=cms.double(7.806e-04)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)

elif "BulkG_WW_inclusive_M1000_W50_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11704) ### Checked
    process.ANEDBR.xsec=cms.double(7.806e-04) #assume same cross section of narrow sample
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_M1000_W150_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(10697) ### Checked
    process.ANEDBR.xsec=cms.double(7.806e-04) #assume same cross section of narrow sample
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_M1000_W300_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11722) ### Checked
    process.ANEDBR.xsec=cms.double(7.806e-04) #assume same cross section of narrow sample
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)

elif "BulkG_WW_inclusive_c0p2_M1100_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11933) ### Checked
    process.ANEDBR.xsec=cms.double(4.038e-04)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M1200_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(12020) ### Checked
    process.ANEDBR.xsec=cms.double(2.167e-04)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M1300_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(12099) ### Checked
    process.ANEDBR.xsec=cms.double(1.206e-04)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M1400_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(12048) ### Checked
    process.ANEDBR.xsec=cms.double(6.907e-05)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M1500_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11071) ### Checked
    process.ANEDBR.xsec=cms.double(4.051e-05)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)

elif "BulkG_WW_inclusive_M1500_W75_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11979) ### Checked
    process.ANEDBR.xsec=cms.double(4.051e-05) #assume same cross section of narrow sample
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_M1500_W225_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11728) ### Checked
    process.ANEDBR.xsec=cms.double(4.051e-05) #assume same cross section of narrow sample
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_M1500_W450_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11776) ### Checked
    process.ANEDBR.xsec=cms.double(4.051e-05) #assume same cross section of narrow sample
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)

elif "BulkG_WW_inclusive_c0p2_M1600_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11902) ### Checked
    process.ANEDBR.xsec=cms.double(2.407e-05)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M1700_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(12129) ### Checked
    process.ANEDBR.xsec=cms.double(1.461e-05)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M1800_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(8776) ### Checked
    process.ANEDBR.xsec=cms.double(8.961e-06)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M1900_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11380) ### Checked
    process.ANEDBR.xsec=cms.double(5.598e-06)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M2000_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11805) ### Checked
    process.ANEDBR.xsec=cms.double(3.522e-06)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M2100_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(12081) ### Checked
    process.ANEDBR.xsec=cms.double(2.225e-06)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)

elif "BulkG_WW_inclusive_M2100_W105_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11993) ### Checked
    process.ANEDBR.xsec=cms.double(2.225e-06) #assume same cross section of narrow sample
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_M2100_W315_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11979) ### Checked
    process.ANEDBR.xsec=cms.double(2.225e-06) #assume same cross section of narrow sample
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_M2100_W630_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11082) ### Checked
    process.ANEDBR.xsec=cms.double(2.225e-06) #assume same cross section of narrow sample
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)

elif "BulkG_WW_inclusive_c0p2_M2200_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11688) ### Checked
    process.ANEDBR.xsec=cms.double(1.435e-06)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M2300_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11947) ### Checked
    process.ANEDBR.xsec=cms.double(9.213e-07)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M2400_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(12055) ### Checked
    process.ANEDBR.xsec=cms.double(5.969e-07)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "BulkG_WW_inclusive_c0p2_M2500_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(11059) ### Checked
    process.ANEDBR.xsec=cms.double(3.895e-07)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
###### W' => WZ, PYTHIA 6, inclusive
elif "WprimeWZ_inclusive_M750_PYTHIA6" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(30000)
    process.ANEDBR.xsec=cms.double(0.727785)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "WprimeWZ_inclusive_M1000_PYTHIA6" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(30000)
    process.ANEDBR.xsec=cms.double(0.1958263)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "WprimeWZ_inclusive_M1500_PYTHIA6" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(30000)
    process.ANEDBR.xsec=cms.double(0.0234412)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "WprimeWZ_inclusive_M1800_PYTHIA6" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(30000)
    process.ANEDBR.xsec=cms.double(0.0077633)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "WprimeWZ_inclusive_M2000_PYTHIA6" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(30000)
    process.ANEDBR.xsec=cms.double(0.0039031)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "WprimeWZ_inclusive_M2200_PYTHIA6" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(30000)
    process.ANEDBR.xsec=cms.double(0.001977)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "WprimeWZ_inclusive_M2500_PYTHIA6" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(30000)
    process.ANEDBR.xsec=cms.double(0.000773)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "WprimeWZ_inclusive_M3000_PYTHIA6" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(30000)
    process.ANEDBR.xsec=cms.double(0.000218)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)

### RSG WW new sample
elif "RSG_WW_lvjj_c0p05_M750_PYTHIA6_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(60192)
    process.ANEDBR.xsec=cms.double(0.054381)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "RSG_WW_lvjj_c0p05_M1000_PYTHIA6_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(60192)
    process.ANEDBR.xsec=cms.double(0.014457)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "RSG_WW_lvjj_c0p05_M1250_PYTHIA6_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(60192)
    process.ANEDBR.xsec=cms.double(0.004749)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "RSG_WW_lvjj_c0p05_M1500_PYTHIA6_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(60192)
    process.ANEDBR.xsec=cms.double(0.001681)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "RSG_WW_lvjj_c0p05_M1750_PYTHIA6_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(54173)
    process.ANEDBR.xsec=cms.double(0.000669)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)
elif "RSG_WW_lvjj_c0p05_M2000_PYTHIA6_xww" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(60192)
    process.ANEDBR.xsec=cms.double(0.000265)
    process.ANEDBR.FillGenLevelCode=cms.uint32(7)


### Data
#zz--doublemu
elif "<SAMPLE>"=="DoubleMu_Run2012A_22Jan2013" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "DoubleMuParked_Run2012" in "<SAMPLE>" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
##zz--doubleEle
elif "<SAMPLE>"=="Photon_Run2012A_22Jan2013" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "DoublePhotonHighPt_Run2012" in  "<SAMPLE>" :
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
   
#for 539
#single mu
elif "<SAMPLE>"=="SingleMu_Run2012A-22Jan2013_xww_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleMu_Run2012B-22Jan2013_xww_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleMu_Run2012C-22Jan2013_xww_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleMu_Run2012D-22Jan2013_xww_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
#single ele
elif "<SAMPLE>"=="SingleElectron_Run2012A-22Jan2013_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleElectron_Run2012B-22Jan2013_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleElectron_Run2012C-22Jan2013_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)
elif "<SAMPLE>"=="SingleElectron_Run2012D-22Jan2013_xww" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
    process.ANEDBR.isMC=cms.bool(False)

else :
    print 'ERROR !!! Sample named <SAMPLE> was not recognized !'

print '---> Ngen=',process.ANEDBR.Ngen,'  Xsect=',process.ANEDBR.xsec


###apply V-tagging scale factor only to signal MC samples
if "BulkG_ZZ_lljj_" in "<SAMPLE>" or "RSG_ZZ_lljj_" in "<SAMPLE>" or "WprimeWZ_" in "<SAMPLE>" or "WW"=="<SAMPLE>" or "WZ"=="<SAMPLE>" or "ZZ"=="<SAMPLE>":
       process.ANEDBR.VTaggingScaleFactorHP=cms.double(0.891)
       process.ANEDBR.VTaggingScaleFactorLP=cms.double(1.277)


       
if "BulkG_WW_inclusive_" in "<SAMPLE>" or "RSG_WW_lvjj_" in "<SAMPLE>"  or "WprimeWZ_" in "<SAMPLE>" or "WW_xww"=="<SAMPLE>" or "WZ_xww"=="<SAMPLE>" or "ZZ_xww"=="<SAMPLE>":
       process.ANEDBR.VTaggingScaleFactorHP=cms.double(0.891)
       process.ANEDBR.VTaggingScaleFactorLP=cms.double(1.277)

process.filterFinalSelPath = cms.EDFilter("HLTHighLevel",
                                          TriggerResultsTag = cms.InputTag("TriggerResults","","CMG"),
                                          HLTPaths = cms.vstring("FullPathDUMMYELE","FullPathDUMMYMU"),
                                          #HLTPaths = cms.vstring("cmgEDBRZZEle","cmgEDBRZZMu"),
                                          eventSetupPathsKey = cms.string(''),
                                          andOr = cms.bool(True),  # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
                                          throw = cms.bool(True)    # throw exception on unknown path names
                                          )

if "WW"=="<ANALYSIS>" :
    process.filterFinalSelPath.HLTPaths = cms.vstring("cmgEDBRWWEle","cmgEDBRWWMu")
else :
    process.filterFinalSelPath.HLTPaths = cms.vstring("cmgEDBRZZEle","cmgEDBRZZMu")
        

#AB: should we add also "ttbarcontrol" ? LS: yes
if processFullSel=="fullsb" or processFullSel=="fullsig" or processFullSel=="fullrange" or processFullSel=="ttbarcontrol":
    process.p=cms.Path(process.filterFinalSelPath+process.ANEDBR)
else :
    process.p=cms.Path(process.ANEDBR)
