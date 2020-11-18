//
// Created by pedro on 06.10.20.
//

#ifndef DISCORD_ISA_HTTPCHUNK_H
#define DISCORD_ISA_HTTPCHUNK_H

#include <string>
#include <stdexcept>

/**
 * HTTP chunked transfer encoding handler
 */
class HttpChunk {
public:
    HttpChunk(std::string);

    long unsigned int size;
    std::string data;
};


#endif //DISCORD_ISA_HTTPCHUNK_H
