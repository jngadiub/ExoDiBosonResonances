import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import *


process = cms.Process("CMG")
###########
# Options #
###########
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100))
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10


process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#from Configuration.PyReleaseValidation.autoCond import autoCond
#process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )
process.GlobalTag.globaltag = cms.string("START53_V7A::All")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff")

readFiles = cms.untracked.vstring()
process.source = cms.Source("PoolSource",
                            noEventSort = cms.untracked.bool(True),
                            duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                            fileNames = readFiles
                            )

readFiles.extend(['/store/cmst3/user/bonato//patTuple/2012/EXOVVtest/newPatTuple_ZZ_600_c1.root'])


process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string("test.root"),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_*_*_*'
                                                                      )
                               )

process.outpath = cms.EndPath(process.out)


###################################################################
# Ele Sequence: select electrons and build di-electrons from them #
###################################################################
              
process.load('ExoDiBosonResonances.EDBRElectron.electron_cff')
process.load('ExoDiBosonResonances.EDBRElectron.diElectron_cff')
process.load('ExoDiBosonResonances.EDBRElectron.skims.selEventsElectron_cfi')
process.load('ExoDiBosonResonances.EDBRElectron.skims.selEventsZ_cff')


process.analysisSequenceElectrons = cms.Sequence(
    process.eleSequence +
    process.selectedElectronSequence +
    process.diElectronSequence +
    process.selectedZSequence
    )

############################################################
# Muon Sequence: select muons and build di-muons from them #
############################################################
              
process.load('ExoDiBosonResonances.EDBRMuon.muon_cff')
process.load('ExoDiBosonResonances.EDBRMuon.diMuon_cff')
process.load('ExoDiBosonResonances.EDBRMuon.skims.selEventsMuon_cfi')
process.load('ExoDiBosonResonances.EDBRMuon.skims.selEventsZ_cff')


process.analysisSequenceMuons = cms.Sequence(
    process.muonSequence +
    process.selectedMuonSequence +
    process.diMuonSequence +
    process.selectedZMMSequence
    )


###################################################################
# Jet Sequence: select jets and build di-jets from them           #
###################################################################
 
process.load('ExoDiBosonResonances.EDBRCommon.jet_cff')
process.load('ExoDiBosonResonances.EDBRCommon.factories.cmgDiJet_cfi')
process.load('ExoDiBosonResonances.EDBRCommon.factories.cmgDiJetKinFit_cfi')
process.load('ExoDiBosonResonances.EDBRCommon.skims.selEventsPFJet_cff')
process.load('ExoDiBosonResonances.EDBRCommon.skims.selEventsZjj_cff')

process.analysisSequenceJets = cms.Sequence(
    process.jetSequence +
    process.selectedJetSequence +
    process.diJetSequence +
    process.selectedZjjSequence+
    process.cmgDiJetKinFit
    )


# build X->ZZ->eejj
process.load('ExoDiBosonResonances.EDBRElectron.resonance_cff')
cloneProcessingSnippet(process,process.edbrSequenceEE, "Ele")


process.analysisSequenceZZEE = cms.Sequence(
    process.analysisSequenceElectrons +

    process.analysisSequenceJets +
    
    process.edbrSequenceEEEle 
    )

# build X->ZZ->mmjj
process.load('ExoDiBosonResonances.EDBRMuon.resonance_cff')
cloneProcessingSnippet(process,process.edbrSequenceMM, "Mu")

process.analysisSequenceZZMM = cms.Sequence(
    process.analysisSequenceMuons +

    process.analysisSequenceJets +
    
    process.edbrSequenceMMMu 
    )


process.preselElePath = cms.Path( process.analysisSequenceZZEE )
process.preselMuPath = cms.Path( process.analysisSequenceZZMM )

