#!/usr/bin/env python

import ROOT as root
from DataFormats.FWLite import Events, Handle
import optparse
import os,sys
import math
from array import array

root.gROOT.SetBatch()        # don't pop up canvases
root.gROOT.SetStyle('Plain') # white background

elebins_pt  = [20,40,60,90,120,150,200,250,300,400,500,600,700,800,900,1000,1200,1500]
elebins_eta = [0.,0.3,0.9,1.2,1.4442,1.566,1.9,2.2,2.5]

jetbins_pt =  [30,50,80,120,150,200,250,300,400,500,600,700,800,900,1000,1200,1500]
jetbins_eta=  [0.,0.3,0.9,1.2,1.5,1.8,2.1,2.4]

mubins_pt  =  elebins_pt
mubins_eta =  jetbins_eta

histo_ele_gen     = root.TH2F("ele_gen","ele_gen",17,array('d',elebins_pt),8,array('d',elebins_eta))    
histo_ele_genMatch = histo_ele_gen.Clone("ele_genreco")  # gen gen quantitites eles matched between reco and gen
histo_ele_reco     = histo_ele_gen.Clone("ele_reco")     # reco quantities for all passing objects
histo_ele_recoMatch= histo_ele_gen.Clone("ele_recoMatch")# reco quantities for eles matched to gen-level
histo_ele_pur      = histo_ele_gen.Clone("ele_pur")      # reco quantities for eles matched to gen-level, goint to the same histogram bin
histo_ele_stab     = histo_ele_gen.Clone("ele_stab")     # gen  quantities for eles matched to gen-level, goint to the same histogram bin

histo_mu_gen     = root.TH2F("mu_gen","mu_gen",17,array('d',mubins_pt),7,array('d',mubins_eta))    
histo_mu_genMatch = histo_mu_gen.Clone("mu_genreco")  # gen gen quantitites mus matched between reco and gen
histo_mu_reco     = histo_mu_gen.Clone("mu_reco")     # reco quantities for all passing objects
histo_mu_recoMatch= histo_mu_gen.Clone("mu_recoMatch")# reco quantities for mus matched to gen-level
histo_mu_pur      = histo_mu_gen.Clone("mu_pur")      # reco quantities for mus matched to gen-level, goint to the same histogram bin
histo_mu_stab     = histo_mu_gen.Clone("mu_stab")     # gen  quantities for mus matched to gen-level, goint to the same histogram bin

histo_jet_gen      = root.TH2F("jet_gen","jet_gen",16,array('d',jetbins_pt),7,array('d',jetbins_eta)) # gen quantities of all resonances in accptance   
histo_jet_genMatch = histo_jet_gen.Clone("jet_genreco")  # gen gen quantitites jets matched between reco and gen
histo_jet_reco     = histo_jet_gen.Clone("jet_reco")     # reco quantities for all passing objects
histo_jet_recoMatch= histo_jet_gen.Clone("jet_recoMatch")# reco quantities for jets matched to gen-level
histo_jet_pur      = histo_jet_gen.Clone("jet_pur")      # reco quantities for jets matched to gen-level, goint to the same histogram bin
histo_jet_stab     = histo_jet_gen.Clone("jet_stab")     # gen  quantities for jets matched to gen-level, goint to the same histogram bin


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

def processSubsample(file,filtered):
    events = Events (file)

    # create handle outside of loop
    genhandle          = Handle ('std::vector<reco::GenParticle>')
    elehandle          = Handle ('std::vector<cmg::Electron>')
    muhandle           = Handle ('std::vector<cmg::Muon>')
    jethandle          = Handle ('std::vector<cmg::VJet>')
    # like and edm::InputTag
    nevent = 0 
    for event in events:
        nevent += 1
        if nevent % 10000 ==0:
            print "event: " + str(nevent)
        
        # determine generated flavor and get generated kinematics
        haveleptons=0
        havejet=0
        flavor=""
        genlepton1p4 = root.TLorentzVector()
        genlepton1C  = 0
        genlepton2p4 = root.TLorentzVector()
        genlepton2C  = 0
        genjetp4     = root.TLorentzVector()
        event.getByLabel ("genParticles", genhandle)
        genparticles = genhandle.product()
        for genp in genparticles:
            if (abs(genp.pdgId())==11 or abs(genp.pdgId())==13) and genp.mother().pdgId()==23:
                #print "found lepton "+str(genp.pdgId())
                if(abs(genp.pdgId())==11):
                    flavor = "ele"
                if(abs(genp.pdgId())==13):
                    flavor = "mu"                    
                if haveleptons==0:
                    genlepton1p4 = genp.p4()
                    genlepton1C = genp.charge()
                    haveleptons=1                
                elif haveleptons==1:
                    genlepton2p4 = genp.p4()
                    genlepton2C = genp.charge()
                    haveleptons=2
            if abs(genp.pdgId())==23 and genp.numberOfDaughters()>0 and abs(genp.daughter(1).pdgId())<7:
                genjetp4=genp.p4()
                havejet=1

            if haveleptons==2 and havejet==1:
                break

        #print str(nevent)+ "  " + flavor
        # gen-level acceptance cuts:
        if genjetp4.pt()<80: # hadronic Z acceptance cut
            continue
        if genjetp4.mass()<70 or genjetp4.mass()>110: # hadronic Z acceptance cut
            continue        
        if (genlepton1p4 + genlepton2p4).pt() < 80: # leptonic Z acceptance cut
            continue 
        if (genlepton1p4 + genlepton2p4).mass() < 70 or(genlepton1p4 + genlepton2p4).mass() >110 :
            continue 
        if flavor == "ele": # electron acceptance
            if abs(genlepton1p4.eta())>2.5:
                continue
            if abs(genlepton2p4.eta())>2.5:
                continue
            if genlepton1p4.pt()<40 or genlepton2p4.pt()<40:
                continue
        if flavor == "mu": # muon acceptance
            if abs(genlepton1p4.eta())>2.4:
                continue
            if abs(genlepton2p4.eta())>2.4:
                continue
            if genlepton1p4.pt()<20 or genlepton2p4.pt()<20:
                continue
            if genlepton1p4.pt()<40 and genlepton2p4.pt()<40:
                contine
        if genjetp4.pt() <30: # jet acceptance
            continue
        if abs(genjetp4.eta())>2.4:
            continue

        #fill pure gen info
        if not filtered:
            if flavor == "ele":
                histo_ele_gen.Fill(genlepton1p4.pt(),abs(genlepton1p4.eta()))
                histo_ele_gen.Fill(genlepton2p4.pt(),abs(genlepton2p4.eta()))
            if flavor == "mu":
                histo_mu_gen.Fill(genlepton1p4.pt(),abs(genlepton1p4.eta()))
                histo_mu_gen.Fill(genlepton2p4.pt(),abs(genlepton2p4.eta()))
            histo_jet_gen.Fill(genjetp4.pt(),abs(genjetp4.eta()))
            continue 

        if(event.eventAuxiliary().event() ==  2434343111):# strange fualty event
            continue
        if(event.eventAuxiliary().event() ==  43111):# strange fualty event
            continue
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
            if jet.prunedMass() < 70 or jet.prunedMass() > 110:
                continue
            if jet.pt() < 80:
                continue
            if abs(jet.eta())>2.4:
                continue
            histo_jet_reco.Fill(jet.pt(),abs(jet.eta()))
            dr = deltaR(jet.eta(),jet.phi(),genjetp4.eta(),genjetp4.phi())
            if dr < closestDr:
                closestDr = dr
                closest = index

        if index != -1 and closestDr < 0.8: # found a matching VJet
            histo_jet_genMatch.Fill(genjetp4.pt(),abs(genjetp4.eta()))
            histo_jet_recoMatch.Fill(recojets[index].pt(),abs(recojets[index].eta()))
            if histo_jet_genMatch.FindBin(genjetp4.pt(),abs(genjetp4.eta())) == histo_jet_genMatch.FindBin(recojets[index].pt(),abs(recojets[index].eta())):
                histo_jet_stab.Fill(genjetp4.pt(),abs(genjetp4.eta()))
                histo_jet_pur.Fill(recojets[index].pt(),abs(recojets[index].eta()))

        #print "startele"
        # fill matched electrons objects
        if flavor == "ele":
            event.getByLabel ("electronPresel", elehandle)
            recoeles = elehandle.product()
            closest1 = -1
            closestDr1 = 99999.
            index1 = -1
            closest2 = -1
            closestDr2 = 99999.
            index2 = -1
            for ele in recoeles:
                index1 += 1
                index2 += 1
            #acceptance cuts
                if ele.pt() < 40:
                    continue
                if abs(ele.eta())>2.5:
                    continue
                if  abs(ele.sourcePtr().get().superCluster().get().eta()) < 1.566 and abs(ele.sourcePtr().get().superCluster().get().eta()) > 1.4442 :
                    continue            
                histo_ele_reco.Fill(ele.pt(),abs(ele.eta()))
                dr1 = deltaR(ele.eta(),ele.phi(),genlepton1p4.eta(),genlepton1p4.phi())
                dr2 = deltaR(ele.eta(),ele.phi(),genlepton2p4.eta(),genlepton2p4.phi())
                if dr1 < closestDr1 and ele.charge() == genlepton1C:
                    closestDr1 = dr1
                    closest1 = index1
                if dr2 < closestDr2 and ele.charge() == genlepton2C:
                    closestDr2 = dr2
                    closest2 = index2

            if index1 != -1 and closestDr1 < 0.1: # found a matching electron for genlepon1
                histo_ele_genMatch.Fill(genlepton1p4.pt(),abs(genlepton1p4.eta()))
                histo_ele_recoMatch.Fill(recoeles[index1].pt(),abs(recoeles[index1].eta()))
                if histo_ele_genMatch.FindBin(genlepton1p4.pt(),abs(genlepton1p4.eta())) == histo_ele_genMatch.FindBin(recoeles[index1].pt(),abs(recoeles[index1].eta())):
                    histo_ele_stab.Fill(genlepton1p4.pt(),abs(genlepton1p4.eta()))
                    histo_ele_pur.Fill(recoeles[index1].pt(),abs(recoeles[index1].eta()))

            if index2 != -1 and closestDr2 < 0.1: # found a matching electron for genlepon2
                histo_ele_genMatch.Fill(genlepton2p4.pt(),abs(genlepton2p4.eta()))
                histo_ele_recoMatch.Fill(recoeles[index2].pt(),abs(recoeles[index2].eta()))
                if histo_ele_genMatch.FindBin(genlepton2p4.pt(),abs(genlepton2p4.eta())) == histo_ele_genMatch.FindBin(recoeles[index2].pt(),abs(recoeles[index2].eta())):
                    histo_ele_stab.Fill(genlepton2p4.pt(),abs(genlepton2p4.eta()))
                    histo_ele_pur.Fill(recoeles[index2].pt(),abs(recoeles[index2].eta()))

    
           
       # fill matched muon objects
        if flavor == "mu":
            event.getByLabel ("muonPreselNoIso", muhandle)
            recomus = muhandle.product()
            closest1 = -1
            closestDr1 = 99999.
            index1 = -1
            closest2 = -1
            closestDr2 = 99999.
            index2 = -1
            for mu in recomus:
                index1 += 1
                index2 += 1
            #acceptance cuts
                if mu.pt() < 20:
                    continue
                if abs(mu.eta())>2.4:
                    continue
                histo_mu_reco.Fill(mu.pt(),abs(mu.eta()))
                dr1 = deltaR(mu.eta(),mu.phi(),genlepton1p4.eta(),genlepton1p4.phi())
                dr2 = deltaR(mu.eta(),mu.phi(),genlepton2p4.eta(),genlepton2p4.phi())
                if dr1 < closestDr1 and mu.charge() == genlepton1C:
                    closestDr1 = dr1
                    closest1 = index1
                if dr2 < closestDr2 and mu.charge() == genlepton2C:
                    closestDr2 = dr2
                    closest2 = index2


            if index1 != -1 and closestDr1 < 0.1: # found a matching muctron for genlepon1
                histo_mu_genMatch.Fill(genlepton1p4.pt(),abs(genlepton1p4.eta()))
                histo_mu_recoMatch.Fill(recomus[index1].pt(),abs(recomus[index1].eta()))
                if histo_mu_genMatch.FindBin(genlepton1p4.pt(),abs(genlepton1p4.eta())) == histo_mu_genMatch.FindBin(recomus[index1].pt(),abs(recomus[index1].eta())):
                    histo_mu_stab.Fill(genlepton1p4.pt(),abs(genlepton1p4.eta()))
                    histo_mu_pur.Fill(recomus[index1].pt(),abs(recomus[index1].eta()))

            if index2 != -1 and closestDr2 < 0.1: # found a matching muctron for genlepon2
                histo_mu_genMatch.Fill(genlepton2p4.pt(),abs(genlepton2p4.eta()))
                histo_mu_recoMatch.Fill(recomus[index2].pt(),abs(recomus[index2].eta()))
                if histo_mu_genMatch.FindBin(genlepton2p4.pt(),abs(genlepton2p4.eta())) == histo_mu_genMatch.FindBin(recomus[index2].pt(),abs(recomus[index2].eta())):
                    histo_mu_stab.Fill(genlepton2p4.pt(),abs(genlepton2p4.eta()))
                    histo_mu_pur.Fill(recomus[index2].pt(),abs(recomus[index2].eta()))
        #print "enloop"
           

            
        

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
    output.Close()
        
    

def main():
    parser = optparse.OptionParser(description='Signal Shape Fitting Tool')
    parser.add_option('-f','--filepath',     help='path to input trees, default: ../trees'                             , default = "../trees" )                                      
    parser.add_option('-g','--genpath',     help='path to input trees, default: ../trees'                             , default = "../trees" )                                      
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


    genlist = []
    listf = open(args.genpath)
    for line in listf:
        genlist.append(line.strip())

##     print genlist
##     sys.exit(0)
                
    for file in genlist:
        print 'File is ',file
        processSubsample(file,False)

    for file in filelist:
        print 'File is ',file
        processSubsample(file,True)
            
            

    makePlots()


if __name__ == "__main__":
    main()
