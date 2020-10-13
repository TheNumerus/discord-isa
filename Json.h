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

// I really hate how easy is this in Rust but impossible in C++

using JsonNumber = std::variant<long, double>;

class JsonValue;

using JsonObject = std::map<std::string, JsonValue>;

using JsonValueType = std::variant<JsonNumber, bool, std::string, std::vector<JsonValue>, nullptr_t, JsonObject>;

// this **** should not be needed, but ******* C++ is C++
class JsonValue {
public:
    JsonValueType value;
    std::string string();
    std::vector<JsonValue> arr();
    JsonObject obj();
};

class Json {
public:
    static std::pair<std::string, JsonObject> parse_object(std::string);
    static std::pair<std::string, std::vector<JsonValue>> parse_array(std::string);
    static std::string parse_whitespace(std::string);
    static std::pair<std::string, JsonNumber> parse_number(std::string);
    static std::pair<std::string, nullptr_t> parse_null(std::string);
    static std::pair<std::string, std::string> parse_string(std::string);
    static std::pair<std::string, bool> parse_bool(std::string);
    static std::pair<std::string, JsonValue> parse_value(std::string);
    static std::pair<std::string, std::pair<std::string, JsonValue>> parse_pair(std::string);
};

#endif //DISCORD_ISA_JSON_H
