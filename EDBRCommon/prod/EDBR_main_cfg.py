import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import *

### read steering options from cmd file:
from ExoDiBosonResonances.EDBRCommon.cmdLine import options
options.parseArguments()


process = cms.Process("CMG")
###########
# Options #
###########
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000))
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


###########
# Input   #
###########


###fullname  = "ExoDiBosonResonances.EDBRCommon.datasets." + options.infile
fullname  = "ExoDiBosonResonances.EDBRCommon.datasets.test_RSGZZ600_cff" 
print 'Importing dataset from '
print fullname
process.load(fullname)
####for synch studies
#process.source.eventsToProcess = cms.untracked.VEventRange(cms.EventRange("166699:715236831"),cms.EventRange("173389:180639524"))
#process.source.eventsToProcess  = cms.untracked.VEventRange(cms.EventRange("1:231104"))



###########
# Output  #
###########
process.load('ExoDiBosonResonances.EDBRCommon.outputModules_cff')
process.outpath = cms.EndPath(process.out)


###################
# JSON Filtering  #
###################
### #only do this for data
if options.mcordata == "DATA" and options.json!="" :
     import PhysicsTools.PythonAnalysis.LumiList as LumiList
     import FWCore.ParameterSet.Types as CfgTypes
     myLumis = LumiList.LumiList(filename = options.json).getCMSSWString().split(',')
     process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
     process.source.lumisToProcess.extend(myLumis)



############
# Event filter    #
############

process.badEventFilter = cms.EDFilter("HLTHighLevel",
                                     TriggerResultsTag =
                                      cms.InputTag("TriggerResults","","PAT"),
                                      HLTPaths =
                                      cms.vstring('primaryVertexFilterPath',
                                                  'noscrapingFilterPath',
                                                  'hcalLaserEventFilterPath',
                                                  'HBHENoiseFilterPath',
    #                                              'totalKinematicsFilterPath' #only for Madgraph MC
                                                  ),
                                      eventSetupPathsKey = cms.string(''),
                                       # how to deal with multiple triggers: True (OR) accept if ANY is true, False
                                      #(AND) accept if ALL are true
                                      andOr = cms.bool(False), 
                                      throw = cms.bool(True)    # throw exception on unknown path names
                                      ) 


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

##############
# PU weights #
##############
process.load('ExoDiBosonResonances.EDBRCommon.PUweights_cff')
## process.PUWeights.filenameData=cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/Pileup_2011_to_173692_LPLumiScale_NEW.root")
## process.PUWeights.filenameMC=cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/Pileup_2011_MC_Oct2011_35bins.root")

process.eleSequence.insert(0,process.PUseq)


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


process.preselElePath = cms.Path(process.badEventFilter+ process.analysisSequenceZZEE )
process.preselMuPath = cms.Path(process.badEventFilter+ process.analysisSequenceZZMM )

