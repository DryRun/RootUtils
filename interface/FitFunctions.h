#ifndef FitFunctions_h
#define FitFunctions_h

#include "TMath.h"

namespace FitFunctions {

	inline double GaussPlusExp(double * x, double * par) {
    	return par[0] * par[1] * TMath::Exp(-1. * par[1] * x[0]) + par[2] / (TMath::Sqrt(2*TMath::Pi()) * par[3]) * TMath::Exp(-0.5 * TMath::Power(x[0] - par[4], 2) / (2 * TMath::Power(par[3], 2)));
	}

}

#endif