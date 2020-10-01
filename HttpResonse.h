//
// Created by pedro on 01.10.20.
//

#ifndef DISCORD_ISA_HTTPRESONSE_H
#define DISCORD_ISA_HTTPRESONSE_H


#include <string_view>
#include <string>

class HttpResonse {
public:
    HttpResonse(std::string response);

    std::string_view message;
    std::string_view body;
};


#endif //DISCORD_ISA_HTTPRESONSE_H
