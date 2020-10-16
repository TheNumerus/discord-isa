//
// Created by pedro on 06.10.20.
//

#ifndef DISCORD_ISA_HTTPHEAD_H
#define DISCORD_ISA_HTTPHEAD_H


#include <map>
#include <string>
#include <stdexcept>
#include <ostream>

// most of this code was written in Rust first, then ported

class HttpHead {
public:
    int code;

    HttpHead(int code);

    std::map<std::string, std::string> headers;
    static std::pair<std::string_view, HttpHead> parse(std::string_view s);

    friend std::ostream &operator<<(std::ostream &os, const HttpHead &header);
};


#endif //DISCORD_ISA_HTTPHEAD_H
