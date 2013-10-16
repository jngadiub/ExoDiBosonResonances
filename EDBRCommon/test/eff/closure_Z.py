
#!/usr/bin/env python

import ROOT as root
from DataFormats.FWLite import Events, Handle
import optparse
import os,sys
import math
from array import array

root.gROOT.SetBatch()        # don't pop up canvases
root.gROOT.SetStyle('Plain') # white background

histofile = root.TFile.Open("efficiency.root")
histo_eff_ele   = histofile.Get("eff_ele")
histo_eff_mu   = histofile.Get("eff_mu")
histo_eff_jet   = histofile.Get("eff_jet")


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
    # like and edm::InputTag
    nevent = 0 
    ngensel = 0
    ngensel_ele = 0
    ngensel_mu = 0
    nevent_ele = 0 
    nevent_mu = 0 
    nsel   = 0
    nsel_ele = 0
    nsel_mu = 0
    for event in events:
        nevent += 1
        if nevent % 10000 ==0:
            print "event: " + str(nevent)
        
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
        event.getByLabel ("genParticles", genhandle)
        genparticles = genhandle.product()
        for genp in genparticles:
            if (abs(genp.pdgId())==11 or abs(genp.pdgId())==13) and genp.mother().pdgId()==23:
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
            if abs(genp.pdgId())==23 and genp.numberOfDaughters()>0 and abs(genp.daughter(1).pdgId())>7:
                genZlep=genp.p4()
                havez=1

            if haveleptons==2 and havejet==1 and havez==1:
                break

        if flavor == "ele":
            nevent_ele += 1
        if flavor == "mu":
            nevent_mu += 1

        if flavor != "ele" and flavor !="mu":
            print "gen level messed up"

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

        ngensel +=1


        eff_zlep=1.
        if flavor=="ele":
            bin1 = histo_eff_ele.FindBin(genZlep.pt(),abs(genZlep.Eta()))
            eff_zlep = histo_eff_ele.GetBinContent(bin1)
            ngensel_ele +=1
        if flavor=="mu":
            bin1 = histo_eff_mu.FindBin(genZlep.pt(),abs(genZlep.Eta()))
            eff_zlep = histo_eff_mu.GetBinContent(bin1)
            ngensel_mu +=1
                                        

        bin = histo_eff_jet.FindBin(genjetp4.pt(),abs(genjetp4.eta()) )
        eff_jet = histo_eff_jet.GetBinContent(bin)
        
        nsel += eff_jet*eff_zlep
        if flavor=="ele":
            nsel_ele += eff_jet*eff_zlep
        if flavor=="mu":
            nsel_mu += eff_jet*eff_zlep
 
        

    print "passing " + str(nsel) + " ( " + str(nsel_ele) + " / " + str(nsel_mu) + " ) events of " + str(nevent) 
    print str(ngensel) + "   " + str(ngensel_ele)+ "   " + str(ngensel_mu)
    return nevent,nsel,nevent_ele,nsel_ele,nevent_mu,nsel_mu
        
    

def main():
    parser =  optparse.OptionParser(description='Signal Shape Fitting Tool')
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
    ntot=0
    stot=0
    netot=0
    setot=0
    nmtot=0
    smtot=0
    
                
    for file in filelist:
        print 'File is ',file
        n,s,ne,se,nm,sm = processSubsample(file)
        ntot+=n
        stot+=s
        netot+=ne
        setot+=se
        nmtot+=nm
        smtot+=sm

    print "estimated efficiency is " + str(stot/ntot) + " ( "+ str(setot/ntot)+" / " + str(smtot/ntot) +" )"
        



if __name__ == "__main__":
    main()
