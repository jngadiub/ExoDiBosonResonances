#####################################
# CMSSW configuration file - Python #
#####################################
import FWCore.ParameterSet.Config as cms
import sys

process = cms.Process("ANALYSIS")

###########################
# Basic process controls. #
###########################

setupFileName = ''
setupSuffix = ''
setupNumEvents =-1
setupInputFilesList = ''

##################
# Basic services #
##################
# import of standard configurations
process.load('Configuration.EventContent.EventContent_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

# Other statements
myOptions = sys.argv
if 'input' in myOptions:
    setupFileName = myOptions[myOptions.index('input')+1]
else:
    setupFileName = ''

if 'suffix' in myOptions:
    setupSuffix = myOptions[myOptions.index('suffix')+1]
else:
    setupSuffix = ''

if 'numEvents' in myOptions:
    setupNumEvents = int(myOptions[myOptions.index('numEvents')+1])
else:
    setupNumEvents = -1
    
#readFiles = cms.untracked.vstring()
#secFiles = cms.untracked.vstring()
#process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
#readFiles.extend(["file:"+setupFileName]);

process.load("ExoDiBosonResonances.GeneratorStudies.source_JHU_Bulk600_ZZ_GENSIM")

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
    )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(setupNumEvents)
    )

### The output
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('output_'+setupSuffix+'.root')
                                   )

process.MessageLogger.cerr.FwkReport.reportEvery = 100


### Print events
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.printTree = cms.EDAnalyzer("ParticleListDrawer",
                                     maxEventsToPrint = cms.untracked.int32(1),
                                     printVertex = cms.untracked.bool(False),
                                     src = cms.InputTag("genParticles")
                                   )

### Prune particles
process.prunedGenParticles = cms.EDProducer("GenParticlePruner",
                                            src = cms.InputTag("genParticles"),
                                            select = cms.vstring("drop  *  ", # this is the default
                                                                 "keep status = 3"
                                                                 )
                                            )

### Check that the Z is decaying correctly
process.checkFlavorUniversality = cms.EDFilter("VCheckFlavour",
                                               src = cms.untracked.InputTag("prunedGenParticles")
                                               )

### Check that all events have Z to ll and Z to qq
process.leptonicDecay = cms.EDFilter("PdgIdAndStatusCandViewSelector",
                                     src = cms.InputTag("genParticles"),
                                     pdgId = cms.vint32( 11, 13 ),
                                     status = cms.vint32( 3 ),
                                     filter = cms.bool(True)
                                     )

process.hadronicDecay = cms.EDFilter("PdgIdAndStatusCandViewSelector",
                                     src = cms.InputTag("genParticles"),
                                     pdgId = cms.vint32( 1,2,3,4,5),
                                     status = cms.vint32( 3 ),
                                     filter = cms.bool(True)
                                     )

### Select the leptons
process.lepton = cms.EDFilter("PdgIdAndStatusCandViewSelector",
                              src = cms.InputTag("genParticles"),
                              pdgId = cms.vint32( 11, 13 ),
                              status = cms.vint32( 3 )
                              )

process.neutrino = cms.EDFilter("PdgIdAndStatusCandViewSelector",
                                src = cms.InputTag("genParticles"),
                                pdgId = cms.vint32( 12, 14 ),
                                status = cms.vint32( 3 )
                                )

process.sortedJets = cms.EDFilter("LargestPtCandViewSelector",
                                  src = cms.InputTag("ak7GenJets"),
                                  maxNumber = cms.uint32(1)
                                  )

process.analysis = cms.EDAnalyzer("GenEventAnalyzer",
                                  gravPdgId = cms.int32(39),
                                  allProcesses = cms.bool(True),
                                  qqbarProcess = cms.bool(False)
                                  )

#process.p = cms.Path(process.printTree)
process.p = cms.Path(process.prunedGenParticles +
                     process.checkFlavorUniversality +
                     process.leptonicDecay +
                     process.hadronicDecay +
                     process.sortedJets +
                     process.analysis)
