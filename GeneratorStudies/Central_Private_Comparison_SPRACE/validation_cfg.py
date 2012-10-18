import FWCore.ParameterSet.Config as cms

process = cms.Process("ANALYSIS")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(True)
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(28850) )

### GEN

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.prunedGenParticles = cms.EDProducer("GenParticlePruner",
                                            src = cms.InputTag("genParticles"),
                                            select = cms.vstring("drop  *  ", # this is the default
                                                                 "keep+ pdgId = {Z0}",
                                                                 "drop pdgId = {Z0} & status = 2"
                                                                 )
                                            )

process.printTree = cms.EDAnalyzer("ParticleListDrawer",
                                   maxEventsToPrint = cms.untracked.int32(5),
                                   printVertex = cms.untracked.bool(False),
                                   src = cms.InputTag("prunedGenParticles")
                                   )

process.checkFlavorUniversality = cms.EDFilter("VCheckFlavour",
                                               src = cms.untracked.InputTag("prunedGenParticles")
                                               )

process.leptonicDecay = cms.EDFilter("PdgIdAndStatusCandViewSelector",
                                     src = cms.InputTag("genParticles"),
                                     pdgId = cms.vint32( 11, 13 ),
                                     status = cms.vint32( 3 ),
                                     filter = cms.bool(True)
                                     )

### RECO

process.goodPV = cms.EDFilter("VertexSelector",
                              src = cms.InputTag("offlinePrimaryVertices"),
                              cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
                              filter = cms.bool(False)
                              )

process.countVertices = cms.EDFilter("CountPrimaryVertices",
                                     vertices = cms.untracked.InputTag("offlinePrimaryVertices")
                                     )

from ExoDiBosonResonances.GeneratorStudies.simpleJetHistos_cff import histograms as jetHistos
from ExoDiBosonResonances.GeneratorStudies.simpleLeptonHistos_cff import histograms as leptonHistos
from ExoDiBosonResonances.GeneratorStudies.simpleZhistos_cff import histograms as ZHistos
from ExoDiBosonResonances.GeneratorStudies.simpleMETHistos_cff import histograms as METhistos

process.jetSelection = cms.EDFilter("CandViewSelector",
                                    src = cms.InputTag("ak7PFJets"),
                                    cut = cms.string("(pt > 30 && (abs(eta) < 2.4))"),
                                    minNumber = cms.int32(1),
                                    filter = cms.bool(False)
                                    )

process.getHardJets = cms.EDFilter("LargestPtCandViewSelector",
                                   src = cms.InputTag("jetSelection"),
                                   maxNumber = cms.uint32(9999)
                                   )

process.muonSelection = cms.EDFilter("CandViewSelector",
                                    src = cms.InputTag("muons"),
                                    cut = cms.string("(pt > 20 && (abs(eta) < 2.1))"),
                                    minNumber = cms.int32(1),
                                    filter = cms.bool(False)
                                    )

process.getHardMuons = cms.EDFilter("LargestPtCandViewSelector",
                                   src = cms.InputTag("muonSelection"),
                                   maxNumber = cms.uint32(9999)
                                   )

process.electronSelection = cms.EDFilter("CandViewSelector",
                                    src = cms.InputTag("gsfElectrons"),
                                    cut = cms.string("(pt > 20 && (abs(eta) < 2.4))"),
                                    minNumber = cms.int32(1),
                                    filter = cms.bool(False)
                                    )

process.getHardElectrons = cms.EDFilter("LargestPtCandViewSelector",
                                        src = cms.InputTag("electronSelection"),
                                        maxNumber = cms.uint32(9999)
                                        )

### Z candidates
process.ZFromElectrons = cms.EDProducer("CandViewCombiner",
                                    decay = cms.string("getHardElectrons@+ getHardElectrons@-"),
                                    cut = cms.string("50.0 < mass < 150.0")
                                    )

process.ZFromMuons = cms.EDProducer("CandViewCombiner",
                                        decay = cms.string("getHardMuons@+ getHardMuons@-"),
                                        cut = cms.string("50.0 < mass < 150.0")
                                        )

process.allZ = cms.EDProducer("CandViewMerger",
                              src = cms.VInputTag( "ZFromMuons", "ZFromElectrons")
                              )

### PLOTS
process.plotJets = cms.EDAnalyzer("CandViewHistoAnalyzer",
                                  src = cms.InputTag("getHardJets"),
                                  histograms = jetHistos
                                  )

process.plotMuons = cms.EDAnalyzer("CandViewHistoAnalyzer",
                                   src = cms.InputTag("getHardMuons"),
                                   histograms = leptonHistos
                                   )

process.plotElectrons = cms.EDAnalyzer("CandViewHistoAnalyzer",
                                 src = cms.InputTag("getHardElectrons"),
                                 histograms = leptonHistos
                                 )

process.plotZs = cms.EDAnalyzer("CandViewHistoAnalyzer",
                                src = cms.InputTag("allZ"),
                                histograms = ZHistos
                                )

process.plotMET = cms.EDAnalyzer("CandViewHistoAnalyzer",
                                 src = cms.InputTag("pfMet"),
                                 histograms = METhistos
                                 )

### SOURCE
# This fragment comes preloaded with our private files for valdiation

process.load("ExoDiBosonResonances/GeneratorStudies.source_Pythia6_RS2000_ZZ_AODSIM")

# These are the files from official production
officialFiles = cms.untracked.vstring("/store/mc/Summer12_DR53X/RSGravitonToZZ_kMpl01_M-2000_TuneZ2star_8TeV-pythia6/AODSIM/PU_S10_START53_V7A-v1/0000/72A83B17-3EF8-E111-BA45-00215E93F080.root",
                                      "/store/mc/Summer12_DR53X/RSGravitonToZZ_kMpl01_M-2000_TuneZ2star_8TeV-pythia6/AODSIM/PU_S10_START53_V7A-v1/0000/A891748B-35F8-E111-9E77-00215E93D738.root",
                                      "/store/mc/Summer12_DR53X/RSGravitonToZZ_kMpl01_M-2000_TuneZ2star_8TeV-pythia6/AODSIM/PU_S10_START53_V7A-v1/0000/2EF4E83B-37F8-E111-A41C-E41F13181A88.root",
                                      "/store/mc/Summer12_DR53X/RSGravitonToZZ_kMpl01_M-2000_TuneZ2star_8TeV-pythia6/AODSIM/PU_S10_START53_V7A-v1/0000/3A1443B3-39F8-E111-BE04-00215E221680.root"
                                      )

# To change to official, just do
process.source.fileNames = officialFiles
 
### OUTPUT
OUTPUT_FILE_NAME = "official_test.root"
process.TFileService = cms.Service("TFileService", fileName = cms.string( OUTPUT_FILE_NAME ))

### PATHS
#process.p = cms.Path(process.prunedGenParticles + process.printTree)
process.genValidation  = cms.Path(process.prunedGenParticles + process.checkFlavorUniversality)
process.recoValidation = cms.Path(#process.leptonicDecay +
                                  process.goodPV +
                                  process.countVertices +
                                  process.jetSelection + process.getHardJets +
                                  process.muonSelection + process.getHardMuons +
                                  process.electronSelection + process.getHardElectrons +
                                  process.ZFromMuons + process.ZFromElectrons + process.allZ +
                                  process.plotJets + process.plotMuons + process.plotElectrons + process.plotZs + process.plotMET
                                )
