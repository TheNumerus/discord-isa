//
// Created by pedro on 30.09.20.
//

#ifndef DISCORD_ISA_NETCLIENT_H
#define DISCORD_ISA_NETCLIENT_H

#include <unistd.h>
#include <memory>
#include <cstring>
#include <iostream>
#include <openssl/ssl.h>
#include <string>

// NetClient methods would shadow socket functions otherwise
namespace sockets {
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <arpa/inet.h>
}

class NetClient {
public:
    NetClient();
    bool connect();
    void send(std::string message);
    std::string receive();

    virtual ~NetClient();

private:
    struct sockets::addrinfo* discord_addr = nullptr;
    int socket_id = -1;
    bool connection_made = false;

    ssl_ctx_st* ssl_ctx = nullptr;
    ssl_st* ssl = nullptr;

    bool find_discord_ip();
};


#endif //DISCORD_ISA_NETCLIENT_H
