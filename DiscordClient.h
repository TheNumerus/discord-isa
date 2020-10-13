//
// Created by pedro on 11.10.20.
//

#ifndef DISCORD_ISA_DISCORDCLIENT_H
#define DISCORD_ISA_DISCORDCLIENT_H

#include "HttpClient.h"
#include "ArgData.h"
#include <pthread.h>

class DiscordClient {
public:
    DiscordClient(const ArgData &args);

    const std::string* find_guildId();
    const std::string* find_channelId();
    void run();

    void send_message(std::string message);
    void get_messages();
    std::string guildId;
    std::string channelId;

private:
    HttpClient httpClient;
    bool verbose;
};


#endif //DISCORD_ISA_DISCORDCLIENT_H
