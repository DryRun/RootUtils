#ifndef XMLHelper_cxx
#define XMLHelper_cxx

#include "RootUtils/SimpleConfiguration.h"

using namespace std;

TString GetNodeAttribute(TXMLNode *node, TString attr_name) {

	TString attr_value = "";
	bool found = false;

	TList *c_attrlist = node->GetAttributes();
	TIter c_iter(c_attrlist);
	while (TObject *obj = c_iter()) {
		TXMLAttr *c_xmlattr = (TXMLAttr*)obj;
		if (c_xmlattr->GetName() == attr_name) {
			attr_value = c_xmlattr->GetValue();
			found = true;
			break;
		}
	}

	return attr_value;
}

ClassImp (SimpleConfiguration);


SimpleConfiguration::~SimpleConfiguration() {

}

SimpleConfiguration* LoadSimpleConfiguration(TString p_path) {
	SimpleConfiguration *configuration = new SimpleConfiguration();
	configuration->LoadConfiguration(p_path);
	return configuration;
}

SimpleConfiguration::SimpleConfiguration() {

}

TString SimpleConfiguration::GetElement(TString elem_name) {
	return GetItem(elem_name);
}

TString SimpleConfiguration::GetItem(TString item_name) {
	if (elem.find(item_name) == elem.end()) {
		std::cerr << "[SimpleConfiguration] ERROR : Element " << item_name << " was not found in the configuration from " << m_loaded_path_ << "." << std::endl;
		exit(1);
		//return TString("");
	}
	return elem[item_name];
}

std::vector<std::pair<TString, TString> >* SimpleConfiguration::GetList(TString list_name) {
	if (list.find(list_name) == list.end()) {
		std::cerr << "[SimpleConfiguration] ERROR : List " << list_name << " was not found in the configuration from " << m_loaded_path_ << "." << std::endl;
		exit(1);
		//return 0x0;
	}
	return &(list[list_name]);
}


TString SimpleConfiguration::operator[](TString key) {
	return elem[key];
}

bool SimpleConfiguration::LoadConfiguration(TString p_path, bool p_verbose) {
	if (p_verbose) std::cout << "[SimpleConfiguration] VERBOSE : Reading selection configuration from " << p_path << std::endl;

	m_loaded_path_ = p_path;

	// Load configuration from XML file. See Config/testSelection.xml for an example. 
	TDOMParser *xmlparser = new TDOMParser();
	TXMLDocument* xmldoc;
	Int_t parseCode(1);

	// Where does this function come from? For now, ignore it. 
	// xmlparser->SetValidate(kFALSE);

	gSystem->ExpandPathName(p_path);
	parseCode = xmlparser->ParseFile(p_path);
	if (parseCode != 0) {
		std::cerr << "[SimpleConfiguration] ERROR : TXMLParser::ParseFile(" << p_path << ") returned error code " << parseCode << std::endl;
		throw parseCode;
		//exit(1);
	} else {
		xmldoc = xmlparser->GetXMLDocument();
		TXMLNode *node1 = xmldoc->GetRootNode();
		TXMLNode *node2 = node1->GetChildren();

		while (node2 != 0) {

			#ifdef DEBUG_XMLHELPER
			std::cout << "[SimpleConfiguration] DEBUG : node2->GetNodeName() = " << node2->GetNodeName() << std::endl;
			#endif

			if (node2->GetNodeName() == TString("Name")) {
				elem["Name"] = node2->GetText();
			} else if (node2->GetNodeName() == TString("Item")) {
				TString element_name = GetNodeAttribute(node2, "name");
				m_list_of_elements.push_back(element_name);
				elem[element_name] = node2->GetText();
			} else if (node2->GetNodeName() == TString("List")) {
				TString list_name = GetNodeAttribute(node2, "name");
				m_list_of_lists.push_back(list_name);
				// Create an empty vector
				std::vector<std::pair<TString, TString> > empty_vector;
				list[list_name] = empty_vector;
				TXMLNode *node3 = node2->GetChildren();
				while (node3 != 0) {
					if (node3->GetNodeName() == TString("ListItem")) {
						// Make pair of <node name> : <node text>. If <node name> doesn't exists, use blank string.
						TString list_element_name = GetNodeAttribute(node3, "name");
						list[list_name].push_back(std::make_pair<TString, TString>(TString(list_element_name), TString(node3->GetText())));
					}
					node3 = node3->GetNextNode();
				}
			}
			
			node2 = node2->GetNextNode();
		}
		if (p_verbose) std::cout << "[SimpleConfiguration] VERBOSE : Finished loading SimpleConfiguration from " << p_path << std::endl;
		if (p_verbose) PrintConfiguration();
	}
	delete xmlparser;
	return (!parseCode);
}

void SimpleConfiguration::SaveConfiguration(TString p_path) {
	gSystem->ExpandPathName(p_path);
	std::cout << "[SimpleConfiguration] INFO : Saving SimpleConfiguration to " << p_path << std::endl;

	TString m_save_directory = gSystem->DirName(p_path);
	gSystem->Exec(TString("mkdir -pv ") + m_save_directory);

	ofstream xmlfile;
	xmlfile.open(p_path);

	xmlfile << "<?xml version=\"1.0\"?>" << std::endl;
	xmlfile << "<!DOCTYPE SimpleConfiguration [" << std::endl;
	xmlfile << "<!ELEMENT SimpleConfiguration (ConfigurationName*,  Item*, List*)>" << std::endl;
	xmlfile << "<!ELEMENT ConfigurationName (#PCDATA)>" << std::endl;
	xmlfile << "<!ELEMENT Item (#PCDATA)>" << std::endl;
	xmlfile << "<!ELEMENT List (ListItem*)>" << std::endl;
	xmlfile << "<!ELEMENT ListItem (#PCDATA)>" << std::endl;
	xmlfile << "<!ATTLIST Item name CDATA \"none\">" << std::endl;
	xmlfile << "<!ATTLIST List name CDATA \"none\">" << std::endl;
	xmlfile << "<!ATTLIST ListItem name CDATA \"none\">" << std::endl;
	xmlfile << "]>" << std::endl;

	xmlfile << std::endl << "<SimpleConfiguration>" << std::endl;

	for (std::map<TString, TString>::iterator it = elem.begin(); it != elem.end(); ++it) {
		xmlfile << "\t<Item name=\"" << (*it).first << "\">" << (*it).second << "</Item>" << std::endl;
	}

	for (std::map<TString, std::vector<std::pair<TString, TString> > >::iterator it = list.begin(); it != list.end(); ++it) {
		xmlfile << "\t<List name=\"" << (*it).first << "\">" << std::endl;
		for (std::vector<std::pair<TString, TString> >::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			xmlfile << "\t\t<ListItem name=\"" << (*it2).first << "\">" << (*it2).second << "</ListItem>" << std::endl;
		}
		xmlfile << "\t</List>" << std::endl;
	}

	xmlfile << "</SimpleConfiguration>" << std::endl;
	xmlfile.close();
	std::cout << "[SimpleConfiguration] INFO : Done saving." << std::endl;
}

void SimpleConfiguration::PrintConfiguration() {
	std::cout << std::endl << "[SimpleConfiguration] INFO : *** Printing SimpleConfiguration ***" << std::endl;
	std::cout << "[SimpleConfiguration] INFO : Elements:" << std::endl;
	for (std::map<TString, TString>::iterator it = elem.begin(); it != elem.end(); ++it) {
		std::cout << "[SimpleConfiguration] INFO : \t" << (*it).first << " = " << (*it).second << std::endl;
	}
	std::cout << "[SimpleConfiguration] INFO : Lists:" << std::endl;

	for (std::map<TString, std::vector<std::pair<TString, TString> > >::iterator it = list.begin(); it != list.end(); ++it) {
		std::cout << "[SimpleConfiguration] INFO : \tList " << (*it).first << ":" << std::endl;
		for (std::vector<std::pair<TString, TString> >::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
			std::cout << "[SimpleConfiguration] INFO : \t\t" << (*it2).first << " = " << (*it2).second << std::endl;
		}
	}
	std::cout << "[SimpleConfiguration] INFO : *** End SimpleConfiguration" << std::endl << std::endl;
}	

std::vector<TString>* SimpleConfiguration::GetListOfElements() {
	//std::vector<TString> list_of_elements;
	//for (std::map<TString, TString>::iterator it = elem.begin(); it != elem.end(); ++it) {
	//	list_of_elements.push_back((*it).first);
	//}
	//return list_of_elements;
	return &(m_list_of_elements);
}

std::vector<TString>* SimpleConfiguration::GetListOfLists() {
	return &(m_list_of_lists);
}


bool SimpleConfiguration::HasElement(TString elem_name) {
	if (elem.find(elem_name) == elem.end()) {
		return false;
	} else {
		return true;
	}
}

bool SimpleConfiguration::HasList(TString list_name) {
	if (list.find(list_name) == list.end()) {
		return false;
	} else {
		return true;
	}
}

bool SimpleConfiguration::HasListItem(TString list_name, TString listitem_name) {
	if (!HasList(list_name)) return false;
	bool found = false;
	for (std::vector<std::pair<TString, TString> >::iterator it = list[list_name].begin(); it != list[list_name].end(); ++it) {
		if ((*it).first.EqualTo(listitem_name)) {
			found = true;
			break;
		}
	}
	return found;
}

void SimpleConfiguration::AddElement(TString elem_name, TString elem_value) {
	if (elem.find(elem_name) != elem.end()) {
		std::cerr << "[SimpleConfiguration] WARNING : Overwriting old element (" << elem_name << " => " << elem[elem_name] << ") with (" << elem_name << " => " << elem_value << ")" << std::endl;
	}
	elem[elem_name] = elem_value;
}

void SimpleConfiguration::RemoveElement(TString elem_name) {
	std::map<TString, TString>::iterator it = elem.find(elem_name);
	if (it != elem.end()) {
		elem.erase(it);
	}
}

void SimpleConfiguration::AddList(TString list_name) {
	std::vector<std::pair<TString, TString> > empty_vec;
	list[list_name] = empty_vec;
}

void SimpleConfiguration::AddList(TString list_name, std::vector<std::pair<TString, TString> > list_values) {
	list[list_name] = list_values;
}

void SimpleConfiguration::AddListItem(TString list_name, TString list_item_name, TString list_item_value) {
	list[list_name].push_back(std::make_pair<TString, TString>(TString(list_item_name), TString(list_item_value)));
}

void SimpleConfiguration::ChangeListItem(TString list_name, TString list_item_name, TString list_item_value) {
	if (!this->HasList(list_name)) {
		list[list_name].push_back(std::make_pair<TString, TString>(TString(list_item_name), TString(list_item_value)));
		return;
	} else {
		// Check if the key already exists in the list
		for (std::vector<std::pair<TString, TString> >::iterator it = list[list_name].begin(); it != list[list_name].end(); ++it) {
			if ((*it).first == list_item_name) {
				list[list_name].erase(it);
				list[list_name].push_back(std::make_pair<TString, TString>(TString(list_item_name), TString(list_item_value)));
				return;
			}
		}

		// If here, the key does not exist, so simply add it to the list.
		list[list_name].push_back(std::make_pair<TString, TString>(TString(list_item_name), TString(list_item_value)));
		return;
	}
}

bool SimpleConfiguration::CheckRequiredElements(std::vector<TString> *p_required_elements) {
	bool good = true;
	for (std::vector<TString>::iterator it = (*p_required_elements).begin(); it != (*p_required_elements).end(); ++it) {
		if (!HasElement((*it))) {
			good = false;
			std::cerr << "[SimpleConfiguration] WARNING : Configuration is missing element " << *it << std::endl;
		}
	}
	return good;
}

bool SimpleConfiguration::CheckRequiredLists(std::vector<TString> *p_required_lists) {
	bool good = true;
	for (std::vector<TString>::iterator it = (*p_required_lists).begin(); it != (*p_required_lists).end(); ++it) {
		if (!HasList((*it))) {
			good = false;
			std::cerr << "[SimpleConfiguration] WARNING : Configuration is missing list " << *it << std::endl;
		}
	}
	return good;
}

TString SimpleConfiguration::GetListItemByName(TString list_name, TString listitem_name) {
	TString listitem_value = "NULL";
	if (this->HasList(list_name)) {
		bool found_listitem = false;
		for (std::vector<std::pair<TString, TString> >::iterator it = list[list_name].begin(); it != list[list_name].end(); ++it) {
			if ((*it).first == listitem_name) {
				listitem_value = (*it).second;
				found_listitem = true;
				break;
			}
		} 
		if (!found_listitem) {
			std::cerr << "[SimpleConfiguration] ERROR : Configuration loaded from " << m_loaded_path_ << " has list " << list_name << ", but no listitem " << listitem_name << endl;
		}
	} else {
		std::cerr << "[SimpleConfiguration] ERROR : Configuration loaded from " << m_loaded_path_ << " does not have <List> named " << list_name << std::endl;
		exit(1);
	}
	return listitem_value;
}

#endif
