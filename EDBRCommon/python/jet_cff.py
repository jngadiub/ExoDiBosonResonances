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
from HiggsAna.HLLJJCommon.factories.cmgJet_cfi import cmgJet as cmgJetRaw
cmgJetRaw.cfg.PFJetFactory.inputCollection=cms.InputTag('selectedPatJets')#"selectedPatJets")
#cmgJet = cmgJetRaw.clone()

# new qg likelihood (2012) 
qglikelihood   = cms.EDProducer(
    "QuarkGluonTagger",
    jets     = cms.InputTag("selectedPatJets"),
    rho      = cms.InputTag("kt6PFJetsForIso:rho"),
    jec      = cms.string('ak5PFL1FastL2L3'),
    isPatJet = cms.bool(True),
    )
# old qg likelihood (2011) 
## qglikelihood=cms.EDProducer("QGProducer",
##                             src=cms.InputTag('customJets'),#"selectedPatJets"), #customJets
##                             rho=cms.InputTag("kt6PFJetsForIso","rho"),
##                             filename=cms.FileInPath("Francesco/QGLikelihood/src/QG_QCD_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11-PU_S3_START42_V11-v2.root"),
##                             ptBins=cms.uint32(20)
## )


# ## new qg likelihood from Francesco
# from JetMETCorrections.Configuration.DefaultJEC_cff import * #ak5PFJetsL1FastL2L3
# from RecoJets.JetProducers.ak5PFJets_cfi import ak5PFJets
# ak5PFJets.doAreaFastjet = cms.bool(True)
# ##from RecoJets.Configuration.RecoPFJets_cff import kt6PFJets
# ##kt6PFJets = process.kt6PFJets.clone( rParam = 0.6, doRhoFastjet = True )
# ##kt6PFJetsForIso = kt6PFJets.clone( rParam = 0.6,
# ##                                           doRhoFastjet = True ,
# ##                                           Rho_EtaMax=2.5)
# ##process.kt6PFJetsForIso.Rho_EtaMax = cms.double(2.5)

# qglAK5PF = cms.EDProducer("QuarkGluonTagger",
#                           jets     = cms.InputTag("customPFJetsNoPUSub"),
#                           rho      = cms.InputTag('kt6PFJetsForIso','rho'),
#                           isPatJets = cms.bool(True),
#                           jec      = cms.string('ak5PFL1FastL2L3'), #ak5PFL1FastL2L3
#                           filename_nCharged=cms.FileInPath("Francesco/QuarkGluonTagger/data/QGTaggerConfig_nCharged_AK5PF.txt"),
#                           filename_nNeutral=cms.FileInPath("Francesco/QuarkGluonTagger/data/QGTaggerConfig_nNeutral_AK5PF.txt"),
#                           filename_ptD=cms.FileInPath("Francesco/QuarkGluonTagger/data/QGTaggerConfig_ptD_AK5PF.txt")
#                           )


from  CMGTools.External.pujetidsequence_cff import puJetId, puJetMva
## puJetIdCustom = puJetId.clone( jets = 'selectedPatJets')
puJetMvaCustom= puJetMva.clone(
    jetids = cms.InputTag("puJetId"),
    jets ='selectedPatJets',
    )

##puJetIdSequence = cms.Sequence(puJetIdCustom*puJetMvaCustom)
puJetIdSequence = cms.Sequence(puJetMvaCustom)

cmgJetR2 = cms.EDProducer("cmgPFJetCleaner",
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




cmgJet = cms.EDProducer("BScaler",
                        src = cms.InputTag("cmgJetR2"),
                        btagSysFactor = cms.double(1.0),
                        bTagAlgoName=cms.string("JP"),
                        seed = cms.int32(1234)
                        )


                        


#################################
#################################
#################################

customJets  = cms.EDProducer("PATJetCleaner",
      #                                  src = cms.InputTag("selectedPatJetsAK5NoPUSub"),
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
##                                                                   recoMuons = cms.PSet( src = cms.InputTag("cmgDiMuon"),
##                                                                                           algorithm = cms.string("byDeltaR"),
##                                                                                           preselection        = cms.string(""),  # don't preselect the muons
##                                                                                           deltaR              = cms.double(999.),
##                                                                                           checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
##                                                                                           pairCut             = cms.string(" ( deltaR(cand1.eta,cand1.phi,cand2.daughter(0).eta,cand2.daughter(0).phi) < 0.5"
##                                                                                                                            +"|| deltaR(cand1.eta,cand1.phi,cand2.daughter(1).eta,cand2.daughter(1).phi) < 0.5 )"),
##                                                                                           requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
##                                                                                           ),
##                                                                   recoElectrons = cms.PSet( src = cms.InputTag("cmgDiElectron2L2Q"),
##                                                                                           algorithm = cms.string("byDeltaR"),
##                                                                                           preselection        = cms.string(""),  # don't preselect the muons
##                                                                                           deltaR              = cms.double(999.),
##                                                                                           checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
##                                                                                           pairCut             = cms.string(" ( deltaR(cand1.eta,cand1.phi,cand2.daughter(0).eta,cand2.daughter(0).phi) < 0.5"
##                                                                                                                            +"|| deltaR(cand1.eta,cand1.phi,cand2.daughter(1).eta,cand2.daughter(1).phi) < 0.5 )"),
##                                                                                           requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
##                                                                                           ),

                                                                  ),                                      
                                        finalCut = cms.string(''),
                                        )

jetSequence = cms.Sequence(
    highPtJets*jetCountFilter
    + genSelectorZDaughter*genSelectorZQDaughter
#    + kt6PFJetsForIso*ak5PFJetsL1FastL2L3*qglAK5PF
    #+customJets
    + puJetIdSequence
    + qglikelihood
#    + ak5PFJets*ak5PFJetsL1FastL2L3 *qglAK5PF 
    + cmgJetRaw
    + cmgJetR2
    + cmgJet
    ) 

