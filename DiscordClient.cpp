//
// Created by pedro on 11.10.20.
//

#include "DiscordClient.h"
#include "Json.h"

void *loop(void* args);

DiscordClient::DiscordClient(const ArgData &args)
        : httpClient(args.token), verbose(args.verbose_output) {}

const std::string *DiscordClient::find_channelId() {
    std::string ch_message = "/api/v8/guilds/" + this->guildId + "/channels";

    auto res_channels = httpClient.get(ch_message);
    std::string_view body_sw(res_channels.body);
    auto json_channels = Json::parse_value(body_sw);

    std::string bot_channel;
    for (auto channel: json_channels.arr()) {
        if (channel.obj()["name"].string() == "isa-bot") {
            this->channelId = std::move(channel.obj()["id"].string());
            return &this->channelId;
        }
    }
    return nullptr;
}

const std::string *DiscordClient::find_guildId() {
    auto res = httpClient.get("/api/v8/users/@me/guilds");
    std::string_view sw(res.body);
    auto json = Json::parse_value(sw);
    if (json.arr().empty()) {
        return nullptr;
    }
    auto guild = json.arr()[0].obj();
    this->guildId = std::move(guild["id"].string());
    return &this->guildId;
}

void DiscordClient::send_message(std::string message) {
    std::string path = "/api/v8/channels/" + this->channelId + "/messages";

    auto res_message = httpClient.post(path, message);

    if (res_message.head.code != 200) {
        throw std::runtime_error("Error while sending message");
    }
}

void DiscordClient::get_messages() {
    auto path = "/api/v8/channels/" + this->channelId + "/messages";
    auto res = httpClient.get(path);
    std::cout << res;

}

void DiscordClient::run() {
    pthread_t pthread;

    pthread_create(&pthread, nullptr, loop, this);

    pthread_join(pthread, nullptr);
}

void *loop(void* args) {
    // I really hate this
    auto dc = (DiscordClient*)(args);

    dc->get_messages();

    std::cout << dc->channelId << std::endl;
    return nullptr;
}
