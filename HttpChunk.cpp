//
// Created by pedro on 06.10.20.
//

#include "HttpChunk.h"

HttpChunk::HttpChunk(std::string i) {
    size_t div = i.find("\r\n");
    if (div == i.npos) {
        throw std::runtime_error("HTTP parse error");
    }
    auto num_str = i.substr(0, div);

    this->size = std::strtol(num_str.c_str(), nullptr, 16);

    this->data = i.substr(div + 2, i.size() - (div + 2) - 2);
    size_t needed_len = this->data.size();
    if (this->size != needed_len) {
        throw std::runtime_error("HTTP parse error");
    }
}
