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
#include <cstddef>
#include "JsonException.h"

using JsonNumber = std::variant<long, double>;

class JsonValue;

using JsonArray = std::vector<JsonValue>;

using JsonObject = std::map<std::string, JsonValue>;

// eva does not like nullptr_t for some reason
class JsonNull {
};

using JsonValueType = std::variant<JsonNumber, bool, std::string, JsonArray, JsonNull, JsonObject>;

/**
 * Main JSON data class
 *
 * Can hold any valid type of value, getter methods will throw exception if user tries to access data type that is not stored
 */
class JsonValue {
public:
    JsonValueType value;
    std::string string();
    std::vector<JsonValue> arr();
    JsonObject obj();
    bool boolean();
    bool is_null();
    JsonNumber number();
};

/**
 * Helper class for parsing functions
 *
 * All parsers will modify string view if value was successfully parsed
 * May throw JSOMException if parser cannot parse value, or input is mangled
 */
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
