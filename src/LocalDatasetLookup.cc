#ifndef LocalDatasetLookup_cxx
#define LocalDatasetLookup_cxx

#include "MyTools/RootUtils/interface/LocalDatasetLookup.h"

LocalDatasetLookup::LocalDatasetLookup() {

}

LocalDatasetLookup::~LocalDatasetLookup() {

}

std::vector<TString> LocalDatasetLookup::GetLocalDatasetFiles(TString p_dataset_name) {
	std::vector<TString> c_dataset_file_list;
	p_dataset_name.ReplaceAll(".root/", ".root");
	SimpleConfiguration *c_files = new SimpleConfiguration();
	TString config_path = "$ROOTCOREDIR/data/RootUtils/LocalDatasets/" + p_dataset_name + ".xml";
	config_path.ReplaceAll("/.xml", ".xml"); // Address the case where the "/" hasn't been removed from the dataset name
	if (!c_files->LoadConfiguration(config_path)) {
		std::cerr << "[LocalDatasetLookup] ERROR : Couldn't find XML file for dataset " << p_dataset_name << std::endl;
		exit(1);
	} else {
		for (std::vector<std::pair<TString, TString> >::iterator it = c_files->GetList("LocalFiles")->begin(); it != c_files->GetList("LocalFiles")->end(); ++it) {
			c_dataset_file_list.push_back((*it).second);
		}
		std::cout << "[LocalDatasetLookup] INFO : Found " << c_dataset_file_list.size() << " local files for dataset " << p_dataset_name << std::endl;
	}
	return c_dataset_file_list;
}


#endif