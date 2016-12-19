
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

typedef std::shared_ptr<std::list<std::unique_ptr<ClientThread>>> UserList;

/** synchronous TCP server where every client is managed in their own separate thread */
class TCPServer {
public:
    TCPServer(const char* address = "127.0.0.1", int port = 10013);
    TCPServer(const TCPServer& original);
    virtual ~TCPServer();
    
    /** initializes the various host attributes and the listening socket */
    void initServer();
    void startServer();
    
private:
    const char* address;
    int port;
    /** socket the server uses as a temporary entry point for incoming connections */
    int listenSocket;
    struct sockaddr_in serverAddr;
    /** error message for the various system calls */
    std::string errorMsg;
    /** return value for the various system calls */
    int res;
    
    /** thread for every user */
    std::list<std::unique_ptr<ClientThread>> clientThreads;
    /** pointer to the list of clients, as that will be shared among every one of them */
    UserList usersPtr;
};

#endif /* TCPSERVER_H */

