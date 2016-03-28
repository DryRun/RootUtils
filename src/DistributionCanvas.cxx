#include "MyTools/RootUtils/interface/DistributionCanvas.h"

ClassImp (Root::DistributionCanvas);

namespace Root {

	DistributionCanvas::DistributionCanvas() : DryCanvas() {

	}

	DistributionCanvas::DistributionCanvas(const char* name, const char* title, Int_t ww, Int_t wh) : DryCanvas(name, title, ww, wh) {

		do_stacked = false;

		m_normalize_histograms = true;

		m_log_x = false;
		m_log_y = false;

		m_draw_histogram_integrals = false;

		m_manual_y_axis_title = false;

		m_yaxis_customlimits = false;

		m_histogram_paint_styles[Points] = false;
		m_histogram_paint_styles[Lines] = false;
		m_histogram_paint_styles[Fill] = false;
	}

	void DistributionCanvas::AddHistogram(TH1D *p_histogram, TString p_name, TString p_legend_entry) {
		histogram_integrals[p_name] = p_histogram->Integral();
		DryCanvas::AddHistogram(p_histogram, p_name, p_legend_entry);
	}

	void DistributionCanvas::DoNormalizeHistograms() {
		if (!m_normalize_histograms) return;
		if (do_stacked) return;

		for (std::vector<TString>::iterator it = m_definition_list.begin(); it != m_definition_list.end(); ++it) {
			m_histograms[*it]->Scale(1. / m_histograms[*it]->Integral());
		}
	}

	void DistributionCanvas::MakeCanvas() {
		DryCanvas::MakeCanvas();
		if (m_normalize_histograms) {
			DoNormalizeHistograms();
		}
		if (m_draw_histogram_integrals) {
			for (std::vector<TString>::iterator it = m_definition_list.begin(); it != m_definition_list.end(); ++it) {
				TString new_legend_entry = DryCanvas::GetLegendEntry(*it);
				new_legend_entry += " (";
				new_legend_entry += histogram_integrals[*it]; 
				new_legend_entry += ")";
				SetLegendEntry(*it, new_legend_entry);
			}
		}

		if (m_definition_list.size() == 0) {
			std::cerr << "[DistributionCanvas] ERROR : No m_histograms in list for DistributionCanvas " << this->GetName() << ". Canvas will not be drawn." << std::endl;
			m_canvas_drawn = false;
			return;
		}

		if (m_log_y) {
			this->SetLogy();
		}
		if (m_log_x) {
			this->SetLogx();
		}

		#ifdef DEBUG_DC
		std::cout << "[DistributionCanvas] DEBUG : In MakeCanvas()" << std::endl;
		#endif

		this->SetRightMargin(0.26);

		l = new TLegend(0.75, 0.18, 0.99, 0.75);
		l->SetBorderSize(1);
		l->SetFillColor(0);
		//l->SetNColumns(4);

		// Make frame
		#ifdef DEBUG_DC
		std::cout << "[DistributionCanvas] DEBUG : Making frames" << std::endl;
		#endif

		TH1D *h_frame = 0;
		THStack *h_stack = new THStack("hs", "hstack");
		if (!do_stacked) {
			double c_left = 0.;
			double c_right = 0;
			double c_top = -1e20;
			double c_bottom = 1e20;
			for (std::vector<TString>::iterator it = m_definition_list.begin(); it != m_definition_list.end(); ++it) {
				if (it == m_definition_list.begin()) {
					c_left = m_histograms[*it]->GetXaxis()->GetXmin();
					c_right = m_histograms[*it]->GetXaxis()->GetXmax();
					if (!m_xaxis_customtitle) m_xtitle = m_histograms[*it]->GetXaxis()->GetTitle();
					if (y_axis_title == TString("")) y_axis_title = m_histograms[*it]->GetXaxis()->GetTitle();
				}
				if (m_histograms[*it]->GetMinimum() < c_bottom) c_bottom = m_histograms[*it]->GetMinimum();
				if (m_histograms[*it]->GetMinimum() < c_bottom) c_bottom = m_histograms[*it]->GetMinimum();
				if (m_histograms[*it]->GetMaximum() > c_top) c_top = m_histograms[*it]->GetMaximum();
				if (m_histograms[*it]->GetMaximum() > c_top) c_top = m_histograms[*it]->GetMaximum();
			}
			if (m_xaxis_customlimits) {
				c_left = m_xmin;
				c_right = m_xmax;
			}
			h_frame = new TH1D("h_frame", "h_frame", 100, c_left, c_right);
			//h_frame->SetMinimum((c_bottom > 0 ? c_bottom : 0.8));
			if (!m_yaxis_customlimits) {
				if (!m_log_y) {
					h_frame->SetMinimum(c_bottom - 0.25 * (c_top - c_bottom));
				}
				h_frame->SetMaximum(c_top + 0.25 * (c_top - c_bottom));
			} else {
				h_frame->SetMinimum(m_ymin);
				h_frame->SetMaximum(m_ymax);
			}
			h_frame->GetXaxis()->SetTitle(m_xtitle);
			if (m_manual_y_axis_title) {
				h_frame->GetYaxis()->SetTitle(y_axis_title);
			}
			if (m_manual_y_axis_title) {
				h_frame->GetYaxis()->SetTitle(y_axis_title);
			}
			h_frame->DrawCopy();
		} else {
			if (m_manual_y_axis_title) {
				h_stack->GetYaxis()->SetTitle(y_axis_title);
			}
		}

		#ifdef DEBUG_DC
		std::cout << "[DistributionCanvas] DEBUG : Drawing m_histograms" << std::endl;
		#endif

		// Draw histograms
		Int_t counter = 0;
		TString m_draw_style = "";
		for (std::map<HistogramStyle, bool>::iterator it = m_histogram_paint_styles.begin(); it != m_histogram_paint_styles.end(); ++it) {
			if (!(*it).second) continue;
			if ((*it).first == Points) {
				m_draw_style += "p";
			} else if ((*it).first == Lines) {
				m_draw_style += "hist";
			} else if ((*it).first == Fill) {
				m_draw_style += "f";
			}
		}
		if (m_draw_style.EqualTo("")) m_draw_style = "p";

		for (std::vector<TString>::iterator it = m_definition_list.begin(); it != m_definition_list.end(); ++it) {
			#ifdef DEBUG_DC
			std::cout << "[DistributionCanvas] DEBUG : \tHistogram " << *it << std::endl;
			#endif

			if (m_definition_list.size() <= colors.size()) {
				m_histograms[*it]->SetLineStyle(1);
				m_histograms[*it]->SetLineColor(colors[counter]);
				m_histograms[*it]->SetMarkerStyle(markers[counter]);
				m_histograms[*it]->SetMarkerColor(colors[counter]);
				m_histograms[*it]->SetFillColor(colors[counter]);
			} else {
				int index = distance(m_definition_list.begin(), it);
				Float_t hue = 360. * index / m_definition_list.size();
				Float_t saturation = 1.;
				Float_t brightness = 1.;
				Float_t rgb[3] = {1,0,0};
				TColor::HSV2RGB(hue, saturation, brightness, rgb[0], rgb[1], rgb[2]);
				TColor *color = gROOT->GetColor(index+10);
				color->SetRGB(rgb[0], rgb[1], rgb[2]);
				m_histograms[*it]->SetMarkerColor(index+10);
				m_histograms[*it]->SetMarkerStyle(20 + (index % 14));
				m_histograms[*it]->SetLineColor(index+10);
			}


			if (!do_stacked) {
				m_histograms[*it]->Draw(m_draw_style + "same");
			} else {
				h_stack->Add(m_histograms[*it]);
			}
			l->AddEntry(m_histograms[*it], m_legend_entries[*it], (do_stacked ? "f" : "lp"));

			counter++;
		}

		if (do_stacked) {
			std::vector<TString>::iterator it = m_definition_list.begin();
			TString x_axis_title = (m_xaxis_customtitle ? TString(m_xtitle) : TString(m_histograms[*it]->GetXaxis()->GetTitle()));
			h_stack->Draw("hist");
			h_stack->GetXaxis()->SetTitle(x_axis_title);
			h_stack->Draw("hist");
		}
		l->Draw();

		if (!do_stacked) {
			if (h_frame != 0x0) h_frame->Delete();
		}

		if (h_stack) {
			delete h_stack;
			h_stack = 0;
		}
	}

	void DistributionCanvas::SetYaxis(double p_ymin, double p_ymax) {
		m_yaxis_customlimits = true;
		m_ymin = p_ymin;
		m_ymax = p_ymax;
	}


	void DistributionCanvas::SetYaxisTitle(TString p_ytitle) {
		m_manual_y_axis_title = true;
		y_axis_title = p_ytitle;
	}

	void DistributionCanvas::NormalizeHistograms(bool p_normalize_histograms) {
		if (m_definition_list.size() > 0) {
			std::cerr << "[DistributionCanvas] ERROR : NormalizeHistograms() must be called before adding any histograms to the canvas." << std::endl;
			exit(1);
		}
		m_normalize_histograms = p_normalize_histograms;
	}

	void DistributionCanvas::SetLogY() {
		m_log_y = true;
	}

	void DistributionCanvas::SetLogX() {
		m_log_x = true;
	}

	void DistributionCanvas::DrawHistogramIntegrals() {
		m_draw_histogram_integrals = true;
	}
		
	void DistributionCanvas::AddPaintStyle(HistogramStyle p_style) {
		m_histogram_paint_styles[p_style] = true;
	}
	
	void DistributionCanvas::RemovePaintStyle(HistogramStyle p_style) {
		m_histogram_paint_styles[p_style] = false;
	}


}

