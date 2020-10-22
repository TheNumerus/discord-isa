//
// Created by pedro on 22.10.20.
//

#include "JsonException.h"

const char *JsonException::what() const noexcept {
    return message;
}

JsonException::JsonException(const JsonError e) : runtime_error("") {
    this->e = e;
    switch (e) {
        case WrongValue:
            this->message = "Tried to parse wrong value";
            break;
        case BrokenInput:
            this->message = "Invalid input";
            break;
    }
}

JsonError JsonException::err_type() const {
    return e;
}
