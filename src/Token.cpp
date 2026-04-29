#include "Token.h"

#include <string>

const char* get_string(Token value) {
#define X(NAME) if (value == (Token::NAME)) return (#NAME);
    TOKENS
#undef X
    return "Invalid";
}
std::string to_string(Token value) {
    return std::string(get_string(value));
}
