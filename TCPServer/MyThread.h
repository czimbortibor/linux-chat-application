
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

class MyThread {
public:
    MyThread(ThreadArgs& threadArgs);
    // MyThread(std::unique_ptr<Runnable> runnable);
    virtual ~MyThread();
    MyThread(const MyThread& original);
    const MyThread& operator=(const MyThread& other);
    void start();
    void join();
    
private:
    pthread_t thread;
    // std::unique_ptr<Runnable> runnable;
    static void* entryFunction(void* threadPtr);
    
protected:
    ThreadArgs* threadArgs;
    virtual void* run() = 0;
};

#endif /* MYTHREAD_H */

