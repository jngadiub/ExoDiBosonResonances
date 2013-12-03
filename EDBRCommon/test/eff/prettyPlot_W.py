#!/usr/bin/env python

import ROOT as root

root.gROOT.SetBatch()        # don't pop up canvases
root.gROOT.SetStyle('Plain') # white background
root.gStyle.SetOptStat(0) # white background
root.gStyle.SetOptTitle(0)
root.gStyle.SetPadRightMargin(0.12)
root.gStyle.SetPaintTextFormat(".2f")

def plotPretty(histo,filename,type):
    c=root.TCanvas("c","c",600,600)
    fPads1=root.TPad("fPads1", "", 0.07, 0.03, 0.9, 1)
    fPads1.Draw()
    fPads1.cd()

    histo.GetXaxis().SetTitle("p_{T,W} [GeV]")
    histo.GetYaxis().SetTitle("|#eta_{W}|    ")    
    histo.GetZaxis().SetTitle("Reconstruction #times ID Efficiency")
    histo.GetXaxis().SetTitleOffset(0.95)
    histo.GetYaxis().SetTitleOffset(1.)
    histo.GetZaxis().SetTitleOffset(1.6)
    histo.GetXaxis().SetTitleSize(0.045)
    histo.GetYaxis().SetTitleSize(0.045)
    histo.GetXaxis().SetNdivisions(505)
    histo.GetYaxis().SetNdivisions(506)
    histo.GetZaxis().SetRangeUser(0,1)
    #histo.GetYaxis().SetLimits(0,2.399)
    histo.GetYaxis().SetRangeUser(0,2.399)
    histo.SetMarkerSize(1.25)
    #histo.Draw("COLZ,TEXT90")
    histo.Draw()
    histo.Draw("COLZ")

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
        tex3 = root.TLatex(0.509497,0.83042,"W #rightarrow e#nu_{e} / e#nu_{e}#nu_{#tau}#nu_{#tau}")
        tex3.SetNDC()
        tex3.SetTextSize(0.044)
        tex3.SetTextFont(42)
        tex3.SetLineWidth(2)
        tex3.Draw()            

    if type==1:
        tex3 = root.TLatex(0.509497,0.83042,"W #rightarrow #mu#nu_{#mu} / #mu#nu_{#mu}#nu_{#tau}#nu_{#tau}")
        tex3.SetNDC()
        tex3.SetTextSize(0.044)
        tex3.SetTextFont(42)
        tex3.SetLineWidth(2)
        tex3.Draw()            

    if type==4:
        tex3 = root.TLatex(0.509497,0.83042,"W_{L} #rightarrow q#bar{q'} #rightarrow 1 jet")
        tex3.SetNDC()
        tex3.SetTextSize(0.044)
        tex3.SetTextFont(42)
        tex3.SetLineWidth(2)
        tex3.Draw()            
                
#         tex4 = root.TLatex(0.509497,0.83042,"Longitudinal")
#         tex4.SetNDC()
#         tex4.SetTextSize(0.044)
#         tex4.SetTextFont(42)
#         tex4.SetLineWidth(2)
#         tex4.Draw()

#         tex5 = root.TLatex(0.509497,0.784965,"W Polarization")
#         tex5.SetNDC()
#         tex5.SetTextSize(0.044)
#         tex5.SetTextFont(42)
#         tex5.SetLineWidth(2)
#         tex5.Draw()

    root.gPad.RedrawAxis()
    root.gPad.Modified()
    root.gPad.Update()
    c.Update()

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
            

histofile = root.TFile.Open("/afs/cern.ch/work/s/santanas/Releases/CMSSW_5_3_9_CMGrel_V5_15_0_ExoDiBosonResonances_GIT_production/CMSSW_5_3_9/src/ExoDiBosonResonances/EDBRCommon/test/eff/plotsEff_BulkG_c0p2_plus_wideRes_final_05_11_2013/efficiency_WW.root") #Bulk
#histofile = root.TFile.Open("/afs/cern.ch/work/s/santanas/Releases/CMSSW_5_3_9_CMGrel_V5_15_0_ExoDiBosonResonances_GIT_production/CMSSW_5_3_9/src/ExoDiBosonResonances/EDBRCommon/test/eff/plotsEff_RSG_c0p2_final_05_11_2013/efficiency_WW.root") #RS pythia (wrong angular distributions)
#histofile = root.TFile.Open("/afs/cern.ch/work/s/santanas/Releases/CMSSW_5_3_9_CMGrel_V5_15_0_ExoDiBosonResonances_GIT_production/CMSSW_5_3_9/src/ExoDiBosonResonances/EDBRCommon/test/eff/plotsEff_RSG_Madgraph_final_05_11_2013/efficiency_WW.root") #RS madgraph (correct angular distributions)


advancedPlots = 1

if advancedPlots == 0:
    histo_eff_ele   = histofile.Get("eff_ele")
    histo_eff_mu   = histofile.Get("eff_mu")
    histo_eff_tautoele   = histofile.Get("eff_tautoele")
    histo_eff_tautomu   = histofile.Get("eff_tautomu")
    histo_eff_jet   = histofile.Get("eff_jet")

    plotPretty(histo_eff_ele,"histo_eff_ele_WW.eps",0)
    plotPretty(histo_eff_mu,"histo_eff_mu_WW.eps",1)
    plotPretty(histo_eff_tautoele,"histo_eff_tautoele_WW.eps",2)
    plotPretty(histo_eff_tautomu,"histo_eff_tautomu_WW.eps",3)
    plotPretty(histo_eff_jet,"histo_eff_jet_WW.eps",4)

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

    plotPretty(histo_eff_ele,"histo_eff_ele_WW.png",0)
    plotPretty(histo_eff_mu,"histo_eff_mu_WW.png",1)
    plotPretty(histo_eff_jet,"histo_eff_jet_WW.png",4)
    plotPretty(histo_eff_ele,"histo_eff_ele_WW.pdf",0)
    plotPretty(histo_eff_mu,"histo_eff_mu_WW.pdf",1)
    plotPretty(histo_eff_jet,"histo_eff_jet_WW.pdf",4)

    output = root.TFile.Open("efficiency_WW_forClosure.root","RECREATE")
    histo_eff_ele.Write()
    histo_eff_mu.Write()
    histo_eff_jet.Write()
    histo_eff_event.Write()
