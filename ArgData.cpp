//
// Created by pedro on 29.09.20.
//

#include "ArgData.h"


ArgData::ArgData(int argc, char **argv) {
    // copy args to vector
    std::vector<std::string> args;
    if (argc > 1) {
        args.assign(argv + 1, argv + argc);
    } else {
        this->print_help = true;
        return;
    }

    bool token_set = false;

    for(auto it = args.begin(); it != args.end(); it++) {
        auto s = *it;
        if(s == "-h" || s == "--help") {
            this->print_help = true;
        } else if (s == "-v" || s == "--verbose") {
            this->verbose_output = true;
        } else if (s == "-t") {
            it++;
            // someone forgot the token
            if (it == args.end()) {
                throw std::runtime_error("token not specified");
            }
            this->token = std::make_shared<std::string>(*it);
            token_set = true;
        } else {
            throw std::runtime_error("invalid parameter");
        }
    }

    if (!token_set) {
        throw std::runtime_error("token not specified");
    }
}
