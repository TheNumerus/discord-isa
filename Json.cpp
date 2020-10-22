//
// Created by pedro on 08.10.20.
//

#include <sstream>
#include "Json.h"

JsonValueType Json::parse_string(std::string_view& i) {
    if (i.find('"') != 0) {
        throw std::runtime_error("JSON parse error, expected start of string literal");
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

JsonValueType Json::parse_array(std::string_view& i) {
    if (i[0] != '[') {
        std::ostringstream message;
        message << "JSON parse error, expected '[', found '" << i[0] << "'";
        throw std::runtime_error(message.str());
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
                    throw std::runtime_error("JSON parse error, unexpected input");
                } else {
                    i = i.substr(1);
                }
            }

            auto value = parse_value(i);
            v.push_back(value);
        }
    }
}

JsonValueType Json::parse_object(std::string_view& i) {
    if (i[0] != '{') {
        std::ostringstream message;
        message << "JSON parse error, expected '{', found '" << i[0] << "'";
        throw std::runtime_error(message.str());
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
                    throw std::runtime_error("JSON parse error, unexpected input");
                } else {
                    i = i.substr(1);
                }
            }

            auto pair = parse_pair(i);
            o.insert(pair);
        }
    }
}

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
        throw std::runtime_error("JSON parse error, number parsing error");
    }

    std::string num(i.substr(0, count));

    try {
        auto i64 = std::stol(num);
        i = i.substr(count);
        return i64;
    } catch (...) {
        auto f64 = std::stod(num);
        i = i.substr(count);
        return f64;
    }
}

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

std::pair<std::string, JsonValue> Json::parse_pair(std::string_view& i) {
    parse_whitespace(i);

    auto key_gen = parse_string(i);
    auto key = std::get<std::string>(key_gen);
    parse_whitespace(i);
    if (i[0] != ':') {
        throw std::runtime_error("JSON parse error, expected ':'");
    }
    i = i.substr(1);
    auto value = parse_value(i);
    return std::make_pair(key, value);
}

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
        } catch(...) {
            continue;
        }
    }

    throw std::runtime_error("JSON parse error, could not find value parser that works");
}

JsonValueType Json::parse_bool(std::string_view& i) {
    if (i.find("true") == 0) {
        i = i.substr(4);
        return true;
    } else if (i.find("false") == 0) {
        i = i.substr(5);
        return false;
    }
    throw std::runtime_error("JSON parse error, invalid bool format");
}

JsonValueType Json::parse_null(std::string_view& i) {
    if (i.find("null") == 0) {
        i = i.substr(4);
        return nullptr;
    }
    throw std::runtime_error("JSON parse error, invalid null format");
}

std::string JsonValue::string() {
    return std::get<std::string>(value);
}

std::vector<JsonValue> JsonValue::arr() {
    return std::get<std::vector<JsonValue>>(value);
}

JsonObject JsonValue::obj() {
    return std::get<JsonObject>(value);
}
