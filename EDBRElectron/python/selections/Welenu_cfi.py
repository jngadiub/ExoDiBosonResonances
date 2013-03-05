import FWCore.ParameterSet.Config as cms 

Welenu_kinematics = cms.PSet(
    mass = cms.string('mass() >= 77.4 && mass() < 83.4'),
    pt = cms.string('leg1().pt() > 90 && leg2().pt() > 40 '),
)
Welenu_quality  = cms.PSet(
    leg1_wp95 = cms.string('leg1.getSelection(\"cuts_cutBasedVeto_eidEE\") ||  leg1.getSelection(\"cuts_cutBasedVeto_eidEB\")')
    #leg2_wp95 = cms.string('leg2.getSelection(\"cuts_cutBasedVeto_eidEE\") ||  leg2.getSelection(\"cuts_cutBasedVeto_eidEB\")')
)
Welenu_validation  = cms.PSet(
    ptmatch = cms.string('(leg1().pt()-leg2().getleppt())/leg1().pt()<0.01&&(leg1().pt()-leg2().getleppt())/leg1().pt()>-0.01'),
	etamatch = cms.string('(leg1().eta()-leg2().getlepeta())/leg1().eta()<0.01&&(leg1().eta()-leg2().getlepeta())/leg1().eta()>-0.01')
)
