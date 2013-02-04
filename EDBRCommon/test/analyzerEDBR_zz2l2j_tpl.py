import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import *
process = cms.Process("EDBR")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string("START53_V7A::All")
process.load("Configuration.StandardSequences.MagneticField_cff")


### input cmgTuples
process.load("ExoDiBosonResonances.EDBRCommon.datasets.cmgTuple_<SAMPLE>_cff")

## source = cms.Source("PoolSource",
##                     noEventSort = cms.untracked.bool(True),
##                     duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
##                     fileNames = cms.vstring(
##     '/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1/Summer12/presel/TTBAR/test_0.root')
##                     )

#process.load("ExoDiBosonResonances.EDBRCommon.analyzerEDBR_cfi.py")
from ExoDiBosonResonances.EDBRCommon.analyzerEDBR_cfi import AnalyzerXZZ
process.ANEDBR = AnalyzerXZZ.clone(
    debug=cms.bool(False),
    outFileName=cms.string("treeEDBR_<SAMPLE>.root"),
    Ngen=cms.uint32(1),
    xsec=cms.double(1.0) ###in pb
    )

if "<SAMPLE>"=="TTBAR" :
    process.ANEDBR.Ngen=cms.uint32(6540800)
    process.ANEDBR.xsec=cms.double(225.197)
elif "<SAMPLE>"=="DYJetsPt50To70" :
    process.ANEDBR.Ngen=cms.uint32(4823000)
    process.ANEDBR.xsec=cms.double(89.0)
elif "<SAMPLE>"=="DYJetsPt70To100" :
    process.ANEDBR.Ngen=cms.uint32(4000000)
    process.ANEDBR.xsec=cms.double(53.0)
elif "<SAMPLE>"=="DYJetsPt100" :
    process.ANEDBR.Ngen=cms.uint32(2500000)
    process.ANEDBR.xsec=cms.double(32.9)
elif "<SAMPLE>"=="DoubleMu_Run2012A_13Jul2012" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
elif "<SAMPLE>"=="DoubleMu_Run2012A_recover" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
elif "<SAMPLE>"=="DoubleMu_Run2012B_13Jul2012" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
elif "<SAMPLE>"=="DoubleMu_Run2012C_24Aug2012" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
elif "<SAMPLE>"=="DoubleMu_Run2012C_PRv1" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
elif "<SAMPLE>"=="DoubleMu_Run2012D_PRv1" :
    process.ANEDBR.Ngen=cms.uint32(1)
    process.ANEDBR.xsec=cms.double(1)
else :
    print 'ERROR !!! Sample named <SAMPLE> was not recognized !'

print '---> Ngen=',process.ANEDBR.Ngen,'  Xsect=',process.ANEDBR.xsec

process.p=cms.Path(process.ANEDBR)
