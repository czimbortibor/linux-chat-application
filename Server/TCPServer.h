
/* 
 * File:   TCPServer.h
 * Author: czimbortibor
 *
 * Created on December 16, 2016, 8:17 PM
 */

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <list>

#include "ClientThread.h"

typedef std::shared_ptr<std::list<std::unique_ptr<ClientThread>>> UserList;

/** synchronous TCP server where every client is managed in their own separate thread */
class TCPServer {
public:
    TCPServer(const char* address = "127.0.0.1", int port = 10013);
    virtual ~TCPServer();
    
    void startServer();
    void removeClient(ClientThread& clientThread);
    
    void lockMutex() { pthread_mutex_lock(&mutex); }
    void unlockMutex() { pthread_mutex_unlock(&mutex); }
    void signalCondition() { pthread_cond_signal(&condition); }
    /** automatically and atomically unlocks the mutex while it waits */
    void waitCondition() { pthread_cond_wait(&condition, &mutex); }
    
private:
    const char* address;
    int port;
    /** socket the server uses as a temporary entry point for incoming connections */
    int listenSocket;
    int acceptSocket;
    struct sockaddr_in serverAddr;
    /** error message for the various system calls */
    std::string errorMsg;
    /** return value for the various system calls */
    int res;
    
    /** thread for every user */
    std::list<std::unique_ptr<ClientThread>> clientThreads;
    /** pointer to the list of clients, as that will be shared among every one of them */
    UserList usersPtr;
    
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    
    /** initializes the various host attributes and the listening socket */
    void initServer();
    void listenServer();
};

#endif /* TCPSERVER_H */

