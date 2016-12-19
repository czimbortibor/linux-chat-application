
/* 
 * File:   ClientThread.h
 * Author: czimbortibor
 *
 * Created on December 15, 2016, 1:33 AM
 */

#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <cstring>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include "MyThread.h"
#include "User.h"
#include "Packaging.h"

class ClientThread : public MyThread {
public:
    ClientThread();
    virtual ~ClientThread();
    
    void lockMutex();
    void unlockMutex();
    void signalCondition();
    void closeSocket();
    
    virtual void* run();
    
    void onLoginRequest();
    void onMessageRequest();
    void onLogoutRequest();
    
    bool loginRequest = false;
    bool messageRequest = false;
    bool logoutRequest = false;
    
    void setAcceptSocket(int& listenSocket, struct sockaddr_in serverAddr);
    
private:
    int acceptSocket;
    char messageBuff[1024];
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    
    /** basic information about a user */
    User user;
    std::string readPackage();
    /** get the current system date + time */
    std::string getTime();
    
    Packaging packaging;
};

#endif /* CLIENTTHREAD_H */

