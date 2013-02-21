import FWCore.ParameterSet.Config as cms


# if there are not at least two jets of moderate pt, give up and move on
highPtJets = cms.EDFilter(
    "CandPtrSelector",  #not possible with PATJetSelector, broken refs to PF cands
    src=cms.InputTag("selectedPatJetsCA8CHSwithQjets"), #customPFJetsNoPUSub
    cut=cms.string("pt>15.0")
    )

jetCountFilter = cms.EDFilter("CandViewCountFilter",
                                 src = cms.InputTag("highPtJets"),
                                 minNumber = cms.uint32(2)
)

genSelectorZQQ = cms.EDFilter("GenParticleSelector",
    src = cms.InputTag("genParticles"),
    cut = cms.string(' abs(pdgId)==23 &&numberOfDaughters> 0 && abs(daughter(0).pdgId)<9 && status==3')
)

genSelectorZDaughter = cms.EDFilter("GenParticleSelector",
    src = cms.InputTag("genParticles"),
    cut = cms.string(' (abs(pdgId)==11 || abs(pdgId)==13)&& abs(mother.pdgId)==23 ')
)
genSelectorZQDaughter = cms.EDFilter("GenParticleSelector",
    src = cms.InputTag("genParticles"),
    cut = cms.string(' (abs(pdgId) < 9 )&& abs(mother.pdgId)==23 ')
)




from  CMGTools.External.pujetidsequence_cff import puJetId, puJetMva
from CMGTools.External.pujetidproducer_cfi import  stdalgos_4x, stdalgos_5x, stdalgos, cutbased, chsalgos_4x, chsalgos_5x, chsalgos
## puJetIdCustom = puJetId.clone( jets = 'selectedPatJets')
##puJetIdCustom = puJetId.clone( jets = 'selectedPatJetsCA8CHSwithQjets')
puJetMvaCustom= puJetMva.clone(
    jetids = cms.InputTag("puJetIdCA8CHS"),
#    jetids = cms.InputTag("puJetIdCustom"),
    jets ='selectedPatJetsCA8CHSwithQjets',
    algos =  chsalgos
    )
###puJetIdSequence = cms.Sequence(puJetIdCustom*puJetMvaCustom)
puJetIdSequence = cms.Sequence(puJetMvaCustom)



# PF base jets (light PF jets, do not store constituents) -------
# must make cmgJet as first  PFJetFactory needs the  puMVA VM
# and they are done with the PAT-jets in PAT-tuples.
from ExoDiBosonResonances.EDBRCommon.factories.cmgJet_cfi import cmgJet as cmgJetRaw
cmgJetRaw.cfg.inputCollection=cms.InputTag('selectedPatJetsCA8CHSwithQjets')#"selectedPatJets")
cmgJetRaw.cfg.puVariables=cms.InputTag("puJetIdCA8CHS")
#cmgJet = cmgJetRaw.clone()




cmgJet = cms.EDProducer("cmgPFJetCleaner",
                          src = cms.InputTag("cmgJetRaw"),
                          preselection = cms.string(''),
                          checkOverlaps = cms.PSet( genLeptons = cms.PSet( src = cms.InputTag("genSelectorZDaughter"),
                                                                           preselection        = cms.string(""),  # don't preselect the muons
                                                                           deltaR              = cms.double(0.3),
                                                                           checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                           pairCut             = cms.string(""),
                                                                           requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                           ),
                                                    genParton = cms.PSet( src = cms.InputTag("genSelectorZQDaughter"),
                                                                          preselection        = cms.string(""),  # don't preselect the muons
                                                                          deltaR              = cms.double(0.5),
                                                                          checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                          pairCut             = cms.string(""),
                                                                          requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                          ),
                                                    ),                                      
                          finalCut = cms.string(''),
                          )




from ExoDiBosonResonances.EDBRCommon.factories.cmgJet_cfi import cmgStructuredJet as cmgJetStructuredRaw
cmgJetStructuredRaw.cfg.inputCollection=cms.InputTag('selectedPatJetsCA8CHSwithQjets')
cmgJetStructuredRaw.cfg.prunedJetCollection=cms.InputTag('selectedPatJetsCA8CHSpruned')
cmgJetStructuredRaw.cfg.puVariables=cms.InputTag("puJetIdCA8CHS")
cmgJetStructured = cms.EDProducer("cmgVJetCleaner",
                          src = cms.InputTag("cmgJetStructuredRaw"),
                          preselection = cms.string(''),
                          checkOverlaps = cms.PSet( genJets = cms.PSet( src = cms.InputTag("genSelectorZQQ"),
                                                                           preselection        = cms.string(""),  # don't preselect the muons
                                                                           deltaR              = cms.double(0.3),
                                                                           checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                           pairCut             = cms.string(""),
                                                                           requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                           ),              
                                                    ),
                                  finalCut = cms.string('')
                                  )

#################################
#################################
#################################

customJets  = cms.EDProducer("PATJetCleaner",
                             src = cms.InputTag("selectedPatJetsCA8CHSwithQjets"),
                                        preselection = cms.string(''),
                                        checkOverlaps = cms.PSet( genLeptons = cms.PSet( src = cms.InputTag("genSelectorZDaughter"),
                                                                                         algorithm = cms.string("byDeltaR"),
                                                                                         preselection        = cms.string(""),  # don't preselect the muons
                                                                                         deltaR              = cms.double(0.3),
                                                                                         checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                                         pairCut             = cms.string(""),
                                                                                         requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                                         ),
                                                                  genParton = cms.PSet( src = cms.InputTag("genSelectorZQDaughter"),
                                                                                         algorithm = cms.string("byDeltaR"),
                                                                                         preselection        = cms.string(""),  # don't preselect the muons
                                                                                         deltaR              = cms.double(0.5),
                                                                                         checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                                         pairCut             = cms.string(""),
                                                                                         requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                                         ),

                                                                  ),                                      
                                        finalCut = cms.string(''),
                                        )

jetSequence = cms.Sequence(
    highPtJets*jetCountFilter
    + genSelectorZDaughter*genSelectorZQDaughter
#    + kt6PFJetsForIso*ak5PFJetsL1FastL2L3*qglAK5PF
    #+customJets
    + puJetIdSequence
#    + ak5PFJets*ak5PFJetsL1FastL2L3 *qglAK5PF 
    + cmgJetRaw
    + cmgJet
 
#    + cmgJet
    )

mergedJetSequence = cms.Sequence(
    highPtJets*jetCountFilter
    + genSelectorZQQ
    + puJetIdSequence
    + cmgJetStructuredRaw
    + cmgJetStructured
)
