//
//   @file    CanvasLabels.h         
//   
//   @author M.Sutton
// 
//   Copyright (C) 2010 Atlas Collaboration
//
//   $Id: CanvasLabels.h 289371 2012-05-23 16:24:19Z spagan $


#ifndef CanvasLabels_h
#define CanvasLabels_h

#include "Rtypes.h"

void CanvasLabel(double x,double y,const char* text=NULL,Color_t color=1, double p_size_modifier = 1.); 

void CanvasLabelOld(double x,double y,bool Preliminary=false,Color_t color=1); 

void CanvasVersion(const char* version=NULL,double x=0.88,double y=0.975,Color_t color=1); 

void myText(double x,double y,Color_t color,const char *text, double p_size_modifier = 1.); 

//void myText(double x, double y, Color_t color, const char *text, double size_scale);

void myBoxText(double x, double y,double boxsize,Int_t mcolor,const char *text); 

void myMarkerText(double x,double y,Int_t color,Int_t mstyle,const char *text); 

#endif // CanvasLabels_h
