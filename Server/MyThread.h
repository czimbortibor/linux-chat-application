
/* 
 * File:   MyThread.h
 * Author: czimbortibor
 *
 * Created on December 15, 2016, 1:16 AM
 */

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <pthread.h>

#include <iostream>
#include <memory>
#include <string>

#include "Error.h"
#include "ThreadArgs.h"
#include "Packaging.h"

class MyThread {
public:
    MyThread();
    virtual ~MyThread();
    MyThread(const MyThread& original);
    const MyThread& operator=(const MyThread& other);
    
    void start();
    void join();
    
private:
    pthread_t thread;
    static void* entryFunction(void* threadPtr);
    
protected:
    std::string errorMsg;
    /** arguments for the thread */
    ThreadArgs* threadArgs;
    
    virtual void lockMutex() = 0;
    virtual void unlockMutex() = 0;
    virtual void signalCondition() = 0;
    virtual void* run() = 0;
};

#endif /* MYTHREAD_H */

