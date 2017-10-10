#!/usr/bin/env python
import os
import sys
import ROOT
from ROOT import *
import math

def CopyTree(input_tree, output_file):
	output_file.cd()
	output_tree = input_tree.CloneTree(0)
	input_tree.CopyAddresses(output_tree)
	return output_tree

def SplitTree(input_filename, tree_name, output_n, output_file_base=None):
	if not output_file_base:
		output_file_base = os.path.basename(input_filename)
		output_file_base.replace(".root", "")

	input_file = TFile(input_filename, "READ")
	input_tree = input_file.Get(tree_name)

	events_per_output_tree = int(math.ceil(float(input_tree.GetEntries()) / output_n))
	print "events_per_output_tree = {}".format(events_per_output_tree)
	output_file_counter = 0
	for entry in xrange(input_tree.GetEntriesFast()):
		if entry % events_per_output_tree == 0:
			# Make new file and tree
			print "Creating new file: {}".format(output_file_base + str(output_file_counter) + ".root")
			output_file = TFile(output_file_base + str(output_file_counter) + ".root", "RECREATE")
			output_tree = CopyTree(input_tree, output_file)
		input_file.cd()
		input_tree.GetEntry(entry)
		output_file.cd()
		output_tree.Fill()

		if entry % events_per_output_tree == -1 or entry == input_tree.GetEntriesFast() - 1:
			output_file.cd()
			output_tree.Write()
			output_file.Close()
			output_file_counter += 1
	input_file.Close()

if __name__ == "__main__":
	import argparse
	parser = argparse.ArgumentParser(description='Split a TTree into smaller files')
	parser.add_argument('input_filename', type=str, help="Input filename")
	parser.add_argument('tree_name', type=str, help="Tree name")
	parser.add_argument('nfiles', type=int, help="Number of output files")
	parser.add_argument('--output_base', type=str, help="Base of output filename (default is input filename)")
	args = parser.parse_args()
	SplitTree(args.input_filename, args.tree_name, args.nfiles, args.output_base)