#ifndef DataBackgroundCanvas_h
#define DataBackgroundCanvas_h

#include "MyTools/RootUtils/interface/DryCanvas.h"
#include "MyTools/RootUtils/interface/DryLegend.h"
#include "MyTools/RootUtils/interface/CommonFunctions.h"

namespace Root {

	class DataBackgroundCanvas : public DryCanvas {
		/**
		  * Class DataBackgroundCanvas
		  * - Intelligently plot signal hypotheses on top of background predictions.
		  * - Options to scale the signal. In the future, consider adding functionality to compute a signal significance. 
		  */

	    ClassDef(DataBackgroundCanvas, 1);

	    /*** Enums ***/
	public:
		enum RatioStyle_t {
			kDeviation, // (Data - Background) / Background
			kDevationPercent, // 100 * (Data - Background) / Background
			kRatio // Data / Background
		};

	    /**
	     * Container for organizing and sorting histograms by integral. 
	     * This is important for log plots, where otherwise the small histograms will be invisible.
	     */
	    struct DataBackgroundCanvasHistogramSortingContainer {
	    	TH1D *hist;
	    	TGraphAsymmErrors *graph;
	    	TString process;
	    	TString legend_entry;
	    	double luminosity;
	    	double manual_scale_factor;
	    };


	    /*** Public Methods ***/
	public:
		DataBackgroundCanvas();
		DataBackgroundCanvas(const char* name, const char* title, Int_t ww, Int_t wh);
		~DataBackgroundCanvas();


		/**
		 * Add a data histogram to the canvas
		 * @param p_hist         Histogram (TH1D)
		 * @param p_process      Unique identifier for the histogram
		 * @param p_legend_entry Legend entry, e.g. "Data 2012"
		 * @param p_luminosity   Luminosity of data histogram
		 * @param p_scale_factor Optional scale factor
		 */
		void AddDataHistogram(TH1D *p_hist, TString p_process, TString p_legend_entry, double p_luminosity, double p_scale_factor = 1.);

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
		 * Custom y-axis range for ratio plot
		 * @param p_ymin [description]
		 * @param p_ymax [description]
		 */
		inline void SetRatioRange(double p_ymin, double p_ymax) {
			m_ratio_custom_limits_ = true;
			m_ratio_min_ = p_ymin;
			m_ratio_max_ = p_ymax;
		}

		/**
		 * Set the scale factor for ymax. Default is x10 for log scale, and 1.5 for linear scale.
		 * @param p_scale Multiplies the default ymax.
		 */
		inline void ScaleYaxis(double p_scale) {
			m_y_scale_ = p_scale;
		}

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

		inline void DoRatioPlot(bool p_do) {
			m_draw_ratio_ = p_do;
		}

		double GetBackgroundIntegral();

		double GetBackgroundIntegralError();

		double GetDataIntegral(TString p_process);

		double GetDataIntegralError(TString p_process);

		inline void AddSystematicsBand(TGraphAsymmErrors* p_systematics_band) {
			m_manual_systematics_band_ = true;
			m_systematics_band_ = p_systematics_band;
		}

		inline void AddBinLabels(std::vector<TString> p_labels) {
			m_use_bin_labels_ = true;
			m_bin_labels_ = p_labels;
		}

		inline void NormalizeByBinWidth(double p_width) {
			m_normalize_by_bin_width_ = true;
			m_bin_width_normalization_ = p_width;
		}

		inline void SetStackSignal(bool p_do) {
			m_do_stack_signal_ = p_do;
		}

		inline void SetDataErrorStyle(ErrorStyle_t p_style) {
			m_data_error_style_ = p_style;
		}

		inline void SetRatioStyle(RatioStyle_t p_style) {
			m_ratio_style_ = p_style;
		}

		inline TPad* GetTopPad() {
			return m_top_;
		}

		inline TPad* GetBottomPad() {
			return m_bottom_;
		}

		inline void SetUnitNormalization(bool p_do) {
			m_unit_normalization_ = p_do;
		}

		/*** Private Methods ***/
	private:


		/*** Public Members ***/
	public:


		/*** Private Members ***/
	private:
		std::map<TString, DataBackgroundCanvasHistogramSortingContainer*> m_data_histograms_;
		std::map<TString, DataBackgroundCanvasHistogramSortingContainer*> m_signal_histograms_;
		std::map<TString, DataBackgroundCanvasHistogramSortingContainer*> m_signalstacked_histograms_;
		std::map<TString, DataBackgroundCanvasHistogramSortingContainer*> m_background_histograms_;
		std::map<TString, DataBackgroundCanvasHistogramSortingContainer*> m_ratio_histograms_;
		std::vector<DataBackgroundCanvasHistogramSortingContainer*> m_data_histograms_list_;
		std::vector<DataBackgroundCanvasHistogramSortingContainer*> m_signal_histograms_list_;
		std::vector<DataBackgroundCanvasHistogramSortingContainer*> m_signalstacked_histograms_list_;
		std::vector<DataBackgroundCanvasHistogramSortingContainer*> m_background_histograms_list_;
		//THStack* ths_backgrounds_;
		TH1D* m_hist_total_background_;
		TH1D* m_hist_total_background_syst_;
		bool m_logy_;
		bool m_yaxis_custom_limits_;
		double m_ymin_;
		double m_ymax_;
		TString m_yaxis_title_;
		TH1D *m_hist_frame_;
		TH1D *m_ratio_frame_;
		double m_luminosity_;
		Root::DryLegend* m_legend_;
		bool m_draw_mc_uncertainty_;
		TPad *m_top_;
		TPad *m_bottom_;
		bool m_draw_ratio_;
		THStack* m_ths_backgrounds_;
		TLine *m_line_unity_;
		TGraphAsymmErrors* m_systematics_band_;
		bool m_manual_systematics_band_;
		bool m_use_bin_labels_;
		std::vector<TString> m_bin_labels_;
		bool m_normalize_by_bin_width_;
		double m_bin_width_normalization_;
		bool m_do_stack_signal_;
		ErrorStyle_t m_data_error_style_;
		bool m_ratio_custom_limits_;
		double m_ratio_min_;
		double m_ratio_max_;
		RatioStyle_t m_ratio_style_;
		double m_y_scale_;
		bool m_unit_normalization_;
	};


}



#endif