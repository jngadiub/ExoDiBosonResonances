from HiggsAna.HLLJJCommon.gen.leptons_cff import *
from HiggsAna.HLLJJCommon.gen.z_cff import *
from HiggsAna.HLLJJCommon.gen.h_cff import *
from HiggsAna.HLLJJCommon.gen.hComposite_cff import *


genHistoSequence = cms.Sequence( genLeptonSequence + genZSequence + genHSequence  + genHCompositeSequence)

