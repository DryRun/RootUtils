#include "MyTools/RootUtils/interface/RatioCanvas.h"

ClassImp (Root::RatioCanvas);

namespace Root {

	RatioCanvas::RatioCanvas() : DryCanvas() {
		CommonInit();
	}

	RatioCanvas::RatioCanvas(const char* name, const char* title, Int_t ww, Int_t wh) : DryCanvas(name, title, ww, wh) {

		plot_style = "Normal";

		CommonInit();
	}

	RatioCanvas::RatioCanvas(const char* name, const char* title, Int_t ww, Int_t wh, TString style) : DryCanvas(name, title, ww, wh) {

		plot_style = style;

		CommonInit();
	}

	RatioCanvas::~RatioCanvas() {
		if (m_ratio_unity_) {
			delete m_ratio_unity_;
		}
		m_ratio_fits_.clear();
	}

	void RatioCanvas::CommonInit() {
		c_ymin = -1.;
		c_ymax = -1.;
		rebin_factor = 1;
		ratio_title = "Ratio";
		debug_level = 0;
		m_use_common_denominator = false;
		normalization_bin_width = -1.;
		m_top_logy = true;
		m_manual_y_axis = false;
		m_draw_markers_ = true;
		m_ratio_unity_ = 0;
	}


	void RatioCanvas::AddHistograms(TString name, TH1D *h_num, TH1D *h_den) {
		AddHistograms(name, h_num, name + " N", h_den, name + " D");
	}

	void RatioCanvas::AddHistograms(TString name, TH1D *h_num, TString leg_num, TH1D *h_den, TString leg_den) {
		// Compute ratio
		TString ts_ratio_name = "h_ratio__"; ts_ratio_name += name;
		//TH1D *h_ratio_current = new TH1D(ts_ratio_name, ts_ratio_name, h_num->GetNbinsX(), h_num->GetXaxis()->GetXmin(), h_num->GetXaxis()->GetXmax());
		TH1D *h_ratio_current = (TH1D*)h_num->Clone();
		h_ratio_current->SetName(ts_ratio_name);
		h_ratio_current->SetTitle(ts_ratio_name);

		for (int bin = 1; bin <= h_ratio_current->GetXaxis()->GetNbins(); bin++) {
			if (h_den->GetBinContent(bin) > 0 && h_num->GetBinContent(bin) > 0) {
				double c_ratio = h_num->GetBinContent(bin) / h_den->GetBinContent(bin);
				double c_ratio_err = 0.;
				if (plot_style == "SingleDataBkg") {
					c_ratio_err = c_ratio * TMath::Sqrt(TMath::Power(h_num->GetBinError(bin) / h_num->GetBinContent(bin), 2) + TMath::Power(h_den->GetBinError(bin) / h_den->GetBinContent(bin), 2));
				} else {
					c_ratio_err = TMath::Sqrt(c_ratio * (1 + c_ratio)) * h_den->GetBinError(bin) / h_den->GetBinContent(bin);
				}
				h_ratio_current->SetBinContent(bin, c_ratio);
				h_ratio_current->SetBinError(bin, c_ratio_err);
			} else {
				h_ratio_current->SetBinContent(bin, 0);
				h_ratio_current->SetBinError(bin, 0);
			}
		}
		AddHistograms(name, h_num, leg_num, h_den, leg_den, h_ratio_current);
	}


	void RatioCanvas::AddHistograms(TString name, TH1D *h_num, TString leg_num, TH1D *h_den, TString leg_den, TH1D *h_ratio_in) {

		if (debug_level >= 1) {
			std::cout << "[RatioCanvas] DEBUG : AddHistograms() called with name = " << name << " / h_num = " << h_num->GetName() << " / h_den = " << h_den->GetName() << std::endl;
		}

		m_pair_definition_list.push_back(name);

		TH1D *h_num_copy = (TH1D*)h_num->Clone();
		TH1D *h_den_copy = (m_use_common_denominator ? (TH1D*)h_common_denominator->Clone() : (TH1D*)h_den->Clone());

		if (debug_level >= 2) {
			std::cout << "[RatioCanvas] DEBUG : \th_num->Integral() = " << h_num->Integral() << std::endl;
			std::cout << "[RatioCanvas] DEBUG : \th_den->Integral() = " << h_den->Integral() << std::endl;
		}

		h_num_copy->SetDirectory(0);
		h_num_copy->GetXaxis()->SetTitle(h_num->GetXaxis()->GetTitle());
		//h_num_copy->Sumw2();
		//h_num_copy->Rebin(rebin_factor);

		h_den_copy->SetDirectory(0);
		if (m_use_common_denominator) {
			h_den_copy->GetXaxis()->SetTitle(h_common_denominator->GetXaxis()->GetTitle());
		} else {
			h_den_copy->GetXaxis()->SetTitle(h_den->GetXaxis()->GetTitle());
		}
		//h_den_copy->Sumw2();
		//h_den_copy->Rebin(rebin_factor);

		h_object_counts[name] = std::make_pair(h_num_copy, h_den_copy);

		// Legend entries
		legend_entries[name] = std::make_pair(leg_num, leg_den);

		h_ratio[name] = (TH1D*)h_ratio_in->Clone();

		if (debug_level >= 2) {
			std::cout << "[RatioCanvas] DEBUG : Printing x axes of new histograms:" << std::endl;
			std::cout << "[RatioCanvas] DEBUG : Numerator = (" << h_object_counts[name].first->GetNbinsX() << ", " << h_object_counts[name].first->GetXaxis()->GetXmin() << ", " << h_object_counts[name].first->GetXaxis()->GetXmax() << ")" << std::endl;
			std::cout << "[RatioCanvas] DEBUG : Denominator = (" << h_object_counts[name].second->GetNbinsX() << ", " << h_object_counts[name].second->GetXaxis()->GetXmin() << ", " << h_object_counts[name].second->GetXaxis()->GetXmax() << ")" << std::endl;
			std::cout << "[RatioCanvas] DEBUG : Ratio = (" << h_ratio[name]->GetNbinsX() << ", " << h_ratio[name]->GetXaxis()->GetXmin() << ", " << h_ratio[name]->GetXaxis()->GetXmax() << ")" << std::endl;
		}

		//TH1D *h_ff = (TH1D*)h_num_copy->Clone();
		//if (!m_use_common_denominator) {
		//	h_ff->Divide(h_den_copy);
		//} else {
		//	h_ff->Divide(h_common_denominator);
		//}
		//TString ts_ffname = "h_ff_"; ts_ffname += name;
		//h_ff->SetName(ts_ffname);
		//h_ratio[name] = h_ff;

	}

	void RatioCanvas::AddStandaloneHistogram(TString name, TH1D *h, TString legend_entry, TString style) {
		m_standalone_histogram_names_.push_back(name);
		m_standalone_histograms_[name] = (TH1D*)h->Clone();
		m_standalone_histograms_[name]->SetDirectory(0);
		m_standalone_histogram_legends_[name] = legend_entry;;
		m_standalone_histogram_styles_[name] = style;
	
	}

	void RatioCanvas::SetRatioLimits(double c_min, double c_max) {

		if (debug_level >= 1) {
			std::cout << "[RatioCanvas] DEBUG : Setting ratio limits to (" << c_min << ", " << c_max << ")" << std::endl;
		}

		manual_ff_limits = true;
		c_ffmin = c_min;
		c_ffmax = c_max;

	}

	void RatioCanvas::SetRebinFactor(Int_t p_rebin_factor) {
		if (debug_level >= 1) {
			std::cout << "[RatioCanvas] DEBUG : Setting rebin factor to " << p_rebin_factor << std::endl;
		}

		rebin_factor = p_rebin_factor;
	}

	void RatioCanvas::MakeCanvas() {

		DryCanvas::MakeCanvas();

		if (debug_level >= 1) {
			std::cout << "[RatioCanvas] DEBUG : Called MakeCanvas()" << std::endl;
		}

		if (plot_style == TString("Normal")) {
			this->cd();
			top = new TPad(TString(this->GetName()) + "_top", TString(this->GetName()) + "_top", 0., 0.4, 1., 1.);
			top->SetBottomMargin(0.03);
			top->Draw();

			this->cd();
			bottom = new TPad(TString(this->GetName()) + "_bottom", TString(this->GetName()) + "_bottom", 0., 0., 1., 0.4);
			bottom->SetTopMargin(0.02);
			bottom->Draw();

			this->cd();

			if (m_manual_legend_) {
				l = new TLegend(m_legend_xmin_, m_legend_ymin_, m_legend_xmax_, m_legend_ymax_);
			} else {
				l = new TLegend(0.55, 0.74, 0.94, 0.94);
			}
			l->SetBorderSize(0);
			l->SetFillColor(0);

		} else if (plot_style == TString("ThreePanes")) {
			//this->SetRightMargin(0.2);

			this->cd();
			top = new TPad(TString(this->GetName()) + "_top", TString(this->GetName()) + "_top", 0., 0.5, 0.7, 1.);
			top->SetBottomMargin(0.03);
			top->Draw();

			this->cd();
			bottom = new TPad(TString(this->GetName()) + "_bottom", TString(this->GetName()) + "_bottom", 0., 0., 0.7, 0.5);
			bottom->SetTopMargin(0.02);
			bottom->Draw();

			this->cd();
			if (m_manual_legend_) {
				l = new TLegend(m_legend_xmin_, m_legend_ymin_, m_legend_xmax_, m_legend_ymax_);
			} else {
				l = new TLegend(0.7, 0.1, 0.99, 0.9);
			}
			l->SetBorderSize(1);
			l->SetFillColor(0);

		} else if (plot_style == TString("SingleDataBkg")) {
			this->cd();
			top = new TPad(TString(this->GetName()) + "_top", TString(this->GetName()) + "_top", 0., 0.5, 1., 1.);
			top->SetBottomMargin(0.03);
			top->Draw();

			this->cd();
			bottom = new TPad(TString(this->GetName()) + "_bottom", TString(this->GetName()) + "_bottom", 0., 0., 1., 0.5);
			bottom->SetTopMargin(0.02);
			bottom->Draw();

			this->cd();

			if (m_manual_legend_) {
				l = new TLegend(m_legend_xmin_, m_legend_ymin_, m_legend_xmax_, m_legend_ymax_);
			} else {
				l = new TLegend(0.7, 0.85, 0.95, 0.97);
			}
			l->SetBorderSize(1);
			l->SetFillColor(0);
		}

		//if (plot_style == TString("Normal")) {
		//	l->SetNColumns(m_pair_definition_list.size());
		//}

		// -- Top
		this->cd();
		top->cd();
		if (m_top_logy) top->SetLogy();
		// Make frame
		double c_left, c_right, c_top, c_bottom;
		c_top = -1e20;
		c_bottom = 1e20;
		c_left = 0.;
		c_right = 0.;
		for (std::vector<TString>::iterator it = m_pair_definition_list.begin(); it != m_pair_definition_list.end(); ++it) {
			if (it == m_pair_definition_list.begin()) {
				c_left = h_object_counts[*it].first->GetXaxis()->GetXmin();
				c_right = h_object_counts[*it].first->GetXaxis()->GetXmax();
			}
			if (h_object_counts[*it].first->GetMinimum() < c_bottom) c_bottom = h_object_counts[*it].first->GetMinimum();
			if (h_object_counts[*it].second->GetMinimum() < c_bottom) c_bottom = h_object_counts[*it].second->GetMinimum();
			if (h_object_counts[*it].first->GetMaximum() > c_top) c_top = h_object_counts[*it].first->GetMaximum();
			if (h_object_counts[*it].second->GetMaximum() > c_top) c_top = h_object_counts[*it].second->GetMaximum();
		}
		if (m_xaxis_customlimits) {
			c_left = m_xmin;
			c_right = m_xmax;
		}
		if (debug_level >= 2) {
			std::cout << "[RatioCanvas] DEBUG : Setting top frame y-axis limits to (" << (c_ymin > 0 ? c_ymin : 0.1) << ", " << (c_ymax> 0 ? c_ymax : c_top + 2 * (c_top - c_bottom)) << ")" << std::endl;
		}

		TH1D *h_frame_top = new TH1D("h_frame_top", "h_frame_top", 100, c_left, c_right);
		if (m_manual_y_axis) {
			std::cout << "[debug] Setting manual top Y axis: (" << c_ymin << ", " << c_ymax << ")" << std::endl;
			h_frame_top->SetMinimum(c_ymin);
			h_frame_top->SetMaximum(c_ymax);
		} else {
			if (m_top_logy) {
				h_frame_top->SetMinimum(c_ymin > 0 ? c_ymin : 0.1);
				h_frame_top->SetMaximum(c_ymax> 0 ? c_ymax : c_top + 8 * (c_top - c_bottom));
			} else {
				h_frame_top->SetMinimum(c_ymin > 0 ? c_ymin : 0.);
				h_frame_top->SetMaximum(c_ymax> 0 ? c_ymax : c_top + 0.3 * (c_top - c_bottom));
			}
		}
		h_frame_top->GetYaxis()->SetTitle(y_axis_title);
		h_frame_top->GetXaxis()->SetLabelSize(0.);
		h_frame_top->GetXaxis()->SetTitleSize(0.);
		h_frame_top->DrawCopy();

		Int_t counter = 0;
		for (std::vector<TString>::iterator it = m_pair_definition_list.begin(); it != m_pair_definition_list.end(); ++it) {

			// If normalization_bin_width has been set, do bin normalization now
			if (normalization_bin_width > 0) {
				for (int bin = 1; bin <= h_object_counts[*it].first->GetNbinsX(); bin++) {
					double bin_width = h_object_counts[*it].first->GetXaxis()->GetBinWidth(bin);
					double old_bin_content = h_object_counts[*it].first->GetBinContent(bin);
					double old_bin_error = h_object_counts[*it].first->GetBinError(bin);
					h_object_counts[*it].first->SetBinContent(bin, old_bin_content * normalization_bin_width / bin_width);
					h_object_counts[*it].first->SetBinError(bin, old_bin_error * normalization_bin_width / bin_width);
				}
				for (int bin = 1; bin <= h_object_counts[*it].second->GetNbinsX(); bin++) {
					double bin_width = h_object_counts[*it].second->GetXaxis()->GetBinWidth(bin);
					double old_bin_content = h_object_counts[*it].second->GetBinContent(bin);
					double old_bin_error = h_object_counts[*it].second->GetBinError(bin);
					h_object_counts[*it].second->SetBinContent(bin, old_bin_content * normalization_bin_width / bin_width);
					h_object_counts[*it].second->SetBinError(bin, old_bin_error * normalization_bin_width / bin_width);
				}
			}

			h_object_counts[*it].first->SetLineStyle(1);
			if (m_background_histograms_color_override_.find(*it) != m_background_histograms_color_override_.end()) {
				h_object_counts[*it].first->SetFillColor(m_background_histograms_color_override_[*it]);
				h_object_counts[*it].first->SetLineColor(m_background_histograms_color_override_[*it]);
				if (m_draw_markers_) {
					h_object_counts[*it].first->SetMarkerStyle(markers[counter]);
					h_object_counts[*it].first->SetMarkerColor(m_background_histograms_color_override_[*it]);
				} else {
					h_object_counts[*it].first->SetMarkerStyle(20);
					h_object_counts[*it].first->SetMarkerSize(0);
				}
			} else {
				if (m_pair_definition_list.size() <= 10) {
					h_object_counts[*it].first->SetFillColor(colors[counter]);
					h_object_counts[*it].first->SetLineColor(colors[counter]);
					if (m_draw_markers_) {
						h_object_counts[*it].first->SetMarkerStyle(markers[counter]);
						h_object_counts[*it].first->SetMarkerColor(colors[counter]);
					} else {
						h_object_counts[*it].first->SetMarkerStyle(20);
						h_object_counts[*it].first->SetMarkerSize(0);
					}
				} else {
					int index = distance(m_pair_definition_list.begin(), it);
					Float_t hue = 360. * index / m_pair_definition_list.size();
					Float_t saturation = 1.;
					Float_t brightness = 1.;
					Float_t rgb[3] = {1,0,0};
					TColor::HSV2RGB(hue, saturation, brightness, rgb[0], rgb[1], rgb[2]);
					TColor *color = gROOT->GetColor(index+10);
					color->SetRGB(rgb[0], rgb[1], rgb[2]);
					if (m_draw_markers_) {
						h_object_counts[*it].first->SetMarkerColor(index+10);
						h_object_counts[*it].first->SetMarkerStyle(20 + (index % 14));
					} else {
						h_object_counts[*it].first->SetMarkerStyle(20);
						h_object_counts[*it].first->SetMarkerSize(0);
					}
					h_object_counts[*it].first->SetLineColor(index+10);
					h_object_counts[*it].first->SetFillColor(index+10);
				}
			}

			h_object_counts[*it].first->Draw("hist same");
			if (m_draw_markers_) {
				l->AddEntry(h_object_counts[*it].first, legend_entries[*it].first, "pl");
			} else {
				l->AddEntry(h_object_counts[*it].first, legend_entries[*it].first, "l");
			}
			h_object_counts[*it].first->SetFillStyle(3001);
			h_object_counts[*it].first->DrawCopy("e2 same");
			h_object_counts[*it].first->SetFillStyle(0);

			if (debug_level >= 2) {
				std::cout << "[RatioCanvas] DEBUG : Drew histogram " << (h_object_counts[*it].first)->GetName() << " / legend entry " << *it + " N" << " (integral = " << (h_object_counts[*it].first)->Integral() << ")" << std::endl;
			}

			counter++;
		}

		counter = 0;
		if (!m_use_common_denominator) {
			for (std::vector<TString>::iterator it = m_pair_definition_list.begin(); it != m_pair_definition_list.end(); ++it) {
				h_object_counts[*it].second->SetLineStyle(2);

				if (m_background_histograms_color_override_.find(*it) != m_background_histograms_color_override_.end()) {
					std::cout << "[RatioCanvas] DEBUG : Setting manual color for process " << *it << std::endl;
					//h_object_counts[*it].second->SetFillColor(m_background_histograms_color_override_[*it]);
					h_object_counts[*it].second->SetLineColor(m_background_histograms_color_override_[*it]);
					h_object_counts[*it].second->SetMarkerStyle(markers[counter]);
					h_object_counts[*it].second->SetMarkerColor(m_background_histograms_color_override_[*it]);
				} else {
					if (m_pair_definition_list.size() <= 10) {
						//h_object_counts[*it].second->SetFillColor(colors[counter]);
						h_object_counts[*it].second->SetLineColor(colors[counter]);
						h_object_counts[*it].second->SetMarkerStyle(markers[counter]);
						h_object_counts[*it].second->SetMarkerColor(colors[counter]);
					} else {
						int index = distance(m_pair_definition_list.begin(), it);
						Float_t hue = 360. * index / m_pair_definition_list.size();
						Float_t saturation = 1.;
						Float_t brightness = 1.;
						Float_t rgb[3] = {1,0,0};
						TColor::HSV2RGB(hue, saturation, brightness, rgb[0], rgb[1], rgb[2]);
						TColor *color = gROOT->GetColor(index+10);
						color->SetRGB(rgb[0], rgb[1], rgb[2]);
						h_object_counts[*it].second->SetMarkerColor(index+10);
						h_object_counts[*it].second->SetMarkerStyle(20 + (index % 14));
						h_object_counts[*it].second->SetLineColor(index+10);
						//h_object_counts[*it].second->SetFillColor(index+10);
					}
				}

				h_object_counts[*it].second->DrawCopy("hist same");
				l->AddEntry(h_object_counts[*it].second, legend_entries[*it].second, "lp");
				h_object_counts[*it].second->SetFillStyle(3001);
				h_object_counts[*it].second->DrawCopy("e2 same");
				h_object_counts[*it].second->SetFillStyle(0);

				// Draw error band on denominator
				h_object_counts[*it].second->SetFillColor(kGray);
				h_object_counts[*it].second->SetFillStyle(3001);
				h_object_counts[*it].second->DrawCopy("e2 same");
				h_object_counts[*it].second->SetFillColor(0);
				h_object_counts[*it].second->SetFillStyle(0);

				if (debug_level >= 2) {
					std::cout << "[RatioCanvas] DEBUG : Drew histogram " << (h_object_counts[*it].second)->GetName() << " / legend entry " << *it + " D" << " (integral = " << (h_object_counts[*it].second)->Integral() << ")" << std::endl;
				}


				counter++;
			}
		} else {
			h_common_denominator->SetLineStyle(1);
			h_common_denominator->SetLineWidth(3);
			if (m_background_histograms_color_override_.find(m_common_denominator_name) != m_background_histograms_color_override_.end()) {
				h_common_denominator->SetLineColor(m_background_histograms_color_override_[m_common_denominator_name]);
			} else {
				h_common_denominator->SetLineColor(kBlack);
			}
			h_common_denominator->SetMarkerStyle(20);
			h_common_denominator->SetMarkerSize(0);
			l->AddEntry(h_common_denominator, m_common_denominator_name, "lp");
			h_common_denominator->SetFillStyle(3001);
			if (m_background_histograms_color_override_.find(m_common_denominator_name) != m_background_histograms_color_override_.end()) {
				h_common_denominator->SetFillColor(m_background_histograms_color_override_[m_common_denominator_name]);
			} else {
				h_common_denominator->SetFillColor(kGray);
			}
			h_common_denominator->DrawCopy("e2 same");
			h_common_denominator->SetFillStyle(0);
			h_common_denominator->DrawCopy("hist same");
		}
		// Redraw the histograms so the markers are on top of the fills
		for (std::vector<TString>::iterator it = m_pair_definition_list.begin(); it != m_pair_definition_list.end(); ++it) {
			h_object_counts[*it].first->Draw("hist same");
		}

		// Standalone histograms
		for (std::vector<TString>::iterator it = m_standalone_histogram_names_.begin(); it != m_standalone_histogram_names_.end(); ++it) {
			if (m_background_histograms_color_override_.find(*it) != m_background_histograms_color_override_.end()) {
				m_standalone_histograms_[*it]->SetFillColor(m_background_histograms_color_override_[*it]);
				m_standalone_histograms_[*it]->SetLineColor(m_background_histograms_color_override_[*it]);
				m_standalone_histograms_[*it]->SetMarkerColor(m_background_histograms_color_override_[*it]);
			}
			if (m_standalone_histogram_styles_[*it].Contains("p")) {
				m_standalone_histograms_[*it]->SetMarkerStyle(20);
				m_standalone_histograms_[*it]->SetMarkerSize(1);
			} else {
				m_standalone_histograms_[*it]->SetMarkerStyle(20);
				m_standalone_histograms_[*it]->SetMarkerSize(0);
			}
			m_standalone_histograms_[*it]->SetLineStyle(1);
			m_standalone_histograms_[*it]->SetLineWidth(1);
			if (m_background_histograms_fillstyle_override_.find(*it) != m_background_histograms_fillstyle_override_.end()) {
				m_standalone_histograms_[*it]->SetFillStyle(m_background_histograms_fillstyle_override_[*it]);
			}
			m_standalone_histograms_[*it]->Draw(m_standalone_histogram_styles_[*it] + " same");
			l->AddEntry(m_standalone_histograms_[*it], m_standalone_histogram_legends_[*it], "l");
		}

		if (plot_style == TString("Normal") || plot_style == TString("SingleDataBkg")) {
			this->cd();
			l->Draw();
		}

		// -- Bottom
		this->cd();
		bottom->cd();

		// Minimum/maximum y-axis of bottom plot
		c_top = -1e20;
		c_bottom = 1e20;

		if (manual_ff_limits) {
			c_top = c_ffmax;
			c_bottom = c_ffmin;
		} else {
			for (std::vector<TString>::iterator it = m_pair_definition_list.begin(); it != m_pair_definition_list.end(); ++it) {
				if (h_ratio[*it]->GetMinimum() < c_bottom) c_bottom = h_ratio[*it]->GetMinimum();
				if (h_ratio[*it]->GetMaximum() > c_top) c_top = h_ratio[*it]->GetMaximum();
			}
		}

		// If the x-axis title hasn't been specified manually, get it from the first histogram.
		if (!m_xaxis_customtitle) {
			m_xtitle = h_ratio[m_pair_definition_list[0]]->GetXaxis()->GetTitle();
		}

		TH1D *h_frame_bottom = new TH1D("h_frame_bottom", "h_frame_bottom", 100, c_left, c_right);
		if (debug_level >= 2) {
			std::cout << "[RatioCanvas] DEBUG : Setting ratio limits to (" << c_bottom << ", " << c_top << ")" << std::endl;
		}
		h_frame_bottom->SetMinimum(c_bottom);
		h_frame_bottom->SetMaximum(c_top);
		h_frame_bottom->GetXaxis()->SetTitle(m_xtitle);
		h_frame_bottom->GetYaxis()->SetTitle(ratio_title);
		h_frame_bottom->DrawCopy();

		m_ratio_unity_ = new TLine(c_left, 1., c_right, 1.);
		m_ratio_unity_->SetLineStyle(1);
		m_ratio_unity_->SetLineWidth(3);
		m_ratio_unity_->SetLineColor(kGray);
		m_ratio_unity_->Draw("same");

		counter = 0;
		for (std::vector<TString>::iterator it = m_pair_definition_list.begin(); it != m_pair_definition_list.end(); ++it) {
			h_ratio[*it]->SetMarkerStyle(20);
			h_ratio[*it]->SetMarkerSize(0);
			h_ratio[*it]->SetLineWidth(3);
			if (m_pair_definition_list.size() == 1) {
				h_ratio[*it]->SetLineColor(kBlack);
			} else if (m_background_histograms_color_override_.find(*it) != m_background_histograms_color_override_.end()) {
				//h_ratio[*it]->SetMarkerStyle(markers[counter]);
				//h_ratio[*it]->SetMarkerColor(m_background_histograms_color_override_[*it]);
				h_ratio[*it]->SetLineColor(m_background_histograms_color_override_[*it]);
			} else {
				if (m_pair_definition_list.size() <= 10) {
					//h_ratio[*it]->SetMarkerStyle(markers[counter]);
					//h_ratio[*it]->SetMarkerColor(colors[counter]);
					h_ratio[*it]->SetLineColor(colors[counter]);
				} else {
					int index = distance(m_pair_definition_list.begin(), it);
					Float_t hue = 360. * index / m_pair_definition_list.size();
					Float_t saturation = 1.;
					Float_t brightness = 1.;
					Float_t rgb[3] = {1,0,0};
					TColor::HSV2RGB(hue, saturation, brightness, rgb[0], rgb[1], rgb[2]);
					TColor *color = gROOT->GetColor(index+10);
					color->SetRGB(rgb[0], rgb[1], rgb[2]);
					//h_ratio[*it]->SetMarkerColor(index+10);
					//h_ratio[*it]->SetMarkerStyle(20 + (index % 14));
					h_ratio[*it]->SetLineColor(index+10);
				}
			}
			h_ratio[*it]->DrawCopy("hist same");

			// Ratio error band
			if (m_pair_definition_list.size() == 1) {
				h_ratio[*it]->SetFillColor(kGray);
			} else if (m_background_histograms_color_override_.find(*it) != m_background_histograms_color_override_.end()) {
				//h_ratio[*it]->SetMarkerStyle(markers[counter]);
				//h_ratio[*it]->SetMarkerColor(m_background_histograms_color_override_[*it]);
				h_ratio[*it]->SetFillColor(m_background_histograms_color_override_[*it]);
			} else {
				if (m_pair_definition_list.size() <= 10) {
					//h_ratio[*it]->SetMarkerStyle(markers[counter]);
					//h_ratio[*it]->SetMarkerColor(colors[counter]);
					h_ratio[*it]->SetFillColor(colors[counter]);
				} else {
					int index = distance(m_pair_definition_list.begin(), it);
					Float_t hue = 360. * index / m_pair_definition_list.size();
					Float_t saturation = 1.;
					Float_t brightness = 1.;
					Float_t rgb[3] = {1,0,0};
					TColor::HSV2RGB(hue, saturation, brightness, rgb[0], rgb[1], rgb[2]);
					TColor *color = gROOT->GetColor(index+10);
					color->SetRGB(rgb[0], rgb[1], rgb[2]);
					//h_ratio[*it]->SetMarkerColor(index+10);
					//h_ratio[*it]->SetMarkerStyle(20 + (index % 14));
					h_ratio[*it]->SetFillColor(index+10);
				}
			}
			h_ratio[*it]->SetFillStyle(3001);
			h_ratio[*it]->DrawCopy("e2 same");
			h_ratio[*it]->SetFillStyle(0);
			h_ratio[*it]->DrawCopy("hist same");

			// Fit, if it exists
			if (m_ratio_fits_.find(*it) != m_ratio_fits_.end()) {
				m_ratio_fits_[*it]->SetLineWidth(1);
				m_ratio_fits_[*it]->SetLineStyle(1);
				if (m_background_histograms_color_override_.find(*it) != m_background_histograms_color_override_.end()) {
					m_ratio_fits_[*it]->SetLineColor(m_background_histograms_color_override_[*it]);
				} else {
					if (m_pair_definition_list.size() <= 10) {
						m_ratio_fits_[*it]->SetLineColor(colors[counter]);
					} else {
						int index = distance(m_pair_definition_list.begin(), it);
						Float_t hue = 360. * index / m_pair_definition_list.size();
						Float_t saturation = 1.;
						Float_t brightness = 1.;
						Float_t rgb[3] = {1,0,0};
						TColor::HSV2RGB(hue, saturation, brightness, rgb[0], rgb[1], rgb[2]);
						TColor *color = gROOT->GetColor(index+10);
						color->SetRGB(rgb[0], rgb[1], rgb[2]);
						m_ratio_fits_[*it]->SetLineColor(index+10);
					}
				}
				m_ratio_fits_[*it]->Draw("same");
			}

			counter++;
			if (debug_level >= 2) {
				std::cout << "[RatioCanvas] DEBUG : Drew histogram " << h_ratio[*it]->GetName() << std::endl;
				Root::PrintHistogramContents(h_ratio[*it]);
			}
		}

		if (plot_style == TString("ThreePanes")) {
			this->cd();
			l->Draw();
		}

		h_frame_top->Delete();
		h_frame_bottom->Delete();
	}

	void RatioCanvas::SetYaxisTitle(TString p_title) {
		y_axis_title = p_title;
	}

	void RatioCanvas::SetRatioTitle(TString p_title) {
		ratio_title = p_title;
	}

	void RatioCanvas::SetYaxisLimits(double c_min, double c_max) {
		m_manual_y_axis = true;
		c_ymin = c_min;
		c_ymax = c_max;

	}

	void RatioCanvas::SetDebug(Int_t p_debug) {
		debug_level = p_debug;
	}

	TH1D* RatioCanvas::GetNumeratorHistogram(TString name) {

		bool name_found = false;
		for (std::vector<TString>::iterator it = m_pair_definition_list.begin(); it != m_pair_definition_list.end(); ++it) {
			if (name == *it) {
				name_found = true;
				break;
			}
		}
		if (!name_found) {
			std::cerr << "[RatioCanvas] WARNING : Could not find histogram with tag " << name << std::endl;
			return 0x0;
		}

		return (h_object_counts[name]).first;

	}

	TH1D* RatioCanvas::GetDenominatorHistogram(TString name) {

		bool name_found = false;
		for (std::vector<TString>::iterator it = m_pair_definition_list.begin(); it != m_pair_definition_list.end(); ++it) {
			if (name == *it) {
				name_found = true;
				break;
			}
		}
		if (!name_found) {
			std::cerr << "[RatioCanvas] WARNING : Could not find histogram with tag " << name << std::endl;
			return 0x0;
		}

		return (h_object_counts[name]).second;

	}

	TH1D* RatioCanvas::GetRatioHistogram(TString name) {

		bool name_found = false;
		for (std::vector<TString>::iterator it = m_pair_definition_list.begin(); it != m_pair_definition_list.end(); ++it) {
			if (name == *it) {
				name_found = true;
				break;
			}
		}
		if (!name_found) {
			std::cerr << "[RatioCanvas] WARNING : Could not find histogram with tag " << name << std::endl;
			return 0x0;
		}

		return h_ratio[name];

	}

	void RatioCanvas::SetCommonDenominator(TH1D *h_in, TString p_common_denominator_name) {
		std::cout << "[RatioCanvas] INFO : Using a common denominator: " << p_common_denominator_name << std::endl;
		m_use_common_denominator = true;
		h_common_denominator = (TH1D*)h_in->Clone();
		if (rebin_factor != 1) {
			h_common_denominator->Rebin(rebin_factor);
		}
		m_common_denominator_name = p_common_denominator_name;
	}


	void RatioCanvas::SetNormalizationBinWidth(double p_bin_width) {
		normalization_bin_width = p_bin_width;
	}

	void RatioCanvas::FitRatio(TString p_fit) {
		if (p_fit == "linear") {
			for (std::map<TString, TH1D*>::iterator it = h_ratio.begin(); it != h_ratio.end(); ++it) {
				m_ratio_fits_[(*it).first] = new TF1("fit_linear_" + (*it).first + "_" + this->GetName(), "[0] + [1]*x", (*it).second->GetXaxis()->GetXmin(), (*it).second->GetXaxis()->GetXmax());
				m_ratio_fits_[(*it).first]->SetParameter(0, (*it).second->GetMean());
				m_ratio_fits_[(*it).first]->SetParameter(1, 0.);
				(*it).second->Fit(m_ratio_fits_[(*it).first], "QR0");
			}
		}
	}

	Int_t RatioCanvas::GetNHistogramPairs() {
		return h_object_counts.size();
	}


	void RatioCanvas::SetTopLogy(bool p_top_logy) {
		m_top_logy = p_top_logy;
	}

}
