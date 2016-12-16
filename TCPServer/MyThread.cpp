
/* 
 * File:   MyThread.cpp
 * Author: czimbortibor
 * 
 * Created on December 15, 2016, 1:16 AM
 */

#include "MyThread.h"

MyThread::MyThread(ThreadArgs& threadArgs) : threadArgs(&threadArgs) { 
    // std::cout << this->threadArgs->acceptSocket << " " << this->threadArgs->messageBuff << "\n";
}

MyThread::~MyThread() {
    if (threadArgs != NULL) {
        delete threadArgs;
        threadArgs = NULL;
    }
}

MyThread::MyThread(const MyThread& original) {
    
}

const MyThread& MyThread::operator=(const MyThread& other) {
    
}

void* MyThread::entryFunction(void* threadPtr) {
    MyThread* thread = static_cast<MyThread*>(threadPtr);
    thread->run();
    return NULL;
}

void MyThread::start() {
    int res = pthread_create(&thread, NULL, &MyThread::entryFunction, (void*) this);
    if (res != 0) {
        std::string errorMsg = "could not create thread";
        error(errorMsg.c_str());
    }
}

void MyThread::join() {
    int res = pthread_join(thread, NULL);
    if (res != 0) {
        std::string errorMsg = "could not join the thread";
        error(errorMsg.c_str());
    }
}
