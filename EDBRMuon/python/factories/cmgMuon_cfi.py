import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgLepton_cfi import leptonFactory
muonFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatMuons"),
       trackType = cms.int32(0),#use the global track,
       #the flag for muonID, see DataFormats/MuonReco/interface/MuonSelectors.h
       muonIDType = cms.string("AllGlobalMuons"),
       leptonFactory = leptonFactory.clone()
       )

#from CMGTools.Common.selections.isolation_cfi import isomuon

from ExoDiBosonResonances.EDBRMuon.selections.hmmjjMuonId_cfi import vbtfmuon,pfmuonId2012
from ExoDiBosonResonances.EDBRMuon.selections.muKinematics_cfi import muKinematics
#from ExoDiBosonResonances.EDBRMuon.selections.muontrigger_cfi import muontrigger

cmgMuon = cms.EDFilter("MuonPOProducer",
    cfg = muonFactory.clone(),
    cuts = cms.PSet(
               kinematics = muKinematics.clone(),
               isomuon = cms.PSet ( reliso = cms.string('((sourcePtr().get().trackIso() + sourcePtr().get().caloIso())/sourcePtr().get().pt())< 0.15')), #AB: PF iso by defualt, to be corrected by eff area
               vbtfmuon = vbtfmuon.clone(),
               tightPFmuon = pfmuonId2012.clone()
#               trigger = muontrigger.clone()
       )    
)
