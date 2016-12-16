
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include <list>
#include <cstring>

#include "ClientThread.h"


const int port = 10015;

int main() {
    int listenSocket = 0;
    struct sockaddr_in serverAddr;
    std::string errorMsg;
    
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

    int nrOfClients = 0;
    
    typedef std::shared_ptr<MyThread> threadPtr;
    std::list<threadPtr> threads;

    while(1<2) {   
        printf("\nListening for clients...\n");
        res = listen(listenSocket, 1);
        if (res < 0) {
            errorMsg = "error while listening on socket";
            error(errorMsg.c_str());
        }

        printf("Accepting client...\n");
        socklen_t addrSize = sizeof(serverAddr);
        int acceptSocket = accept(listenSocket, NULL, NULL);
        if (acceptSocket < 0) {
            errorMsg = "error while creating the accepting socket";
            error(errorMsg.c_str());
        }

        /** create thread for the new client */
        printf("Creating a new thread for the client...\n");
        ThreadArgs* threadArgs = new ThreadArgs();
        //threadArgs->messageBuff = new char[512];
        threadArgs->acceptSocket = acceptSocket;
        std::shared_ptr<MyThread> clientThread(new ClientThread(*threadArgs));
        
        threads.push_back(clientThread);
        /** start the thread */
        clientThread->start();
        clientThread->join();
        ++nrOfClients;

        close(acceptSocket);
    }
    
    return 0;
}
