//
// Created by pedro on 30.09.20.
//

#include "HttpClient.h"
#include "HttpChunk.h"

HttpClient::HttpClient(const std::shared_ptr<std::string>& token) {
    this->token = std::shared_ptr(token);
    this->nc = std::make_unique<NetClient>();
}

/**
 * HTTP GET method handler
 * @param path - path to set request to
 * @return parsed http response
 */
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

/**
 * HTTP POST method handler
 * @param path - path to send request to
 * @param message - body of request
 * @return
 */
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

    // check for rate limit
    if (head.code == 429) {
        auto wait_str = head.headers["x-ratelimit-reset-after"];
        auto wait_f = std::strtod(wait_str.c_str(), nullptr);
        sleep(((int)wait_f) + 1);

        nc->send(post_message);

        head_maybe_chunk = nc->receive();
        sw = std::string_view(head_maybe_chunk);

        auto head_res = HttpHead::parse(sw);
        body_sw = std::get<0>(head_res);
        head = std::get<1>(head_res);
    }

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
