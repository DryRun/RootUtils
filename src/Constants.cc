#ifndef Constants_cxx
#define Constants_cxx

#include "MyTools/RootUtils/interface/Constants.h"

namespace Constants {
	double ElectronMass(EnergyUnit p_unit) {
		return 0.51099891 / TMath::Power(10, p_unit - MeV);
	}

	double MuonMass(EnergyUnit p_unit) {
		return 105.6583715 / TMath::Power(10, p_unit - MeV);
	}

	double TauMass(EnergyUnit p_unit) {
		return 1776.82 / TMath::Power(10, p_unit - MeV);
	}

	double ZMass(EnergyUnit p_unit) {
		return 91.1876 / TMath::Power(10, p_unit - GeV);
	}

	double WMass(EnergyUnit p_unit) {
		return 80.385 / TMath::Power(10, p_unit - GeV);
	}

	double HiggsMass(EnergyUnit p_unit) {
		return 125.9 / TMath::Power(10, p_unit - GeV);
	}

}

#endif