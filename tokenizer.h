

#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <string>
#include <vector>

enum TokenType {
    CONSTANTS, VARIANT, OPERATOR, LEFT_BRACKETS, RIGHT_BRACKETS
};


class token {
public:
    token(const std::string& str, TokenType t) : value(str), type(t) {}
    ~token();
    std::string getValue() { return value; };
    TokenType getType() const { return type; }
private:
    std::string value;
    TokenType type;
};

class Tokenizer {
public:
    Tokenizer(const std::string& str = "") : lineText(str) {}
    ~Tokenizer();
    void setLineText(const std::string& str) { lineText = str; };
    void constructTokens();
    const std::vector<token>& getTokens();
private:
    std::string lineText;
    std::vector<token> tokens;
};



#endif //TOKENIZER_H
