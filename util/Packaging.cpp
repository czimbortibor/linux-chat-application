
/*
 * File:   Packaging.cpp
 * Author: czimbortibor
 *
 * Created on December 17, 2016, 8:47 PM
 */

#include "Packaging.h"


Packaging::Packaging() {}

Packaging::Packaging(const std::string& receiver, std::size_t pckglength, const std::string& message, const std::string& sender) {
    this->receiver = receiver;
    this->pckglength = pckglength;
    this->message = message;
    this->sender = sender;
}

Packaging::Packaging(std::size_t pckglength, const std::string& message, const std::string& sender) {
    this->pckglength = pckglength;
    this->message = message;
    this->sender = sender;
}

Packaging::~Packaging() {}

std::string Packaging::createLoginPackage(const std::string& username) {
    this->receiver = "server";
    this->message = " ";
    this->sender = username;
    return constructPackage();
}

std::string Packaging::createTimePackage(const std::string& message) {
    this->receiver = "self";
    this->message = message;
    this->sender = "server";
    return constructPackage();
}

std::string Packaging::createDisconnectPackage() {
    this->receiver = "self";
    this->message = "disconnect";
    this->sender = sender;
    return constructPackage();
}

std::string Packaging::createGlobalPackage(const std::string& message, const std::string& sender) {
    this->receiver = "global";
    this->message = message;
    this->sender = sender;
    return constructPackage();
}

std::string Packaging::createPivatePackage(const std::string& receiver, const std::string& message, const std::string& sender) {
    this->receiver = receiver;
    this->message = message;
    this->sender = sender;
    return constructPackage();
}

std::string Packaging::createOnlineUsersPackage(const std::vector<std::string>& onlineUsers) {
    std::ostringstream joinedFields;
    // inserts the elements into the "joinedFields" stream, with the "," delimiter 
    std::copy(onlineUsers.begin(), onlineUsers.end(), std::ostream_iterator<std::string>(joinedFields, ","));
    std::string usersList = joinedFields.str();
    // unnecessary delimiter on the end of the string
    //usersList[usersList.length() - 1] = '\0';
    /*std::string usersList;
    usersList.assign(tmpStr, 0, tmpStr.length() - 1);*/
    this->receiver = "list_users";
    this->message = usersList;
    this->sender = "server";
    return constructPackage();
}

std::string Packaging::constructPackage() {
    std::string pckg = receiver + glue + glue + message + glue + sender + glue;
    size_t len = pckg.length();
    std::string pckglength = std::to_string(pckg.length() + std::to_string(len).length());
    // package: receiver|pkglength|message|sender
    return receiver + glue + pckglength + glue + message + glue + sender + glue;
}

void Packaging::parsePackage(std::string package) {
    size_t pos = 0;
    std::string token;
    std::deque<std::string> tokens;
    while ((pos = package.find(glue)) != std::string::npos) {
        // next token
        token = package.substr(0, pos);
		tokens.push_back(token);
        // erase the token + the glue
        package.erase(0, pos + 1);
    }

    // package: receiver|pkglength|message|sender
    receiver = tokens[0];
    tokens.pop_front();
    pckglength = std::stoi(tokens[0]);
    tokens.pop_front();
    message = tokens[0];
    tokens.pop_front();
    sender = tokens[0];
}

std::string Packaging::identifyRequest(std::string package) {
    size_t pos = 0;
    std::string token;
    while ((pos = package.find(glue)) != std::string::npos) {
        // next token
        token = package.substr(0, pos);
        if (token.compare("self") == 0) {
            return "time_package";
        }
        if (token.compare("global") == 0) {
            return "global_package";
        }
        if (token.compare("server") == 0) {
            return "login_request";
        }
        if (token.compare("disconnect") == 0) {
            return "logout_request";
        }
        if (token.compare("list_users") == 0) {
            return "online_users_package";
        }
        // erase token + glue
        package.erase(0, pos + 1);
    }
	return "private_package";
}
