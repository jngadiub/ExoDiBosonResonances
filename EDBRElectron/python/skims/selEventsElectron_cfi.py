import FWCore.ParameterSet.Config as cms

electronPreselNoIso = cms.EDFilter(
    "CmgElectronSelector",
    src = cms.InputTag("cmgElectron"),
    cut = cms.string( "getSelection(\"cuts_kinematics\") "
                      +"&& (getSelection(\"cuts_cutBasedLoose_eidEE\")||getSelection(\"cuts_cutBasedLoose_eidEB\") )"
                      )
    )

## isolation cut done by a special module for including PU-corrections
## https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipes
electronPresel = cms.EDProducer(
    "ElectronIsoCorrector",
    src=cms.InputTag('electronPreselNoIso'),
    rho=cms.InputTag('kt6PFJetsForIso:rho'),
    ###rho=cms.InputTag('kt6PFJetsCentral:rho'),
    relisocut=cms.double(0.15),
    EAetaBins=cms.vdouble(0.0, 1.0, 1.479, 2.0, 2.2, 2.3, 2.4, 999.0),
    EAvals=cms.vdouble(0.10,  0.12, 0.085, 0.11, 0.12, 0.12,0.13),
    verbose=cms.bool(False)
    )

selectedElectronSequence = cms.Sequence(electronPreselNoIso+electronPresel)
