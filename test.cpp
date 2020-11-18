//
// Created by pedro on 17.11.20.
//
#include <iostream>
#include "Json.h"
#include "HttpHead.h"

#define GREEN "\033[32m"
#define RESET "\033[0m"
#define RED "\033[31m"

// constants for testing

// http1.1 www.wikipedia.org
const char* HEADER =
"HTTP/1.1 200 OK\r\n"
"Date: Tue, 17 Nov 2020 02:42:44 GMT\r\n"
"Cache-Control: s-maxage=86400, must-revalidate, max-age=3600\r\n"
"Server: ATS/8.0.8\r\n"
"ETag: W/\"10d12-5b31e183d96b7\"\r\n"
"Last-Modified: Mon, 02 Nov 2020 11:32:00 GMT\r\n"
"Content-Type: text/html\r\n"
"Vary: Accept-Encoding\r\n"
"Age: 26145\r\n"
"X-Cache: cp3062 miss, cp3060 hit/349878\r\n"
"X-Cache-Status: hit-front\r\n"
"Server-Timing: cache;desc=\"hit-front\"\r\n"
"Strict-Transport-Security: max-age=106384710; includeSubDomains; preload\r\n"
"Report-To: { \"group\": \"wm_nel\", \"max_age\": 86400, \"endpoints\": [{ \"url\": \"https://intake-logging.wikimedia.org/v1/events?stream=w3c.reportingapi.network_error&schema_uri=/w3c/reportingapi/network_error/1.0.0\" }] }\r\n"
"NEL: { \"report_to\": \"wm_nel\", \"max_age\": 86400, \"failure_fraction\": 0.05, \"success_fraction\": 0.0}\r\n"
"Set-Cookie: WMF-Last-Access=17-Nov-2020;Path=/;HttpOnly;secure;Expires=Sat, 19 Dec 2020 00:00:00 GMT\r\n"
"Set-Cookie: WMF-Last-Access-Global=17-Nov-2020;Path=/;Domain=.wikipedia.org;HttpOnly;secure;Expires=Sat, 19 Dec 2020 00:00:00 GMT\r\n"
"X-Client-IP: 89.24.42.169\r\n"
"Set-Cookie: GeoIP=CZ:10:Prague:50.08:14.45:v4; Path=/; secure; Domain=.wikipedia.org\r\n"
"Accept-Ranges: bytes\r\n"
"Content-Length: 68882\r\n"
"Connection: keep-alive\r\n\r\n<body here>\r\n";

// vscode settings
const char* JSON = R"({
    "workbench.colorTheme": "Dracula",
    "editor.fontFamily": "'Iosevka', 'Droid Sans Mono', 'monospace', monospace, 'Droid Sans Fallback'",
    "editor.fontLigatures": true,
    "editor.semanticHighlighting.enabled": true,
    "editor.semanticTokenColorCustomizations": {
        "rules": {
            "*.mutable": {
                "underline": true
            }
        }
    },
    "workbench.iconTheme": "material-icon-theme",
    "editor.formatOnSave": true,
    "files.autoSave": "afterDelay",
    "editor.renderWhitespace": "all",
    "git.confirmSync": false,
    "git.autofetch": true,
    "editor.stopRenderingLineAfter": -1,
    "editor.codeActionsOnSave": null
})";

// assert macro for output
#define assert_eq(left, right) { \
    if (left != right) { \
        std::cerr << RED << "[test.cpp:" << __LINE__ << "] ERROR: " << RESET << "Expected: \"" << right << "\", got: \"" << left << "\"" << std::endl; \
        exit(-1); \
    } \
}

void test_http_parser() {
    std::cout << "Testing HTTP parser..." << std::endl;
    std::string_view sw(HEADER);
    auto [body, head] = HttpHead::parse(sw);
    assert_eq(body, "<body here>\r\n");
    assert_eq(head.code, 200);
    assert_eq(head.headers["Server"], "ATS/8.0.8");
    assert_eq(head.headers["Vary"], "Accept-Encoding");
    assert_eq(head.headers["NEL"], R"({ "report_to": "wm_nel", "max_age": 86400, "failure_fraction": 0.05, "success_fraction": 0.0})");
}

void test_json_parser() {
    std::cout << "Testing JSON parser..." << std::endl;
    std::string_view sw(JSON);
    auto obj = Json::parse_value(sw);
    assert_eq(obj.obj()["workbench.colorTheme"].string(), "Dracula");
    assert_eq(obj.obj()["git.confirmSync"].boolean(), false);
    assert_eq(obj.obj()["editor.codeActionsOnSave"].is_null(), true);
    assert_eq(std::get<long>(obj.obj()["editor.stopRenderingLineAfter"].number()), -1);
    assert_eq(obj.obj()["editor.semanticTokenColorCustomizations"].obj()["rules"].obj()["*.mutable"].obj()["underline"].boolean(), true);
}

int main() {
    try {
        test_http_parser();
        test_json_parser();
    } catch (std::exception& e) {
        // catch all exceptions about things like missing keys, or out of bound errors
        std::cerr << RED << "Exception: " << RESET << e.what() << std::endl;
        exit(-1);
    }
    std::cout << GREEN << "All tests passed successfully" << RESET << std::endl;
}