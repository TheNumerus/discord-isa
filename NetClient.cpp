//
// Created by pedro on 30.09.20.
//

#include "NetClient.h"

NetClient::NetClient() {
    this->socket_id = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->socket_id == -1) {
        throw std::runtime_error("Could not create socket");
    }

    if (!this->find_discord_ip()) {
        throw std::runtime_error("Could not find discord IP");
    }

    // merlin has older version than my system and eva
#if OPENSSL_VERSION_NUMBER<0x10100000
    this->ssl_ctx = SSL_CTX_new(TLSv1_2_client_method());
#else
    this->ssl_ctx = SSL_CTX_new(TLS_client_method());
#endif

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

/**
 * Will try to get discord api IP address
 * @return tree if something was found
 */
bool NetClient::find_discord_ip() {
    struct addrinfo hints = {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *result;

    int status = getaddrinfo("discord.com", "https", &hints, &result);

    if (status != 0) {
        return false;
    }

    // now grab first result
    this->discord_addr = result;
    freeaddrinfo(result->ai_next);

    return true;
}

/**
 * Initialize connection
 * @return true if operation was successful
 */
bool NetClient::conn() {
    this->ssl = SSL_new(this->ssl_ctx);
    SSL_set_fd(this->ssl, this->socket_id);
    int status = connect(this->socket_id, this->discord_addr->ai_addr, this->discord_addr->ai_addrlen);

    int ssl_conn_res = SSL_connect(this->ssl);
    if (ssl_conn_res != 1) {
        throw std::runtime_error("SSL error code: " + std::to_string(SSL_get_error(this->ssl, ssl_conn_res)));
    }
    this->connection_made = status == 0;
    return status == 0;
}

/**
 * Sends message to discord server
 * @param message string to send
 */
void NetClient::send(std::string message) {
    if (!this->connection_made) {
        this->conn();
    }

    int write_res = SSL_write(this->ssl, message.data(), message.size());

    if (write_res <= 0) {
        int err = SSL_get_error(this->ssl, write_res);
        std::string message = "SSL write error " + std::to_string(err);
        throw std::runtime_error(message);
    }
}

/**
 * Recieves message from discord server
 * @return recieved message
 */
std::string NetClient::receive() {
    // 4000 was too little, so double it
    char buffer[8000] = {0};

    std::string response;

    int read_res = SSL_read(this->ssl, buffer, 8000);

    if (read_res <= 0) {
        int err = SSL_get_error(this->ssl, read_res);
        std::string message = "SSL read error " + std::to_string(err);
        throw std::runtime_error(message);
    }

    response.append(buffer);
    return response;
}

