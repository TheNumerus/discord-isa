//
// Created by pedro on 11.10.20.
//

#include "DiscordClient.h"

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
            this->channelId = channel.obj()["id"].string();
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
    this->guildId = guild["id"].string();
    return &this->guildId;
}

void DiscordClient::send_message(const std::string& message) {
    std::string path = "/api/v8/channels/" + this->channelId + "/messages";

    auto res_message = httpClient.post(path, message);

    if (res_message.head.code != 200) {
        throw std::runtime_error("Error while sending message");
    }
}

JsonArray DiscordClient::get_messages() {
    auto path = "/api/v8/channels/" + this->channelId + "/messages";
    auto res = httpClient.get(path);
    std::string_view  sw(res.body);
    auto json = Json::parse_array(sw);
    return std::get<JsonArray>(json);
}

JsonArray DiscordClient::get_messages_after(const std::string& id) {
    auto path = "/api/v8/channels/" + this->channelId + "/messages?after=" + id;
    auto res = httpClient.get(path);
    std::string_view  sw(res.body);
    auto json = Json::parse_array(sw);
    return std::get<JsonArray>(json);
}

void DiscordClient::loop() {
    std::string newest_id;
    try {
        while(true) {
            JsonArray messages;
            if (newest_id.empty()) {
                messages = this->get_messages();
            } else {
                messages = this->get_messages_after(newest_id);
            }

            if (!messages.empty()) {
                for (auto ri = messages.rbegin(); ri != messages.rend(); ++ri) {
                    auto message = *ri;
                    auto obj = message.obj();
                    auto content = obj["content"].string();
                    auto user = obj["author"].obj()["username"].string();
                    auto id = obj["id"].string();

                    // TODO not respond to self

                    if (content.find("bot") != content.npos && !newest_id.empty() && user.find("bot") == user.npos) {
                        if (this->verbose) {
                            std::cout << "#isa-bot - " << user << ": " << content << std::endl;
                        }

                        std::string echo = R"({"content": "echo: )" + user + " - " + content + R"("})";
                        this->send_message(echo);
                    }
                }
                newest_id = messages[0].obj()["id"].string();
            }

            sleep(1);
        }
    } catch (const std::exception& e) {
        // TODO
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
