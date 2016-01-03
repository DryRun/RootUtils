#ifndef RatioCanvas_h
#define RatioCanvas_h

#include "RootUtils/DryCanvas.h"
#include <TLine.h>

namespace Root {

	class RatioCanvas : public DryCanvas {
		/**
		  * Class DistributionCanvas
		  * - Intelligently plot multiple histograms on the same canvas.
		  * - Similar to FakeFactorCanvas, except no ratio plot.
		  */

    	ClassDef(RatioCanvas, 1);


    	/*** Public Methods ***/
	public:
		RatioCanvas();

		/**
		  * Constructor
		  * - Same syntax as TCanvas
		  */
		RatioCanvas(const char* name, const char* title, Int_t ww, Int_t wh);


		/**
		  * Constructor
		  * - Adds a style options:
		  *		- Normal: two stacked panes with legend on top
		  *		- ThreePanes: two stacked panes with legend to the right
		  */

		RatioCanvas(const char* name, const char* title, Int_t ww, Int_t wh, TString style);

		~RatioCanvas();

		/**
		  * Method SetRebinFactor
		  * - Rebins all of the histograms.  
		  * - Call BEFORE adding any histograms!
		  */
		void SetRebinFactor(Int_t p_rebin_factor);


		/**
		  * Method SetNormalizationBinWidth
		  * - Sets a bin width for bin normalization
		  */
		void SetNormalizationBinWidth(double p_bin_width);

		/**
		  * Method AddHistograms
		  * - Add a pair of histograms to the list, record the legend entries, and make a ratio histogram.
		  */
		void AddHistograms(TString name, TH1D *h_num, TString leg_num, TH1D *h_den, TString leg_den);


		/**
		  * Method AddHistograms
		  * - Add a pair of histograms to the list, record the legend entries.
		  * - Manually specify h_ratio, in case the computation of the ratio is nontrivial.
		  */
		void AddHistograms(TString name, TH1D *h_num, TString leg_num, TH1D *h_den, TString leg_den, TH1D *h_ratio_in);


		/**
		  * Method AddHistograms
		  * - Calls AddHistorams(TString name, TH1D *h_num, TString leg_num, TH1D *h_den, TString leg_den)
		  *     with default legend entries, name + "N" and name + " D"
		  */
		void AddHistograms(TString name, TH1D *h_num, TH1D *h_den);

		/**
		 * Add a standalone histogram that doesn't contribute to any ratios.
		 * @param name         Histogram name
		 * @param h            Histogram (TH1D)
		 * @param legend_entry Legend entry
		 */
		void AddStandaloneHistogram(TString name, TH1D *h, TString legend_entry, TString style);

		/**
		  * Method SetCommonDenominator
		  * Sets a common denominator histogram.
		  */
		void SetCommonDenominator(TH1D *h_in, TString p_common_denominator_name);

		/**********************************************************************/
		// Canvas drawing functions
		/**********************************************************************/
		/**
		  * Method MakeCanvas
		  * - Draw the histograms. Call after adding histograms and customizing the plot style.
		  */
		void MakeCanvas();


		/*** Methods to customize the canvas ***/


		/**
		  * Method SetYaxisTitle
		  * - Sets the y-axis title for the top plot.
		  */
		void SetYaxisTitle(TString p_title);


		/**
		  * Method SetRatioTitle
		  * - Sets the y-axis title for the bottom plot.
		  */
		void SetRatioTitle(TString p_title);


		/**
		  * Method SetYaxisLimits
		  * - Manually set the top plot (N/D histograms) y-axis (default is inherited from the histograms)
		  */
		void SetYaxisLimits(double c_min, double c_max);


		/**
		  * Method SetRatioLimits
		  * - Manually set the bottom plot (ratio) y-axis (default is inherited from the histograms)
		  */
		void SetRatioLimits(double c_min, double c_max);


		/**
		  * FitRatio(TString fit)
		  * - Fit the ratio plot with simple functions.
		  * - Available functions:
		  * 	- "linear"
		  */
		void FitRatio(TString p_fit);

		inline TF1* GetFit(TString p_name) {
			if (m_ratio_fits_.find(p_name) != m_ratio_fits_.end()) {
				return m_ratio_fits_[p_name];
			} else {
				return 0;
			}
		}

		void SetTopLogy(bool p_top_logy);

		inline void DrawMarkers(bool p_do) {
			m_draw_markers_ = p_do;
		}


		/**********************************************************************/
		// Debug capabilities
		/**********************************************************************/
		/**
		  * Method SetDebug
		  * - 0 = Nothing
		  * - 1 = Print methods as they are called
		  * - 2 = Verbose
		  */
		void SetDebug(Int_t p_debug);

		/**********************************************************************/
		// Accessor methods
		/**********************************************************************/
		/**
		  * Method GetNumeratorHistogram
		  * - Get numerator histograms by name
		  * - Returns 0x0 if name is not recognized
		  */
		TH1D* GetNumeratorHistogram(TString name);

		/**
		  * Method GetDenominatorHistogram
		  * - Get denominator histograms by name
		  * - Returns 0x0 if name is not recognized
		  */
		TH1D* GetDenominatorHistogram(TString name);

		/**
		  * Method GetRatioHistogram
		  * - Get ratio histograms by name
		  * - Returns 0x0 if name is not recognized
		  */
		TH1D* GetRatioHistogram(TString name);

		/**
		  * GetNHistogramPairs
		  * - Get the number of histogram pairs added to the canvas.
		  */
		Int_t GetNHistogramPairs();

		inline TPad* GetTopPad() {
			return top;
		}

		inline TPad* GetBottomPad() {
			return bottom;
		}


    	/*** Protected Methods ***/


    	/*** Private Methods ***/
    	/**
    	  * CommonInit
    	  * - Called by all constructors to initialize some of the members.
    	  */
		void CommonInit();

    	/*** Public Members ***/


    	/*** Protected Members ***/


    	/*** Private Members ***/

	private:
		TPad *top;
		TPad *bottom;
		bool manual_ff_limits;
		double c_ffmax;
		double c_ffmin;
		double c_ymin;
		double c_ymax;
		Int_t rebin_factor;
		double normalization_bin_width; // For normalizing bin contents to bin width
		TString y_axis_title;
		TString ratio_title;
		TString plot_style;
		bool m_top_logy;
		bool m_manual_y_axis;
		bool m_draw_markers_;
		TLine *m_ratio_unity_;
		std::map<TString, TF1*> m_ratio_fits_;

		// Allow a common denominator
		bool m_use_common_denominator; // Set to true if there is a single, common denominator histogram.
		TH1D *h_common_denominator;
		TString m_common_denominator_name;

		// Histogram bookkeeping: do it in-house, rather than using the superclass, so we can do everything pair-wise.
		std::vector<TString> m_pair_definition_list;
		std::map<TString, std::pair<TH1D*, TH1D*> > h_object_counts;
		std::map<TString, std::pair<TString, TString> > legend_entries;
		std::map<TString, TH1D*> h_ratio;

		// Bookkeeping for standalone histograms
		std::vector<TString> m_standalone_histogram_names_;
		std::map<TString, TH1D*> m_standalone_histograms_;
		std::map<TString, TString> m_standalone_histogram_legends_;
		std::map<TString, TString> m_standalone_histogram_styles_;

		Int_t debug_level;


	};

}
#endif