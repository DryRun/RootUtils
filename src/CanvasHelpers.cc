#include "MyTools/RootUtils/interface/CanvasHelpers.h"

#include <iostream>

#include "TLatex.h"
#include "TLine.h"
#include "TPave.h"
#include "TMarker.h"
#include "TPad.h"
#include "TROOT.h"

namespace Root {
  void CMSLabel(double x,double y,const char* text,Color_t color, double p_size_modifier, double p_offset) 
  {
    TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
    l.SetNDC();
    l.SetTextFont(62);
    l.SetTextSize(0.08 * p_size_modifier);
    l.SetTextColor(color);

    double delx = 0.075*696*gPad->GetWh()/(472*gPad->GetWw()) * 1.7 * p_size_modifier * p_offset;

    l.DrawLatex(x,y,"CMS");
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
  void CMSLabelTwoPane(double x,double y,const char* text,Color_t color, double p_size_modifier) 
  {
    TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
    l.SetNDC();
    l.SetTextFont(62);
    l.SetTextSize(0.08 * p_size_modifier);
    l.SetTextColor(color);

    double delx = 0.075*696*gPad->GetWh()/(472*gPad->GetWw()) * 0.85 * p_size_modifier;

    l.DrawLatex(x,y,"CMS");
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

  void SetCanvasStyle ()
  {
    static TStyle* canvasStyle = 0;
    std::cout << "\nApplying canvas style settings...\n" << std::endl ;
    if ( canvasStyle==0 ) canvasStyle = CanvasStyle();
    gROOT->SetStyle("CMS");
    gROOT->ForceStyle();
  }

  TStyle* CanvasStyle() 
  {
    TStyle *canvasStyle = new TStyle("CMS","Canvas style");

    // use plain black on white colors
    Int_t icol=0; // WHITE
    canvasStyle->SetFrameBorderMode(icol);
    canvasStyle->SetFrameFillColor(icol);
    canvasStyle->SetCanvasBorderMode(icol);
    canvasStyle->SetCanvasColor(icol);
    canvasStyle->SetPadBorderMode(icol);
    canvasStyle->SetPadColor(icol);
    canvasStyle->SetStatColor(icol);
    //canvasStyle->SetFillColor(icol); // don't use: white fill color for *all* objects

    // set the paper & margin sizes
    canvasStyle->SetPaperSize(20,26);

    // set margin sizes
    canvasStyle->SetPadTopMargin(0.05);
    canvasStyle->SetPadRightMargin(0.05);
    canvasStyle->SetPadBottomMargin(0.16);
    canvasStyle->SetPadLeftMargin(0.16);

    // set title offsets (for axis label)
    canvasStyle->SetTitleXOffset(1.4);
    canvasStyle->SetTitleYOffset(1.4);

    // use large fonts
    //Int_t font=72; // Helvetica italics
    Int_t font=42; // Helvetica
    double tsize=0.05;
    canvasStyle->SetTextFont(font);

    canvasStyle->SetTextSize(tsize);
    canvasStyle->SetLabelFont(font,"x");
    canvasStyle->SetTitleFont(font,"x");
    canvasStyle->SetLabelFont(font,"y");
    canvasStyle->SetTitleFont(font,"y");
    canvasStyle->SetLabelFont(font,"z");
    canvasStyle->SetTitleFont(font,"z");
    
    canvasStyle->SetLabelSize(tsize,"x");
    canvasStyle->SetTitleSize(tsize,"x");
    canvasStyle->SetLabelSize(tsize,"y");
    canvasStyle->SetTitleSize(tsize,"y");
    canvasStyle->SetLabelSize(tsize,"z");
    canvasStyle->SetTitleSize(tsize,"z");

    // use bold lines and markers
    canvasStyle->SetMarkerStyle(20);
    canvasStyle->SetMarkerSize(1.2);
    canvasStyle->SetHistLineWidth(2.);
    canvasStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

    // get rid of X error bars 
    //canvasStyle->SetErrorX(0.001);
    // get rid of error bar caps
    canvasStyle->SetEndErrorSize(0.);

    // do not display any of the standard histogram decorations
    canvasStyle->SetOptTitle(0);
    //canvasStyle->SetOptStat(1111);
    canvasStyle->SetOptStat(0);
    //canvasStyle->SetOptFit(1111);
    canvasStyle->SetOptFit(0);

    // put tick marks on top and RHS of plots
    canvasStyle->SetPadTickX(1);
    canvasStyle->SetPadTickY(1);

    return canvasStyle;

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
}
