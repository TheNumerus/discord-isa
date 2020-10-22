//
// Created by pedro on 22.10.20.
//

#ifndef DISCORD_ISA_JSONEXCEPTION_H
#define DISCORD_ISA_JSONEXCEPTION_H


#include <stdexcept>

enum JsonError{
    WrongValue,
    BrokenInput
};

class JsonException: public std::runtime_error {
public:
    explicit JsonException(JsonError);
    [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

    [[nodiscard]] JsonError err_type() const;

private:
    JsonError e;
    const char* message;
};


#endif //DISCORD_ISA_JSONEXCEPTION_H
