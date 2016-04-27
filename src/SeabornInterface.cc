#ifndef SeabornInterface_cxx
#define SeabornInterface_cxx

#include "MyTools/RootUtils/interface/SeabornInterface.h"

namespace Root {

/*
Example TPython commands:
	TPython::Bind(&tp_mcnumber, "mcnumber");
	TPython::Bind(&tp_xsec, "xsec");
	TPython::Exec("xsec.SetVal(csl.GetCrossSection(mcnumber.GetVal()))");
*/
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	    std::stringstream ss(s);
	    std::string item;
	    while (std::getline(ss, item, delim)) {
	        elems.push_back(item);
	    }
	    return elems;
	}

	std::vector<std::string> split(const std::string &s, char delim) {
	    std::vector<std::string> elems;
	    split(s, delim, elems);
	    return elems;
	}

	SeabornInterface::SeabornInterface() {

	}

	SeabornInterface::~SeabornInterface() {

	}

	void SeabornInterface::Initialize() {
		std::auto_ptr<SimpleConfiguration> m_save_colors(new SimpleConfiguration());
		m_save_colors->LoadConfiguration("$CMSSW_BASE/src/MyTools/RootUtils/data/SeabornInterface/ColorPalettes.xml");
		for (std::vector<TString>::iterator it_palette_name = m_save_colors->GetListOfLists()->begin(); it_palette_name != m_save_colors->GetListOfLists()->end(); ++it_palette_name) {
			m_palettes_.push_back(*it_palette_name);
			for (std::vector<std::pair<TString, TString> >::iterator it_color_rgb = m_save_colors->GetList(*it_palette_name)->begin(); it_color_rgb != m_save_colors->GetList(*it_palette_name)->end(); ++it_color_rgb) {
				// Second element in pair is a comma-separated (r,g,b) pair
				std::vector<std::string> c_rgb_string = split((*it_color_rgb).second.Data(), ',');
				std::vector<double> c_rgb;
				for (std::vector<std::string>::iterator it_rgb = c_rgb_string.begin(); it_rgb != c_rgb_string.end(); ++it_rgb) {
					c_rgb.push_back(TString(*it_rgb).Atof());
				}

				// Save RGB color
				m_palette_rgb_[*it_palette_name].push_back(c_rgb);

				// Make a ROOT color
				int c_root_index = TColor::GetColor((float)c_rgb[0], c_rgb[1], c_rgb[2]);
				m_palette_root_indices_[*it_palette_name].push_back(c_root_index);
			}
		}
	}

	Int_t SeabornInterface::GetColorRoot(TString p_palette, Int_t p_step, Int_t p_total) {
		if (std::find(m_palettes_.begin(), m_palettes_.end(), p_palette) == m_palettes_.end()) {
			std::cerr << "[SeabornInterface] ERROR : Request for unknown palette " << p_palette << std::endl;
			exit(1);
		}
		Int_t c_color;
		Int_t n_colors_available = m_palette_root_indices_[p_palette].size();
		if (p_total < 0) {
			c_color = m_palette_root_indices_[p_palette][p_step % n_colors_available];
		} else {
			c_color = m_palette_root_indices_[p_palette][TMath::Nint(1. * p_step / p_total * n_colors_available)];
		}
		return c_color;
	}

	std::vector<double> SeabornInterface::GetColorRGB(TString p_palette, Int_t p_step, Int_t p_total) {
		std::vector<double> c_color;
		Int_t n_colors_available = m_palette_rgb_[p_palette].size();
		if (p_total < 0) {
			c_color = m_palette_rgb_[p_palette][p_step % n_colors_available];
		} else {
			c_color = m_palette_rgb_[p_palette][TMath::Nint(1. * p_step / p_total * n_colors_available)];
		}
		return c_color;
	}

}

#endif