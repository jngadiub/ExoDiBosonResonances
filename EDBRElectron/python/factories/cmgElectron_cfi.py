import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgLepton_cfi import leptonFactory
electronFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatElectrons"),
       primaryVertexCollection = cms.InputTag("offlinePrimaryVerticesWithBS"),
       leptonFactory = leptonFactory.clone()
#       electronMVAFile = cms.FileInPath("CMGTools/Common/data/TMVA_BDTSimpleCat.weights.xml")
       )

#from CMGTools.Common.selections.isolation_cfi import isomuon

from ExoDiBosonResonances.EDBRElectron.selections.heejjElectronId_cfi import *
from ExoDiBosonResonances.EDBRElectron.selections.eKinematics_cfi import eKinematics
#from ExoDiBosonResonances.EDBRMuon.selections.muontrigger_cfi import muontrigger

cmgElectron = cms.EDFilter("ElectronPOProducer",
    cfg = electronFactory.clone(),
    cuts = cms.PSet(
               kinematics = eKinematics.clone(),
               isoelectron = cms.PSet ( reliso = cms.string('((sourcePtr().get().trackIso() + sourcePtr().get().caloIso())/sourcePtr().get().pt())< 0.15')), #AB: PF iso by defualt, to be corrected by eff area
               HEEPID = HEEPelectronBstdId2012.clone()
               #trigger = muontrigger.clone()
               )
)
