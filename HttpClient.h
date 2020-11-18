//
// Created by pedro on 30.09.20.
//

#ifndef DISCORD_ISA_HTTPCLIENT_H
#define DISCORD_ISA_HTTPCLIENT_H


#include "NetClient.h"
#include "HttpResponse.h"
#include "HttpHead.h"

/**
 * HTTP 1.1 client, not very great or customiazble
 */
class HttpClient {
public:
    HttpClient(const std::shared_ptr<std::string>&);
    HttpResponse get(const std::string& path);
    HttpResponse post(const std::string& path, const std::string& message);

private:
    std::unique_ptr<NetClient> nc;
    std::shared_ptr<std::string> token;
};


#endif //DISCORD_ISA_HTTPCLIENT_H
