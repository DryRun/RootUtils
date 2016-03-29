#ifndef CommonFunctions_cxx
#define CommonFunctions_cxx

#include "MyTools/RootUtils/interface/CommonFunctions.h"

namespace Root {

	double GetInvariantMass(double m1, double e1, double eta1, double phi1, double m2, double e2, double eta2, double phi2) {
		double p1 = TMath::Sqrt(e1 * e1 - m1 * m1);
		double px1 = p1 / cosh(eta1) * cos(phi1);
		double py1 = p1 / cosh(eta1) * sin(phi1);
		double pz1 = p1 * tanh(eta1);

		double p2 = TMath::Sqrt(e2 * e2 - m2 * m2);
		double px2 = p2 / cosh(eta2) * cos(phi2);
		double py2 = p2 / cosh(eta2) * sin(phi2);
		double pz2 = p2 * tanh(eta2);

		return TMath::Sqrt(TMath::Power(e1 + e2, 2) - TMath::Power(px1 + px2, 2) - TMath::Power(py1 + py2, 2) - TMath::Power(pz1 + pz2, 2));
	}

	TString GetCurrentTimestamp() {
		TDatime now;
		TString timestamp = Form("%d-%06d", now.GetDate(), now.GetTime());	
		return timestamp;	
	}

	TString RoundFloat(double p_number, Int_t p_decimal_precision) {
		TString m_string = "";

		// Positive/negative
		Int_t sign = TMath::Abs(p_number) / p_number;

		// Get integer part
		if (p_number > 1) {
			Int_t integer_part = TMath::Nint(sign * TMath::Floor(TMath::Abs(p_number)));
			m_string += integer_part;
		}


		if (p_decimal_precision > 0) {
			m_string += ".";
			// Get decimal part
			double decimal_part = TMath::Abs(p_number) - TMath::Floor(TMath::Abs(p_number));
			for (int i = 1; i <= p_decimal_precision; i++) {
				double working_number = decimal_part * 10;
				decimal_part = working_number - TMath::Floor(working_number);
				Int_t ith_digit = TMath::Nint(working_number - decimal_part);
				m_string += ith_digit;
			}
		}

		return m_string;
	}

	void PrintHistogramBins(TH1D *h) {
		std::cout << "[PrintHistogramBins] INFO : Printing histogram bins for histogram " << h->GetName() << std::endl;
		TString bin_string = "[PrintHistogramBins] INFO : Bins = ";
		for (int bin = 1; bin <= h->GetNbinsX(); bin++) {
			bin_string += h->GetXaxis()->GetBinLowEdge(bin);
			bin_string += TString("\t");
		}
		bin_string += h->GetXaxis()->GetBinUpEdge(h->GetNbinsX());
		std::cout << bin_string << std::endl;
	}

	void PrintHistogramContents(TH1D *h) {
		std::cout << "[PrintHistogramContents] INFO : Printing histogram bins for histogram " << h->GetName() << std::endl;
		for (int bin = 1; bin <= h->GetNbinsX(); bin++) {
			std::cout << "[PrintHistogramContents] INFO : Bin " << bin << " = " << h->GetBinContent(bin) << " +/- " << h->GetBinError(bin) << std::endl;
		}
	}

	std::vector<TString> SplitTString(TString s_in, char delim) {
		std::stringstream ss(s_in.Data());
		std::vector<TString> s_out;
		std::string item;
		while (std::getline(ss, item, delim)) {
			s_out.push_back(TString(item));
		}
		return s_out;
	}



	std::vector<TString> ListFiles(TString dirname, TString file_pattern)
	{
		std::vector<TString> file_list;

		std::cout << "[ListFiles] Looking for files in " << dirname << " containing " << file_pattern << std::endl;

	   TSystemDirectory dir(dirname, dirname);
	   TList *files = dir.GetListOfFiles();
	   if (files) {
	      TSystemFile *file;
	      TString fname;
	      TIter next(files);
	      while ((file=(TSystemFile*)next())) {
	         fname = file->GetName();
	         if (!file->IsDirectory() && fname.Contains(file_pattern)) {
	            file_list.push_back(fname);
	         }
	      }
	   }
	   return file_list;
	}

	TGraphAsymmErrors* ConvertTH1DToTGraphAsymmErrors(TH1D* p_hist) {
 		TGraphAsymmErrors* tg = new TGraphAsymmErrors(p_hist->GetNbinsX());
 		for (int bin = 1; bin <= p_hist->GetNbinsX(); ++bin) {
 			tg->SetPoint(bin-1, p_hist->GetXaxis()->GetBinCenter(bin), p_hist->GetBinContent(bin));
 			tg->SetPointError(bin-1, p_hist->GetXaxis()->GetBinWidth(bin) / 2., p_hist->GetXaxis()->GetBinWidth(bin) / 2., p_hist->GetBinError(bin), p_hist->GetBinError(bin));
 		}
 		return tg;
	}

	TGraphAsymmErrors* ConvertTH1DToTGraphAsymmErrorsPoisson(TH1D* p_hist) {
 		TGraphAsymmErrors* tg = new TGraphAsymmErrors(p_hist->GetNbinsX());
 		for (int bin = 1; bin <= p_hist->GetNbinsX(); ++bin) {
 			double content = p_hist->GetBinContent(bin);
 			tg->SetPoint(bin-1, p_hist->GetXaxis()->GetBinCenter(bin), content);
 			tg->SetPointError(bin-1, p_hist->GetXaxis()->GetBinWidth(bin) / 2., p_hist->GetXaxis()->GetBinWidth(bin) / 2., 0.5 + TMath::Sqrt(0.25 + content), -0.5 + TMath::Sqrt(0.25 + content));
 		}
 		return tg;
	}

	TH1D* ConvertTGraphToTH1D(TGraphErrors* p_tgraph) {
		// Get axis stops
		double axis_stops[p_tgraph->GetN() + 1];
		for (int point = 0; point <= p_tgraph->GetN(); ++point) {
			if (point == 0) {
				// First point
				axis_stops[point] = p_tgraph->GetX()[0] - (p_tgraph->GetX()[1] - p_tgraph->GetX()[0]);
			} else if (point == p_tgraph->GetN()) {
				axis_stops[point] = p_tgraph->GetX()[p_tgraph->GetN()] + (p_tgraph->GetX()[p_tgraph->GetN()] - p_tgraph->GetX()[p_tgraph->GetN() - 1]);
			} else {
				axis_stops[point] = (p_tgraph->GetX()[point] - p_tgraph->GetX()[point - 1]) / 2.;
			}
		}
		TH1D* hist = new TH1D(TString(p_tgraph->GetName()) + "_hist", TString(p_tgraph->GetName()) + "_hist", p_tgraph->GetN(), axis_stops);
		for (int point = 0; point < p_tgraph->GetN(); ++point) {
			hist->SetBinContent(point + 1, p_tgraph->GetX()[point], p_tgraph->GetY()[point]);
			hist->GetBinError(point + 1, (p_tgraph->GetEYlow()[point] + p_tgraph->GetEYhigh()[point]) / 2.);
		}
		return hist;
	}

	TH1D* CombineHistogramBins(TH1D* p_hist, Int_t p_lowbin, Int_t p_highbin, Bool_t p_rescale) {
		// Assert p_lowbin < p_highbin
		if (p_lowbin >= p_highbin) {
			std::cerr << "[CombinedHistogramBins] ERROR : p_lowbin = " << p_lowbin << ", p_highbin = " << p_highbin << " not in order." << std::endl;
			std::cerr << "[CombinedHistogramBins] ERROR : Histogram name = " << p_hist->GetName() << std::endl;
			p_hist->Print();
			exit(1);
		}

		// Get bins
		int nbins = p_hist->GetNbinsX() - (p_highbin - p_lowbin);
		double * new_bins = new double[nbins + 1];
		int counter = 0;
		for (int bin = 1; bin <= p_hist->GetNbinsX(); ++bin) {
			if (bin <= p_lowbin || bin > p_highbin) {
				new_bins[counter] = p_hist->GetXaxis()->GetBinLowEdge(bin);
				++counter;
			}
		}
		new_bins[nbins] = p_hist->GetXaxis()->GetXmax();

		TH1D* hist_rebinned = new TH1D(TString(p_hist->GetName()) + "_rebinned", TString(p_hist->GetTitle()) + "_rebinned", nbins, new_bins);
		hist_rebinned->Sumw2();
		for (int bin = 1; bin <= p_hist->GetNbinsX(); ++bin) {
			double c_content = p_hist->GetBinContent(bin);
			double c_err = p_hist->GetBinError(bin);
			double c_bin_center = p_hist->GetXaxis()->GetBinCenter(bin);
			int new_bin = hist_rebinned->GetXaxis()->FindBin(c_bin_center);
			hist_rebinned->SetBinContent(new_bin, hist_rebinned->GetBinContent(new_bin) + c_content);
			hist_rebinned->SetBinError(new_bin, TMath::Sqrt(TMath::Power(hist_rebinned->GetBinError(new_bin), 2) + TMath::Power(c_err, 2)));
		}

		// Scale the new bin to preserve bin normalization?
		if (p_rescale) {
			hist_rebinned->SetBinContent(p_lowbin, hist_rebinned->GetBinContent(p_lowbin) / (p_highbin - p_lowbin));
			hist_rebinned->SetBinError(p_lowbin, hist_rebinned->GetBinError(p_lowbin) / (p_highbin - p_lowbin));
		}

		return hist_rebinned;

	}

}


#endif