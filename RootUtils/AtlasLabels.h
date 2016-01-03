//
//   @file    AtlasLabels.h         
//   
//   @author M.Sutton
// 
//   Copyright (C) 2010 Atlas Collaboration
//
//   $Id: AtlasLabels.h 289371 2012-05-23 16:24:19Z spagan $


#ifndef __ATLASLABELS_H
#define __ATLASLABELS_H

#include "Rtypes.h"

void ATLASLabel(double x,double y,const char* text=NULL,Color_t color=1, double p_size_modifier = 1.); 

void ATLASLabelOld(double x,double y,bool Preliminary=false,Color_t color=1); 

void ATLASVersion(const char* version=NULL,double x=0.88,double y=0.975,Color_t color=1); 

void myText(double x,double y,Color_t color,const char *text, double p_size_modifier = 1.); 

//void myText(double x, double y, Color_t color, const char *text, double size_scale);

void myBoxText(double x, double y,double boxsize,Int_t mcolor,const char *text); 

void myMarkerText(double x,double y,Int_t color,Int_t mstyle,const char *text); 

#endif // __ATLASLABELS_H
