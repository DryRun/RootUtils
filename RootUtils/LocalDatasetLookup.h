#ifndef LocalDatasetLookup_h
#define LocalDatasetLookup_h

#include <iostream>
#include <vector>
#include <map>

#include "RootUtils/SimpleConfiguration.h"
#include "TString.h"

class LocalDatasetLookup {

	/*** Public Methods ***/
public:
	LocalDatasetLookup();

	~LocalDatasetLookup();

	std::vector<TString> GetLocalDatasetFiles(TString p_dataset_name);


};


#endif