
#!/usr/bin/env python

import ROOT as root
from DataFormats.FWLite import Events, Handle
import optparse
import os,sys
import math
from array import array

root.gROOT.SetBatch()        # don't pop up canvases
root.gROOT.SetStyle('Plain') # white background

histofile = root.TFile.Open("efficiency_WW.root")
histo_eff_ele   = histofile.Get("eff_ele")
histo_eff_mu   = histofile.Get("eff_mu")
histo_eff_jet   = histofile.Get("eff_jet")
histo_eff_event   = histofile.Get("histo_event_eff")

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
    nevent_all = 0 #all decays in the file

    nevent = 0 #only decays if interest (i.e. semi-leptonic)
    nevent_ele = 0 
    nevent_mu = 0 

    ngensel = 0
    ngensel_ele = 0
    ngensel_mu = 0

    nsel   = 0
    nsel_ele = 0
    nsel_mu = 0

    #loop over events
    for event in events:
        nevent_all += 1
        if nevent_all % 1000 ==0:
            print "event: " + str(nevent_all)

        #print str(event.eventAuxiliary().run())
        #print str(event.object().triggerResultsByName("CMG").accept("preselEleMergedPath"))
        # determine generated flavor and get generated kinematics
        havechargedleptons=0
        haveneutralleptons=0
        havejet=0
        haveVlep=0
        flavor=""
        genVlep      = root.TLorentzVector()
        genlepton1p4 = root.TLorentzVector()
        genlepton1C  = 0
        genlepton2p4 = root.TLorentzVector()
        genlepton2C  = 0
        genjetp4     = root.TLorentzVector()
        event.getByLabel ("genParticles", genhandle)
        genparticles = genhandle.product()
        counter      = 0
        for genp in genparticles:
            counter = counter + 1
            if (abs(genp.pdgId())==11 or abs(genp.pdgId())==13) and abs(genp.mother().pdgId())==24:
                #print "found lepton "+str(genp.pdgId())
                if(abs(genp.pdgId())==11):
                    flavor = "ele"
                if(abs(genp.pdgId())==13):
                    flavor = "mu"                    
                if havechargedleptons==0:
                    genlepton1p4 = genp.p4()
                    genlepton1C = genp.charge()
                    havechargedleptons=1                
            if (abs(genp.pdgId())==12 or abs(genp.pdgId())==14) and abs(genp.mother().pdgId())==24:
                if haveneutralleptons==0:
                    genlepton2p4 = genp.p4()
                    genlepton2C = genp.charge()
                    haveneutralleptons=1
            if abs(genp.pdgId())==24 and genp.numberOfDaughters()>0 and abs(genp.daughter(1).pdgId())<7:
                genjetp4=genp.p4()
                havejet=1
            if abs(genp.pdgId())==24 and genp.numberOfDaughters()>0 and abs(genp.daughter(1).pdgId())>7:
                genVlep=genp.p4()
                haveVlep=1

            if counter > 25:
                break    

            if havechargedleptons==1 and haveneutralleptons==1 and havejet==1 and haveVlep==1:
                break

        #select only WW semi-leptonic events    
        if havechargedleptons!=1 or haveneutralleptons!=1 or havejet!=1 or haveVlep!=1:
            continue

        #Number of generated semi-leptonic events
        nevent += 1        
        if flavor == "ele":
            nevent_ele += 1
        if flavor == "mu":
            nevent_mu += 1
        if flavor != "ele" and flavor !="mu":
            print "gen level messed up"            

        # gen-level acceptance cuts:
        if genjetp4.pt()<200: # hadronic V acceptance cut
            continue
        if genjetp4.mass()<65 or genjetp4.mass()>105: # hadronic V acceptance cut
            continue        
        if genVlep.pt()<200: # leptonic V acceptance cut
            continue 
        if flavor == "ele": # electron acceptance
            if abs(genlepton1p4.eta())>2.5:
                continue
            if genlepton1p4.pt()<90: 
                continue 
            if genlepton2p4.pt()<80: 
                continue
        if flavor == "mu": # muon acceptance
            if abs(genlepton1p4.eta())>2.1: 
                continue
            if genlepton1p4.pt()<50: 
                continue
            if genlepton2p4.pt()<40: 
                continue
        if abs(genjetp4.eta())>2.4: # jet acceptance
            continue
        #back-to-back topology
        if deltaR(genjetp4.eta(),genjetp4.phi(),genlepton1p4.eta(),genlepton1p4.phi()) < 3.141592/2: 
            continue
        if abs( deltaPhi(genjetp4.phi(),genlepton2p4.phi()) ) < 2:
            continue
        if abs( deltaPhi(genjetp4.phi(),genVlep.phi()) ) < 2:
            continue

        #Number of selected events (after acceptance cuts)
        ngensel +=1
        if flavor=="ele":
            ngensel_ele +=1
        if flavor=="mu":
            ngensel_mu +=1

        #Efficiency leptonic V
        eff_Vlep=1.
        if flavor=="ele":
            bin1 = histo_eff_ele.FindBin( genVlep.pt(),abs(genVlep.Eta()) )
            eff_Vlep = histo_eff_ele.GetBinContent(bin1)
        if flavor=="mu":
            bin1 = histo_eff_mu.FindBin( genVlep.pt(),abs(genVlep.Eta()) )
            eff_Vlep = histo_eff_mu.GetBinContent(bin1)
                                        
        #Efficiency hadronic V
        bin = histo_eff_jet.FindBin( genjetp4.pt(),abs(genjetp4.eta()) )
        eff_jet = histo_eff_jet.GetBinContent(bin)

        #Efficiency per event (b-tag, lepton-veto)
        eff_event = histo_eff_event.GetBinContent(1)

        #Number of selected events (after efficiency re-weighting)
        nsel += eff_jet * eff_Vlep * eff_event
        if flavor=="ele":
            nsel_ele += eff_jet * eff_Vlep * eff_event
        if flavor=="mu":
            nsel_mu += eff_jet * eff_Vlep * eff_event

    print "-----------------------------------------------------------------------------------------------------"
    print file  
    print "Total numer of events in file (all decays): " + str(nevent_all) 
    print "Total numer of events of interest (semi-leptonic decays): " \
          + str(nevent) + " of which ele = " + str(nevent_ele) + " and mu = " + str(nevent_mu) 
    print "Total numer of events of passing acceptance cuts: " \
          + str(ngensel) + " of which ele = " + str(ngensel_ele) + " and mu = " + str(ngensel_mu) 
    print "Total numer of events of after efficiency re-weighting: " \
          + str(nsel) + " of which ele = " + str(nsel_ele) + " and mu = " + str(nsel_mu) 
    
    return nevent,nevent_ele,nevent_mu,ngensel,ngensel_ele,ngensel_mu,nsel,nsel_ele,nsel_mu
            

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
    nevent_final=0 
    nevent_ele_final=0
    nevent_mu_final=0
    ngensel_final=0
    ngensel_ele_final=0
    ngensel_mu_final=0
    nsel_final=0
    nsel_ele_final=0
    nsel_mu_final=0    
                
    for file in filelist:
        #print 'File is ',file
        nevent_current,nevent_ele_current,nevent_mu_current, ngensel_current,ngensel_ele_current,ngensel_mu_current, nsel_current,nsel_ele_current,nsel_mu_current = processSubsample(file)

        nevent_final += nevent_current 
        nevent_ele_final += nevent_ele_current
        nevent_mu_final += nevent_mu_current

        ngensel_final += ngensel_current
        ngensel_ele_final += ngensel_ele_current
        ngensel_mu_final += ngensel_mu_current

        nsel_final += nsel_current
        nsel_ele_final += nsel_ele_current
        nsel_mu_final += nsel_mu_current   

    print "-----------------------------------------------------------------------------------------------------"
    print "ALL FILES TOGETHER"
    print "Total numer of events of interest (semi-leptonic decays): " \
          + str(nevent_final) + " of which ele = " + str(nevent_ele_final) + " and mu = " + str(nevent_mu_final) 
    print "Total numer of events of passing acceptance cuts: " \
          + str(ngensel_final) + " of which ele = " + str(ngensel_ele_final) + " and mu = " + str(ngensel_mu_final) 
    print "Total numer of events of after efficiency re-weighting: " \
          + str(nsel_final) + " of which ele = " + str(nsel_ele_final) + " and mu = " + str(nsel_mu_final) 
    print "-----------------------------------------------------------------------------------------------------"     
    print "===> The estimated efficiency is " + str(nsel_final/nevent_final) + " ( "+ str(nsel_ele_final/nevent_final)+" for ele + " + str(nsel_mu_final/nevent_final) +" for mu )"
    print "-----------------------------------------------------------------------------------------------------"     

if __name__ == "__main__":
    main()
