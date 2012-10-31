import FWCore.ParameterSet.Config as cms


from ExoDiBosonResonances.EDBRElectron.factories.cmgDiElectron_cfi import *


diElectronSequence = cms.Sequence(
    cmgDiElectronEDBR +
    cmgDiElectronSelEDBR
)
