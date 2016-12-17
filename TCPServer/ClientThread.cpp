
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
    std::string errorMsg;
    
    /** lock the mutex and wait for signal */
    pthread_mutex_lock(&mutex);
    while(!loggedIn) {
        /** automatically and atomically unlocks the mutex while it waits */
        pthread_cond_wait(&condition, &mutex);
        std::cout << "Preparing the message for the client...\n";
        int acceptSocket = threadArgs->acceptSocket;
        int sizeOfMsg = sizeof(threadArgs->messageBuff);
        char sendBuff[sizeOfMsg];

        // std::memcpy(sendBuff, threadData->sendBuff, lenOfBuff+1);

        /** get the current system date + time */
        time_t serverTime;
        serverTime = time(NULL);
        std::string loginTime = ctime(&serverTime);
        /** create the new User object */
        user = User(loginTime);
        /** prepare the first package to be sent to the user, their login time */
        snprintf(sendBuff, sizeOfMsg, "%.24s", ctime(&serverTime));

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
