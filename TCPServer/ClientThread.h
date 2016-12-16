
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
    virtual void* run();
    
private:
    ThreadArgs* threadArgs;
};

#endif /* CLIENTTHREAD_H */

