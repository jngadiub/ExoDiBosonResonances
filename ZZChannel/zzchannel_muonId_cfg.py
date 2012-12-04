import FWCore.ParameterSet.Config as cms
import sys

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

myOptions = sys.argv
skipEvents = 0
numEvents = -1
useData = True
inputFileName = ''
suffix = ''

if 'skipEvents' in myOptions:
    skipEvents = int(myOptions[myOptions.index('skipEvents')+1])
if 'numEvents' in myOptions:
    numEvents = int(myOptions[myOptions.index('numEvents')+1])
if 'useData' in myOptions:
    x = myOptions[myOptions.index('useData')+1]
    if x == 'False':
        useData = False
    if x == 'True':
        useData = True
if 'input' in myOptions:
    inputFileName = myOptions[myOptions.index('input')+1]
if 'suffix' in myOptions:
    suffix = '_'+myOptions[myOptions.index('suffix')+1]+'_'

outputFileName = 'histograms'+suffix+'.root '

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(numEvents) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring("file:"+inputFileName)
                            )   

process.source.fileNames = cms.untracked.vstring(
    "/store/mc/Summer12_DR53X/RSGravitonToZZToLLNuNu_kMpl005_M-1000_TuneZ2star_8TeV-pythia6-tauola/AODSIM/PU_S10_START53_V7A-v1/00000/58267D63-C30D-E211-96D5-1CC1DE04DF70.root",
    "/store/mc/Summer12_DR53X/RSGravitonToZZToLLNuNu_kMpl005_M-1000_TuneZ2star_8TeV-pythia6-tauola/AODSIM/PU_S10_START53_V7A-v1/00000/78F0A3A9-C60D-E211-8846-1CC1DE1D03EA.root",
    "/store/mc/Summer12_DR53X/RSGravitonToZZToLLNuNu_kMpl005_M-1000_TuneZ2star_8TeV-pythia6-tauola/AODSIM/PU_S10_START53_V7A-v1/00000/2035357C-CB0D-E211-88BC-00266CFFBE14.root",
    "/store/mc/Summer12_DR53X/RSGravitonToZZToLLNuNu_kMpl005_M-1000_TuneZ2star_8TeV-pythia6-tauola/AODSIM/PU_S10_START53_V7A-v1/00000/8AAE6874-D00D-E211-A92C-0025B3E022C2.root",
    "/store/mc/Summer12_DR53X/RSGravitonToZZToLLNuNu_kMpl005_M-1000_TuneZ2star_8TeV-pythia6-tauola/AODSIM/PU_S10_START53_V7A-v1/00000/4E9985CD-CE0D-E211-ADD8-1CC1DE04FF50.root",
    "/store/mc/Summer12_DR53X/RSGravitonToZZToLLNuNu_kMpl005_M-1000_TuneZ2star_8TeV-pythia6-tauola/AODSIM/PU_S10_START53_V7A-v1/00000/2E7F3D3D-D20D-E211-8E5C-1CC1DE04DF70.root",
    "/store/mc/Summer12_DR53X/RSGravitonToZZToLLNuNu_kMpl005_M-1000_TuneZ2star_8TeV-pythia6-tauola/AODSIM/PU_S10_START53_V7A-v1/00000/AE870704-CD0D-E211-8CF5-1CC1DE1CE128.root"
    )

## For gen-level comparison
process.genElectrons = cms.EDFilter("PdgIdAndStatusCandViewSelector",
                                    src = cms.InputTag("genParticles"),
                                    pdgId = cms.vint32( 11 ),
                                    status = cms.vint32( 3 ),
                                    filter = cms.bool(True)
                                    )

process.genMuons = cms.EDFilter("PdgIdAndStatusCandViewSelector",
                                src = cms.InputTag("genParticles"),
                                pdgId = cms.vint32( 13 ),
                                status = cms.vint32( 3 ),
                                filter = cms.bool(True)
                                )

process.genGravitons = cms.EDFilter("PdgIdAndStatusCandViewSelector",
                                    src = cms.InputTag("genParticles"),
                                    pdgId = cms.vint32( 39 ),
                                    status = cms.vint32( 3 ),
                                    filter = cms.bool(False)
                                    )

## ZToMM
process.highPtMuons = cms.EDFilter("PATMuonSelector",
                                   src = cms.InputTag("patMuonsWithTrigger"),
                                   cut = cms.string("pt > 100.0"),
                                   filter = cms.bool(True)
                                   )

ANALYSIS_SOURCE = "highPtMuons"
ANALYSIS_SOURCE = "patMuonsWithTrigger"
ANALYSIS_SOURCE = "muons"

process.hptmAnalyzer = cms.EDAnalyzer("HighPtMuonStudy",
                                      muons=cms.InputTag(ANALYSIS_SOURCE),
                                      singleMuonID = cms.string("TRACKER"),
                                      dimuonID = cms.string("HPTTRK"),
                                      )

process.isoAnalyzer = cms.EDAnalyzer("IsolationMuonStudy",
                                      muons=cms.InputTag(ANALYSIS_SOURCE),
                                     )


process.finalAnalyzer = cms.EDAnalyzer("FinalMuonStudy",
                                      muons=cms.InputTag(ANALYSIS_SOURCE),
                                     )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(outputFileName)
                                   )
 
process.p = cms.Path(process.genMuons +
                     #                         process.highPtMuons +
                     #                     process.hptmAnalyzer +
                     #                     process.isoAnalyzer +
                     process.finalAnalyzer
                     )
