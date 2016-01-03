import os
import sys
import glob
from datetime import datetime
import re
import ROOT
from ROOT import *
sys.path.append("~/Multilepton/Code/VectorLeptons/RootUtils/python/")

# Get list of local files in a dataset using rucio-ls
def IndexLocalFiles(p_dataset_name):
	c_datetime = datetime.now()
	c_ts_string = str(c_datetime.year) + "-" + str(c_datetime.month) + "-" + str(c_datetime.day) + "T" + str(c_datetime.hour) + "-" + str(c_datetime.month) + "-" + str(c_datetime.second)
	tmp_file_name = "~/tmp_" + c_ts_string + ".txt"
	# srm://pdsfdtn1.nersc.gov:62443/srm/v2/server?SFN=/eliza18/atlas/atlasdata/atlaslocalgroupdisk/rucio/data13_2p76TeV/ca/75/data13_2p76TeV.00219257.calibration_PixelBeam.merge.NTUP_IDVTXLUMI.c829_m1297._0002.1

	local_files = []

	rucio_command = "rucio list-file-replicas " + p_dataset_name + " --rse NERSC_LOCALGROUPDISK | grep srm:// | sed 's/^.*\/eliza18/\/eliza18/' >& " + tmp_file_name
	print rucio_command
	os.system(rucio_command)
	file_list_txt = open(tmp_file_name, 'r')
	for line in file_list_txt:
		local_files.append(line.rstrip())
	os.system("rm -f " + tmp_file_name)
	return local_files


# Save list of local files to an XML file, using IndexLocalFiles
def AddDataset(p_dataset_name):
	if "*" in p_dataset_name:
		raise Exception("[AddDataset] ERROR : Cannot have wildcards in dataset name: " + p_dataset_name)
		sys.exit(1)
	# Remove trailing /
	#if p_dataset_name[-1:] == "/":
	#	p_dataset_name = p_dataset_name[:-1]

	dataset_name_noslash = p_dataset_name
	if dataset_name_noslash[-1:] == "/":
		dataset_name_noslash = dataset_name_noslash[:-1]
	
	local_files = IndexLocalFiles(p_dataset_name)
	print "[AddDataset] INFO : Found " + str(len(local_files)) + " files for dataset " + p_dataset_name
	if len(local_files) > 0:
		c_dataset_save = SimpleConfiguration()
		c_dataset_save.AddElement("DatasetName", p_dataset_name)
		c_dataset_save.AddList("LocalFiles")
		for c_file in local_files:
			c_dataset_save.AddListItem("LocalFiles", "", c_file)
		c_dataset_save.SaveConfiguration("$ROOTCOREBIN/data/RootUtils/LocalDatasets/" + dataset_name_noslash + ".xml")
	else:
		print "[AddDataset] INFO : No files, doing nothing."

def GetLocalFiles(p_dataset_pattern):
	if p_dataset_pattern[-1:] == "/":
		p_dataset_pattern = p_dataset_pattern[:-1]
	c_files = []
	glob_pattern_ts = TString("$ROOTCOREBIN/data/RootUtils/LocalDatasets/" + p_dataset_pattern + ".xml")
	gSystem.ExpandPathName(glob_pattern_ts)
	glob_pattern = glob_pattern_ts.Data()
	#print "[GetLocalFiles] DEBUG : Globbing " + glob_pattern
	file_list_candidates = glob.glob(glob_pattern)
	if len(file_list_candidates) == 0:
		# Try to find files
		print "[GetLocalFiles] Couldn't find local XML file corresponding to dataset pattern {}. Trying to query rucio now...".format(p_dataset_pattern)
		AddDataset(p_dataset_pattern)
		file_list_candidates = glob.glob(glob_pattern)
		if len(file_list_candidates) == 0:
			raise Exception("Failed to look up dataset " + p_dataset_pattern + ". Try by hand or correct dataset name: \npython $ROOTCOREBIN/python/RootUtils/DQ2Helper.py --addLocalDataset " + p_dataset_pattern + "\n")
	elif len(file_list_candidates) > 1:
		raise Exception("Found several XML files corresponding to dataset pattern " + p_dataset_pattern)
	file_list = SimpleConfiguration()
	file_list.LoadConfiguration(file_list_candidates[0])
	for c_listitem in file_list.GetList("LocalFiles"):
		c_files.append(c_listitem.second)
	return c_files


def AddDataset(p_dataset_name):
	# Remove trailing /
	if p_dataset_name[-1:] == "/":
		p_dataset_name = p_dataset_name[:-1]

	c_files = IndexLocalFiles(p_dataset_name)
	c_dataset_save = SimpleConfiguration()
	c_dataset_save.AddElement("DatasetName", p_dataset_name)
	c_dataset_save.AddList("LocalFiles")
	for c_file in c_files:
		c_dataset_save.AddListItem("LocalFiles", "", c_file)
	c_dataset_save.SaveConfiguration("$ROOTCOREDIR/data/RootUtils/LocalDatasets/" + p_dataset_name + ".xml")

if __name__ == "__main__":
	# Note: this cannot be called more than once! 
	gROOT.ProcessLine(".x $ROOTCOREDIR/scripts/load_packages.C")
	import argparse
	parser = argparse.ArgumentParser(description = 'Local dataset management')
	parser.add_argument('--addLocalDataset', type=str, help='Search for a dataset locally and save list of files')
	parser.add_argument('--listLocalDataset', type=str, help='List local dataset')
	args = parser.parse_args()

	if args.addLocalDataset:
		#AddDataset(args.addLocalDataset)
		AddDataset(args.addLocalDataset)
	elif args.listLocalDataset:
		#c_files = GetLocalFiles(args.listLocalDataset)
		c_files = IndexLocalFiles(args.listLocalDataset)
		for c_file in c_files:
			print "\t" + c_file
	print "All done."