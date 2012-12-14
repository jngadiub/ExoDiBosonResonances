import FWCore.ParameterSet.Config as cms

zmumu = cms.PSet(
    mass = cms.string('mass() >= 70 && mass() < 110'),
    leg1_kinematics = cms.PSet(
                               pt = cms.string('(leg1().pt() > 20 && leg2().pt() > 40)||(leg1().pt() > 40 && leg2().pt() > 20)'),
                               )
)

HPTmuGlob = cms.PSet(
    isGB=cms.string('leg1().getSelection(\"cuts_HPTGBmuon\") || leg2().getSelection(\"cuts_HPTGBmuon\")')
    )
