#!/usr/bin/env python

import ROOT as root

def plotPretty(histo):
    #get ptaxis
    ptbins  = histo.GetXaxis().GetXbins()
    nbinspt = histo.GetXaxis().GetNbins()
    etabins  = histo.GetYaxis().GetXbins()
    nbinseta = histo.GetYaxis().GetNbins()

    print "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
    print "\\begin{sidewaystable}[hb]"
    print "\centering"
    print "\\tiny"
    print "\\begin{tabular}{|l|",
    for i in range(nbinseta):
        print "c|",
    print "}"
    print "\hline"
    print "$\pt$ range in GeV&\multicolumn{"+str(nbinseta)+"}{c|}{$\eta$ range}\\\\"
    print "\hline"

    print " &",
    for i in range(nbinseta):
        print "$" + "%.1f" % etabins[i] +"-" + "%.1f" % etabins[i+1] +"$",
        if i != nbinseta-1:
            print "&",
        else:
            print "\\\\"
    print "\hline"
    for pt in range(nbinspt):
        print "$" + str(int(ptbins[pt]))+"-" +str(int(ptbins[pt+1]))+"$&",
        for eta in range(nbinseta):
            eff=histo.GetBinContent(pt+1,eta+1)
            effstring =""
            if eff==0.0:
                effstring="-"
            else:
                effstring = "%.2f" % eff
            print effstring,
            if eta != nbinseta-1:
                print "&",
            else:
                print "\\\\"
        
            
    

    print "\hline"
    print "\end{tabular}"
    print "\caption{}"
    print "\label{tab:}"
    print "\end{sidewaystable}"
    print "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
    print
    print

histofile = root.TFile.Open("efficiency.root")
histo_eff_ele   = histofile.Get("eff_ele")
histo_eff_mu   = histofile.Get("eff_mu")
histo_eff_jet   = histofile.Get("eff_jet")

plotPretty(histo_eff_ele)
plotPretty(histo_eff_mu)
plotPretty(histo_eff_jet)
