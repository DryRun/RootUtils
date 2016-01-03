#ifndef SystematicsCanvas_cxx
#define SystematicsCanvas_cxx

#include "RootUtils/SystematicsCanvas.h"

namespace Root {

	SystematicsCanvas::SystematicsCanvas() : DryCanvas() {
		CommonInit();
	}

	SystematicsCanvas::SystematicsCanvas(const char* name, const char* title, Int_t ww, Int_t wh) : DryCanvas(name, title, ww, wh) {
		CommonInit();
	}

	void SystematicsCanvas::CommonInit() {
		m_yaxis_top_customtitle = false;
		m_yaxis_bottom_customtitle = false;
		m_yaxis_top_customlimits = false;
		m_yaxis_bottom_customlimits = false;
		m_xaxis_logx = false;
	}

	void SystematicsCanvas::AddReferenceHistogram(TH1D *p_hist) {
		DryCanvas::AddHistogram(p_hist, "Reference", "Reference");
	}

	void SystematicsCanvas::AddHistograms(TString p_systematic_name, TH1D *p_syst_up, TH1D *p_syst_down) {
		DryCanvas::AddHistogram(p_syst_up, p_systematic_name + "Up", p_systematic_name + "#uparrow");
		DryCanvas::AddHistogram(p_syst_down, p_systematic_name + "Down", p_systematic_name + "#downarrow");
		m_systematics_list.push_back(p_systematic_name);
	}

	void SystematicsCanvas::MakeSummaryTGraphs() {
		// Please note that this method does a stupid combination of + and - fluctuations in quadrature. There is no real justification for this approach. Do it right!!
		tg_total_systematics = new TGraphAsymmErrors(m_histograms["Reference"]->GetNbinsX());
		tg_total_uncertainty = new TGraphAsymmErrors(m_histograms["Reference"]->GetNbinsX());
		tg_total_systematics_percentage = new TGraphAsymmErrors(m_histograms["Reference"]->GetNbinsX());
		tg_total_uncertainty_percentage = new TGraphAsymmErrors(m_histograms["Reference"]->GetNbinsX());

		for (int bin = 1; bin <= m_histograms["Reference"]->GetNbinsX(); ++bin) {
			double bin_center = m_histograms["Reference"]->GetXaxis()->GetBinCenter(bin);
			double bin_width = m_histograms["Reference"]->GetXaxis()->GetBinWidth(bin);
			double central_value = m_histograms["Reference"]->GetBinContent(bin);
			double statistical_error = m_histograms["Reference"]->GetBinError(bin);

			double fluctuation_up_squared = 0.;
			double fluctuation_down_squared = 0.;
			for (std::vector<TString>::iterator it = m_systematics_list.begin(); it != m_systematics_list.end(); ++it) {
				double value1 = m_histograms[*it + "Up"]->GetBinContent(bin);
				double value2 = m_histograms[*it + "Down"]->GetBinContent(bin);
				if (value1 > central_value) {
					fluctuation_up_squared += TMath::Power(value1 - central_value, 2);
					fluctuation_down_squared += TMath::Power(central_value - value2, 2);
				} else {
					fluctuation_down_squared += TMath::Power(central_value - value1, 2);
					fluctuation_up_squared += TMath::Power(value2 - central_value, 2);
				}
			}

			double fluctuation_up = TMath::Sqrt(TMath::Abs(fluctuation_up_squared));
			double fluctuation_down = TMath::Sqrt(TMath::Abs(fluctuation_down_squared));
			tg_total_systematics->SetPoint(bin - 1, bin_center, central_value);
			if (central_value != 0.) {
				tg_total_systematics->SetPointError(bin - 1, bin_width / 2., bin_width / 2., fluctuation_down, fluctuation_up);
			} else {
				tg_total_systematics->SetPointError(bin - 1, bin_width / 2., bin_width / 2., 0., 0.);
			}

			double total_uncertainty_up = TMath::Sqrt(fluctuation_up_squared + TMath::Power(statistical_error, 2));
			double total_uncertainty_down = TMath::Sqrt(fluctuation_down_squared + TMath::Power(statistical_error, 2));

			tg_total_uncertainty->SetPoint(bin - 1, bin_center, central_value);
			if (central_value != 0.) {
				tg_total_uncertainty->SetPointError(bin - 1, bin_width / 2., bin_width / 2., total_uncertainty_down, total_uncertainty_up);
			} else {
				tg_total_uncertainty->SetPointError(bin - 1, bin_width / 2., bin_width / 2., 0., 0.);
			}

			tg_total_systematics_percentage->SetPoint(bin - 1, bin_center, 0.);
			if (central_value != 0.) {
				tg_total_systematics_percentage->SetPointError(bin - 1, bin_width / 2., bin_width / 2., 100. * fluctuation_down / central_value, 100. * fluctuation_up / central_value);
			} else {
				tg_total_systematics_percentage->SetPointError(bin - 1, bin_width / 2., bin_width / 2., 0., 0.);
			}

			tg_total_uncertainty_percentage->SetPoint(bin - 1, bin_center, 0.);
			if (central_value != 0.) {
				tg_total_uncertainty_percentage->SetPointError(bin - 1, bin_width / 2., bin_width / 2., 100. * total_uncertainty_down / central_value, 100. * total_uncertainty_up / central_value);
			} else {
				tg_total_uncertainty_percentage->SetPointError(bin - 1, bin_width / 2., bin_width / 2., 0., 0.);
			}
		}
	}

	void SystematicsCanvas::MakeCanvas() {
		DryCanvas::MakeCanvas();

		// Make the total systematics TGraphAsymmErrors
		MakeSummaryTGraphs();

		// Draw TPads
		top = new TPad("top", "top", 0., 0.42, 0.8, 1.);
		top->SetBottomMargin(0.02);
		top->Draw();
		this->cd();
		bottom = new TPad("bottom", "bottom", 0., 0., 0.8, 0.42);
		bottom->SetTopMargin(0.01);
		bottom->SetBottomMargin(0.2);
		bottom->Draw();

		if (m_xaxis_logx) {
			top->SetLogx();
			bottom->SetLogx();
		}

		l = new TLegend(0.78, 0.2, 0.99, 0.85);
		l->SetFillColor(0);
		l->SetBorderSize(1);

		// Draw top canvas: All the histograms
		top->cd();
		tg_total_uncertainty->SetFillStyle(1001);
		tg_total_uncertainty->SetFillColor(kOrange+7);
		if (m_yaxis_top_customtitle) {
			tg_total_uncertainty->GetYaxis()->SetTitle(m_topytitle);
		}
		if (m_yaxis_top_customlimits) {
			tg_total_uncertainty->GetHistogram()->SetMinimum(p_top_ymin);
			tg_total_uncertainty->GetHistogram()->SetMaximum(p_top_ymax);
		}
		if (m_xaxis_customlimits) {
			tg_total_uncertainty->GetHistogram()->GetXaxis()->Set(100, m_xmin, m_xmax);
		}
		tg_total_uncertainty->GetXaxis()->SetLabelSize(0);
		tg_total_uncertainty->GetXaxis()->SetTitleSize(0);
		tg_total_uncertainty->Draw("a2");
		l->AddEntry(tg_total_uncertainty, "Total Uncertainty", "f");

		tg_total_systematics->SetFillStyle(1001);
		tg_total_systematics->SetFillColor(kGreen);
		tg_total_systematics->Draw("2");
		l->AddEntry(tg_total_systematics, "Total Systematic", "f");

		// Reference histogram: solid black line
		m_histograms["Reference"]->SetLineStyle(1);
		m_histograms["Reference"]->SetLineColor(kBlack);
		m_histograms["Reference"]->SetFillStyle(0);
		m_histograms["Reference"]->Draw("hist same");
		l->AddEntry(m_histograms["Reference"], "Reference", "l");

		Int_t counter = 1;
		if (m_systematics_list.size() > 1) { // If there's only one, then it's identical to tg_total_systematics.
			for (std::vector<TString>::iterator it = m_systematics_list.begin(); it != m_systematics_list.end(); ++it) {
				if ((*it) == TString("Reference")) continue;
				m_histograms[(*it) + "Up"]->SetLineStyle(2);
				m_histograms[(*it) + "Up"]->SetLineColor(colors[counter]);
				m_histograms[(*it) + "Up"]->Draw("hist same");
				m_histograms[(*it) + "Down"]->SetLineStyle(2);
				m_histograms[(*it) + "Down"]->SetLineColor(colors[counter]);
				m_histograms[(*it) + "Down"]->Draw("hist same");
				l->AddEntry(m_histograms[(*it) + "Up"], *it, "l");
				counter++;
			}
		}
		tg_total_uncertainty->GetHistogram()->Draw("axis same");

		// Bottom
		this->cd();
		bottom->cd();

		if (m_xaxis_customtitle) {
			tg_total_uncertainty_percentage->GetXaxis()->SetTitle(m_xtitle);
		}
		if (m_xaxis_customlimits) {
			tg_total_uncertainty_percentage->GetHistogram()->GetXaxis()->Set(100, m_xmin, m_xmax);
		}
		if (m_yaxis_bottom_customtitle) {
			tg_total_uncertainty_percentage->GetHistogram()->GetYaxis()->SetTitle(m_bottomytitle);
		}
		if (m_yaxis_bottom_customlimits) {
			tg_total_uncertainty_percentage->GetHistogram()->SetMinimum(m_bottom_ymin);
			tg_total_uncertainty_percentage->GetHistogram()->SetMaximum(m_bottom_ymax);
		}
		tg_total_uncertainty_percentage->SetFillStyle(1001);
		tg_total_uncertainty_percentage->SetFillColor(kOrange+7);
		tg_total_uncertainty_percentage->GetXaxis()->SetLabelSize(0.06);
		tg_total_uncertainty_percentage->GetXaxis()->SetTitleSize(0.06);
		tg_total_uncertainty_percentage->Draw("a2");

		tg_total_systematics_percentage->SetFillStyle(1001);
		tg_total_systematics_percentage->SetFillColor(kGreen);
		tg_total_systematics_percentage->Draw("2");

		tg_total_uncertainty_percentage->GetHistogram()->Draw("axis same");

		this->cd();
		l->Draw();
	}

	void SystematicsCanvas::SetTopYaxisTitle(TString p_ytitle) {
		m_yaxis_top_customtitle = true;
		m_topytitle = p_ytitle;
	}

	void SystematicsCanvas::SetBottomYaxisTitle(TString p_ytitle) {
		m_yaxis_bottom_customtitle = true;
		m_bottomytitle = p_ytitle;
	}

	void SystematicsCanvas::SetTopYaxisLimits(double p_ymin, double p_ymax) {
		m_yaxis_top_customlimits = true;
		p_top_ymin = p_ymin;
		p_top_ymax = p_ymax;
	}

	void SystematicsCanvas::SetBottomYaxisLimits(double p_ymin, double p_ymax) {
		m_yaxis_bottom_customlimits = true;
		m_bottom_ymin = p_ymin;
		m_bottom_ymax = p_ymax;
	}

	void SystematicsCanvas::PrintLatex(TString p_path) {
		std::ofstream m_latex_file(p_path.Data());

		if (!m_latex_file.is_open()) {
			std::cerr << "[SystematicsCanvas] ERROR : Couldn't open latex file for writing at " << p_path << std::endl;
			return;
		}

		m_latex_file << std::setiosflags(std::ios::fixed) << std::setprecision(2);
		if (!tg_total_uncertainty) {
			std::cerr << "[SystematicsCanvas] WARNING : PrintLatex() called before MakeCanvas(). Please call MakeCanvas() first." << std::endl;
		}
		m_latex_file << "\\begin{table}" << std::endl;
		m_latex_file << "\t\\centering" << std::endl;
		m_latex_file << "\t\\begin{tabular}{";
		for (int i = 0; i < m_systematics_list.size() + 3; i++) {
			m_latex_file << "|c";
		}
		m_latex_file << "|}" << std::endl;
		m_latex_file << "\t\t\\hline" << std::endl;
		m_latex_file << "\t\t Bin & Stat ";
		for (std::vector<TString>::iterator it = m_systematics_list.begin(); it != m_systematics_list.end(); ++it) {
			m_latex_file << " & " << (*it) << " ";
		}
		m_latex_file << " & Total \\\\" << std::endl << "\t\t\\hline" << std::endl;
		for (int bin = 1; bin <= m_histograms["Reference"]->GetNbinsX(); ++bin) {
			// Bin boundaries
			m_latex_file << "\t\t $" << m_histograms["Reference"]->GetXaxis()->GetBinLowEdge(bin) << "-" << m_histograms["Reference"]->GetXaxis()->GetBinUpEdge(bin) << "$ ";

			// Statistical error
			m_latex_file << " & $" << m_histograms["Reference"]->GetBinError(bin) / m_histograms["Reference"]->GetBinContent(bin) * 100 << "$ ";

			for (std::vector<TString>::iterator it = m_systematics_list.begin(); it != m_systematics_list.end(); ++it) {
				// Get the up and down percent errors
				double c_value_central = m_histograms["Reference"]->GetBinContent(bin);
				double c_value_up = m_histograms[(*it) + "Up"]->GetBinContent(bin);
				double c_value_down = m_histograms[(*it) + "Down"]->GetBinContent(bin);

				if (TMath::Abs(c_value_up - c_value_central) == TMath::Abs(c_value_central - c_value_down)) {
					// Case 1: up and down errors are the same.
					m_latex_file << " & $" << TMath::Abs(c_value_up - c_value_central) / c_value_central * 100. << "$ ";
				} else {
					// Case 2: up and down errors are different
					m_latex_file << " & $ \\begin{array}{c} ";
					m_latex_file << " +" << TMath::Abs(c_value_up - c_value_central) / c_value_central * 100. << " \\\\ ";
					m_latex_file << " -" << TMath::Abs(c_value_central - c_value_down) / c_value_central * 100. << " \\end{array} $";
				}
			}
			// Total
			double c_uncertainty_totalup = tg_total_uncertainty_percentage->GetErrorYhigh(bin-1);
			double c_uncertainty_totaldown = tg_total_uncertainty_percentage->GetErrorYlow(bin-1);
			if (TMath::Abs(c_uncertainty_totalup) == TMath::Abs(c_uncertainty_totaldown)) {
				m_latex_file << " & $ " << TMath::Abs(c_uncertainty_totalup) << " $ ";
			} else {
				m_latex_file << " & $ \\begin{array}{c} ";
				m_latex_file << " +" << c_uncertainty_totalup << " \\\\ " << " -" << c_uncertainty_totaldown;
				m_latex_file << " \\end{array} $ ";
			}
			m_latex_file << "\\\\" << std::endl << "\t\t\\hline" << std::endl;
		}
		m_latex_file << "\t\\end{tabular}" << std::endl;
		m_latex_file << "\\end{table}" << std::endl;
		m_latex_file.close();
	}

	void SystematicsCanvas::SetLogx() {
		m_xaxis_logx = true;
	}
}




#endif
