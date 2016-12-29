
#include "TCPServer.h"

int main() {
    std::string address = "127.0.0.1";
    const int port = 10013;
    
    TCPServer tcpServer(address.c_str(), port);
    tcpServer.startServer();
    
    return 0;
}