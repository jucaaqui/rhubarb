
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "texture.h"
#include "rhubarb.h"

namespace rhu {

enum Type {
    WINSIZE,
    TARGET,
    TEXTURE,
    SHADER,
    IDENTIFIER,
    INT,
    STRING,
    SOURCE,
    ERROR,
    END
};

struct Token {
    Type type;
    std::string_view val;
    int row;
    int col;
};

struct Lexer {
    std::string src;
    int cur;
    int beg;
    int row;
    int bol;
};

std::vector<Token> tokenize(const std::string &src);

}
