import ROOT
from ROOT import TH1F
from ROOT import TH1D
from ROOT import TH2F
from ROOT import TH2D
from ROOT import TH3F
from ROOT import TH3D
import math
from math import sqrt

def divide_histograms_binomial(h_num, h_den):
	if not h_num.GetNbinsX() == h_den.GetNbinsX():
		raise Exception("Histograms have different numbers of bins") 
	h_ratio = h_num.Clone()
	h_ratio.SetName(h_num.GetName() + "_over_" + h_den.GetName())
	for bin in xrange(0, h_num.GetNbinsX() + 2):
		num = h_num.GetBinContent(num)
		den = h_den.GetBinContent(bin)
		if den > 0 and num > 0 and num <= den:
			ratio = num / den
			err = max(sqrt(ratio * (1. - ratio) / den), 1. / den)
			h_ratio.SetBinContent(bin, ratio)
			h_ratio.SetBinError(bin, err)
		elif den == 0 and num == 0:
			# Allowable values for which the ratio is ill defined
			h_ratio.SetBinContent(bin, 0)
			h_ratio.SetBinError(bin, 0)
		else:
			raise ValueError("Bad values: " + str(num) + " / " + str(den))
	return h_ratio

def rebin_histogram(histogram, bins, normalization_bin_width=None):
	histogram_copy = histogram.Clone()
	histogram_copy = histogram_copy.Rebin(len(bins) - 1, histogram.GetName() + "_rebinned", bins)
	histogram_copy.SetDirectory(0)
	if normalization_bin_width:
		for bin in xrange(1, histogram_copy.GetNbinsX() + 1):
			histogram_copy.SetBinContent(bin, histogram_copy.GetBinContent(bin) / histogram_copy.GetXaxis().GetBinWidth(bin) * normalization_bin_width)
			histogram_copy.SetBinError(bin, histogram_copy.GetBinError(bin) / histogram_copy.GetXaxis().GetBinWidth(bin) * normalization_bin_width)
	return histogram_copy
