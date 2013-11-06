#!/usr/bin/env python

import ROOT as root
from DataFormats.FWLite import Events, Handle
import optparse
import os,sys
import math
from array import array

root.gROOT.SetBatch()        # don't pop up canvases
root.gROOT.SetStyle('Plain') # white background

#weights
weight_Vtag = 0.891 #scale factor for HP category 
weight_btagVeto = 0.915 #efficiency of b-tag veto (flat within 1% for all the masses)
weight_leptonVeto = 0.983 #efficiency of b-tag veto (flat within 2% for all the masses) 
def weight_HLT(flavour, eta): #trigger efficiencies (not applied in MC) X lepton ID
    theWeight = 1
    ###
    if flavour == 1: #ele
        if abs(eta)>0.0 and abs(eta)<1.4442:
            theWeight = 0.991*0.98
        if abs(eta)>1.566 and abs(eta)<2.5:
            theWeight = 0.976*0.98
        if abs(eta)>2.5:
            theWeight = 0.
    ###        
    if flavour == 2: #mu
        if abs(eta)>0.0 and abs(eta)<0.9:
            theWeight = 0.94010*0.99
        if abs(eta)>0.9 and abs(eta)<1.2:
            theWeight = 0.84368*0.99
        if abs(eta)>1.2 and abs(eta)<2.1:
            theWeight = 0.82423*0.99
        if abs(eta)>2.1:
            theWeight = 0.
    ###             
    return theWeight;


#histos
elebins_pt  = [20,40,60,90,120,150,200,250,300,400,500,600,700,800,900,1000,1200,1500,2000]
elebins_eta = [0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.5,2.0,2.5,3.0]

jetbins_pt =  [30,50,80,120,150,200,250,300,400,500,600,700,800,900,1000,1200,1500,2000]
jetbins_eta=  [0.,0.3,0.9,1.2,1.5,1.8,2.1,2.4]

mubins_pt  =  elebins_pt
mubins_eta =  [0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.5,2.0,2.4,3.0]

histo_ele_gen     = root.TH2F("ele_gen","ele_gen",len(elebins_pt)-1,array('d',elebins_pt),len(elebins_eta)-1,array('d',elebins_eta))
histo_ele_gen.Sumw2()
histo_ele_genMatch = histo_ele_gen.Clone("ele_genreco")  # gen gen quantitites eles matched between reco and gen
histo_ele_reco     = histo_ele_gen.Clone("ele_reco")     # reco quantities for all passing objects
histo_ele_recoMatch= histo_ele_gen.Clone("ele_recoMatch")# reco quantities for eles matched to gen-level
histo_ele_pur      = histo_ele_gen.Clone("ele_pur")      # reco quantities for eles matched to gen-level, goint to the same histogram bin
histo_ele_stab     = histo_ele_gen.Clone("ele_stab")     # gen  quantities for eles matched to gen-level, goint to the same histogram bin

histo_mu_gen     = root.TH2F("mu_gen","mu_gen",len(mubins_pt)-1,array('d',mubins_pt),len(mubins_eta)-1,array('d',mubins_eta))    
histo_mu_gen.Sumw2()
histo_mu_genMatch = histo_mu_gen.Clone("mu_genreco")  # gen gen quantitites mus matched between reco and gen
histo_mu_reco     = histo_mu_gen.Clone("mu_reco")     # reco quantities for all passing objects
histo_mu_recoMatch= histo_mu_gen.Clone("mu_recoMatch")# reco quantities for mus matched to gen-level
histo_mu_pur      = histo_mu_gen.Clone("mu_pur")      # reco quantities for mus matched to gen-level, goint to the same histogram bin
histo_mu_stab     = histo_mu_gen.Clone("mu_stab")     # gen  quantities for mus matched to gen-level, goint to the same histogram bin

histo_tautoele_gen     = root.TH2F("tautoele_gen","tautoele_gen",len(elebins_pt)-1,array('d',elebins_pt),len(elebins_eta)-1,array('d',elebins_eta))
histo_tautoele_gen.Sumw2()
histo_tautoele_genMatch = histo_tautoele_gen.Clone("tautoele_genreco")  # gen gen quantitites eles matched between reco and gen

histo_tautomu_gen     = root.TH2F("tautomu_gen","tautomu_gen",len(mubins_pt)-1,array('d',mubins_pt),len(mubins_eta)-1,array('d',mubins_eta))    
histo_tautomu_gen.Sumw2()
histo_tautomu_genMatch = histo_tautomu_gen.Clone("tautomu_genreco")  # gen gen quantitites mus matched between reco and gen

histo_jet_gen      = root.TH2F("jet_gen","jet_gen",len(jetbins_pt)-1,array('d',jetbins_pt),len(jetbins_eta)-1,array('d',jetbins_eta)) # gen quantities of all resonances in accptance   
histo_jet_gen.Sumw2()
histo_jet_genMatch = histo_jet_gen.Clone("jet_genreco")  # gen gen quantitites jets matched between reco and gen
histo_jet_reco     = histo_jet_gen.Clone("jet_reco")     # reco quantities for all passing objects
histo_jet_recoMatch= histo_jet_gen.Clone("jet_recoMatch")# reco quantities for jets matched to gen-level
histo_jet_pur      = histo_jet_gen.Clone("jet_pur")      # reco quantities for jets matched to gen-level, goint to the same histogram bin
histo_jet_stab     = histo_jet_gen.Clone("jet_stab")     # gen  quantities for jets matched to gen-level, goint to the same histogram bin

histo_jet_deltaR = root.TH1F("histo_jet_deltaR","histo_jet_deltaR",100,0,5)
histo_jet_deltaR.Sumw2()
histo_ele_deltaR = root.TH1F("histo_ele_deltaR","histo_ele_deltaR",100,0,5)
histo_ele_deltaR.Sumw2()
histo_mu_deltaR  = root.TH1F("histo_mu_deltaR","histo_mu_deltaR",100,0,5)    
histo_mu_deltaR.Sumw2()
histo_tautoele_deltaR = root.TH1F("histo_tautoele_deltaR","histo_tautoele_deltaR",100,0,5)
histo_tautoele_deltaR.Sumw2()
histo_tautomu_deltaR  = root.TH1F("histo_tautomu_deltaR","histo_tautomu_deltaR",100,0,5)    
histo_tautomu_deltaR.Sumw2()

histo_event_eff    = root.TH1F("histo_event_eff","histo_event_eff",1,0,1) # b-tag , lepton-veto
histo_event_eff.Sumw2()
histo_event_eff.SetBinContent(1,weight_btagVeto*weight_leptonVeto)

#delta-R computation copied from CMSSW

def deltaPhi(phi1, phi2):

    M_PI = 3.1415926535897
    
    result = phi1 - phi2
    while (result > M_PI):
        result -= 2*M_PI
    while (result <= -M_PI):
        result += 2*M_PI

    return result;


def deltaR2(eta1,phi1,eta2,phi2):
    deta = eta1 - eta2
    dphi = deltaPhi(phi1, phi2)
    return deta*deta + dphi*dphi

def deltaR(eta1, phi1, eta2, phi2):
    return math.sqrt(deltaR2(eta1, phi1, eta2, phi2))



def desc(filepath):
    descriptor =  os.path.basename(filepath).replace("treeEDBR_","").replace(".root","")
    return descriptor

def processSubsample(file):
    events = Events (file)

    # create handle outside of loop
    genhandle          = Handle ('std::vector<reco::GenParticle>')
    elehandle          = Handle ('std::vector<cmg::DiObject<cmg::Electron,cmg::Neutrino> >')
    muhandle           = Handle ('std::vector<cmg::DiObject<cmg::Muon,cmg::Neutrino> >')
    jethandle          = Handle ('std::vector<cmg::VJet>')
    # like and edm::InputTag
    nevent = 0
    nevent_Wenu = 0
    nevent_Wmunu = 0
    nevent_Wtaunu_tautoele = 0
    nevent_Wtaunu_tautomu = 0       
    
    for event in events:
        nevent += 1
        if nevent % 1000 ==0:
            print "event: " + str(nevent)

        #print str(event.eventAuxiliary().run())
        #print str(event.object().triggerResultsByName("CMG").accept("preselEleMergedPath"))
        # determine generated flavor and get generated kinematics
        havechargedleptons=0   #ele,mu i) from W-->lv or ii) from tau leptonic decay from W-->taunu
        haveneutralleptons=0   #nuele,numu,nutau from W-->lv
        haveneutralleptonsfromtaus=0  #nuele,numu,nutau from tau leptonic decay from W-->taunu
        havejet=0
        haveVlep=0
        flavor=""
        genVlep      = root.TLorentzVector()
        genlepton1p4 = root.TLorentzVector() #ele,mu i) from W-->lv or ii) from tau leptonic decay from W-->taunu
        genlepton1C  = 0
        genlepton2p4 = root.TLorentzVector() #nuele,numu,nutau from W-->lv
        genlepton2C  = 0
        genlepton2p41stFromTau = root.TLorentzVector() #nuele,numu,nutau from tau leptonic decay from W-->taunu (1st)
        genlepton2C1stFromTau = 0
        genlepton2p42ndFromTau = root.TLorentzVector() #nuele,numu,nutau from tau leptonic decay from W-->taunu (2nd)
        genlepton2C2ndFromTau = 0
        genallneutrinos = root.TLorentzVector() #sum of all neutrinos from Wlep decay               
        genjetp4     = root.TLorentzVector()
        event.getByLabel ("genParticles", genhandle)
        genparticles = genhandle.product()
        counter      = 0
        for genp in genparticles:
            counter = counter + 1
            ##ele,mu from W-->lv
            if (abs(genp.pdgId())==11 or abs(genp.pdgId())==13) and abs(genp.mother().pdgId())==24 and genp.status()==3:
                #print "found lepton "+str(genp.pdgId())
                if(abs(genp.pdgId())==11):
                    flavor = "ele"
                if(abs(genp.pdgId())==13):
                    flavor = "mu"
                if havechargedleptons==0:
                    genlepton1p4 = genp.p4()
                    genlepton1C = genp.charge()
                    havechargedleptons=1
            ##nuele,numu from W-->lv        
            if (abs(genp.pdgId())==12 or abs(genp.pdgId())==14 or abs(genp.pdgId())==16) and abs(genp.mother().pdgId())==24 and genp.status()==3:
                if haveneutralleptons==0:
                    genlepton2p4 = genp.p4()
                    genlepton2C = genp.charge()
                    haveneutralleptons=1
            ##ele,mu from tau from decay W-->taunu                
            if (abs(genp.pdgId())==11 or abs(genp.pdgId())==13) and genp.status()==1 and abs(genp.mother().pdgId())==15 and abs(genp.mother().status())==2 and abs(genp.mother().mother().mother().pdgId())==24:
                if(abs(genp.pdgId())==11):
                    flavor = "tautoele"
                if(abs(genp.pdgId())==13):
                    flavor = "tautomu"
                if havechargedleptons==0:
                    genlepton1p4 = genp.p4()
                    genlepton1C = genp.charge()
                    havechargedleptons=1
            ##nuele,numu,nutau from tau decay from W-->taunu                        
            if (abs(genp.pdgId())==12 or abs(genp.pdgId())==14 or abs(genp.pdgId())==16) and genp.status()==1 and abs(genp.mother().pdgId())==15 and abs(genp.mother().status())==2 and abs(genp.mother().mother().mother().pdgId())==24:
                if haveneutralleptonsfromtaus==0:
                    genlepton2p41stFromTau = genp.p4()
                    genlepton2C1stFromTau = genp.charge()
                    haveneutralleptonsfromtaus=1
                elif haveneutralleptonsfromtaus==1:    
                    genlepton2p42ndFromTau = genp.p4()
                    genlepton2C2ndFromTau = genp.charge()
                    haveneutralleptonsfromtaus=2
            ##W-->qq'        
            if abs(genp.pdgId())==24 and genp.numberOfDaughters()>0 and abs(genp.daughter(1).pdgId())<7 and genp.status()==3:
                genjetp4=genp.p4()
                if havejet==1: ##exit from loop over gen particles when two hadronic bosons are found since not interesting decays (done to save CPU time)
                    havejet=2
                    break
                havejet=1
            ##W-->lv    
            if abs(genp.pdgId())==24 and genp.numberOfDaughters()>0 and abs(genp.daughter(1).pdgId())>7 and genp.status()==3:
                genVlep=genp.p4()
                haveVlep=1

            ##speed up things for "ele" and "mu" cases
            ##(for "tautoele" and "tautomu" cases we need to loop over the entire collection because taus decays at the end)
            if   flavor == "ele" or flavor == "mu":  
                if counter > 25:
                    break                

#            if havechargedleptons==1 and haveneutralleptons==1 and havejet==1 and haveVlep==1:
#                break

        #select only WW semi-leptonic events (including tau-->lvv decays)   
        if havechargedleptons!=1 or haveneutralleptons!=1 or havejet!=1 or haveVlep!=1:
            continue

        #sum all the neutrinos
        if flavor == "ele" or flavor == "mu":
            allneutrinos = genlepton2p4
        if flavor == "tautoele" or flavor == "tautomu":    
            allneutrinos = genlepton2p4 + genlepton2p41stFromTau + genlepton2p42ndFromTau

        if flavor == "ele":
            nevent_Wenu += 1
        if flavor == "mu":
            nevent_Wmunu += 1
        if flavor == "tautoele":
            nevent_Wtaunu_tautoele += 1
        if flavor == "tautomu":
            nevent_Wtaunu_tautomu += 1        

        #print str(nevent)+ "  " + flavor
        # gen-level acceptance cuts:
        if genjetp4.pt()<200: # hadronic V acceptance cut
            continue
        if genjetp4.mass()<65 or genjetp4.mass()>105: # hadronic V acceptance cut
            continue        
        if genVlep.pt()<200: # leptonic V acceptance cut
            continue 
        if flavor == "ele" or flavor == "tautoele": # electron acceptance
            if abs(genlepton1p4.eta())>2.5:
                continue
            if genlepton1p4.pt()<90: 
                continue
            if allneutrinos.pt()<80:
                continue
        if flavor == "mu" or flavor == "tautomu": # muon acceptance
            if abs(genlepton1p4.eta())>2.1: 
                continue
            if genlepton1p4.pt()<50: 
                continue
            if allneutrinos.pt()<40:
                continue
        if abs(genjetp4.eta())>2.4: # jet acceptance
            continue
        #back-to-back topology
        if deltaR(genjetp4.eta(),genjetp4.phi(),genlepton1p4.eta(),genlepton1p4.phi()) < 3.141592/2: 
            continue
        if abs( deltaPhi(genjetp4.phi(),allneutrinos.phi()) ) < 2:
            continue
        if abs( deltaPhi(genjetp4.phi(),genVlep.phi()) ) < 2:
            continue

        #fill pure gen info
        if flavor == "ele":
            histo_ele_gen.Fill(genVlep.pt(),abs(genVlep.Eta()))
        if flavor == "mu":
            histo_mu_gen.Fill(genVlep.pt(),abs(genVlep.Eta()))
        if flavor == "tautoele":
            histo_tautoele_gen.Fill(genVlep.pt(),abs(genVlep.Eta()))
        if flavor == "tautomu":
            histo_tautomu_gen.Fill(genVlep.pt(),abs(genVlep.Eta()))
        histo_jet_gen.Fill(genjetp4.pt(),abs(genjetp4.eta()))

#         if(event.eventAuxiliary().event() ==  38434):# strange fualty event
#             continue
#         if(event.eventAuxiliary().event() ==  38491):# strange fualty event
#             continue
#         print str(event.eventAuxiliary().event())

        # fill matched jets objects
        event.getByLabel ("jetIDMerged", jethandle)
        recojets = jethandle.product()
        closest = -1
        closestDr = 99999.
        index = -1
        for jet in recojets:
            index += 1
            #acceptance cuts
            if jet.prunedMass() < 65 or jet.prunedMass() > 105:
                continue
            if jet.pt() < 200:
                continue
            if abs(jet.eta())>2.4:
                continue
            if not jet.getSelection("cuts_looseJetId"):
                continue
            if not jet.getSelection("cuts_TOBTECjetsId"):
                continue
            if jet.sourcePtr().get().userFloat("tau2")/jet.sourcePtr().get().userFloat("tau1") > 0.5:
                continue
            
            histo_jet_reco.Fill(jet.pt(),abs(jet.eta()),weight_Vtag)
            dr = deltaR(jet.eta(),jet.phi(),genjetp4.eta(),genjetp4.phi())
            histo_jet_deltaR.Fill(dr,weight_Vtag)
            if dr < closestDr: 
                closestDr = dr
                closest = index

        if index != -1 and closestDr < 0.8: # found a matching VJet
            histo_jet_genMatch.Fill(genjetp4.pt(),abs(genjetp4.eta()),weight_Vtag)
            #histo_jet_recoMatch.Fill(recojets[index].pt(),abs(recojets[index].eta()),weight_Vtag)
            if histo_jet_genMatch.FindBin(genjetp4.pt(),abs(genjetp4.eta())) == histo_jet_genMatch.FindBin(recojets[index].pt(),abs(recojets[index].eta())):
                histo_jet_stab.Fill(genjetp4.pt(),abs(genjetp4.eta()),weight_Vtag)
                histo_jet_pur.Fill(recojets[index].pt(),abs(recojets[index].eta()),weight_Vtag)

        #only proceed to leptons if event filters pass
        #this ensures trigger eff is taken into account
        #other event fitlers could go anywhere but need to be restricted to
        #either leptons or jets to avoid double counting
#        if not event.object().triggerResultsByName("CMG").accept("eventFilterPath"):
#            #print "failed trigger"
#            continue

        #print "startele"
        # fill matched electrons objects
        if flavor == "ele" or flavor == "tautoele":
            event.getByLabel ("WelenuCand", elehandle)
            recoeles = elehandle.product()
            closest1 = -1
            closestDr1 = 99999.
            index1 = -1
            closestVlepWeight1 = 1.
            for ele in recoeles:
                index1 += 1
                #acceptance cuts
                if ele.pt() < 200:
                    continue
                if ele.leg2().getleppt() < 80:
                    continue            

                VlepWeight1 = weight_HLT(1,ele.leg1().eta())
                #print str(flavor) + " " + str(ele.leg1().eta()) + " " + str(VlepWeight1)

                dr1 = deltaR(ele.eta(),ele.phi(),genVlep.eta(),genVlep.phi())
                if flavor == "ele":
                    histo_ele_reco.Fill(ele.pt(),abs(ele.eta()), VlepWeight1)                
                    histo_ele_deltaR.Fill(dr1,VlepWeight1)
                if flavor == "tautoele":                    
                    histo_tautoele_deltaR.Fill(dr1,VlepWeight1)                    
                if dr1 < closestDr1 :
                    closestDr1 = dr1
                    closest1 = index1
                    closestVlepWeight1 = VlepWeight1
            
            if index1 != -1 and closestDr1 < 1.0: # found a matching electron for genlepton1
                if flavor == "ele":
                    histo_ele_genMatch.Fill(genVlep.pt(),abs(genVlep.Eta()),closestVlepWeight1)
                    #histo_ele_recoMatch.Fill(recoeles[index1].pt(),abs(recoeles[index1].eta()),closestVlepWeight1)
                    if histo_ele_genMatch.FindBin(genVlep.pt(),abs(genVlep.Eta())) == histo_ele_genMatch.FindBin(recoeles[index1].pt(),abs(recoeles[index1].eta())):
                        histo_ele_stab.Fill(genVlep.pt(),abs(genVlep.Eta()),closestVlepWeight1)
                        histo_ele_pur.Fill(recoeles[index1].pt(),abs(recoeles[index1].eta()),closestVlepWeight1)
                if flavor == "tautoele":
                    histo_tautoele_genMatch.Fill(genVlep.pt(),abs(genVlep.Eta()),closestVlepWeight1)
           
       # fill matched muon objects
        if flavor == "mu" or flavor == "tautomu":
            event.getByLabel ("WmunuCand", muhandle)
            recomus = muhandle.product()
            closest1 = -1
            closestDr1 = 99999.
            index1 = -1
            closestVlepWeight1 = 1.
            for mu in recomus:
                index1 += 1
            #acceptance cuts
                if mu.pt() < 200:
                    continue

                VlepWeight1 = weight_HLT(2,mu.leg1().eta())
                #print str(flavor) + " " + str(mu.leg1().eta()) + " " + str(VlepWeight1)

                dr1 = deltaR(mu.eta(),mu.phi(),genVlep.eta(),genVlep.phi())
                if flavor == "mu":                
                    histo_mu_reco.Fill(mu.pt(),abs(mu.eta()),VlepWeight1)
                    histo_mu_deltaR.Fill(dr1,VlepWeight1)
                if flavor == "tautomu":
                    histo_tautomu_deltaR.Fill(dr1,VlepWeight1)
                if dr1 < closestDr1 :
                    closestDr1 = dr1
                    closest1 = index1
                    closestVlepWeight1 = VlepWeight1

            if index1 != -1 and closestDr1 < 1.0: # found a matching muon for genlepton1
                if flavor == "mu":
                    histo_mu_genMatch.Fill(genVlep.pt(),abs(genVlep.Eta()),closestVlepWeight1)
                    #histo_mu_recoMatch.Fill(recomus[index1].pt(),abs(recomus[index1].eta()),closestVlepWeight1)
                    if histo_mu_genMatch.FindBin(genVlep.pt(),abs(genVlep.Eta())) == histo_mu_genMatch.FindBin(recomus[index1].pt(),abs(recomus[index1].eta())):
                        histo_mu_stab.Fill(genVlep.pt(),abs(genVlep.Eta()),closestVlepWeight1)
                        histo_mu_pur.Fill(recomus[index1].pt(),abs(recomus[index1].eta()),closestVlepWeight1)
                if flavor == "tautomu":
                    histo_tautomu_genMatch.Fill(genVlep.pt(),abs(genVlep.Eta()),closestVlepWeight1)


##############
                    
    print "tot : " + str(nevent)
    print "W-->enu : " + str(nevent_Wenu)
    print "W-->munu : " + str(nevent_Wmunu)
    print "W-->taunu-->enununu : " + str(nevent_Wtaunu_tautoele)
    print "W-->taunu-->munununu : " + str(nevent_Wtaunu_tautomu)

def makePlots():
    canvas = root.TCanvas("c","c",400,400)
    # basic gen distributions
    histo_ele_gen.Draw("COLZ")
    canvas.SaveAs("histo_ele_gen_WW.eps")
    histo_mu_gen.Draw("COLZ")
    canvas.SaveAs("histo_mu_gen_WW.eps")
    histo_jet_gen.Draw("COLZ")
    canvas.SaveAs("histo_jet_gen_WW.eps")

    #purity
    tmphist = histo_jet_pur.Clone("tmphist")
    tmphist.Divide(histo_jet_reco)
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_jet_purity_WW.eps")
    tmphist = histo_ele_pur.Clone("tmphist")
    tmphist.Divide(histo_ele_reco)
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_ele_purity_WW.eps")
    tmphist = histo_mu_pur.Clone("tmphist")
    tmphist.Divide(histo_mu_reco)
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_mu_purity_WW.eps")
        
    #stability
    tmphist = histo_jet_stab.Clone("tmphist")
    tmphist.Divide(histo_jet_gen)
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_jet_stability_WW.eps")
    tmphist = histo_ele_stab.Clone("tmphist")
    tmphist.Divide(histo_ele_gen)
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_ele_stability_WW.eps")
    tmphist = histo_mu_stab.Clone("tmphist")
    tmphist.Divide(histo_mu_gen)
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_mu_stability_WW.eps")
        
    #eff
    output = root.TFile.Open("efficiency_WW.root","RECREATE")
    histo_event_eff.Write()
    tmphist = histo_jet_genMatch.Clone("eff_jet")
    tmphist.Divide(histo_jet_gen)
    histo_jet_genMatch.Write()
    histo_jet_gen.Write()
    tmphist.Write()
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_jet_efficiency_WW.eps")
    tmphist = histo_ele_genMatch.Clone("eff_ele")
    tmphist.Divide(histo_ele_gen)
    histo_ele_genMatch.Write()
    histo_ele_gen.Write()
    tmphist.Write()
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_ele_efficiency_WW.eps")
    tmphist = histo_mu_genMatch.Clone("eff_mu")
    tmphist.Divide(histo_mu_gen)
    histo_mu_genMatch.Write()
    histo_mu_gen.Write()
    tmphist.Write()
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_mu_efficiency_WW.eps")
    tmphist = histo_tautoele_genMatch.Clone("eff_tautoele")
    tmphist.Divide(histo_tautoele_gen)
    histo_tautoele_genMatch.Write()
    histo_tautoele_gen.Write()
    tmphist.Write()
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_tautoele_efficiency_WW.eps")
    tmphist = histo_tautomu_genMatch.Clone("eff_tautomu")
    tmphist.Divide(histo_tautomu_gen)
    histo_tautomu_genMatch.Write()
    histo_tautomu_gen.Write()
    tmphist.Write()
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_tautomu_efficiency_WW.eps")
    output.Close()

    #deltaR    
    histo_jet_deltaR.Draw()
    canvas.SaveAs("histo_jet_deltaR_WW.root")
    histo_ele_deltaR.Draw()
    canvas.SaveAs("histo_ele_deltaR_WW.root")
    histo_mu_deltaR.Draw()
    canvas.SaveAs("histo_mu_deltaR_WW.root")
    histo_tautoele_deltaR.Draw()
    canvas.SaveAs("histo_tautoele_deltaR_WW.root")
    histo_tautomu_deltaR.Draw()
    canvas.SaveAs("histo_tautomu_deltaR_WW.root")
    

def main():
    parser = optparse.OptionParser(description='Signal Shape Fitting Tool')
    parser.add_option('-f','--filepath',     help='path to input trees, default: ../trees'                             , default = "../trees" )                                      
    parser.add_option('-b','--bosonflavor',  help='boson flavor, ZZ or WW, default: ZZ'                             , default = "ZZ" )                                      

    (args,XXX) = parser.parse_args()
    
    root.gROOT.SetBatch(True)

    print 'PRINTING ARGUMENTS:'
    print args

    filelist = []
    # do we run on a single file or a whole diretory?
    if os.path.isfile(args.filepath): # single file
        if ".root" in args.filepath:
            filelist.append(args.filepath)
        else :#single fiel is actually a list
            f = open(args.filepath)            
            for line in iter(f):
                filelist.append(line.rstrip())
            
    else: #lets run on all suitable files in this directory
        for file in os.listdir(args.filepath):
            if(("BulkG_"+args.bosonflavor) in desc(file) ): # found a signal MC file
                filelist.append(args.filepath + "/" +  file)

    #print filelist
    #sys.exit(0)
                
    for file in filelist:
        print 'File is ',file
        processSubsample(file)
            

    makePlots()


if __name__ == "__main__":
    main()
