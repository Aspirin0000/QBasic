
#include "tokenizer.h"
#include <regex>
#include <iostream>

token::~token() {
}

Tokenizer::~Tokenizer() {
}

void Tokenizer::constructTokens() {
    std::regex token(R"((-?\d+(\.\d+)?)|([+\-*/%**]+|MOD)|([a-zA-Z_][a-zA-Z0-9_]*)|([()]))");
    std::sregex_token_iterator regexIter(lineText.begin(), lineText.end(), token);
    std::vector<std::string> answer;
    for (; regexIter != std::sregex_token_iterator(); ++regexIter) {
        answer.push_back(*regexIter);
    }
    for (auto it = answer.begin(); it != answer.end(); ++it) {
        if(std::all_of(it->begin(), it->end(), ::isdigit)
            || (it->size() >= 2 && *it->begin() == '-' && std::all_of(it->begin() + 1, it->end(), ::isdigit)))
        {
            tokens.push_back({*it,CONSTANTS});
        }
        else if(*it == "+" || *it == "-" || *it == "*" || *it == "/" || *it == "MOD" || *it == "**")
        {
            tokens.push_back({*it,OPERATOR});
        }
        else if(*it == "(")
        {
            tokens.push_back({*it,LEFT_BRACKETS});
        }
        else if(*it == ")")
        {
            tokens.push_back({*it,RIGHT_BRACKETS});
        }
        else
        {
            tokens.push_back({*it,VARIANT});
        }
    }
}

const std::vector<token>&  Tokenizer::getTokens()
{
    return tokens;
}
