#ifndef HistogramManager_cxx
#define HistogramManager_cxx

#include "RootUtils/HistogramManager.h"

namespace Root {
	HistogramManager::HistogramManager() {
		m_wk = 0;
	}

	HistogramManager::~HistogramManager() {
		// Annoying: if you are using an EventLoop::Worker, the worker takes ownership of all of its outputs, so you can't delete the histograms here.
		if (m_wk == 0) {
			for (std::map<TString, TH1F*>::iterator it = m_th1f.begin(); it != m_th1f.end(); ++it) {
				if ((*it).second) {
					delete (*it).second;
				}
			}
			for (std::map<TString, TH1D*>::iterator it = m_th1d.begin(); it != m_th1d.end(); ++it) {
				if ((*it).second) {
					delete (*it).second;
				}
			}
			for (std::map<TString, TH2F*>::iterator it = m_th2f.begin(); it != m_th2f.end(); ++it) {
				if ((*it).second) {
					delete (*it).second;
				}
			}
			for (std::map<TString, TH2D*>::iterator it = m_th2d.begin(); it != m_th2d.end(); ++it) {
				if ((*it).second) {
					delete (*it).second;
				}
			}
			for (std::map<TString, TH3F*>::iterator it = m_th3f.begin(); it != m_th3f.end(); ++it) {
				if ((*it).second) {
					delete (*it).second;
				}
			}
			for (std::map<TString, TH3D*>::iterator it = m_th3d.begin(); it != m_th3d.end(); ++it) {
				if ((*it).second) {
					delete (*it).second;
				}
			}
			m_th1f.clear();
			m_th1d.clear();
			m_th2f.clear();
			m_th2d.clear();
			m_th3f.clear();
			m_th3d.clear();
		}
	}

	TH1F* HistogramManager::AddTH1F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX) {
		CheckName(p_name);
		if (nbinsX <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH1F, you requested histogram " << p_name << " with nbinsX = " << nbinsX << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH1F;

		m_th1f[p_key] = new TH1F(p_name, p_title, nbinsX, minX, maxX);
		m_th1f[p_key]->SetDirectory(0);
		m_th1f[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th1f[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th1f[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th1f[p_key]);
		}
		return m_th1f[p_key];
	}

	TH1F* HistogramManager::AddTH1F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *bins) {
		CheckName(p_name);
		if (nbinsX <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH1F, you requested histogram " << p_name << " with nbinsX = " << nbinsX << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH1F;

		m_th1f[p_key] = new TH1F(p_name, p_title, nbinsX, bins);
		m_th1f[p_key]->SetDirectory(0);
		m_th1f[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th1f[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th1f[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th1f[p_key]);
		}
		return m_th1f[p_key];
	}

	TH1D* HistogramManager::AddTH1D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX) {
		CheckName(p_name);
		if (nbinsX <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH1D, you requested histogram " << p_name << " with nbinsX = " << nbinsX << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH1D;

		m_th1d[p_key] = new TH1D(p_name, p_title, nbinsX, minX, maxX);
		m_th1d[p_key]->SetDirectory(0);
		m_th1d[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th1d[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th1d[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th1d[p_key]);
		}
		return m_th1d[p_key];
	}
	TH1D* HistogramManager::AddTH1D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *bins) {
		CheckName(p_name);
		if (nbinsX <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH1D, you requested histogram " << p_name << " with nbinsX = " << nbinsX << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH1D;

		m_th1d[p_key] = new TH1D(p_name, p_title, nbinsX, bins);
		m_th1d[p_key]->SetDirectory(0);
		m_th1d[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th1d[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th1d[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th1d[p_key]);
		}
		return m_th1d[p_key];
	}

	TH2F* HistogramManager::AddTH2F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY) {
		CheckName(p_name);
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2F, you requested histogram " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH2F;

		m_th2f[p_key] = new TH2F(p_name, p_title, nbinsX, minX, maxX, nbinsY, minY, maxY);
		m_th2f[p_key]->SetDirectory(0);
		m_th2f[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th2f[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		m_th2f[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th2f[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th2f[p_key]);
		}
		return m_th2f[p_key];
	}

	TH2F* HistogramManager::AddTH2F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *binsX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY) {
		CheckName(p_name);
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2F, you requested " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH2F;

		m_th2f[p_key] = new TH2F(p_name, p_title, nbinsX, binsX, nbinsY, minY, maxY);
		m_th2f[p_key]->SetDirectory(0);
		m_th2f[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th2f[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		m_th2f[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th2f[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th2f[p_key]);
		}
		return m_th2f[p_key];
	}

	TH2F* HistogramManager::AddTH2F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double* binsY) {
		CheckName(p_name);
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2F, you requested " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH2F;

		m_th2f[p_key] = new TH2F(p_name, p_title, nbinsX, minX, maxX, nbinsY, binsY);
		m_th2f[p_key]->SetDirectory(0);
		m_th2f[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th2f[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		m_th2f[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th2f[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th2f[p_key]);
		}
		return m_th2f[p_key];
	}

	TH2F* HistogramManager::AddTH2F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *binsX, TString p_yaxistitle, Int_t nbinsY, double* binsY) {
		CheckName(p_name);
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2F, you requested " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH2F;

		m_th2f[p_key] = new TH2F(p_name, p_title, nbinsX, binsX, nbinsY, binsY);
		m_th2f[p_key]->SetDirectory(0);
		m_th2f[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th2f[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		m_th2f[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th2f[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th2f[p_key]);
		}
		return m_th2f[p_key];
	}

	TH2D* HistogramManager::AddTH2D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY) {
		CheckName(p_name);
		TString p_key = p_name;
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2D, you requested " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH2D;

		m_th2d[p_key] = new TH2D(p_name, p_title, nbinsX, minX, maxX, nbinsY, minY, maxY);
		m_th2d[p_key]->SetDirectory(0);
		m_th2d[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th2d[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		m_th2d[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th2d[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th2d[p_key]);
		}
		return m_th2d[p_key];
	}

	TH2D* HistogramManager::AddTH2D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *binsX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY) {
		CheckName(p_name);
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2D, you requested histogram " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH2D;

		m_th2d[p_key] = new TH2D(p_name, p_title, nbinsX, binsX, nbinsY, minY, maxY);
		m_th2d[p_key]->SetDirectory(0);
		m_th2d[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th2d[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		m_th2d[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th2d[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th2d[p_key]);
		}
		return m_th2d[p_key];
	}

	TH2D* HistogramManager::AddTH2D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double* binsY) {
		CheckName(p_name);
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2D, you requested histogram " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH2D;

		m_th2d[p_key] = new TH2D(p_name, p_title, nbinsX, minX, maxX, nbinsY, binsY);
		m_th2d[p_key]->SetDirectory(0);
		m_th2d[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th2d[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		m_th2d[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th2d[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th2d[p_key]);
		}
		return m_th2d[p_key];
	}

	TH2D* HistogramManager::AddTH2D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *binsX, TString p_yaxistitle, Int_t nbinsY, double* binsY) {
		CheckName(p_name);
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2D, you requested histogram " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH2D;

		m_th2d[p_key] = new TH2D(p_name, p_title, nbinsX, binsX, nbinsY, binsY);
		m_th2d[p_key]->SetDirectory(0);
		m_th2d[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th2d[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		m_th2d[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th2d[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th2d[p_key]);
		}
		return m_th2d[p_key];
	}

	TH3F* HistogramManager::AddTH3F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY, TString p_zaxistitle, Int_t nbinsZ, double minZ, double maxZ) {
		CheckName(p_name);
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH3F;

		m_th3f[p_key] = new TH3F(p_name, p_title, nbinsX, minX, maxX, nbinsY, minY, maxY, nbinsZ, minZ, maxZ);
		m_th3f[p_key]->SetDirectory(0);
		m_th3f[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th3f[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		m_th3f[p_key]->GetZaxis()->SetTitle(p_zaxistitle);
		m_th3f[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th3f[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th3f[p_key]);
		}
		return m_th3f[p_key];
	}

	TH3F* HistogramManager::AddTH3F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double* binsX, TString p_yaxistitle, Int_t nbinsY, double* binsY, TString p_zaxistitle, Int_t nbinsZ, double* binsZ) {
		CheckName(p_name);
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH3F;

		m_th3f[p_key] = new TH3F(p_name, p_title, nbinsX, binsX, nbinsY, binsY, nbinsZ, binsZ);
		m_th3f[p_key]->SetDirectory(0);
		m_th3f[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th3f[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		m_th3f[p_key]->GetZaxis()->SetTitle(p_zaxistitle);
		m_th3f[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th3f[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th3f[p_key]);
		}
		return m_th3f[p_key];
	}

	TH3D* HistogramManager::AddTH3D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY, TString p_zaxistitle, Int_t nbinsZ, double minZ, double maxZ) {
		CheckName(p_name);
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH3D;

		m_th3d[p_key] = new TH3D(p_name, p_title, nbinsX, minX, maxX, nbinsY, minY, maxY, nbinsZ, minZ, maxZ);
		m_th3d[p_key]->SetDirectory(0);
		m_th3d[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th3d[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		m_th3d[p_key]->GetZaxis()->SetTitle(p_zaxistitle);
		m_th3d[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th3d[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th3d[p_key]);
		}
		return m_th3d[p_key];
	}

	TH3D* HistogramManager::AddTH3D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double* binsX, TString p_yaxistitle, Int_t nbinsY, double* binsY, TString p_zaxistitle, Int_t nbinsZ, double* binsZ) {
		CheckName(p_name);
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		m_histogram_types[p_key] = t_TH3D;

		m_th3d[p_key] = new TH3D(p_name, p_title, nbinsX, binsX, nbinsY, binsY, nbinsZ, binsZ);
		m_th3d[p_key]->SetDirectory(0);
		m_th3d[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		m_th3d[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		m_th3d[p_key]->GetZaxis()->SetTitle(p_zaxistitle);
		m_th3d[p_key]->Sumw2();
		m_th1[p_key] = static_cast<TH1*>(m_th3d[p_key]);
		if (m_wk) {
			m_wk->addOutput(m_th3d[p_key]);
		}
		return m_th3d[p_key];
	}

	void HistogramManager::AddBinLabels(TString p_name, const std::vector<TString>& p_labels, TString p_axis) {
		TString p_key = p_name;
		if (!(m_prefix.EqualTo(""))) {
			p_key = p_name;
			p_name = m_prefix + p_name;
		}
		if (m_th1.find(p_key) == m_th1.end()) {
			std::cerr << "[HistogramManager] ERROR : In AddBinLabels(), no histogram named " << p_name << " exists." << std::endl;
			exit(1);
		}
		TAxis *axis;
		if (p_axis.EqualTo("x")) {
			axis = m_th1[p_key]->GetXaxis();
		} else if (p_axis.EqualTo("y")) {
			axis = m_th1[p_key]->GetYaxis();
		} else if (p_axis.EqualTo("z")) {
			axis = m_th1[p_key]->GetZaxis();
		} else {
			std::cerr << "[HistogramManager] ERROR : In AddBinLabels(), p_axis must be x, y, or z." << std::endl;
			exit(1);
		}
		if (axis->GetNbins() != p_labels.size()) {
			std::cerr << "[HistogramManager] ERROR : In AddBinLabels(), number of bin labels (" << p_labels.size() << ") != number of bins (" << axis->GetNbins() << ") for histogram " << p_name << std::endl;
			exit(1);
		}
		
		for (int bin = 1; bin <= axis->GetNbins(); ++bin) {
			axis->SetBinLabel(bin, p_labels[bin-1]);
		}
	}


	TH1* HistogramManager::operator[](TString p_key) {
		return m_th1[p_key];
	}

	void HistogramManager::SaveAll(TFile *f) {
		if (m_wk) {
			std::cerr << "[HistogramManager] WARNING : SaveAll called, but histograms are already being saved to the EventLoop output! Saving anyways, but this is redundant and less supported." << std::endl;
		}

		bool make_backup_file = false;
		if (f == 0x0) {
			std::cerr << "[HistogramManager] WARNING : Save file does not exist! Creating a backup file at HistogramManagerBackup.root." << std::endl;
			f = new TFile("HistogramManagerBackup.root", "UPDATE");
			make_backup_file = true;
		}

		f->cd();

		for (std::vector<TString>::iterator it = m_key_list.begin(); it != m_key_list.end(); ++it) {
			if (IsA(*it) == t_TH1F) {
				static_cast<TH1F*>(m_th1[*it])->Write(m_th1[*it]->GetName());
			} else if (IsA(*it) == t_TH1D) {
				static_cast<TH1D*>(m_th1[*it])->Write(m_th1[*it]->GetName());
			} else if (IsA(*it) == t_TH2F) {
				static_cast<TH2F*>(m_th1[*it])->Write(m_th1[*it]->GetName());
			} else if (IsA(*it) == t_TH2D) {
				static_cast<TH2D*>(m_th1[*it])->Write(m_th1[*it]->GetName());
			} else if (IsA(*it) == t_TH3F) {
				static_cast<TH3F*>(m_th1[*it])->Write(m_th1[*it]->GetName());
			} else if (IsA(*it) == t_TH3D) {
				static_cast<TH3D*>(m_th1[*it])->Write(m_th1[*it]->GetName());
			}
			// Do we need to do this to ensure the HistogramManager keeps ownership?
			m_th1[*it]->SetDirectory(0);
		}

		if (make_backup_file) {
			f->Close();
			delete f;
		}
	}
 
	void HistogramManager::CheckName(TString p_name) {
		if (std::find(m_key_list.begin(), m_key_list.end(), p_name) != m_key_list.end()) {
			std::cerr << "[HistogramManager] ERROR : Attempt to add already existing histogram " << p_name << ". Please use a unique name." << std::endl;
			std::exit(1);
		} else if (p_name.EqualTo("")) {
			std::cerr << "[HistogramManager] ERROR : Attempt to add histogram with name=\"\". Please specify a real name." << std::endl;
			std::exit(1);
		}
		m_key_list.push_back(p_name);
	}

	HistogramManager::HistogramType HistogramManager::IsA(TString p_key) {
		return m_histogram_types[p_key];
	}

	TH1F* HistogramManager::GetTH1F(TString p_name) {
		if (m_th1f.find(p_name) != m_th1f.end()) {
			return m_th1f[p_name];
		} else {
			std::cerr << "[HistogramManager] WARNING : TH1F " << p_name << " not known. HistogramManager prefix = " << m_prefix << ". Returning 0." << std::endl;
			return 0;
		}
	}

	TH1D* HistogramManager::GetTH1D(TString p_name) {
		if (m_th1d.find(p_name) != m_th1d.end()) {
			return m_th1d[p_name];
		} else {
			std::cerr << "[HistogramManager] WARNING : TH1D " << p_name << " not known. HistogramManager prefix = " << m_prefix << ". Returning 0." << std::endl;
			return 0;
		}
	}

	TH2F* HistogramManager::GetTH2F(TString p_name) {
		if (m_th2f.find(p_name) != m_th2f.end()) {
			return m_th2f[p_name];
		} else {
			std::cerr << "[HistogramManager] WARNING : TH2F " << p_name << " not known. HistogramManager prefix = " << m_prefix << ". Returning 0." << std::endl;
			return 0;
		}
	}

	TH2D* HistogramManager::GetTH2D(TString p_name) {
		if (m_th2d.find(p_name) != m_th2d.end()) {
			return m_th2d[p_name];
		} else {
			std::cerr << "[HistogramManager] WARNING : TH2D " << p_name << " not known. HistogramManager prefix = " << m_prefix << ". Returning 0." << std::endl;
			return 0;
		}
	}

	TH3F* HistogramManager::GetTH3F(TString p_name) {
		if (m_th3f.find(p_name) != m_th3f.end()) {
			return m_th3f[p_name];
		} else {
			std::cerr << "[HistogramManager] WARNING : TH3F " << p_name << " not known. HistogramManager prefix = " << m_prefix << ". Returning 0." << std::endl;
			return 0;
		}
	}

	TH3D* HistogramManager::GetTH3D(TString p_name) {
		if (m_th3d.find(p_name) != m_th3d.end()) {
			return m_th3d[p_name];
		} else {
			std::cerr << "[HistogramManager] WARNING : TH3D " << p_name << " not known. HistogramManager prefix = " << m_prefix << ". Returning 0." << std::endl;
			return 0;
		}
	}

	void HistogramManager::AddPrefix(TString p_prefix) {
		if (m_th1.size() > 0) {
			std::cerr << "[HistogramManager] WARNING : Attempt to add a histogram prefix after histograms have already been added. Please do this before making any histograms. Doing nothing for now!" << std::endl;
		} else {
			m_prefix = p_prefix;
		}
	}

	void HistogramManager::SetELOutput(EL::Worker *p_wk) {
		m_wk = p_wk;
	}

}



#endif