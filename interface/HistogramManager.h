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

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

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
		HistogramManager(edm::Service<TFileService> *p_fs);
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
		inline void SetFileService(edm::Service<TFileService> *p_fs) {
			fs_ = p_fs;
		}

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
		std::map<TString, TH1F*> th1f_;
		std::map<TString, TH1D*> th1d_;
		std::map<TString, TH2F*> th2f_;
		std::map<TString, TH2D*> th2d_;
		std::map<TString, TH3F*> th3f_;
		std::map<TString, TH3D*> th3d_;
		std::map<TString, TH1*> th1_;
		std::vector<TString> key_list_;
		std::map<TString, HistogramType> histogram_types_;
		TString prefix_;
		TString output_file_path_;
		edm::Service<TFileService>* fs_;
	};

}


#endif