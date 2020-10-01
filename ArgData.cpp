//
// Created by pedro on 29.09.20.
//

#include <vector>
#include "ArgData.h"

ArgData::ArgData() {
    this->verbose_output = false;
    this->print_help = false;
}

std::optional<ArgData> ArgData::parse(int argc, char **argv) {
    ArgData data;
    // copy args to vector
    std::vector<std::string> args;
    if (argc > 1) {
        args.assign(argv + 1, argv + argc);
    }

    for(auto it = args.begin(); it != args.end(); it++) {
        auto s = *it;
        if(s == "-h" || s == "--help") {
            data.print_help = true;
            return data;
        } else if (s == "-v" || s == "--verbose") {
            data.verbose_output = true;
        } else if (s == "-t") {
            it++;
            // someone forgot the token
            if (it == args.end()) {
                return {};
            }
            data.token = std::make_shared<std::string>(*it);
        } else {
            return {};
        }
    }
    return data;
}
