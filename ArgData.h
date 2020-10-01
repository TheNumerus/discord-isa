//
// Created by pedro on 29.09.20.
//

#ifndef DISCORD_ISA_ARGDATA_H
#define DISCORD_ISA_ARGDATA_H


#include <string>
#include <optional>
#include <memory>

class ArgData {
public:
    ArgData();

    bool print_help;
    bool verbose_output;
    std::shared_ptr<std::string> token;

    static std::optional<ArgData> parse(int argc, char *argv[]);
};


#endif //DISCORD_ISA_ARGDATA_H
