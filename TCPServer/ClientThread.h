
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

class ClientThread : public MyThread {
public:
    ClientThread(ThreadArgs& threadArgs);
    virtual ~ClientThread();
    
    void lockMutex();
    void unlockMutex();
    void signalCondition();
    
    virtual void* run();
    
    bool loggedIn = false;
    
private:
    ThreadArgs* threadArgs;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
};

#endif /* CLIENTTHREAD_H */

