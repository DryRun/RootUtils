#ifndef MessageService_cxx
#define MessageService_cxx

#include "MyTools/RootUtils/interface/MessageService.h"

MessageService::MessageService(std::string p_class_name) {
	m_minimum_level = INFO;
	m_class_name = p_class_name;

	m_message_level_string[VERBOSE] = "VERBOSE";
	m_message_level_string[DEBUG] = "DEBUG";
	m_message_level_string[INFO] = "INFO";
	m_message_level_string[WARNING] = "WARNING";
	m_message_level_string[ERROR] = "ERROR";
}

MessageService::~MessageService() {

}

void MessageService::SetMessageThreshold(MessageLevel p_minimum_level) {
	std::cout << "[MessageService] INFO : Setting message threshold to " << m_message_level_string[p_minimum_level] << std::endl;
	m_minimum_level = p_minimum_level;
}

void MessageService::Print(const std::string p_msg, MessageLevel p_level) {

	if (p_level >= m_minimum_level) {
		if (p_level <= 0) {
			std::cout << "[" << m_class_name << "] " << m_message_level_string[p_level] << " : " << p_msg << std::endl;
		} else {
			std::cerr << "[" << m_class_name << "] " << m_message_level_string[p_level] << " : " << p_msg << std::endl;
		}
	}
}

void MessageService::Print(const char * p_msg, MessageLevel p_level) {
	std::string str(p_msg);
	Print(str, p_level);
}


void MessageService::Die(std::string p_msg) {
	std::cerr << "[" << m_class_name << "] FATAL : " << p_msg << std::endl;
	exit(1);
}



#endif