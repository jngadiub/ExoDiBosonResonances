#!/usr/bin/env python
from array import *
import ROOT as root

root.gROOT.SetBatch()        # don't pop up canvases
root.gROOT.SetStyle('Plain') # white background
root.gStyle.SetOptStat(0) # white background
root.gStyle.SetOptTitle(0)
root.gStyle.SetPadRightMargin(0.12)
root.gStyle.SetPaintTextFormat(".2f")
root.gStyle.SetNumberContours(500);


def plotPretty(histo,filename,type):
    c=root.TCanvas("c","c",600,600)
    c.SetFillStyle(4000)
    c.SetTicks(1,1)
    c.SetRightMargin(0.2)
    #fPads1=root.TPad("fPads1", "", 0.07, 0.03, 1, 1)
    #fPads1.Draw()
    #fPads1.cd()
    histo.SetMaximum(1)
    histo.GetXaxis().SetTitle("p_{t,Z} [GeV]")
    histo.GetYaxis().SetTitle("|#eta_{Z}|")    
    histo.GetZaxis().SetTitle("Reconstruction #times ID Efficiency")
    histo.GetXaxis().SetTitleOffset(1.1)
    histo.GetYaxis().SetTitleOffset(1.2)    
    histo.GetZaxis().SetTitleOffset(1.6)
    histo.GetXaxis().SetNdivisions(505)
    histo.GetYaxis().SetNdivisions(506)
    if type==0 or type==4:
        histo.GetYaxis().SetRangeUser(0,2.499)
    else:
        histo.GetYaxis().SetRangeUser(0,2.399)
    histo.GetZaxis().SetRangeUser(0,1)
    histo.SetMarkerSize(1.25)
    histo.Draw()
    histo.Draw("COLZ")#,TEXT90")

    tex = root.TLatex(0.127517,0.911329,"CMS Simulation")
    tex.SetNDC()
    tex.SetTextSize(0.044)
    tex.SetTextFont(42)
    tex.SetLineWidth(2)
    tex.Draw()

    tex2 = root.TLatex(0.721477,0.911329,"#sqrt{s}=8TeV")
    tex2.SetNDC()
    tex2.SetTextSize(0.044)
    tex2.SetTextFont(42)
    tex2.SetLineWidth(2)
    tex2.Draw()

    if type==0:
        tex3 = root.TLatex(0.509497,0.83042,"Z #rightarrow ee")
        tex3.SetNDC()
        tex3.SetTextSize(0.044)
        tex3.SetTextFont(42)
        tex3.SetLineWidth(2)
        tex3.Draw()            

    if type==1:
        tex3 = root.TLatex(0.509497,0.83042,"Z #rightarrow #mu#mu")
        tex3.SetNDC()
        tex3.SetTextSize(0.044)
        tex3.SetTextFont(42)
        tex3.SetLineWidth(2)
        tex3.Draw()            
    if type==4:
        tex3 = root.TLatex(0.509497,0.83042,"Z_{L} #rightarrow q#bar{q} #rightarrow 1 jet")
        tex3.SetNDC()
        tex3.SetTextSize(0.044)
        tex3.SetTextFont(42)
        tex3.SetLineWidth(2)
        tex3.Draw()            
   
    c.RedrawAxis()
    #root.gPad.Modified()
    #root.gPad.Update()
    c.Update()

    c.SaveAs(filename)



histofile = root.TFile.Open("efficiency_ZZ_forClosure.root")
histo_eff_ele   = histofile.Get("eff_ele")
histo_eff_mu   = histofile.Get("eff_mu")
histo_eff_jet   = histofile.Get("eff_jet")

plotPretty(histo_eff_ele,"histo_eff_ele.png",0)
plotPretty(histo_eff_mu,"histo_eff_mu.png",1)
plotPretty(histo_eff_jet,"histo_eff_jet.png",4)
plotPretty(histo_eff_ele,"histo_eff_ele.eps",0)
plotPretty(histo_eff_mu,"histo_eff_mu.eps",1)
plotPretty(histo_eff_jet,"histo_eff_jet.eps",4)
