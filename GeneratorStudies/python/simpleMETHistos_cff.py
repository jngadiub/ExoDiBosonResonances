import FWCore.ParameterSet.Config as cms

histograms = cms.VPSet(cms.PSet(
    # Basic kinematics
        nbins = cms.untracked.int32(150),
        description = cms.untracked.string('MET_%d_et'),
        plotquantity = cms.untracked.string('et'),
        min = cms.untracked.double(0.0),
        max = cms.untracked.double(1500.0),
        itemsToPlot = cms.untracked.int32(1),
        name = cms.untracked.string('MET_%d_et')
        ),
                       cms.PSet(
    nbins = cms.untracked.int32(72),
    description = cms.untracked.string('MET_%d_phi'),
    plotquantity = cms.untracked.string('phi'),
    min = cms.untracked.double(-3.14159265359),
    max = cms.untracked.double(3.14159265359),
    itemsToPlot = cms.untracked.int32(1),
    name = cms.untracked.string('MET_%d_phi')
    ))
