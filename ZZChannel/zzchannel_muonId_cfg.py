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
"/store/mc/Summer12_DR53X/RSGravitonToZZToLLNuNu_kMpl005_M-2000_TuneZ2star_8TeV-pythia6-tauola/AODSIM/PU_S10_START53_V7A-v1/00000/C8A95927-8E0E-E211-9878-00266CFFBF68.root",
"/store/mc/Summer12_DR53X/RSGravitonToZZToLLNuNu_kMpl005_M-2000_TuneZ2star_8TeV-pythia6-tauola/AODSIM/PU_S10_START53_V7A-v1/00000/8655772B-850E-E211-BA84-1CC1DE1CE170.root",
"/store/mc/Summer12_DR53X/RSGravitonToZZToLLNuNu_kMpl005_M-2000_TuneZ2star_8TeV-pythia6-tauola/AODSIM/PU_S10_START53_V7A-v1/00000/7062F4BD-7E0E-E211-820C-1CC1DE1D1F80.root",
"/store/mc/Summer12_DR53X/RSGravitonToZZToLLNuNu_kMpl005_M-2000_TuneZ2star_8TeV-pythia6-tauola/AODSIM/PU_S10_START53_V7A-v1/00000/121923F3-800E-E211-90F1-78E7D1E49636.root",
"/store/mc/Summer12_DR53X/RSGravitonToZZToLLNuNu_kMpl005_M-2000_TuneZ2star_8TeV-pythia6-tauola/AODSIM/PU_S10_START53_V7A-v1/00000/D6C415F8-870E-E211-8869-00237DA12CA0.root",
"/store/mc/Summer12_DR53X/RSGravitonToZZToLLNuNu_kMpl005_M-2000_TuneZ2star_8TeV-pythia6-tauola/AODSIM/PU_S10_START53_V7A-v1/00000/5CBC1DA2-710E-E211-8DBF-0017A4770430.root",
"/store/mc/Summer12_DR53X/RSGravitonToZZToLLNuNu_kMpl005_M-2000_TuneZ2star_8TeV-pythia6-tauola/AODSIM/PU_S10_START53_V7A-v1/00000/166E6D6F-890E-E211-A395-1CC1DE051118.root"    )

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

process.otherMuons = cms.EDFilter("CandViewSelector",
                                  src = cms.InputTag("genParticles"),
                                  cut = cms.string("pt > 5 & abs(pdgId) == 13 & status == 1"),
                                  filter = cms.bool(True)
                                  )

process.hptPhotons = cms.EDFilter("CandViewSelector",
                                  src = cms.InputTag("genParticles"),
                                  cut = cms.string("pt > 20 & abs(pdgId) == 22 & status == 1"),
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
                     process.otherMuons + 
                     process.hptPhotons + 
                     #                         process.highPtMuons +
                     #                     process.hptmAnalyzer +
                     #                     process.isoAnalyzer +
                     process.finalAnalyzer
                     )
