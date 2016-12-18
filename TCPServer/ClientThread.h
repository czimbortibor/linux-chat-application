
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

#include "MyThread.h"
#include "User.h"

class ClientThread : public MyThread {
public:
    ClientThread(ThreadArgs& threadArgs);
    virtual ~ClientThread();
    
    void lockMutex();
    void unlockMutex();
    void signalCondition();
    void closeSocket();
    
    virtual void* run();
    
    void onTimeRequest();
    void onMessageRequest();
    
    bool loggedIn = false;
    bool messageRequest = false;
    
private:
    ThreadArgs* threadArgs;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    
    /** basic information about a user */
    User user;
    std::string readMessage();
    /** get the current system date + time */
    std::string getTime();
};

#endif /* CLIENTTHREAD_H */

