import FWCore.ParameterSet.Config as cms
import sys

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

myOptions = sys.argv
skipEvents = 0
numEvents = -1
useData = True
inputFileName = ''

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

outputFileName = 'histograms_'+inputFileName

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

process.genGravitons = cms.EDFilter("PdgIdAndStatusCandViewSelector",
                                    src = cms.InputTag("genParticles"),
                                    pdgId = cms.vint32( 39 ),
                                    status = cms.vint32( 3 ),
                                    filter = cms.bool(False)
                                    )

process.plotGravitons = cms.EDAnalyzer("CandViewHistoAnalyzer",
                                       src = cms.InputTag("genGravitons"),
                                       histograms = XHistos
                                       )

process.genPlots = cms.Sequence(process.genGravitons + process.plotGravitons)

## Z-->ee/mumu filter
EE = ("patElectronsWithTrigger patElectronsWithTrigger")
MUMU = ("patMuonsWithTrigger patMuonsWithTrigger")
DILEPTON_KINCUT = ("70.0 < mass < 110.0 && pt > 5.0")
HADV_KINCUT = ("60.0 < mass < 110.0 && pt > 5.0")

## ZToEE
process.ZToEEcand = cms.EDProducer("CandViewShallowCloneCombiner",
                                   decay = cms.string(EE),
                                   checkCharge = cms.bool(False),                                   
                                   cut = cms.string(DILEPTON_KINCUT)
                                   )

process.ZToEEfilter = cms.EDFilter("CandViewCountFilter",
                                   src = cms.InputTag("ZToEEcand"),
                                   minNumber = cms.uint32(1),
                                   filter = cms.bool(True)
                                   )

process.leadingZ = cms.EDFilter("LargestPtCandViewSelector",
                                src = cms.InputTag("ZToEEcand"),
                                maxNumber = cms.uint32(1)
                                )

process.ZToEEskimSequence = cms.Sequence( process.ZToEEcand * process.ZToEEfilter + process.leadingZ)

### Jets
process.jetsNoZ =  cms.EDProducer("PATJetCleaner",
                                  ## pat jet input source
                                  src = cms.InputTag("selectedPatJetsCHSpruned"),
                                  # preselection (any string-based cut for pat::Electron)
                                  preselection = cms.string(''),
                                  # overlap checking configurables
                                  checkOverlaps = cms.PSet(Zbosons = cms.PSet(
                                      src       = cms.InputTag("patElectronsWithTrigger"),
                                      algorithm = cms.string("byDeltaR"),
                                      preselection        = cms.string(""),  # don't preselect the Zbosons
                                      deltaR              = cms.double(0.5),
                                      checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                      pairCut             = cms.string(""),
                                      requireNoOverlaps = cms.bool(True), # overlaps CAUSE the jet to be discared
                                      )
                                                           ),
                                  finalCut = cms.string('(pt > 30.0) && (eta < 2.4)'),
                                  )

process.jetIdCut = cms.EDFilter("PATJetSelector",
                                src = cms.InputTag("selectedPatJetsCHSpruned"),
                                cut = cms.string("(neutralHadronEnergyFraction < 0.99) &&"+
                                                 "(neutralEmEnergyFraction < 0.99) &&"+
                                                 "(numberOfDaughters > 1) &&"+
                                                 "(chargedHadronEnergyFraction > 0) &&"+
                                                 "(chargedMultiplicity > 0) &&"+
                                                 "(chargedEmEnergyFraction < 0.99)"
                                                 )
                                )

process.massiveJets = cms.EDFilter("CandViewSelector",
                                   src = cms.InputTag("jetsNoZ"),
                                   cut = cms.string(HADV_KINCUT),
                                   filter = cms.bool(True)
                                   )

process.leadingJet = cms.EDFilter("LargestPtCandViewSelector",
                                  src = cms.InputTag("massiveJets"),
                                  maxNumber = cms.uint32(1)
                                  )

process.orderedJets = cms.EDFilter("LargestPtCandViewSelector",
                                  src = cms.InputTag("jetsNoZ"),
                                  maxNumber = cms.uint32(9999)
                                  )

process.ZToJJcand = cms.EDProducer("HadronicBosonProducer",
                                   jets = cms.InputTag("orderedJets"),
                                   )

process.goodZToJJcand = cms.EDFilter("CandViewSelector",
                                     src = cms.InputTag("ZToJJcand"),
                                     cut = cms.string(HADV_KINCUT),
                                     filter = cms.bool(True)
                                     )

process.leadingZhad = cms.EDFilter("LargestPtCandViewSelector",
                                   src = cms.InputTag("ZToJJcand"),
                                   maxNumber = cms.uint32(1)
                                   )

process.ZToJetsskimSequence = cms.Sequence( process.jetsNoZ + cms.ignore(process.massiveJets) + process.leadingJet +
                                            process.orderedJets + process.ZToJJcand + cms.ignore(process.goodZToJJcand) + process.leadingZhad)
                                   
                                   
### ZZCand
process.ZZcandLJ = cms.EDProducer("CandViewShallowCloneCombiner",
                                  decay = cms.string("leadingZ leadingZhad"),
                                  checkCharge = cms.bool(False),                                   
                                  cut = cms.string("")
                                  )

process.ZZcandAJ = cms.EDProducer("CandViewShallowCloneCombiner",
                                  decay = cms.string("leadingZ leadingJet"),
                                  checkCharge = cms.bool(False),                                   
                                  cut = cms.string("")
                                  )

process.ZZcandskimSequence = cms.Sequence(process.ZZcandLJ + process.ZZcandAJ)

### Plots
process.plotJets = cms.EDAnalyzer("CandViewHistoAnalyzer",
                                  src = cms.InputTag("leadingJet"),
                                  histograms = jetHistos
                                  )

process.plotZHads = cms.EDAnalyzer("CandViewHistoAnalyzer",
                                   src = cms.InputTag("leadingZhad"),
                                   histograms = jetHistos
                                   )

process.plotElectrons = cms.EDAnalyzer("CandViewHistoAnalyzer",
                                       src = cms.InputTag("patElectronsWithTrigger"),
                                       histograms = leptonHistos
                                       )

process.plotZs = cms.EDAnalyzer("CandViewHistoAnalyzer",
                                src = cms.InputTag("ZToEEcand"),
                                histograms = ZHistos
                                )

process.plotGLJs = cms.EDAnalyzer("CandViewHistoAnalyzer",
                                  src = cms.InputTag("ZZcandLJ"),
                                  histograms = XHistos
                                  )

process.plotGAJs = cms.EDAnalyzer("CandViewHistoAnalyzer",
                                  src = cms.InputTag("ZZcandAJ"),
                                  histograms = XHistos
                                  )

process.hadronicBosonStudy = cms.EDAnalyzer("HadronicBosonStudy",
                                            hadronicBosons = cms.InputTag("ZToJJcand")
                                            )

process.plottingSequence = cms.Sequence(process.plotJets + process.plotZHads +
                                        process.plotElectrons + process.plotZs +
                                        process.plotGLJs + process.plotGAJs)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(outputFileName)
#                                   fileName = cms.string("histograms.root")
                                   )
 
process.p = cms.Path(process.genElectrons +
                     process.genPlots +
                     process.ZToEEskimSequence +
#                     process.jetIdCut + 
                     process.ZToJetsskimSequence +
                     process.ZZcandskimSequence +
                     process.hadronicBosonStudy +
                     process.plottingSequence
                     )
