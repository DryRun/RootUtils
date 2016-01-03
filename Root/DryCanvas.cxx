#include "RootUtils/DryCanvas.h"

namespace Root {
	DryCanvas::DryCanvas() : TCanvas() {
		GlobalInit();
	}

	DryCanvas::DryCanvas(const char* name, const char* title, Int_t ww, Int_t wh) : TCanvas(name, title, ww, wh) {
		GlobalInit();
	}

	DryCanvas::~DryCanvas() {
		for (std::map<TString, TH1D*>::iterator it = m_histograms.begin(); it != m_histograms.end(); ++it) {
			delete (*it).second;
		}
		m_histograms.clear();

		delete m_root_colors_;
		delete m_seaborn_;
	}
	
	void DryCanvas::GlobalInit() {
		m_canvas_drawn = false;
		m_xaxis_customlimits = false;
		m_xaxis_customtitle = false;
		m_xaxis_keepbinlabels_ = false;

		//colors.push_back(kRed-3);
		//colors.push_back(kBlack);
		//colors.push_back(kGreen+3);
		//colors.push_back(kCyan+3);
		//colors.push_back(kBlue-3);
		//colors.push_back(kYellow-1);
		//colors.push_back(kCyan-6);
		//colors.push_back(kViolet+9);
		//colors.push_back(kYellow-3);
		//colors.push_back(kMagenta+1);
		//colors.push_back(kOrange+3);
		//colors.push_back(kPink+9);
		//colors.push_back(kOrange+1);
		//colors.push_back(kRed+4);
		
		// Nice Mike-like color scheme
		colors.push_back(kBlue-3);
		colors.push_back(kGreen+3);
		colors.push_back(kOrange+1);
		colors.push_back(kCyan);
		colors.push_back(kGreen);
		colors.push_back(kMagenta);
		colors.push_back(kBlue-6);
		colors.push_back(kGreen+4);
		colors.push_back(kRed-3);
		colors.push_back(kPink);
		colors.push_back(kViolet+10);
		colors.push_back(kYellow);
	

		markers.push_back(20);
		markers.push_back(21);
		markers.push_back(22);
		markers.push_back(23);
		markers.push_back(24);
		markers.push_back(33);
		markers.push_back(25);
		markers.push_back(26);
		markers.push_back(32);
		markers.push_back(27);
		markers.push_back(29);
		markers.push_back(30);

		m_root_colors_ = new Root::RootColors();
		m_root_colors_->SetGradientSteps(180);

		m_seaborn_ = new Root::SeabornInterface();
		m_seaborn_->Initialize();

		m_manual_legend_ = false;
		m_legend_xmin_ = -1.;
		m_legend_xmax_ = -1.;
		m_legend_ymin_ = -1.;
		m_legend_ymax_ = -1.;
		m_legend_columns_ = -1;
	}

	void DryCanvas::AddHistogram(TH1D *p_histogram, TString p_name, TString p_legend_entry) {
		if (find(m_definition_list.begin(), m_definition_list.end(), p_name) != m_definition_list.end()) {
			std::cerr << "[DryCanvas] WARNING : Overwriting histogram " << p_name << std::endl;
		} else {
			m_definition_list.push_back(p_name);
		}
		m_histograms[p_name] = (TH1D*)p_histogram->Clone();
		m_histograms[p_name]->SetDirectory(0);
		m_legend_entries[p_name] = p_legend_entry;
	}

	void DryCanvas::SetLegendEntry(TString p_name, TString p_legend_entry) {
		m_legend_entries[p_name] = p_legend_entry;
	}

	TString DryCanvas::GetLegendEntry(TString p_name) {
		if (m_legend_entries.find(p_name) != m_legend_entries.end()) {
			return m_legend_entries[p_name];
		} else {
			std::cerr << "[DryCanvas] WARNING : Requested legend entry " << p_name << " was not found. Returning NULL" << std::endl;
			return TString("NULL");
		}
	}

	void DryCanvas::MakeCanvas() {
		this->Clear();
		m_canvas_drawn = true;
	}


	void DryCanvas::SetXaxisRange(double p_xmin, double p_xmax) {
		if (p_xmax < p_xmin) {
			std::cerr << "[DryCanvas] WARNING : You just tried to specify xmax < xmin for this canvas. Doing nothing..." << std::endl;
			return;
		}
		m_xaxis_customlimits = true;
		m_xmin = p_xmin;
		m_xmax = p_xmax;
	}

	void DryCanvas::SetXaxis(double p_xmin, double p_xmax) {
		SetXaxisRange(p_xmin, p_xmax);
	}

	void DryCanvas::SetXaxisTitle(TString p_xtitle) {
		m_xaxis_customtitle = true;
		m_xtitle =  p_xtitle;
	}


	// I/O
	/**
	  * SaveAs
	  * - Overload the SaveAs function: if m_canvas_drawn == false, print an error and do nothing.
	  */
	void DryCanvas::SaveAs(const char *filename) {
		if (m_canvas_drawn) {
			dynamic_cast<TCanvas*>(this)->SaveAs(filename);
		} else {
			std::cerr << "[DryCanvas] WARNING : DryCanvas::SaveAs() called before canvas was drawn. Doing nothing." << std::endl;
		}
	}

	void DryCanvas::SaveAs(TString filename) {
		SaveAs(filename.Data());
	}


	// Text drawing
	void DryCanvas::AddAtlasLabel(Float_t p_x, Float_t p_y, TString p_text, double p_size_modifier) {
		this->cd();
		ATLASLabel(p_x, p_y, p_text, kBlack, p_size_modifier);
	}


	void DryCanvas::AddText(double p_x, double p_y, TString p_text, double p_size_modifier) {
		this->cd();
		myText(p_x, p_y, kBlack, p_text, p_size_modifier);
	}

	TCanvas* DryCanvas::GetTCanvas() {
		TCanvas *c = new TCanvas(TString(this->GetName()) + "_clone", TString(this->GetTitle()) + "_clone", this->GetWw(), this->GetWh());
		c->cd();
		this->DrawClonePad();
		return c;
	}
}
