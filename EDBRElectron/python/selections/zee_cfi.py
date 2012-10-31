import FWCore.ParameterSet.Config as cms

zee_kinematics = cms.PSet(
    mass = cms.string('mass() >= 70 && mass() < 110'),
    pt = cms.string('leg1().pt() > 40 || leg2().pt() > 40 '),
)
zee_quality  = cms.PSet(
    leg1_wp95 = cms.string('leg1.getSelection(\"cuts_cutBasedVeto_eidEE\") ||  leg1.getSelection(\"cuts_cutBasedVeto_eidEB\")'),
    leg2_wp95 = cms.string('leg2.getSelection(\"cuts_cutBasedVeto_eidEE\") ||  leg2.getSelection(\"cuts_cutBasedVeto_eidEB\")')
)
