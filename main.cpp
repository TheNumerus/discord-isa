#include <iostream>
#include "ArgData.h"
#include "HttpClient.h"

void print_help();

int main(int argc, char *argv[]) {
    SSL_library_init();
    try {
        ArgData args(argc, argv);

        if (args.print_help) {
            print_help();
            return 0;
        }

        HttpClient httpClient(args.token);

        httpClient.get("/api/v8/users/@me");
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        CRYPTO_cleanup_all_ex_data();
        return -1;
    }
    CRYPTO_cleanup_all_ex_data();
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
