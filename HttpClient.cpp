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
    auto [body, head] = HttpHead::parse(head_maybe_chunk);

    if (head.headers.find("Transfer-Encoding") == head.headers.end()) {
        // no more chunks
        return HttpResponse(head, body);
    }

    std::string chunked_response;
    bool last_chunk = false;
    do {
        auto chunk = HttpChunk(body);
        if (chunk.size == 0) {
            last_chunk = true;
        } else {
            chunked_response.append(chunk.data);
            body = nc->receive();
        }
    } while (!last_chunk);

    std::cout << head << std::endl;
    std::cout << chunked_response << std::endl;
    return HttpResponse(head, chunked_response);
}

HttpResponse HttpClient::post(const std::string& message) {
    return HttpResponse(HttpHead(500), "");
}
