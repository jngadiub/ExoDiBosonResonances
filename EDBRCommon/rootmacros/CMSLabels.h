#include "TLatex.h"

TLatex* makeCMSPreliminaryURC(int energy = 7, double x = 0.67, double y = 0.85) {
  char buffer[256];
  sprintf(buffer,"CMS Preliminary");
  TLatex* tex = new TLatex(x,y,buffer);
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->Draw();

  sprintf(buffer,"#sqrt{s} = %i TeV",energy);
  tex = new TLatex(x,y-0.05,buffer);
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->Draw();
  return tex;
}

TLatex* makeCMSFinalURC(int energy = 7, double x = 0.67, double y = 0.85) {
  char buffer[256];
  sprintf(buffer,"CMS");
  TLatex* tex = new TLatex(x,y,buffer);
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->Draw();

  sprintf(buffer,"#sqrt{s} = %i TeV",energy);
  tex = new TLatex(x,y-0.05,buffer);
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->Draw();
  return tex;
}

/*
TLatex* makeCMSFinalTop(int energy = 7, double x = 0.15, double y = 0.94) {
  char buffer[256];
  sprintf(buffer,"CMS        #sqrt{s} = %i TeV",energy);
  TLatex* tex = new TLatex(x,y,buffer);
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->Draw();
  return tex;
}
*/
TLatex* makeCMSFinalTop(double x = 0.15, double y = 0.94) {
  char buffer[256];
  sprintf(buffer,"CMS ");
  TLatex* tex = new TLatex(x,y,buffer);
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->Draw();
  return tex;
}



TLatex* makeCMSPreliminaryTop(int energy = 7, double x = 0.15, double y = 0.94) {
  char buffer[256];
  sprintf(buffer,"CMS Preliminary  #sqrt{s} = %i TeV",energy);
  TLatex* tex = new TLatex(x,y,buffer);
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->SetTextSize(0.040);
  tex->Draw();
  return tex;
}

/*
TLatex* makeCMSLumi(double lumi = 5.0, double x = 0.666, double y = 0.72) {
  char buffer[256];
  sprintf(buffer,"#int L dt = %4.1lf fb^{-1}",lumi);
  TLatex* tex = new TLatex(x,y,buffer);
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->SetTextSize(0.030);
  tex->Draw();
  return tex;
}
*/
TLatex* makeCMSLumi(int energy = 7,double lumi = 5.0, double x = 0.666, double y = 0.72) {
  char buffer[256];
  sprintf(buffer,"#sqrt{s} = %i TeV,  #int L dt = %4.1lf fb^{-1}",energy,lumi);
  TLatex* tex = new TLatex(x,y,buffer);
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->SetTextSize(0.030);
  tex->Draw();
  return tex;
}

TLatex* makeChannelLabel(int nJets, int flavour, bool isZZchannel, double x = 0.75, double y = 0.94) {
  char buffer[256];
  char c;
  if(nJets==1)
    c='1';
  if(nJets==2)
    c='2';
  if(nJets==-1)
    c='X';

  if(flavour == 11 && isZZchannel == true)
    sprintf(buffer,"2e %cj channel",c);
  if(flavour == 13 && isZZchannel == true)
    sprintf(buffer,"2#mu %cj channel",c);
  if(flavour == 11 && isZZchannel == false)
    sprintf(buffer,"e#nu_{e} %cj channel",c);
  if(flavour == 13 && isZZchannel == false)
    sprintf(buffer,"#mu#nu_{#mu} %cj channel",c);
  TLatex* tex = new TLatex(x,y,buffer);
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->SetTextSize(0.030);
  tex->Draw();
  return tex;
}
