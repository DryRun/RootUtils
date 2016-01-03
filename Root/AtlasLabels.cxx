#include "atlasstyle/AtlasLabels.h"

#include "TLatex.h"
#include "TLine.h"
#include "TPave.h"
#include "TMarker.h"
#include "TPad.h"


void ATLASLabel(double x,double y,const char* text,Color_t color, double p_size_modifier) 
{
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextSize(0.08 * p_size_modifier);
  l.SetTextColor(color);

  double delx = 0.115*696*gPad->GetWh()/(472*gPad->GetWw()) * 1.7 * p_size_modifier;

  l.DrawLatex(x,y,"ATLAS");
  if (text) {
    TLatex p; 
    p.SetNDC();
    p.SetTextFont(42);
    p.SetTextSize(0.08 * p_size_modifier);
    p.SetTextColor(color);
    p.DrawLatex(x+delx,y,text);
    //    p.DrawLatex(x,y,"#sqrt{s}=900GeV");
  }
}


void ATLASLabelOld(double x,double y,bool Preliminary,Color_t color) 
{
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextColor(color);
  l.DrawLatex(x,y,"ATLAS");
  if (Preliminary) {
    TLatex p; 
    p.SetNDC();
    p.SetTextFont(42);
    p.SetTextColor(color);
    p.DrawLatex(x+0.115,y,"Preliminary");
  }
}



void ATLASVersion(const char* version,double x,double y,Color_t color) 
{

  if (version) {
    char versionString[100];
    sprintf(versionString,"Version %s",version);
    TLatex l;
    l.SetTextAlign(22); 
    l.SetTextSize(0.04); 
    l.SetNDC();
    l.SetTextFont(72);
    l.SetTextColor(color);
    l.DrawLatex(x,y,versionString);
  }
}



void myText(double x,double y,Color_t color,const char *text, double p_size_modifier) 
{
  double tsize=0.08 * p_size_modifier;
  TLatex l; //l.SetTextAlign(12); 
  l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}
/*
void myText(double x, double y, Color_t color, const char *text, double size_scale) 
{
  double tsize=0.08 * size_scale;
  TLatex l; //l.SetTextAlign(12); 
  l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}
*/
 
void myBoxText(double x, double y,double boxsize,Int_t mcolor,const char *text) 
{
  double tsize=0.06;

  TLatex l; l.SetTextAlign(12); //l.SetTextSize(tsize); 
  l.SetNDC();
  l.DrawLatex(x,y,text);

  double y1=y-0.25*tsize;
  double y2=y+0.25*tsize;
  double x2=x-0.3*tsize;
  double x1=x2-boxsize;

  printf("x1= %f x2= %f y1= %f y2= %f \n",x1,x2,y1,y2);

  TPave *mbox= new TPave(x1,y1,x2,y2,0,"NDC");

  mbox->SetFillColor(mcolor);
  mbox->SetFillStyle(1001);
  mbox->Draw();

  TLine mline;
  mline.SetLineWidth(4);
  mline.SetLineColor(1);
  mline.SetLineStyle(1);
  double y_new=(y1+y2)/2.;
  mline.DrawLineNDC(x1,y_new,x2,y_new);

}

void myMarkerText(double x,double y,Int_t color,Int_t mstyle,const char *text) 
{
  //  printf("**myMarker: text= %s\ m ",text);

  double tsize=0.06;
  TMarker *marker = new TMarker(x-(0.4*tsize),y,8);
  marker->SetMarkerColor(color);  marker->SetNDC();
  marker->SetMarkerStyle(mstyle);
  marker->SetMarkerSize(1.0);
  marker->Draw();

  TLatex l; l.SetTextAlign(12); //l.SetTextSize(tsize); 
  l.SetNDC();
  l.DrawLatex(x,y,text);
}

