from CMGTools.Common.electron_cff import *
from ExoDiBosonResonances.EDBRElectron.selections.heejjElectronId_cfi import *
from ExoDiBosonResonances.EDBRElectron.selections.eKinematics_cfi import eKinematics

genSelectorZDaughterE = cms.EDFilter("GenParticleSelector",
    src = cms.InputTag("genParticles"),
    cut = cms.string(' (abs(pdgId)==11 )&& abs(mother.pdgId)==23 ')
)

selectedPatElectrons  = cms.EDProducer("PATElectronCleaner",
                                        src = cms.InputTag("patElectronsWithTrigger"), #selectedPatElectronsAK5
                                        preselection = cms.string(''),
                                        checkOverlaps = cms.PSet( genLeptons = cms.PSet( src = cms.InputTag("genSelectorZDaughterE"),
                                                                                         algorithm = cms.string("byDeltaR"),
                                                                                         preselection        = cms.string(""),  # don't preselect the muons
                                                                                         deltaR              = cms.double(0.3),
                                                                                         checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                                         pairCut             = cms.string(""),
                                                                                         requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                                         ),
                                                                  ),                                      
                                        finalCut = cms.string(''),
                                        )



cmgElectron.cfg.inputCollection="selectedPatElectrons"

# ele ID requirements
cmgElectron.cuts.mvaTrigSel =mvaTrigEleId.clone() 
cmgElectron.cuts.wp95c = wp95cEleId.clone()
cmgElectron.cuts.cutBasedVeto = cutBasedVetoEleId.clone()
cmgElectron.cuts.cutBasedLoose = cutBasedLooseEleId.clone()
cmgElectron.cuts.conversionVeto = cms.PSet(
    patFlag=cms.string('sourcePtr().passConversionVeto()')    
    )

### Isolation requirements (also embedded in CombIsoVBTFXX and wp95c)
cmgElectron.cuts.reliso = cms.PSet ( reliso = cms.string(" (sourcePtr.get.dr03HcalTowerSumEt + sourcePtr.get.dr03EcalRecHitSumEt + sourcePtr.get.dr03TkSumPt) / et < 0.15 ") )


### kinematic requirements
cmgElectron.cuts.kinematics = eKinematics.clone()

### MC matching
cmgElectron.cuts.genLepton = cms.PSet( genLepton = cms.string("sourcePtr().get().hasOverlaps('genLeptons')"))
 
eleSequence = cms.Sequence(genSelectorZDaughterE + selectedPatElectrons + cmgElectron)

