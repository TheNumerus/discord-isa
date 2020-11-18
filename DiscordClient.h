//
// Created by pedro on 11.10.20.
//

#ifndef DISCORD_ISA_DISCORDCLIENT_H
#define DISCORD_ISA_DISCORDCLIENT_H

#include "HttpClient.h"
#include "ArgData.h"
#include "Json.h"
#include <unistd.h>

/**
 * Class for communicating with Discord API
 */
class DiscordClient {
public:
    DiscordClient(const ArgData &args);

    const std::string* find_guildId();
    const std::string* find_channelId();
    void loop();

    void send_message(const std::string& message);
    JsonArray get_messages();
    JsonArray get_messages_after(const std::string &id);
    std::string guildId;
    std::string channelId;

private:
    HttpClient httpClient;
    bool verbose;
};


#endif //DISCORD_ISA_DISCORDCLIENT_H
