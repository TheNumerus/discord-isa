//
// Created by pedro on 01.10.20.
//

#include "HttpResponse.h"

HttpResponse::HttpResponse(HttpHead head, std::string body) : head(head) {
    this->head = head;
    this->body = body;
}

std::ostream &operator<<(std::ostream &os, const HttpResponse &response) {
    os << response.head << std::endl;
    os << response.body << std::endl;
    return os;
}
