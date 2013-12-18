#!/usr/bin/env python

import ROOT as root
from DataFormats.FWLite import Events, Handle
import optparse
import os,sys
import math
from array import array

root.gROOT.SetBatch()        # don't pop up canvases
root.gROOT.SetStyle('Plain') # white background

elebins_pt  = [20,40,60,90,120,150,200,250,300,400,500,600,700,800,900,1000,1200,1500,2000]
elebins_eta = [0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.5,2.0,2.5,3.0]

jetbins_pt =  [30,50,80,120,150,200,250,300,400,500,600,700,800,900,1000,1200,1500,2000]
jetbins_eta=  [0.,0.3,0.9,1.2,1.5,1.8,2.1,2.4]

mubins_pt  =  elebins_pt
mubins_eta =  [0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.5,2.0,2.4,3.0]

deltaRbins   =  [0.0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.6,0.7,0.8,0.9,1.0,1.5,2.0,3.]
costhetabins =  [-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]

histo_ele_gen     = root.TH2F("ele_gen","ele_gen",len(elebins_pt)-1,array('d',elebins_pt),len(elebins_eta)-1,array('d',elebins_eta))    
histo_ele_genMatch = histo_ele_gen.Clone("ele_genreco")  # gen gen quantitites eles matched between reco and gen
histo_ele_reco     = histo_ele_gen.Clone("ele_reco")     # reco quantities for all passing objects
histo_ele_recoMatch= histo_ele_gen.Clone("ele_recoMatch")# reco quantities for eles matched to gen-level
histo_ele_pur      = histo_ele_gen.Clone("ele_pur")      # reco quantities for eles matched to gen-level, goint to the same histogram bin
histo_ele_stab     = histo_ele_gen.Clone("ele_stab")     # gen  quantities for eles matched to gen-level, goint to the same histogram bin

histo_mu_gen     = root.TH2F("mu_gen","mu_gen",len(mubins_pt)-1,array('d',mubins_pt),len(mubins_eta)-1,array('d',mubins_eta))    
histo_mu_genMatch = histo_mu_gen.Clone("mu_genreco")  # gen gen quantitites mus matched between reco and gen
histo_mu_reco     = histo_mu_gen.Clone("mu_reco")     # reco quantities for all passing objects
histo_mu_recoMatch= histo_mu_gen.Clone("mu_recoMatch")# reco quantities for mus matched to gen-level
histo_mu_pur      = histo_mu_gen.Clone("mu_pur")      # reco quantities for mus matched to gen-level, goint to the same histogram bin
histo_mu_stab     = histo_mu_gen.Clone("mu_stab")     # gen  quantities for mus matched to gen-level, goint to the same histogram bin

histo_jet_gen      = root.TH2F("jet_gen","jet_gen",len(jetbins_pt)-1,array('d',jetbins_pt),len(jetbins_eta)-1,array('d',jetbins_eta)) # gen quantities of all resonances in accptance   
histo_jet_genMatch = histo_jet_gen.Clone("jet_genreco")  # gen gen quantitites jets matched between reco and gen
histo_jet_reco     = histo_jet_gen.Clone("jet_reco")     # reco quantities for all passing objects
histo_jet_recoMatch= histo_jet_gen.Clone("jet_recoMatch")# reco quantities for jets matched to gen-level
histo_jet_pur      = histo_jet_gen.Clone("jet_pur")      # reco quantities for jets matched to gen-level, goint to the same histogram bin
histo_jet_stab     = histo_jet_gen.Clone("jet_stab")     # gen  quantities for jets matched to gen-level, goint to the same histogram bin

histo_ele_dr = root.TH1F("ele_dr","ele_dr",len(deltaRbins)-1,array('d',deltaRbins))
histo_ele_drSel = histo_ele_dr.Clone("ele_dr_sel")
histo_mu_dr = root.TH1F("mu_dr","mu_dr",len(deltaRbins)-1,array('d',deltaRbins))
histo_mu_drSel = histo_mu_dr.Clone("mu_dr_sel")
histo_jet_dr = root.TH1F("jet_dr","jet_dr",len(deltaRbins)-1,array('d',deltaRbins))
histo_jet_drSel = histo_jet_dr.Clone("jet_dr_sel")

histo_ele_ct = root.TH1F("ele_ct","ele_ct",len(costhetabins)-1,array('d',costhetabins))
histo_ele_ctSel = histo_ele_ct.Clone("ele_ct_sel")
histo_mu_ct = root.TH1F("mu_ct","mu_ct",len(costhetabins)-1,array('d',costhetabins))
histo_mu_ctSel = histo_mu_ct.Clone("mu_ct_sel")
histo_jet_ct = root.TH1F("jet_ct","jet_ct",len(costhetabins)-1,array('d',costhetabins))
histo_jet_ctSel = histo_jet_ct.Clone("jet_ct_sel")


hist_mjet_gen = root.TH1F("mgen","mgen",20,50,130)
hist_mjet_rec = root.TH1F("mrec","mrec",20,50,130)

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
    elehandle          = Handle ('std::vector<cmg::DiObject<cmg::Electron,cmg::Electron> >')
    muhandle           = Handle ('std::vector<cmg::DiObject<cmg::Muon,cmg::Muon> >')
    jethandle          = Handle ('std::vector<cmg::VJet>')
    # like and edm::InputTag
    nevent = 0 
    for event in events:
        nevent += 1
        if nevent % 10000 ==0:
            print "event: " + str(nevent)
            
        #print str(event.eventAuxiliary().run())
        #print str(event.object().triggerResultsByName("CMG").accept("preselEleMergedPath"))
        # determine generated flavor and get generated kinematics
        haveleptons=0
        havejet=0
        havez=0
        flavor=""
        genZlep      = root.TLorentzVector()
        genlepton1p4 = root.TLorentzVector()
        genlepton1C  = 0
        genlepton2p4 = root.TLorentzVector()
        genlepton2C  = 0
        genjetp4     = root.TLorentzVector()
        genq1p4      = root.TLorentzVector()
        genq1C       = 0
        genq2p4      = root.TLorentzVector()
        genq2C       = 0
        event.getByLabel ("genParticles", genhandle)
        event.getByLabel ("genParticles", genhandle)
        genparticles = genhandle.product()

        havetau=0
        for genp in genparticles:
            if abs(genp.pdgId())==23 and genp.status()==3 and genp.numberOfDaughters()>0 and abs(genp.daughter(1).pdgId())<7:
                genjetp4=genp.p4()
                genq1p4=genp.daughter(0).p4()
                genq1C =genp.daughter(0).charge()
                genq2p4=genp.daughter(1).p4()
                genq1C =genp.daughter(1).charge()
                havejet=1

            if haveleptons==2 and havejet==1 and havez==1:
                break

        #only look for hadronic Z with W-tagging cuts
        if  havejet!=1:
            continue 

        if havetau:
            print "passing altough there is a tau"

        #print str(havejet)

        #print str(nevent)+ "  " + flavor
        # gen-level acceptance cuts:
        if genjetp4.pt()<200: # hadronic Z acceptance cut
            continue

        hist_mjet_gen.Fill(genjetp4.mass())
        if genjetp4.mass()<65 or genjetp4.mass()>105: # hadronic W acceptance cut
            continue        
        if abs(genjetp4.eta())>2.4:
            continue


        #compute angle
        zvec = genjetp4
        bv=root.TVector3(-zvec.x()/zvec.t(),-zvec.y()/zvec.t(),-zvec.z()/zvec.t())
        #bv = zvec.BoostVector()
        q1b = root.TLorentzVector(genq1p4.x(),genq1p4.y(),genq1p4.z(),genq1p4.t())
        q2b = root.TLorentzVector(genq2p4.x(),genq2p4.y(),genq2p4.z(),genq2p4.t())
        q1b.Boost(bv)
        q2b.Boost(bv)
        costheta1 = root.TMath.Cos(q1b.Angle(bv))
        costheta2 = root.TMath.Cos(q2b.Angle(bv))

        #print costheta1, costheta2
        if(genq2C <0):
            costhetajet = costheta1
        else:
            costhetajet = costheta2


        histo_jet_gen.Fill(genjetp4.pt(),abs(genjetp4.eta()))
        histo_jet_ct.Fill(costhetajet)
        histo_jet_dr.Fill(deltaR(genq1p4.eta(),genq1p4.phi(),genq2p4.eta(),genq2p4.phi()))
                                                                                 
        #print str(event.eventAuxiliary().event())
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

            
            histo_jet_reco.Fill(jet.pt(),abs(jet.eta()))
            dr = deltaR(jet.eta(),jet.phi(),genjetp4.eta(),genjetp4.phi())
            if dr < closestDr:
                closestDr = dr
                closest = index

        if index != -1 and closestDr < 0.8: # found a matching VJet
            hist_mjet_rec.Fill(recojets[closest].prunedMass())

            histo_jet_genMatch.Fill(genjetp4.pt(),abs(genjetp4.eta()),0.891)
            histo_jet_recoMatch.Fill(recojets[closest].pt(),abs(recojets[closest].eta()))
            if histo_jet_genMatch.FindBin(genjetp4.pt(),abs(genjetp4.eta())) == histo_jet_genMatch.FindBin(recojets[closest].pt(),abs(recojets[closest].eta())):
                histo_jet_stab.Fill(genjetp4.pt(),abs(genjetp4.eta()))
                histo_jet_pur.Fill(recojets[closest].pt(),abs(recojets[closest].eta()))
            histo_jet_drSel.Fill(deltaR(genq1p4.eta(),genq1p4.phi(),genq2p4.eta(),genq2p4.phi()))
            histo_jet_ctSel.Fill(costhetajet)

    
           
           

            
        

def makePlots():
    canvas = root.TCanvas("c","c",400,400)
    # basic gen distributions
    histo_ele_gen.Draw("COLZ")
    canvas.SaveAs("histo_ele_gen.eps")
    histo_mu_gen.Draw("COLZ")
    canvas.SaveAs("histo_mu_gen.eps")
    histo_jet_gen.Draw("COLZ")
    canvas.SaveAs("histo_jet_gen.eps")

    #purity
    tmphist = histo_jet_pur.Clone("tmphist")
    tmphist.Divide(histo_jet_reco)
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_jet_purity.eps")
    tmphist = histo_ele_pur.Clone("tmphist")
    tmphist.Divide(histo_ele_reco)
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_ele_purity.eps")
    tmphist = histo_mu_pur.Clone("tmphist")
    tmphist.Divide(histo_mu_reco)
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_mu_purity.eps")
        
    #stability
    tmphist = histo_jet_stab.Clone("tmphist")
    tmphist.Divide(histo_jet_gen)
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_jet_stability.eps")
    tmphist = histo_ele_stab.Clone("tmphist")
    tmphist.Divide(histo_ele_gen)
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_ele_stability.eps")
    tmphist = histo_mu_stab.Clone("tmphist")
    tmphist.Divide(histo_mu_gen)
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_mu_stability.eps")
        
    #eff
    output = root.TFile.Open("efficiency.root","RECREATE")
    tmphist = histo_jet_genMatch.Clone("eff_jet")
    tmphist.Divide(histo_jet_gen)
    histo_jet_genMatch.Write()
    histo_jet_gen.Write()
    tmphist.Write()
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_jet_efficiency.eps")
    tmphist = histo_ele_genMatch.Clone("eff_ele")
    tmphist.Divide(histo_ele_gen)
    histo_ele_genMatch.Write()
    histo_ele_gen.Write()
    tmphist.Write()
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_ele_efficiency.eps")
    tmphist = histo_mu_genMatch.Clone("eff_mu")
    tmphist.Divide(histo_mu_gen)
    histo_mu_genMatch.Write()
    histo_mu_gen.Write()
    tmphist.Write()
    tmphist.Draw("COLZ")
    canvas.SaveAs("histo_mu_efficiency.eps")

    histo_mu_dr.Write()
    histo_ele_dr.Write()
    histo_jet_dr.Write()
    histo_mu_ct.Write()
    histo_ele_ct.Write()
    histo_jet_ct.Write()
    histo_mu_drSel.Write()
    histo_ele_drSel.Write()
    histo_jet_drSel.Write()
    histo_mu_ctSel.Write()
    histo_ele_ctSel.Write()
    histo_jet_ctSel.Write()

    hist_mjet_gen.Write()
    hist_mjet_rec.Write()


    output.Close()
        
    

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
