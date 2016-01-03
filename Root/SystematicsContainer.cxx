#ifndef SystematicsContainer_cxx
#define SystematicsContainer_cxx

#include "RootUtils/SystematicsContainer.h"

namespace Root {
	SystematicsContainer::SystematicsContainer() {
		m_hist_central_ = 0;
		m_tg_total_uncertainty_ = 0;
	}

	SystematicsContainer::~SystematicsContainer() {
		if (m_hist_central_) {
			delete m_hist_central_;
		}
		if (m_tg_total_uncertainty_) {
			delete m_tg_total_uncertainty_;
		}
		m_tg_single_systematics_.clear();
		m_symmetric_shape_uncertainties_.clear();
		m_shape_uncertainties_.clear();
	}

	TGraphAsymmErrors* SystematicsContainer::ConvertHistogramToTGraph(TH1D* p_hist) {
		TGraphAsymmErrors* tg = new TGraphAsymmErrors(p_hist->GetNbinsX());
		TString tgname = p_hist->GetName();
		tgname += "_tg";
		tg->SetName(tgname);
		for (int i = 1; i <= p_hist->GetNbinsX(); ++i) {
			tg->SetPoint(i - 1, p_hist->GetXaxis()->GetBinCenter(i), p_hist->GetBinContent(i));
			double bin_width = p_hist->GetXaxis()->GetBinWidth(i);
			tg->SetPointError(i - 1, bin_width / 2, bin_width / 2, p_hist->GetBinError(i), p_hist->GetBinError(i));
		}
		return tg;		
	}

	void SystematicsContainer::AddCentralHistogram(TH1D *p_hist) {
		m_hist_central_ = (TH1D*)p_hist->Clone();
		m_hist_central_->SetDirectory(0);

		// Convert to TGraph
		m_tg_total_uncertainty_ = ConvertHistogramToTGraph(m_hist_central_);

		// Save copy of single systematic
		m_tg_single_systematics_["MCStats"] = (TGraphAsymmErrors*)m_tg_total_uncertainty_->Clone();
		m_systematics_list_.push_back("MCStats");
	}

	void SystematicsContainer::AddNormalizationUncertainty(TString p_name, double p_normalization_uncertainty) {
		// Require central histogram to be loaded first
		if (!m_hist_central_) {
			throw std::runtime_error("Attempt to add systematic before central histogram is loaded.");
		}
		if (!CheckSystematicName(p_name)) {
			throw std::runtime_error("Reused normalization systematic name");
		}
		//std::cout << "[SystematicsContainer] DEBUG : Adding normalization uncertainty " << p_name << " = " << p_normalization_uncertainty << std::endl;
		m_normalization_uncertainties_[p_name] = std::make_pair<double, double>(double(p_normalization_uncertainty), double(p_normalization_uncertainty));

		// Add uncertainties to TGraphAsymmErrors
		for (int i = 0; i < m_tg_total_uncertainty_->GetN(); ++i) {
			double x_value = 0.;
			double y_value = 0.;
			m_tg_total_uncertainty_->GetPoint(i, x_value, y_value);
			double initial_fractional_uncertainty_up = 0.;
			double initial_fractional_uncertainty_down = 0.;
			if (y_value != 0.) {
				initial_fractional_uncertainty_up   = m_tg_total_uncertainty_->GetErrorYhigh(i) / y_value;
				initial_fractional_uncertainty_down = m_tg_total_uncertainty_->GetErrorYlow(i) / y_value;
			} 
			double final_fractional_uncertainty_up = TMath::Sqrt(TMath::Power(initial_fractional_uncertainty_up, 2) + TMath::Power(p_normalization_uncertainty, 2));
			double final_fractional_uncertainty_down = TMath::Sqrt(TMath::Power(initial_fractional_uncertainty_down, 2) + TMath::Power(p_normalization_uncertainty, 2));
			m_tg_total_uncertainty_->SetPointError(i, m_tg_total_uncertainty_->GetErrorXlow(i), m_tg_total_uncertainty_->GetErrorXhigh(i), final_fractional_uncertainty_down * y_value, final_fractional_uncertainty_up * y_value);
		}

		// Single systematic TGraph
		m_tg_single_systematics_[p_name] = ConvertHistogramToTGraph(m_hist_central_);
		for (int i = 0; i < m_tg_total_uncertainty_->GetN(); ++i) {
			double x_value = m_tg_total_uncertainty_->GetX()[i];
			double y_value = m_tg_total_uncertainty_->GetY()[i];
			//m_tg_total_uncertainty_->GetPoint(i, x_value, y_value);
			//std::cout << "[SystematicsContainer] DEBUG : Setting single-systematic for point " << i << " to " << y_value << "*" << p_normalization_uncertainty << " = " << y_value * p_normalization_uncertainty << std::endl;
			m_tg_single_systematics_[p_name]->SetPointError(i, m_tg_total_uncertainty_->GetErrorXlow(i), m_tg_total_uncertainty_->GetErrorXhigh(i), y_value * p_normalization_uncertainty, y_value * p_normalization_uncertainty);
		}
	}

	void SystematicsContainer::AddNormalizationUncertainty(TString p_name, double p_normalization_uncertainty_down, double p_normalization_uncertainty_up) {
		// Require central histogram to be loaded first
		if (!m_hist_central_) {
			throw std::runtime_error("Attempt to add systematic before central histogram is loaded.");
		}
		if (!CheckSystematicName(p_name)) {
			throw std::runtime_error("Reused normalization systematic name");
		}
		m_normalization_uncertainties_[p_name] = std::make_pair<double, double>(double(p_normalization_uncertainty_up), double(p_normalization_uncertainty_down));

		// Add uncertainties to TGraphAsymmErrors
		for (int i = 0; i < m_tg_total_uncertainty_->GetN(); ++i) {
			double x_value = 0.;
			double y_value = 0.;
			m_tg_total_uncertainty_->GetPoint(i, x_value, y_value);
			double initial_fractional_uncertainty_up = 0.;
			double initial_fractional_uncertainty_down = 0.;
			if (y_value != 0.) {
				initial_fractional_uncertainty_up = m_tg_total_uncertainty_->GetErrorYhigh(i) / y_value;
				initial_fractional_uncertainty_down = m_tg_total_uncertainty_->GetErrorYlow(i) / y_value;
			}
			double final_fractional_uncertainty_up = TMath::Sqrt(TMath::Power(initial_fractional_uncertainty_up, 2) + TMath::Power(p_normalization_uncertainty_up, 2));
			double final_fractional_uncertainty_down = TMath::Sqrt(TMath::Power(initial_fractional_uncertainty_down, 2) + TMath::Power(p_normalization_uncertainty_down, 2));
			m_tg_total_uncertainty_->SetPointError(i, m_tg_total_uncertainty_->GetErrorXlow(i), m_tg_total_uncertainty_->GetErrorXhigh(i), final_fractional_uncertainty_down * y_value, final_fractional_uncertainty_up * y_value);
		}

		// Single systematic TGraph
		m_tg_single_systematics_[p_name] = ConvertHistogramToTGraph(m_hist_central_);
		for (int i = 0; i < m_tg_total_uncertainty_->GetN(); ++i) {
			double x_value = 0.;
			double y_value = 0.;
			m_tg_total_uncertainty_->GetPoint(i, x_value, y_value);
			m_tg_single_systematics_[p_name]->SetPointError(i, m_tg_total_uncertainty_->GetErrorXlow(i), m_tg_total_uncertainty_->GetErrorXhigh(i), y_value * p_normalization_uncertainty_down, y_value * p_normalization_uncertainty_up);
		}
	}

	void SystematicsContainer::AddShapeUncertainty(TString p_name, TH1D *p_hist) {
		// Require central histogram to be loaded first
		if (!m_hist_central_) {
			throw std::runtime_error("Attempt to add systematic before central histogram is loaded.");
		}
		if (!CheckSystematicName(p_name)) {
			throw std::runtime_error("Reused shape systematic name");
		}
		if (!(CheckHistogramCompatibility(p_hist, m_hist_central_))) {
			std::cerr << "[SystematicsContainer] ERROR : Systematics histogram is not compatible with central histogram." << std::endl;
			std::cerr << "[SystematicsContainer] ERROR : Name = " << p_name << std::endl;
			std::cerr << "[SystematicsContainer] ERROR : Min x = " << p_hist->GetXaxis()->GetXmin() << std::endl;
			std::cerr << "[SystematicsContainer] ERROR : Max x = " << p_hist->GetXaxis()->GetXmax() << std::endl;
			std::cerr << "[SystematicsContainer] ERROR : Central min x = " << m_hist_central_->GetXaxis()->GetXmin() << std::endl;
			std::cerr << "[SystematicsContainer] ERROR : Central max x = " << m_hist_central_->GetXaxis()->GetXmax() << std::endl;
			throw std::runtime_error("Systematics histogram is not compatible with central histogram");
		}

		// Store histogram
		m_symmetric_shape_uncertainties_[p_name] = (TH1D*)p_hist->Clone();
		m_symmetric_shape_uncertainties_[p_name]->SetDirectory(0);

		// Add uncertainties to TGraphAsymmErrors
		for (int i = 0; i < m_tg_total_uncertainty_->GetN(); ++i) {
			double x_value = 0.;
			double y_value = 0.;
			m_tg_total_uncertainty_->GetPoint(i, x_value, y_value);
			double initial_uncertainty_up   = m_tg_total_uncertainty_->GetErrorYhigh(i);
			double initial_uncertainty_down = m_tg_total_uncertainty_->GetErrorYlow(i);
			double syst_uncertainty         = TMath::Abs(m_hist_central_->GetBinContent(i + 1) - p_hist->GetBinContent(i + 1));
			double final_uncertainty_up     = TMath::Sqrt(TMath::Power(initial_uncertainty_up, 2) + TMath::Power(syst_uncertainty, 2));
			double final_uncertainty_down   = TMath::Sqrt(TMath::Power(initial_uncertainty_down, 2) + TMath::Power(syst_uncertainty, 2));
			m_tg_total_uncertainty_->SetPointError(i, m_tg_total_uncertainty_->GetErrorXlow(i), m_tg_total_uncertainty_->GetErrorXhigh(i), final_uncertainty_down, final_uncertainty_up);
		}

		// Single systematic TGraph
		m_tg_single_systematics_[p_name] = ConvertHistogramToTGraph(m_hist_central_);
		for (int i = 0; i < m_tg_total_uncertainty_->GetN(); ++i) {
			m_tg_single_systematics_[p_name]->SetPointError(i, m_tg_total_uncertainty_->GetErrorXlow(i), m_tg_total_uncertainty_->GetErrorXhigh(i), TMath::Abs(p_hist->GetBinContent(i + 1) - m_hist_central_->GetBinContent(i + 1)), TMath::Abs(p_hist->GetBinContent(i + 1) - m_hist_central_->GetBinContent(i + 1)));
		}
	}

	void SystematicsContainer::AddShapeUncertainty(TString p_name, TH1D *p_hist_down, TH1D *p_hist_up) {
		// Require central histogram to be loaded first
		if (!m_hist_central_) {
			throw std::runtime_error("Attempt to add systematic before central histogram is loaded.");
			exit(1);
		}
		if (!CheckSystematicName(p_name)) {
			std::cout << "[SystematicsContainer] ERROR : Detected reuse of systematic name " << p_name << std::endl;
			throw std::runtime_error("Reused shape systematic name");
			exit(1);
		}
		if (!(CheckHistogramCompatibility(p_hist_up, m_hist_central_))) {
			std::cout << "[SystematicsContainer] ERROR : Detected histogram compatibility error with up histogram for systematic " << p_name << ". Hopefully the runtime exception reflects this!" << std::endl;
			throw std::runtime_error("Up systematics histogram is not compatible with central histogram.");
			exit(1);
		}
		if (!(CheckHistogramCompatibility(p_hist_down, m_hist_central_))) {
			std::cout << "[SystematicsContainer] ERROR : Detected histogram compatibility error with down histogram for systematic " << p_name << ". Hopefully the runtime exception reflects this!" << std::endl;
			throw std::runtime_error("Down systematics histogram is not compatible with central histogram.");
			exit(1);
		}

		// Store histograms
		m_shape_uncertainties_[p_name] = std::make_pair<TH1D*, TH1D*>((TH1D*)p_hist_down->Clone(), (TH1D*)p_hist_up->Clone());
		m_shape_uncertainties_[p_name].first->SetDirectory(0);
		m_shape_uncertainties_[p_name].second->SetDirectory(0);

		// Add uncertainties to TGraphAsymmErrors
		for (int i = 0; i < m_tg_total_uncertainty_->GetN(); ++i) {
			double x_value = 0.;
			double y_value = 0.;
			m_tg_total_uncertainty_->GetPoint(i, x_value, y_value);
			double initial_uncertainty_up   = m_tg_total_uncertainty_->GetErrorYhigh(i);
			double initial_uncertainty_down = m_tg_total_uncertainty_->GetErrorYlow(i);
			// Get the two possible systematic variations, and try to classify as up/down/both in the same direction
			double syst_uncertainty1         = p_hist_up->GetBinContent(i + 1) - m_hist_central_->GetBinContent(i + 1);
			double syst_uncertainty2         = p_hist_down->GetBinContent(i + 1) - m_hist_central_->GetBinContent(i + 1);
			double final_uncertainty_up = 0.;
			double final_uncertainty_down = 0.;
			if (syst_uncertainty1 >= 0. && syst_uncertainty2 <= 0.) {
				final_uncertainty_up     = TMath::Sqrt(TMath::Power(initial_uncertainty_up, 2) + TMath::Power(syst_uncertainty1, 2));
				final_uncertainty_down   = TMath::Sqrt(TMath::Power(initial_uncertainty_down, 2) + TMath::Power(syst_uncertainty2, 2));
			} else if (syst_uncertainty1 <= 0. && syst_uncertainty2 >= 0.) {
				final_uncertainty_up     = TMath::Sqrt(TMath::Power(initial_uncertainty_up, 2) + TMath::Power(syst_uncertainty2, 2));
				final_uncertainty_down   = TMath::Sqrt(TMath::Power(initial_uncertainty_down, 2) + TMath::Power(syst_uncertainty1, 2));
			} else if ((syst_uncertainty1 > 0. && syst_uncertainty2 > 0.) || (syst_uncertainty1 < 0. && syst_uncertainty2 < 0.)) {
				// Uncertainties in the same direction! ...Average and specify an asymmetric uncertainty?
				double syst_uncertainty3 = 0.5 * (syst_uncertainty1 + syst_uncertainty2);
				final_uncertainty_up     = TMath::Sqrt(TMath::Power(initial_uncertainty_up, 2) + TMath::Power(syst_uncertainty3, 2));
				final_uncertainty_down   = TMath::Sqrt(TMath::Power(initial_uncertainty_down, 2) + TMath::Power(syst_uncertainty3, 2));
			}
			m_tg_total_uncertainty_->SetPointError(i, m_tg_total_uncertainty_->GetErrorXlow(i), m_tg_total_uncertainty_->GetErrorXhigh(i), final_uncertainty_down, final_uncertainty_up);
		}

		// Single systematic TGraph
		m_tg_single_systematics_[p_name] = ConvertHistogramToTGraph(m_hist_central_);
		for (int i = 0; i < m_tg_total_uncertainty_->GetN(); ++i) {
			m_tg_single_systematics_[p_name]->SetPointError(i, m_tg_total_uncertainty_->GetErrorXlow(i), m_tg_total_uncertainty_->GetErrorXhigh(i), TMath::Abs(p_hist_down->GetBinContent(i + 1) - m_hist_central_->GetBinContent(i + 1)), TMath::Abs(p_hist_up->GetBinContent(i + 1) - m_hist_central_->GetBinContent(i + 1)));
		}

	}


	bool SystematicsContainer::CheckSystematicName(TString p_name) {
		if (std::find(m_systematics_list_.begin(), m_systematics_list_.end(), p_name) != m_systematics_list_.end()) {
			std::cerr << "[SystematicsContainer] ERROR : Attempt to add existing systematic: " << p_name << std::endl;
			return false;
		} else {
			m_systematics_list_.push_back(p_name);
			return true;
		}
	}

	double SystematicsContainer::GetIntegral() {
		if (!m_hist_central_) {
			std::cerr << "[SystematicsContainer] ERROR : In GetIntegral(), m_hist_central_ doesn't exist." << std::endl;
			exit(1);
		}
		return m_hist_central_->Integral(1, m_hist_central_->GetNbinsX());
	}

	std::pair<double, double> SystematicsContainer::GetIntegralSystematicUncertainty() {
		double integral_uncertainty_up2 = 0.;
		double integral_uncertainty_down2 = 0.;
		double integral_value = m_hist_central_->Integral(1, m_hist_central_->GetNbinsX());

		// Statistical uncertainty
		for (int bin = 1; bin <= m_hist_central_->GetNbinsX(); ++bin) {
			integral_uncertainty_up2 += TMath::Power(m_hist_central_->GetBinError(bin), 2);
			integral_uncertainty_down2 += TMath::Power(m_hist_central_->GetBinError(bin), 2);
		}

		// Normalization uncertainties
		for (std::map<TString, std::pair<double, double> >::iterator it = m_normalization_uncertainties_.begin(); it != m_normalization_uncertainties_.end(); ++it) {
			integral_uncertainty_down2 += TMath::Power(integral_value * (*it).second.first, 2);
			integral_uncertainty_up2 += TMath::Power(integral_value * (*it).second.second, 2);
		}

		// Shape uncertainties
		for (std::map<TString, std::pair<TH1D*, TH1D*> >::iterator it = m_shape_uncertainties_.begin(); it != m_shape_uncertainties_.end(); ++it) {
			double c_uncertainty_down = TMath::Abs((*it).second.first->Integral() - integral_value);
			double c_uncertainty_up = TMath::Abs((*it).second.second->Integral() - integral_value);
			integral_uncertainty_up2 += TMath::Power(c_uncertainty_up, 2);
			integral_uncertainty_down2 += TMath::Power(c_uncertainty_down, 2);
		}
		for (std::map<TString, TH1D*>::iterator it = m_symmetric_shape_uncertainties_.begin(); it != m_symmetric_shape_uncertainties_.end(); ++it) {
			double c_uncertainty = TMath::Abs((*it).second->Integral() - integral_value);
			integral_uncertainty_up2 += TMath::Power(c_uncertainty, 2);
			integral_uncertainty_down2 += TMath::Power(c_uncertainty, 2);
		}
		
		double integral_uncertainty_up = TMath::Sqrt(integral_uncertainty_up2);
		double integral_uncertainty_down = TMath::Sqrt(integral_uncertainty_down2);
		return std::make_pair<double, double>(double(integral_uncertainty_down), double(integral_uncertainty_up));
	}

	std::pair<double, double> SystematicsContainer::GetSingleSystematicIntegralSystematicUncertainty(TString p_syst) {
		std::pair<double, double> single_systematic_integral_uncertainty = std::make_pair<double, double>(0., 0.);
		if (HasSystematic(p_syst)) {
			double integral_uncertainty_up2 = 0.;
			double integral_uncertainty_down2 = 0.;
			double integral_value = m_hist_central_->Integral(1, m_hist_central_->GetNbinsX());
			double integral_uncertainty_up = 0.; 
			double integral_uncertainty_down = 0.; 

			if (p_syst.EqualTo("MCStats")) {
				for (int bin = 1; bin <= m_hist_central_->GetNbinsX(); ++bin) {
					integral_uncertainty_up2 += TMath::Power(m_hist_central_->GetBinError(bin), 2);
					integral_uncertainty_down2 += TMath::Power(m_hist_central_->GetBinError(bin), 2);
				}
				integral_uncertainty_up = TMath::Sqrt(integral_uncertainty_up2);
				integral_uncertainty_down = TMath::Sqrt(integral_uncertainty_down2);
			} else if (m_normalization_uncertainties_.find(p_syst) != m_normalization_uncertainties_.end()) {
				integral_uncertainty_down = integral_value * m_normalization_uncertainties_[p_syst].first;
				integral_uncertainty_up = integral_value * m_normalization_uncertainties_[p_syst].second;
			} else if (m_shape_uncertainties_.find(p_syst) != m_shape_uncertainties_.end()) {
				double c_uncertainty_down = m_shape_uncertainties_[p_syst].first->Integral() - integral_value;
				double c_uncertainty_up = m_shape_uncertainties_[p_syst].second->Integral() - integral_value;
				integral_uncertainty_up = c_uncertainty_up;
				integral_uncertainty_down = c_uncertainty_down;
			} else if (m_symmetric_shape_uncertainties_.find(p_syst) != m_symmetric_shape_uncertainties_.end()) {
				double c_uncertainty = TMath::Abs(m_symmetric_shape_uncertainties_[p_syst]->Integral() - integral_value);
				integral_uncertainty_up = c_uncertainty;
				integral_uncertainty_down = c_uncertainty;
			} else {
				std::cerr << "[SystematicsContainer] ERROR : I don't know what kind of uncertainty " << p_syst << " is! Debugging needed." << std::endl;
				exit(1);
			}

			single_systematic_integral_uncertainty = std::make_pair<double, double>(double(integral_uncertainty_down), double(integral_uncertainty_up));
		}
		return single_systematic_integral_uncertainty;
	}


	bool SystematicsContainer::CheckHistogramCompatibility(TH1D* p_hist1, TH1D* p_hist2) {
		bool consistent = true;
		if (p_hist1->GetNbinsX() != p_hist2->GetNbinsX()) {
			std::cerr << "[SystematicsContainer] WARNING : Histograms " << p_hist1->GetName() << " and " << p_hist2->GetName() << " have different NBinsX: " << p_hist1->GetNbinsX() << ", " << p_hist2->GetNbinsX() << ". Returning false!" << std::endl;
			consistent = false;
		} else {
			for (int bin = 1; bin <= p_hist1->GetNbinsX(); ++bin) {
				if (p_hist1->GetXaxis()->GetBinLowEdge(bin) != p_hist2->GetXaxis()->GetBinLowEdge(bin)) {
					consistent = false;
					break;
				}
				if (p_hist1->GetXaxis()->GetBinUpEdge(bin) != p_hist2->GetXaxis()->GetBinUpEdge(bin)) {
					consistent = false;
					break;
				}
			}
			if (!consistent) {
				std::cerr << "[SystematicsContainer] WARNING : Histograms " << p_hist1->GetName() << " and " << p_hist2->GetName() << " are not compatible! Returning false!" << std::endl;
				std::cerr << "[SystematicsContainer] WARNING : Bin : (h1 low, h1 high) (h2 low, h2 high)" << std::endl;
				for (int bin = 1; bin <= p_hist1->GetNbinsX(); ++bin) {
					std::cerr << "[SystematicsContainer] WARNING : " << bin << " : (" << p_hist1->GetXaxis()->GetBinLowEdge(bin) << ", " << p_hist1->GetXaxis()->GetBinUpEdge(bin) << ") (" << p_hist2->GetXaxis()->GetBinLowEdge(bin) << ", " << p_hist2->GetXaxis()->GetBinUpEdge(bin) << ")" << std::endl;
				}
			}
		}
		return consistent;
	}
	
	bool SystematicsContainer::HasSystematic(TString p_syst_name) {
		return (std::find(m_systematics_list_.begin(), m_systematics_list_.end(), p_syst_name) != m_systematics_list_.end());
	}

}
#endif