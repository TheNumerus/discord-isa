//
// Created by pedro on 30.09.20.
//

#include "NetClient.h"

NetClient::NetClient() {
    this->socket_id = sockets::socket(PF_INET, sockets::SOCK_STREAM, sockets::IPPROTO_TCP);
    if (this->socket_id == -1) {
        throw std::runtime_error("Could not create socket");
    }

    if (!this->find_discord_ip()) {
        throw std::runtime_error("Could not find discord IP");
    }

    this->ssl_ctx = SSL_CTX_new(TLS_client_method());
}

NetClient::~NetClient() {
    if (this->ssl != nullptr) {
        SSL_free(this->ssl);
    }

    if (this->ssl_ctx != nullptr) {
        SSL_CTX_free(this->ssl_ctx);
    }

    if (this->socket_id != 1) {
        close(this->socket_id);
    }

    if (this->discord_addr != nullptr) {
        free(this->discord_addr);
    }

}

bool NetClient::find_discord_ip() {
    struct sockets::addrinfo hints = {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = sockets::SOCK_STREAM;

    struct sockets::addrinfo *result;

    int status = sockets::getaddrinfo("discord.com", "https", &hints, &result);

    if (status != 0) {
        return false;
    }

    // now grab first result
    this->discord_addr = result;
    sockets::freeaddrinfo(result->ai_next);

    return true;
}

bool NetClient::connect() {
    this->ssl = SSL_new(this->ssl_ctx);
    SSL_set_fd(this->ssl, this->socket_id);
    int status = sockets::connect(this->socket_id, this->discord_addr->ai_addr, this->discord_addr->ai_addrlen);

    int ssl_conn_res = SSL_connect(this->ssl);
    if (ssl_conn_res != 1) {
        throw std::runtime_error("SSL error code: " + std::to_string(SSL_get_error(this->ssl, ssl_conn_res)));
    }
    this->connection_made = status == 0;
    return status == 0;
}

void NetClient::send(std::string message) {
    if (!this->connection_made) {
        this->connect();
    }

    int write_res = SSL_write(this->ssl, message.data(), message.size());

    if (write_res <= 0) {
        throw std::runtime_error("SSL write error");
    }
}

std::string NetClient::receive() {
    char buffer[8000] = {0};

    std::string response;

    int read_res = SSL_read(this->ssl, buffer, 8000);

    if (read_res <= 0) {
        throw std::runtime_error("SSL read error");
    }

    response.append(buffer);
    return response;
}

