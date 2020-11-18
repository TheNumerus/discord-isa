//
// Created by pedro on 08.10.20.
//

#include <sstream>
#include "Json.h"

/**
 * Parser for strings
 *
 * Does not escape or unescape
 * @param i - input
 * @return parsed string
 */
JsonValueType Json::parse_string(std::string_view& i) {
    if (i.find('"') != 0) {
        throw JsonException(JsonError::WrongValue);
    }
    i = i.substr(1);
    std::string s;
    int pos = 0;
    while (true) {
        if (i[pos] == '"' && i[pos - 1] != '\\') {
            pos ++;
            i = i.substr(pos);
            return s;
        } else {
            s.append(1, i[pos]);
        }
        pos++;
    }
}

/**
 * Parser for arrays
 * @param i - input
 * @return parsed array
 */
JsonValueType Json::parse_array(std::string_view& i) {
    if (i[0] != '[') {
        throw JsonException(JsonError::WrongValue);
    }

    i = i.substr(1);
    parse_whitespace(i);

    auto first = true;

    JsonArray v;

    while (true) {
        if (i[0] == ']') {
            i = i.substr(1);
            return v;
        } else {
            if (first) {
                first = false;
            } else {
                if (i[0] != ',') {
                    throw JsonException(JsonError::BrokenInput);
                } else {
                    i = i.substr(1);
                }
            }

            auto value = parse_value(i);
            v.push_back(value);
        }
    }
}

/**
 * Parser for objects
 * @param i - input
 * @return parsed object
 */
JsonValueType Json::parse_object(std::string_view& i) {
    if (i[0] != '{') {
        throw JsonException(JsonError::WrongValue);
    }

    i = i.substr(1);
    parse_whitespace(i);

    auto first = true;

    JsonObject o;

    while (true) {
        if (i[0] == '}') {
            i = i.substr(1);
            return o;
        } else {
            if (first) {
                first = false;
            } else {
                if (i[0] != ',') {
                    throw JsonException(JsonError::BrokenInput);
                } else {
                    i = i.substr(1);
                }
            }

            auto pair = parse_pair(i);
            o.insert(pair);
        }
    }
}

/**
 * Parser for numbers
 * @param i - input
 * @return parsed number
 */
JsonValueType Json::parse_number(std::string_view& i) {
    int count = 0;
    while(true) {
        if ((i[count] >= '0' && i[count] <= '9') || i[count] == 'e' || i[count] == 'E' || i[count] == '.' || i[count] == '-') {
            count++;
        } else {
            break;
        }
    }

    if (count == 0) {
        throw JsonException(JsonError::WrongValue);
    }

    std::string num(i.substr(0, count));

    try {
        auto i64 = std::stol(num);
        i = i.substr(count);
        return i64;
    } catch (...) {
        try {
            auto f64 = std::stod(num);
            i = i.substr(count);
            return f64;
        } catch (...) {
            throw JsonException(JsonError::BrokenInput);
        }
    }
}

/**
 * Parser for white space
 * @param i - input
 */
void Json::parse_whitespace(std::string_view& i) {
    int count = 0;
    while(true) {
        if (i[count] == '\t' || i[count] == '\r' || i[count] == '\n' || i[count] == ' ') {
            count++;
        } else {
            i = i.substr(count);
            return;
        }
    }
}

/**
 * Parser for key: value pairs
 * @param i - input
 * @return parsed pair
 */
std::pair<std::string, JsonValue> Json::parse_pair(std::string_view& i) {
    parse_whitespace(i);

    auto key_gen = parse_string(i);
    auto key = std::get<std::string>(key_gen);
    parse_whitespace(i);
    if (i[0] != ':') {
        throw JsonException(JsonError::BrokenInput);
    }
    i = i.substr(1);
    auto value = parse_value(i);
    return std::make_pair(key, value);
}

/**
 * Parser for any JSON value
 *
 * Will try to use any of the other specific type parsers until one succeeds
 * @param i - input
 * @return parsed value
 */
JsonValue Json::parse_value(std::string_view& i) {
    parse_whitespace(i);

    JsonValue val;

    // I'm weirdly proud of this monstrosity
    JsonValueType(*func[6])(std::string_view&) = {parse_string, parse_number, parse_object, parse_array, parse_bool, parse_null};

    for(auto fn: func) {
        try{
            val.value = fn(i);
            parse_whitespace(i);
            return val;
        } catch(const JsonException& e) {
            if (e.err_type() == JsonError::WrongValue) {
                continue;
            } else {
                throw JsonException(JsonError::BrokenInput);
            }
        }
    }

    throw JsonException(JsonError::BrokenInput);
}

/**
 * Parser for bool values
 * @param i - input
 * @return parsed bool
 */
JsonValueType Json::parse_bool(std::string_view& i) {
    if (i.find("true") == 0) {
        i = i.substr(4);
        return true;
    } else if (i.find("false") == 0) {
        i = i.substr(5);
        return false;
    }
    throw JsonException(JsonError::WrongValue);
}

/**
 * Parser for null values
 * @param i - input
 * @return parsed null
 */
JsonValueType Json::parse_null(std::string_view& i) {
    if (i.find("null") == 0) {
        i = i.substr(4);
        return JsonNull();
    }
    throw JsonException(JsonError::WrongValue);
}

/**
 * String access function
 * @return string
 */
std::string JsonValue::string() {
    return std::get<std::string>(value);
}

/**
 * Array access function
 * @return array
 */
std::vector<JsonValue> JsonValue::arr() {
    return std::get<std::vector<JsonValue>>(value);
}

/**
 * Object access function
 * @return object
 */
JsonObject JsonValue::obj() {
    return std::get<JsonObject>(value);
}

/**
 * Bool access function
 * @return bool
 */
bool JsonValue::boolean() {
    return std::get<bool>(value);
}

/**
 * Checks if value is null
 * @return is value null?
 */
bool JsonValue::is_null() {
    return std::holds_alternative<JsonNull>(value);
}

/**
 * Number access function
 * @return number
 */
JsonNumber JsonValue::number() {
    return std::get<JsonNumber>(value);
}
