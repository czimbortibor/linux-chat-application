
/* 
 * File:   Packaging.h
 * Author: czimbortibor
 *
 * Created on December 17, 2016, 8:47 PM
 */

#ifndef PACKAGING_H
#define PACKAGING_H

#include <string>
<<<<<<< HEAD
#include <deque>

/**	Realizes the protocol inside the chat application.
 *
 *	handles:
 *		- package assembling -> creates a package for a specific request
 *		- package disassembling -> read the package and identify the request
 *
 *	general package format: receiver|msglength|message|sender
 *	joined together with '|'
 *		- receiver: the user whom the message is addressed
 *		- length: length of the user's message
 *		- message: the actual data
 *		- sender: who sent the message
 *
 *	identifications:
 *		- login: receiver = "server"
 *		- disconnect: message = "disconnect"
 *		- global: receiver = "global"
*/
class Packaging {
public:
	Packaging();
	Packaging(const std::string& receiver, std::size_t msglength, const std::string& message, const std::string& sender);
	Packaging(std::size_t msglength, const std::string& message, const std::string& sender);
    virtual ~Packaging();

	/** package used for login requests, the message contains the username */
	std::string createLoginPackage(const std::string& username);
	/** package used for disconnect requests */
	std::string createDisconnectPackage();
	/** package used for sending message to every online user */
	std::string createGlobalPackage();
	/** private message to a specific user */
	std::string createPivatePackage(const std::string& receiver);

	void parsePackage(std::string package);

	/** parses the package and returns the request's name */
	std::string identifyRequest(std::string package);

	std::string getMessage() const { return message; }
	std::string getSender() const { return sender; }

private:
	std::string receiver;
	std::size_t msglength;
	std::string message;
	std::string sender;

	const char glue = '|';
	/** joins the fields together */
	std::string constructPackage();
=======

/** handles:
 * package assembling -> creates a package for a specific request
 * package disassembling |SERVER SIDE ONLY| -> read the package and identify the request
*/
class Packaging {
public:
	Packaging(std::string message);
    virtual ~Packaging();

	/** package used for login requests */
	std::string getLoginPackage() const { return loginPackage; }
	/** package used for sending message to every online user */
	std::string getGlobalMessagePackage() const { return globalMessagePackage; }
	/** package used for sending message to a specific user */
	std::string getPrivateMessagePackage() const { return privateMessagePackage; }

	void createLoginPackage();
	void createGlobalPackage();
	void createPivatePackage();

private:
	std::string message;
	std::string loginPackage;
	std::string globalMessagePackage;
	std::string privateMessagePackage;

>>>>>>> 1a123a584b2b361786849f2a6f4730d715edc42b
};

#endif /* PACKAGING_H */

