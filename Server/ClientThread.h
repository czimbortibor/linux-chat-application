
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

#include <chrono>
#include <thread>
#include <list>
#include <vector>
#include <algorithm>

#include "../util/Packaging.h"
#include "MyThread.h"
#include "User.h"


class TCPServer;

class ClientThread : public MyThread {
    typedef std::shared_ptr<std::list<std::unique_ptr<ClientThread>>> UserList;
public:
    explicit ClientThread(TCPServer& tcpserver, UserList usersPtr, int acceptSocket);
    virtual ~ClientThread();
    
    virtual void* run();
    
    void onLoginRequest(const std::string& package);
    void onLogoutRequest();
    void onGlobalMessageRequest(const std::string& package);
    void onPrivateMessageRequest(const std::string& package);
    
    bool logoutRequest = false;
    
    void closeSocket();
    
    /** get the current system date + time */
    std::string getTime();
    std::string getUsername();
    
private:
    int acceptSocket;
    char messageBuff[1024];
    
    /** basic information about a user */
    User user;
    void sendPackage(const std::string& package);
    /** reads the incoming package from the user */
    std::string readPackage();
    /** sends the package to every online user*/
    void sendPackageToAll(const std::string& package);
    void sendPackageToTarget(const std::string& package, const std::string& target);
    void sendUsersList();
    
    /** a reference back to the server */
    std::shared_ptr<TCPServer> tcpserver;
    Packaging packaging;
    UserList usersPtr;
};

#endif /* CLIENTTHREAD_H */

