//
//   @file    AtlasUtils.h         
//   
//
//   @author M.Sutton
// 
//   Copyright (C) 2010 Atlas Collaboration
//
//   $Id: AtlasUtils.h 289371 2012-05-23 16:24:19Z spagan $


#ifndef __ATLASUTILS_H
#define __ATLASUTILS_H

#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"

void ATLAS_LABEL(double x,double y,Color_t color=1); 

TGraphErrors* myTGraphErrorsDivide(TGraphErrors* g1,TGraphErrors* g2);

TGraphAsymmErrors* myTGraphErrorsDivide(TGraphAsymmErrors* g1,TGraphAsymmErrors* g2);

TGraphAsymmErrors* myMakeBand(TGraphErrors* g0, TGraphErrors* g1,TGraphErrors* g2);

void myAddtoBand(TGraphErrors* g1, TGraphAsymmErrors* g2);

TGraphErrors* TH1TOTGraph(TH1 *h1);

void myText(double x,double y,Color_t color,TString text);

void myText(double x,double y,Color_t color,TString text, double size);

void myBoxText(double x, double y,double boxsize,Int_t mcolor,char *text);

void myMarkerText(double x,double y,Int_t color,Int_t mstyle,char *text,Float_t msize=2.); 

#endif // __ATLASUTILS_H
