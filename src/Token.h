#pragma once
#include <string>
#include <cstdint>

#define TOKENS /* NAME */\
    X(Identifier)\
    X(Number)\
    X(Dot)\
    X(Bar)\
    X(EndOfFile)\
    X(Invalid)

enum class Token {
#define X(NAME) NAME,
    TOKENS
#undef X
};

const char* get_string(Token value);
std::string to_string(Token value);

using num_data_t = uint64_t;

struct Lexeme {
    Token tok;
    uint64_t id;
};
