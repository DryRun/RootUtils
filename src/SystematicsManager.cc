#ifndef SystematicsManager_cxx
#define SystematicsManager_cxx

#include "MyTools/RootUtils/interface/SystematicsManager.h"

namespace Root {
	SystematicsManager::SystematicsManager() {
		m_adding_process_ = false;
		m_first_process_ = true;
		m_systematics_available_ = false;
	}

	SystematicsManager::~SystematicsManager() {
		m_systematics_containers_.clear();
		if (m_uncertainty_tgraph_) {
			delete m_uncertainty_tgraph_;
		}
		if (m_fractional_uncertainty_tgraph_) {
			delete m_fractional_uncertainty_tgraph_;
		}
		m_singleprocess_uncertainty_tgraph_.clear();
		m_singleprocess_fractional_uncertainty_tgraph_.clear();
		m_singlesystematic_uncertainty_tgraph_.clear();
		m_singlesystematic_fractional_uncertainty_tgraph_.clear();
	}

	void SystematicsManager::StartNewProcess(const TString p_process_name, TH1D *p_hist_central) {
		if (m_first_process_) {
			m_first_process_ = false;
			m_reference_histogram_ = p_hist_central;
		}
		if (std::find(m_process_list_.begin(), m_process_list_.end(), p_process_name) != m_process_list_.end()) {
			std::cerr << "[SystematicsManager] ERROR : Attempt to add process " << p_process_name << ", which already exists." << std::endl;
			exit(1);
		} else {
			m_process_list_.push_back(p_process_name);
		}

		m_adding_process_ = true;
		m_systematics_containers_[p_process_name] = new Root::SystematicsContainer();
		m_systematics_containers_[p_process_name]->AddCentralHistogram(p_hist_central);
	}

	void SystematicsManager::AddProcessNormalizationUncertainty(const TString p_process_name, const TString p_systematic_name, double p_value) {
		if (std::find(m_systematics_list_.begin(), m_systematics_list_.end(), p_systematic_name) == m_systematics_list_.end()) {
			m_systematics_list_.push_back(p_systematic_name);
		}
		try {
			m_systematics_containers_[p_process_name]->AddNormalizationUncertainty(p_systematic_name, p_value);
		} catch (std::string err_msg) {
			std::cout << "[SystematicsManager] ERROR : In AddProcessNormalizationUncertainty, failed to add uncertainty to SystematicsContainer: " << err_msg << std::endl;
			exit(1);
		}
	}

	void SystematicsManager::AddProcessNormalizationUncertainty(const TString p_process_name, const TString p_systematic_name, double p_value_down, double p_value_up) {
		if (std::find(m_systematics_list_.begin(), m_systematics_list_.end(), p_systematic_name) == m_systematics_list_.end()) {
			m_systematics_list_.push_back(p_systematic_name);
		}
		try {
			m_systematics_containers_[p_process_name]->AddNormalizationUncertainty(p_systematic_name, p_value_down, p_value_up);
		} catch (std::string err_msg) {
			std::cout << "[SystematicsManager] ERROR : In AddProcessNormalizationUncertainty, failed to add uncertainty to SystematicsContainer: " << err_msg << std::endl;
			exit(1);
		}
	}

	void SystematicsManager::AddProcessShapeUncertainty(const TString p_process_name, const TString p_systematic_name, TH1D *p_hist_down, TH1D *p_hist_up) {
		if (std::find(m_systematics_list_.begin(), m_systematics_list_.end(), p_systematic_name) == m_systematics_list_.end()) {
			m_systematics_list_.push_back(p_systematic_name);
		}
		try {
			m_systematics_containers_[p_process_name]->AddShapeUncertainty(p_systematic_name, p_hist_down, p_hist_up);
		} catch (std::string err_msg) {
			std::cout << "[SystematicsManager] ERROR : In AddProcessShapeUncertainty, failed to add uncertainty to SystematicsContainer: " << err_msg << std::endl;
			exit(1);
		}
	}

	void SystematicsManager::AddProcessShapeUncertainty(const TString p_process_name, const TString p_systematic_name, TH1D *p_hist) {
		if (std::find(m_systematics_list_.begin(), m_systematics_list_.end(), p_systematic_name) == m_systematics_list_.end()) {
			m_systematics_list_.push_back(p_systematic_name);
		}
		try {
			m_systematics_containers_[p_process_name]->AddShapeUncertainty(p_systematic_name, p_hist);
		} catch (std::string err_msg) {
			std::cout << "[SystematicsManager] ERROR : In AddProcessShapeUncertainty, failed to add uncertainty to SystematicsContainer: " << err_msg << std::endl;
			exit(1);
		}
	}

	void SystematicsManager::FinalizeNewProcess(const TString p_process_name) {
		if (m_systematics_containers_.find(p_process_name) == m_systematics_containers_.end()) {
			std::cerr << "[SystematicsManager] ERROR : Attempt to call FinalizeNewProcess on unknown process " << p_process_name << std::endl;
			exit(1);
		}
		m_process_integrals_[p_process_name] = m_systematics_containers_[p_process_name]->GetIntegral();
		m_process_integral_uncertainties_[p_process_name] = m_systematics_containers_[p_process_name]->GetIntegralSystematicUncertainty();
		m_adding_process_ = false;
	}

	void SystematicsManager::ComputeSystematics() {
		std::cout << std::endl << "[SystematicsManager] INFO : *** Computing systematics ***" << std::endl;
		for (std::vector<TString>::iterator it = m_systematics_list_.begin(); it != m_systematics_list_.end(); ++it) {
			std::cout << "[SystematicsManager] INFO : \tSystematic " << *it << std::endl;
		}
		m_systematics_available_ = true;

		// Normalizations, total uncertainty, single process uncertainties
		m_total_integral_ = 0.;
		m_total_integral_uncertainty_.first = 0.;
		m_total_integral_uncertainty_.second = 0.;
		for (std::vector<TString>::iterator it = m_process_list_.begin(); it != m_process_list_.end(); ++it) {
			m_total_integral_ += m_systematics_containers_[*it]->GetIntegral();
			m_total_integral_uncertainty_.first += TMath::Power(m_systematics_containers_[*it]->GetIntegralSystematicUncertainty().first, 2);
			m_total_integral_uncertainty_.second += TMath::Power(m_systematics_containers_[*it]->GetIntegralSystematicUncertainty().second, 2);
		
			m_process_integrals_[*it] = m_systematics_containers_[*it]->GetIntegral();
			m_process_integral_uncertainties_[*it] = m_systematics_containers_[*it]->GetIntegralSystematicUncertainty();
		}
		m_total_integral_uncertainty_.first = TMath::Sqrt(m_total_integral_uncertainty_.first);
		m_total_integral_uncertainty_.second = TMath::Sqrt(m_total_integral_uncertainty_.second);

		// Total uncertainty due to single systematics
		for (std::vector<TString>::iterator it_syst = m_systematics_list_.begin(); it_syst != m_systematics_list_.end(); ++it_syst) {
			m_single_systematic_integral_uncertainties_[*it_syst] = std::make_pair<double, double>(0., 0.);
			for (std::vector<TString>::iterator it_process = m_process_list_.begin(); it_process != m_process_list_.end(); ++it_process) {
				if (m_systematics_containers_[*it_process]->HasSystematic(*it_syst)) {
					m_single_systematic_integral_uncertainties_[*it_syst].first += m_systematics_containers_[*it_process]->GetSingleSystematicIntegralSystematicUncertainty(*it_syst).first;
					m_single_systematic_integral_uncertainties_[*it_syst].second += m_systematics_containers_[*it_process]->GetSingleSystematicIntegralSystematicUncertainty(*it_syst).second;
				}
			}
		}
		m_single_systematic_integral_uncertainties_["MCStats"] = std::make_pair<double, double>(0., 0.);
		for (std::vector<TString>::iterator it_process = m_process_list_.begin(); it_process != m_process_list_.end(); ++it_process) {
			m_single_systematic_integral_uncertainties_["MCStats"].first += m_systematics_containers_[*it_process]->GetSingleSystematicIntegralSystematicUncertainty("MCStats").first;
			m_single_systematic_integral_uncertainties_["MCStats"].second += m_systematics_containers_[*it_process]->GetSingleSystematicIntegralSystematicUncertainty("MCStats").second;
		}

		// Uncertainty broken down by process and systematic
		for (std::vector<TString>::iterator it_process = m_process_list_.begin(); it_process != m_process_list_.end(); ++it_process) {
			for (std::vector<TString>::iterator it_syst = m_systematics_list_.begin(); it_syst != m_systematics_list_.end(); ++it_syst) {
				m_process_single_systematic_integral_uncertainties_[*it_process][*it_syst] = m_systematics_containers_[*it_process]->GetSingleSystematicIntegralSystematicUncertainty(*it_syst);
			}
			m_process_single_systematic_integral_uncertainties_[*it_process]["MCStats"] = m_systematics_containers_[*it_process]->GetSingleSystematicIntegralSystematicUncertainty("MCStats");

		}

		// TGraphs
		// Total uncertainty
		m_uncertainty_tgraph_ = new TGraphAsymmErrors(m_reference_histogram_->GetNbinsX());
		m_fractional_uncertainty_tgraph_ = new TGraphAsymmErrors(m_reference_histogram_->GetNbinsX());
		std::vector<TString> m_systematics_list_plus_mcstats_ = m_systematics_list_;
		m_systematics_list_plus_mcstats_.push_back("MCStats");
		for (int point = 0; point < m_reference_histogram_->GetNbinsX(); ++point) {
			int bin = point + 1;
			double x = m_reference_histogram_->GetXaxis()->GetBinCenter(bin);
			double ex = 0.5 * m_reference_histogram_->GetXaxis()->GetBinWidth(bin);
			
			// Central value
			double y = 0.;
			for (std::map<TString, SystematicsContainer*>::iterator it = m_systematics_containers_.begin(); it != m_systematics_containers_.end(); ++it) {
				if ((*it).second->GetCentralHistogram()->GetBinContent(bin) > 0) {
					y += (*it).second->GetCentralHistogram()->GetBinContent(bin);
				}
			}

			// Error
			double eylow2 = 0.;
			double eyhigh2 = 0.;
			for (std::vector<TString>::iterator it_syst = m_systematics_list_plus_mcstats_.begin(); it_syst != m_systematics_list_plus_mcstats_.end(); ++it_syst) {
				// MCStats should be added quadratically
				if ((*it_syst).EqualTo("MCStats")) {
					for (std::map<TString, SystematicsContainer*>::iterator it = m_systematics_containers_.begin(); it != m_systematics_containers_.end(); ++it) {
						eylow2 += TMath::Power((*it).second->GetSingleSystematicPointUncertainty(point,*it_syst).first, 2);
						eyhigh2 += TMath::Power((*it).second->GetSingleSystematicPointUncertainty(point,*it_syst).second, 2);
					}
				} else {
					// For all other systematics, add everything up linearly before adding in quadrature to the total.
					double this_eylow = 0.;
					double this_eyhigh = 0.;

					for (std::map<TString, SystematicsContainer*>::iterator it = m_systematics_containers_.begin(); it != m_systematics_containers_.end(); ++it) {
						if ((*it).second->HasSystematic(*it_syst)) {
							this_eylow += (*it).second->GetSingleSystematicPointUncertainty(point,*it_syst).first;
							this_eyhigh += (*it).second->GetSingleSystematicPointUncertainty(point,*it_syst).second;
						}
					}
					eylow2 += TMath::Power(this_eylow, 2);
					eyhigh2 += TMath::Power(this_eyhigh, 2);
				}

			}
			double eylow = TMath::Sqrt(eylow2);
			double eyhigh = TMath::Sqrt(eyhigh2);
			m_uncertainty_tgraph_->SetPoint(point, x, y);
			m_uncertainty_tgraph_->SetPointError(point, ex, ex, eylow, eyhigh);

			m_fractional_uncertainty_tgraph_->SetPoint(point, x, 0.);
			if (y != 0) {
				m_fractional_uncertainty_tgraph_->SetPointError(point, ex, ex, eylow / y, eyhigh / y);
			} else {
				m_fractional_uncertainty_tgraph_->SetPointError(point, ex, ex, 0., 0.);
			}
		}

		// Single process
		for (std::map<TString, SystematicsContainer*>::iterator it = m_systematics_containers_.begin(); it != m_systematics_containers_.end(); ++it) {
			m_singleprocess_uncertainty_tgraph_[(*it).first] = new TGraphAsymmErrors(m_reference_histogram_->GetNbinsX());
			m_singleprocess_fractional_uncertainty_tgraph_[(*it).first] = new TGraphAsymmErrors(m_reference_histogram_->GetNbinsX());
			for (int point = 0; point < m_reference_histogram_->GetNbinsX(); ++point) {
				int bin = point + 1;
				double x = m_reference_histogram_->GetXaxis()->GetBinCenter(bin);
				double ex = 0.5 * m_reference_histogram_->GetXaxis()->GetBinWidth(bin);
				
				double y = (*it).second->GetCentralHistogram()->GetBinContent(bin);;
				double eylow = (*it).second->GetPointUncertainty(point).first;
				double eyhigh = (*it).second->GetPointUncertainty(point).second;
				m_singleprocess_uncertainty_tgraph_[(*it).first]->SetPoint(point, x, y);
				m_singleprocess_uncertainty_tgraph_[(*it).first]->SetPointError(point, ex, ex, eylow, eyhigh);

				m_singleprocess_fractional_uncertainty_tgraph_[(*it).first]->SetPoint(point, x, 0.);
				if (y != 0) {
					m_singleprocess_fractional_uncertainty_tgraph_[(*it).first]->SetPointError(point, ex, ex, eylow / y, eyhigh / y);
				} else {
					m_singleprocess_fractional_uncertainty_tgraph_[(*it).first]->SetPointError(point, ex, ex, 0., 0.);
				}
			}
		}

		// Single systematic
		for (std::vector<TString>::iterator it_syst = m_systematics_list_plus_mcstats_.begin(); it_syst != m_systematics_list_plus_mcstats_.end(); ++it_syst) {
			//std::cout << "[SystematicsManager] DEBUG : Making single systematic TGraph for systematic " << *it_syst << std::endl;

			m_singlesystematic_uncertainty_tgraph_[*it_syst] = new TGraphAsymmErrors(m_reference_histogram_->GetNbinsX());
			m_singlesystematic_fractional_uncertainty_tgraph_[*it_syst] = new TGraphAsymmErrors(m_reference_histogram_->GetNbinsX());
			for (int point = 0; point < m_reference_histogram_->GetNbinsX(); ++point) {
				int bin = point + 1;
				double x = m_reference_histogram_->GetBinContent(bin);
				double ex = 0.5 * m_reference_histogram_->GetXaxis()->GetBinWidth(bin);

				double y = 0.;
				double eylow = 0.;
				double eyhigh = 0.;
				for (std::vector<TString>::iterator it_process = m_process_list_.begin(); it_process != m_process_list_.end(); ++it_process) {
					if (m_systematics_containers_[*it_process]->GetCentralHistogram()->GetBinContent(bin) > 0) {
						y += m_systematics_containers_[*it_process]->GetCentralHistogram()->GetBinContent(bin);
					}
					if (m_systematics_containers_[*it_process]->HasSystematic(*it_syst)) {
						//std::cout << "[SystematicsManager] DEBUG : Process " << *it_process << " contributes " << m_systematics_containers_[*it_process]->GetCentralHistogram()->GetBinContent(bin) << " +" << m_systematics_containers_[*it_process]->GetSingleSystematicTGraph(*it_syst)->GetEYhigh()[point] << " -" << m_systematics_containers_[*it_process]->GetSingleSystematicTGraph(*it_syst)->GetEYlow()[point] << std::endl;
						if ((*it_syst).EqualTo("MCStats")) {
							eylow += TMath::Power(m_systematics_containers_[*it_process]->GetSingleSystematicTGraph(*it_syst)->GetEYlow()[point], 2);
							eyhigh +=TMath::Power( m_systematics_containers_[*it_process]->GetSingleSystematicTGraph(*it_syst)->GetEYhigh()[point], 2);
						} else {
							eylow += m_systematics_containers_[*it_process]->GetSingleSystematicTGraph(*it_syst)->GetEYlow()[point];
							eyhigh += m_systematics_containers_[*it_process]->GetSingleSystematicTGraph(*it_syst)->GetEYhigh()[point];
						}
					}
				}
				if ((*it_syst).EqualTo("MCStats")) {
					eylow = TMath::Sqrt(eylow);
					eyhigh = TMath::Sqrt(eyhigh);
				}

				m_singlesystematic_uncertainty_tgraph_[*it_syst]->SetPoint(point, x, y);
				m_singlesystematic_uncertainty_tgraph_[*it_syst]->SetPointError(point, ex, ex, eylow, eyhigh);
				m_singlesystematic_fractional_uncertainty_tgraph_[*it_syst]->SetPoint(point, x, 0.);
				if (y != 0) {
					m_singlesystematic_fractional_uncertainty_tgraph_[*it_syst]->SetPointError(point, ex, ex, TMath::Abs(eylow / y), TMath::Abs(eyhigh / y));
					//std::cout << "[SystematicsManager] DEBUG : For systematic " << *it_syst << ", setting fractional error up = " << TMath::Abs(eyhigh / y) << std::endl;
				} else {
					m_singlesystematic_fractional_uncertainty_tgraph_[*it_syst]->SetPointError(point, ex, ex, 0., 0.);
				}
			}
		}
	}

	void SystematicsManager::PrintSummary() {
		std::cout << "[SystematicsManager] INFO : Printing systematics summary" << std::endl;
		std::cout << "[SystematicsManager] INFO : Total integral = " << m_total_integral_ << " -" << m_total_integral_uncertainty_.first << " +" << m_total_integral_uncertainty_.second << std::endl;
		for (std::vector<TString>::iterator it_syst = m_systematics_list_.begin(); it_syst != m_systematics_list_.end(); ++it_syst) {
			std::cout << "[SystematicsManager] INFO : \tUncertainty " << *it_syst << " = -" << m_single_systematic_integral_uncertainties_[*it_syst].first << " +" << m_single_systematic_integral_uncertainties_[*it_syst].second << std::endl;
		}

		std::cout << std::endl;
		for (std::vector<TString>::iterator it_process = m_process_list_.begin(); it_process != m_process_list_.end(); ++it_process) {
			std::cout << std::endl << "[SystematicsManager] INFO : \tProcess " << (*it_process) << " integral = " << m_process_integrals_[*it_process] << " -" << m_process_integral_uncertainties_[*it_process].first << " +" << m_process_integral_uncertainties_[*it_process].second << std::endl;
			if (m_process_integrals_[*it_process] != 0) {
				for (std::vector<TString>::iterator it_syst = m_systematics_list_.begin(); it_syst != m_systematics_list_.end(); ++it_syst) {
					if (m_process_single_systematic_integral_uncertainties_[*it_process][*it_syst].first != 0 || m_process_single_systematic_integral_uncertainties_[*it_process][*it_syst].second != 0) {
						std::cout << "[SystematicsManager] INFO : \t\tUncertainty " << *it_syst << " = -" << m_process_single_systematic_integral_uncertainties_[*it_process][*it_syst].first << " +" << m_process_single_systematic_integral_uncertainties_[*it_process][*it_syst].second << std::endl;
		
					}
				}
			}
		}
	}
}

#endif