#ifndef SeabornInterface_h
#define SeabornInterface_h

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>

#include <TROOT.h>
#include "TString.h"
#include "TMath.h"
#include "TColor.h"

#include "RootUtils/SimpleConfiguration.h"

namespace Root {
	class SeabornInterface {
		/*** Public Methods ***/
	public:

		SeabornInterface();

		~SeabornInterface();

		/**
		 * Call every time you initialize. Loads saved schemes from the data folder.
		 */
		void Initialize();

		/**
		 * Get a Seaborn color in ROOT format
		 * @param  p_palette Name of seaborn palette. Options are listed in the enum above.
		 * @param  p_step Step of color (<p_total)
		 * @param  p_total Total number of colors to be requested  
		 * @return           [description]
		 */
		Int_t GetColorRoot(TString p_palette, Int_t p_step, Int_t p_total = -1);

		/**
		 * Get a Seaborn color in ROOT format
		 * @param  p_palette Name of seaborn palette. Options are listed in the enum above.
		 * @param  p_step Step of color (<p_total)
		 * @param  p_total Total number of colors to be requested  
		 * @return           [description]
		 */
		std::vector<double> GetColorRGB(TString p_palette, Int_t p_step, Int_t p_total = -1);

		inline const std::vector<TString>* GetListOfPalettes() {
			return &m_palettes_;
		}

		/*** Private Methods ***/
	private:

		/*** Public Members ***/
	public:

		/*** Private Members ***/
	private:
		std::vector<TString> m_palettes_;
		std::map<TString, std::vector<std::vector<double> > > m_palette_rgb_; // Palette name : list of RGB triplets
		std::map<TString, std::vector<Int_t> > m_palette_root_indices_;


	};
}

#endif