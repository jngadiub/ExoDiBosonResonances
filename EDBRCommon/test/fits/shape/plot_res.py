#!/usr/bin/env python


import ROOT as root
import optparse
import os,sys
import math


def main():
    parser = optparse.OptionParser(description='resolution plotter')
    parser.add_option('-f','--filepath',  help='path to input trees, default: pars'                             , default = "pars" )                                      

    
    ( args, XXX) = parser.parse_args()
 
    root.gROOT.SetBatch(True)
 
    print 'PRINTING ARGUMENTS:'
    print args

    filelist = []
    for file in os.listdir(args.filepath):
        if "outpars" in file: # only look at final values
            filelist.append(args.filepath + "/" +  file)
                
    mean = root.RooRealVar("mean_match","mean_match",0)
    sigma = root.RooRealVar("sigma_match","sigma_match",0)
    args = root.RooArgSet(mean,sigma)

    vallists = dict()
    
    for file in filelist:
        print 'File is ',file
        mass = 0
        gamma = 0
        flavor = 0
        substrings = file.split("_")
        for substr in substrings:
            if len(substr)>1 and substr[0]=="M" and substr[1]!="U":
                mass = float(substr[1:])
            if len(substr)>0 and  substr[0]=="G":
                gamma = float(substr[1:])
                if gamma < 10:
                    gamma = 5
            if substr=="MU.config":
                flavor = -1
            if substr=="ELE.config":
                flavor = 1
            
        if mass == 0 or gamma == 0 or flavor == 0:
            print "Could not deduce sample properties"
            continue

        gamma = gamma*flavor

        args.readFromFile(file)
        if gamma in vallists:
            vallists[gamma].append((mass,sigma.getVal()/mass))            
        else:
            vallists[gamma]=[(mass,sigma.getVal()/mass)]
        
    canvas = root.TCanvas()
    dummy = root.TH1F("dummy","dummy",1,500,3000)
    dummy.SetMinimum(0)
    dummy.SetMaximum(0.1)
    dummy.GetXaxis().SetTitle("Mass")
    dummy.GetYaxis().SetTitle("#sigma /M")
    dummy.Draw()

    leg=root.TLegend(0.8,0.8,1.0,0.2)

    graphsaver=[]
    colormap={}
    color=1
    print vallists
    for key,list in vallists.iteritems():
        sortedlist = sorted(list,key = lambda pair: pair[0])
        print sortedlist
        graph  = root.TGraph(len(sortedlist))
        n=0
        for pair in sortedlist:
            graph.SetPoint(n,pair[0],pair[1])
            n=n+1
        if abs(key) in colormap:
            col = colormap[abs(key)]
        else:
            col = color
            color += 1
            colormap[abs(key)]=col
        graph.SetLineColor(col)
        graph.SetMarkerColor(col)
        graph.SetLineStyle(int(key/abs(key))+1)
        graph.SetMarkerStyle(34)
        graph.SetMarkerSize(2)
        graph.SetLineWidth(5)
        graph.Draw("same,PL")
        graphsaver.append(graph)
        label = "#Gamma =" + str(abs(key)) +" ("
        if(key>0):
            label +="ele"
        else:
            label +="mu"
        label+=")"
        leg.AddEntry(graph,label,"LP")


    leg.Draw()
    canvas.SaveAs("test.eps")


if __name__ == "__main__":
    main()
