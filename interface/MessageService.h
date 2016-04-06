#ifndef MessageService_h
#define MessageService_h

#include <iostream>
#include <map>
#include <cstdlib>

using namespace std;

class MessageService {
public:
	enum MessageLevel {
		VERBOSE = -2,
		DEBUG   = -1,
		INFO    = 0,
		WARNING = 1,
		ERROR   = 2
	};

	/*** Public Methods ***/
public:
	MessageService(std::string p_class_name);

	~MessageService();

	/**
	 * Set the minimum threshold for printing messages (default is MessageLevel::INFO)
	 * @param p_minimum_level Minimum message importance for printing.
	 */
	void SetMessageThreshold(MessageLevel p_minimum_level);

	/**
	 * Print a message to std::cout or std::cerr if the message importance exceeds the level set in initialization.
	 * @param p_msg   [description]
	 * @param p_level [description]
	 */
	void Print(const std::string p_msg, MessageLevel p_level);

	void Print(const char * p_msg, MessageLevel p_level);


	/**
	 * Print an error message to std::cerr and kill the program.
	 * @param p_msg Error message
	 */
	void Die(std::string p_msg);

	/*** Private Methods ***/
private:

	/*** Public Members ***/
public:

	/*** Private Members ***/
private:
	MessageLevel m_minimum_level;

	std::string m_class_name;

	std::map<MessageLevel, string> m_message_level_string;

};


#endif
