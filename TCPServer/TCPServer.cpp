
/* 
 * File:   TCPServer.cpp
 * Author: czimbortibor
 * 
 * Created on December 16, 2016, 8:17 PM
 */

#include "TCPServer.h"

TCPServer::TCPServer(const char* address, int port) : address(address) {
    this->port = port;
}

TCPServer::TCPServer(const TCPServer& original) {
}

TCPServer::~TCPServer() {
    if (address != NULL) {
        delete address;
        address = NULL;
    }
    dropConnections();
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
    
    std::cout << "Listening for clients...\n";
    res = listen(listenSocket, 1);
    if (res < 0) {
        errorMsg = "error while listening on socket";
        error(errorMsg.c_str());
    }
}

void TCPServer::startServer() {
    while(1<2) {
        std::cout << "Accepting client...\n";
        std::unique_ptr<ClientThread> clientThread(new ClientThread());
        clientThread->setAcceptSocket(listenSocket, serverAddr);
        std::cout << "Creating a new thread for the client...\n";
        clientThread->start();
        /** move the ownership of the unique_ptr to the list */
        clientThreads.push_back(std::move(clientThread));
       // for (auto& clientThread : clientThreads) {
        //    clientThread->start();
            /*sleep(1);
            clientThread->lockMutex();
            // ------ critical section -----
            if (!clientThread->loggedIn) {
                clientThread->loggedIn = true;
            }
            clientThread->messageRequest = true;
            // -----------------------------
            clientThread->signalCondition();
            clientThread->unlockMutex();
        */
        //}
    }
}

void TCPServer::dropConnections() {
    for (auto& clientThread : clientThreads) {
        if (!clientThread->loginRequest) {
            clientThread->closeSocket();
            clientThreads.remove(clientThread);
        }
    }
}