import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import *

### read steering options from cmd file:
from ExoDiBosonResonances.EDBRCommon.cmdLine import options
options.parseArguments()


process = cms.Process("CMG")
###########
# Options #
###########
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents))
###process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000))
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


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
#fullname  = "ExoDiBosonResonances.EDBRCommon.datasets.test_RSGZZ600_cff" 
print 'Importing dataset from '
print fullname
process.load(fullname)
##skip events with problem related kinematic fit                    DYJetsToLL_PtZ-50To70              TTBar                    WJetsPt70To100
process.source.eventsToSkip  = cms.untracked.VEventRange(cms.EventRange("1:58698863"),cms.EventRange("1:11250208"),cms.EventRange("1:15386873"))
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


process.badEventFilter = cms.EDFilter("HLTHighLevel",
                                     TriggerResultsTag =
                                      cms.InputTag("TriggerResults","","PAT"),
                                      HLTPaths =
                                      cms.vstring('primaryVertexFilterPath',
                                                  'noscrapingFilterPath',
                                                  'hcalLaserEventFilterPath',
                                                  'HBHENoiseFilterPath',
                                                  'trackingFailureFilterPath',
                                                  'CSCTightHaloFilterPath',
                                                  'eeBadScFilterPath',
                                                  'EcalDeadCellTriggerPrimitiveFilterPath'
    #                                              'totalKinematicsFilterPath' #only for Madgraph MC
                                                  ),
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
HLTlistMu  = cms.vstring("HLT_Mu17_Mu8*","HLT_Mu22_TkMu22*")   # triggers for DoubleMuon PD   
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
##     process.eventFilterSequence.insert(0, process.rndmEventBlinding) ##insert at the front of the list
     process.genParticles = cms.EDProducer("DummyGenProducer")
     process.eventFilterSequence.insert(1, process.genParticles)



if options.mcordata == "DATAELE" :
     process.eventFilterSequence +=process.hltHighLevelEle
if options.mcordata == "DATASE" :
     process.eventFilterSequence +=process.hltHighLevelSE
if options.mcordata == "DATAMU" :
     process.eventFilterSequence +=process.hltHighLevelMu
if options.mcordata == "DATASM" :
     process.eventFilterSequence +=process.hltHighLevelSM


###################################################################
# Ele Sequence: select electron and neutrino and build Welenu from them #
###################################################################
    
process.load('ExoDiBosonResonances.EDBRElectron.electron_cff')
process.load('ExoDiBosonResonances.EDBRElectron.skims.selEventsElectron_cfi')

process.load('ExoDiBosonResonances.EDBRCommon.factories.cmgNeutrino_cff')
process.load('ExoDiBosonResonances.EDBRCommon.skims.selEventsEleNeutrino_cfi')

process.load('ExoDiBosonResonances.EDBRElectron.factories.cmgWelenu_cfi')  
process.load('ExoDiBosonResonances.EDBRElectron.skims.selEventsWelenu_cff')

process.analysisSequenceElectrons = cms.Sequence(
    process.eleSequence +
    process.selectedElectronSequence +

    process.cmgEleNeutrino +
    process.selectedEleNeutrinoSequence +
    process.cmgWelenuEDBR +
    process.selectedWelenuSequence 

    ) 


##############
# PU weights #
##############
process.load('ExoDiBosonResonances.EDBRCommon.PUweights_cff')
## process.PUWeights.filenameData=cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/Pileup_2011_to_173692_LPLumiScale_NEW.root")
## process.PUWeights.filenameMC=cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/Pileup_2011_MC_Oct2011_35bins.root")

process.eleSequence.insert(0,process.PUseq)


###################################################################
# Mu Sequence: select muon and neutrino and build Wmunu from them #
###################################################################

process.load('ExoDiBosonResonances.EDBRMuon.muon_cff')
process.load('ExoDiBosonResonances.EDBRMuon.skims.selEventsMuon_cfi')

process.load('ExoDiBosonResonances.EDBRCommon.skims.selEventsMuNeutrino_cfi')

process.load('ExoDiBosonResonances.EDBRMuon.factories.cmgWmunu_cfi')
process.load('ExoDiBosonResonances.EDBRMuon.skims.selEventsWmunu_cff')

process.analysisSequenceMuons = cms.Sequence(
    process.muonSequence +
    process.selectedMuonSequence +
    process.cmgMuNeutrino +
    process.selectedMuNeutrinoSequence +
    process.cmgWmunuEDBR +
    process.selectedWmunuSequence
    )


if not ( options.lepton == "both" or options.lepton == "ele"): #only muon
     process.muonSequence.insert(0,process.PUseq)

###################################################################
# Jet Sequence: select jets and build di-jets from them           #
###################################################################
 
process.load('ExoDiBosonResonances.EDBRCommon.jet_cff')
process.load('ExoDiBosonResonances.EDBRCommon.factories.cmgDiJet_cfi')
process.load('ExoDiBosonResonances.EDBRCommon.factories.cmgDiJetKinFit_W_cfi')
process.load('ExoDiBosonResonances.EDBRCommon.skims.selEventsPFJet_cff')
process.load('ExoDiBosonResonances.EDBRCommon.skims.selEventsZjj_cff')

process.analysisSequenceJets = cms.Sequence(
    process.jetSequence +
    process.selectedJetSequence +
    process.diJetSequence +
    process.selectedZjjSequence+
    process.cmgDiJetKinFit
    )

process.analysisSequenceMergedJets = cms.Sequence(
    process.mergedJetSequence +
    process.selectedMergedJetSequence 
    )


###########################################################
# Resonance Sequence: build EXO resonance from W bosons   #
###########################################################

# build X->WW->evjj
process.load('ExoDiBosonResonances.EDBRElectron.resonanceWele_cff')
cloneProcessingSnippet(process,process.edbrSequenceEVJJ, "Ele")


process.analysisSequenceEVJJ = cms.Sequence(
    process.analysisSequenceElectrons +
    process.analysisSequenceJets +
    process.edbrSequenceEVJJEle
    )


# build X->WW->evj
cloneProcessingSnippet(process,process.edbrSequenceMergedEVJ, "Ele")
process.analysisSequenceEVJ = cms.Sequence(
    process.analysisSequenceElectrons +
    process.analysisSequenceMergedJets
  +  process.edbrSequenceMergedEVJEle
    )


# build X->WW->mmjj
process.load('ExoDiBosonResonances.EDBRMuon.resonanceWmu_cff')
#cloneProcessingSnippet(process,process.edbrSequenceMVJJ, "Mu")

process.analysisSequenceMVJJ = cms.Sequence(
    process.analysisSequenceMuons +
    process.analysisSequenceJets +
    process.edbrSequenceMVJJ
    )


# build X->WW->mmj
#cloneProcessingSnippet(process,process.edbrSequenceMergedMVJ, "Mu")
process.analysisSequenceMVJ = cms.Sequence(
    process.analysisSequenceMuons +
    process.analysisSequenceMergedJets +
    process.edbrSequenceMergedMVJ
    )



#update input collections for event filters
process.selectedEDBRKinFitCandFilterEle = process.selectedEDBRKinFitCandFilter.clone(
                                                           src = cms.InputTag('cmgEDBRSelKinFitEle')
                                                           )
process.selectedEDBRMergedCandFilterEle = process.selectedEDBRMergedCandFilter.clone(
                                                           src = cms.InputTag('cmgEDBRMergedSelEle')
                                                           )



if ( options.lepton == "both" or options.lepton == "ele"):
     process.preselElePath = cms.Path(process.eventFilterSequence + process.analysisSequenceEVJJ + process.selectedEDBRKinFitCandFilterEle)
     process.preselEleMergedPath = cms.Path(process.eventFilterSequence + process.analysisSequenceEVJ+process.selectedEDBRMergedCandFilterEle )
     
if ( options.lepton == "both" or options.lepton == "mu"):
     process.preselMuPath = cms.Path(process.eventFilterSequence + process.analysisSequenceMVJJ + process.selectedEDBRKinFitCandFilterMu)
     process.preselMuMergedPath = cms.Path(process.eventFilterSequence + process.analysisSequenceMVJ +process.selectedEDBRMergedCandFilterMu )



####################################
# Final selection and arbitration  #
####################################

# apply VBF tag, final cuts and run BestSelector
# for arbitrating between different topologies

process.load("ExoDiBosonResonances.EDBRCommon.FinalSelection_W_cff")

#default is electrons
cloneProcessingSnippet(process,process.cmgSeq, "Ele")
### already done by cloneProcessingSnippet
#massSearchReplaceAnyInputTag(process.cmgSeqEle,cms.InputTag("SingleJetVBFTagger"),cms.InputTag("SingleJetVBFTaggerEle"))
#massSearchReplaceAnyInputTag(process.cmgSeqEle,cms.InputTag("BestSelectorKinFit:singleJet"),cms.InputTag("BestSelectorKinFitEle:singleJet"))


###muons need filter types + inputs adjusted
cloneProcessingSnippet(process,process.cmgSeq, "Mu")
massSearchReplaceParam(process.cmgSeqMu,"_TypedParameterizable__type","WelenuDiJetEDBRBestCandidateSelector","WmunuDiJetEDBRBestCandidateSelector")

massSearchReplaceParam(process.cmgSeqMu,"_TypedParameterizable__type","WelenuDiJetEDBRTagger","WmunuDiJetEDBRTagger")
massSearchReplaceParam(process.cmgSeqMu,"_TypedParameterizable__type","WelenuSingleJetEDBRTagger","WmunuSingleJetEDBRTagger")
massSearchReplaceParam(process.cmgSeqMu,"_TypedParameterizable__type","WelenuNJetEDBRBestCandidateSelector","WmunuNJetEDBRBestCandidateSelector")



### some magic with python because we want to re-use
### the Sequences used for the preSel path, but removing the filters
### on number of Z->1j or Z->2j because when running the full analysis chain
### we want to merge them in the end by means of the BestCandidateSelector
cloneProcessingSnippet(process,process.analysisSequenceEVJJ, "FullE")
cloneProcessingSnippet(process,process.analysisSequenceMVJJ, "FullM")
cloneProcessingSnippet(process,process.analysisSequenceMergedJets, "FullJ")
process.analysisSequenceEVJJFullE.remove(process.selectedZjjCandFilterFullE)
process.analysisSequenceMVJJFullM.remove(process.selectedZjjCandFilterFullM)
process.analysisSequenceMergedJetsFullJ.remove(process.selectedVJetCandFilterFullJ)

massSearchReplaceAnyInputTag(process.cmgSeqEle,cms.InputTag("cmgEDBRSelKinFitEle"),cms.InputTag("cmgEDBRSelKinFitEleFullE"))
massSearchReplaceAnyInputTag(process.cmgSeqMu,cms.InputTag("cmgEDBRSelKinFitEle"),cms.InputTag("cmgEDBRSelKinFitMuFullM"))
massSearchReplaceAnyInputTag(process.cmgSeqMu,cms.InputTag("cmgEDBRMergedSelEle"),cms.InputTag("cmgEDBRMergedSelMu"))




#collect adjusted sequences into paths
if options.lepton == "both" or options.lepton == "ele":
     process.cmgEDBRWWEle = cms.Path(process.eventFilterSequence+
                                    process.analysisSequenceEVJJFullE +
                                    process.analysisSequenceMergedJetsFullJ + process.edbrSequenceMergedEVJEle +
                                    process.cmgSeqEle )

if options.lepton == "both" or options.lepton == "mu":
     process.cmgEDBRWWMu = cms.Path(process.eventFilterSequence+
                                    process.analysisSequenceMVJJFullM +
                                    process.analysisSequenceMergedJetsFullJ + process.edbrSequenceMergedMVJ +
                                    process.cmgSeqMu )


