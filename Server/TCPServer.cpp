
/* 
 * File:   TCPServer.cpp
 * Author: czimbortibor
 * 
 * Created on December 16, 2016, 8:17 PM
 */

#include "TCPServer.h"

TCPServer::TCPServer(const char* address, int port) : address(address) {
    this->port = port;
    usersPtr = std::make_shared<std::list<std::unique_ptr<ClientThread>>>();
}

TCPServer::TCPServer(const TCPServer& original) {
}

TCPServer::~TCPServer() {
    if (address != NULL) {
        delete address;
        address = NULL;
    }
}

void TCPServer::initServer() {
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket < 0) {
        errorMsg = "error while opening the server socket"; 
        error(errorMsg.c_str());
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(address);
    serverAddr.sin_port = htons(port);

    int res = bind(listenSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
    if (res < 0) {
        errorMsg = "error on binding";
        error(errorMsg.c_str());
    }
}

void TCPServer::listenServer() {
    std::cout << "Listening for clients...\n";
    int res = listen(listenSocket, 1);
    if (res < 0) {
        errorMsg = "error while listening on socket";
        error(errorMsg.c_str());
    }
}

void TCPServer::startServer() {
    while(1<2) {
        listenServer();
        std::cout << "Accepting client...\n";
        socklen_t addrSize = sizeof(serverAddr);
        acceptSocket = accept(listenSocket, NULL, NULL);
        if (acceptSocket < 0) {
            errorMsg = "error while creating the accepting socket";
            error(errorMsg.c_str());
        }
        
        std::unique_ptr<ClientThread> clientThread(new ClientThread(usersPtr, acceptSocket));
        /** creates the accept socket for the thread */
        //clientThread->setAcceptSocket(listenSocket, serverAddr);
        std::cout << "Creating a new thread for the client...\n";
        clientThread->start();
        /** move the ownership of the unique_ptr to the list and insert it into the list */
        usersPtr->push_back(std::move(clientThread));
    }
}
