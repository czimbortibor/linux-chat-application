
/*
 * File:   Packaging.h
 * Author: czimbortibor
 *
 * Created on December 17, 2016, 8:47 PM
 */

#ifndef PACKAGING_H
#define PACKAGING_H

#include <string>
#include <sstream>
#include <deque>
#include <vector>
#include <iterator>

/**     Realizes the protocol inside the chat application.
 *
 *	handles:
 *		- package assembling -> creates a package for a specific request
 *		- package disassembling -> read the package and identify the request
 *
 *	general package format: receiver|pckglength|message|sender
 *	joined together with '|'
 *		- receiver: the user whom the message is addressed
 *		- pckglength: length of the package
 *		- message: the actual data
 *		- sender: who sent the message
 *
 *	identifications:
 *		- login: receiver = "server"
 *		- disconnect: message = "disconnect"
 *		- global: receiver = "global"
 *		- list_users: receiver = "list_users"
 *		- file_size: receiver = "file_size"
*/
class Packaging {
public:
    Packaging();
    Packaging(const std::string& receiver, std::size_t pckglength, const std::string& message, const std::string& sender);
    Packaging(std::size_t pckglength, const std::string& message, const std::string& sender);
    virtual ~Packaging();

    /** package used for login requests, the message contains the username */
    std::string createLoginPackage(const std::string& username);
    /** package used by the server after the user's login */
    std::string createTimePackage(const std::string& message);
    /** package used for disconnect requests */
    std::string createDisconnectPackage();
    /** package used for sending message to every online user */
    std::string createGlobalPackage(const std::string& message, const std::string& sender);
    /** private message to a specific user */
    std::string createPivatePackage(const std::string& receiver, const std::string& message, const std::string& sender);
    /** package used to send the online user's list to a user */
    std::string createOnlineUsersPackage(const std::vector<std::string>& onlineUsers);
	/** package used to signal the incoming file's size */
	std::string createFileSizePackage(int fileSize);
	/** package used to send a binary stream */
	std::string createFilePackage(const std::string& blob, const std::string& receiver, const std::string& sender);

    void parsePackage(std::string package);

    /** parses the package and returns the request's name */
    std::string identifyRequest(std::string package);

    std::string getMessage() const { return message; }
    std::string getSender() const { return sender; }
    std::string getReceiver() const { return receiver; }
    size_t getPckglength() const { return pckglength; }

private:
    std::string receiver;
    std::size_t pckglength;
    std::string message;
    std::string sender;

    const char glue = '|';
    /** joins the fields together */
    std::string constructPackage();
};

#endif /* PACKAGING_H */
