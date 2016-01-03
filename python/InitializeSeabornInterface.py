# Script to convert the seaborn things into ROOT things, and save to text files

import os
import sys
import ROOT
from ROOT import *
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

gROOT.ProcessLine(".x $ROOTCOREDIR/scripts/load_packages.C")
gROOT.SetBatch(True)
SetAtlasStyle()

def InitializeColors():
	print "Welcome to InitializeColors()"
	
	output = SimpleConfiguration()

	# Default palettes: 6 colors
	default_palettes = ["default", "pastel", "muted", "deep", "dark", "colorblind"]
	for c_palette_name in default_palettes:
		if c_palette_name == "default":
			c_palette = sns.color_palette()
		else:
			c_palette = sns.color_palette(c_palette_name)
		output.AddList(c_palette_name)
		for c_color_rgb in c_palette:
			output.AddListItem(c_palette_name, "", str(c_color_rgb[0]) + "," + str(c_color_rgb[1]) + "," + str(c_color_rgb[2]))

	# Gradient palettes
	steps = 60 # Choose a large number with many divisors. 
	c_palette = sns.color_palette("husl", 60)
	output.AddList("husl")
	for c_color_rgb in c_palette:
		output.AddListItem("husl", "", str(c_color_rgb[0]) + "," + str(c_color_rgb[1]) + "," + str(c_color_rgb[2]))

	c_palette = sns.color_palette("hls", 60)
	output.AddList("hls")
	for c_color_rgb in c_palette:
		output.AddListItem("hls", "", str(c_color_rgb[0]) + "," + str(c_color_rgb[1]) + "," + str(c_color_rgb[2]))

	c_palette = sns.color_palette("coolwarm", 60)
	output.AddList("coolwarm")
	for c_color_rgb in c_palette:
		output.AddListItem("coolwarm", "", str(c_color_rgb[0]) + "," + str(c_color_rgb[1]) + "," + str(c_color_rgb[2]))

	# Black-and-white safe: "cubehelix", 8 colors
	c_palette = sns.cubehelix_palette(8, start=1., rot=2)
	output.AddList("cubehelix")
	for c_color_rgb in c_palette:
		output.AddListItem("cubehelix", "", str(c_color_rgb[0]) + "," + str(c_color_rgb[1]) + "," + str(c_color_rgb[2]))
	output.SaveConfiguration("$ROOTCOREDIR/data/RootUtils/SeabornInterface/ColorPalettes.xml")

	c_palette = sns.cubehelix_palette(30, start=0., rot=3)
	output.AddList("cubehelixlarge")
	for c_color_rgb in c_palette:
		output.AddListItem("cubehelixlarge", "", str(c_color_rgb[0]) + "," + str(c_color_rgb[1]) + "," + str(c_color_rgb[2]))
	output.SaveConfiguration("$ROOTCOREDIR/data/RootUtils/SeabornInterface/ColorPalettes.xml")

	# 6 rotations, 60 colors
	# Each 0-9 is roughly...
	# 0=blue
	# 3=green
	# 4=yellow
	# 5=orange
	# 7=purple
	c_palette = sns.cubehelix_palette(60, start=2.7, rot=8, light=0.75)
	output.AddList("cubehelixhuge")
	for c_color_rgb in c_palette:
		output.AddListItem("cubehelixhuge", "", str(c_color_rgb[0]) + "," + str(c_color_rgb[1]) + "," + str(c_color_rgb[2]))
	output.SaveConfiguration("$ROOTCOREDIR/data/RootUtils/SeabornInterface/ColorPalettes.xml")



def TestColors():
	print "Welcome to TestColors()"
	sbi = Root.SeabornInterface()
	sbi.Initialize()
	rand = TRandom3(100)
	for c_palette_name in sbi.GetListOfPalettes():
		c_palette_name = c_palette_name.Data()
		print "On palette " + c_palette_name
		c = TCanvas(c_palette_name, c_palette_name, 1200, 800)
		c.SetLogy()
		l = TLegend(0.7, 0.6, 0.94, 0.89)
		l.SetFillColor(0)
		l.SetBorderSize(0)
		# Make a bunch of histograms
		histograms = []
		c_maximum = 0
		for i in xrange(6):
			histograms.append(TH1D("hist_" + c_palette_name + "_" + str(i), "hist_" + c_palette_name + "_" + str(i), 20, 0., 1000.))
			# Make a random exponential function
			f = TF1("f_" + c_palette_name + "_" + str(i), "expo(0) + gaus(2)", 0., 1000.)
			f.SetParameter(0, (0.7*rand.Rndm() + 0.3)) # Normalization of exp
			f.SetParameter(1, -1. / ((0.8*rand.Rndm() + 0.2) * 1000.)) # Decay of exp
			f.SetParameter(2, rand.Rndm()**5) # Normalization of gaussian
			f.SetParameter(3, 1000. * rand.Rndm()) # Center of gaussian
			f.SetParameter(4, 50. * rand.Rndm()) # Width of gaussian
			print "Filling with random function " + str(f.GetParameter(0)) + "*exp(" + str(f.GetParameter(1)) + "*x) + " + str(f.GetParameter(2)) + "*exp(-(x-" + str(f.GetParameter(3)) + ")**2 / 2*" + str(f.GetParameter(4)) + "**2)"
			histograms[i].FillRandom("f_" + c_palette_name + "_" + str(i), 10000)
			histograms[i].Scale((0.3*rand.Rndm()+0.7) / histograms[i].Integral())
			if histograms[-1].GetMaximum() > c_maximum:
				c_maximum = histograms[-1].GetMaximum()

		for i in xrange(6):
			histograms[i].SetFillColor(sbi.GetColorRoot(c_palette_name, i))
			histograms[i].SetMaximum(0.25)
			histograms[i].SetMinimum(0.01)
			l.AddEntry(histograms[i], str(i), "f")
			if i == 0:
				draw_options = "hist"
			else:
				draw_options = "hist same"
			histograms[i].Draw(draw_options)
		histograms[0].Draw("axis same")
		l.Draw()
		c.SaveAs("$ROOTCOREDIR/data/RootUtils/SeabornInterface/" + c.GetName() + ".pdf")


if __name__ == "__main__":
	import argparse
	parser = argparse.ArgumentParser(description = 'Convert seaborn things into ROOT things')
	parser.add_argument('--colors', action='store_true', help='Convert seaborn color palettes')
	parser.add_argument('--testcolors', action='store_true', help='Test seaborn color palettes')
	args = parser.parse_args()

	if args.colors:
		InitializeColors()
	if args.testcolors:
		TestColors()

	print "All done."
