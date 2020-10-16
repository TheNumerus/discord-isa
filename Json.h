//
// Created by pedro on 08.10.20.
//

#ifndef DISCORD_ISA_JSON_H
#define DISCORD_ISA_JSON_H

#include <variant>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <string_view>

using JsonNumber = std::variant<long, double>;

class JsonValue;

using JsonObject = std::map<std::string, JsonValue>;

using JsonValueType = std::variant<JsonNumber, bool, std::string, std::vector<JsonValue>, nullptr_t, JsonObject>;

class JsonValue {
public:
    JsonValueType value;
    std::string string();
    std::vector<JsonValue> arr();
    JsonObject obj();
};

class Json {
public:
    static JsonValueType parse_object(std::string_view& i);
    static JsonValueType parse_array(std::string_view& i);
    static JsonValueType parse_number(std::string_view& i);
    static JsonValueType parse_null(std::string_view& i);
    static JsonValueType parse_string(std::string_view& i);
    static JsonValueType parse_bool(std::string_view& i);
    static JsonValue parse_value(std::string_view& i);
    static void parse_whitespace(std::string_view& i);
    static std::pair<std::string, JsonValue> parse_pair(std::string_view& i);
};

#endif //DISCORD_ISA_JSON_H
