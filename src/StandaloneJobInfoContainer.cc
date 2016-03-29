#include "MyTools/RootUtils/interface/StandaloneJobInfoContainer.h"

StandaloneJobInfoContainer::StandaloneJobInfoContainer() {
	JobDescription = "";
	D3PDName = "";
	D3PDJobName = "";
	GridOutputName = "";
	MCProcess = "";
	MCCrossSection = -1.;

}

StandaloneJobInfoContainer::~StandaloneJobInfoContainer() {

}

void StandaloneJobInfoContainer::SaveXMLFile(TString path) {
	std::ofstream job_info(path);
	job_info << "<?xml version=\"1.0\"?>" << std::endl << std::endl;;
	job_info << "<!DOCTYPE JobInfo SYSTEM \"http://www-atlas.lbl.gov/~dryu/Multilepton/JobInfo.dtd\">" << std::endl << std::endl;
	job_info << "<JobInfo>" << std::endl;
	//job_info << "\t<ConfigurationName>JobInfo</ConfigurationName>" 	<< std::endl;
	job_info << "\t<JobDescription>" << JobDescription << "</JobDescription>" << std::endl;
	job_info << "\t<D3PDName>" 	<< D3PDName 	<< "</D3PDName>" << std::endl;
	job_info << "\t<D3PDJobName>" 	<< D3PDJobName 	<< "</D3PDJobName>" << std::endl;
	if (SubmissionMethod == TString("grid")) job_info << "\t<GridOutputName>" 	<< GridOutputName 	<< "</GridOutputName>" << std::endl;
	if (IsMC) {
		job_info << "\t<MCInfo>" << std::endl;
		job_info << "\t\t<Process>" << MCProcess << "</Process>" << std::endl;
		job_info << "\t\t<CrossSection>" << MCCrossSection << "</CrossSection>" << std::endl;
		job_info << "\t\t<Generator>" << MCGenerator << "</Generator>" << std::endl;
		job_info << "\t</MCInfo>" << std::endl;
	}
	for (std::vector<ObjectDefinitionInfo*>::iterator it = ObjectDefinitions.begin(); it != ObjectDefinitions.end(); ++it) {
		job_info << "\t<ObjectDefinition>" << std::endl;
		job_info << "\t\t<Name>" << (*it)->Name << "</Name>" << std::endl;
		job_info << "\t\t<Type>" << (*it)->Type << "</Type>" << std::endl;
		job_info << "\t</ObjectDefinition>" << std::endl;
	}
	job_info << "</JobInfo>" 	<< std::endl;
	job_info.close();
}

bool StandaloneJobInfoContainer::LoadXMLFile(TString path, bool print) {

	IsMC = false;

	std::cout << "[StandaloneJobInfoContainer] INFO : Reading job configuration from " << path << std::endl;
	TDOMParser *xmlparser = new TDOMParser();
	TXMLDocument* xmldoc;
	TXMLNode *node1;
	Int_t parseCode;

	parseCode = xmlparser->ParseFile(path);
	if (parseCode != 0) {
		std::cerr << "[CountFakeObjects] ERROR : Couldn't open job configuration XML file at " << path << std::endl;
		return false;
	} else {
		xmldoc = xmlparser->GetXMLDocument();
		node1 = xmldoc->GetRootNode();
		TXMLNode *node2 = node1->GetChildren();
		while (node2 != 0) {
			if (node2->GetNodeName() == TString("SubmissionMethod")) {
				SubmissionMethod = node2->GetText();
			} else if (node2->GetNodeName() == TString("JobDescription")) {
				JobDescription = node2->GetText();
			} else if (node2->GetNodeName() == TString("D3PDName")) {
				D3PDName = node2->GetText();
			} else if (node2->GetNodeName() == TString("D3PDJobName")) {
				D3PDJobName = node2->GetText();
			} else if (node2->GetNodeName() == TString("OutputName")) {
				GridOutputName = node2->GetText();
			} else if (node2->GetNodeName() == TString("MCInfo")) {
				IsMC = true;
				TXMLNode *node3 = node2->GetChildren();
				while (node3 != 0) {
					if (node3->GetNodeName() == TString("Process")) {
						MCProcess = node3->GetText();
					} else if (node3->GetNodeName() == TString("CrossSection")) {
						std::stringstream ss;
						ss.str(node3->GetText());
						try {
							ss >> MCCrossSection;
						} catch (const char *) {
							std::cerr << "[StandaloneJobInfoContainer] Couldn't understand specified cross section: " << node3->GetText() << std::endl;
							exit(1);
						}
					} else if (node3->GetNodeName() == TString("Generator")) {
						MCGenerator = node3->GetText();
					} 
					node3 = node3->GetNextNode();
				}
			} else if (node2->GetNodeName() == TString("Luminosity")) {
				 std::stringstream ss;
				 ss.str(node2->GetText());
				 try {
				 	ss >> Luminosity;
				 } catch (const char *) {
				 	std::cerr << "[StandaloneJobInfoContainer] Couldn't understand specified luminosity: " << node2->GetText() << std::endl;
				 	exit(1);
				 }
			} else if (node2->GetNodeName() == TString("ObjectDefinition")) {
				TXMLNode *node3 = node2->GetChildren();
				ObjectDefinitionInfo *odi = new ObjectDefinitionInfo();
				while (node3 != 0) {
					if (node3->GetNodeName() == TString("Name")) { 
						odi->Name = node3->GetText();
					} else if (node3->GetNodeName() == TString("Type")) { 
						odi->Type = node3->GetText();
					}  else if (node3->GetNodeName() == TString("Description")) { 
						odi->Description = node3->GetText();
					}
					node3 = node3->GetNextNode();
				}
				ObjectDefinitions.push_back(odi);
			}
			node2 = node2->GetNextNode();
		}
	}

	if (print) PrintJobInfo();

	return true;
}

void StandaloneJobInfoContainer::PrintJobInfo() {
	if (SubmissionMethod != TString("")) std::cout << "[StandaloneJobInfoContainer] INFO : SubmissionMethod = " << SubmissionMethod << std::endl;
	if (JobDescription != TString("")) std::cout << "[StandaloneJobInfoContainer] INFO : JobDescription = " << JobDescription << std::endl;
	if (D3PDName != TString("")) std::cout << "[StandaloneJobInfoContainer] INFO : D3PDName = " << D3PDName << std::endl;
	if (D3PDJobName != TString("")) std::cout << "[StandaloneJobInfoContainer] INFO : D3PDJobName = " << D3PDJobName << std::endl;
	if (GridOutputName != TString("")) std::cout << "[StandaloneJobInfoContainer] INFO : GridOutputName = " << GridOutputName << std::endl;
	if (IsMC) {
		if (MCProcess != TString("")) std::cout << "[StandaloneJobInfoContainer] INFO : MCProcess = " << MCProcess << std::endl;
		if (MCGenerator != TString("")) std::cout << "[StandaloneJobInfoContainer] INFO : MCGenerator = " << MCGenerator << std::endl;
		if (MCCrossSection != TString("")) std::cout << "[StandaloneJobInfoContainer] INFO : MCCrossSection = " << MCCrossSection << std::endl;
	}
	for (std::vector<ObjectDefinitionInfo*>::iterator it = ObjectDefinitions.begin(); it != ObjectDefinitions.end(); ++it) {
		std::cout << "[StandaloneJobInfoContainer] INFO : Object definition " << (*it)->Name << std::endl;
		std::cout << "[StandaloneJobInfoContainer] INFO : \tType = " << (*it)->Type << std::endl;
		std::cout << "[StandaloneJobInfoContainer] INFO : \tDescription = " << (*it)->Description << std::endl;
	}
}

