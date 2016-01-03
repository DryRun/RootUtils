#ifndef DataBackgroundCanvas_cxx
#define DataBackgroundCanvas_cxx

#include "RootUtils/DataBackgroundCanvas.h"

namespace Root {
	bool CompareDataBackgroundCanvasHistogramSortingContainers(DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer *hsc1, DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer *hsc2) {
		return (hsc1->hist->Integral() < hsc2->hist->Integral());
	}

	DataBackgroundCanvas::DataBackgroundCanvas() : DryCanvas() {
		m_hist_total_background_ = 0;
		m_hist_total_background_syst_ = 0;
		m_hist_frame_ = 0;
		m_ratio_frame_ = 0;
		m_legend_ = 0;
		m_top_ = 0;
		m_bottom_ = 0;
		m_ths_backgrounds_ = 0;
		m_line_unity_	 = 0;
		m_systematics_band_ = 0;
		m_manual_systematics_band_ = false;
		m_use_bin_labels_ = false;
		m_normalize_by_bin_width_ = false;
		m_bin_width_normalization_ = 0.;
		m_do_stack_signal_ = false;
		m_data_error_style_ = kGaussian;
		m_ratio_custom_limits_ = false;
		m_ratio_min_ = 0.;
		m_ratio_max_ = 0.;
		m_ratio_style_ = kRatio;
		m_y_scale_ = 0.;
		m_unit_normalization_ = false;
	}

	DataBackgroundCanvas::DataBackgroundCanvas(const char* name, const char* title, Int_t ww, Int_t wh) : DryCanvas(name, title, ww, wh) {
		DataBackgroundCanvas();
		m_logy_ = false;
		m_yaxis_custom_limits_ = false;
		m_draw_mc_uncertainty_ = false;
		m_hist_frame_ = 0;
		m_luminosity_ = 20.3435;
		m_yaxis_title_ = "";
		m_draw_ratio_= false;
		m_systematics_band_ = 0;
		m_manual_systematics_band_ = false;
		m_use_bin_labels_ = false;
		m_normalize_by_bin_width_ = false;
		m_bin_width_normalization_ = 0.;
		m_do_stack_signal_ = false;
		m_data_error_style_ = kGaussian;
		m_ratio_custom_limits_ = false;
		m_ratio_min_ = 0.;
		m_ratio_max_ = 0.;
		m_ratio_style_ = kRatio;
		m_y_scale_ = 0.;
		m_unit_normalization_ = false;
	}

	DataBackgroundCanvas::~DataBackgroundCanvas() {
		m_data_histograms_.clear();
		m_signal_histograms_.clear();
		m_signalstacked_histograms_.clear();
		m_background_histograms_.clear();
		if (m_ths_backgrounds_) {
			//std::cout << "[DataBackgroundCanvas] DEBUG : m_ths_backgrounds_ = " << m_ths_backgrounds_ << std::endl;
			delete m_ths_backgrounds_;
		}
		if (m_top_) delete m_top_;
		if (m_draw_ratio_) {
			if (m_bottom_) delete m_bottom_;
			if (m_ratio_frame_) delete m_ratio_frame_;
			if (m_ratio_style_ == kRatio) {
				if (m_line_unity_) delete m_line_unity_;
			}
		}
		if (m_hist_frame_) delete m_hist_frame_;
		if (m_legend_) delete m_legend_;
		//if (m_hist_total_background_) delete m_hist_total_background_;
	}

	void DataBackgroundCanvas::AddDataHistogram(TH1D *p_hist, TString p_process, TString p_legend_entry, double p_luminosity, double p_scale_factor) {
		DryCanvas::AddHistogram(p_hist, p_process, p_legend_entry);

		m_data_histograms_[p_process] = new DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer();
		m_data_histograms_[p_process]->hist = m_histograms[p_process];
		m_data_histograms_[p_process]->hist->SetDirectory(0);
		m_data_histograms_[p_process]->process = p_process;
		m_data_histograms_[p_process]->legend_entry = p_legend_entry;
		m_data_histograms_[p_process]->luminosity = p_luminosity;
		m_data_histograms_[p_process]->manual_scale_factor = p_scale_factor;
		m_data_histograms_list_.push_back(m_data_histograms_[p_process]);
	}

	void DataBackgroundCanvas::AddSignalHistogram(TH1D *p_hist, TString p_process, TString p_legend_entry, double p_luminosity, double p_scale_factor) {
		DryCanvas::AddHistogram(p_hist, p_process, p_legend_entry);

		m_signal_histograms_[p_process] = new DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer();
		m_signal_histograms_[p_process]->hist = m_histograms[p_process];
		m_signal_histograms_[p_process]->hist->SetDirectory(0);
		m_signal_histograms_[p_process]->process = p_process;
		m_signal_histograms_[p_process]->legend_entry = p_legend_entry;
		m_signal_histograms_[p_process]->luminosity = p_luminosity;
		m_signal_histograms_[p_process]->manual_scale_factor = p_scale_factor;
		m_signal_histograms_list_.push_back(m_signal_histograms_[p_process]);

		// Save an additional copy for making signal + background histograms
		m_signalstacked_histograms_[p_process] = new DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer();
		m_signalstacked_histograms_[p_process]->hist = (TH1D*)m_histograms[p_process]->Clone();
		m_signalstacked_histograms_[p_process]->hist->SetDirectory(0);
		m_signalstacked_histograms_[p_process]->process = p_process;
		m_signalstacked_histograms_[p_process]->legend_entry = p_legend_entry;
		m_signalstacked_histograms_[p_process]->luminosity = p_luminosity;
		m_signalstacked_histograms_[p_process]->manual_scale_factor = p_scale_factor;
		m_signalstacked_histograms_list_.push_back(m_signalstacked_histograms_[p_process]);
	}

	void DataBackgroundCanvas::AddBackgroundHistogram(TH1D *p_hist, TString p_process, TString p_legend_entry, double p_luminosity) {

		// Skip empty histograms
		if (p_hist->Integral() == 0) {
			//std::cerr << "[DataBackgroundCanvas] WARNING : Histogram " << p_hist->GetName() << " has integral 0. Skipping drawing." << std::endl;
			return;
		}
		// Skip 0 luminosity
		if (p_luminosity <= 0) {
			std::cerr << "[DataBackgroundCanvas] WARNING : Histogram " << p_hist->GetName() << " has specified luminosity of 0. Skipping drawing." << std::endl;
			return;
		}

		DryCanvas::AddHistogram(p_hist, p_process, p_legend_entry);
		
		m_background_histograms_[p_process] = new DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer();
		m_background_histograms_[p_process]->hist = m_histograms[p_process];
		m_background_histograms_[p_process]->hist->SetDirectory(0);
		m_background_histograms_[p_process]->process = p_process;
		m_background_histograms_[p_process]->legend_entry = p_legend_entry;
		m_background_histograms_[p_process]->luminosity = p_luminosity;
		m_background_histograms_[p_process]->manual_scale_factor = 1.;
		m_background_histograms_list_.push_back(m_background_histograms_[p_process]);
	}

	void DataBackgroundCanvas::SetYaxisRange(double p_ymin, double p_ymax) {
		m_yaxis_custom_limits_ = true;
		m_ymin_ = p_ymin;
		m_ymax_ = p_ymax;
	}

	void DataBackgroundCanvas::SetYaxisTitle(TString p_title) {
		m_yaxis_title_ = p_title;
	}

	void DataBackgroundCanvas::SetLogy(bool p_logy) {
		m_logy_ = p_logy;
	}

	void DataBackgroundCanvas::SetLuminosity(double p_luminosity) {
		m_luminosity_ = p_luminosity;
	}

	void DataBackgroundCanvas::MakeCanvas() {
		// - Make sure we have some histograms
		if (m_background_histograms_list_.size() == 0) {
			std::cerr << "[DataBackgroundCanvas] WARNING : No background histograms loaded." << std::endl;
			//return;
		}
		if (m_signal_histograms_list_.size() == 0) {
			std::cerr << "[DataBackgroundCanvas] WARNING : No signal histograms loaded." << std::endl;
			//return;
		}

		DryCanvas::MakeCanvas();

		// - Make background THStack
		// Normalize histograms
		for (std::map<TString, DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_signal_histograms_.begin(); it != m_signal_histograms_.end(); ++it) {
			//std::cout << "[DataBackgroundCanvas] DEBUG : Scale factor = " << m_luminosity_ / (*it).second->luminosity * (*it).second->manual_scale_factor << " for histogram " << (*it).second->legend_entry << std::endl;
			if (m_unit_normalization_) {
				(*it).second->hist->Scale(((*it).second->hist->Integral() > 0 ? 1. / (*it).second->hist->Integral() : 0.));
			} else {
				(*it).second->hist->Scale(m_luminosity_ / (*it).second->luminosity * (*it).second->manual_scale_factor);
			}
		}

		if (m_unit_normalization_) {
			double total_background_integral = 0.;
			for (std::map<TString, DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_background_histograms_.begin(); it != m_background_histograms_.end(); ++it) {
				total_background_integral += (*it).second->hist->Integral();
			}
			for (std::map<TString, DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_background_histograms_.begin(); it != m_background_histograms_.end(); ++it) {
				(*it).second->hist->Scale(total_background_integral > 0 ? 1. / total_background_integral : 0.);
			}
		} else {
			for (std::map<TString, DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_background_histograms_.begin(); it != m_background_histograms_.end(); ++it) {
				(*it).second->hist->Scale(m_luminosity_ / (*it).second->luminosity * (*it).second->manual_scale_factor);
			}
		}


		if (m_normalize_by_bin_width_) {
			for (std::map<TString, DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_signal_histograms_.begin(); it != m_signal_histograms_.end(); ++it) {
				for (int bin = 1; bin <= (*it).second->hist->GetNbinsX(); ++bin) {
					(*it).second->hist->SetBinContent(bin, (*it).second->hist->GetBinContent(bin) * m_bin_width_normalization_ / (*it).second->hist->GetXaxis()->GetBinWidth(bin));
					(*it).second->hist->SetBinError(bin, (*it).second->hist->GetBinError(bin) * m_bin_width_normalization_ / (*it).second->hist->GetXaxis()->GetBinWidth(bin));

				}
			}
			for (std::map<TString, DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_background_histograms_.begin(); it != m_background_histograms_.end(); ++it) {
				for (int bin = 1; bin <= (*it).second->hist->GetNbinsX(); ++bin) {
					(*it).second->hist->SetBinContent(bin, (*it).second->hist->GetBinContent(bin) * m_bin_width_normalization_ / (*it).second->hist->GetXaxis()->GetBinWidth(bin));
					(*it).second->hist->SetBinError(bin, (*it).second->hist->GetBinError(bin) * m_bin_width_normalization_ / (*it).second->hist->GetXaxis()->GetBinWidth(bin));

				}
			}
			for (std::map<TString, DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_data_histograms_.begin(); it != m_data_histograms_.end(); ++it) {
				for (int bin = 1; bin <= (*it).second->hist->GetNbinsX(); ++bin) {
					(*it).second->hist->SetBinContent(bin, (*it).second->hist->GetBinContent(bin) * m_bin_width_normalization_ / (*it).second->hist->GetXaxis()->GetBinWidth(bin));
					(*it).second->hist->SetBinError(bin, (*it).second->hist->GetBinError(bin) * m_bin_width_normalization_ / (*it).second->hist->GetXaxis()->GetBinWidth(bin));

				}
			}
		}

		// Sort the background process list by histogram integral
		if (m_background_histograms_list_.size() > 0) {
			std::sort(m_background_histograms_list_.begin(), m_background_histograms_list_.end(), CompareDataBackgroundCanvasHistogramSortingContainers);
		}
		// Style the background histograms
		int counter = 0;
		for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_background_histograms_list_.begin(); it != m_background_histograms_list_.end(); ++it) {
			if (m_background_histograms_color_override_.find((*it)->process) != m_background_histograms_color_override_.end()) {
				(*it)->hist->SetLineColor(1);
				(*it)->hist->SetMarkerColor(m_background_histograms_color_override_[(*it)->process]);
				(*it)->hist->SetFillColor(m_background_histograms_color_override_[(*it)->process]);
			} else if (m_background_histograms_list_.size() <= colors.size()) {
				/** Color-hashed histograms **/
				/*
				(*it)->hist->SetLineColor(m_root_colors_->GetColor(Root::RootColors::kRainbow, 31 - (5 * counter)));
				(*it)->hist->SetMarkerColor(m_root_colors_->GetColor(Root::RootColors::kRainbow, 31 - (5 * counter)));
				(*it)->hist->SetFillColor(m_root_colors_->GetColor(Root::RootColors::kRainbow, 31 - (5 * counter)));
				//(*it)->hist->SetFillColor(m_root_colors_->GetColor(Root::RootColors::kRainbowMatte, TMath::CeilNint(35 - 2 * counter)));
				*/
				/** Seaborn colors **/
				(*it)->hist->SetLineColor(1);
				(*it)->hist->SetMarkerColor(m_seaborn_->GetColorRoot("dark", counter));
				(*it)->hist->SetFillColor(m_seaborn_->GetColorRoot("dark", counter));
			} else {
				/** Manual hue gradient **/
				/*
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
				*/
				/** Seaborn HUSL gradient **/
				(*it)->hist->SetMarkerColor(m_seaborn_->GetColorRoot("husl", counter, m_background_histograms_list_.size()));
				(*it)->hist->SetLineColor(1);
				(*it)->hist->SetFillColor(m_seaborn_->GetColorRoot("husl", counter, m_background_histograms_list_.size()));
			}
			(*it)->hist->SetLineStyle(1);
			(*it)->hist->SetLineWidth(1);
			(*it)->hist->SetMarkerStyle(20 + (counter % 14));
			(*it)->hist->SetMarkerStyle(markers[counter]);
			if (m_background_histograms_fillstyle_override_.find((*it)->process) != m_background_histograms_fillstyle_override_.end()) {
				(*it)->hist->SetFillStyle(m_background_histograms_fillstyle_override_[(*it)->process]);
			} else {
				//(*it)->hist->SetFillStyle(3005);
				(*it)->hist->SetFillStyle(1001);
			}
			++counter;
		}
		// Make THStack and total background histograms
		m_ths_backgrounds_ = new THStack("ths_backgrounds", "Backgrounds");
		if (m_background_histograms_list_.size() > 0) {
			for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_background_histograms_list_.begin(); it != m_background_histograms_list_.end(); ++it) {
				m_ths_backgrounds_->Add((*it)->hist);
				if (it == m_background_histograms_list_.begin()) {
					m_hist_total_background_ = (TH1D*)(*it)->hist->Clone();
					m_hist_total_background_->SetDirectory(0);
					m_hist_total_background_->SetName("h_total_background");
					m_hist_total_background_->Sumw2();
				} else {
					m_hist_total_background_->Add((*it)->hist);
				}
			}
		} else {
			m_hist_total_background_ = 0;
		}

		// Make signal+background histograms
		int line_style_counter = 1;
		for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_signalstacked_histograms_list_.begin(); it != m_signalstacked_histograms_list_.end(); ++it) {
			(*it)->hist->Add(m_hist_total_background_);
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
			(*it)->hist->SetLineStyle(line_style_counter);
			++line_style_counter;
		}

		// Start actual drawing. 
		if (m_draw_ratio_) {
			//std::cout << "[DataBackgroundCanvas] DEBUG : Making m_top_" << std::endl;
			m_top_ = new TPad("top", "top", 0., 0.41, 1., 1.);
			m_top_->SetBottomMargin(0.03);
			m_top_->Draw();
			if (m_logy_) {
				m_top_->SetLogy();
			}
			this->cd();
			//std::cout << "[DataBackgroundCanvas] DEBUG : Making m_bottom_" << std::endl;
			m_bottom_ = new TPad("bottom", "bottom", 0., 0., 1., 0.4);
			m_bottom_->SetBottomMargin(0.25);
			m_bottom_->SetTopMargin(0.03);
			m_bottom_->Draw();
			this->cd();
		} else {
			//std::cout << "[DataBackgroundCanvas] DEBUG : Making m_top_" << std::endl;
			m_top_ = new TPad("top", "top", 0., 0., 1., 1.);
			m_top_->Draw();
			if (m_logy_) {
				m_top_->SetLogy();
			}
		}

		// Top canvas
		//std::cout << "[DataBackgroundCanvas] DEBUG : Starting drawing of top pad" << std::endl;
		m_top_->cd();

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
			std::cout << "[DataBackgroundCanvas] DEBUG : Making histogram frame with limits " << m_xmin << ", " << m_xmax << std::endl;
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
			double c_frame_max = 1.;
			if (m_signal_histograms_list_.size() > 0) {
				for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_signal_histograms_list_.begin(); it != m_signal_histograms_list_.end(); ++it) {
					if ((*it)->hist->GetMaximum() > c_frame_max) c_frame_max = (*it)->hist->GetMaximum();
				}
			}
			if (m_data_histograms_list_.size() > 0) {
				for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_data_histograms_list_.begin(); it != m_data_histograms_list_.end(); ++it) {
					if ((*it)->hist->GetMaximum() > c_frame_max) c_frame_max = (*it)->hist->GetMaximum();
				}
			}
			if (m_background_histograms_list_.size() > 0) {
				if (m_hist_total_background_) {
					if (m_hist_total_background_->GetMaximum() > c_frame_max) {
						c_frame_max = m_hist_total_background_->GetMaximum();
					}
				}
				for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_background_histograms_list_.begin(); it != m_background_histograms_list_.end(); ++it) {
					if ((*it)->hist->GetMaximum() > c_frame_max) c_frame_max = (*it)->hist->GetMaximum();
				}
			}
			if (m_y_scale_ > 0) {
				m_hist_frame_->SetMaximum(c_frame_max * m_y_scale_);
			} else {
				m_hist_frame_->SetMaximum(c_frame_max * (m_logy_ ? 10 : 1.5));
			}
		}		
		m_hist_frame_->GetYaxis()->SetTitle(m_yaxis_title_);
		
		//if (m_logy_) {
		//	dynamic_cast<TCanvas*>(m_top_)->SetLogy();
		//}

		if (m_draw_ratio_) {
			m_hist_frame_->GetXaxis()->SetLabelSize(0.);
			m_hist_frame_->GetXaxis()->SetTitleSize(0.);
			m_hist_frame_->GetYaxis()->SetLabelSize(0.0610);
			m_hist_frame_->GetYaxis()->SetTitleSize(0.0610);
			m_hist_frame_->GetYaxis()->SetTitleOffset(0.85);
		} else {
			// If no ratio, the top frame gets the labels
			if (m_use_bin_labels_) {
				// Check that we have the right number of bin labels
				if (m_bin_labels_.size() != m_hist_frame_->GetNbinsX()) {
					std::cerr << "[DataBackgroundCanvas] You specified " << m_bin_labels_.size() << " labels, but the frame has " << m_hist_frame_->GetNbinsX() << " bins. Please fix." << std::endl;
					exit(1);
				}
				for (int i_bin = 1; i_bin <= m_hist_frame_->GetNbinsX(); ++i_bin) {
					m_hist_frame_->GetXaxis()->SetBinLabel(i_bin, m_bin_labels_[i_bin-1]);
				}
			}
		}

		m_top_->cd();
		m_hist_frame_->Draw();

		// If drawing signal + background stacked histograms, draw them first. They should get covered up by everything else.
		if (m_do_stack_signal_) {
			for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_signalstacked_histograms_list_.begin(); it != m_signalstacked_histograms_list_.end(); ++it) {
				(*it)->hist->Draw("hist same");
			}
		}

		// Draw THStack
		m_ths_backgrounds_->Draw("hist same");
		// Make MC uncertainty band
		if (m_draw_mc_uncertainty_) {
			if (m_background_histograms_list_.size() > 0) {
				m_hist_total_background_->SetMarkerSize(0);
				m_hist_total_background_->SetFillStyle(3002);
				m_hist_total_background_->SetFillColor(kGray+2);
				m_hist_total_background_->DrawCopy("e2 same");
			}
		}

		// Draw data histograms
		int counter_data = 0;
		for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_data_histograms_list_.begin(); it != m_data_histograms_list_.end(); ++it) {
			// Convert histogram to TGraph with Poisson errors
			int n_good_points = 0;
			for (int bin = 1; bin <= (*it)->hist->GetNbinsX(); ++bin) {
				if ((*it)->hist->GetBinContent(bin) > 0) {
					++n_good_points;
				}
			}
			(*it)->graph = new TGraphAsymmErrors(n_good_points);
			int counter = 0;
			for (int bin = 1; bin <= (*it)->hist->GetNbinsX(); ++bin) {
				double content = (*it)->hist->GetBinContent(bin);
				if (content > 0) {
					(*it)->graph->SetPoint(counter, (*it)->hist->GetXaxis()->GetBinCenter(bin), content);
					if (m_data_error_style_ == kGaussian) {
						(*it)->graph->SetPointError(counter, (*it)->hist->GetXaxis()->GetBinWidth(bin) / 2., (*it)->hist->GetXaxis()->GetBinWidth(bin) / 2., TMath::Sqrt(content), TMath::Sqrt(content));
					} else {
						(*it)->graph->SetPointError(counter, (*it)->hist->GetXaxis()->GetBinWidth(bin) / 2., (*it)->hist->GetXaxis()->GetBinWidth(bin) / 2., -0.5 + TMath::Sqrt(0.25 + content), 0.5 + TMath::Sqrt(0.25 + content));
					}
					++counter;
				}
			}

			(*it)->graph->SetMarkerSize(1.2);
			(*it)->graph->SetMarkerStyle(20 + counter_data);
			(*it)->graph->SetMarkerColor(kBlack);
			(*it)->graph->SetLineWidth(1);
			(*it)->graph->SetLineStyle(1);
			(*it)->graph->SetLineColor(kBlack);
			(*it)->graph->Draw("p");
			++counter_data;
		}

		// If not stacking the signal histograms on top of background, they have to be drawn last, so they appear on top of the colored background histograms.
		if (!m_do_stack_signal_) {
			// Draw a thicker white background first
			// 
			for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_signal_histograms_list_.begin(); it != m_signal_histograms_list_.end(); ++it) {
				(*it)->hist->SetLineStyle(1);
				(*it)->hist->SetLineWidth(3);
				(*it)->hist->SetLineColor(0);
				(*it)->hist->DrawCopy("hist same");
			}
			// Now draw the real color line
			int counter_sig = 0;
			for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_signal_histograms_list_.begin(); it != m_signal_histograms_list_.end(); ++it) {
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
				(*it)->hist->SetLineStyle(counter_sig + 1);
				(*it)->hist->Draw("hist same");
				++counter_sig;
			}
		}

		// Make legend
		if (m_manual_legend_) {
			m_legend_ = new Root::DryLegend(m_legend_xmin_, m_legend_ymin_, m_legend_xmax_, m_legend_ymax_);
		} else {
			m_legend_ = new Root::DryLegend(0.75, 0.23, 0.92, 0.73);
		}
		m_legend_->SetBorderSize(0);
		if (m_legend_columns_ > 0) {
			m_legend_->SetNColumns(m_legend_columns_);
			if (m_legend_columns_ == 2) {
				m_legend_->SetColumnSeparation(0.05);
			}
		}
		for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_data_histograms_list_.begin(); it != m_data_histograms_list_.end(); ++it) {
			m_legend_->AddEntry((*it)->hist, (*it)->legend_entry, "pe");
		}
		for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::reverse_iterator it = m_background_histograms_list_.rbegin(); it != m_background_histograms_list_.rend(); ++it) {
			// Hide histograms that fall outside the frame
			if ((*it)->hist->GetMaximum() < m_hist_frame_->GetMinimum()) {
				continue;
			}
			m_legend_->AddEntry((*it)->hist, (*it)->legend_entry, "f");

		}
		if (m_do_stack_signal_) {
			for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_signalstacked_histograms_list_.begin(); it != m_signalstacked_histograms_list_.end(); ++it) {
				TString c_legend_entry = (*it)->legend_entry;
				if ((*it)->manual_scale_factor != 1.) {
					c_legend_entry += " #times";
					c_legend_entry += (*it)->manual_scale_factor;
				}
				m_legend_->AddEntry((*it)->hist, c_legend_entry, "l");
			}
		} else {
			for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_signal_histograms_list_.begin(); it != m_signal_histograms_list_.end(); ++it) {
				TString c_legend_entry = (*it)->legend_entry;
				if ((*it)->manual_scale_factor != 1.) {
					c_legend_entry += " #times";
					c_legend_entry += (*it)->manual_scale_factor;
				}
				m_legend_->AddEntry((*it)->hist, c_legend_entry, "f");
			}

		}
		m_legend_->Draw();

		// Redraw axes
		m_hist_frame_->Draw("axis same");

		// - Bottom canvas
		if (m_draw_ratio_) {
			//std::cout << "[DataBackgroundCanvas] DEBUG : Starting drawing of bottom pad" << std::endl;
			this->cd();
			m_bottom_->cd();
			double c_ratio_min = 1.e20;
			double c_ratio_max = -1.e20;
			for (std::map<TString, DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_data_histograms_.begin(); it != m_data_histograms_.end(); ++it) {
				// Make ratio
				m_ratio_histograms_[(*it).first] = new DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer();
				m_ratio_histograms_[(*it).first]->hist                = (TH1D*)(*it).second->hist->Clone();
				m_ratio_histograms_[(*it).first]->process             = (*it).second->process;
				m_ratio_histograms_[(*it).first]->legend_entry        = (*it).second->legend_entry;
				m_ratio_histograms_[(*it).first]->luminosity          = (*it).second->luminosity;
				m_ratio_histograms_[(*it).first]->manual_scale_factor = (*it).second->manual_scale_factor;

				//m_ratio_histograms_[(*it).first]->hist->Divide(m_hist_total_background_);

				// Divide histograms
				// Uncertainties: put only the data statistical uncertainties on the ratio histogram, and put the systematics into a band
				// The histogram has to have symmetric Gaussian errors, which might not be appropriate if you want Poisson errors!
				for (int bin = 1; bin <= m_ratio_histograms_[(*it).first]->hist->GetNbinsX(); ++bin) {
					double c_background;
					if (m_background_histograms_list_.size() > 0) {
						c_background = m_hist_total_background_->GetBinContent(bin);
					} else {
						c_background = 0.;
					}
					double c_ratio_value;
					double c_ratio_uncertainty;
					if (c_background > 0) {
						if (m_ratio_style_ == kDeviation) {
							c_ratio_value = (m_ratio_histograms_[(*it).first]->hist->GetBinContent(bin) / c_background - 1.);
							c_ratio_uncertainty = m_ratio_histograms_[(*it).first]->hist->GetBinError(bin) / c_background;
						} else if (m_ratio_style_ == kDevationPercent) {
							c_ratio_value = 100. * (m_ratio_histograms_[(*it).first]->hist->GetBinContent(bin) / c_background - 1.);
							c_ratio_uncertainty = 100. * m_ratio_histograms_[(*it).first]->hist->GetBinError(bin) / c_background;
						} else if (m_ratio_style_ == kRatio) {
							c_ratio_value = (m_ratio_histograms_[(*it).first]->hist->GetBinContent(bin) / c_background);
							c_ratio_uncertainty = m_ratio_histograms_[(*it).first]->hist->GetBinError(bin) / c_background;
						}
					} else {
						c_ratio_value = -1.e20;
						c_ratio_uncertainty = 0.;
					}
					m_ratio_histograms_[(*it).first]->hist->SetBinContent(bin, c_ratio_value);
					m_ratio_histograms_[(*it).first]->hist->SetBinError(bin, c_ratio_uncertainty);

					if (c_ratio_value - c_ratio_uncertainty < c_ratio_min) c_ratio_min = c_ratio_value - c_ratio_uncertainty;
					if (c_ratio_value + c_ratio_uncertainty > c_ratio_max) c_ratio_max = c_ratio_value + c_ratio_uncertainty;
				}

				// Ratio TGraph, for asymmetric errors
				int n_good_points = 0;
				for (int bin = 1; bin <= (*it).second->hist->GetNbinsX(); ++bin) {
					double data = (*it).second->hist->GetBinContent(bin);
					double background = (m_background_histograms_list_.size() > 0 ? m_hist_total_background_->GetBinContent(bin) : 0.);
					if (data > 0. && background > 0.) {
						++n_good_points;
					}
				}
				m_ratio_histograms_[(*it).first]->graph = new TGraphAsymmErrors(n_good_points);
				int c_point = 0;		
				for (int bin = 1; bin <= (*it).second->hist->GetNbinsX(); ++bin) {
					double data = (*it).second->hist->GetBinContent(bin);
					double background = (m_background_histograms_list_.size() > 0 ? m_hist_total_background_->GetBinContent(bin) : 0.);
					if (data > 0. && background > 0.) {
						double ratio;
						double ratio_err_up;
						double ratio_err_down;
						if (m_ratio_style_ == kRatio) {
							ratio = data / background;
							ratio_err_up = (0.5 + TMath::Sqrt(0.25 + data)) / background;
							ratio_err_down = (-0.5 + TMath::Sqrt(0.25 + data)) / background;
						} else if (m_ratio_style_ == kDeviation) {
							ratio = (data / background) - 1.;
							ratio_err_up = (0.5 + TMath::Sqrt(0.25 + data)) / background;
							ratio_err_down = (-0.5 + TMath::Sqrt(0.25 + data)) / background;
						} else if (m_ratio_style_ == kDevationPercent) {
							ratio = 100. * ((data / background) - 1.);
							ratio_err_up = 100. * (0.5 + TMath::Sqrt(0.25 + data)) / background;
							ratio_err_down = 100. * (-0.5 + TMath::Sqrt(0.25 + data)) / background;
						}
						m_ratio_histograms_[(*it).first]->graph->SetPoint(c_point, m_ratio_histograms_[(*it).first]->hist->GetXaxis()->GetBinCenter(bin), ratio);
						m_ratio_histograms_[(*it).first]->graph->SetPointError(c_point, (*it).second->hist->GetXaxis()->GetBinWidth(bin) / 2., (*it).second->hist->GetXaxis()->GetBinWidth(bin) / 2., ratio_err_down, ratio_err_up);
						++c_point;
					}
				}
				m_ratio_histograms_[(*it).first]->graph->SetMarkerStyle(m_ratio_histograms_[(*it).first]->hist->GetMarkerStyle());
				//m_ratio_histograms_[(*it).first]->graph->SetMarkerSize(m_ratio_histograms_[(*it).first]->hist->GetMarkerSize());
				m_ratio_histograms_[(*it).first]->graph->SetMarkerSize(1.2);
				m_ratio_histograms_[(*it).first]->graph->SetMarkerColor(m_ratio_histograms_[(*it).first]->hist->GetMarkerColor());
				m_ratio_histograms_[(*it).first]->graph->SetLineStyle(1);
				m_ratio_histograms_[(*it).first]->graph->SetLineWidth(1);
				m_ratio_histograms_[(*it).first]->graph->SetLineColor(m_ratio_histograms_[(*it).first]->hist->GetLineColor());

				//if (m_ratio_histograms_[(*it).first]->hist->GetMinimum() < c_ratio_min) c_ratio_min = m_ratio_histograms_[(*it).first]->hist->GetMinimum();
				//if (m_ratio_histograms_[(*it).first]->hist->GetMaximum() > c_ratio_max) c_ratio_max = m_ratio_histograms_[(*it).first]->hist->GetMaximum();
				
				DryCanvas::AddHistogram(m_ratio_histograms_[(*it).first]->hist, m_ratio_histograms_[(*it).first]->process + "_ratio", m_ratio_histograms_[(*it).first]->process + "_ratio");
			}
			// Frame
			m_ratio_frame_ = (TH1D*)m_hist_frame_->Clone();
			m_ratio_frame_->GetXaxis()->SetLabelSize(0.11);
			m_ratio_frame_->GetXaxis()->SetTitleSize(0.11);
			m_ratio_frame_->GetXaxis()->SetLabelOffset(0.008);
			m_ratio_frame_->GetXaxis()->SetTitleOffset(1.0);
			m_ratio_frame_->GetYaxis()->SetLabelSize(0.09);
			m_ratio_frame_->GetYaxis()->SetTitleSize(0.1);
			m_ratio_frame_->GetYaxis()->SetTitleOffset(0.6);
			if (m_ratio_custom_limits_) {
				m_ratio_frame_->SetMinimum(m_ratio_min_);
				m_ratio_frame_->SetMaximum(m_ratio_max_);
			} else {
				if (c_ratio_max <= c_ratio_min) {
					std::cerr << "[DataBackgroundCanvas] WARNING : Ratio max <= ratio min. Setting to +/- 1." << std::endl;
					if (m_ratio_style_ == kRatio) {
						c_ratio_min = 0.;
						c_ratio_max = 2.;
					} else if (m_ratio_style_ == kDeviation) {
						c_ratio_min = -1.;
						c_ratio_max = 1.;
					} else if (m_ratio_style_ == kDevationPercent) {
						c_ratio_min = -100.;
						c_ratio_max = 100.;
					}
				}

				// Some hard limits on the ratio limits, to prevent the scale from going out of control.
				double c_ratio_min_limit;
				double c_ratio_max_limit;
				if (m_ratio_style_ == kRatio) {
					c_ratio_min_limit = 0.;
					c_ratio_max_limit = 2.;
				} else if (m_ratio_style_ == kDeviation) {
					c_ratio_min_limit = -1.;
					c_ratio_max_limit = 1.;
				} else if (m_ratio_style_ == kDevationPercent) {
					c_ratio_min_limit = -100.;
					c_ratio_max_limit = 100.;
				}

				m_ratio_frame_->SetMinimum(TMath::Max(c_ratio_min - 0.2 * (c_ratio_max - c_ratio_min), c_ratio_min_limit));
				m_ratio_frame_->SetMaximum(TMath::Min(c_ratio_max + 0.2 * (c_ratio_max - c_ratio_min), c_ratio_max_limit));
			}
			if (m_ratio_style_ == kDeviation) {
				m_ratio_frame_->GetYaxis()->SetTitle("#frac{Data-Background}{Background}");
			} else if (m_ratio_style_ == kDevationPercent) {
				m_ratio_frame_->GetYaxis()->SetTitle("#frac{Data-Background}{Background} [%]");
			} else if (m_ratio_style_ == kRatio) {
				m_ratio_frame_->GetYaxis()->SetTitle("#frac{Data}{Background}");
			}
			if (m_use_bin_labels_) {
				// Check that we have the right number of bin labels
				if (m_bin_labels_.size() != m_ratio_frame_->GetNbinsX()) {
					std::cerr << "[DataBackgroundCanvas] You specified " << m_bin_labels_.size() << " labels, but the frame has " << m_ratio_frame_->GetNbinsX() << " bins. Please fix." << std::endl;
					exit(1);
				}
				for (int i_bin = 1; i_bin <= m_ratio_frame_->GetNbinsX(); ++i_bin) {
					m_ratio_frame_->GetXaxis()->SetBinLabel(i_bin, m_bin_labels_[i_bin-1]);
				}
			}

			m_ratio_frame_->Draw();

			// Background systematic uncertainty
			if (m_manual_systematics_band_) {
				m_systematics_band_->SetMarkerSize(0);
				m_systematics_band_->SetMarkerStyle(20);
				m_systematics_band_->SetMarkerColor(1);
				m_systematics_band_->SetLineStyle(1);
				m_systematics_band_->SetLineColor(kBlack);
				m_systematics_band_->SetLineWidth(1);
				m_systematics_band_->SetFillStyle(1001);
				m_systematics_band_->SetFillColor(kGray);
				m_systematics_band_->Draw("e2 same");
			} else {
				if (m_background_histograms_list_.size() > 0) {
					m_hist_total_background_syst_ = (TH1D*)m_hist_total_background_->Clone();
					for (int bin = 1; bin <= m_hist_total_background_syst_->GetNbinsX(); ++bin) {
						m_hist_total_background_syst_->SetBinContent(bin, 0.);
						if (m_hist_total_background_->GetBinContent(bin) == 0) {
							m_hist_total_background_syst_->SetBinError(bin, 0.);
						} else {
							m_hist_total_background_syst_->SetBinError(bin, 100. * m_hist_total_background_->GetBinError(bin) / m_hist_total_background_->GetBinContent(bin));
						}
					}
					m_hist_total_background_syst_->SetMarkerSize(0);
					m_hist_total_background_syst_->SetMarkerStyle(20);
					m_hist_total_background_syst_->SetMarkerColor(1);
					m_hist_total_background_syst_->SetFillColor(kGray);
					m_hist_total_background_syst_->SetFillStyle(3144);
					m_hist_total_background_syst_->SetLineStyle(1);
					m_hist_total_background_syst_->SetLineColor(kBlack);
					m_hist_total_background_syst_->SetLineWidth(1);
					m_hist_total_background_syst_->Draw("e2 same");
				}
			}

			// Ratio plots
			for (std::map<TString, DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_ratio_histograms_.begin(); it != m_ratio_histograms_.end(); ++it) {
				(*it).second->graph->Draw("pe0 same");
			}

			// Line at one
			if (m_ratio_style_ == kRatio) {
				m_line_unity_ = new TLine(m_ratio_frame_->GetXaxis()->GetXmin(), 1., m_ratio_frame_->GetXaxis()->GetXmax(), 1.);
				m_line_unity_->SetLineStyle(2);
				m_line_unity_->SetLineWidth(2);
				m_line_unity_->SetLineColor(kGray+2);
				m_line_unity_->Draw("same");
			}
			
			// Redraw axis
			m_ratio_frame_->Draw("axis same");
		}

		// Print histogram integrals
		for (std::vector<DataBackgroundCanvas::DataBackgroundCanvasHistogramSortingContainer*>::iterator it = m_data_histograms_list_.begin(); it != m_data_histograms_list_.end(); ++it) {
			double c_sum = 0.;
			double c_sumw2 = 0.;
			for (int bin = 0; bin <= (*it)->hist->GetNbinsX() + 1; ++bin) {
				c_sum += (*it)->hist->GetBinContent(bin);
				c_sumw2 += TMath::Power((*it)->hist->GetBinError(bin), 2);
			}
			//std::cout << "[DataBackgroundCanvas] INFO : Data integral (" << (*it)->legend_entry << ") = " << c_sum << " +/- " << TMath::Sqrt(c_sumw2) << std::endl;
		}
		double c_sum = 0.;
		double c_sumw2 = 0.;
		if (m_background_histograms_list_.size() > 0) {
			for (int bin = 0; bin <= m_hist_total_background_->GetNbinsX() + 1; ++bin) {
				c_sum += m_hist_total_background_->GetBinContent(bin);
				c_sumw2 += TMath::Power(m_hist_total_background_->GetBinError(bin), 2);
			}
		}
		//std::cout << "[DataBackgroundCanvas] INFO : Background integral = " << c_sum << " +/- " << TMath::Sqrt(c_sumw2) << std::endl;
		this->cd();
	}

	double DataBackgroundCanvas::GetBackgroundIntegral() {
		if (!m_hist_total_background_) {
			std::cerr << "[DataBackgroundCanvas] WARNING : GetBackgroundIntegral() called, but m_hist_total_background_ is null. Returning 0." << std::endl;
			return 0;
		} else {
			return m_hist_total_background_->Integral(0, m_hist_total_background_->GetNbinsX() + 1);	
		}
	}

	double DataBackgroundCanvas::GetBackgroundIntegralError() {
		if (!m_hist_total_background_) {
			std::cerr << "[DataBackgroundCanvas] WARNING : GetBackgroundIntegral() called, but m_hist_total_background_ is null. Returning 0." << std::endl;
			return 0;
		} else {
			double c_sumw2 = 0.;
			for (int bin = 0; bin <= m_hist_total_background_->GetNbinsX() + 1; ++bin) {
				c_sumw2 += TMath::Power(m_hist_total_background_->GetBinError(bin), 2);
			}
			return TMath::Sqrt(c_sumw2);
		}
	}

	double DataBackgroundCanvas::GetDataIntegral(TString p_process) {
		if (m_data_histograms_.find(p_process) == m_data_histograms_.end()) {
			std::cerr << "[DataBackgroundCanvas] WARNING : In GetDataIntegral(), requested histogram tag " << p_process << " was not found. Returning 0." << std::endl;
			return 0.;
		}
		if (!(m_data_histograms_[p_process]->hist)) {
			std::cerr << "[DataBackgroundCanvas] WARNING : In GetDataIntegral(" << p_process << "), m_data_histograms_[p_process]->hist==0. Returning 0." << std::endl;
			return 0.;
		}
		return m_data_histograms_[p_process]->hist->Integral(0, m_data_histograms_[p_process]->hist->GetNbinsX() + 1);
	}

	double DataBackgroundCanvas::GetDataIntegralError(TString p_process) {
		if (m_data_histograms_.find(p_process) == m_data_histograms_.end()) {
			std::cerr << "[DataBackgroundCanvas] WARNING : In GetDataIntegral(), requested histogram tag " << p_process << " was not found." << std::endl;
			return -1.;
		}
		double c_sumw2 = 0.;
		for (int bin = 0; bin <= m_data_histograms_[p_process]->hist->GetNbinsX() + 1; ++bin) {
			c_sumw2 += TMath::Power(m_data_histograms_[p_process]->hist->GetBinError(bin), 2);
		}
		return TMath::Sqrt(c_sumw2);
	}

}

#endif