#ifndef DryCanvas_h
#define DryCanvas_h

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <getopt.h>
#include <stdlib.h>
#include <string>

#include "TROOT.h"
#include "TString.h"
#include "TH1D.h"
#include "TF1.h"
#include "THStack.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TLeafI.h"
#include "TLeafD.h"
#include "TMath.h"
#include "TPython.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TColor.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLine.h"

#include "MyTools/RootUtils/interface/CommonFunctions.h"
#include "MyTools/RootUtils/interface/RootColors.h"
#include "MyTools/RootUtils/interface/SeabornInterface.h"
#include "MyTools/RootUtils/interface/CanvasLabels.h"

namespace Root {

	class DryCanvas : public TCanvas {
		/**
		  * Class DryCanvas
		  * - Pure virtual superclass for David's ROOT canvases.
		  * - Implements some common features, like adding histograms and drawing labels.
		  */
   		ClassDef(DryCanvas, 1);

   		/*** Enums ***/
   		enum ErrorStyle_t {
   			kGaussian,
   			kPoisson
   		};

		/*** Public Methods ***/
	public:
		DryCanvas();
		DryCanvas(const char* name, const char* title, Int_t ww, Int_t wh);
		~DryCanvas();

		/**
		  * AddHistogram
		  * - Adds a histogram to the histogram index. 
		  * - What is done with the index is up to the subclass implementation.
		  */
		virtual void AddHistogram(TH1D *p_histogram, TString p_name, TString p_legend_entry);


		/**
		  * SetLegendEntry
		  * - Allows for changing a legend entry by the histogram index
		  */
		void SetLegendEntry(TString p_name, TString p_legend_entry);


		/**
		  * GetLegendEntry
		  */
		TString GetLegendEntry(TString p_name);


		/**
		  * MakeCanvas
		  * - Every child class uses this method to draw the stored histograms.
		  * - The superclass method sets the flag m_canvas_drawn to true.
		  */
		virtual void MakeCanvas();


		// Canvas customization methods
		// - The superclass methods set flags and store the requested parameters. Implementation in MakeCanvas is up to the subclasses. 
		/**
		  * SetXaxis
		  * - Legacy function calling SetXaxisLimits (change was made for better function naming). 
		  */
		void SetXaxis(double p_xmin, double p_xmax);

		/**
		  * SetXaxisRange
		  * - Specifies manual limits for the x axis. 
		  */
		void SetXaxisRange(double p_xmin, double p_xmax);

		/**
		  * SetXaxisTitle
		  */
		void SetXaxisTitle(TString p_xtitle);

		/**
		 * Sets flag m_xaxis_keepbinlabels_, for use by derived classes.
		 */
		inline void KeepXaxisBinLabels(bool p_bool) {
			m_xaxis_keepbinlabels_ = p_bool;
		}

		// I/O
		/**
		  * SaveAs
		  * - Overload the SaveAs function: if m_canvas_drawn == false, print an error and do nothing.
		  */
		void SaveAs(const char *filename);
		void SaveAs(TString filename);


		// Text drawing
		/**
		  * AddAtlasLabel
		  * - Draw an ATLAS label on the upper plot, optionally with text
		  */
		void AddAtlasLabel(Float_t p_x, Float_t p_y, TString p_text = "", double p_size_modifier = 1.);


		/**
		  * AddText
		  * - Draw text on canvas
		  */
		void AddText(double p_x, double p_y, TString p_text, double p_size_modifier = 1.);


		/**
		  * GetTCanvas
		  * - Returns a cast of this object to TCanvas
		  */
		TCanvas* GetTCanvas();

		inline void SetHistogramColor(TString p_process, Int_t p_color) {
			m_background_histograms_color_override_[p_process] = p_color;
		}

		inline void SetHistogramFillStyle(TString p_process, Int_t p_fillstyle) {
			m_background_histograms_fillstyle_override_[p_process] = p_fillstyle;
		}

		inline void SetLegend(double p_xmin, double p_ymin, double p_xmax, double p_ymax) {
			m_manual_legend_ = true;
			m_legend_xmin_ = p_xmin;
			m_legend_xmax_ = p_xmax;
			m_legend_ymin_ = p_ymin;
			m_legend_ymax_ = p_ymax;
		}

		inline void SetLegendNColumns(int p_n) {
			m_legend_columns_ = p_n;
		}

		/*** Protected Methods ***/
	protected:

		/*** Private Methods ***/
	private:
		/**
		  * GlobalInit
		  * - Initialize common members.
		  */
		void GlobalInit();

		/*** Public Members ***/
	public:

		/*** Protected Members ***/
	protected:
		TLegend *l;
		Root::RootColors *m_root_colors_;
		Root::SeabornInterface *m_seaborn_;
		std::vector<Color_t> colors;
		std::vector<Int_t> markers;
		bool m_canvas_drawn;

		// X axis attributes
		bool m_xaxis_customlimits;
		double m_xmin, m_xmax;

		bool m_xaxis_customtitle;
		TString m_xtitle;

		bool m_xaxis_keepbinlabels_;

		std::vector<TString> m_definition_list;
		std::map<TString, TH1D*> m_histograms;
		std::map<TString, TString> m_legend_entries;

		std::map<TString, Int_t> m_background_histograms_color_override_;
		std::map<TString, Int_t> m_background_histograms_fillstyle_override_;

		bool m_manual_legend_;
		double m_legend_xmin_;
		double m_legend_xmax_;
		double m_legend_ymin_;
		double m_legend_ymax_;
		int m_legend_columns_;
		/*** Private Members ***/
	private:


	};

}

#endif