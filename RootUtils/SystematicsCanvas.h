#ifndef SystematicsCanvas_h
#define SystematicsCanvas_h

#include <iostream>
#include <iomanip>
#include <limits>

#include "RootUtils/DryCanvas.h"

namespace Root {
	class SystematicsCanvas : public DryCanvas {
		/**
		  * Class SystematicsCanvas
		  * - Load reference histogram + variation histograms up and down.
		  */
   		ClassDef(SystematicsCanvas, 1);

		/*** Public Methods ***/
	public:
		SystematicsCanvas();
		SystematicsCanvas(const char* name, const char* title, Int_t ww, Int_t wh);

		void AddReferenceHistogram(TH1D *p_hist);

		void AddHistograms(TString p_systematic_name, TH1D *p_syst_up, TH1D *p_syst_down);

		void MakeCanvas();

		void SetTopYaxisTitle(TString p_ytitle);

		void SetBottomYaxisTitle(TString p_ytitle);

		void SetTopYaxisLimits(double p_ymin, double p_ymax);

		void SetBottomYaxisLimits(double p_ymin, double p_ymax);

		void SetLogx();

		void PrintLatex(TString p_path);

		/*** Private Methods ***/
	private:
		void CommonInit();
		void MakeSummaryTGraphs();


		/*** Private Members ***/
	private:
		TGraphAsymmErrors *tg_total_systematics;
		TGraphAsymmErrors *tg_total_uncertainty;
		TGraphAsymmErrors *tg_total_systematics_percentage;
		TGraphAsymmErrors *tg_total_uncertainty_percentage;

		TPad *top;
		TPad *bottom;

		Bool_t m_yaxis_top_customtitle;
		TString m_topytitle;
		Bool_t m_yaxis_bottom_customtitle;
		TString m_bottomytitle;
		Bool_t m_yaxis_top_customlimits;
		double p_top_ymin;
		double p_top_ymax;
		Bool_t m_yaxis_bottom_customlimits;
		double m_bottom_ymin;
		double m_bottom_ymax;

		Bool_t m_xaxis_logx;

		std::vector<TString> m_systematics_list;
	};
}










#endif