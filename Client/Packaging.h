
/* 
 * File:   Packaging.h
 * Author: czimbortibor
 *
 * Created on December 17, 2016, 8:47 PM
 */

#ifndef PACKAGING_H
#define PACKAGING_H

#include <string>

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

};

#endif /* PACKAGING_H */

