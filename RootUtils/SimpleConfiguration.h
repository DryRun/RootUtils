#ifndef XMLHELPER_h
#define XMLHELPER_h

#include <TROOT.h>
#include "TCollection.h"
#include "TXMLNode.h"
#include "TXMLDocument.h"
#include "TDOMParser.h"
#include "TXMLAttr.h"
#include "TString.h"
#include "TList.h"
#include <TSystem.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

TString GetNodeAttribute(TXMLNode *node, TString attr_name);

class SimpleConfiguration : public TObject {

   	ClassDef(SimpleConfiguration, 1);

   	/*** Public Methods ***/
public:
	/**
	  * Constructor
	  */
	SimpleConfiguration();
	
	/**
	  * Destructor
	  */
	~SimpleConfiguration();

	// Global IO
	/**
	  * LoadConfiguration
	  * - Loads SimpleConfiguration from XML
	  * - Optionally, print out the configuration to std::cout
	  */
	bool LoadConfiguration(TString p_path, bool p_verbose = false);

	/**
	  * SaveConfiguration
	  * - Writes the configuration to XML
	  */
	void SaveConfiguration(TString p_path);
	
	/**
	  * PrintConfiguration
	  * - Prints the configuration to std::cout
	  */
	void PrintConfiguration();

	// Data accessors
	/**
	  * GetElement
	  * - Get a <Item></Item> element from the configuration
	  */
	TString GetElement(TString elem_name);

	/**
	  * GetItem
	  * - Get a <Item></Item> element from the configuration
	  */
	TString GetItem(TString item_name);

	/**
	  * GetList
	  * - Get a <List></List> from the configuration
	  * - Note that a vector of pairs, rather than a map, is returned, to allow for possibly having multiple <ListItem>s with the same name.
	  */
	std::vector<std::pair<TString, TString> >* GetList(TString list_name);

	/**
	  * GetListItemByName
	  * - Search for a specific <ListItem> by name. Note that this will return the first one found; beware duplicates!
	  */
	TString GetListItemByName(TString list_name, TString listitem_name);

	/**
	  * operator []
	  * - Mostly for backwards compatibility, but essentially equivalent to GetElement.
	  */
	TString operator[](TString key); // For backwards compability (SimpleConfiguration used to be a std::map<TString, TString>)

	// For looping over all elements
	std::vector<TString>* GetListOfElements();

	// For looping over all list
	std::vector<TString>* GetListOfLists();

	// For checking required elements
	bool HasElement(TString elem_name);
	bool HasList(TString list_name);
	bool HasListItem(TString list_name, TString listitem_name);
	bool CheckRequiredElements(std::vector<TString> *p_required_elements);
	bool CheckRequiredLists(std::vector<TString> *p_required_lists);

	// Add elements and lists
	void AddElement(TString elem_name, TString elem_value);
	void RemoveElement(TString elem_name);
	void AddList(TString list_name);
	void AddList(TString list_name, std::vector<std::pair<TString, TString> > list_values);
	void AddListItem(TString list_name, TString list_item_name, TString list_item_value);
	void ChangeListItem(TString list_name, TString list_item_name, TString list_item_value);


	/*** Private Members ***/
private:
	// Data members
	std::vector<TString> m_list_of_elements;
	std::map<TString, TString> elem;
	std::map<TString, std::vector<std::pair<TString, TString> > > list;
	std::vector<TString> m_list_of_lists;
	TString m_loaded_path_;

};
	/**
	  * LoadSimpleConfiguration
	  * - DTD located at www-atlas.lbl.gov/~dryu/SimpleConfiguration.dtd
	  */
SimpleConfiguration* LoadSimpleConfiguration(TString p_path);

#endif