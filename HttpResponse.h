//
// Created by pedro on 01.10.20.
//

#ifndef DISCORD_ISA_HTTPRESPONSE_H
#define DISCORD_ISA_HTTPRESPONSE_H


#include <string_view>
#include <string>
#include <ostream>
#include "HttpHead.h"

class HttpResponse {
public:
    HttpResponse(HttpHead head, std::string body);

    HttpHead head;
    std::string body;

    friend std::ostream &operator<<(std::ostream &os, const HttpResponse &response);
};


#endif //DISCORD_ISA_HTTPRESPONSE_H
