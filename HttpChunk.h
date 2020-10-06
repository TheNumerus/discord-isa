//
// Created by pedro on 06.10.20.
//

#ifndef DISCORD_ISA_HTTPCHUNK_H
#define DISCORD_ISA_HTTPCHUNK_H

#include <string>
#include <stdexcept>

class HttpChunk {
public:
    HttpChunk(std::string);

    int size;
    std::string data;
};


#endif //DISCORD_ISA_HTTPCHUNK_H