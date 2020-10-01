//
// Created by pedro on 30.09.20.
//

#ifndef DISCORD_ISA_NETCLIENT_H
#define DISCORD_ISA_NETCLIENT_H

#include <unistd.h>
#include <memory>
#include <cstring>
#include <iostream>

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

    bool create();
    bool connect();
    std::string send(std::string message);

    virtual ~NetClient();

private:
    struct sockets::sockaddr_in sa;
    struct sockets::addrinfo* discord_addr = nullptr;
    int socket_id = -1;
    bool conection_made = false;

    bool find_discord_ip();
};


#endif //DISCORD_ISA_NETCLIENT_H
