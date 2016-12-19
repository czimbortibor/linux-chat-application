
/*
 * File:   Packaging.cpp
 * Author: czimbortibor
 *
 * Created on December 17, 2016, 8:47 PM
 */

#include "Packaging.h"

Packaging::Packaging() {}

Packaging::Packaging(const std::string& receiver, std::size_t msglength, const std::string& message, const std::string& sender) {
	this->receiver = receiver;
	this->msglength = msglength;
	this->message = message;
	this->sender = sender;
}

Packaging::Packaging(std::size_t msglength, const std::string& message, const std::string& sender) {
	this->msglength = msglength;
	this->message = message;
	this->sender = sender;
}

Packaging::~Packaging() {}

std::string Packaging::createLoginPackage(const std::string& username) {
	message = username;
	return constructPackage();
}

std::string Packaging::createDisconnectPackage() {
	message = "disconnect";
	return constructPackage();
}

std::string Packaging::createGlobalPackage() {
	receiver = "global";
	return constructPackage();
}

std::string Packaging::createPivatePackage(const std::string& receiver) {
	this->receiver = receiver;
	return constructPackage();
}

std::string Packaging::constructPackage() {
	std::string length = std::to_string(message.length());
	// package: receiver|msglength|message|sender
	return receiver + glue + length + glue + message + glue + sender;
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

	// package: receiver|msglength|message|sender
	receiver = tokens[0];
	tokens.pop_front();
	msglength = std::stoi(tokens[0]);
	tokens.pop_front();
	message = tokens[0];
	tokens.pop_front();
	sender = tokens[0];
	tokens.pop_front();
}

std::string Packaging::identifyRequest(std::string package) {
	size_t pos = 0;
	std::string token;
	while ((pos = package.find(glue)) != std::string::npos) {
		// next token
		token = package.substr(0, pos);
		if (token.compare("global") == 0) {
			return "global_package";
		}
		if (token.compare("server") == 0) {
			return "login_request";
		}
		if (token.compare("disconnect") == 0) {
			return "disconnect_request";
		}
		// erase token + glue
		package.erase(0, pos + 1);
	}
	return "unidentifiable";
}
