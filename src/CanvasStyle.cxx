//
// Canvas Style, based on a style file from ATLAS/BaBar
//

#include <iostream>

#include "MyTools/RootUtils/interface/CanvasStyle.h"

#include "TROOT.h"

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

