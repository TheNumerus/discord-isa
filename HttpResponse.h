//
// Created by pedro on 01.10.20.
//

#ifndef DISCORD_ISA_HTTPRESPONSE_H
#define DISCORD_ISA_HTTPRESPONSE_H


#include <string_view>
#include <string>
#include "HttpHead.h"

class HttpResponse {
public:
    HttpResponse(HttpHead head, std::string body);

    HttpHead head;
    std::string body;
};


#endif //DISCORD_ISA_HTTPRESPONSE_H
