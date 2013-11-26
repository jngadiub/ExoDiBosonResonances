#!/usr/bin/env python

import ROOT as root

root.gROOT.SetBatch()        # don't pop up canvases
root.gROOT.SetStyle('Plain') # white background
root.gStyle.SetOptStat(0) # white background
root.gStyle.SetOptTitle(0)
root.gStyle.SetPadRightMargin(0.12)
root.gStyle.SetPaintTextFormat(".2f")

def plotPretty(histo,filename):
    c=root.TCanvas("c","c",600,600)
    histo.SetMaximum(1)
    histo.GetXaxis().SetTitle("p_{t} [GeV]")
    histo.GetYaxis().SetTitle("|#eta|")    
    histo.GetYaxis().SetTitleOffset(1.2)    
    histo.Draw("COLZ,TEXT90")
    c.SaveAs(filename)



histofile = root.TFile.Open("efficiency_ZZ_forClosure.root")
histo_eff_ele   = histofile.Get("eff_ele")
histo_eff_mu   = histofile.Get("eff_mu")
histo_eff_jet   = histofile.Get("eff_jet")

plotPretty(histo_eff_ele,"histo_eff_ele.eps")
plotPretty(histo_eff_mu,"histo_eff_mu.eps")
plotPretty(histo_eff_jet,"histo_eff_jet.eps")
