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
    histo.GetXaxis().SetTitle("p_{t} [GeV]")
    histo.GetYaxis().SetTitle("|#eta|")    
    histo.GetYaxis().SetTitleOffset(1.2)
    histo.GetZaxis().SetRangeUser(0,1)    
    histo.Draw("COLZ,TEXT90")
    c.SaveAs(filename)

def cleanHisto(histo):
    for binX in range(1,histo.GetXaxis().GetNbins()+1):
        for binY in range(1,histo.GetYaxis().GetNbins()+1):
            #print str(binX)+" "+str(binY)
            binContent = histo.GetBinContent(binX,binY)
            if binContent<25:
                histo.SetBinContent(binX,binY,0.)
                #print binContent
    return histo
            

#histofile = root.TFile.Open("/afs/cern.ch/work/s/santanas/Releases/CMSSW_5_3_9_CMGrel_V5_15_0_ExoDiBosonResonances_GIT_production/CMSSW_5_3_9/src/ExoDiBosonResonances/EDBRCommon/test/eff/plotsEff_BulkG_c0p2_plus_wideRes_final_05_11_2013/efficiency_WW.root") #Bulk
#histofile = root.TFile.Open("/afs/cern.ch/work/s/santanas/Releases/CMSSW_5_3_9_CMGrel_V5_15_0_ExoDiBosonResonances_GIT_production/CMSSW_5_3_9/src/ExoDiBosonResonances/EDBRCommon/test/eff/plotsEff_RSG_c0p2_final_05_11_2013/efficiency_WW.root") #RS pythia (wrong angular distributions)
histofile = root.TFile.Open("/afs/cern.ch/work/s/santanas/Releases/CMSSW_5_3_9_CMGrel_V5_15_0_ExoDiBosonResonances_GIT_production/CMSSW_5_3_9/src/ExoDiBosonResonances/EDBRCommon/test/eff/plotsEff_RSG_Madgraph_final_05_11_2013/efficiency_WW.root") #RS madgraph (correct angular distributions)


advancedPlots = 1

if advancedPlots == 0:
    histo_eff_ele   = histofile.Get("eff_ele")
    histo_eff_mu   = histofile.Get("eff_mu")
    histo_eff_tautoele   = histofile.Get("eff_tautoele")
    histo_eff_tautomu   = histofile.Get("eff_tautomu")
    histo_eff_jet   = histofile.Get("eff_jet")

    plotPretty(histo_eff_ele,"histo_eff_ele_WW.eps")
    plotPretty(histo_eff_mu,"histo_eff_mu_WW.eps")
    plotPretty(histo_eff_tautoele,"histo_eff_tautoele_WW.eps")
    plotPretty(histo_eff_tautomu,"histo_eff_tautomu_WW.eps")
    plotPretty(histo_eff_jet,"histo_eff_jet_WW.eps")

elif advancedPlots == 1:

    histo_eff_event   = histofile.Get("histo_event_eff")

    #take histograms from file
    histo_ele_gen   = histofile.Get("ele_gen")
    histo_ele_genreco   = histofile.Get("ele_genreco")
    histo_mu_gen   = histofile.Get("mu_gen")
    histo_mu_genreco   = histofile.Get("mu_genreco")
    histo_tautoele_gen   = histofile.Get("tautoele_gen")
    histo_tautoele_genreco   = histofile.Get("tautoele_genreco")
    histo_tautomu_gen   = histofile.Get("tautomu_gen")
    histo_tautomu_genreco   = histofile.Get("tautomu_genreco")
    histo_jet_gen   = histofile.Get("jet_gen")
    histo_jet_genreco   = histofile.Get("jet_genreco")

    #ele + tautoele
    histo_ele_gen.Add(histo_tautoele_gen)
    histo_ele_genreco.Add(histo_tautoele_genreco)
    #mu + tautomu
    histo_mu_gen.Add(histo_tautomu_gen)
    histo_mu_genreco.Add(histo_tautomu_genreco)

    #remove bins with small number of entries
    cleanHisto(histo_ele_gen)
    cleanHisto(histo_mu_gen)
    cleanHisto(histo_jet_gen)
    #cleanHisto(histo_ele_genreco)
    #cleanHisto(histo_mu_genreco)
    #cleanHisto(histo_jet_genreco)

    histo_eff_ele   = histo_ele_genreco.Clone("eff_ele")
    histo_eff_mu   = histo_mu_genreco.Clone("eff_mu")
    histo_eff_jet   = histo_jet_genreco.Clone("eff_jet")

    histo_eff_ele.Divide(histo_ele_gen)   
    histo_eff_mu.Divide(histo_mu_gen)      
    histo_eff_jet.Divide(histo_jet_gen)      

    plotPretty(histo_eff_ele,"histo_eff_ele_WW.eps")
    plotPretty(histo_eff_mu,"histo_eff_mu_WW.eps")
    plotPretty(histo_eff_jet,"histo_eff_jet_WW.eps")

    output = root.TFile.Open("efficiency_WW_forClosure.root","RECREATE")
    histo_eff_ele.Write()
    histo_eff_mu.Write()
    histo_eff_jet.Write()
    histo_eff_event.Write()
