/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 * @(#)root/roofit:$Id$
 * Authors:                                                                  *
 *   TS, Thomas Schietinger, SLAC,           schieti@slac.stanford.edu       *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

/**
\file RooCBPlusVoigtian.cxx
\class RooCBPlusVoigtian
\ingroup Roofit

RooCBPlusVoigtian is an efficient implementation of the convolution of a 
Breit-Wigner with a Gaussian, making use of the complex error function.
RooFitCore provides two algorithms for the evaluation of the complex error 
function (the default CERNlib C335 algorithm, and a faster, look-up-table 
based method). By default, RooCBPlusVoigtian employs the default (CERNlib) 
algorithm. Select the faster algorithm either in the constructor, or with
the selectFastAlgorithm() method.
**/

#include <cmath>
#include <complex>

#include "RooFit.h"

#include "Riostream.h"

#include "MyTools/RootUtils/interface/RooCBPlusVoigtian.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooMath.h"

using namespace std;

ClassImp(RooCBPlusVoigtian)


////////////////////////////////////////////////////////////////////////////////

RooCBPlusVoigtian::RooCBPlusVoigtian(const char *name, const char *title,
	RooAbsReal& x, 
	RooAbsReal& mean, 
	RooAbsReal& voigt_width, 
	RooAbsReal& voigt_sigma,
	RooAbsReal& cb_sigma,
	RooAbsReal& cb_alpha,
	RooAbsReal& cb_n,
	RooAbsReal& f_cb) :
	RooAbsPdf(name,title),
	x_("x","Dependent",this,x),
	mean_("mean","Mean",this,mean),
	voigt_width_("voigt_width","Breit-Wigner Width",this,voigt_width),
	voigt_sigma_("voigt_sigma","Gauss Width",this,voigt_sigma),
	cb_sigma_("cb_sigma", "Crystal Ball Sigma", this, cb_sigma),
	cb_alpha_("cb_alpha", "Crystal Ball Alpha", this, cb_alpha),
	cb_n_("cb_n", "Crystal Ball n", this, cb_n),
	f_cb_("f_cb", "Fraction in Crystal Ball", this, f_cb)
{
	_invRootPi= 1./sqrt(atan2(0.,-1.));
}



////////////////////////////////////////////////////////////////////////////////

RooCBPlusVoigtian::RooCBPlusVoigtian(const RooCBPlusVoigtian& other, const char* name) : 
	RooAbsPdf(other,name), 
	x_("x", this, other.x_),
	mean_("mean", this, other.mean_),
	voigt_width_("voigt_width", this, other.voigt_width_),
	voigt_sigma_("voigt_sigma", this, other.voigt_sigma_),
	cb_sigma_("cb_sigma", this, other.cb_sigma_),
	cb_alpha_("cb_alpha", this, other.cb_alpha_),
	cb_n_("cb_n", this, other.cb_n_),
	f_cb_("f_cb", this, other.f_cb_)
{
	_invRootPi= 1./sqrt(atan2(0.,-1.));
}



////////////////////////////////////////////////////////////////////////////////

Double_t RooCBPlusVoigtian::evaluate() const
{
	// Voigtian part
	Double_t s = (voigt_sigma_>0) ? voigt_sigma_ : -voigt_sigma_ ;
	Double_t w = (voigt_width_>0) ? voigt_width_ : -voigt_width_ ;

	Double_t coef= -0.5/(s*s);
	Double_t arg = x_ - mean_;

	// return constant for zero width and sigma
	if (s==0. && w==0.) return 1.;

	// Breit-Wigner for zero sigma
	if (s==0.) return (1./(arg*arg+0.25*w*w));

	// Gauss for zero width
	if (w==0.) return exp(coef*arg*arg);

	// actual Voigtian for non-trivial width and sigma
	Double_t c = 1./(sqrt(2.)*s);
	Double_t a = 0.5*c*w;
	Double_t u = c*arg;
	std::complex<Double_t> z(u,a) ;
	std::complex<Double_t> v(0.) ;

	v = RooMath::faddeeva(z);
	Double_t voigt_part = c*_invRootPi*v.real();

	// CB part
	Double_t cb_part = 0.;
	Double_t t = (x_-mean_)/cb_sigma_;
	if (cb_alpha_ < 0) t = -t;

	Double_t absAlpha = fabs((Double_t)cb_alpha_);

	if (t >= -absAlpha) {
		cb_part = exp(-0.5*t*t);
	}
	else {
		Double_t a =  TMath::Power(cb_n_/absAlpha,cb_n_)*exp(-0.5*absAlpha*absAlpha);
		Double_t b= cb_n_/absAlpha - absAlpha; 

		cb_part = a/TMath::Power(b - t, cb_n_);
	}

	return (f_cb_ * cb_part) + ((1. - f_cb_) * voigt_part);
}


Double_t RooCBPlusVoigtian::ApproxErf(Double_t arg) const 
{
	static const double erflim = 5.0;
	if( arg > erflim )
		return 1.0;
	if( arg < -erflim )
		return -1.0;
	
	return RooMath::erf(arg);
}

