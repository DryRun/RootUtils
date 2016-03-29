#ifndef SignalCanvas_cxx
#define SignalCanvas_cxx

#include "MyTools/RootUtils/interface/SignalCanvas.h"

namespace Root {
	bool CompareSignalCanvasHistogramSortingContainers(SignalCanvas::SignalCanvasHistogramSortingContainer *hsc1, SignalCanvas::SignalCanvasHistogramSortingContainer *hsc2) {
		return (hsc1->hist->Integral() < hsc2->hist->Integral());
	}

	SignalCanvas::SignalCanvas() : DryCanvas() {}

	SignalCanvas::SignalCanvas(const char* name, const char* title, Int_t ww, Int_t wh) : DryCanvas(name, title, ww, wh) {
		m_logy_ = false;
		m_yaxis_custom_limits_ = false;
		m_draw_mc_uncertainty_ = true;
		m_hist_frame_ = 0;
		m_luminosity_ = 20.3;
		m_yaxis_title_ = "";
	}

	SignalCanvas::~SignalCanvas() {
		delete m_ths_backgrounds_;
		if (m_background_histograms_list_.size() > 0) {
			if (m_hist_total_background_) {
				delete m_hist_total_background_;
				m_hist_total_background_ = 0;
			}
		}
		m_signal_histograms_.clear();
		m_background_histograms_.clear();
		//if (m_hist_frame_ != 0x0) delete m_hist_frame_;
		delete m_hist_frame_;
	}

	void SignalCanvas::AddSignalHistogram(TH1D *p_hist, TString p_process, TString p_legend_entry, double p_luminosity, double p_scale_factor) {
		DryCanvas::AddHistogram(p_hist, p_process, p_legend_entry);

		m_signal_histograms_[p_process] = new SignalCanvas::SignalCanvasHistogramSortingContainer();
		m_signal_histograms_[p_process]->hist = m_histograms[p_process];
		m_signal_histograms_[p_process]->hist->SetDirectory(0);
		m_signal_histograms_[p_process]->process = p_process;
		m_signal_histograms_[p_process]->legend_entry = p_legend_entry;
		m_signal_histograms_[p_process]->luminosity = p_luminosity;
		m_signal_histograms_[p_process]->manual_scale_factor = p_scale_factor;
		m_signal_histograms_list_.push_back(m_signal_histograms_[p_process]);
	}

	void SignalCanvas::AddBackgroundHistogram(TH1D *p_hist, TString p_process, TString p_legend_entry, double p_luminosity) {

		// Skip empty histograms
		if (p_hist->Integral() == 0) {
			std::cerr << "[SignalCanvas] WARNING : Histogram " << p_hist->GetName() << " has integral 0. Skipping drawing." << std::endl;
			return;
		}
		// Skip 0 luminosity
		if (p_luminosity <= 0) {
			std::cerr << "[SignalCanvas] WARNING : Histogram " << p_hist->GetName() << " has specified luminosity of 0. Skipping drawing." << std::endl;
			return;
		}

		DryCanvas::AddHistogram(p_hist, p_process, p_legend_entry);
		
		m_background_histograms_[p_process] = new SignalCanvas::SignalCanvasHistogramSortingContainer();
		m_background_histograms_[p_process]->hist = m_histograms[p_process];
		m_background_histograms_[p_process]->hist->SetDirectory(0);
		m_background_histograms_[p_process]->process = p_process;
		m_background_histograms_[p_process]->legend_entry = p_legend_entry;
		m_background_histograms_[p_process]->luminosity = p_luminosity;
		m_background_histograms_[p_process]->manual_scale_factor = 1.;
		m_background_histograms_list_.push_back(m_background_histograms_[p_process]);
	}

	void SignalCanvas::SetYaxisRange(double p_ymin, double p_ymax) {
		m_yaxis_custom_limits_ = true;
		m_ymin_ = p_ymin;
		m_ymax_ = p_ymax;
	}

	void SignalCanvas::SetYaxisTitle(TString p_title) {
		m_yaxis_title_ = p_title;
	}

	void SignalCanvas::SetLogy(bool p_logy) {
		m_logy_ = p_logy;
	}

	void SignalCanvas::SetLuminosity(double p_luminosity) {
		m_luminosity_ = p_luminosity;
	}

	void SignalCanvas::MakeCanvas() {
		// - Make sure we have some histograms
		if (m_background_histograms_list_.size() == 0) {
			std::cerr << "[SignalCanvas] ERROR : No background histograms loaded." << std::endl;
			exit(1);
		}
		if (m_signal_histograms_list_.size() == 0) {
			std::cerr << "[SignalCanvas] ERROR : No signal histograms loaded." << std::endl;
			exit(1);
		}

		DryCanvas::MakeCanvas();

		// - Make background THStack
		// Normalize histograms
		for (std::map<TString, SignalCanvas::SignalCanvasHistogramSortingContainer*>::iterator it = m_signal_histograms_.begin(); it != m_signal_histograms_.end(); ++it) {
			std::cout << "[SignalCanvas] DEBUG : Scale factor = " << m_luminosity_ / (*it).second->luminosity * (*it).second->manual_scale_factor << " for histogram " << (*it).second->legend_entry << std::endl;
			(*it).second->hist->Scale(m_luminosity_ / (*it).second->luminosity * (*it).second->manual_scale_factor);
		}
		for (std::map<TString, SignalCanvas::SignalCanvasHistogramSortingContainer*>::iterator it = m_background_histograms_.begin(); it != m_background_histograms_.end(); ++it) {
			(*it).second->hist->Scale(m_luminosity_ / (*it).second->luminosity * (*it).second->manual_scale_factor);
		}
		// Sort the background process list by histogram integral
		std::sort(m_background_histograms_list_.begin(), m_background_histograms_list_.end(), CompareSignalCanvasHistogramSortingContainers);
		// Style the background histograms
		int counter = 0;
		for (std::vector<SignalCanvas::SignalCanvasHistogramSortingContainer*>::iterator it = m_background_histograms_list_.begin(); it != m_background_histograms_list_.end(); ++it) {
			if (m_background_histograms_color_override_.find((*it)->process) != m_background_histograms_color_override_.end()) {
				(*it)->hist->SetLineColor(m_background_histograms_color_override_[(*it)->process]);
				(*it)->hist->SetMarkerColor(m_background_histograms_color_override_[(*it)->process]);
				(*it)->hist->SetFillColor(m_background_histograms_color_override_[(*it)->process]);
			} else if (m_background_histograms_list_.size() <= colors.size()) {
				//(*it)->hist->SetLineColor(m_root_colors_->GetColor(Root::RootColors::kRainbowMatte, TMath::CeilNint(35 - 2 * counter)));
				//(*it)->hist->SetMarkerColor(m_root_colors_->GetColor(Root::RootColors::kRainbowMatte, TMath::CeilNint(35 - 2 * counter)));
				//(*it)->hist->SetLineColor(kRed-10 + counter);
				//(*it)->hist->SetMarkerColor(kRed-10 + counter);
				//(*it)->hist->SetFillColor(kRed-10 + counter);
				(*it)->hist->SetLineColor(m_root_colors_->GetColor(Root::RootColors::kRainbow, 31 - (5 * counter)));
				(*it)->hist->SetMarkerColor(m_root_colors_->GetColor(Root::RootColors::kRainbow, 31 - (5 * counter)));
				(*it)->hist->SetFillColor(m_root_colors_->GetColor(Root::RootColors::kRainbow, 31 - (5 * counter)));
				//(*it)->hist->SetFillColor(m_root_colors_->GetColor(Root::RootColors::kRainbowMatte, TMath::CeilNint(35 - 2 * counter)));
			} else {
				Float_t hue = 360. * counter / m_background_histograms_list_.size();
				Float_t saturation = 1.;
				Float_t brightness = 1.;
				Float_t rgb[3] = {1,0,0};
				TColor::HSV2RGB(hue, saturation, brightness, rgb[0], rgb[1], rgb[2]);
				TColor *color = gROOT->GetColor(counter+10);
				color->SetRGB(rgb[0], rgb[1], rgb[2]);
				(*it)->hist->SetMarkerColor(counter+10);
				(*it)->hist->SetLineColor(counter+10);
				(*it)->hist->SetFillColor(counter+10);
			}
			(*it)->hist->SetLineStyle(1);
			(*it)->hist->SetLineWidth(0);
			(*it)->hist->SetMarkerStyle(20 + (counter % 14));
			(*it)->hist->SetMarkerStyle(markers[counter]);
			if (m_background_histograms_fillstyle_override_.find((*it)->process) != m_background_histograms_fillstyle_override_.end()) {
				(*it)->hist->SetFillStyle(m_background_histograms_fillstyle_override_[(*it)->process]);
			} else {
				(*it)->hist->SetFillStyle(3005);
			}
			++counter;
		}
		// Make THStack
		m_ths_backgrounds_ = new THStack("ths_backgrounds", "Backgrounds");
		for (std::vector<SignalCanvas::SignalCanvasHistogramSortingContainer*>::iterator it = m_background_histograms_list_.begin(); it != m_background_histograms_list_.end(); ++it) {
			m_ths_backgrounds_->Add((*it)->hist);
			if (it == m_background_histograms_list_.begin()) {
				m_hist_total_background_ = (TH1D*)(*it)->hist->Clone();
				m_hist_total_background_->SetName("h_total_background");
				m_hist_total_background_->Sumw2();
			} else {
				m_hist_total_background_->Add((*it)->hist);
			}
		}

		// Make frame
		TH1D *c_example_histogram = (m_histograms.begin()->second);
		if (m_xaxis_keepbinlabels_) {
			Int_t old_nbins = c_example_histogram->GetNbinsX();
			Int_t new_nbins = TMath::CeilNint(1.33 * old_nbins);
			double new_xmax = c_example_histogram->GetXaxis()->GetXmax() + (1. * new_nbins - old_nbins) / old_nbins * (c_example_histogram->GetXaxis()->GetXmax() - c_example_histogram->GetXaxis()->GetXmin());
			m_hist_frame_ = new TH1D("frame", "frame", new_nbins, c_example_histogram->GetXaxis()->GetXmin(), new_xmax);
			m_hist_frame_->SetDirectory(0);
			for (int bin = 1; bin <= m_hist_frame_->GetNbinsX(); ++bin) {
				if (bin <= c_example_histogram->GetNbinsX()) {
					m_hist_frame_->GetXaxis()->SetBinLabel(bin, c_example_histogram->GetXaxis()->GetBinLabel(bin));
				} else {
					m_hist_frame_->GetXaxis()->SetBinLabel(bin, "");
				}
			}
		} else if (m_xaxis_customlimits) {
			m_hist_frame_ = new TH1D("frame", "frame", 100, m_xmin, m_xmax);
		} else {
			double c_xmin = c_example_histogram->GetXaxis()->GetXmin();
			double c_xmax = c_example_histogram->GetXaxis()->GetXmax();
			m_hist_frame_ = new TH1D("frame", "frame", 100, c_xmin, c_xmax + (0.2 * c_xmax - c_xmin));
		}

		if (m_xaxis_customtitle) {
			m_hist_frame_->GetXaxis()->SetTitle(m_xtitle);
		} else {
			m_hist_frame_->GetXaxis()->SetTitle(c_example_histogram->GetXaxis()->GetTitle());
		}

		if (m_yaxis_custom_limits_) {
			m_hist_frame_->SetMinimum(m_ymin_);
			m_hist_frame_->SetMaximum(m_ymax_);
		} else {
			m_hist_frame_->SetMinimum((m_logy_ ? 0.1 : 0.));
			// Get max
			double c_frame_max = m_hist_total_background_->GetMaximum();
			for (std::vector<SignalCanvas::SignalCanvasHistogramSortingContainer*>::iterator it = m_signal_histograms_list_.begin(); it != m_signal_histograms_list_.end(); ++it) {
				if ((*it)->hist->GetMaximum() > c_frame_max) c_frame_max = (*it)->hist->GetMaximum();
			}
			m_hist_frame_->SetMaximum(c_frame_max * (m_logy_ ? 3 : 1.5));
		}		
		m_hist_frame_->GetYaxis()->SetTitle(m_yaxis_title_);
		
		if (m_logy_) {
			dynamic_cast<TCanvas*>(this)->SetLogy();
		}

		this->cd();
		m_hist_frame_->Draw();

		// Draw THStack
		m_ths_backgrounds_->Draw("hist same");
		// Make MC uncertainty band
		if (m_draw_mc_uncertainty_) {
			m_hist_total_background_->SetMarkerSize(0);
			m_hist_total_background_->SetFillStyle(3002);
			m_hist_total_background_->SetFillColor(kGray+2);
			m_hist_total_background_->DrawCopy("e2 same");
		}

		// Draw signal histograms
		int counter_sig = 0;
		for (std::vector<SignalCanvas::SignalCanvasHistogramSortingContainer*>::iterator it = m_signal_histograms_list_.begin(); it != m_signal_histograms_list_.end(); ++it) {
			(*it)->hist->SetLineStyle(1);
			if (m_background_histograms_color_override_.find((*it)->process) != m_background_histograms_color_override_.end()) {
				(*it)->hist->SetLineColor(m_background_histograms_color_override_[(*it)->process]);
				(*it)->hist->SetFillColor(m_background_histograms_color_override_[(*it)->process]);
			} else {
				(*it)->hist->SetLineColor(kBlue);
				(*it)->hist->SetFillColor(kBlue);
			}
			if (m_background_histograms_fillstyle_override_.find((*it)->process) != m_background_histograms_fillstyle_override_.end()) {
				(*it)->hist->SetFillStyle(m_background_histograms_fillstyle_override_[(*it)->process]);
			} else {
				(*it)->hist->SetFillStyle(3004);
			}
			(*it)->hist->SetLineWidth(2);
			(*it)->hist->Draw("hist same");
			++counter_sig;
		}

		// Make legend
		m_legend_ = new Root::DryLegend(0.78, 0.3, 0.93, 0.8);
		m_legend_->SetBorderSize(0);
		for (std::vector<SignalCanvas::SignalCanvasHistogramSortingContainer*>::iterator it = m_background_histograms_list_.begin(); it != m_background_histograms_list_.end(); ++it) {
			m_legend_->AddEntry((*it)->hist, (*it)->legend_entry, "f");
		}
		for (std::vector<SignalCanvas::SignalCanvasHistogramSortingContainer*>::iterator it = m_signal_histograms_list_.begin(); it != m_signal_histograms_list_.end(); ++it) {
			TString c_legend_entry = (*it)->legend_entry;
			if ((*it)->manual_scale_factor != 1.) {
				c_legend_entry += " #times";
				c_legend_entry += (*it)->manual_scale_factor;
			}
			m_legend_->AddEntry((*it)->hist, c_legend_entry, "f");
		}
		m_legend_->Draw();

		// Redraw axes
		m_hist_frame_->Draw("axis same");
	}


}

#endif