#ifndef EnvelopeCanvas_h
#define EnvelopeCanvas_h

#include "RootUtils/DryCanvas.h"

namespace Root {

	class EnvelopeCanvas : public DryCanvas {
		/**
		  * Class EnvelopeCanvas
		  * - Intelligently plot multiple histograms on the same canvas.
		  * - Similar to FakeFactorCanvas, except no ratio plot.
		  */

	    ClassDef(EnvelopeCanvas, 1);

	    /*** Public Methods ***/

	    /**
	      * Constructor
	      * - Copies the TCanvas constructor
	      */
	    EnvelopeCanvas();
		EnvelopeCanvas(const char* name, const char* title, Int_t ww, Int_t wh);


		/**
		  * AddReferenceHistogram
		  */
		void AddReferenceHistogram(TH1D *p_histogram, TString p_legend_entry = "Reference");


		/**
		  * MakeCanvas
		  * - Make the envelope TGraphAsymmErrors, and then draw everything
		  */
		void MakeCanvas(); 

		/**
		  * Canvas details
		  */
		void SetYaxisTitle(TString p_title);
		void SetUpperPlotLimits(double p_ymin, double p_ymax);
		void SetBottomPlotLimits(double p_ymin, double p_ymax);


		/*** Public Members ***/
		// Histograms and related bookkeeping
		public:
			//TH1D *h_reference;
			//TString m_reference_legend_entry;

			// Formatting aspects
			// Labels
			TString y_axis_title;

		/*** Private Members ***/
		private:
			double m_top_ymin;
			double m_top_ymax;
			bool m_top_customlimits;
			double m_bottom_ymin;
			double m_bottom_ymax;
			bool m_bottom_customlimits;

	};

}

#endif