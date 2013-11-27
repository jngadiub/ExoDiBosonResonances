#!/usr/bin/env python

import ROOT as root
root.gROOT.SetBatch()        # don't pop up canvases

def cleanHisto(histo):
    for binX in range(1,histo.GetXaxis().GetNbins()+1):
        for binY in range(1,histo.GetYaxis().GetNbins()+1):
            #print str(binX)+" "+str(binY)
            binContent = histo.GetBinContent(binX,binY)
            if binContent<25:
                histo.SetBinContent(binX,binY,0.)
                #print binContent
    return histo

histofile = root.TFile.Open("efficiency.root")

histo_ele_gen   = histofile.Get("ele_gen")
histo_ele_genreco   = histofile.Get("ele_genreco")
histo_mu_gen   = histofile.Get("mu_gen")
histo_mu_genreco   = histofile.Get("mu_genreco")
histo_jet_gen   = histofile.Get("jet_gen")
histo_jet_genreco   = histofile.Get("jet_genreco")

cleanHisto(histo_ele_gen)
cleanHisto(histo_mu_gen)
cleanHisto(histo_jet_gen)

histo_eff_ele   = histo_ele_genreco.Clone("eff_ele")
histo_eff_mu   = histo_mu_genreco.Clone("eff_mu")
histo_eff_jet   = histo_jet_genreco.Clone("eff_jet")

histo_eff_ele.Divide(histo_ele_gen)   
histo_eff_mu.Divide(histo_mu_gen)      
histo_eff_jet.Divide(histo_jet_gen)      

output = root.TFile.Open("efficiency_ZZ_forClosure.root","RECREATE")
histo_eff_ele.Write()
histo_eff_mu.Write()
histo_eff_jet.Write()
