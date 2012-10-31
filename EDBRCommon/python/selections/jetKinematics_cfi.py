import FWCore.ParameterSet.Config as cms


jetKinematics = cms.PSet(
    pt = cms.string('pt() > 30'),
    eta = cms.string('abs(eta()) < 2.4'),
    phi = cms.string('abs(phi()) < 3.2')
    )


zjj = cms.PSet(
    mass = cms.string('mass() >= 60 && mass() < 130'),
)
isSignal = cms.PSet(
    mass = cms.string('mass() >= 75 && mass() < 105'),
)
isSideband = cms.PSet(
    mass = cms.string('(mass() >= 60 && mass() < 75) || ( mass() >= 105 && mass() < 130 )'),
)

