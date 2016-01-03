#ifndef SystematicsManager_h
#define SystematicsManager_h

#include <TObject.h>

#include "RootUtils/SystematicsContainer.h"
#include "RootUtils/CommonFunctions.h"

namespace Root {

	class SystematicsManager : public TObject {
	    ClassDef(SystematicsManager, 1);

		/*** Public Methods ***/
	public:
		SystematicsManager();

		~SystematicsManager();


		/**
		 * Create a new SystematicsContainer, and set a flag to indicate that the process is being modified.
		 * @param p_name Name of process, e.g. WZ
		 * @param p_hist_central Central value histogram
		 */
		void StartNewProcess(const TString p_process_name, TH1D *p_hist_central);

		/**
		 * Add a symmetric normalization uncertainty.
		 * @param p_process_name  Process name specified in StartNewProcess
		 * @param p_systematic_name Systematic name
		 * @param p_value [description]
		 */
		void AddProcessNormalizationUncertainty(const TString p_process_name, const TString p_systematic_name, double p_value);

		/**
		 *  Add an asymmetric normalization uncertainty.
		 * @param p_process_name       Process name specified in StartNewProcess
		 * @param p_systematic_name Systematic name
		 * @param p_value_up   [description]
		 * @param p_value_down [description]
		 */
		void AddProcessNormalizationUncertainty(const TString p_process_name, const TString p_systematic_name, double p_value_down, double p_value_up);

		/**
		 * Add a shape uncertainty.
		 * @param p_process_name      Process name specified in StartNewProcess
		 * @param p_systematic_name Systematic name
		 * @param p_hist_up   [description]
		 * @param p_hist_down [description]
		 */
		void AddProcessShapeUncertainty(const TString p_process_name, const TString p_systematic_name, TH1D *p_hist_down, TH1D *p_hist_up);

		/**
		 * Add a symmetric shape uncertainty.
		 * @param p_process_name      Process name specified in StartNewProcess
		 * @param p_systematic_name Systematic name
		 * @param p_hist   [description]
		 */
		void AddProcessShapeUncertainty(const TString p_process_name, const TString p_systematic_name, TH1D *p_hist);

		/**
		 * Wrap up the creating of a new process. Get the total uncertainty. Set modification flag to false.
		 * @param p_process_name [description]
		 * @param p_systematic_name Systematic name
		 */
		void FinalizeNewProcess(const TString p_process_name);

		/**
		 * Compute all the systematics stuff! 
		 */
		void ComputeSystematics();

		inline std::vector<TString> GetListOfSystematics() {
			return m_systematics_list_;
		}

		inline bool HasSystematic(TString p_syst) {
			return (std::find(m_systematics_list_.begin(), m_systematics_list_.end(), p_syst) != m_systematics_list_.end());
		}

		/**
		 * Get total integral (sum of integrals of central histograms)
		 * @return [description]
		 */
		inline const double GetIntegral() {
			if (!m_systematics_available_) {
				std::cerr << "[SystematicsManager] ERROR : GetIntegral() called before ComputeSystematics()" << std::endl;
				exit(1);
			}
			return m_total_integral_;
		}

		/**
		 * Get up and down total uncertainties on the integral central value
		 */
		inline const std::pair<double, double> GetIntegralTotalUncertainty() {
			if (!m_systematics_available_) {
				std::cerr << "[SystematicsManager] ERROR : GetIntegralTotalUncertainty() called before ComputeSystematics()" << std::endl;
				exit(1);
			}
			return m_total_integral_uncertainty_;
		}

		inline const double GetProcessIntegral(const TString p_process_name) {
			if (!m_systematics_available_) {
				std::cerr << "[SystematicsManager] ERROR : GetProcessIntegral(" << p_process_name << ") called before ComputeSystematics()" << std::endl;
				exit(1);
			}
			return m_process_integrals_[p_process_name];
		}

		inline const std::pair<double, double> GetProcessIntegralTotalUncertainty(const TString p_process_name) {
			if (!m_systematics_available_) {
				std::cerr << "[SystematicsManager] ERROR : GetProcessIntegralTotalUncertainty(" << p_process_name << ") called before ComputeSystematics()" << std::endl;
				exit(1);
			}
			return m_process_integral_uncertainties_[p_process_name];
		}

		inline const std::pair<double, double> GetIntegralSingleSystematicUncertainty(const TString p_syst) {
			if (!m_systematics_available_) {
				std::cerr << "[SystematicsManager] ERROR : GetProcessIntegralTotalUncertainty(" << p_syst << ") called before ComputeSystematics()" << std::endl;
				exit(1);
			}
			return m_single_systematic_integral_uncertainties_[p_syst];
		}

		inline const std::pair<double, double> GetProcessSingleSystematicIntegralTotalUncertainty(const TString p_process_name, const TString p_syst) {
			if (!m_systematics_available_) {
				std::cerr << "[SystematicsManager] ERROR : GetProcessSingleSystematicIntegralTotalUncertainty(" << p_process_name << ") called before ComputeSystematics()" << std::endl;
				exit(1);
			}
			return m_process_single_systematic_integral_uncertainties_[p_process_name][p_syst];
		}


		inline const TGraphAsymmErrors* GetTotalUncertaintyTGraph() {
			if (!m_systematics_available_) {
				std::cerr << "[SystematicsManager] ERROR : GetTotalUncertaintyTGraph() called before ComputeSystematics()" << std::endl;
				exit(1);
			}
			return m_uncertainty_tgraph_;
		}

		inline const TGraphAsymmErrors* GetFractionalUncertaintyTGraph() {
			if (!m_systematics_available_) {
				std::cerr << "[SystematicsManager] ERROR : GetFractionalUncertaintyTGraph() called before ComputeSystematics()" << std::endl;
				exit(1);
			}
			return m_fractional_uncertainty_tgraph_;
		}

		inline const TGraphAsymmErrors* GetProcessTotalUncertaintyTGraph(TString p_process_name) {
			if (!m_systematics_available_) {
				std::cerr << "[SystematicsManager] ERROR : GetProcessTotalUncertaintyTGraph(" << p_process_name << ") called before ComputeSystematics()" << std::endl;
				exit(1);
			}
			if (m_singleprocess_uncertainty_tgraph_.find(p_process_name) != m_singleprocess_uncertainty_tgraph_.end()) {
				return m_singleprocess_uncertainty_tgraph_[p_process_name];
			} else {
				std::cerr << "[SystematicsManager] ERROR : In GetProcessTotalUncertaintyTGraph, didn't recognize process name " << p_process_name << std::endl;
				exit(1);
			}
		}

		inline const TGraphAsymmErrors* GetProcessFractionalUncertaintyTGraph(TString p_process_name) {
			if (!m_systematics_available_) {
				std::cerr << "[SystematicsManager] ERROR : GetProcessFractionalUncertaintyTGraph(" << p_process_name << ") called before ComputeSystematics()" << std::endl;
				exit(1);
			}
			if (m_singleprocess_fractional_uncertainty_tgraph_.find(p_process_name) != m_singleprocess_fractional_uncertainty_tgraph_.end()) {
				return m_singleprocess_fractional_uncertainty_tgraph_[p_process_name];
			} else {
				std::cerr << "[SystematicsManager] ERROR : In GetProcessFractionalUncertaintyTGraph, didn't recognize process name " << p_process_name << std::endl;
				exit(1);
			}
		}

		inline const TGraphAsymmErrors* GetSingleSystematicUncertaintyTGraph(TString p_systematic_name) {
			if (!m_systematics_available_) {
				std::cerr << "[SystematicsManager] ERROR : GetSingleSystematicUncertaintyTGraph(" << p_systematic_name << ") called before ComputeSystematics()" << std::endl;
				exit(1);
			}
			if (m_singlesystematic_uncertainty_tgraph_.find(p_systematic_name) != m_singlesystematic_uncertainty_tgraph_.end()) {
				return m_singlesystematic_uncertainty_tgraph_[p_systematic_name];
			} else {
				std::cerr << "[SystematicsManager] ERROR : In GetSingleSystematicUncertaintyTGraph, didn't recognize process name " << p_systematic_name << std::endl;
				exit(1);
			}
		}

		inline const TGraphAsymmErrors* GetSingleSystematicFractionalUncertaintyTGraph(TString p_systematic_name) {
			if (!m_systematics_available_) {
				std::cerr << "[SystematicsManager] ERROR : GetSingleSystematicFractionalUncertaintyTGraph(" << p_systematic_name << ") called before ComputeSystematics()" << std::endl;
				exit(1);
			}
			if (m_singlesystematic_fractional_uncertainty_tgraph_.find(p_systematic_name) != m_singlesystematic_fractional_uncertainty_tgraph_.end()) {
				return m_singlesystematic_fractional_uncertainty_tgraph_[p_systematic_name];
			} else {
				std::cerr << "[SystematicsManager] WARNING : In GetSingleSystematicFractionalUncertaintyTGraph, didn't recognize systematic name " << p_systematic_name << ". Returning 0." << std::endl;
				return 0;
			}
		}
		
		/** Print a summary of all processes and systematic uncertainties */
		void PrintSummary();

		/*** Private Methods ***/
	private:


		/*** Public Members ***/
	public:


		/*** Private Members ***/
	private:
		std::map<TString, SystematicsContainer*> m_systematics_containers_;
		std::vector<TString> m_process_list_;
		std::vector<TString> m_systematics_list_;
		bool m_adding_process_;
		bool m_systematics_available_;

		// Reference histogram for ensuring consistency: keep track of the first histogram loaded
		TH1D* m_reference_histogram_;
		bool m_first_process_;
		
		// Return values
		double m_total_integral_;
		std::pair<double, double> m_total_integral_uncertainty_;
		std::map<TString, double> m_process_integrals_;
		std::map<TString, std::pair<double, double> > m_process_integral_uncertainties_;
		std::map<TString, std::pair<double, double> > m_single_systematic_integral_uncertainties_;
		std::map<TString, std::map<TString, std::pair<double, double> > > m_process_single_systematic_integral_uncertainties_; // Process : Systematic Name : Uncertainty

		TGraphAsymmErrors* m_uncertainty_tgraph_;
		TGraphAsymmErrors* m_fractional_uncertainty_tgraph_;
		std::map<TString, TGraphAsymmErrors*> m_singleprocess_uncertainty_tgraph_;
		std::map<TString, TGraphAsymmErrors*> m_singleprocess_fractional_uncertainty_tgraph_;
		std::map<TString, TGraphAsymmErrors*> m_singlesystematic_uncertainty_tgraph_;
		std::map<TString, TGraphAsymmErrors*> m_singlesystematic_fractional_uncertainty_tgraph_;


	};

}


#endif