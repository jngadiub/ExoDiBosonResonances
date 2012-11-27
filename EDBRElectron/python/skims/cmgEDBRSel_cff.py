import FWCore.ParameterSet.Config as cms


cmgEDBRSel = cms.EDFilter(
    "CmgDiElectronDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_overlap\")" )
    )
cmgEDBRSelKinFit = cms.EDFilter(
    "CmgDiElectronDiJetEDBRSelector",
    src = cms.InputTag( "cmgEDBRKinFitWeighted" ),
    cut = cms.string( "!getSelection(\"cuts_overlap\")" )
    )
