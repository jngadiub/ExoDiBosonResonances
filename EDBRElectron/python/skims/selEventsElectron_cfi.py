import FWCore.ParameterSet.Config as cms

electronPreselNoIso = cms.EDFilter(
    "CmgElectronSelector",
    src = cms.InputTag("cmgElectron"),
    cut = cms.string( "getSelection(\"cuts_kinematics\") "
                      +"&& getSelection(\"cuts_HEEPID\") " )
    )

## isolation cut done by a special module for including PU-corrections
## https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipes
## Modified by Thiago: specialized module for electron iso.
## (See https://twiki.cern.ch/twiki/bin/viewauth/CMS/HEEPElectronID)
electronPresel = cms.EDProducer(
    "ElectronDetIsoCorrector",
    src=cms.InputTag('electronPreselNoIso'),
    rho=cms.InputTag('kt6PFJetsForIso:rho'),
    constantTermBarrel = cms.double(2.0),
    linearTermBarrel = cms.double(0.03),
    constantTermEndcap = cms.double(2.5),
    linearTermEndcap = cms.double(0.03),
    linearThresholdEndcap = cms.double(50.0),
    ###rho=cms.InputTag('kt6PFJetsCentral:rho'),
#    EAetaBins=cms.vdouble(0.0, 1.0, 1.479, 2.0, 2.2, 2.3, 2.4, 999.0),
    EAetaBins=cms.vdouble(0.0, 999.0),
#    EAvals=cms.vdouble(0.10,  0.12, 0.085, 0.11, 0.12, 0.12,0.13),
    EAvals=cms.vdouble(0.28),
    verbose=cms.bool(False)
    )

electronPreselNoIsoLoose = cms.EDFilter(
    "CmgElectronSelector",
    src = cms.InputTag("cmgElectron"),
    cut = cms.string( "getSelection(\"cuts_kinematics\") "
                      +"&& getSelection(\"cuts_HEEPIDloose\") " )
    )   

## isolation cut done by a special module for including PU-corrections
## https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipes
## Modified by Thiago: specialized module for electron iso.
## (See https://twiki.cern.ch/twiki/bin/viewauth/CMS/HEEPElectronID)
electronPreselLoose = cms.EDProducer(
    "ElectronDetIsoCorrector",
    src=cms.InputTag('electronPreselNoIsoLoose'),
    rho=cms.InputTag('kt6PFJetsForIso:rho'),
    constantTermBarrel = cms.double(2.0),
    linearTermBarrel = cms.double(0.03),
    constantTermEndcap = cms.double(2.5),
    linearTermEndcap = cms.double(0.03),
    linearThresholdEndcap = cms.double(50.0),
    ###rho=cms.InputTag('kt6PFJetsCentral:rho'),
#    EAetaBins=cms.vdouble(0.0, 1.0, 1.479, 2.0, 2.2, 2.3, 2.4, 999.0),
    EAetaBins=cms.vdouble(0.0, 999.0),
#    EAvals=cms.vdouble(0.10,  0.12, 0.085, 0.11, 0.12, 0.12,0.13),
    EAvals=cms.vdouble(0.28),
    verbose=cms.bool(False)
    ) 



selectedElectronCandFilter = cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag('electronPresel'),
   minNumber = cms.uint32(0)
 )


selectedElectronLooseCandFilter = cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag('electronPreselLoose'),
   minNumber = cms.uint32(1),
   maxNumber = cms.uint32(1)
 )

selectedElectronSequence = cms.Sequence(electronPreselNoIso+electronPresel+selectedElectronCandFilter)
selectedElectronLooseSequence = cms.Sequence(electronPreselNoIsoLoose+electronPreselLoose+selectedElectronLooseCandFilter)

