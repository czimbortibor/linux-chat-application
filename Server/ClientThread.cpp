
/*
 * File:   ClientThread.cpp
 * Author: czimbortibor
 *
 * Created on December 15, 2016, 1:33 AM
 */

#include "ClientThread.h"
#include "TCPServer.h"

typedef std::shared_ptr<std::list<std::unique_ptr<ClientThread>>> UserList;

ClientThread::ClientThread(TCPServer& tcpserver, UserList usersPtr, int acceptSocket) : tcpserver(&tcpserver),
        usersPtr(usersPtr) {
    this->acceptSocket = acceptSocket;

}

ClientThread::~ClientThread() {

}

void ClientThread::closeSocket() {
    close(acceptSocket);
}

void* ClientThread::run() {
    while (!logoutRequest) {
        // TODO: waitCondition();
        std::string package = readPackage();
        if (package.length() != 0) {
            std::cout << "received package from client: " << package << "\n";
        }

        std::string request;
        request = packaging.identifyRequest(package);
        
        if (request.compare("login_request") == 0) {
            std::cout << "request: " << "login_request" << "\n";
            onLoginRequest(package);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            sendUsersList();
        }
        else if (request.compare("logout_request") == 0) {
            std::cout << "request: " << "logout_request" << "\n";
            onLogoutRequest();
        }
        else if (request.compare("global_package") == 0) {
            std::cout << "request: " << "global_package" << "\n";
            onGlobalMessageRequest(package);
        } else if (request.compare("private_package") == 0) {
            std::cout << "request: " << "private_package" << "\n";
            onPrivateMessageRequest(package);
        } else if (request.compare("file_size_package") == 0) {
            std::cout << "request: " << "file transfer" << "\n";
            onFileTransfer(package);
        }
        
        //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}

void ClientThread::sendPackage(const std::string& package) {
    //tcpserver->lockMutex();
    int res = send(acceptSocket, package.c_str(), package.length(), 0);
    if (res < 0) {
        errorMsg = "Error while sending the message to the client!";
        error(errorMsg.c_str());
    }
    std::cout << "Message sent to the client.\n" ;
    //tcpserver->unlockMutex();
}

void ClientThread::sendPackageToAll(const std::string& package) {
    auto list = usersPtr.get();
    std::list<std::unique_ptr<ClientThread>>::const_iterator iterator;
    //std::cout << "number of online users: " << list->size() << "\n";

    tcpserver->lockMutex();
    for (iterator = list->begin(); iterator != list->end(); ++iterator) {
        (**iterator).sendPackage(package);
    }
    tcpserver->unlockMutex();
}

void ClientThread::sendPackageToTarget(const std::string& package, const std::string& target) {
    auto list = usersPtr.get();
    std::list<std::unique_ptr<ClientThread>>::const_iterator iterator;
    tcpserver->lockMutex();
    for (iterator = list->begin(); iterator != list->end(); ++iterator) {
        std::string username = (**iterator).getUsername();
        if (username.compare(target) == 0) {
            (**iterator).sendPackage(package);
            break;
        }
    }
    tcpserver->unlockMutex();
}

void ClientThread::onLoginRequest(const std::string& package) {
    int buffSize = sizeof(messageBuff);
    char sendBuff[buffSize];

    std::string serverTime = this->getTime();

    // create the new User object 
    packaging.parsePackage(package);
    std::string username = packaging.getSender();
    user = User(username, serverTime);
    
    // create the first package to be sent to the user, their login time
    snprintf(sendBuff, buffSize, "%.24s", serverTime.c_str());
    std::string outgoingPackage = packaging.createTimePackage(sendBuff);
    std::cout << "package: " << outgoingPackage << "\n";
    std::cout << "Sending the date+time to the client...\n";
    sendPackage(outgoingPackage);
}

void ClientThread::sendUsersList() {
    std::vector<std::string> onlineUsers = tcpserver->getOnlineUsers();
    // remove 
    /*std::vector<std::string>::const_iterator it = std::find(onlineUsers.begin(), onlineUsers.end(), getUsername());
    onlineUsers.erase(it);*/
    // send the online user's list to every user
    std::string package = packaging.createOnlineUsersPackage(onlineUsers);
    std::cout << "Sending the user's list to the clients...\n";
    std::cout << package << "\n";
    sendPackageToAll(package);
}

void ClientThread::onGlobalMessageRequest(const std::string& package) {
    std::cout << "Sending message to every user...\n";

    packaging.parsePackage(package);
    std::string message = packaging.getMessage();
    std::cout << "message: " << message << "\n";

    sendPackageToAll(package);
}

void ClientThread::onPrivateMessageRequest(const std::string& package) {
    packaging.parsePackage(package);
    std::string receiver = packaging.getReceiver();
    std::string message = packaging.getMessage();
    std::string sender = packaging.getSender();
    
    sendPackageToTarget(package, receiver);
}

void ClientThread::onFileTransfer(const std::string& package) {
    packaging.parsePackage(package);
    int fileSize = stoi(packaging.getMessage());
    std::cout << "file size: " << fileSize << "\n";
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::string readPackage = readFilePackage(fileSize);
    //std::cout << readPackage;
    std::ofstream file;
    file.open("../output.txt", std::ios::out);
    file << readPackage;
    file.close();
}

void ClientThread::onLogoutRequest() {
    std::cout << "Disconnecting user..\n";
    // TODO: remove the disconnected user from the online user's list
    //tcpserver->removeClient(*this);

    closeSocket();
    std::cout << "User disconnected\n";
    pthread_exit(NULL);
}

std::string ClientThread::readPackage() {
    int buffSize = sizeof(messageBuff);

    int res = recv(acceptSocket, messageBuff, buffSize, 0);
    if (res > 0) {
        std::cout << " bytes: " << res << "\n";
    }
    else if (res == 0) {
        tcpserver->removeClient(user.getUsername());
        pthread_exit(NULL);
    }
    else {
        errorMsg = "error when receiving the message from the client!\n";
        error(errorMsg.c_str());
    }
    messageBuff[res] = '\0';
    return messageBuff;
}

std::string ClientThread::readFilePackage(int fileSize) {
    int buffSize = sizeof(messageBuff);
    char msgBuff[fileSize];
    
    int res = recv(acceptSocket, msgBuff, fileSize, 0);
    if (res > 0) {
        std::cout << "\n file bytes read: " << res << "\n";
    }
    else {
        errorMsg = "error when receiving the message from the client!\n";
        error(errorMsg.c_str());
    }
    msgBuff[res] = '\0';
    return msgBuff;
}

std::string ClientThread::getTime() {
    time_t now;
    now = time(NULL);
    std::string currentTime = ctime(&now);
    return currentTime;
}

std::string ClientThread::getUsername() {
    return user.getUsername();
}
