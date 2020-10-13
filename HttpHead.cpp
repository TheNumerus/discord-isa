//
// Created by pedro on 06.10.20.
//

#include "HttpHead.h"

std::pair<std::string, std::string> tag(std::string i, std::string n);
std::pair<std::string, int> res_code(std::string i);
std::pair<std::string, std::pair<std::string, std::string>> parse_header(std::string i);

std::pair<std::string, HttpHead> HttpHead::parse(std::string i) {
    HttpHead hh(200);

    auto [i2, _] = tag(i, "HTTP/1.1 ");
    auto [i3, code] = res_code(i2);
    hh.code = code;

    // TODO total cleanup, this is ugly

    do {
        try {
            auto [i4, h] = parse_header(i3);
            i3 = i4;
            hh.headers.insert(h);
        } catch (...) {
            break;
        }
    } while (true);

    i3 = i3.substr(2, i3.size() - 2);

    return std::make_pair(i3, hh);
}

std::ostream &operator<<(std::ostream &os, const HttpHead &header) {
    os << "HttpHead: {\n    code: " << header.code << ",\n    headers: {\n";
    for (auto [key, value]: header.headers) {
        os << "        \"" << key << "\": \"" << value << "\",\n";
    }
    os << "    }\n}\n";
    return os;
}

HttpHead::HttpHead(int code) : code(code) {}

std::pair<std::string, std::string> tag(std::string i, std::string n) {
    if(i.substr(0, n.size()) == n) {
        return std::make_pair(i.substr(n.size(), i.size() - n.size()), n);
    }
    throw std::runtime_error("parse error");
}

std::pair<std::string, int> res_code(std::string i) {
    int found = i.find("\r\n");
    if (found == i.npos) {
        throw std::runtime_error("parse error");
    }

    // TODO handle some others
    auto sub = i.substr(0, found);
    if (sub == "200 OK") {
        auto rest = i.substr(8);
        return std::make_pair(rest, 200);
    } else if (sub == "400 Bad Request") {
        auto rest = i.substr(17);
        return std::make_pair(rest, 400);
    } else if (sub == "401 Unauthorized") {
        auto rest = i.substr(18);
        return std::make_pair(rest, 401);
    } else if (sub == "404 Not Found") {
        auto rest = i.substr(15);
        return std::make_pair(rest, 404);
    } else if (sub == "411 Length Required") {
        auto rest = i.substr(21);
        return std::make_pair(rest, 411);
    } else {
        throw std::runtime_error("Unknown or not implemented HTTP response code");
    }
}

std::pair<std::string, std::pair<std::string, std::string>> parse_header(std::string i) {
    if (i[0] == '\r') {
        throw std::runtime_error("header not found");
    }
    int colon = i.find(":");
    auto key = i.substr(0, colon);
    auto value_and_rest = i.substr(colon + 2);
    int newline = value_and_rest.find("\r\n");
    auto value = value_and_rest.substr(0, newline);
    auto rest = value_and_rest.substr(value.size() + 2);
    return std::make_pair(rest, std::make_pair(key, value));
}
