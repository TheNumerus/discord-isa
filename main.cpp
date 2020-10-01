#include <iostream>
#include "ArgData.h"
#include "HTTPClient.h"

void print_help();

int main(int argc, char *argv[]) {
    // parse args
    auto args = ArgData::parse(argc, argv);
    if (!args.has_value()) {
        std::cerr << "Invalid combination of arguments." << std::endl;
        return -1;
    }

    if (args->print_help) {
        print_help();
        return 0;
    }

    HTTPClient httpClient(args->token);
    httpClient.get("/api/users/@me");
    std::cout << "test\n";
    httpClient.get("/api/users/@me");

    return 0;
}

void print_help() {
    std::cout << "isabot - Discord bot for repeating messages\n\n";
    std::cout << "Usage: isabot [PARAMETERS]\n\n";
    std::cout << "Mandatory parameters:\n";
    std::cout << "    -t [TOKEN]       auth token for the bot\n\n";
    std::cout << "Optional parameters:\n";
    std::cout << "    -h, --help       displays help then exits\n";
    std::cout << "    -v, --verbose    prints messages the bot reacted to" << std::endl;
}
