#ifndef HistogramManager_cxx
#define HistogramManager_cxx

#include "MyTools/RootUtils/interface/HistogramManager.h"

namespace Root {
	HistogramManager::HistogramManager(edm::Service<TFileService> *p_fs) : fs_(p_fs) {
	}

	HistogramManager::~HistogramManager() {}

	TH1F* HistogramManager::AddTH1F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX) {
		CheckName(p_name);
		if (nbinsX <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH1F, you requested histogram " << p_name << " with nbinsX = " << nbinsX << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH1F;

		th1f_[p_key] = (*fs_)->make<TH1F>(p_name, p_title, nbinsX, minX, maxX);
		th1f_[p_key]->SetDirectory(0);
		th1f_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th1f_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th1f_[p_key]);

		return th1f_[p_key];
	}

	TH1F* HistogramManager::AddTH1F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *bins) {
		CheckName(p_name);
		if (nbinsX <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH1F, you requested histogram " << p_name << " with nbinsX = " << nbinsX << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH1F;

		th1f_[p_key] = (*fs_)->make<TH1F>(p_name, p_title, nbinsX, bins);
		th1f_[p_key]->SetDirectory(0);
		th1f_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th1f_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th1f_[p_key]);

		return th1f_[p_key];
	}

	TH1D* HistogramManager::AddTH1D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX) {
		CheckName(p_name);
		if (nbinsX <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH1D, you requested histogram " << p_name << " with nbinsX = " << nbinsX << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH1D;

		th1d_[p_key] = (*fs_)->make<TH1D>(p_name, p_title, nbinsX, minX, maxX);
		th1d_[p_key]->SetDirectory(0);
		th1d_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th1d_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th1d_[p_key]);

		return th1d_[p_key];
	}
	TH1D* HistogramManager::AddTH1D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *bins) {
		CheckName(p_name);
		if (nbinsX <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH1D, you requested histogram " << p_name << " with nbinsX = " << nbinsX << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH1D;

		th1d_[p_key] = (*fs_)->make<TH1D>(p_name, p_title, nbinsX, bins);
		th1d_[p_key]->SetDirectory(0);
		th1d_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th1d_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th1d_[p_key]);

		return th1d_[p_key];
	}

	TH2F* HistogramManager::AddTH2F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY) {
		CheckName(p_name);
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2F, you requested histogram " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH2F;

		th2f_[p_key] = (*fs_)->make<TH2F>(p_name, p_title, nbinsX, minX, maxX, nbinsY, minY, maxY);
		th2f_[p_key]->SetDirectory(0);
		th2f_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th2f_[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		th2f_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th2f_[p_key]);

		return th2f_[p_key];
	}

	TH2F* HistogramManager::AddTH2F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *binsX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY) {
		CheckName(p_name);
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2F, you requested " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH2F;

		th2f_[p_key] = (*fs_)->make<TH2F>(p_name, p_title, nbinsX, binsX, nbinsY, minY, maxY);
		th2f_[p_key]->SetDirectory(0);
		th2f_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th2f_[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		th2f_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th2f_[p_key]);

		return th2f_[p_key];
	}

	TH2F* HistogramManager::AddTH2F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double* binsY) {
		CheckName(p_name);
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2F, you requested " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH2F;

		th2f_[p_key] = (*fs_)->make<TH2F>(p_name, p_title, nbinsX, minX, maxX, nbinsY, binsY);
		th2f_[p_key]->SetDirectory(0);
		th2f_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th2f_[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		th2f_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th2f_[p_key]);

		return th2f_[p_key];
	}

	TH2F* HistogramManager::AddTH2F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *binsX, TString p_yaxistitle, Int_t nbinsY, double* binsY) {
		CheckName(p_name);
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2F, you requested " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH2F;

		th2f_[p_key] = (*fs_)->make<TH2F>(p_name, p_title, nbinsX, binsX, nbinsY, binsY);
		th2f_[p_key]->SetDirectory(0);
		th2f_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th2f_[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		th2f_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th2f_[p_key]);

		return th2f_[p_key];
	}

	TH2D* HistogramManager::AddTH2D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY) {
		CheckName(p_name);
		TString p_key = p_name;
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2D, you requested " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH2D;

		th2d_[p_key] = (*fs_)->make<TH2D>(p_name, p_title, nbinsX, minX, maxX, nbinsY, minY, maxY);
		th2d_[p_key]->SetDirectory(0);
		th2d_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th2d_[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		th2d_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th2d_[p_key]);

		return th2d_[p_key];
	}

	TH2D* HistogramManager::AddTH2D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *binsX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY) {
		CheckName(p_name);
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2D, you requested histogram " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH2D;

		th2d_[p_key] = (*fs_)->make<TH2D>(p_name, p_title, nbinsX, binsX, nbinsY, minY, maxY);
		th2d_[p_key]->SetDirectory(0);
		th2d_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th2d_[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		th2d_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th2d_[p_key]);

		return th2d_[p_key];
	}

	TH2D* HistogramManager::AddTH2D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double* binsY) {
		CheckName(p_name);
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2D, you requested histogram " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH2D;

		th2d_[p_key] = (*fs_)->make<TH2D>(p_name, p_title, nbinsX, minX, maxX, nbinsY, binsY);
		th2d_[p_key]->SetDirectory(0);
		th2d_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th2d_[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		th2d_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th2d_[p_key]);

		return th2d_[p_key];
	}

	TH2D* HistogramManager::AddTH2D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double *binsX, TString p_yaxistitle, Int_t nbinsY, double* binsY) {
		CheckName(p_name);
		if (nbinsX <= 0 || nbinsY <= 0) {
			std::cerr << "[HistogramManager] ERROR : In AddTH2D, you requested histogram " << p_name << " with nbinsX = " << nbinsX << ", nbinsY = " << std::endl;
			throw -1;
		}
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH2D;

		th2d_[p_key] = (*fs_)->make<TH2D>(p_name, p_title, nbinsX, binsX, nbinsY, binsY);
		th2d_[p_key]->SetDirectory(0);
		th2d_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th2d_[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		th2d_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th2d_[p_key]);

		return th2d_[p_key];
	}

	TH3F* HistogramManager::AddTH3F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY, TString p_zaxistitle, Int_t nbinsZ, double minZ, double maxZ) {
		CheckName(p_name);
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH3F;

		th3f_[p_key] = (*fs_)->make<TH3F>(p_name, p_title, nbinsX, minX, maxX, nbinsY, minY, maxY, nbinsZ, minZ, maxZ);
		th3f_[p_key]->SetDirectory(0);
		th3f_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th3f_[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		th3f_[p_key]->GetZaxis()->SetTitle(p_zaxistitle);
		th3f_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th3f_[p_key]);

		return th3f_[p_key];
	}

	TH3F* HistogramManager::AddTH3F(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double* binsX, TString p_yaxistitle, Int_t nbinsY, double* binsY, TString p_zaxistitle, Int_t nbinsZ, double* binsZ) {
		CheckName(p_name);
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH3F;

		th3f_[p_key] = (*fs_)->make<TH3F>(p_name, p_title, nbinsX, binsX, nbinsY, binsY, nbinsZ, binsZ);
		th3f_[p_key]->SetDirectory(0);
		th3f_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th3f_[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		th3f_[p_key]->GetZaxis()->SetTitle(p_zaxistitle);
		th3f_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th3f_[p_key]);

		return th3f_[p_key];
	}

	TH3D* HistogramManager::AddTH3D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double minX, double maxX, TString p_yaxistitle, Int_t nbinsY, double minY, double maxY, TString p_zaxistitle, Int_t nbinsZ, double minZ, double maxZ) {
		CheckName(p_name);
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH3D;

		th3d_[p_key] = (*fs_)->make<TH3D>(p_name, p_title, nbinsX, minX, maxX, nbinsY, minY, maxY, nbinsZ, minZ, maxZ);
		th3d_[p_key]->SetDirectory(0);
		th3d_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th3d_[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		th3d_[p_key]->GetZaxis()->SetTitle(p_zaxistitle);
		th3d_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th3d_[p_key]);

		return th3d_[p_key];
	}

	TH3D* HistogramManager::AddTH3D(TString p_name, TString p_title, TString p_xaxistitle, Int_t nbinsX, double* binsX, TString p_yaxistitle, Int_t nbinsY, double* binsY, TString p_zaxistitle, Int_t nbinsZ, double* binsZ) {
		CheckName(p_name);
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		histogram_types_[p_key] = t_TH3D;

		th3d_[p_key] = (*fs_)->make<TH3D>(p_name, p_title, nbinsX, binsX, nbinsY, binsY, nbinsZ, binsZ);
		th3d_[p_key]->SetDirectory(0);
		th3d_[p_key]->GetXaxis()->SetTitle(p_xaxistitle);
		th3d_[p_key]->GetYaxis()->SetTitle(p_yaxistitle);
		th3d_[p_key]->GetZaxis()->SetTitle(p_zaxistitle);
		th3d_[p_key]->Sumw2();
		th1_[p_key] = static_cast<TH1*>(th3d_[p_key]);

		return th3d_[p_key];
	}

	void HistogramManager::AddBinLabels(TString p_name, const std::vector<TString>& p_labels, TString p_axis) {
		TString p_key = p_name;
		if (!(prefix_.EqualTo(""))) {
			p_key = p_name;
			p_name = prefix_ + p_name;
		}
		if (th1_.find(p_key) == th1_.end()) {
			std::cerr << "[HistogramManager] ERROR : In AddBinLabels(), no histogram named " << p_name << " exists." << std::endl;
			exit(1);
		}
		TAxis *axis;
		if (p_axis.EqualTo("x")) {
			axis = th1_[p_key]->GetXaxis();
		} else if (p_axis.EqualTo("y")) {
			axis = th1_[p_key]->GetYaxis();
		} else if (p_axis.EqualTo("z")) {
			axis = th1_[p_key]->GetZaxis();
		} else {
			std::cerr << "[HistogramManager] ERROR : In AddBinLabels(), p_axis must be x, y, or z." << std::endl;
			exit(1);
		}
		if ((unsigned int)axis->GetNbins() != p_labels.size()) {
			std::cerr << "[HistogramManager] ERROR : In AddBinLabels(), number of bin labels (" << p_labels.size() << ") != number of bins (" << axis->GetNbins() << ") for histogram " << p_name << std::endl;
			exit(1);
		}
		
		for (int bin = 1; bin <= axis->GetNbins(); ++bin) {
			axis->SetBinLabel(bin, p_labels[bin-1]);
		}
	}


	TH1* HistogramManager::operator[](TString p_key) {
		return th1_[p_key];
	}

	void HistogramManager::SaveAll(TFile *f) {
		if (fs_) {
			std::cerr << "[HistogramManager] WARNING : SaveAll called, but histograms are always saved to a TFileService! Saving anyways, but this is redundant and less supported." << std::endl;
		}

		bool make_backup_file = false;
		if (f == 0x0) {
			std::cerr << "[HistogramManager] WARNING : Save file does not exist! Creating a backup file at HistogramManagerBackup.root." << std::endl;
			f = new TFile("HistogramManagerBackup.root", "UPDATE");
			make_backup_file = true;
		}

		f->cd();

		for (std::vector<TString>::iterator it = key_list_.begin(); it != key_list_.end(); ++it) {
			if (IsA(*it) == t_TH1F) {
				static_cast<TH1F*>(th1_[*it])->Write(th1_[*it]->GetName());
			} else if (IsA(*it) == t_TH1D) {
				static_cast<TH1D*>(th1_[*it])->Write(th1_[*it]->GetName());
			} else if (IsA(*it) == t_TH2F) {
				static_cast<TH2F*>(th1_[*it])->Write(th1_[*it]->GetName());
			} else if (IsA(*it) == t_TH2D) {
				static_cast<TH2D*>(th1_[*it])->Write(th1_[*it]->GetName());
			} else if (IsA(*it) == t_TH3F) {
				static_cast<TH3F*>(th1_[*it])->Write(th1_[*it]->GetName());
			} else if (IsA(*it) == t_TH3D) {
				static_cast<TH3D*>(th1_[*it])->Write(th1_[*it]->GetName());
			}
			// Do we need to do this to ensure the HistogramManager keeps ownership?
			th1_[*it]->SetDirectory(0);
		}

		if (make_backup_file) {
			f->Close();
			delete f;
		}
	}
 
	void HistogramManager::CheckName(TString p_name) {
		if (!fs_) {
			std::cerr << "[HistogramManager] ERROR : Attempt to register histogram without setting TFileService first." << std::endl;
			exit(1);
		}
		if (std::find(key_list_.begin(), key_list_.end(), p_name) != key_list_.end()) {
			std::cerr << "[HistogramManager] ERROR : Attempt to add already existing histogram " << p_name << ". Please use a unique name." << std::endl;
			std::exit(1);
		} else if (p_name.EqualTo("")) {
			std::cerr << "[HistogramManager] ERROR : Attempt to add histogram with name=\"\". Please specify a real name." << std::endl;
			std::exit(1);
		}
		key_list_.push_back(p_name);
	}

	HistogramManager::HistogramType HistogramManager::IsA(TString p_key) {
		return histogram_types_[p_key];
	}

	TH1F* HistogramManager::GetTH1F(TString p_name) {
		if (th1f_.find(p_name) != th1f_.end()) {
			return th1f_[p_name];
		} else {
			std::cerr << "[HistogramManager] WARNING : TH1F " << p_name << " not known. HistogramManager prefix = " << prefix_ << ". Returning 0." << std::endl;
			return 0;
		}
	}

	TH1D* HistogramManager::GetTH1D(TString p_name) {
		if (th1d_.find(p_name) != th1d_.end()) {
			return th1d_[p_name];
		} else {
			std::cerr << "[HistogramManager] WARNING : TH1D " << p_name << " not known. HistogramManager prefix = " << prefix_ << ". Returning 0." << std::endl;
			return 0;
		}
	}

	TH2F* HistogramManager::GetTH2F(TString p_name) {
		if (th2f_.find(p_name) != th2f_.end()) {
			return th2f_[p_name];
		} else {
			std::cerr << "[HistogramManager] WARNING : TH2F " << p_name << " not known. HistogramManager prefix = " << prefix_ << ". Returning 0." << std::endl;
			return 0;
		}
	}

	TH2D* HistogramManager::GetTH2D(TString p_name) {
		if (th2d_.find(p_name) != th2d_.end()) {
			return th2d_[p_name];
		} else {
			std::cerr << "[HistogramManager] WARNING : TH2D " << p_name << " not known. HistogramManager prefix = " << prefix_ << ". Returning 0." << std::endl;
			return 0;
		}
	}

	TH3F* HistogramManager::GetTH3F(TString p_name) {
		if (th3f_.find(p_name) != th3f_.end()) {
			return th3f_[p_name];
		} else {
			std::cerr << "[HistogramManager] WARNING : TH3F " << p_name << " not known. HistogramManager prefix = " << prefix_ << ". Returning 0." << std::endl;
			return 0;
		}
	}

	TH3D* HistogramManager::GetTH3D(TString p_name) {
		if (th3d_.find(p_name) != th3d_.end()) {
			return th3d_[p_name];
		} else {
			std::cerr << "[HistogramManager] WARNING : TH3D " << p_name << " not known. HistogramManager prefix = " << prefix_ << ". Returning 0." << std::endl;
			return 0;
		}
	}

	void HistogramManager::AddPrefix(TString p_prefix) {
		if (th1_.size() > 0) {
			std::cerr << "[HistogramManager] WARNING : Attempt to add a histogram prefix after histograms have already been added. Please do this before making any histograms. Doing nothing for now!" << std::endl;
		} else {
			prefix_ = p_prefix;
		}
	}

}



#endif