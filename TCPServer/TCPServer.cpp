
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
}

void TCPServer::initServer() {
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket < 0) {
        errorMsg = "error while opening the server socket"; 
        error(errorMsg.c_str());
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(port);

    int res = bind(listenSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
    if (res < 0) {
        errorMsg = "error on binding";
        error(errorMsg.c_str());
    }
}

void TCPServer::startServer() {
    while(1<2) {
        std::cout << "\nListening for clients...\n";
        res = listen(listenSocket, 1);
        if (res < 0) {
            errorMsg = "error while listening on socket";
            error(errorMsg.c_str());
        }

        std::cout << "Accepting client...\n";
        socklen_t addrSize = sizeof(serverAddr);
        int acceptSocket = accept(listenSocket, NULL, NULL);
        if (acceptSocket < 0) {
            errorMsg = "error while creating the accepting socket";
            error(errorMsg.c_str());
        }

        /** create thread for the new client */
        std::cout << "Creating a new thread for the client...\n";
        ThreadArgs* threadArgs = new ThreadArgs();
        //threadArgs->messageBuff = new char[512];
        threadArgs->acceptSocket = acceptSocket;
        std::shared_ptr<ClientThread> clientThread(new ClientThread(*threadArgs));
        clientThreads.push_back(clientThread);
        
        clientThread->start();
        sleep(1);
        clientThread->lockMutex();
        // ------ critical section -----
        clientThread->loggedIn = true;
        // -----------------------------
        clientThread->signalCondition();
        clientThread->unlockMutex();
        
        clientThread->join();
        ++nrOfClients;

        close(acceptSocket);
    }
}
