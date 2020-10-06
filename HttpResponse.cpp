//
// Created by pedro on 01.10.20.
//

#include "HttpResponse.h"

HttpResponse::HttpResponse(HttpHead head, std::string body) : head(head) {
    this->head = head;
    this->body = body;
}
