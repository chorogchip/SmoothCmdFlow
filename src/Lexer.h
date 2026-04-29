#pragma once

#include <string_view>
#include <vector>

#include "Token.h"
#include "StringPool.h"

namespace scf {

class Lexer {
private:
    static bool is_identifier_char(char ch) {
        return ('A' <= ch && ch <= 'Z') ||
               ('a' <= ch && ch <= 'z') ||
               ch == '-' ||
               ch == '_';
    }

public:
    inline static std::vector<Lexeme> lex(std::string_view str, StringPool& table) {
        table.clear();

        std::vector<Lexeme> out;
        size_t i = 0;
        const size_t sz = str.size();

        while (i < sz) {
            while (i < sz && str[i] == ' ') {
                ++i;
            }

            if (i >= sz) {
                break;
            }

            char ch = str[i];

            if (ch == '.') {
                out.push_back(Lexeme{Token::Dot, 0});
                ++i;
            } else if (ch == '|') {
                out.push_back(Lexeme{Token::Bar, 0});
                ++i;
            } else if ('0' <= ch && ch <= '9') {
                num_data_t data = 0;

                while (i < sz && '0' <= str[i] && str[i] <= '9') {
                    data = data * 10 + static_cast<num_data_t>(str[i] - '0');
                    ++i;
                }

                out.push_back(Lexeme{Token::Number, data});
            } else {
                size_t start = i;

                while (i < sz && is_identifier_char(str[i])) {
                    ++i;
                }

                if (i == start) {
                    out.push_back(Lexeme{Token::Invalid, i});
                    break;
                }

                size_t id = table.add(str.substr(start, i - start));
                out.push_back(Lexeme{Token::Identifier, id});
            }
        }
        out.push_back(Lexeme{Token::EndOfFile, 0});
        return out;
    }
};

}
