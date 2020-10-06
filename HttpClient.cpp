//
// Created by pedro on 30.09.20.
//

#include "HttpClient.h"

HttpClient::HttpClient(const std::shared_ptr<std::string>& token) {
    this->token = std::shared_ptr(token);
    this->nc = std::make_unique<NetClient>();
}

HttpResponse HttpClient::get(const std::string &path) {
    std::string message = "GET " + path + " HTTP/1.1\r\n" +
        "Host: discord.com\r\n" +
        "Accept: */*\r\n" +
        "User-Agent: DiscordBot (https://github.com/TheNumerus/discord-isa, 0.1.0)\r\n"
        "Authorizatfion: Bot " + *this->token + "\r\n\r\n";
    nc->send(message);
    std::cout << nc->receive() << nc->receive() << std::endl;
    return HttpResponse("");
}

HttpResponse HttpClient::post(const std::string& message) {
    return HttpResponse("");
}
