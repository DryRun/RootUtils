#ifndef CanvasHelpers_h
#define CanvasHelpers_h

#include "Rtypes.h"
#include "TStyle.h"
#include <iostream>

namespace Root {

  void CMSLabel(double x,double y,const char* text=NULL,Color_t color=1, double p_size_modifier = 1., double p_offset = 1.); 

	void CMSLabelTwoPane(double x,double y,const char* text=NULL,Color_t color=1, double p_size_modifier = 1.); 

	void myText(double x,double y,Color_t color,const char *text, double p_size_modifier = 1.); 

	void myBoxText(double x, double y,double boxsize,Int_t mcolor,const char *text); 

	void myMarkerText(double x,double y,Int_t color,Int_t mstyle,const char *text); 

	void SetCanvasStyle();

	TStyle* CanvasStyle(); 
}

#endif
