//
// Created by pedro on 22.10.20.
//

#ifndef DISCORD_ISA_JSONEXCEPTION_H
#define DISCORD_ISA_JSONEXCEPTION_H


#include <stdexcept>

/**
 * Error type
 */
enum JsonError{
    WrongValue,
    BrokenInput
};

/**
 * Custom JSON exception
 */
class JsonException: public std::runtime_error {
public:
    explicit JsonException(JsonError);
    const char *what() const noexcept override;

    JsonError err_type() const;

private:
    JsonError e;
    const char* message;
};


#endif //DISCORD_ISA_JSONEXCEPTION_H
