//
// Created by pedro on 30.09.20.
//

#include "Client.h"

bool Client::create() {

    this->socket_id = socket(PF_INET, sockets::SOCK_STREAM, sockets::IPPROTO_TCP);
    if (this->socket_id == -1) {
        return false;
    }

    if (!this->find_discord_ip()) {
        return false;
    }

    return true;
}

Client::~Client() {
    close(this->socket_id);
    if (this->discord_addr != nullptr) {
        free(this->discord_addr);
    }
}

Client::Client() {
    this->socket_id = 0;
    this->sa = {0};
}

bool Client::find_discord_ip() {
    struct sockets::addrinfo hints = {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = sockets::SOCK_STREAM;

    struct sockets::addrinfo *result;

    int status;

    if ((status = sockets::getaddrinfo("discord.com", "https", &hints, &result)) != 0) {
        // TODO handle
        sockets::freeaddrinfo(result);
        return false;
    }

    // now grab first result
    this->discord_addr = result;

    freeaddrinfo(result->ai_next);

    return true;
}

bool Client::connect() {
    // check if socket was even created
    if (this->socket_id == -1) {
        return false;
    }
    int status = sockets::connect(this->socket_id, this->discord_addr->ai_addr, this->discord_addr->ai_addrlen);
    this->conection_made = status == 0;
    return status == 0;
}

std::string Client::send(std::string message) {
    // check if socket was even created and connection made
    if (this->socket_id == -1 || !this->conection_made) {
        return "";
    }

    int bytes_send = sockets::send(this->socket_id, message.c_str(), message.size() + 1, 0);

    if (bytes_send == -1) {
        // TODO handle error
    } else if (bytes_send != (message.size() + 1)) {
        // TODO message might be big so bytes send will not match message length, so try to resend
    }

    char buffer[1024];

    int bytes_recieved = sockets::recv(this->socket_id, buffer, 1024, 0);

    if (bytes_recieved == -1) {
        // TODO handle error
    } else if (bytes_recieved == 0) {
        this->conection_made = false;
    } else {
        // TODO try to recieve more
    }

    std::string response = buffer;

    return response;
}
