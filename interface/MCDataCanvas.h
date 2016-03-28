#ifndef MCDataCanvas_h
#define MCDataCanvas_h

#include "MyTools/RootUtils/interface/DryCanvas.h"

#include "MyTools/RootUtils/interface/CommonFunctions.h"
#include "MyTools/RootUtils/interface/DryLegend.h"
#include "MyTools/RootUtils/interface/CanvasLabels.h"

namespace Root {

	class MCDataCanvas : public DryCanvas {
		/**
		  * Class MCDataCanvas
		  * - Intelligently plot multiple histograms on the same canvas.
		  * - Similar to FakeFactorCanvas, except no ratio plot.
		  */

	    ClassDef(MCDataCanvas, 1);

	    struct HistogramSortingContainer {
	    	TH1D *hist;
	    	TString process;
	    	TString legend_entry;
	    	double luminosity;
	    };

	    /*** Public Methods ***/
	public:
	    /**
	      * Constructor
	      * - Copies the TCanvas constructor
	      */
	    MCDataCanvas();
		MCDataCanvas(const char* name, const char* title, Int_t ww, Int_t wh);
		~MCDataCanvas();

		/**
		  * AddDataHistogram
		  * - Have to supply a histogram, a label for the legend, and a luminosity (nb^-1)
		  */
		void AddDataHistogram(TH1D *p_histogram, TString p_legend_entry, double p_luminosity);

		
		/**
		  * AddMCHistogram
		  * - Supply a histogram, a process name, and a cross section (nb)
		  */
		void AddMCHistogram(TH1D *p_histogram, TString p_process, TString p_legend_entry, double p_luminosity);


		/**
		  * SetRatioLimits
		  * - Manually set the y-axis limits for the ratio canvas
		  */
		void SetRatioLimits(double p_min, double p_max);


		/**
		  * MakeCanvas
		  * - Normalize all the histograms to data luminosity, make the THStack, and draw.
		  */
		void MakeCanvas(); 


		/**
		  * InvertRatio
		  * - Default behavior is Data / MC. This switch sets it to MC / Data.
		  */
		void InvertRatio(bool p_invert_ratio = true);

		/**
		  * Canvas details
		  */
		void SetYaxisTitle(TString p_title);
		void SetTopYaxisLimits(double p_min, double p_max);
		void ScaleTopYaxisLimits(double p_yaxis_top_scale_lower, double p_yaxis_top_scale_upper);
		void SetTopLogy(bool p_top_log);
		void SetBottomLogy(bool p_bottom_log);
		std::pair<double, double> GetTopYaxisLimits();

		/*** Public Members ***/
		// Histograms and related bookkeeping
	public:
		//TH1D *h_data;
		//TString m_data_name; // Index entry for the data histogram
		double m_data_luminosity;
		TString m_data_legend_entry;

		//std::map<TString, TH1D*> h_mc;
		//std::map<TString, TString> m_mc_legend_entries; 
		std::map<TString, double> m_mc_luminosities;
		std::vector<TString> m_mc_process_list;

	public:
		// Formatting aspects
		bool do_stacked;
		bool m_canvas_drawn;

		// Labels
		TString y_axis_title;

	/*** Private Members ***/
	private:
		TH1D *h_frame;
		TPad *c_top;
		TPad *c_bottom;
		bool m_manual_ratio_limits;
		double m_ratio_min;
		double m_ratio_max;
		bool m_invert_ratio;
		bool m_yaxis_customtitle;
		bool m_yaxis_top_customlimits;
		double m_top_ymin;
		double m_top_ymax;
		bool m_yaxis_top_scalelimits;
		double m_yaxis_top_scale_upper;
		double m_yaxis_top_scale_lower;
		bool m_top_logy;
		bool m_bottom_logy;

		std::vector<HistogramSortingContainer*> MCHistogramList;
		std::map<TString, HistogramSortingContainer*> MCHistogramMap;

	};

}

#endif