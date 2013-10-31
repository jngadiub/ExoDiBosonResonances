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
##skip events with problem related to kinematic fit                    DYJetsToLL_PtZ-50To70              TTBar                    WJetsPt70To100
process.source.eventsToSkip  = cms.untracked.VEventRange(cms.EventRange("1:58698863"),cms.EventRange("1:11250208"),cms.EventRange("1:15386873"))
####for synch studies
#process.source.eventsToProcess = cms.untracked.VEventRange(cms.EventRange("166699:715236831"),cms.EventRange("173389:180639524"))
#process.source.eventsToProcess  = cms.untracked.VEventRange(cms.EventRange("1:231104"))

#BTagEffmapFilename="ExoDiBosonResonances/EDBRCommon/data/BtaggingEffs/WW_TuneZ2star_8TeV_pythia6_tauola_AK5PF_CSVM_bTaggingEfficiencyMap.root"
BTagEffmapFilename="ExoDiBosonResonances/EDBRCommon/data/BtaggingEffs/TT_CT10_TuneZ2star_8TeV-powheg-tauola_AK5PF_CSVM_bTaggingEfficiencyMap.root"
#BTagEffmapFilename="ExoDiBosonResonances/EDBRCommon/data/BtaggingEffs/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball_AK5PF_CSVM_bTaggingEfficiencyMap.root"
#BTagEffmapFilename="ExoDiBosonResonances/EDBRCommon/data/BtaggingEffs/QCD_Pt-800to1000_MuEnrichedPt5_TuneZ2star_8TeV_pythia6_AK5PF_CSVM_bTaggingEfficiencyMap.root"
BTag_scale_b = 0
BTag_scale_light = 0


###########
# Output  #
###########
process.load('ExoDiBosonResonances.EDBRCommon.outputModules_cff')
#adjust name of full path for XWW analysis
if options.selection == "full":
     process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring() )
     if options.lepton == "both" or options.lepton == "ele":
          process.out.SelectEvents.SelectEvents.append("cmgEDBRWWEle")
     if options.lepton == "both" or options.lepton == "mu":
          process.out.SelectEvents.SelectEvents.append("cmgEDBRWWMu")
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
                                      HLTPaths =cms.vstring(eventFiltersList),
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
#HLTlistSE = cms.vstring("HLT_Ele80_CaloIdVT_GsfTrkIdT_v1 AND NOT HLT_DoubleEle33*") # triggers fro SingleElectron PD
#HLTlistSM  = cms.vstring("HLT_Mu40_* AND NOT HLT_Mu17_Mu8* AND NOT HLT_Mu22_TkMu22*")

HLTlistSE = cms.vstring("HLT_Ele80_CaloIdVT_*")
HLTlistSM  = cms.vstring("HLT_Mu40_eta2p1*")

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
# Met Sequence: apply met phi correction #
###################################################################

process.load("JetMETCorrections/Type1MET/pfMETsysShiftCorrections_cfi")
process.pfMEtSysShiftCorr.src = cms.InputTag('patMETs')
process.pfMEtSysShiftCorr.srcMEt = cms.InputTag('patMETs')
#process.pfMEtSysShiftCorr.srcJets = cms.InputTag('selectedPatJetsPFlow')

if "DATA" in options.mcordata :
 # pfMEtSysShiftCorrParameters_2012runAplusBvsNvtx_data
 process.pfMEtSysShiftCorr.parameter = cms.PSet(
      numJetsMin = cms.int32(-1),
      numJetsMax = cms.int32(-1),
      px = cms.string("+0.2661 + 0.3217*Nvtx"),
      py = cms.string("-0.2251 - 0.1747*Nvtx")
 #    px = cms.string("+1.68804e-01 + 3.37139e-01*Nvtx"),
 #    py = cms.string("-1.72555e-01 - 1.79594e-01*Nvtx")
 )
else :
 # pfMEtSysShiftCorrParameters_2012runAplusBvsNvtx_mc
 process.pfMEtSysShiftCorr.parameter = cms.PSet(
      numJetsMin = cms.int32(-1),
      numJetsMax = cms.int32(-1),
      px = cms.string("+0.1166 + 0.0200*Nvtx"),
      py = cms.string("+0.2764 - 0.1280*Nvtx")
 #    px = cms.string("+2.22335e-02 - 6.59183e-02*Nvtx"),
 #    py = cms.string("+1.52720e-01 - 1.28052e-01*Nvtx")
 )
            
process.patMetShiftCorrected = cms.EDProducer("CorrectedPATMETProducer",
                                               src = cms.InputTag('patMETs'),
                                               applyType1Corrections = cms.bool(True),
                                               srcType1Corrections = cms.VInputTag(
                                               cms.InputTag('pfMEtSysShiftCorr')),
                                               applyType2Corrections = cms.bool(False)
                                              )

process.metphiCorretionSequence = cms.Sequence(
        process.pfMEtSysShiftCorrSequence *
        process.patMetShiftCorrected
        )


###################################################################
# Ele Sequence: select electron and neutrino and build Welenu from them #
###################################################################
    
process.load('ExoDiBosonResonances.EDBRElectron.electron_cff')
process.load('ExoDiBosonResonances.EDBRElectron.skims.selEventsElectron_cfi')
process.load('ExoDiBosonResonances.EDBRCommon.factories.cmgNeutrino_cff')
process.load('ExoDiBosonResonances.EDBRCommon.skims.selEventsEleNeutrino_cfi')
process.load('ExoDiBosonResonances.EDBRElectron.factories.cmgWelenu_cfi')  
process.load('ExoDiBosonResonances.EDBRElectron.skims.selEventsWelenu_cff')

###################################################################
# Mu Sequence: select muon and neutrino and build Wmunu from them #
###################################################################

process.load('ExoDiBosonResonances.EDBRMuon.muon_cff')
process.load('ExoDiBosonResonances.EDBRMuon.skims.selEventsMuon_cfi')
process.load('ExoDiBosonResonances.EDBRCommon.skims.selEventsMuNeutrino_cfi')
process.load('ExoDiBosonResonances.EDBRMuon.factories.cmgWmunu_cfi')
process.load('ExoDiBosonResonances.EDBRMuon.skims.selEventsWmunu_cff')


process.analysisSequenceElectrons = cms.Sequence(
	process.metphiCorretionSequence +
    process.eleSequence +
    process.selectedElectronSequence +
    process.muonSequence +
    process.selectedMuonSequence +
    process.cmgEleNeutrino +
    process.selectedEleNeutrinoSequence +
    process.cmgWelenuEDBR +
    process.selectedWelenuSequence 

    ) 


process.analysisSequenceMuons = cms.Sequence(
	process.metphiCorretionSequence +
    process.eleSequence +
    process.selectedElectronSequence +
    process.muonSequence +
    process.selectedMuonSequence +
    process.cmgMuNeutrino +
    process.selectedMuNeutrinoSequence +
    process.cmgWmunuEDBR +
    process.selectedWmunuSequence
    )


if options.selection=="none":
     process.selectedWelenuCandFilter.minNumber=0
if options.selection=="none":
     process.selectedWmunuCandFilter.minNumber=0


##############
# PU weights #
##############
process.load('ExoDiBosonResonances.EDBRCommon.PUweights_cff')
## process.PUWeights.filenameData=cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/Pileup_2011_to_173692_LPLumiScale_NEW.root")
## process.PUWeights.filenameMC=cms.FileInPath("ExoDiBosonResonances/EDBRCommon/data/Pileup_2011_MC_Oct2011_35bins.root")

process.eleSequence.insert(0,process.PUseq)

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
process.load('ExoDiBosonResonances.EDBRCommon.jetAK5_cff')

process.analysisSequenceJets1 = cms.Sequence(
    process.jetSequence +
    process.selectedJetSequence +
    process.diJetSequence)

process.analysisSequenceJets1NoFilter = cms.Sequence(
    process.jetSequence +
    process.selectedJetSequence +
    process.diJetSequence+
    process.selectedZjjSequence)
process.analysisSequenceJets1NoFilter.remove(process.selectedJetCandFilter)
process.analysisSequenceJets1NoFilter.remove(process.selectedZjjCandFilter)

##    process.selectedZjjSequence+
process.analysisSequenceJets2 = cms.Sequence(
    process.cmgDiJetKinFit +
    process.jetAK5Sequence
    )

process.analysisSequenceMergedJets = cms.Sequence(
    process.mergedJetSequence +
    process.selectedMergedJetSequence +
    process.jetAK5Sequence
    )


###########################################################
# Resonance Sequence: build EXO resonance from W bosons   #
###########################################################

# build X->WW->evjj
process.load('ExoDiBosonResonances.EDBRElectron.resonanceWele_cff')

process.BTagWeightsKinFitEle.EffmapFilename = cms.FileInPath(BTagEffmapFilename)
process.BTagWeightsKinFitEle.scale_b = cms.double(BTag_scale_b)
process.BTagWeightsKinFitEle.scale_light = cms.double(BTag_scale_light)

process.BTagWeightsEle.EffmapFilename = cms.FileInPath(BTagEffmapFilename)
process.BTagWeightsEle.scale_b = cms.double(BTag_scale_b)
process.BTagWeightsEle.scale_light = cms.double(BTag_scale_light)

process.BTagWeightsMergedEle.EffmapFilename = cms.FileInPath(BTagEffmapFilename)
process.BTagWeightsMergedEle.scale_b = cms.double(BTag_scale_b)
process.BTagWeightsMergedEle.scale_light = cms.double(BTag_scale_light)

cloneProcessingSnippet(process,process.edbrSequenceEVJJ, "Ele")


process.analysisSequenceEVJJ = cms.Sequence(
    process.analysisSequenceElectrons +
    process.analysisSequenceJets1 +
    process.selectedZjjSequence +
    process.analysisSequenceJets2 +
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

process.BTagWeightsKinFitMu.EffmapFilename = cms.FileInPath(BTagEffmapFilename)
process.BTagWeightsKinFitMu.scale_b = cms.double(BTag_scale_b)
process.BTagWeightsKinFitMu.scale_light = cms.double(BTag_scale_light)

process.BTagWeightsMu.EffmapFilename = cms.FileInPath(BTagEffmapFilename)
process.BTagWeightsMu.scale_b = cms.double(BTag_scale_b)
process.BTagWeightsMu.scale_light = cms.double(BTag_scale_light)

process.BTagWeightsMergedMu.EffmapFilename = cms.FileInPath(BTagEffmapFilename)
process.BTagWeightsMergedMu.scale_b = cms.double(BTag_scale_b)
process.BTagWeightsMergedMu.scale_light = cms.double(BTag_scale_light)
#cloneProcessingSnippet(process,process.edbrSequenceMVJJ, "Mu")

process.analysisSequenceMVJJ = cms.Sequence(
    process.analysisSequenceMuons +
    process.analysisSequenceJets1 +
    process.selectedZjjSequence +
    process.analysisSequenceJets2 +
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


# assamble actual paths. Move Bad-Event & HLT filters to extra paths
# for efficiency studies when no selection is required, so that
# graviton reconstruction runs on every event

if not options.selection == "none":
     if ( options.lepton == "both" or options.lepton == "ele"):
          process.preselElePath = cms.Path(process.eventFilterSequence + process.analysisSequenceEVJJ + process.selectedEDBRKinFitCandFilterEle)
          process.preselEleMergedPath = cms.Path(process.eventFilterSequence + process.analysisSequenceEVJ+process.selectedEDBRMergedCandFilterEle )
    
     if ( options.lepton == "both" or options.lepton == "mu"):
               process.preselMuPath = cms.Path(process.eventFilterSequence + process.analysisSequenceMVJJ + process.selectedEDBRKinFitCandFilterMu)
               process.preselMuMergedPath = cms.Path(process.eventFilterSequence + process.analysisSequenceMVJ +process.selectedEDBRMergedCandFilterMu )


else:
     process.eventFilterPath = cms.Path(process.eventFilterSequence)
     if ( options.lepton == "both" or options.lepton == "ele"):
          process.preselElePath = cms.Path( process.analysisSequenceEVJJ + process.selectedEDBRKinFitCandFilterEle)
          process.preselEleMergedPath = cms.Path( process.analysisSequenceEVJ+process.selectedEDBRMergedCandFilterEle )

     if ( options.lepton == "both" or options.lepton == "mu"):
          process.preselMuPath = cms.Path( process.analysisSequenceMVJJ + process.selectedEDBRKinFitCandFilterMu)
          process.preselMuMergedPath = cms.Path( process.analysisSequenceMVJ +process.selectedEDBRMergedCandFilterMu )



####################################


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
massSearchReplaceAnyInputTag(process.cmgSeqEle,cms.InputTag("cmgEDBRSelKinFitEle"),cms.InputTag("cmgEDBRSelKinFitEle"))
massSearchReplaceAnyInputTag(process.cmgSeqMu,cms.InputTag("cmgEDBRSelKinFitEle"),cms.InputTag("cmgEDBRSelKinFitMu"))
massSearchReplaceAnyInputTag(process.cmgSeqEle,cms.InputTag("cmgEDBRMergedSelEle"),cms.InputTag("cmgEDBRMergedSelEle"))
massSearchReplaceAnyInputTag(process.cmgSeqMu,cms.InputTag("cmgEDBRMergedSelEle"),cms.InputTag("cmgEDBRMergedSelMu"))


#collect adjusted sequences into paths
if options.lepton == "both" or options.lepton == "ele":
     process.cmgEDBRWWEle = cms.Path(process.eventFilterSequence+
                                     process.analysisSequenceElectrons +
                                     process.analysisSequenceJets1NoFilter +
                                     process.analysisSequenceJets2 +
                                     process.edbrSequenceEVJJEle+
                                     process.mergedJetSequence +
                                     process.jetAK5Sequence+
                                     process.edbrSequenceMergedEVJEle +
                                     process.cmgSeqEle )

if options.lepton == "both" or options.lepton == "mu":
     process.cmgEDBRWWMu = cms.Path(process.eventFilterSequence+
                                    process.analysisSequenceMuons +
                                    process.analysisSequenceJets1NoFilter +
                                    process.analysisSequenceJets2 +
                                    process.edbrSequenceMVJJ +                                    
                                    process.mergedJetSequence +
                                    process.jetAK5Sequence+
                                    process.edbrSequenceMergedMVJ +
                                    process.cmgSeqMu )


