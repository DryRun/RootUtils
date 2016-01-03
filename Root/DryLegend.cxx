#ifndef DryLegend_cxx
#define DryLegend_cxx

#include "RootUtils/DryLegend.h"

namespace Root {

	DryLegend::DryLegend() : TLegend() {}

	DryLegend::DryLegend(double x1, double y1, double x2, double y2, const char* header, Option_t* option) : TLegend(x1, y1, x2, y2, header, option) {
		this->SetFillColor(0);
		this->SetBorderSize(1);
		this->SetTextFont(42);
	}

	DryLegend::~DryLegend() {}

}

#endif