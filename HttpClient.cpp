//
// Created by pedro on 30.09.20.
//

#include "HttpClient.h"
#include "HttpChunk.h"

HttpClient::HttpClient(const std::shared_ptr<std::string>& token) {
    this->token = std::shared_ptr(token);
    this->nc = std::make_unique<NetClient>();
}

HttpResponse HttpClient::get(const std::string &path) {
    std::string message = "GET " + path + " HTTP/1.1\r\n" +
        "Host: discord.com\r\n" +
        "Accept: */*\r\n" +
        "User-Agent: DiscordBot (https://github.com/TheNumerus/discord-isa, 0.1.0)\r\n"
        "Authorization: Bot " + *this->token + "\r\n\r\n";
    nc->send(message);

    auto head_maybe_chunk = nc->receive();
    std::string_view sw(head_maybe_chunk);

    auto [body_sw, head] = HttpHead::parse(sw);
    std::string body(body_sw);


    if (head.headers.find("Transfer-Encoding") == head.headers.end()) {
        // no more chunks
        return HttpResponse(head, body);
    }

    std::string chunked_response;

    while(true) {
        try {
            auto chunk = HttpChunk(body);
            if (chunk.size == 0) {
                break;
            } else {
                chunked_response.append(chunk.data);
                body = nc->receive();
            }
        } catch (...) {
            body.append(nc->receive());
        }
    };

    return HttpResponse(head, chunked_response);
}

HttpResponse HttpClient::post(const std::string& path, const std::string& message) {
    std::string post_message = "POST " + path + " HTTP/1.1\r\n" +
                          "Host: discord.com\r\n" +
                          "Accept: */*\r\n" +
                          "User-Agent: DiscordBot (https://github.com/TheNumerus/discord-isa, 0.1.0)\r\n" +
                          "Content-Type: application/json\r\n" +
                          "Content-Length: " + std::to_string(message.size()) + "\r\n" +
                          "Authorization: Bot " + *this->token + "\r\n\r\n" + message;
    nc->send(post_message);

    auto head_maybe_chunk = nc->receive();
    std::string_view sw(head_maybe_chunk);

    auto [body_sw, head] = HttpHead::parse(sw);
    std::string body(body_sw);

    if (head.headers.find("Transfer-Encoding") == head.headers.end()) {
        // no more chunks
        return HttpResponse(head, body);
    }

    std::string chunked_response;

    while(true) {
        try {
            auto chunk = HttpChunk(body);
            if (chunk.size == 0) {
                break;
            } else {
                chunked_response.append(chunk.data);
                body = nc->receive();
            }
        } catch (...) {
            //body.append(body);
            body.append(nc->receive());
        }
    };

    return HttpResponse(head, chunked_response);
}
