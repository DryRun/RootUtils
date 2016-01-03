#ifndef SystematicsContainer_h
#define SystematicsContainer_h

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>

#include <TH1.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TH2D.h>
#include <TH3F.h>
#include <TH3D.h>
#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <TMath.h>

#include "EventLoop/Worker.h"

namespace Root {

	class SystematicsContainer {
		/*** Public Methods ***/
	public:
		/**
		 * Class SystematicsContainer
		 * - Manages a histogram (with statistical uncertainties) and a set of systematic uncertainties, represented either as "global" normalization uncertainties, or "shape" uncertainties.
		 * - Usage: load the central histogram first with AddCentralHistogram, and then the systematics with the appropriate functions. 
		 * - Return options:
		 * 		- TGraphAsymmErrors with central values taken from the central histogram, and y-error bars representing the total uncertainty.
		 * 		- Integral and total uncertainty.
		 * - A note on overflow bins: this procedure completely ignores overflow. If you want those represented, do it yourself before loading the histograms!
		 */
		SystematicsContainer();
		~SystematicsContainer();
		/**
		 * Add the central values in histogram form. Bin uncertainties should be statistical only.
		 * @param p_hist [description]
		 */
		void AddCentralHistogram(TH1D *p_hist);

		/**
		 * Add a symmetric normalization uncertainty.
		 * @param p_name  [description]
		 * @param p_normalization_uncertainty [description]
		 */
		void AddNormalizationUncertainty(TString p_name, double p_normalization_uncertainty);

		/**
		 *  Add an asymmetric normalization uncertainty.
		 * @param p_name       [description]
		 * @param p_normalization_uncertainty_up   [description]
		 * @param p_normalization_uncertainty_down [description]
		 */
		void AddNormalizationUncertainty(TString p_name, double p_normalization_uncertainty_down, double p_normalization_uncertainty_up);

		/**
		 * Add a shape uncertainty.
		 * @param p_name      [description]
		 * @param p_hist_up   [description]
		 * @param p_hist_down [description]
		 */
		void AddShapeUncertainty(TString p_name, TH1D *p_hist_down, TH1D *p_hist_up);

		/**
		 * Add a symmetric shape uncertainty.
		 * @param p_name      [description]
		 * @param p_hist   [description]
		 */
		void AddShapeUncertainty(TString p_name, TH1D *p_hist);

		inline TH1D* GetCentralHistogram() {
			return m_hist_central_;
		}

		inline TGraphAsymmErrors* GetTotalUncertaintyTGraph() {
			return m_tg_total_uncertainty_;
		}

		inline std::pair<double, double> GetBinUncertainty(int bin) {
			return std::make_pair<double, double>(double(m_tg_total_uncertainty_->GetEYlow()[bin-1]), double(m_tg_total_uncertainty_->GetEYhigh()[bin-1]));
		}

		inline std::pair<double, double> GetPointUncertainty(int point) {
			return std::make_pair<double, double>(double(m_tg_total_uncertainty_->GetEYlow()[point]), double(m_tg_total_uncertainty_->GetEYhigh()[point]));
		}

		inline std::pair<double, double> GetSingleSystematicPointUncertainty(int point, TString p_syst_name) {
			if (m_tg_single_systematics_.find(p_syst_name) == m_tg_single_systematics_.end()) {
				std::cerr << "[SystematicsContainer] ERROR : Didn't recognize systematic name " << p_syst_name << std::endl;
				exit(1);
			}
			return std::make_pair<double, double>(double(m_tg_single_systematics_[p_syst_name]->GetEYlow()[point]), double(m_tg_single_systematics_[p_syst_name]->GetEYhigh()[point]));
		}

		inline TGraphAsymmErrors* GetSingleSystematicTGraph(const TString p_syst_name) {
			return m_tg_single_systematics_[p_syst_name];
		}

		double GetIntegral();

		std::pair<double, double> GetIntegralSystematicUncertainty();

		std::pair<double, double> GetSingleSystematicIntegralSystematicUncertainty(TString p_syst);


		bool HasSystematic(TString p_syst_name);

		/*** Private Methods ***/
	private:
		/**
		 * Check whether two histograms have the same bins
		 * @param  p_hist1 [description]
		 * @param  p_hist2 [description]
		 * @return         [description]
		 */
		bool CheckHistogramCompatibility(TH1D* p_hist1, TH1D* p_hist2);

		TGraphAsymmErrors* ConvertHistogramToTGraph(TH1D* p_hist);

		/**
		 * Check if the systematic name has already been specified. Otherwise, add it to the list.
		 * @param  p_name Name of systematic
		 * @return        True if systematic name does not already exist (things are OK)
		 */
		bool CheckSystematicName(TString p_name);

		/*** Public Members ***/
	public:


		/*** Private Members ***/
	private:
		bool m_modified_;
		TH1D* m_hist_central_;		
		std::vector<TString> m_systematics_list_;
		std::map<TString, std::pair<double, double> > m_normalization_uncertainties_;
		std::map<TString, std::pair<TH1D*, TH1D*> > m_shape_uncertainties_;
		std::map<TString, TH1D*> m_symmetric_shape_uncertainties_;
		TGraphAsymmErrors* m_tg_total_uncertainty_;
		std::map<TString, TGraphAsymmErrors*> m_tg_single_systematics_;

	};


}

#endif