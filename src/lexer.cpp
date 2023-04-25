
#include <iostream>
#include <string_view>
#include <string>
#include <vector>

#include "lexer.h"
#include "texture.h"

namespace rhu {

bool starts_with(Lexer &lex, std::string_view x) {
    return std::string_view(lex.src.c_str() + lex.cur)
        .starts_with(x);
}

bool is_at_end(Lexer &lex) {
    return lex.cur >= (int) lex.src.length();
}

Token new_token(Lexer &lex, Type type) {
    return Token {
        .type = type,
        .val = std::string_view(lex.src.c_str() + lex.beg, 
                                lex.cur - lex.beg),
        .row = lex.row,
        .col = lex.beg - lex.bol
    };
}

Token directive(Lexer &lex) {
    Type type;
    if (starts_with(lex, "#winsize")) type = WINSIZE;
    if (starts_with(lex, "#target" )) type = TARGET;
    if (starts_with(lex, "#texture")) type = TEXTURE;
    if (starts_with(lex, "#shader" )) type = SHADER;

    ++lex.cur;
    while (isalpha(lex.src[lex.cur])) ++lex.cur;

    return new_token(lex, type);
}

Token identifier(Lexer &lex) {
    while (isalnum(lex.src[lex.cur])) ++lex.cur;
    return new_token(lex, IDENTIFIER);
}

Token number(Lexer &lex) {
    while (isdigit(lex.src[lex.cur])) ++lex.cur;
    return new_token(lex, INT);
}

Token string(Lexer &lex) {
    while (lex.src[++lex.cur] != '"') 
        if (lex.src[lex.cur] == '\n') return new_token(lex, ERROR);
    ++lex.cur;

    return new_token(lex, STRING);
}

Token source(Lexer &lex) {
    int row = lex.row;
    while (!is_at_end(lex)) {
        if (lex.src[lex.cur] == '\n') ++row;
        if (lex.src[lex.cur] == '#') {
            if (starts_with(lex, "#winsize")) break;
            if (starts_with(lex, "#target" )) break;
            if (starts_with(lex, "#texture")) break;
            if (starts_with(lex, "#shader" )) break;
        }
        ++lex.cur;
    }
    Token token = new_token(lex, SOURCE);
    lex.row = row;
    lex.bol = lex.cur - 1;
    return token;
}

Token error(Lexer &lex) {
    while (!is_at_end(lex)) ++lex.cur;
    return new_token(lex, ERROR);
}

Token next_token(Lexer &lex) {
    // trim white space left of token
    while (isspace(lex.src[lex.cur])) {
        if (lex.src[lex.cur] == '\n') {
            ++lex.row;
            lex.bol = lex.cur;
        }
        ++lex.cur;

        if (starts_with(lex, "//"))
            while (lex.src[lex.cur] != '\n') ++lex.cur;
    }
    lex.beg = lex.cur;

    if (lex.src[lex.cur] == '#'  ) return directive(lex);
    if (lex.src[lex.cur] == '"'  ) return string(lex);
    if (isalpha(lex.src[lex.cur])) return identifier(lex);
    if (isdigit(lex.src[lex.cur])) return number(lex);
    if (is_at_end(lex)           ) return Token { .type = END };
    return error(lex);
}

const char *map(Type type) {
    switch (type) {

    case WINSIZE:    return "WINSIZE";
    case TARGET:     return "TARGET";
    case TEXTURE:    return "TEXTURE";
    case SHADER:     return "SHADER";
    case IDENTIFIER: return "IDENTIFIER";
    case INT:        return "INT";
    case STRING:     return "STRING";
    case SOURCE:     return "SOURCE";
    case ERROR:      return "ERROR";
    case END:        return "END";
    default:         return "-------";

    }
}

std::vector<Token> tokenize(const std::string &src) {
    Lexer lex = {
        .src = src,
        .cur = 0,
        .beg = 0,
        .row = 1,
        .bol = 0
    };
    std::vector<Token> tokens;

    while (!is_at_end(lex)) {
        tokens.push_back(next_token(lex));

        if (tokens.back().type == SHADER) {
            while (lex.src[lex.cur] != '\n') {
                tokens.push_back(next_token(lex));
            }
            ++lex.cur;
            ++lex.row;
            lex.bol = lex.cur;
            lex.beg = lex.cur;
            tokens.push_back(source(lex));
        }

    }

    for (auto i : tokens) {
        std::cout << i.val << " \t" << i.row 
                           << '\t' << i.col
                           << '\t' << map(i.type) << '\n';
    }

    return tokens;
}
}

