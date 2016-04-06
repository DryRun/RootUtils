#ifndef CommonFunctions_h
#define CommonFunctions_h

#include <iostream>
#include <istream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <getopt.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <iterator>

#include "TROOT.h"
#include "TString.h"
#include "TH1D.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TLeafI.h"
#include "TLeafD.h"
#include "TMath.h"
#include "TChain.h"
#include "TSystem.h"
#include "TSystemDirectory.h"
#include "TCollection.h"
#include "TList.h"

namespace Root {

	double GetInvariantMass(double m1, double e1, double eta1, double phi1, double m2, double e2, double eta2, double phi2);

	/**
	  * GetCurrentTimestamp
	  * - Returns a TString of the current Datime.
	  */
	TString GetCurrentTimestamp();


	/**
	  * RoundFloat
	  * - Returns a TString of a float to the specified number of decimal places
	  */
	TString RoundFloat(double p_number, Int_t p_decimal_precision);

	/**
	  * PrintHistogramBins
	  * - Prints bins of histogram, separated by tabs.
	  */
	void PrintHistogramBins(TH1D *h);


	/**
	  * PrintHistogramContents
	  * - Prints each bin content of histogram h.
	  */
	void PrintHistogramContents(TH1D *h);

	/**
	  * SplitTString
	  * - Returns a vector of TStrings, consisting of string s_in split with s_delim.
	  */
	std::vector<TString> SplitTString(TString s_in, char delim);

	/**
	  * ListFiles
	  * - Lists all files in dirname containing file_pattern.
	  */
	std::vector<TString> ListFiles(TString dirname, TString file_pattern="NTUP_IDVTXLUMI");

	TGraphAsymmErrors* ConvertTH1DToTGraphAsymmErrors(TH1D* p_hist);

	TGraphAsymmErrors* ConvertTH1DToTGraphAsymmErrorsPoisson(TH1D* p_hist);

	TH1D* ConvertTGraphToTH1D(TGraphErrors* p_tgraph);

	TH1D* CombineHistogramBins(TH1D* p_hist, Int_t p_lowbin, Int_t p_highbin, Bool_t p_rescale);
}

#endif