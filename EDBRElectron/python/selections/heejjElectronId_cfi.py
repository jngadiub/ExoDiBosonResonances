import FWCore.ParameterSet.Config as cms

#electron identification
def addEleIDSelection(var):
    heejjeleid = cms.PSet(
        selection = cms.string('test_bit(sourcePtr().electronID(\"%s\"),0) ' % var)
    )
    return heejjeleid

mvaTrigEleId = cms.PSet(
    mvaTrigId= cms.string('sourcePtr().electronID(\"mvaTrigV0\")>0.01' )  #to be tuned  
    )

mvaNoTrigEleId = cms.PSet(
    mvaTrigId= cms.string('sourcePtr().electronID(\"mvaNonTrigV0\")>0.01' )    
    )

# see https://twiki.cern.ch/twiki/bin/view/CMS/EgammaCutBasedIdentification
cutBasedVetoEleId = cms.PSet(
    eidEE= cms.PSet( ee = cms.string('sourcePtr().isEE '),
                     sigmaieie  = cms.string('abs(sigmaIetaIeta)<0.03'),
                     dphi = cms.string('abs(deltaPhiSuperClusterTrackAtVtx)<0.7'),
                     deta = cms.string(' abs(deltaEtaSuperClusterTrackAtVtx)<0.01'),
                     dxy  = cms.string(' abs(dxy)<0.04'),
                     dz   = cms.string('abs(dz)<0.2')),
    eidEB= cms.PSet( eb = cms.string('sourcePtr().isEB'),
                     sigmaieie  = cms.string('abs(sigmaIetaIeta)<0.01'),
                     dphi = cms.string('abs(deltaPhiSuperClusterTrackAtVtx)<0.8'),
                     deta = cms.string(' abs(deltaEtaSuperClusterTrackAtVtx)<0.007'),
                     hoe = cms.string(' hadronicOverEm < 0.15 '),
                     dxy  = cms.string('abs(dxy)<0.04'),
                     dz   = cms.string(' abs(dz)<0.2 '))
    )
cutBasedLooseEleId = cms.PSet(
    eidEE=cms.PSet(ee = cms.string('sourcePtr().isEE'),
                     sigmaieie  = cms.string('abs(sigmaIetaIeta)<0.03'),
                     dphi = cms.string('abs(deltaPhiSuperClusterTrackAtVtx)<0.1'),
                     deta = cms.string('abs(deltaEtaSuperClusterTrackAtVtx)<0.01'),
                     dxy  = cms.string(' abs(dxy)<0.02'),
                     dz   = cms.string('abs(dz)<0.2'),
                     hoe = cms.string('hadronicOverEm < 0.10'),
                     eop = cms.string('abs(1.0/energy - 1.0/p)<0.05 ')),
    eidEB=cms.PSet(eb = cms.string('sourcePtr().isEB'),
                   sigmaieie  = cms.string('abs(sigmaIetaIeta)<0.01'),
                   dphi = cms.string('abs(deltaPhiSuperClusterTrackAtVtx)<0.15'),
                   deta = cms.string('abs(deltaEtaSuperClusterTrackAtVtx)<0.007'),
                   hoe = cms.string('hadronicOverEm < 0.12'),
                   dxy  = cms.string(' abs(dxy)<0.02'),
                   dz   = cms.string(' abs(dz)<0.2'),
                   eop = cms.string('abs(1.0/energy - 1.0/p)<0.05 '))
    )

HEEPelectronBstdId2012 = cms.PSet(
    isHEEP = cms.string('sourcePtr().userInt("HEEPId") == 0'),
    isIsolTrk = cms.string('sourcePtr().userIso(0) < 5.0'),
#   Calorimeter Isolation is implemented in an specialized module:
#   (ElectronDetIsoCorrector, with label electronPresel) 
#    isCaloTrk = cms.string('(sourcePtr().userIso(1) + sourcePtr().userIso(2)) < (a*rho + b*et + c)')
    )



HEEPelectronBstdId2012Loose = cms.PSet(  ### now it is the same as HEEPelectronBstdId2012
    isHEEP = cms.string('sourcePtr().userInt("HEEPId") == 0'),
    isIsolTrk = cms.string('sourcePtr().userIso(0) < 5.0'),
#   Calorimeter Isolation is implemented in an specialized module:
#   (ElectronDetIsoCorrector, with label electronPresel) 
#    isCaloTrk = cms.string('(sourcePtr().userIso(1) + sourcePtr().userIso(2)) < (a*rho + b*et + c)')
    ) 
