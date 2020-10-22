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

int DiscordClient::run() {
    pthread_t pthread;

    pthread_create(&pthread, nullptr, loop, this);

    void* thread_ret;

    pthread_join(pthread, &thread_ret);

    int ret = *(int*)thread_ret;

    delete (int*)thread_ret;

    return ret;
}

void *loop(void* args) {
    // I really hate this
    auto dc = (DiscordClient*)(args);

    try {
        auto messages = dc->get_messages();

        for (auto message: messages) {
            auto obj = message.obj();
            auto content = obj["content"].string();
            auto user = obj["author"].obj()["username"].string();
            auto id = obj["id"].string();

            std::cout << "message #" << id << " from \"" << user << "\": " << content << std::endl;
        }

        //dc->send_message(R"({"content": ")" + messages[0].obj()["content"].string() + R"("})");
    } catch (const std::exception& e) {
        std::cerr << "Error in child thread: " << e.what() << std::endl;
        return new int(-1);
    }

    return new int(0);
}
