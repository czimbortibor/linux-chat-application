
/* 
 * File:   ClientThread.cpp
 * Author: czimbortibor
 * 
 * Created on December 15, 2016, 1:33 AM
 */

#include "ClientThread.h"

ClientThread::ClientThread() {
    /** initializing mutex and condition variables */
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);
}

ClientThread::~ClientThread() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);
}

void ClientThread::lockMutex() {
    pthread_mutex_lock(&mutex);
}

void ClientThread::unlockMutex() {
    pthread_mutex_unlock(&mutex);
}

void ClientThread::signalCondition() {
    pthread_cond_signal(&condition);
}

void ClientThread::closeSocket() {
    close(acceptSocket);
}

void ClientThread::setAcceptSocket(int& listenSocket, struct sockaddr_in serverAddr) {
    socklen_t addrSize = sizeof(serverAddr);
    acceptSocket = accept(listenSocket, NULL, NULL);
    if (acceptSocket < 0) {
        errorMsg = "error while creating the accepting socket";
        error(errorMsg.c_str());
    }
}

void* ClientThread::run() {
    while (!logoutRequest) {
        std::string package = readPackage();
        if (package.length() != 0) {
            std::cout << "received message from client: " << package << "\n";
        }
        
        std::string request;
        //packaging.parsePackage(package);
        request = packaging.identifyRequest(package);
        std::cout << "request: " << request << "\n";
        if (request.compare("login_request") == 0) {
            onLoginRequest();
        }
        sleep(3);
    }
}

void ClientThread::onLoginRequest() {
    /** lock the mutex and wait for signal */
    //pthread_mutex_lock(&mutex);
    //while(!loginRequest) {
        /** automatically and atomically unlocks the mutex while it waits */
       // pthread_cond_wait(&condition, &mutex);
        std::cout << "Preparing the message for the client...\n";
        int buffSize = sizeof(messageBuff);
        char sendBuff[buffSize];

        std::string serverTime = this->getTime();
        
        /** create the new User object */
        user = User(serverTime);
        /** creat the first package to be sent to the user, their login time */
        snprintf(sendBuff, buffSize, "%.24s", serverTime.c_str());
        std::string package = packaging.createTimePackage(sendBuff);
        std::cout << "package: " << package << "\n";
        std::cout << "Sending the date+time to the client...\n";
        int res = send(acceptSocket, package.c_str(), package.length(), 0);
        if (res < 0) {
            errorMsg = "Error while sending the message to the client!";
            error(errorMsg.c_str());
        }
        std::cout << "Message sent to the client.\n" ;
    //}
    //pthread_mutex_unlock(&mutex);
}

void ClientThread::onMessageRequest() {
    /** lock the mutex and wait for signal */
    pthread_mutex_lock(&mutex);
    while(!messageRequest) {
        /** automatically and atomically unlocks the mutex while it waits */
        pthread_cond_wait(&condition, &mutex);
        std::cout << "Preparing the message for the client...\n";
        int buffSize = sizeof(messageBuff);
        char sendBuff[buffSize];
        std::string message = readPackage();
    }
    pthread_mutex_unlock(&mutex);
}

void ClientThread::onLogoutRequest() {
    close(acceptSocket);
}

std::string ClientThread::readPackage() {
    int buffSize = sizeof(messageBuff);
    char msgBuff[1024];
    
    /*fd_set readfd;
    FD_ZERO(&readfd);
    FD_SET(acceptSocket, &readfd);

    timeval tv;
    tv.tv_sec = 999;
    tv.tv_usec = 0;
    int res = select(acceptSocket + 1, &readfd, NULL, NULL, &tv);
    if (res > 0) {
        std::cout << "res: " << res << "\n";
        std::cout << "read socket: " << acceptSocket;*/
    
    int res = recv(acceptSocket, msgBuff, buffSize, 0);
    std::cout << "  bytes: " << res << "\n";
    if (res < 0) {
        errorMsg = "error when receiving the message from the client!\n";
        error(errorMsg.c_str());
    }
    msgBuff[res] = '\0';
    //std::cout << "received message from client: " << msgBuff << "\n";
    // }
    return msgBuff;
}

std::string ClientThread::getTime() {
    time_t now;
    now = time(NULL);
    std::string currentTime = ctime(&now);
    return currentTime;
}