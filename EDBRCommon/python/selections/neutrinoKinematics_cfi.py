import FWCore.ParameterSet.Config as cms


neuKinematics = cms.PSet(
    pt = cms.string('pt() > 30.0'),
#    eta = cms.string('abs(eta()) < 2.4'),
#    phi = cms.string('abs(phi()) < 3.2')
    )
