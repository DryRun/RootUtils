#ifndef SignalCanvas_h
#define SignalCanvas_h

#include "RootUtils/DryCanvas.h"
#include "RootUtils/DryLegend.h"

namespace Root {

	class SignalCanvas : public DryCanvas {
		/**
		  * Class SignalCanvas
		  * - Intelligently plot signal hypotheses on top of background predictions.
		  * - Options to scale the signal. In the future, consider adding functionality to compute a signal significance. 
		  */

	    ClassDef(SignalCanvas, 1);

	    /**
	     * Container for organizing and sorting histograms by integral. 
	     * This is important for log plots, where otherwise the small histograms will be invisible.
	     */
	    struct SignalCanvasHistogramSortingContainer {
	    	TH1D *hist;
	    	TString process;
	    	TString legend_entry;
	    	double luminosity;
	    	double manual_scale_factor;
	    };


	    /*** Public Methods ***/
	public:
		SignalCanvas();
		SignalCanvas(const char* name, const char* title, Int_t ww, Int_t wh);
		~SignalCanvas();

		/**
		 * Add a signal histogram to the canvas
		 * @param p_hist         Histogram (TH1D)
		 * @param p_process      Process name. Not really used except internally for bookkeeping
		 * @param p_legend_entry Legend entry
		 * @param p_luminosity   Luminosity
		 * @param p_scale_factor Optional scale factor to make the histogram visible.
		 */
		void AddSignalHistogram(TH1D *p_hist, TString p_process, TString p_legend_entry, double p_luminosity, double p_scale_factor = 1.);


		/**
		 * Add a background histogram to the canvas
		 * @param p_hist         Histogram (TH1D)
		 * @param p_process      Process name. Not really used except internally for bookkeeping.
		 * @param p_legend_entry Legend entry
		 * @param p_luminosity   Luminosity
		 */
		void AddBackgroundHistogram(TH1D *p_hist, TString p_process, TString p_legend_entry, double p_luminosity);


		// - Stuff for customizing the canvas appearance
		void SetYaxisRange(double p_ymin, double p_ymax);
		void SetYaxisTitle(TString p_title);
		void SetLogy(bool p_logy = true);

		/**
		 * Set a fixed luminosity for normalizing all of the histograms. Default is 20.3 fb^-1. 
		 * @param p_luminosity Luminosity
		 */
		void SetLuminosity(double p_luminosity);


		/**
		  * MakeCanvas
		  * - Normalize all the histograms to data luminosity, make the THStack, and draw.
		  */
		void MakeCanvas(); 


		/**
		 * Accessor to legend
		 * @return [description]
		 */
		inline Root::DryLegend* GetLegend() {
			return m_legend_;
		}

		inline void DrawMCUncertainty(bool p_do) {
			m_draw_mc_uncertainty_ = p_do;
		}

		/*** Private Methods ***/
	private:


		/*** Public Members ***/
	public:


		/*** Private Members ***/
	private:
		std::map<TString, SignalCanvasHistogramSortingContainer*> m_signal_histograms_;
		std::map<TString, SignalCanvasHistogramSortingContainer*> m_background_histograms_;
		std::vector<SignalCanvasHistogramSortingContainer*> m_signal_histograms_list_;
		std::vector<SignalCanvasHistogramSortingContainer*> m_background_histograms_list_;
		//THStack* ths_backgrounds_;
		//TH1D* m_hist_total_background_;
		bool m_logy_;
		bool m_yaxis_custom_limits_;
		double m_ymin_;
		double m_ymax_;
		TString m_yaxis_title_;
		TH1D *m_hist_frame_;
		double m_luminosity_;
		Root::DryLegend* m_legend_;
		bool m_draw_mc_uncertainty_;
		THStack* m_ths_backgrounds_;
		TH1D* m_hist_total_background_;
	};


}



#endif