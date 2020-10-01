//
// Created by pedro on 30.09.20.
//

#include "HTTPClient.h"

HTTPClient::HTTPClient(const std::shared_ptr<std::string>& token) {
    this->token = token;
    this->nc = NetClient();
    nc.create();
}

bool HTTPClient::get(std::string path) {
    std::string message = "GET " + path + " HTTP/1.1\r\n" +
        "Host: discord.com\r\n" +
        "Authorization: Bot "+ this->token.get()->c_str() + "\r\n\r\n";
    //std::cout << message << std::endl;
    //nc.connect();
    std::cout << nc.send(message) << std::endl;
    return false;
}

bool HTTPClient::post() {
    return false;
}
