//
// Created by pedro on 11.10.20.
//

#include "DiscordClient.h"

DiscordClient::DiscordClient(const ArgData &args)
        : httpClient(args.token), verbose(args.verbose_output) {}

/**
 * Will try to find '#isa-bot' channel in active guild
 * @return channel ID, if any was found
 */
const std::string *DiscordClient::find_channelId() {
    if (this->guildId.empty()) {
        return nullptr;
    }
    std::string ch_message = "/api/v8/guilds/" + this->guildId + "/channels";

    auto res_channels = httpClient.get(ch_message);

    if (res_channels.head.code == 401) {
        throw std::runtime_error("HTTP 401 Unauthorized, might be invalid token");
    }

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

/**
 * Will try to find a guild ID
 * @return Guild ID, if any was found, else `nullptr`
 */
const std::string *DiscordClient::find_guildId() {
    auto res = httpClient.get("/api/v8/users/@me/guilds");
    if (res.head.code == 401) {
        throw std::runtime_error("HTTP 401 Unauthorized, might be invalid token");
    }
    std::string_view sw(res.body);
    auto json = Json::parse_value(sw);
    if (json.arr().empty()) {
        return nullptr;
    }
    auto guild = json.arr()[0].obj();
    this->guildId = guild["id"].string();
    return &this->guildId;
}

/**
 * Sends a message to '#isa-bot' channel, if any was found previously
 * @param message - JSON message which will be sent
 */
void DiscordClient::send_message(const std::string& message) {
    if (this->channelId.empty()) {
        return;
    }
    std::string path = "/api/v8/channels/" + this->channelId + "/messages";

    auto res_message = httpClient.post(path, message);

    if (res_message.head.code != 200) {
        throw std::runtime_error("Error while sending message");
    }
}

/**
 * Returns recent messages from active channel
 * @return messages
 */
JsonArray DiscordClient::get_messages() {
    auto path = "/api/v8/channels/" + this->channelId + "/messages";
    auto res = httpClient.get(path);
    std::string_view  sw(res.body);
    auto json = Json::parse_array(sw);
    return std::get<JsonArray>(json);
}

/**
 * Returns messages sent after given message id
 * @param id - message id
 * @return messages
 */
JsonArray DiscordClient::get_messages_after(const std::string& id) {
    auto path = "/api/v8/channels/" + this->channelId + "/messages?after=" + id;
    auto res = httpClient.get(path);
    std::string_view  sw(res.body);
    auto json = Json::parse_array(sw);
    return std::get<JsonArray>(json);
}

/**
 * Main bot function
 *
 * Will loop and respond to messages
 */
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

                    if (!newest_id.empty() && user.find("bot") == user.npos) {
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
