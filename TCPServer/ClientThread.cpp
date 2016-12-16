
/* 
 * File:   ClientThread.cpp
 * Author: czimbortibor
 * 
 * Created on December 15, 2016, 1:33 AM
 */

#include "ClientThread.h"

ClientThread::ClientThread(ThreadArgs& threadArgs): MyThread(threadArgs), threadArgs(&threadArgs) {
    // std::cout << this->threadArgs->acceptSocket << " " << this->threadArgs->messageBuff << "\n";
}

ClientThread::~ClientThread() {
    if (threadArgs != NULL) {
        delete threadArgs;
        threadArgs = NULL;
    }
}

void* ClientThread::run() {
    std::string errorMsg;
    printf("Preparing the message for the client...\n");
    int acceptSocket = threadArgs->acceptSocket;
    int sizeOfMsg = sizeof(threadArgs->messageBuff);
    char sendBuff[sizeOfMsg];
   
    // std::memcpy(sendBuff, threadData->sendBuff, lenOfBuff+1);
    
    /** get the current system date + time */
    time_t serverTime;
    serverTime = time(NULL);
    snprintf(sendBuff, sizeOfMsg, "%.24s", ctime(&serverTime));

    printf("Sending the date/time to the client...\n");
    int res = send(acceptSocket, sendBuff, strlen(sendBuff), 0);
    if (res < 0) {
        errorMsg = "Error while sending the message to the client!";
        error(errorMsg.c_str());
    }
    printf("Message sent to the client.\n");
    pthread_exit(NULL);
} 
