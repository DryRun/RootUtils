/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 *    File: $Id: RooCBPlusVoigtian.h,v 1.7 2007/07/12 20:30:49 wouter Exp $
 * Authors:                                                                  *
 *   TS, Thomas Schietinger, SLAC,          schieti@slac.stanford.edu        *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

#ifndef RooCBPlusVoigtian_h
#define RooCBPlusVoigtian_h

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;

class RooCBPlusVoigtian : public RooAbsPdf {
public:
	RooCBPlusVoigtian() {} ;
	RooCBPlusVoigtian(const char *name, const char *title,
	RooAbsReal& x, 
	RooAbsReal& mean, 
	RooAbsReal& voigt_width, 
	RooAbsReal& voigt_sigma,
	RooAbsReal& cb_sigma,
	RooAbsReal& cb_alpha,
	RooAbsReal& cb_n,
	RooAbsReal& f_cb);
	RooCBPlusVoigtian(const RooCBPlusVoigtian& other, const char* name=0) ;
	virtual TObject* clone(const char* newname) const { return new RooCBPlusVoigtian(*this,newname); }
	inline virtual ~RooCBPlusVoigtian() { }

// These methods allow the user to select the fast evaluation
// of the complex error function using look-up tables
// (default is the "slow" CERNlib algorithm)

	//inline void selectFastAlgorithm()    { _doFast = kTRUE;  }
	//inline void selectDefaultAlgorithm() { _doFast = kFALSE; }

protected:

	RooRealProxy x_ ;
	RooRealProxy mean_ ;
	RooRealProxy voigt_width_ ;
	RooRealProxy voigt_sigma_ ;
	RooRealProxy cb_sigma_;
	RooRealProxy cb_alpha_;
	RooRealProxy cb_n_;
	RooRealProxy f_cb_;

	Double_t evaluate() const ;

private:

	Double_t _invRootPi;
	Double_t ApproxErf(Double_t arg) const;
	ClassDef(RooCBPlusVoigtian,1) // CB + Voigtian PDF (Gauss (x) BreitWigner)
};

#endif
