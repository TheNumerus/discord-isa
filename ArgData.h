//
// Created by pedro on 29.09.20.
//

#ifndef DISCORD_ISA_ARGDATA_H
#define DISCORD_ISA_ARGDATA_H


#include <string>
#include <optional>
#include <memory>
#include <vector>
#include <stdexcept>

/**
 * Data class for program arguments
 */
class ArgData {
public:
    ArgData(int argc, char *argv[]);

    bool print_help = false;
    bool verbose_output = false;
    std::shared_ptr<std::string> token;
};


#endif //DISCORD_ISA_ARGDATA_H
