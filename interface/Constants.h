#ifndef Constants_h
#define Constants_h

#include <stdlib.h>
#include "TROOT.h"
#include "TMath.h"

namespace Constants {

	enum EnergyUnit {
		uev = -6,
		mev = -3,
		eV  = 0,
		keV = 3,
		MeV = 6,
		GeV = 9,
		TeV = 12
	};

	double ElectronMass(EnergyUnit p_unit);
	double MuonMass(EnergyUnit p_unit);
	double TauMass(EnergyUnit p_unit);
	double ZMass(EnergyUnit p_unit);
	double WMass(EnergyUnit p_unit);
	double HiggsMass(EnergyUnit p_unit);


}




#endif