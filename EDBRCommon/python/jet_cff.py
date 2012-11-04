import FWCore.ParameterSet.Config as cms


# if there are not at least two jets of moderate pt, give up and move on
highPtJets = cms.EDFilter(
    "CandPtrSelector",  #not possible with PATJetSelector, broken refs to PF cands
    src=cms.InputTag("selectedPatJets"), #customPFJetsNoPUSub
    cut=cms.string("pt>15.0")
    )

jetCountFilter = cms.EDFilter("CandViewCountFilter",
                                 src = cms.InputTag("highPtJets"),
                                 minNumber = cms.uint32(2)
)

genSelectorZDaughter = cms.EDFilter("GenParticleSelector",
    src = cms.InputTag("genParticles"),
    cut = cms.string(' (abs(pdgId)==11 || abs(pdgId)==13)&& abs(mother.pdgId)==23 ')
)
genSelectorZQDaughter = cms.EDFilter("GenParticleSelector",
    src = cms.InputTag("genParticles"),
    cut = cms.string(' (abs(pdgId) < 9 )&& abs(mother.pdgId)==23 ')
)



# PF base jets (light PF jets, do not store constituents) -------
# must make cmgJet as first  PFJetFactory needs the  puMVA VM
# and they are done with the PAT-jets in PAT-tuples. Impossible
# to remake the puMVA VM because PF jets in our PAT-tuple miss
# embedded PF constituents
from ExoDiBosonResonances.EDBRCommon.factories.cmgJet_cfi import cmgJet as cmgJetRaw
cmgJetRaw.cfg.inputCollection=cms.InputTag('selectedPatJets')#"selectedPatJets")
#cmgJet = cmgJetRaw.clone()

from  CMGTools.External.pujetidsequence_cff import puJetId, puJetMva
## puJetIdCustom = puJetId.clone( jets = 'selectedPatJets')
puJetMvaCustom= puJetMva.clone(
    jetids = cms.InputTag("puJetId"),
    jets ='selectedPatJets',
    )

##puJetIdSequence = cms.Sequence(puJetIdCustom*puJetMvaCustom)
puJetIdSequence = cms.Sequence(puJetMvaCustom)

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






                        


#################################
#################################
#################################

customJets  = cms.EDProducer("PATJetCleaner",
                             src = cms.InputTag("selectedPatJets"),
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

