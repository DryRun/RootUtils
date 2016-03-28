#ifndef DryLegend_h
#define DryLegend_h

#include "TLegend.h"

namespace Root {
	class DryLegend : public TLegend {
		/**
		  * Class DryLegend
		  * - Simple extension of the ROOT TLegend to match the ATLAS style.
		  */

		ClassDef(DryLegend, 1);

		/*** Public Methods ***/
	public:
		// Empty constructor, per ROOT requirements
		DryLegend();

		// TLegend constructor
		DryLegend(double x1, double y1, double x2, double y2, const char* header = "", Option_t* option = "brNDC");

		~DryLegend();


	};
}


#endif