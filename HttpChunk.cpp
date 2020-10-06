//
// Created by pedro on 06.10.20.
//

#include "HttpChunk.h"

HttpChunk::HttpChunk(std::string i) {
    int div = i.find("\r\n");
    auto num_str = i.substr(0, div);

    this->size = std::strtol(num_str.c_str(), nullptr, 16);
    this->data = i.substr(div + 2, i.size() - (div + 2) - 2);
}
