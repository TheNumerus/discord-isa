#include <iostream>
#include <vector>
#include "ArgData.h"
#include "DiscordClient.h"

void print_help();

int main(int argc, char *argv[]) {
    SSL_library_init();
    try {
        ArgData args(argc, argv);

        if (args.print_help) {
            print_help();
            return 0;
        }

        DiscordClient discordClient(args);
        // init bot
        auto guild = discordClient.find_guildId();
        if (guild == nullptr) {
            std::cerr << "Bot not in any guild, shutting down." << std::endl;
            CRYPTO_cleanup_all_ex_data();
            exit(-1);
        }

        auto channel = discordClient.find_channelId();
        if (channel == nullptr) {
            std::cerr << "Did not found any channel with name '#isa-bot', shutting down." << std::endl;
            CRYPTO_cleanup_all_ex_data();
            exit(-1);
        }

        // run loop
        discordClient.loop();
        CRYPTO_cleanup_all_ex_data();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        CRYPTO_cleanup_all_ex_data();
        return -1;
    }
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
