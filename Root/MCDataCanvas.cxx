#ifndef MCDataCanvas_cxx
#define MCDataCanvas_cxx

#include "RootUtils/MCDataCanvas.h"

namespace Root {

	bool CompareHistogramSortingContainers(MCDataCanvas::HistogramSortingContainer *hsc1, MCDataCanvas::HistogramSortingContainer *hsc2) {
		return (hsc1->hist->Integral() < hsc2->hist->Integral());
	}

	MCDataCanvas::MCDataCanvas() : DryCanvas() {

	}

	MCDataCanvas::MCDataCanvas(const char* name, const char* title, Int_t ww, Int_t wh) : DryCanvas(name, title, ww, wh) {

		// Initialize options
		y_axis_title             = "";
		m_yaxis_customtitle      = false;
		m_manual_ratio_limits    = false;
		m_invert_ratio           = false;
		m_yaxis_top_customlimits = false;
		m_top_logy               = false;
		m_bottom_logy            = false;
		m_yaxis_top_scalelimits  = false;
		m_yaxis_top_scale_upper  = 1;
		m_yaxis_top_scale_lower  = 1;
	}

	MCDataCanvas::~MCDataCanvas() {
		if (h_frame != 0x0) {
			//delete h_frame;
		}
		for (std::vector<HistogramSortingContainer*>::iterator it = MCHistogramList.begin(); it != MCHistogramList.end(); ++it) {
			delete (*it);
		}
	}


	void MCDataCanvas::AddDataHistogram(TH1D *p_histogram, TString p_legend_entry, double p_luminosity) {
		//std::cout << "[MCDataCanvas] INFO : Adding data histogram with luminosity " << p_luminosity << std::endl;
		DryCanvas::AddHistogram(p_histogram, "Data", p_legend_entry);
		m_data_luminosity = p_luminosity;
		m_data_legend_entry = p_legend_entry;
	}

	void MCDataCanvas::AddMCHistogram(TH1D *p_histogram, TString p_process, TString p_legend_entry, double p_luminosity) {
		//h_mc[p_process] = (TH1D*)p_histogram->Clone();
		//h_mc[p_process]->SetDirectory(0);
		//m_mc_legend_entries[p_process] = p_legend_entry;
		//m_mc_luminosities[p_process] = p_luminosity;
		//m_mc_process_list.push_back(p_process);

		if (p_histogram->Integral() > 0) {

			HistogramSortingContainer *hsc = new HistogramSortingContainer();
			hsc->hist = (TH1D*)p_histogram->Clone();
			hsc->hist->SetDirectory(0);
			hsc->legend_entry = p_legend_entry;
			hsc->luminosity = p_luminosity;
			hsc->process = p_process;

			MCHistogramMap[p_process] = hsc;
			MCHistogramList.push_back(hsc);

			DryCanvas::AddHistogram(hsc->hist, p_process, p_legend_entry);

			//std::cout << "[MCDataCanvas] INFO : Added MC histogram for process " << p_process << " with luminosity=" << p_luminosity << " / integral=" << p_histogram->Integral() << std::endl;
		}
	}

	void MCDataCanvas::MakeCanvas() {
		// If no histograms are in the list, skip!
		if (MCHistogramList.size() == 0) {
			std::cerr << "[MCDataCanvas] ERROR : No MC histograms were loaded! Doing nothing." << std::endl;
			m_canvas_drawn = false;
			return;
		}
		if (m_histograms.find("Data") == m_histograms.end()) {
			std::cerr << "[MCDataCanvas] ERROR : No data histogram was loaded! Doing nothing." << std::endl;
			m_canvas_drawn = false;
			return;
		}

		DryCanvas::MakeCanvas();

		// Setup TPads
		c_top = new TPad("c_top", "c_top", 0., 0.3, 0.7, 1.);
		c_top->SetBottomMargin(0.02);
		c_top->Draw();
		this->cd();
		c_bottom = new TPad("c_bottom", "c_bottom", 0., 0., 0.7, 0.3);
		c_bottom->SetTopMargin(0.02);
		c_bottom->SetBottomMargin(0.25);
		c_bottom->Draw();
		this->cd();

		c_top->SetLogy(m_top_logy ? 1 : 0);
		c_bottom->SetLogy(m_bottom_logy ? 1 : 0);

		// Normalize MC histograms
		for (std::vector<HistogramSortingContainer*>::iterator it = MCHistogramList.begin(); it != MCHistogramList.end(); ++it) {
			(*it)->hist->Scale(m_data_luminosity / (*it)->luminosity);
		}

		// Sort the MC process list by histogram integral
		std::sort(MCHistogramList.begin(), MCHistogramList.end(), CompareHistogramSortingContainers);

		// Style the histograms
		m_histograms["Data"]->SetMarkerStyle(20);
		m_histograms["Data"]->SetMarkerColor(kBlack);
		m_histograms["Data"]->SetMarkerSize(1);

		Int_t counter = 0;
		for (std::vector<HistogramSortingContainer*>::iterator it = MCHistogramList.begin(); it != MCHistogramList.end(); ++it) {
			// Style colors
			if (m_background_histograms_color_override_.find((*it)->process) != m_background_histograms_color_override_.end()) {
				(*it)->hist->SetLineColor(m_background_histograms_color_override_[(*it)->process]);
				(*it)->hist->SetMarkerColor(m_background_histograms_color_override_[(*it)->process]);
				(*it)->hist->SetFillColor(m_background_histograms_color_override_[(*it)->process]);
				(*it)->hist->SetMarkerStyle(markers[counter]);
			} else if (MCHistogramList.size() <= colors.size()) {
				(*it)->hist->SetLineColor(colors[counter]);
				(*it)->hist->SetMarkerColor(colors[counter]);
				(*it)->hist->SetFillColor(colors[counter]);
				(*it)->hist->SetMarkerStyle(markers[counter]);
				counter++;
			} else {
				Float_t hue = 360. * counter / MCHistogramList.size();
				Float_t saturation = 1.;
				Float_t brightness = 1.;
				Float_t rgb[3] = {1,0,0};
				TColor::HSV2RGB(hue, saturation, brightness, rgb[0], rgb[1], rgb[2]);
				TColor *color = gROOT->GetColor(counter+10);
				color->SetRGB(rgb[0], rgb[1], rgb[2]);
				(*it)->hist->SetMarkerColor(counter+10);
				(*it)->hist->SetMarkerStyle(20 + (counter % 14));
				(*it)->hist->SetLineColor(counter+10);
				(*it)->hist->SetFillColor(counter+10);
				counter++;
			}
			(*it)->hist->SetLineStyle(1);
		}

		// Make the THStack and sum histogram
		THStack *hs_mc = new THStack("hs_mc_mc", "MC");
		TH1D *h_mc_sum;
		for (std::vector<HistogramSortingContainer*>::iterator it = MCHistogramList.begin(); it != MCHistogramList.end(); ++it) {
			hs_mc->Add((*it)->hist);
			if (it == MCHistogramList.begin()) {
				h_mc_sum = (TH1D*)(*it)->hist->Clone();
				h_mc_sum->SetName("h_mc_sum");
				h_mc_sum->Sumw2();
			} else {
				h_mc_sum->Add((*it)->hist);
			}
		}

		// Format THStack
		//if (y_axis_title == TString("")) {
		//	hs_mc->GetXaxis()->SetTitle(m_histograms["Data"]->GetXaxis()->GetTitle());
		//} else {
		//	hs_mc->GetXaxis()->SetTitle(y_axis_title);
		//}

		// Draw top: data and MC overlaid
		c_top->cd();
		h_frame = new TH1D("frame", "frame", 100, m_histograms["Data"]->GetXaxis()->GetXmin(), m_histograms["Data"]->GetXaxis()->GetXmax());
		if (m_xaxis_customlimits) {
			h_frame->GetXaxis()->Set(100, m_xmin, m_xmax);
		}
		if (m_yaxis_customtitle) {
			h_frame->GetYaxis()->SetTitle(y_axis_title);
		}
		//h_frame->SetMinimum(1);
		if (m_yaxis_top_customlimits) { 
			h_frame->SetMinimum(m_top_ymin);
			h_frame->SetMaximum(m_top_ymax);
		} else {
			if (m_histograms["Data"]->GetMaximum() > hs_mc->GetMaximum()) {
				m_top_ymax = m_histograms["Data"]->GetMaximum() * 1.75;
				h_frame->SetMaximum(m_top_ymax);
			} else {
				m_top_ymax = hs_mc->GetMaximum() * 1.75;
				h_frame->SetMaximum(m_top_ymax);
			}
		}
		if (m_yaxis_top_scalelimits) {
			double old_min = h_frame->GetMinimum();
			double old_max = h_frame->GetMaximum();
			double new_min = old_min + (old_max - old_min) * m_yaxis_top_scale_lower;
			double new_max = old_min + (old_max - old_min) * m_yaxis_top_scale_upper;
			h_frame->SetMinimum(new_min);
			h_frame->SetMaximum(new_max);
		}

		h_frame->GetXaxis()->SetLabelSize(0);
		h_frame->Draw();
		hs_mc->Draw("hist same");
		// Make MC uncertainty band
		h_mc_sum->SetMarkerSize(0);
		h_mc_sum->SetFillStyle(3001);
		h_mc_sum->SetFillColor(kGray+2);
		h_mc_sum->DrawCopy("e2 same");

		m_histograms["Data"]->SetMarkerStyle(20);
		m_histograms["Data"]->SetMarkerSize(1);
		m_histograms["Data"]->SetLineWidth(1);
		m_histograms["Data"]->SetLineStyle(1);
		m_histograms["Data"]->Draw("p e1 same");
		// Redraw frame
		h_frame->Draw("axis same");

		this->cd();

		// Make the ratio histogram
		if (!m_invert_ratio) {
			m_histograms["DataOverMC"] = (TH1D*)m_histograms["Data"]->Clone();
			m_histograms["DataOverMC"]->SetName("h_DataOverMC");
			m_histograms["DataOverMC"]->Divide(h_mc_sum);
		} else {
			m_histograms["DataOverMC"] = (TH1D*)h_mc_sum->Clone();
			m_histograms["DataOverMC"]->SetName("h_DataOverMC");
			m_histograms["DataOverMC"]->Divide(m_histograms["Data"]);
		}
		m_histograms["DataOverMC"]->SetLineColor(kBlack);
		m_histograms["DataOverMC"]->SetLineWidth(1);
		m_histograms["DataOverMC"]->SetLineStyle(1);
		if (!m_invert_ratio) {
			m_histograms["DataOverMC"]->GetYaxis()->SetTitle("#frac{Data}{MC}");
		} else {
			m_histograms["DataOverMC"]->GetYaxis()->SetTitle("#frac{MC}{Data}");
		}
		m_histograms["DataOverMC"]->GetYaxis()->SetTitleSize(0.08);
		m_histograms["DataOverMC"]->GetYaxis()->SetLabelSize(0.06);
		m_histograms["DataOverMC"]->GetYaxis()->SetTitleOffset(0.6);
		if (!m_xaxis_customtitle) {
			m_histograms["DataOverMC"]->GetXaxis()->SetTitle(m_histograms["Data"]->GetXaxis()->GetTitle());
		} else {
			m_histograms["DataOverMC"]->GetXaxis()->SetTitle(m_xtitle);
		}

		// Draw bottom: ratio
		c_bottom->cd();
		m_histograms["DataOverMC"]->GetXaxis()->SetTitleSize(0.1);
		m_histograms["DataOverMC"]->GetXaxis()->SetLabelSize(0.1);
		m_histograms["DataOverMC"]->GetXaxis()->SetTitleOffset(0.8);
		if (m_manual_ratio_limits) {
			m_histograms["DataOverMC"]->SetMinimum(m_ratio_min);
			m_histograms["DataOverMC"]->SetMaximum(m_ratio_max);
		}
		m_histograms["DataOverMC"]->Draw();

		// Draw line at 1
		TLine *l_unity = new TLine(m_histograms["DataOverMC"]->GetXaxis()->GetXmin(), 1, m_histograms["DataOverMC"]->GetXaxis()->GetXmax(), 1);
		l_unity->SetLineColor(kRed - 3);
		l_unity->SetLineStyle(2);
		l_unity->Draw("same");
		m_histograms["DataOverMC"]->Draw("same");

		this->cd();

		// Make legend
		l = new Root::DryLegend(0.7, 0.2, 0.99, 0.8);
		l->SetFillColor(0);
		l->SetBorderSize(1);
		l->AddEntry(m_histograms["Data"], m_data_legend_entry, "p");
		for (std::vector<HistogramSortingContainer*>::iterator it = MCHistogramList.begin(); it != MCHistogramList.end(); ++it) {
			l->AddEntry((*it)->hist, (*it)->legend_entry, "f");
		}
		l->Draw();
	}

	void MCDataCanvas::SetYaxisTitle(TString p_title) {
		y_axis_title = p_title;
		m_yaxis_customtitle = true;
	}
	void MCDataCanvas::SetTopLogy(bool p_top_log) {
		m_top_logy = p_top_log;
		//if (c_top) {
		//	c_top->SetLogy(p_top_log ? 1 : 0);
		//}
	}
	void MCDataCanvas::SetBottomLogy(bool p_bottom_log) {
		m_bottom_logy = p_bottom_log;
		//if (c_bottom) {
		//	c_bottom->SetLogy(p_bottom_log ? 1 : 0);
		//}
	}

	void MCDataCanvas::SetRatioLimits(double p_min, double p_max) {
		m_manual_ratio_limits = true;
		m_ratio_min = p_min;
		m_ratio_max = p_max;
	}

	void MCDataCanvas::InvertRatio(bool p_invert_ratio) {
		m_invert_ratio = p_invert_ratio;
	}

	void MCDataCanvas::SetTopYaxisLimits(double p_min, double p_max) {
		m_yaxis_top_customlimits = true;
		m_top_ymin = p_min;
		m_top_ymax = p_max;
	}

	std::pair<double, double> MCDataCanvas::GetTopYaxisLimits() {
		return std::make_pair(m_top_ymin, m_top_ymax);
	}
	
	void MCDataCanvas::ScaleTopYaxisLimits(double p_yaxis_top_scale_lower, double p_yaxis_top_scale_upper) {
		if (p_yaxis_top_scale_upper < p_yaxis_top_scale_lower) {
			std::cerr << "[MCDataCanvas] WARNING : In ScaleTopYaxisLimits, require arg1 < arg2. Doing nothing." << std::endl;
		} else {
			m_yaxis_top_scalelimits = true;
			m_yaxis_top_scale_upper = p_yaxis_top_scale_upper;
			m_yaxis_top_scale_lower = p_yaxis_top_scale_lower;
		}
	}

}

#endif