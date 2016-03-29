#ifndef EnvelopeCanvas_cxx
#define EnvelopeCanvas_cxx

#include "MyTools/RootUtils/interface/EnvelopeCanvas.h"

namespace Root {

	EnvelopeCanvas::EnvelopeCanvas() : DryCanvas() {

	}

	EnvelopeCanvas::EnvelopeCanvas(const char* name, const char* title, Int_t ww, Int_t wh) : DryCanvas(name, title, ww, wh) {
		y_axis_title = "";
		m_top_customlimits = false;
		m_bottom_customlimits = false;
	}


	void EnvelopeCanvas::AddReferenceHistogram(TH1D *p_histogram, TString p_legend_entry) {
		DryCanvas::AddHistogram(p_histogram, "Reference", p_legend_entry);
	}

	void EnvelopeCanvas::MakeCanvas() {
		// If no histograms are in the list, skip!
		if (m_histograms.size() == 0) {
			return;
		}

		DryCanvas::MakeCanvas();

		// Make legend
		l = new TLegend(0.7, 0.1, 0.99, 0.75);
		l->SetFillColor(0);
		l->SetBorderSize(1);

		// Style the histograms
		Int_t counter = 0;
		for (std::vector<TString>::iterator it = m_definition_list.begin(); it != m_definition_list.end(); ++it) {
			// The reference is styled as a line
			if (*it == TString("Reference")) {
				m_histograms[*it]->SetLineStyle(1);
				m_histograms[*it]->SetLineWidth(1);
				m_histograms[*it]->SetLineColor(kBlack);
				m_histograms[*it]->SetMarkerStyle(1);
				m_histograms[*it]->SetMarkerSize(0);
				continue;
			}

			// The rest of the histograms get styled manually: either according to the predefined list, or a continuous color spectrum if there are too many.
			if (m_definition_list.size() <= colors.size()) {
				m_histograms[*it]->SetLineStyle(1);
				m_histograms[*it]->SetLineColor(colors[counter]);
				m_histograms[*it]->SetMarkerStyle(markers[counter]);
				m_histograms[*it]->SetMarkerColor(colors[counter]);
				m_histograms[*it]->SetFillColor(colors[counter]);
				counter++;
			} else {
				Float_t hue = 360. * counter / m_histograms.size();
				Float_t saturation = 1.;
				Float_t brightness = 1.;
				Float_t rgb[3] = {1,0,0};
				TColor::HSV2RGB(hue, saturation, brightness, rgb[0], rgb[1], rgb[2]);
				TColor *color = gROOT->GetColor(counter+10);
				color->SetRGB(rgb[0], rgb[1], rgb[2]);
				m_histograms[*it]->SetMarkerColor(counter+10);
				m_histograms[*it]->SetMarkerStyle(20 + (counter % 14));
				m_histograms[*it]->SetLineColor(counter+10);
				m_histograms[*it]->SetFillColor(counter+10);
				counter++;
			}
		}

		// Make envelope histogram
		TGraphAsymmErrors *tg_envelope = new TGraphAsymmErrors(m_histograms["Reference"]->GetNbinsX());
		double m_global_max = -10000000000.;
		double m_global_min = 10000000000.;
		for (int bin = 1; bin <= m_histograms["Reference"]->GetNbinsX(); bin++) {
			double c_max = -10000000000.;
			double c_min = 10000000000.;
			for (std::map<TString, TH1D*>::iterator it = m_histograms.begin(); it != m_histograms.end(); ++it) {
				if ((*it).first == TString("Reference")) continue;
				double c_bin_content = (*it).second->GetBinContent(bin);
				if (c_bin_content > c_max) c_max = c_bin_content;
				if (c_bin_content < c_min) c_min = c_bin_content;
				if (c_bin_content > m_global_max) m_global_max = c_bin_content;
				if (c_bin_content < m_global_min) m_global_min = c_bin_content;
			}
			double c_reference = m_histograms["Reference"]->GetBinContent(bin);
			double c_bin_center = m_histograms["Reference"]->GetXaxis()->GetBinCenter(bin);
			double c_bin_halfwidth = 0.5 * m_histograms["Reference"]->GetXaxis()->GetBinWidth(bin);
			tg_envelope->SetPoint(bin - 1, c_bin_center, 0.);
			if (c_reference > 0) {
				tg_envelope->SetPointError(bin - 1, c_bin_halfwidth, c_bin_halfwidth, 100 * TMath::Abs((c_reference - c_min) / c_reference), 100 * TMath::Abs((c_reference - c_max) / c_reference));
			} else {
				tg_envelope->SetPointError(bin - 1, c_bin_halfwidth, c_bin_halfwidth, 0., 0.);
			}
		}

		// Setup TPads
		TPad *c_top = new TPad("c_top", "c_top", 0., 0.3, 0.7, 1.);
		c_top->SetBottomMargin(0.02);
		c_top->Draw();
		this->cd();
		TPad *c_bottom = new TPad("c_bottom", "c_bottom", 0., 0., 0.7, 0.3);
		c_bottom->SetTopMargin(0.02);
		c_bottom->SetBottomMargin(0.25);
		c_bottom->Draw();
		this->cd();

		// Draw top: reference as a line, all others as points
		c_top->cd();
		TH1D *h_frame = new TH1D("frame", "frame", 100, m_histograms["Reference"]->GetXaxis()->GetXmin(), m_histograms["Reference"]->GetXaxis()->GetXmax());
		if (m_xaxis_customlimits) {
			h_frame->GetXaxis()->Set(100, m_xmin, m_xmax);
		}
		if (m_top_customlimits) {
			h_frame->SetMinimum(m_top_ymin);
			h_frame->SetMaximum(m_top_ymax);
		} else {
			h_frame->SetMinimum(m_global_min - 0.25 * (m_global_max - m_global_min));
			h_frame->SetMaximum(m_global_max + 0.25 * (m_global_max - m_global_min));
		}
		h_frame->GetXaxis()->SetLabelSize(0);
		if (y_axis_title == TString("")) {
			h_frame->GetYaxis()->SetTitle(m_histograms["Reference"]->GetYaxis()->GetTitle());
		} else {
			h_frame->GetYaxis()->SetTitle(y_axis_title);
		}
		h_frame->Draw();

		m_histograms["Reference"]->SetMarkerSize(0);
		m_histograms["Reference"]->DrawCopy("hist same");
		m_histograms["Reference"]->SetFillStyle(3001);
		m_histograms["Reference"]->SetFillColor(kBlack);
		m_histograms["Reference"]->DrawCopy("e2same");
		l->AddEntry(m_histograms["Reference"], m_legend_entries["Reference"], "l"); 
		for (std::vector<TString>::iterator it = m_definition_list.begin(); it != m_definition_list.end(); ++it) {
			if ((*it) == TString("Reference")) continue;
			m_histograms[*it]->Draw("p e1 same");
			l->AddEntry(m_histograms[*it], m_legend_entries[*it], "p");
		}

		// Draw bottom: envelope
		c_bottom->cd();
		tg_envelope->SetFillColor(kGreen + 3);
		tg_envelope->SetFillStyle(3001);
		tg_envelope->SetMarkerSize(0);
		tg_envelope->SetLineWidth(0);
		if (m_xaxis_customlimits) {
			tg_envelope->GetHistogram()->GetXaxis()->Set(100, m_xmin, m_xmax);
		}
		if (m_bottom_customlimits) {
			tg_envelope->SetMinimum(m_bottom_ymin);
			tg_envelope->SetMaximum(m_bottom_ymax);
		}
		if (!m_xaxis_customtitle) {
			tg_envelope->GetHistogram()->GetXaxis()->SetTitle(m_histograms["Reference"]->GetXaxis()->GetTitle());
		} else {
			tg_envelope->GetHistogram()->GetXaxis()->SetTitle(m_xtitle);
		}
		tg_envelope->GetHistogram()->GetXaxis()->SetLabelSize(0.08);
		tg_envelope->GetHistogram()->GetXaxis()->SetTitleSize(0.08);
		tg_envelope->GetHistogram()->GetYaxis()->SetTitle("Deviation from reference (%)");
		tg_envelope->GetHistogram()->GetYaxis()->SetLabelSize(0.06);
		tg_envelope->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
		tg_envelope->Draw("a2f");
		tg_envelope->GetHistogram()->Draw("axis same");

		// Draw line at 0
		TLine *l_unity;
		if (m_xaxis_customlimits) {
			l_unity = new TLine(m_xmin, 0, m_xmax, 0);
		} else {
			l_unity = new TLine(h_frame->GetXaxis()->GetXmin(), 0, h_frame->GetXaxis()->GetXmax(), 0);
		}
		l_unity->SetLineColor(kRed - 3);
		l_unity->SetLineStyle(2);
		l_unity->Draw("same");

		// Draw legend
		this->cd();
		l->Draw();
	}

	void EnvelopeCanvas::SetYaxisTitle(TString p_title) {
		y_axis_title = p_title;
	}
	
	void EnvelopeCanvas::SetUpperPlotLimits(double p_ymin, double p_ymax) {
		m_top_customlimits = true;
		m_top_ymin = p_ymin;
		m_top_ymax = p_ymax;
	}

	void EnvelopeCanvas::SetBottomPlotLimits(double p_ymin, double p_ymax) {
		m_bottom_customlimits = true;
		m_bottom_ymin = p_ymin;
		m_bottom_ymax = p_ymax;
	}

}

#endif