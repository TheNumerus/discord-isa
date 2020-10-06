//
// Created by pedro on 01.10.20.
//

#ifndef DISCORD_ISA_HTTPRESPONSE_H
#define DISCORD_ISA_HTTPRESPONSE_H


#include <string_view>
#include <string>

class HttpResponse {
public:
    HttpResponse(std::string response);

    std::string_view message;
    std::string_view body;
};


#endif //DISCORD_ISA_HTTPRESPONSE_H
