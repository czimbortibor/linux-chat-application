
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

#include <list>

#include "MyThread.h"
#include "User.h"
#include "Packaging.h"


class TCPServer;

class ClientThread : public MyThread {
    typedef std::shared_ptr<std::list<std::unique_ptr<ClientThread>>> UserList;
public:
    explicit ClientThread(TCPServer& tcpserver, UserList usersPtr, int acceptSocket);
    virtual ~ClientThread();
    
    virtual void* run();
    
    void onLoginRequest();
    void onLogoutRequest();
    void onGlobalMessageRequest(std::string package);
    
    bool logoutRequest = false;
    
    void closeSocket();
    
private:
    int acceptSocket;
    char messageBuff[1024];
    
    /** basic information about a user */
    User user;
    void sendPackage(const std::string& package);
    std::string readPackage();
    /** get the current system date + time */
    std::string getTime();
    
    /** a reference back to the server */
    std::shared_ptr<TCPServer> tcpserver;
    Packaging packaging;
    UserList usersPtr;
};

#endif /* CLIENTTHREAD_H */

