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

outputFileName = 'histograms'+suffix+inputFileName

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(numEvents) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        "file:"+inputFileName
    )
)

### Histograms
from ExoDiBosonResonances.GeneratorStudies.simpleJetHistos_cff import histograms as jetHistos
from ExoDiBosonResonances.GeneratorStudies.simpleLeptonHistos_cff import histograms as leptonHistos
from ExoDiBosonResonances.GeneratorStudies.simpleZhistos_cff import histograms as ZHistos
from ExoDiBosonResonances.GeneratorStudies.simpleXhistos_cff import histograms as XHistos
from ExoDiBosonResonances.GeneratorStudies.simpleMETHistos_cff import histograms as METhistos

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

process.hptmAnalyzer = cms.EDAnalyzer("HighPtMuonStudy",
                                      muons=cms.InputTag(ANALYSIS_SOURCE),
                                      singleMuonID = cms.string("TRACKER"),
                                      dimuonID = cms.string("HPTTRK"),
                                      )

process.isoAnalyzer = cms.EDAnalyzer("IsolationMuonStudy",
                                      muons=cms.InputTag(ANALYSIS_SOURCE),
                                     )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(outputFileName)
                                   )
 
process.p = cms.Path(process.genMuons +
                     #                         process.highPtMuons +
                     process.hptmAnalyzer +
                     process.isoAnalyzer)
