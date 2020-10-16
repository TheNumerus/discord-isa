//
// Created by pedro on 06.10.20.
//

#include "HttpHead.h"

void find_and_consume(std::string_view& i, const std::string& n);
int res_code(std::string_view& i);
std::optional<std::pair<std::string, std::string>> parse_header(std::string_view& i);

std::pair<std::string_view, HttpHead> HttpHead::parse(std::string_view i) {
    HttpHead hh(200);

    find_and_consume(i, "HTTP/1.1 ");

    hh.code = res_code(i);

    while (auto header = parse_header(i)) {
        hh.headers.insert(*header);
    }

    // remove head and body separator
    i = i.substr(2, i.size() - 2);

    return std::make_pair(i, hh);
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


void find_and_consume(std::string_view& i, const std::string& n) {
    if(i.substr(0, n.size()) == n) {
        i = i.substr(n.size(), i.size() - n.size());
        return;
    }
    throw std::runtime_error("parse error, could not found " + n);
}

int res_code(std::string_view& i) {
    // find end of line after http status code
    size_t found = i.find("\r\n");
    if (found == i.npos) {
        throw std::runtime_error("parse error, invalid HTTP");
    }

    // TODO handle some others
    auto sub = i.substr(0, found);
    if (sub == "200 OK") {
        i = i.substr(8);
        return 200;
    } else if (sub == "400 Bad Request") {
        i= i.substr(17);
        return 400;
    } else if (sub == "401 Unauthorized") {
        i = i.substr(18);
        return 401;
    } else if (sub == "404 Not Found") {
        i = i.substr(15);
        return 404;
    } else if (sub == "411 Length Required") {
        i = i.substr(21);
        return 411;
    } else {
        throw std::runtime_error("Unknown or not implemented HTTP response code");
    }
}

std::optional<std::pair<std::string, std::string>> parse_header(std::string_view& i) {
    // no header left
    if (i[0] == '\r') {
        return {};
    }
    int colon = i.find(':');
    auto key = std::string(i.substr(0, colon));
    auto value_and_rest = i.substr(colon + 2);
    int newline = value_and_rest.find("\r\n");
    auto value = std::string(value_and_rest.substr(0, newline));
    i = value_and_rest.substr(value.size() + 2);
    return std::make_pair(key, value);
}
