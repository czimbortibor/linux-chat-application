
/* 
 * File:   TCPServer.h
 * Author: czimbortibor
 *
 * Created on December 16, 2016, 8:17 PM
 */

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "ClientThread.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <list>

/** std::shared_ptr<MyThread> */
// typedef std::shared_ptr<MyThread> threadPtr;

/** synchronous TCP server where every client is managed in their own separate thread */
class TCPServer {
public:
    TCPServer(const char* address = "127.0.0.1", int port = 10013);
    TCPServer(const TCPServer& original);
    virtual ~TCPServer();
    
    void initServer();
    void startServer();
    
private:
    const char* address;
    int port;
    /** socket the server uses as an entry point for incoming connections */
    int listenSocket = 0;
    struct sockaddr_in serverAddr;
    /** error message for the various system calls */
    std::string errorMsg = "";
    /** return value for the various system calls */
    int res = 0;
    int nrOfClients = 0;
    /** thread for every client */
    std::list<std::shared_ptr<ClientThread>> clientThreads;
};

#endif /* TCPSERVER_H */

