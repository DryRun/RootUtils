//
//   @file    CanvasUtils.h         
//   
//
//   @author M.Sutton
// 
//   Copyright (C) 2010 Atlas Collaboration
//
//   $Id: CanvasUtils.h 289371 2012-05-23 16:24:19Z spagan $


#ifndef CanvasUtils_h
#define CanvasUtils_h

#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"

void CANVAS_LABEL(double x,double y,Color_t color=1); 

TGraphErrors* myTGraphErrorsDivide(TGraphErrors* g1,TGraphErrors* g2);

TGraphAsymmErrors* myTGraphErrorsDivide(TGraphAsymmErrors* g1,TGraphAsymmErrors* g2);

TGraphAsymmErrors* myMakeBand(TGraphErrors* g0, TGraphErrors* g1,TGraphErrors* g2);

void myAddtoBand(TGraphErrors* g1, TGraphAsymmErrors* g2);

TGraphErrors* TH1TOTGraph(TH1 *h1);

void myText(double x,double y,Color_t color,TString text);

void myText(double x,double y,Color_t color,TString text, double size);

void myBoxText(double x, double y,double boxsize,Int_t mcolor,char *text);

void myMarkerText(double x,double y,Int_t color,Int_t mstyle,char *text,Float_t msize=2.); 

#endif // CanvasUtils_h
