#ifndef RootColors_h
#define RootColors_h

#include <iostream>
#include <vector>
#include <map>

#include "TROOT.h"
#include "TColor.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TSystem.h"
#include "TString.h"

namespace Root {

	class RootColors {
	public:
		enum Palette {
			kDefault,
			kRainbow,
			kRainbowMatte
		};

		/*** Public Methods***/
	public:
		RootColors();
		~RootColors();

		/**
		 * Get integer of color from loaded palette
		 * @param  p_step Step number of color to load (e.g. color 10 / 40, if 40 colors were created in LoadPalette)
		 * @return        Number of color. Use, e.g., TAttMarker::SetMarkerColor(val)
		 */
		Int_t GetColor(Root::RootColors::Palette p_palette, Int_t p_step);


		/**
		 * Set the number of gradient steps. Note: does not retroactively change already loaded palettes.
		 * @param p_steps Number of gradient steps
		 */
		void SetGradientSteps(Int_t p_steps);

		
		void TestModule(Root::RootColors::Palette p_palette, TString p_figure_directory);
	

		/*** Private Methods ***/
	private:
		/**
		 * Load a color palette
		 * @param p_palette See enum Palette
		 * @param p_steps   Number of steps in gradient
		 */
		void LoadPalette(Root::RootColors::Palette p_palette);

		/*** Public Members ***/
	public:

		/*** Private Members ***/
	private:
		std::map<Palette, TString> m_palette_string_;
		std::map<Palette, std::vector<TColor> > m_palette_colors_;
		Int_t m_gradient_steps_;
		std::map<Palette, bool> m_palette_loaded_;
	};



};


#endif