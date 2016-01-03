#ifndef StandaloneJobInfoContainer_h
#define StandaloneJobInfoContainer_h

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <getopt.h>
#include <stdlib.h>

#include "TROOT.h"
#include "TString.h"
#include "TFile.h"
#include "TSystem.h"
#include "TMath.h"
#include "TCollection.h"
#include "TXMLNode.h"
#include "TXMLDocument.h"
#include "TDOMParser.h"
#include "TXMLAttr.h"
#include "TList.h"


class StandaloneJobInfoContainer {
public:
	StandaloneJobInfoContainer();
	~StandaloneJobInfoContainer();

	struct ObjectDefinitionInfo {
		TString Name;
		TString Type;
		TString Description;
		std::vector<TString> Triggers;
	};

	TString SubmissionMethod;
	TString JobDescription;
	TString D3PDName;
	TString D3PDJobName;
	TString GridOutputName; // Grid output dataset name
	bool IsMC;
	TString MCProcess;
	TString MCGenerator;
	double MCCrossSection;
	double Luminosity;
	std::vector<ObjectDefinitionInfo*> ObjectDefinitions;

	void SaveXMLFile(TString path);
	bool LoadXMLFile(TString path, bool print = false);
	void PrintJobInfo();

};


#endif