
/*
 * File:   Packaging.h
 * Author: czimbortibor
 *
 * Created on December 17, 2016, 8:47 PM
 */

#ifndef PACKAGING_H
#define PACKAGING_H

#include <string>
#include <deque>

/**     Realizes the protocol inside the chat application.
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
    /** package used by the server after the user's login */
    std::string createTimePackage(const std::string& message);
    /** package used for disconnect requests */
    std::string createDisconnectPackage();
    /** package used for sending message to every online user */
    std::string createGlobalPackage(const std::string& message, const std::string& sennder);
    /** private message to a specific user */
    std::string createPivatePackage(const std::string& receiver, const std::string& message, const std::string& sender);

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
};

#endif /* PACKAGING_H */
