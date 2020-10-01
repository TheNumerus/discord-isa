//
// Created by pedro on 30.09.20.
//

#ifndef DISCORD_ISA_HTTPCLIENT_H
#define DISCORD_ISA_HTTPCLIENT_H


#include "NetClient.h"

class HTTPClient {
public:
    HTTPClient(const std::shared_ptr<std::string>&);
    bool get(std::string message);
    bool post();

private:
    NetClient nc;
    std::shared_ptr<std::string> token;
};


#endif //DISCORD_ISA_HTTPCLIENT_H
