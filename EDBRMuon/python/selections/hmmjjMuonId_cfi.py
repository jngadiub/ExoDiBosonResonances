import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.vbtfmuon_cfi import vbtfmuon

pfmuonId2012 = cms.PSet(
    isGlobal = cms.string('isGlobalMuon()'),
    isPF = cms.string('isPF()'),
    #Updated for 2012 with new cut from the Muon POG
    #numberOfValidTrackerHits = cms.string('numberOfValidTrackerHits() > 10'),
    trackerLayersWithMeasurement = cms.string('trackerLayersWithMeasurement() > 5'),
    numberOfValidPixelHits = cms.string('numberOfValidPixelHits() > 0'),
    numberOfValidMuonHits = cms.string('numberOfValidMuonHits() > 0'),
    numberOfMatches = cms.string('numberOfMatches() > 1'),
    normalizedChi2 = cms.string('normalizedChi2() < 10'),
 #   dxy = cms.string('abs(dB(pat::Muon::PV3D)) < 0.2'),
    dxy = cms.string('abs(dxy()) < 0.2'),
    dz = cms.string('abs(dz()) < 0.5')
    )
