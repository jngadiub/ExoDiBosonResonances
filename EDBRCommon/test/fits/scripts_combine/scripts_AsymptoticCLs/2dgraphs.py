import ROOT as root
from array import *
import math

def createColors(i):
##     r = [0., 0.0, 1.0, 1.0, 1.0]
##     g = [0., 0.0, 0.0, 1.0, 1.0]
##     b = [0., 1.0, 0.0, 0.0, 1.0]
##     stop = [0., .25, .50, .75, 1.0]
    r = [1.0, 0.0, 0.0]
    g = [0.0, 1.0, 0.0]
    b = [1.0, 0.0, 1.0]
    stop = [0.,.50, 1.0]
    ra= array('d',r)
    ga= array('d',g)
    ba= array('d',b)
    stopa= array('d',stop)
    FI = root.TColor.CreateGradientColorTable(3, stopa, ra, ga, ba, i);
    
def setColors2d():
    #   black  blue  red  yellow   white
#    r = [0.,   0.0,  1.0, 1.0,     1.0]
#    g = [0.,   0.0,  0.0, 1.0,     1.0]
#    b = [0.,   1.0,  0.0, 0.0,     1.0]

    #   purple  blue   light-blue cyan  light-green  darker-green    yellow  dark-yell   orange    red
    #   r = [0.3,   0.0,    0.0,      0.2 ,   0.2,      0.0 ,  0.6 ,    1.0 ,    0.6,  1.0 ,    0.8]
    #   g = [0.,    0.0,    0.5,      1.0 ,   1.0,      1.0 , 1.0 ,     1.0 ,    0.6,   0.5 ,    0.0]
    #   b = [0.6,   1.0,    1.0,      1.0 ,   0.6,      0.0 , 0.2 ,     0.0 ,    0.0,  0.0 ,    0.0]
    r = [0.3,   0.0,    0.0,      0.2 ,     0.2 , 0.4, 0.7, 0.6 ,    1.0 ,    0.6,  1.0 ,    0.8]
    g = [0.,    0.0,    0.5,      1.0 ,     1.0 , 1.0, 1.0, 1.0 ,     1.0 ,    0.6,   0.5 ,    0.0]
    b = [0.6,   1.0,    1.0,      1.0 ,     0.6 , 0.4, 0.4, 0.2 ,     0.0 ,    0.0,  0.0 ,    0.0]

    stop = [0., 0.04, 0.08, 0.12,0.16, 0.20, 0.24,  0.36, 0.50, 0.675, 0.75, 1.0]
    ra= array('d',r)
    ga= array('d',g)
    ba= array('d',b)
    stopa= array('d',stop)
    FI = root.TColor.CreateGradientColorTable(12, stopa, ra, ga, ba, 200);
#    for i in 1,200:
#        MyDensePalette[i] = FI+i;


    #   purple  blue   light-blue   light-green  darker-green    yellow  orange    red
    r = [0.3,   0.0,    0.0,         0.2,        0.6 ,           1.0 ,   1.0 ,    0.8]
    g = [0.,    0.0,    0.5,         1.0,        1.0 ,           1.0 ,  0.5 ,    0.0]
    b = [0.6,   1.0,    1.0,         0.6,        0.2 ,           0.0 ,  0.0 ,    0.0]
    stop = [0., 0.125, 0.25, 0.375, 0.50, 0.675, 0.75, 0.875, 1.0]
    ## stop = [0.,0.06, 0.125, 0.18, 0.25,0.375, 0.50, 0.75, 1.0]
    ##stop = [0.,  0.25,  0.50, 0.675, 0.75, 0.80, 0.875, 0.92, 1.0]
    ra= array('d',r)
    ga= array('d',g)
    ba= array('d',b)
    stopa= array('d',stop)
    FI2 = root.TColor.CreateGradientColorTable(8, stopa, ra, ga, ba, 100);
   


def setFPStyle():
    

  root.gStyle.SetPadBorderMode(0)
  root.gStyle.SetFrameBorderMode(0)
  root.gStyle.SetPadBottomMargin(0.12)
  root.gStyle.SetPadLeftMargin(0.12)
  root.gStyle.SetCanvasColor(root.kWhite)
  root.gStyle.SetCanvasDefH(600) #Height of canvas
  root.gStyle.SetCanvasDefW(600) #Width of canvas
  root.gStyle.SetCanvasDefX(0)   #POsition on screen
  root.gStyle.SetCanvasDefY(0)

  root.gStyle.SetPadTopMargin(0.05)
  root.gStyle.SetPadBottomMargin(0.15)#0.13)
  root.gStyle.SetPadLeftMargin(0.15)#0.16)
  root.gStyle.SetPadRightMargin(0.05)#0.02)



 # For the Pad:
  root.gStyle.SetPadBorderMode(0)
  # root.gStyle.SetPadBorderSize(Width_t size = 1)
  root.gStyle.SetPadColor(root.kWhite)
  root.gStyle.SetPadGridX(root.kFALSE)
  root.gStyle.SetPadGridY(root.kFALSE)
  root.gStyle.SetGridColor(0)
  root.gStyle.SetGridStyle(3)
  root.gStyle.SetGridWidth(1)

  # For the frame:
  root.gStyle.SetFrameBorderMode(0)
  root.gStyle.SetFrameBorderSize(1)
  root.gStyle.SetFrameFillColor(0)
  root.gStyle.SetFrameFillStyle(0)
  root.gStyle.SetFrameLineColor(1)
  root.gStyle.SetFrameLineStyle(1)
  root.gStyle.SetFrameLineWidth(1)

  root.gStyle.SetAxisColor(1, "XYZ")
  root.gStyle.SetStripDecimals(root.kTRUE)
  root.gStyle.SetTickLength(0.03, "XYZ")
  root.gStyle.SetNdivisions(510, "XYZ")
  root.gStyle.SetPadTickX(1)  # To get tick marks on the opposite side of the frame
  root.gStyle.SetPadTickY(1)
  root.gStyle.SetGridColor(0)
  root.gStyle.SetGridStyle(3)
  root.gStyle.SetGridWidth(1)


  root.gStyle.SetTitleColor(1, "XYZ")
  root.gStyle.SetTitleFont(42, "XYZ")
  root.gStyle.SetTitleSize(0.05, "XYZ")
  # root.gStyle.SetTitleXSize(Float_t size = 0.02) # Another way to set the size?
  # root.gStyle.SetTitleYSize(Float_t size = 0.02)
  root.gStyle.SetTitleXOffset(1.15)#0.9)
  root.gStyle.SetTitleYOffset(1.3) # => 1.15 if exponents
  root.gStyle.SetLabelColor(1, "XYZ")
  root.gStyle.SetLabelFont(42, "XYZ")
  root.gStyle.SetLabelOffset(0.007, "XYZ")
  root.gStyle.SetLabelSize(0.045, "XYZ")

  root.gStyle.SetPadBorderMode(0)
  root.gStyle.SetFrameBorderMode(0)
  root.gStyle.SetTitleTextColor(1)
  root.gStyle.SetTitleFillColor(10)
  root.gStyle.SetTitleFontSize(0.05)

  root.gStyle.SetOptStat(0)

  for i in xrange(10):
      lsstr = "[12 12"
      lsstr +=" "+str(4*(i+1)) +" 12"
      lsstr +="]"
      print lsstr
      root.gStyle.SetLineStyleString(11+i,lsstr)


  





def main():
    root.gROOT.SetBatch(True)

    widths=[]

    f = open('widths.txt')
    for line in iter(f):
        if float(line.strip())<=0.4 : widths.append(line.strip())


    createColors(len(widths))
    
    sw = sorted(widths , key=float)

    f.close()

    expected_graphs = {}
    for width in sw:
        fn = "AsymptoticCLs_TGraph_"+width+".root"
        file = root.TFile.Open(fn)
        expected_graphs[width] = file.Get("LimitExpectedCLs")
        


    setFPStyle()

    cMCMC=root.TCanvas("c_lim_Asymp","canvas with limits for Asymptotic CLs",630,600)
    cMCMC.cd()
    cMCMC.SetGridx(1)
    cMCMC.SetGridy(1)

    hr = cMCMC.DrawFrame(600,0.8,2500,400.0,"")
    hr.SetXTitle("M_{X} [GeV]")
    hr.SetYTitle("N_{events} excl. at 95% C.L.") #rightarrow 2l2q
    hr.GetXaxis().SetNdivisions(505);
    
    n=0
    for width in sw:
        expected_graphs[width].SetLineStyle(11+n)
        expected_graphs[width].SetLineColor(root.TColor.GetColorPalette(n))
        expected_graphs[width].Draw("same")
        n+=1

        
    leg = root.TLegend(.66,.55,.94,.92);
   
    leg.SetFillColor(0);
    leg.SetShadowColor(0);
    leg.SetTextFont(42);
    leg.SetTextSize(0.025);
    #   leg.SetBorderMode(0);
    #if(unblind)leg.AddEntry(grobslim_cls, "Asympt. CL_{S} Observed", "LP");
    #leg.AddEntry(gr68_cls, "Asympt. CL_{S}  Expected #pm 1#sigma", "LF");
    #leg.AddEntry(gr95_cls, "Asympt. CL_{S}  Expected #pm 2#sigma", "LF");
    for width in sw:        
        leg.AddEntry(expected_graphs[width], "#Gamma= "+width+" #times M_{X}", "L" ) #rightarrow 2l2q

    leg.Draw();


    banner1 = root.TLatex(0.15,0.96,"CMS");
    banner1.SetNDC();
    banner1.SetTextSize(0.04);
    banner1.SetTextFont(42);
    banner1.Draw();
    #      banner2 = TLatex(0.68,0.96,("%.1f fb^{-1} at #sqrt{s}=8TeV  2%s-1j"%(self.GetLumi(),self.channel)));
    banner2 = root.TLatex(0.64,0.96,("%.1f fb^{-1} at #sqrt{s}=8TeV"%(19.7)));
    banner2.SetNDC();
    banner2.SetTextSize(0.04);
    banner2.SetTextFont(42);
    banner2.Draw();

    root.gPad.RedrawAxis("")
    cMCMC.Update()
    cMCMC.SaveAs("test.eps")
    cMCMC.SaveAs("test.pdf")
    cMCMC.SaveAs("test.root")
    root.gPad.SetLogy();
    cMCMC.SaveAs("test_log.eps")
    cMCMC.SaveAs("test_log.pdf")
    root.gPad.SetLogy(False);

    #plot as 2d histo
    binsy = [0.000]
    for i in xrange(len(sw)):
       ## if i==0: sw[i]=0.0 #### it is 0.001 in array but we pretend it is 0
        print 'Width: ',sw[i]
    
        if i==0:
            binsy.append(0.5*float(sw[i+1]))
        elif i != len(sw)-1:
            binsy.append(0.5*(float(sw[i])+float(sw[i+1])))
        else:
            binsy.append(float(sw[i])+0.025)
            binsy.append(float(sw[i])+0.040)#extra margin at the top

            
    print "Bins of Y-axis of 2D histo: ",binsy

    binsx=[]
    d1, d2 = root.Double(0), root.Double(0)
    expected_graphs[sw[0]].GetPoint( 0 , d1, d2 )
    last = float(d1)
    for i in xrange( expected_graphs[sw[0]].GetN()):
        expected_graphs[sw[0]].GetPoint( i , d1, d2 )
        binsx.append(0.5*(last+d1))
        last=float(d1)

    binsx.append(last+(last-binsx[expected_graphs[sw[0]].GetN()-1])  )
    binsx[0]=binsx[0]-(binsx[1]-binsx[0])

    print binsx
    binsxa=array('d',binsx)
    binsya=array('d',binsy)
    limhist = root.TH2D("limhist","limhist",len(binsx)-1,binsxa,len(binsy)-1,binsya)
    
    for w in sw:
        for m in xrange( expected_graphs[w].GetN()):
            expected_graphs[w].GetPoint( m , d1, d2 )
            #            limhist.Fill(d1,float(w),math.log10(d2) )
            limhist.Fill(d1,float(w),d2 )

    
    setColors2d()
    limhist.GetXaxis().SetNdivisions(504)
    #limhist.GetXaxis().SetNdivisions(limhist.GetNbinsX())
    limhist.GetXaxis().SetTitle("M_{X} [GeV]")
    limhist.GetXaxis().SetTitleFont(42)
    limhist.GetXaxis().SetTitleSize(0.06)
    limhist.GetXaxis().SetTitleOffset(1.05)

    limhist.GetYaxis().SetNdivisions(limhist.GetNbinsY())
    limhist.GetYaxis().SetTitle("#Gamma / M_{X}")
    limhist.GetYaxis().SetTitleFont(42)
    limhist.GetYaxis().SetTitleSize(0.06)
    limhist.GetYaxis().SetTitleOffset(1.05)
    
    limhist.GetZaxis().SetTitle("N_{events} excl. at 95% C.L.")
    limhist.GetZaxis().SetTitleFont(42)
    limhist.GetZaxis().SetTitleSize(0.04)
    limhist.GetZaxis().SetTitleOffset(1.1)
    
    root.gPad.SetRightMargin(0.17)
    root.gPad.SetGridx()
    root.gPad.SetGridy()
    ###root.gStyle.SetPaintTextFormat(".2f")
    limhist.Draw("COLZ ")


    banner1.Draw();
    banner2.SetX(0.55);
    banner2.Draw();
    
    #root.gPad.RedrawAxis("")
    #cMCMC.Update()
    cMCMC.SaveAs("test2d.eps")
    cMCMC.SaveAs("test2d.pdf")
    cMCMC.SaveAs("test2d.png")
    cMCMC.SaveAs("test2d.root")
    ##cMCMC.SetLogZ(1);
    cMCMC.SaveAs("test2d_logZ.eps")
    cMCMC.SaveAs("test2d_logZ.pdf")


if __name__ == "__main__":
    main()
