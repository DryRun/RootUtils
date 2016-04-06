#ifndef DistributionCanvas_h
#define DistributionCanvas_h

#include "MyTools/RootUtils/interface/DryCanvas.h"

namespace Root {

	class DistributionCanvas : public DryCanvas {
		/**
		  * Class DistributionCanvas
		  * - Intelligently plot multiple histograms on the same canvas.
		  * - Similar to FakeFactorCanvas, except no ratio plot.
		  */
   		ClassDef(DistributionCanvas, 1);

   		enum HistogramStyle {
   			Points,
   			Lines,
   			Fill
   		};

		/*** Public Methods ***/
	public:
		DistributionCanvas();
		DistributionCanvas(const char* name, const char* title, Int_t ww, Int_t wh);
		void MakeCanvas();
		void SetYaxis(double p_ymin, double p_ymax);
		void SetYaxisTitle(TString p_ytitle);
		void NormalizeHistograms(bool p_normalize_histograms);
		void SetLogX();
		void SetLogY();
		void DrawHistogramIntegrals();
		void AddHistogram(TH1D *p_histogram, TString p_name, TString p_legend_entry);
		void AddPaintStyle(HistogramStyle p_style);
		void RemovePaintStyle(HistogramStyle p_style);

		/*** Protected Methods ***/
	protected:

		/*** Private Methods ***/
	private:
		// Called by MakeCanvas to normalize the histograms (or not). 
		void DoNormalizeHistograms();

		/*** Public Members ***/
	public:
		bool do_stacked;

		/*** Protected Members ***/


		/*** Private Members ***/
	private:
		TString y_axis_title;

		bool m_normalize_histograms;
		bool m_log_x;
		bool m_log_y;
		bool m_draw_histogram_integrals;
		bool m_manual_y_axis_title;
		bool m_yaxis_customlimits;
		double m_ymin, m_ymax;

		std::map<TString, Float_t> histogram_integrals;

		std::map<HistogramStyle, bool> m_histogram_paint_styles;

	};

}

#endif