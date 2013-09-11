import FWCore.ParameterSet.Config as cms


from ExoDiBosonResonances.EDBRElectron.factories.cmgDiElectron_cfi import *
from ExoDiBosonResonances.EDBRElectron.skims.cmgDiElectronSel_cfi import *


diElectronSequence = cms.Sequence(
    cmgDiElectronEDBR +
    cmgDiElectronSelEDBR
)


##test Git number 2
