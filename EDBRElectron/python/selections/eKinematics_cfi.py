import FWCore.ParameterSet.Config as cms

eKinematics = cms.PSet(
    pt = cms.string('pt() > 20'),
    eta = cms.string('abs(eta()) < 2.5'),
    gap = cms.string('!( abs(sourcePtr().get().superCluster().get().eta()) < 1.566 && abs(sourcePtr().get().superCluster().get().eta()) > 1.4442)'),
    phi = cms.string('abs(phi()) < 3.2')
    )

eKinematicsGen = eKinematics.clone()
eKinematicsGen.gap=('!( abs(eta()) < 1.566 && abs(eta()) > 1.4442)')
    

