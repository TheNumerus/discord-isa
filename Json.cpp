//
// Created by pedro on 08.10.20.
//

#include "Json.h"

// this may be the uglies code I have ever written


std::pair<std::string, std::string> Json::parse_string(std::string i) {
    if (i.find('"') != 0) {
        throw std::runtime_error("parse error");
    }
    auto i2 = i.substr(1);
    std::string s;
    int pos = 0;
    while(1) {
        if (i2[pos] == '"') {
            pos ++;
            return std::make_pair(i2.substr(pos), s);
        } else if (i2[pos] != '\\') {
            s.append(1, i2[pos]);
        } else {
            switch (i2[pos + 1]) {
                case '\\':
                    s.append(1, '\\');
                    pos++;
                    break;
                case '/':
                    s.append(1, '/');
                    pos++;
                case '"':
                    s.append(1, '"');
                    pos++;
                    break;
                case 'b':
                    s.append(1, '\b');
                    pos++;
                    break;
                case 'f':
                    s.append(1, '\f');
                    pos++;
                    break;
                case 'n':
                    s.append(1, '\n');
                    pos++;
                    break;
                case 'r':
                    s.append(1, '\r');
                    pos++;
                    break;
                case 't':
                    s.append(1, '\t');
                    pos++;
                    break;
                case 'u':

                default:
                    // there might be \u0647 like sequences, ignore them
                    pos += 5;
            }
        }
        pos++;
    }
}

std::pair<std::string, std::vector<JsonValue>> Json::parse_array(std::string i) {
    std::vector<JsonValue> v;

    if (i[0] != '[') {
        throw std::runtime_error("parse error");
    }

    i = i.substr(1);
    i = parse_whitespace(i);

    auto first = true;

    while (true) {
        if (i[0] == ']') {
            i = i.substr(1);
            return std::make_pair(i, v);
        } else {
            if (first) {
                first = false;
            } else {
                if (i[0] != ',') {
                    throw std::runtime_error("parse error");
                } else {
                    i = i.substr(1);
                }
            }

            auto [i2, value] = parse_value(i);
            v.push_back(value);
            i = i2;
        }
    }
}

std::pair<std::string, JsonObject> Json::parse_object(std::string i) {
    JsonObject o;

    if (i[0] != '{') {
        throw std::runtime_error("parse error");
    }

    auto i2 = i.substr(1);
    auto i3 = parse_whitespace(i2);

    auto first = true;

    while (true) {
        if (i3[0] == '}') {
            i3 = i3.substr(1);
            return std::make_pair(i3, o);
        } else {
            if (first) {
                first = false;
            } else {
                if (i3[0] != ',') {
                    throw std::runtime_error("parse error");
                } else {
                    i3 = i3.substr(1);
                }
            }

            auto [i4, pair] = parse_pair(i3);
            o.insert(pair);
            i3 = i4;
        }
    }
}

std::pair<std::string, JsonNumber> Json::parse_number(std::string i) {
    int count = 0;
    while(true) {
        if ((i[count] >= '0' && i[count] <= '9') || i[count] == 'e' || i[count] == 'E' || i[count] == '.' || i[count] == '-') {
            count++;
        } else {
            break;
        }
    }

    if (count == 0) {
        throw std::runtime_error("parse error");
    }

    auto num = i.substr(0, count);

    try {
        auto i64 = std::stol(num);
        return std::make_pair(i.substr(count), i64);
    } catch (...) {
        auto f64 = std::stod(num);
        return std::make_pair(i.substr(count), f64);
    }
}

std::string Json::parse_whitespace(std::string i) {
    int count = 0;
    while(true) {
        if (i[count] == '\t' || i[count] == '\r' || i[count] == '\n' || i[count] == ' ') {
            count++;
        } else {
            return i.substr(count);
        }
    }
}

std::pair<std::string, std::pair<std::string, JsonValue>> Json::parse_pair(std::string i) {
    i = parse_whitespace(i);

    auto [i2, key] = parse_string(i);
    i2 = parse_whitespace(i2);
    if (i2[0] != ':') {
        throw std::runtime_error("parse error");
    }
    i2 = i2.substr(1);
    auto [i3, value] = parse_value(i2);
    return std::make_pair(i3, std::make_pair(key, value));
}

std::pair<std::string, JsonValue> Json::parse_value(std::string i) {
    i = parse_whitespace(i);

    JsonValue val;
    try{
        auto [i2, s] = parse_string(i);
        i = i2;
        val.value = s;
    } catch(...) {
        try {
            auto [i2, s] = parse_number(i);
            i = i2;
            val.value = s;
        } catch(...) {
            try {
                auto [i2, s] = parse_object(i);
                i = i2;
                val.value = s;
            } catch(...) {
                try {
                    auto [i2, s] = parse_array(i);
                    i = i2;
                    val.value = s;
                } catch(...) {
                    try {
                        auto [i2, s] = parse_bool(i);
                        i = i2;
                        val.value = s;
                    } catch(...) {
                        try {
                            auto [i2, s] = parse_null(i);
                            i = i2;
                            val.value = s;
                        } catch(...) {
                            throw std::runtime_error("parse error");
                        }
                    }
                }
            }
        }
    }
    i = parse_whitespace(i);

    return std::make_pair(i, val);
}

std::pair<std::string, bool> Json::parse_bool(std::string i) {
    if (i.find("true") == 0) {
        return std::make_pair(i.substr(4), true);
    } else if (i.find("false") == 0) {
        return std::make_pair(i.substr(5), false);
    }
    throw std::runtime_error("parse error");
}

std::pair<std::string, nullptr_t> Json::parse_null(std::string i) {
    if (i.find("null") == 0) {
        return std::make_pair(i.substr(4), nullptr);
    }
    throw std::runtime_error("parse error");
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
