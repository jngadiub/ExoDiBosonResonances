import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import *

### read steering options from cmd file:
from ExoDiBosonResonances.EDBRCommon.cmdLine import options
options.parseArguments()


process = cms.Process("CMG")
###########
# Options #
###########
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#from Configuration.PyReleaseValidation.autoCond import autoCond
#process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )
process.GlobalTag.globaltag = cms.string("START53_V7A::All")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff")


###########
# Input   #
###########

fullname  = "ExoDiBosonResonances.EDBRCommon.datasets." + options.infile
##fullname  = "ExoDiBosonResonances.EDBRCommon.datasets.summer12_BulkG_ZZ_lljj_c0p2_M1600_cff" 
print 'Importing dataset from '
print fullname
process.load(fullname)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents))
###process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000))

##skip events with problem related to kinematic fit                    DYJetsToLL_PtZ-50To70              TTBar                    WJetsPt70To100
process.source.eventsToSkip  = cms.untracked.VEventRange(cms.EventRange("1:58698863"),
                                                         cms.EventRange("1:11250208"),
                                                         cms.EventRange("1:15386873"),
                                                         cms.EventRange("1:19438582"),
                                                         cms.EventRange("1:21193075")
                                                         )
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
if "DATA" in options.mcordata and options.json!="" :
     import PhysicsTools.PythonAnalysis.LumiList as LumiList
     import FWCore.ParameterSet.Types as CfgTypes
     myLumis = LumiList.LumiList(filename = options.json).getCMSSWString().split(',')
     process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
     process.source.lumisToProcess.extend(myLumis)



############
# Event filter    #
############

process.rndmEventBlinding = cms.EDFilter("EDBREventSampler",
                                         RandomGenSeed = cms.int32(87654),
                                         SamplingFactor = cms.double(0.2) # 1/5 events pass the filter
                                         )

eventFiltersList=['primaryVertexFilterPath',
                  'noscrapingFilterPath',
                  'hcalLaserEventFilterPath',
                  'HBHENoiseFilterPath',
                  'trackingFailureFilterPath',
                  'CSCTightHaloFilterPath',
                  'eeBadScFilterPath',
                  'EcalDeadCellTriggerPrimitiveFilterPath',
                  'ecalLaserFilterPath']

if "DATA" in options.mcordata :
     eventFiltersList.append('trkPOGFiltersPath')
     
process.badEventFilter = cms.EDFilter("HLTHighLevel",
                                     TriggerResultsTag =
                                      cms.InputTag("TriggerResults","","PAT"),
                                      HLTPaths = cms.vstring(eventFiltersList),
                                      eventSetupPathsKey = cms.string(''),
                                       # how to deal with multiple triggers: True (OR) accept if ANY is true, False
                                      #(AND) accept if ALL are true
                                      andOr = cms.bool(False), 
                                      throw = cms.bool(True)    # throw exception on unknown path names
                                      ) 


###########
# HLT filter
###########

# provide list of HLT paths (or patterns) you want
#HLTlistMu  = cms.vstring("HLT_Mu17_Mu8*","HLT_Mu22_TkMu22*")   # triggers for DoubleMuon PD
HLTlistMu  = cms.vstring("HLT_Mu22_TkMu8_*") # triggers for DoubleMuon PD, after conversation with Muon POG conveners
HLTlistEle = cms.vstring("HLT_DoubleEle33_*") # triggers for DoubleElectron PD

### for SingleElectron and SingleMuon PD, request single lept trigger and
#veto the same triggers used for double ele and DoubleMu PD: in this way
#remove events in both PDs
HLTlistSE = cms.vstring("HLT_Ele80_CaloIdVT_GsfTrkIdT_v1 AND NOT HLT_DoubleEle33*") # triggers fro SingleElectron PD
HLTlistSM  = cms.vstring("HLT_Mu40_* AND NOT HLT_Mu17_Mu8* AND NOT HLT_Mu22_TkMu22*")

process.hltHighLevelEle = cms.EDFilter("HLTHighLevel",
                                       TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                       HLTPaths = cms.vstring(HLTlistEle),
                                       eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
                                       andOr = cms.bool(True),  # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
                                       throw = cms.bool(True)    # throw exception on unknown path names
                                       )
process.hltHighLevelMu = cms.EDFilter("HLTHighLevel",
                                       TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                       HLTPaths = cms.vstring(HLTlistMu),
                                       eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
                                       andOr = cms.bool(True),   # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
                                       throw = cms.bool(True)    # throw exception on unknown path names
   )

process.hltHighLevelMC = cms.EDFilter("HLTHighLevel",
                                       TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                       HLTPaths = cms.vstring(HLTlistEle+HLTlistMu),
                                       eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
                                       andOr = cms.bool(True),   # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
                                       throw = cms.bool(True)    # throw exception on unknown path names
   )

process.hltHighLevelSM = cms.EDFilter("HLTHighLevel",
                                       TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                       HLTPaths = cms.vstring(HLTlistSM),
                                       eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
                                       andOr = cms.bool(True),   # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
                                       throw = cms.bool(True)    # throw exception on unknown path names
   )

process.hltHighLevelSE = cms.EDFilter("HLTHighLevel",
                                       TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                       HLTPaths = cms.vstring(HLTlistSE),
                                       eventSetupPathsKey = cms.string(''),
                                       andOr = cms.bool(True),
                                       throw = cms.bool(True) 
                                      )

### add them to event filter
process.eventFilterSequence = cms.Sequence(process.badEventFilter)

####################
# Hacks for DATA   #
####################
if "DATA" in options.mcordata :
###     process.eventFilterSequence.insert(0, process.rndmEventBlinding) ##insert at the front of the list
     process.genParticles = cms.EDProducer("DummyGenProducer")
     process.eventFilterSequence.insert(1, process.genParticles)


#### add HLT filters to path
if options.mcordata == "DATAELE" :
     process.eventFilterSequence +=process.hltHighLevelEle
if options.mcordata == "DATASE" :
     process.eventFilterSequence +=process.hltHighLevelSE
if options.mcordata == "DATAMU" :
     process.eventFilterSequence +=process.hltHighLevelMu
if options.mcordata == "DATASM" :
     process.eventFilterSequence +=process.hltHighLevelSM

if options.mcordata == "MC" :
     process.eventFilterSequence +=process.hltHighLevelMC


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

if options.selection=="none":
     process.selectedZeeCandFilter.minNumber=0
     
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
if not ( options.lepton == "both" or options.lepton == "ele"): #only muon
     process.muonSequence.insert(0,process.PUseq)

if options.selection=="none":
     process.selectedZmmCandFilter.minNumber=0

          

###################################################################
# Jet Sequence: select jets and build di-jets from them           #
###################################################################
 
process.load('ExoDiBosonResonances.EDBRCommon.jet_cff')
process.load('ExoDiBosonResonances.EDBRCommon.factories.cmgDiJet_cfi')
process.load('ExoDiBosonResonances.EDBRCommon.factories.cmgDiJetKinFit_cfi')
process.load('ExoDiBosonResonances.EDBRCommon.skims.selEventsPFJet_cff')
process.load('ExoDiBosonResonances.EDBRCommon.skims.selEventsZjj_cff')
#process.load('ExoDiBosonResonances.EDBRCommon.jetAK5_cff')

process.analysisSequenceJets1 = cms.Sequence(
    process.jetSequence +
    process.selectedJetSequence +
    process.diJetSequence)

process.analysisSequenceJets1NoFilter = cms.Sequence(
    process.jetSequence +
    process.selectedJetSequence +
    process.diJetSequence +
    process.selectedZjjSequence)
process.analysisSequenceJets1NoFilter.remove(process.selectedJetCandFilter)
process.analysisSequenceJets1NoFilter.remove(process.selectedZjjCandFilter)

##    process.selectedZjjSequence+
process.analysisSequenceJets2 = cms.Sequence(
    process.cmgDiJetKinFit 
#   + process.jetAK5Sequence
    )

process.analysisSequenceMergedJets = cms.Sequence(
    process.mergedJetSequence +
    process.selectedMergedJetSequence 
#    process.jetAK5Sequence
    )

###########################################################
# Resonance Sequence: build EXO resonance from Z bosons   #
###########################################################

# build X->ZZ->eejj
process.load('ExoDiBosonResonances.EDBRElectron.resonanceEle_cff')
cloneProcessingSnippet(process,process.edbrSequenceEEJJ, "Ele")
process.analysisSequenceEEJJ = cms.Sequence(
    process.analysisSequenceElectrons +
    process.analysisSequenceJets1 +
    process.selectedZjjSequence +
    process.analysisSequenceJets2 +    
    process.edbrSequenceEEJJEle
    )

# build X->ZZ->eej
cloneProcessingSnippet(process,process.edbrSequenceMerged, "Ele")
process.analysisSequenceEEJ = cms.Sequence(
    process.analysisSequenceElectrons +
    process.analysisSequenceMergedJets +
    process.edbrSequenceMergedEle 
    )


# build X->ZZ->mmjj
process.load('ExoDiBosonResonances.EDBRMuon.resonanceMu_cff')
##cloneProcessingSnippet(process,process.edbrSequenceMMJJ, "Mu")
process.analysisSequenceMMJJ = cms.Sequence(
    process.analysisSequenceMuons +
    process.analysisSequenceJets1 +
    process.selectedZjjSequence +
    process.analysisSequenceJets2 +
    process.edbrSequenceMMJJ
    )

# build X->ZZ->mmj
###cloneProcessingSnippet(process,process.edbrSequenceMerged, "Mu")
process.analysisSequenceMMJ = cms.Sequence(
    process.analysisSequenceMuons +
    process.analysisSequenceMergedJets +
    process.edbrSequenceMergedMMJ
    )

#update input collections for event filters
process.selectedEDBRKinFitCandFilterEle = process.selectedEDBRKinFitCandFilter.clone(
                                                           src = cms.InputTag('cmgEDBRSelKinFitEle')
                                                           )
process.selectedEDBRMergedCandFilterEle = process.selectedEDBRMergedCandFilter.clone(
                                                           src = cms.InputTag('cmgEDBRMergedSelEle')
                                                           )

# assamble actual paths. Move Bad-Event & HLT filters to extra paths
# for efficiency studies when no selection is required, so that
# graviton reconstruction runs on every event

if not options.selection == "none":
     if ( options.lepton == "both" or options.lepton == "ele"):
          process.preselElePath = cms.Path(process.eventFilterSequence + process.analysisSequenceEEJJ + process.selectedEDBRKinFitCandFilterEle)
          process.preselEleMergedPath = cms.Path(process.eventFilterSequence + process.analysisSequenceEEJ+process.selectedEDBRMergedCandFilterEle )
          
     if ( options.lepton == "both" or options.lepton == "mu"):
               process.preselMuPath = cms.Path(process.eventFilterSequence + process.analysisSequenceMMJJ + process.selectedEDBRKinFitCandFilterMu)
               process.preselMuMergedPath = cms.Path(process.eventFilterSequence + process.analysisSequenceMMJ +process.selectedEDBRMergedCandFilterMu )

else:
     process.eventFilterPath = cms.Path(process.eventFilterSequence)
     if ( options.lepton == "both" or options.lepton == "ele"):
          process.preselElePath = cms.Path( process.analysisSequenceEEJJ + process.selectedEDBRKinFitCandFilterEle)
          process.preselEleMergedPath = cms.Path( process.analysisSequenceEEJ+process.selectedEDBRMergedCandFilterEle )
          
     if ( options.lepton == "both" or options.lepton == "mu"):
          process.preselMuPath = cms.Path( process.analysisSequenceMMJJ + process.selectedEDBRKinFitCandFilterMu)
          process.preselMuMergedPath = cms.Path( process.analysisSequenceMMJ +process.selectedEDBRMergedCandFilterMu )
   


####################################
# Final selection and arbitration  #
####################################

# apply VBF tag, final cuts and run BestSelector
# for arbitrating between different topologies

process.load("ExoDiBosonResonances.EDBRCommon.FinalSelection_cff")

#default is electrons
cloneProcessingSnippet(process,process.cmgSeq, "Ele")
### already done by cloneProcessingSnippet
#massSearchReplaceAnyInputTag(process.cmgSeqEle,cms.InputTag("SingleJetVBFTagger"),cms.InputTag("SingleJetVBFTaggerEle"))
#massSearchReplaceAnyInputTag(process.cmgSeqEle,cms.InputTag("BestSelectorKinFit:singleJet"),cms.InputTag("BestSelectorKinFitEle:singleJet"))


###muons need filter types + inputs adjusted
cloneProcessingSnippet(process,process.cmgSeq, "Mu")
massSearchReplaceParam(process.cmgSeqMu,"_TypedParameterizable__type","DiElectronDiJetEDBRBestCandidateSelector","DiMuonDiJetEDBRBestCandidateSelector")

massSearchReplaceParam(process.cmgSeqMu,"_TypedParameterizable__type","DiElectronDiJetEDBRTagger","DiMuonDiJetEDBRTagger")
massSearchReplaceParam(process.cmgSeqMu,"_TypedParameterizable__type","DiElectronSingleJetEDBRTagger","DiMuonSingleJetEDBRTagger")
massSearchReplaceParam(process.cmgSeqMu,"_TypedParameterizable__type","DiElectronNJetEDBRBestCandidateSelector","DiMuonNJetEDBRBestCandidateSelector")
massSearchReplaceAnyInputTag(process.cmgSeqEle,cms.InputTag("cmgEDBRSelKinFitEle"),cms.InputTag("cmgEDBRSelKinFitEle"))
massSearchReplaceAnyInputTag(process.cmgSeqMu,cms.InputTag("cmgEDBRSelKinFitEle"),cms.InputTag("cmgEDBRSelKinFitMu"))
massSearchReplaceAnyInputTag(process.cmgSeqEle,cms.InputTag("cmgEDBRMergedSelEle"),cms.InputTag("cmgEDBRMergedSelEle"))
massSearchReplaceAnyInputTag(process.cmgSeqMu,cms.InputTag("cmgEDBRMergedSelEle"),cms.InputTag("cmgEDBRMergedSelMu"))

#collect adjusted sequences into paths
if options.lepton == "both" or options.lepton == "ele":
     process.cmgEDBRZZEle = cms.Path(process.eventFilterSequence+
                                     process.analysisSequenceElectrons +
                                     process.analysisSequenceJets1NoFilter +
                                     process.analysisSequenceJets2 +
                                     process.edbrSequenceEEJJEle+
                                     process.mergedJetSequence +
                                     process.edbrSequenceMergedEle +
                                     process.cmgSeqEle )

if options.lepton == "both" or options.lepton == "mu":
     process.cmgEDBRZZMu = cms.Path(process.eventFilterSequence+
                                    process.analysisSequenceMuons +
                                    process.analysisSequenceJets1NoFilter +
                                    process.analysisSequenceJets2 +
                                    process.edbrSequenceMMJJ +                                    
                                    process.mergedJetSequence +
                                    process.edbrSequenceMergedMMJ +
                                    process.cmgSeqMu )


