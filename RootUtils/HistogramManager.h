#ifndef HistogramManager_h
#define HistogramManager_h

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <algorithm>

#include <TH1.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TH2D.h>
#include <TH3F.h>
#include <TH3D.h>
#include <TFile.h>

#include "EventLoop/Worker.h"

namespace Root {

	class HistogramManager {
	public:
		enum HistogramType { 
			t_NULL,
			t_TH1F,
			t_TH1D,
			t_TH2F,
			t_TH2D,
			t_TH3F,
			t_TH3D
		};

		/*** Public Methods ***/
	public:
		HistogramManager();
		~HistogramManager();

		void AddPrefix(TString p_prefix);

		TH1F* AddTH1F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX);
		TH1F* AddTH1F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *bins);
		TH1D* AddTH1D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX);
		TH1D* AddTH1D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *bins);

		TH2F* AddTH2F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY);
		TH2F* AddTH2F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *binsX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY);
		TH2F* AddTH2F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double* binsY);
		TH2F* AddTH2F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *binsX, TString p_yaxistitle, Int_t nbinsY, double* binsY);
		TH2D* AddTH2D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY);
		TH2D* AddTH2D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *binsX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY);
		TH2D* AddTH2D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double* binsY);
		TH2D* AddTH2D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *binsX, TString p_yaxistitle, Int_t nbinsY, double* binsY);

		TH3F* AddTH3F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY, TString p_zaxistitle, Int_t nbinsZ, double minZ, double maxZ);
		TH3F* AddTH3F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double* binsX, TString p_yaxistitle, Int_t nbinsY, double* binsY, TString p_zaxistitle, Int_t nbinsZ, double* binsZ);
		TH3D* AddTH3D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY, TString p_zaxistitle, Int_t nbinsZ, double minZ, double maxZ);
		TH3D* AddTH3D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double* binsX, TString p_yaxistitle, Int_t nbinsY, double* binsY, TString p_zaxistitle, Int_t nbinsZ, double* binsZ);

		void AddBinLabels(TString p_name, const std::vector<TString>& p_labels, TString p_axis = "x");

		// Accessors
		/**
		 * operator[]
		 * @param p_key Key of histogram
		 * @return		Pointer to TH1 corresponding to key (probably have to typecast to use)
		 */
		TH1* operator[](TString p_key);

		/**
		 * Get THXX
		 * @param  p_name Key of THXX
		 * @return        Histogram corresponding to key
		 */
		TH1F* GetTH1F(TString p_name);
		TH1D* GetTH1D(TString p_name);
		TH2F* GetTH2F(TString p_name);
		TH2D* GetTH2D(TString p_name);
		TH3F* GetTH3F(TString p_name);
		TH3D* GetTH3D(TString p_name);

		/**
		 * Add a pointer to an EL::Worker to save the output automatically
		 * @param p_wk [description]
		 */
		void SetELOutput(EL::Worker *p_wk);

		/**
		 * Save all histograms to an existing file. It is allowed to do this in conjunction with SetELOutput.
		 * @param f Open TFile for histogram saving.
		 */
		void SaveAll(TFile *f);

		void CheckName(TString p_name);

		HistogramType IsA(TString p_key);

		/*** Private Methods ***/
	private:



		/*** Public Members ***/
	public:

		/*** Private Members ***/
	private:
		std::map<TString, TH1F*> m_th1f;
		std::map<TString, TH1D*> m_th1d;
		std::map<TString, TH2F*> m_th2f;
		std::map<TString, TH2D*> m_th2d;
		std::map<TString, TH3F*> m_th3f;
		std::map<TString, TH3D*> m_th3d;
		std::map<TString, TH1*> m_th1;
		std::vector<TString> m_key_list;
		std::map<TString, HistogramType> m_histogram_types;
		TString m_prefix;
		EL::Worker* m_wk;
		TString m_output_file_path;
	};

}


#endif