import FWCore.ParameterSet.Config as cms



genMatchVQQ = cms.PSet(genMatch = cms.string("leg1.getSelection(\"cuts_genP\") && leg2.getSelection(\"cuts_genQuarks\") && leg2.getSelection(\"cuts_genAntiQuarks\") "))
genMatchVV  = cms.PSet(genMatch = cms.string("leg1.getSelection(\"cuts_genP\") && leg2.getSelection(\"cuts_genJets\")"))
                    
