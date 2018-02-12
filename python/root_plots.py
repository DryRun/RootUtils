import os
import sys
import copy
import ROOT
from array import array
from ROOT import *
import math
from math import sqrt

sys.path.append("/uscms/home/dryu/Dijets/CMSSW_7_4_15/python/MyTools/RootUtils")
import histogram_tools

gROOT.SetBatch(True)
gStyle.SetOptStat(0)
gStyle.SetOptTitle(0)

# Hack for newer ROOT versions
if "CMSSW_7" in os.path.expandvars("$CMSSW_BASE"):
	ROOT.gInterpreter.Declare("#include \"MyTools/RootUtils/interface/CanvasHelpers.h\"")
	ROOT.gInterpreter.Declare("#include \"MyTools/RootUtils/interface/SeabornInterface.h\"")
gSystem.Load(os.path.expandvars("$CMSSW_BASE/lib/slc6_amd64_gcc491/libMyToolsRootUtils.so"))
seaborn = Root.SeabornInterface()
seaborn.Initialize()
Root.SetCanvasStyle()

# Plot: data compared with background estimate, specified as components.
class DataMCPlot():
	def __init__(self):
		self._data_histogram = None
		self._data_name = ""
		self._mc_histograms = {}
		self._mc_names = []
		self._mc_colors = {}
		self._mc_sum = None

		self._signal_names = []
		self._signal_histograms = {}
		self._signal_colors = {}
		self._signal_styles = {}
		self._stack_signals = False
		self._signal_inc_pull = {}
		self._signal_plot_pull = {}

		self._tf1_names = []
		self._tf1s = {}
		self._tf1_colors = {}

		self._bottom_fraction = 0.32

		self._canvas = None
		self._top = None
		self._bottom = None
		self._legend = None

		self._draw_tf1only = False

	def add_data(self, hist, name="Data"):
		self._data_histogram = hist
		self._data_name = name

	def add_mc(self, mc_hist, mc_name, color=None):
		self._mc_names.append(mc_name)
		self._mc_histograms[mc_name] = mc_hist
		if color:
			self._mc_colors[mc_name] = color
		if self._mc_sum:
			self._mc_sum.Add(mc_hist)
		else:
			self._mc_sum = mc_hist.Clone()
			self._mc_sum.SetDirectory(0)
			self._mc_sum.SetName("mc_sum")

	def add_tf1(self, tf1, name, color=None):
		self._tf1_names.append(name)
		self._tf1s[name] = tf1
		if color:
			self._tf1_colors[name] = color

	# Skip the drawing of MC histograms, and only draw TF1s
	def draw_tf1only(self, draw):
		self._draw_tf1only = draw

	def add_signal(self, signal_hist, signal_name, color=None, line_style=1, stacked=False, inc_pull=False, plot_pull=False):
		self._signal_names.append(signal_name)
		self._signal_histograms[signal_name] = signal_hist
		if color:
			self._signal_colors[signal_name] = color
		self._signal_styles[signal_name] = line_style
		if stacked:
			self._stack_signals = True
		self._signal_inc_pull[signal_name] = inc_pull
		self._signal_plot_pull[signal_name] = plot_pull

	def draw(self, binning=None, logy=False, draw_ratio=False, draw_pull=False, x_range=None, y_range=None, ratio_range=None, pull_range=None, pull_dataerrors=False, save_tag="", cms_label=None, x_title=None, y_title=None, ratio_title=None, color_scheme="cubehelixhuge", complex_rebinning=None, simple_rebinning=None, legend_position="topright", lumi_string=None, save_directory=None):
		print "[debug] x_range = ",
		print x_range
		if complex_rebinning:
			self._data_histogram = histogram_tools.rebin_histogram(self._data_histogram, complex_rebinning, normalization_bin_width=1.)
			self._mc_sum = histogram_tools.rebin_histogram(self._mc_sum, complex_rebinning, normalization_bin_width=1.)
			for mc_name in self._mc_names:
				self._mc_histograms[mc_name] = histogram_tools.rebin_histogram(self._mc_histograms[mc_name], complex_rebinning, normalization_bin_width=1.)
				self._mc_histograms[mc_name].SetDirectory(0)
			for signal_name in self._signal_names:
				self._signal_histograms[signal_name] = histogram_tools.rebin_histogram(self._signal_histograms[signal_name], complex_rebinning, normalization_bin_width=1.)
				self._signal_histograms[signal_name].SetDirectory(0)
		elif simple_rebinning:
			self._data_histogram.Rebin(simple_rebinning)
			for mc_name in self._mc_names:
				self._mc_histograms[mc_name].Rebin(simple_rebinning)
			for signal_name in self._signal_names:
				self._signal_histograms[signal_name].Rebin(simple_rebinning)

		if draw_ratio or draw_pull:
			self._canvas = TCanvas("c_" + save_tag, "c_" + save_tag, 800, 800)
			self._top = TPad("top_" + save_tag, "top_" + save_tag, 0., self._bottom_fraction, 1., 1.)
			self._top.SetTopMargin(0.07)
			self._top.SetBottomMargin(0.03)
			self._top.Draw()
			self._canvas.cd()
			self._bottom = TPad("bottom_" + save_tag, "bottom_" + save_tag, 0., 0., 1., self._bottom_fraction)
			self._bottom.SetTopMargin(0.04)
			self._bottom.SetBottomMargin(0.28)
			self._bottom.Draw()
			self._canvas.cd()
		else:
			self._canvas = TCanvas("c_" + save_tag, "c_" + save_tag, 800, 600)

		if legend_position == "topright":
			self._legend = TLegend(0.5, 0.53, 0.92, 0.87)
		elif legend_position == "topleft":
			self._legend = TLegend(0.2, 0.6, 0.45, 0.8)
		elif legend_position == "bottomright":
			self._legend = TLegend(0.6, 0.2, 0.85, 0.4)
		elif legend_position == "bottomleft":
			self._legend = TLegend(0.2, 0.2, 0.45, 0.4)
		self._legend.SetFillColor(0)
		self._legend.SetBorderSize(0)

		if draw_ratio or draw_pull:
			self._top.cd()
		self._frame_top = self._data_histogram.Clone()
		self._frame_top.Reset()
		if logy:
			if draw_ratio or draw_pull:
				self._top.SetLogy()
			else:
				self._canvas.SetLogy()
		if x_range:
			self._frame_top.GetXaxis().SetRangeUser(x_range[0], x_range[1])
		if y_range:
			y_min = y_range[0]
			y_max = y_range[1]
		else:
			if logy:
				y_min = 0.1
				y_max = max(self._data_histogram.GetMaximum(), self._mc_sum.GetMaximum()) * 50.
			else:
				y_min = 0.
				y_max = max(self._data_histogram.GetMaximum(), self._mc_sum.GetMaximum()) * 1.3
		self._frame_top.SetMinimum(y_min)
		self._frame_top.SetMaximum(y_max)
		if draw_ratio or draw_pull:
			self._frame_top.GetXaxis().SetLabelSize(0)
			self._frame_top.GetXaxis().SetTitleSize(0)
		else:
			if x_title:
				self._frame_top.GetXaxis().SetTitle(x_title)
		if y_title:
			self._frame_top.GetYaxis().SetTitle(y_title)
		self._frame_top.GetYaxis().SetTitleSize(0.06)
		self._frame_top.GetYaxis().SetTitleOffset(0.95)
		self._frame_top.GetYaxis().SetLabelSize(0.035 / (1.-self._bottom_fraction))
		self._frame_top.Draw("axis")

		# MC: sort histograms by integral and stack
		for tf1_name in self._tf1_names:
			if tf1_name in self._tf1_colors:
				self._tf1s[tf1_name].SetLineColor(self._tf1_colors[tf1_name])
				self._tf1s[tf1_name].SetLineWidth(2)
				self._tf1s[tf1_name].SetLineStyle(1)
				self._tf1s[tf1_name].Draw("same")

		if not self._draw_tf1only:
			mc_names_sorted = sorted(self._mc_names, key=lambda x: self._mc_histograms[x].Integral())
			self._mc_stack = THStack("mc_stack", "mc_stack")
			style_counter = 0
			for mc_name in mc_names_sorted:
				if mc_name in self._mc_colors:
					self._mc_histograms[mc_name].SetFillColor(self._mc_colors[mc_name])
				else:
					self._mc_histograms[mc_name].SetFillColor(seaborn.GetColorRoot(color_scheme, style_counter, len(mc_names_sorted)))
				self._mc_histograms[mc_name].SetLineColor(kBlack)
				self._mc_histograms[mc_name].SetLineWidth(1)
				self._mc_histograms[mc_name].SetFillStyle(1001)
				self._mc_histograms[mc_name].SetMarkerStyle(20)
				self._mc_histograms[mc_name].SetMarkerSize(0)
				self._mc_stack.Add(self._mc_histograms[mc_name])
			self._mc_stack.Draw("hist same")
			ROOT.SetOwnership(self._mc_stack, False)
			for mc_name in mc_names_sorted:
				ROOT.SetOwnership(self._mc_histograms[mc_name], False)


		for signal_name in self._signal_names:
			if self._stack_signals:
				self._signal_histograms[signal_name].Add(self._mc_sum)
			if signal_name in self._signal_styles:
				self._signal_histograms[signal_name].SetLineStyle(self._signal_styles[signal_name])
			if signal_name in self._signal_colors:
				self._signal_histograms[signal_name].SetLineColor(self._signal_colors[signal_name])
			self._signal_histograms[signal_name].SetLineWidth(3)
			self._signal_histograms[signal_name].Draw("hist same ][")

		self._data_histogram.SetMarkerStyle(20)
		self._data_histogram.SetMarkerSize(1)
		self._data_histogram.SetMarkerColor(kBlack)
		self._data_histogram.Draw("same")
		print "[debug] Data integral = " + str(self._data_histogram.Integral()) + " / MC integral = " + str(self._mc_sum.Integral())
		self._frame_top.Draw("axis same")

		self._legend.AddEntry(self._data_histogram, self._data_name, "ep")
		for tf1_name in self._tf1_names:
			self._legend.AddEntry(self._tf1s[tf1_name], tf1_name, "l")
		if not self._draw_tf1only:
			for mc_name in reversed(mc_names_sorted):
				self._legend.AddEntry(self._mc_histograms[mc_name], mc_name, "lf")
		for signal_name in self._signal_names:
			self._legend.AddEntry(self._signal_histograms[signal_name], signal_name, "l")
		self._legend.Draw()
		if cms_label != None:
			if draw_ratio or draw_pull:
				cms_label_offset = 1. - self._bottom_fraction
			else:
				cms_label_offset = 1.
			Root.CMSLabel(0.22, 0.8, cms_label, 1, 0.65, cms_label_offset); 

		if lumi_string:
			Root.myText(0.74, 0.95, kBlack, lumi_string, 0.57)

		if draw_ratio:
			self._canvas.cd()
			self._bottom.cd()
			self._frame_bottom = self._data_histogram.Clone()
			self._frame_bottom.Reset()
			if ratio_range:
				self._frame_bottom.SetMinimum(ratio_range[0])
				self._frame_bottom.SetMaximum(ratio_range[1])
			else:
				self._frame_bottom.SetMinimum(0.)
				self._frame_bottom.SetMaximum(2.)
			if x_range:
				self._frame_bottom.GetXaxis().SetRangeUser(x_range[0], x_range[1])
			if x_title:
				self._frame_bottom.GetXaxis().SetTitle(x_title)
			self._frame_bottom.GetXaxis().SetTitleSize(0.04 / self._bottom_fraction)
			self._frame_bottom.GetXaxis().SetTitleOffset(1.15)
			self._frame_bottom.GetXaxis().SetLabelOffset(0.005 * 4.4)

			self._frame_bottom.GetYaxis().SetTitleSize(0.04 / self._bottom_fraction)
			self._frame_bottom.GetYaxis().SetLabelSize(0.04 / self._bottom_fraction)
			self._frame_bottom.GetYaxis().SetTitleOffset(0.6)
			self._frame_bottom.GetYaxis().SetLabelOffset(0.005 * 2.2)

			self._frame_bottom.GetYaxis().SetTitle("Data / Background")
			self._frame_bottom.Draw("axis")

			self._ratio_histogram = self._data_histogram.Clone()
			self._ratio_histogram.Reset()
			for bin in xrange(1, self._data_histogram.GetNbinsX() + 1):
				num = self._data_histogram.GetBinContent(bin)
				num_err = self._data_histogram.GetBinError(bin)
				den = self._mc_sum.GetBinContent(bin)
				den_err = self._mc_sum.GetBinError(bin)
				if num > 0 and den > 0:
					ratio = num / den
					ratio_err = ratio * sqrt((num_err/num)**2 + (den_err/den)**2)
				else:
					ratio = 0.
					ratio_err = 0.
				self._ratio_histogram.SetBinContent(bin, ratio)
				self._ratio_histogram.SetBinError(bin, ratio_err)
				print "[debug] Set ratio(mjj=" + str(self._data_histogram.GetXaxis().GetBinCenter(bin)) + ") = " + str(ratio) 
			self._ratio_histogram.SetMarkerStyle(24)
			self._ratio_histogram.SetMarkerSize(1)
			self._ratio_histogram.SetMarkerColor(kBlack)
			self._ratio_histogram.SetLineWidth(1)
			self._ratio_histogram.SetLineColor(kBlack)
			self._ratio_histogram.Draw("same")
			self._frame_bottom.Draw("axis same")
		elif draw_pull:
			self._canvas.cd()
			self._bottom.cd()
			self._frame_bottom = self._data_histogram.Clone()
			self._frame_bottom.Reset()
			if pull_range:
				self._frame_bottom.SetMinimum(pull_range[0])
				self._frame_bottom.SetMaximum(pull_range[1])
			else:
				self._frame_bottom.SetMinimum(-3.)
				self._frame_bottom.SetMaximum(3.)
			if x_range:
				self._frame_bottom.GetXaxis().SetRangeUser(x_range[0], x_range[1])
			if pull_dataerrors:
				self._frame_bottom.GetYaxis().SetTitle("#frac{(Data - Bkgd)}{#sqrt{Data}}")
			else:
				self._frame_bottom.GetYaxis().SetTitle("#frac{(Data - Bkgd)}{#sqrt{Bkgd}}")
			if x_title:
				self._frame_bottom.GetXaxis().SetTitle(x_title)
			self._frame_bottom.GetXaxis().SetTitleSize(0.035 / self._bottom_fraction)
			self._frame_bottom.GetXaxis().SetLabelSize(0.035 / self._bottom_fraction)
			self._frame_bottom.GetXaxis().SetTitleOffset(1.07)
			self._frame_bottom.GetXaxis().SetLabelOffset(0.005 * 4.2)
			self._frame_bottom.GetYaxis().SetTitleSize(0.035 / self._bottom_fraction)
			self._frame_bottom.GetYaxis().SetLabelSize(0.035 / self._bottom_fraction)
			self._frame_bottom.GetYaxis().SetTitleOffset(0.55)
			self._frame_bottom.GetYaxis().SetLabelOffset(0.005 * 2.2)
			self._frame_bottom.Draw("axis")

			self._pull_histogram = self._data_histogram.Clone()
			self._pull_histogram.Reset()
			self._systematic_histogram = self._data_histogram.Clone()
			self._systematic_histogram.Reset()
			for bin in xrange(1, self._data_histogram.GetNbinsX() + 1):
				data = self._data_histogram.GetBinContent(bin)
				data_err = self._data_histogram.GetBinError(bin)
				background = self._mc_sum.GetBinContent(bin)
				background_err = self._mc_sum.GetBinError(bin)

				if complex_rebinning:
						# Bins are scaled by bin width. Need to undo before calculating background uncertainty.
					bin_width = self._data_histogram.GetXaxis().GetBinWidth(bin)
					data = data * bin_width
					data_err = data_err * bin_width
					background = background * bin_width
					background_err = background_err * bin_width
				if background > 0:
					if pull_dataerrors:
						pull = (data - background) / data_err
					else:
						pull = (data - background) / sqrt(background)
					syst = background_err / sqrt(background)
				else:
					pull = 0.
					syst = 0.

				self._pull_histogram.SetBinContent(bin, pull)
				self._pull_histogram.SetBinError(bin, 0.)
				self._systematic_histogram.SetBinContent(bin, 0.)
				self._systematic_histogram.SetBinError(bin, syst)
			self._systematic_histogram.SetFillColor(17)
			self._systematic_histogram.SetFillStyle(1001)
			self._systematic_histogram.SetMarkerStyle(20)
			self._systematic_histogram.SetMarkerSize(0)
			#self._systematic_histogram.Draw("e2 hist same")
			self._pull_histogram.SetMarkerStyle(24)
			self._pull_histogram.SetMarkerSize(1)
			self._pull_histogram.SetMarkerColor(kBlack)
			self._pull_histogram.SetLineStyle(1)
			self._pull_histogram.SetLineWidth(2)
			self._pull_histogram.SetLineColor(1)
			self._pull_histogram.SetFillStyle(1001)
			self._pull_histogram.SetFillColor(seaborn.GetColorRoot("default", 2))
			#self._pull_histogram.SetLineWidth(1)
			#self._pull_histogram.SetLineColor(kBlack)
			self._pull_histogram.Draw("hist same")
			self._frame_bottom.Draw("axis same")

			self._signal_pulls = {}
			for signal_name in self._signal_names:
				if self._signal_plot_pull[signal_name]:
					self._signal_pulls[signal_name] = self._signal_histograms[signal_name].Clone()
					for bin in xrange(1, self._signal_histograms[signal_name].GetNbinsX() + 1):
						signal_counts = self._signal_histograms[signal_name].GetBinContent(bin)
						signal_err = self._signal_histograms[signal_name].GetBinError(bin)
						background = self._mc_sum.GetBinContent(bin)
						background_err = self._mc_sum.GetBinError(bin)

						if complex_rebinning:
								# Bins are scaled by bin width. Need to undo before calculating background uncertainty.
							bin_width = self._data_histogram.GetXaxis().GetBinWidth(bin)
							signal_counts = signal_counts * bin_width
							signal_err = signal_err * bin_width
							background = background * bin_width
							background_err = background_err * bin_width
						if background > 0:
							if pull_dataerrors:
								pull = signal_counts / signal_err
							else:
								pull = signal_counts / sqrt(background)
						else:
							pull = 0.
						self._signal_pulls[signal_name].SetBinContent(bin, pull)
						self._signal_pulls[signal_name].SetBinError(bin, 0.)
					self._signal_pulls[signal_name].Draw("hist same")
		self._canvas.cd()

		if draw_pull or draw_ratio:
			ROOT.SetOwnership(self._canvas, False)
			ROOT.SetOwnership(self._top, False)
			ROOT.SetOwnership(self._bottom, False)
		if save_directory:
			self._canvas.cd()
			self._canvas.SaveAs(save_directory + "/" + self._canvas.GetName() + ".png")
			self._canvas.SaveAs(save_directory + "/" + self._canvas.GetName() + ".pdf")
			self._canvas.SaveAs(save_directory + "/" + self._canvas.GetName() + ".eps")
		return self._canvas

	def save(self, save_directory):
		print self._mc_stack
		for mc_name, mc_hist in self._mc_histograms.iteritems():
			print mc_hist
		self._canvas.cd()
		self._canvas.SaveAs(save_directory + "/" + self._canvas.GetName() + ".png")
		self._canvas.SaveAs(save_directory + "/" + self._canvas.GetName() + ".pdf")
		self._canvas.SaveAs(save_directory + "/" + self._canvas.GetName() + ".eps")
		print self._mc_stack
		for mc_name, mc_hist in self._mc_histograms.iteritems():
			print mc_hist

def EfficiencyPlot(h_num, h_den, name_num="Numerator", name_den="Denominator", legend_coordinates=[0.6,0.6,0.88,0.88], logy=False, formats=[".pdf", ".eps"], save_tag="", save_directory=os.path.expandvars("$HOME")):
	c = TCanvas("c_eff_" + save_tag, "c_eff_" + save_tag, 800, 1000)
	l = TLegend(legend_coordinates[0], legend_coordinates[1], legend_coordinates[2], legend_coordinates[3])
	l.SetFillColor(0)
	l.SetBorderSize(0)
	top = TPad("top", "top", 0., 0.5, 1., 1.)
	if logy:
		top.SetLogy()
	top.SetBottomMargin(0.02)
	top.Draw()
	top.cd()
	frame_top = h_num.Clone()
	frame_top.Reset()
	frame_top.GetXaxis().SetTitleSize(0)
	frame_top.GetXaxis().SetLabelSize(0)
	frame_top.GetYaxis().SetTitle(h_num.GetYaxis().GetTitle())
	if logy:
		ymin = 0.1
		ymax = 10. * max(h_num.GetMaximum(), h_den.GetMaximum())
	else:
		ymin = 0.
		ymax = 1.3 * max(h_num.GetMaximum(), h_den.GetMaximum())
	frame_top.SetMinimum(ymin)
	frame_top.SetMaximum(ymax)
	frame_top.Draw("axis")
	h_num.SetMarkerStyle(24)
	h_num.SetMarkerSize(1)
	h_num.SetMarkerColor(kBlack)
	h_num.Draw("same")
	h_den.SetMarkerStyle(20)
	h_den.SetMarkerSize(1)
	h_den.SetMarkerColor(kBlack)
	h_den.Draw("same")
	l.AddEntry(h_num, name_num, "pl")
	l.AddEntry(h_den, name_den, "pl")
	l.Draw()

	c.cd()
	bottom = TPad("bottom", "bottom", 0., 0., 1., 0.5)
	bottom.SetTopMargin(0.03)
	bottom.SetBottomMargin(0.23)
	bottom.Draw()
	bottom.cd()
	ratio = h_num.Clone()
	ratio.Divide(h_num, h_den, 1, 1, "B")
	ratio.GetYaxis().SetTitle("#epsilon=" + name_num + "/" + name_den)
	ratio.Draw()

	c.cd()
	for iformat in formats:
		c.SaveAs(save_directory + "/" + c.GetName() + "." + iformat)
	ROOT.SetOwnership(c, False)
	ROOT.SetOwnership(top, False)
	ROOT.SetOwnership(bottom, False)

