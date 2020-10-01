//
// Created by pedro on 30.09.20.
//

#include "NetClient.h"

bool NetClient::create() {

    this->socket_id = socket(PF_INET, sockets::SOCK_STREAM, sockets::IPPROTO_TCP);
    if (this->socket_id == -1) {
        return false;
    }

    if (!this->find_discord_ip()) {
        return false;
    }

    this->ssl_ctx = SSL_CTX_new(TLS_method());

    return true;
}

NetClient::~NetClient() {
    if (this->socket_id != 1) {
        close(this->socket_id);
    }
    SSL_CTX_free(this->ssl_ctx);
    if (this->discord_addr != nullptr) {
        free(this->discord_addr);
    }
    SSL_free(this->ssl);
}

bool NetClient::find_discord_ip() {
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

bool NetClient::connect() {
    // check if socket was even created
    if (this->socket_id == -1) {
        return false;
    }
    this->ssl = SSL_new(this->ssl_ctx);
    SSL_set_fd(this->ssl, this->socket_id);
    int status = sockets::connect(this->socket_id, this->discord_addr->ai_addr, this->discord_addr->ai_addrlen);
    SSL_connect(this->ssl);
    this->conection_made = status == 0;
    return status == 0;
}

std::string NetClient::send(std::string message) {
    // check if socket was even created and connection made
    if (this->socket_id == -1) {
        return "";
    }

    //if (!this->conection_made) {
        this->connect();
    //}

    std::cout << "in send";

    size_t bytes_send = -1;

    SSL_write_ex(this->ssl, message.c_str(), message.size() + 1, &bytes_send);

    if (bytes_send == -1) {
        // TODO handle error
    } else if (bytes_send != (message.size() + 1)) {
        // TODO message might be big so bytes send will not match message length, so try to resend
    }

    char buffer[10240] = {0};

    size_t bytes_recieved = -1;
    SSL_read_ex(this->ssl, buffer, 10240, &bytes_recieved);

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
