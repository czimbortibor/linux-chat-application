
/* 
 * File:   ClientThread.cpp
 * Author: czimbortibor
 * 
 * Created on December 15, 2016, 1:33 AM
 */

#include "ClientThread.h"

ClientThread::ClientThread(ThreadArgs& threadArgs) : MyThread(threadArgs), threadArgs(&threadArgs) {
    /** initializing mutex and condition variables */
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);
}

ClientThread::~ClientThread() {
    if (threadArgs != NULL) {
        delete threadArgs;
        threadArgs = NULL;
    }
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

void* ClientThread::run() {
    
    /** lock the mutex and wait for signal */
    pthread_mutex_lock(&mutex);
    while(!loggedIn) {
        /** automatically and atomically unlocks the mutex while it waits */
        pthread_cond_wait(&condition, &mutex);
        std::cout << "Preparing the message for the client...\n";
        int acceptSocket = threadArgs->acceptSocket;
        int buffSize = sizeof(threadArgs->messageBuff);
        char sendBuff[buffSize];

        // std::memcpy(sendBuff, threadData->sendBuff, lenOfBuff+1);

        std::string serverTime = this->getTime();
        
        /** create the new User object */
        user = User(serverTime);
        /** prepare the first package to be sent to the user, their login time */
        
        snprintf(sendBuff, buffSize, "%.24s", serverTime.c_str());

        std::cout << "Sending the date+time to the client...\n";
        int res = send(acceptSocket, sendBuff, strlen(sendBuff), 0);
        if (res < 0) {
            errorMsg = "Error while sending the message to the client!";
            error(errorMsg.c_str());
        }
        std::cout << "Message sent to the client.\n" ;
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

std::string ClientThread::readMessage() {
    int buffSize = sizeof(threadArgs->messageBuff);
    char msgBuff[buffSize];
    int acceptSocket = threadArgs->acceptSocket;
    int res = recv(acceptSocket, msgBuff, buffSize, 0);
    if (res < 0) {
        errorMsg = "error when receiving the message from the client!\n";
	error(errorMsg.c_str());
    }
    msgBuff[res] = '\0';
    std::cout << "received message from client: " << msgBuff << "\n";
}

std::string ClientThread::getTime() {
    time_t now;
    now = time(NULL);
    std::string currentTime = ctime(&now);
    return currentTime;
}