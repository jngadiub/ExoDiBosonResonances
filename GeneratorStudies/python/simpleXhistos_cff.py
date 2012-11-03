import FWCore.ParameterSet.Config as cms


histograms = cms.VPSet(cms.PSet(
    # Basic kinematics
        nbins = cms.untracked.int32(150),
        description = cms.untracked.string('X_%d_et'),
        plotquantity = cms.untracked.string('et'),
        min = cms.untracked.double(0.0),
        max = cms.untracked.double(1500.0),
        itemsToPlot = cms.untracked.int32(2),
        name = cms.untracked.string('X_%d_et')
        ),
                       cms.PSet(
    nbins = cms.untracked.int32(100),
    description = cms.untracked.string('X_%d_eta'),
    plotquantity = cms.untracked.string('eta'),
    min = cms.untracked.double(-4.0),
    max = cms.untracked.double(4.0),
    itemsToPlot = cms.untracked.int32(2),
    name = cms.untracked.string('X_%d_eta')
    ),
                       cms.PSet(
    nbins = cms.untracked.int32(150),
    description = cms.untracked.string('X_%d_pt'),
    plotquantity = cms.untracked.string('pt'),
    min = cms.untracked.double(0.0),
    max = cms.untracked.double(1500.0),
    itemsToPlot = cms.untracked.int32(2),
                    name = cms.untracked.string('X_%d_pt')
    ),
                       # X mass
                       cms.PSet(
    nbins = cms.untracked.int32(400),
    description = cms.untracked.string('X_%d_mass'),
    plotquantity = cms.untracked.string('mass'),
    min = cms.untracked.double(0.0),
    max = cms.untracked.double(2000.0),
    itemsToPlot = cms.untracked.int32(2),
    name = cms.untracked.string('X_%d_mass')
    ))
