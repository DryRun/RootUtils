#ifndef RootColors_cxx
#define RootColors_cxx

#include "MyTools/RootUtils/interface/RootColors.h"

namespace Root {
	RootColors::RootColors() {
		m_palette_string_[kDefault] = "Default";
		m_palette_string_[kRainbow] = "Rainbow";
		m_palette_string_[kRainbowMatte] = "RainbowMatte";
		m_palette_loaded_[kDefault] = true;
		m_palette_loaded_[kRainbow] = false;
		m_palette_loaded_[kRainbowMatte] = false;
		m_gradient_steps_ = 40;
	}
	
	RootColors::~RootColors() {

	}

	void RootColors::SetGradientSteps(Int_t p_steps) {
		m_gradient_steps_ = p_steps;
	}

	void RootColors::LoadPalette(Root::RootColors::Palette p_palette) {
		if (m_palette_loaded_[p_palette]) {
			std::cerr << "[RootColors] WARNING : Palette " << m_palette_string_[p_palette] << " already loaded. Doing nothing." << std::endl;
			return;
		}
		m_palette_loaded_[p_palette] = true;

		if (p_palette == Root::RootColors::kRainbow) {
			std::cout << "[RootColors] INFO : Loading color palette 'Rainbow'" << std::endl;
			for (int i = 0; i < m_gradient_steps_; ++i) {
				float c_hue = 360. * i / m_gradient_steps_;
				float c_sat = 1.;
				float c_bri = 1.;
				float c_red;
				float c_blue;
				float c_green;
				TColor::HSV2RGB(c_hue, c_sat, c_bri, c_red, c_green, c_blue);
				int c_index = TColor::GetColor(c_red, c_green, c_blue);
				m_palette_colors_[p_palette].push_back(TColor(c_index, c_red, c_green, c_blue));
			}
		} else if (p_palette == Root::RootColors::kRainbowMatte) { 
			std::cout << "[RootColors] INFO : Loading color palette 'RainbowMatte'" << std::endl;
			for (int i = 0; i < m_gradient_steps_; ++i) {
				float c_hue = 360. * i / m_gradient_steps_;
				float c_sat = 0.7;
				float c_bri = 1.0;
				float c_red;
				float c_blue;
				float c_green;
				TColor::HSV2RGB(c_hue, c_sat, c_bri, c_red, c_green, c_blue);
				int c_index = TColor::GetColor(c_red, c_green, c_blue);
				m_palette_colors_[p_palette].push_back(TColor(c_index, c_red, c_green, c_blue));
			}
		}
	}


	Int_t RootColors::GetColor(Root::RootColors::Palette p_palette, Int_t p_step) {
		if (p_palette == kDefault) {
			return p_step;
		} else {
			if (!m_palette_loaded_[p_palette]) {
				LoadPalette(p_palette);
			}
			if (p_step >= m_gradient_steps_) {
				std::cerr << "[RootColors] WARNING : Color requested beyond number of gradient steps (=" << m_gradient_steps_ << ") . Consider increasing the number of steps with RootColors::SetGradientSteps." << std::endl;
			}
			return m_palette_colors_[p_palette][p_step % m_gradient_steps_].GetNumber();
		}
	}

	void RootColors::TestModule(Root::RootColors::Palette p_palette, TString p_figure_directory) {
		std::cout << "[RootColors] INFO : In RootColors::TestModule" << std::endl;
		gSystem->Exec(TString("mkdir -pv ") + p_figure_directory);
		TRandom3 *c_rand = new TRandom3();

		std::map<Palette, TString> c_test_palettes;
		c_test_palettes[kRainbow] = "Rainbow";
		c_test_palettes[kRainbowMatte] = "RainbowMatte";

		for (std::map<Palette, TString>::iterator it = c_test_palettes.begin(); it != c_test_palettes.end(); ++it) {
			LoadPalette((*it).first);
			TCanvas *c_poisson = new TCanvas("c_poisson", "c_poisson", 1200, 800);
			TH1D* h_frame = new TH1D("h_frame", "h_frame", 101, -0.5, 100.5);
			h_frame->SetMaximum(500);
			h_frame->Draw();
			std::map<int, TH1D*> h_poisson;
			for (int i = 0; i < 40; ++i) {
				TString hname = "h_poisson_"; hname += i;
				h_poisson[i] = new TH1D(hname, hname, 101, -0.5, 100.5);
				for (int j = 0; j < 1000; ++j) {
					h_poisson[i]->Fill(c_rand->Poisson(i));
				}
				std::cout << "[RootColors] DEBUG : Setting color for histogram " << i << " to " << this->GetColor((*it).first, i) << std::endl;
				h_poisson[i]->SetLineColor(this->GetColor((*it).first, i));
				h_poisson[i]->SetLineWidth(1);
				h_poisson[i]->SetLineStyle(1);
				h_poisson[i]->Draw("hist same");
			}
			c_poisson->SaveAs(p_figure_directory + "/RootColors_" + (*it).second + ".pdf");
		}

		delete c_rand;
	}

}




#endif