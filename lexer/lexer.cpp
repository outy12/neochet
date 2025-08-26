#include "lexer/lexer.h"
#include "lexer/token.h"
#include <cctype>
#include <vector>
#include <unordered_map>

// 关键字映射
static const std::unordered_map<std::string, TokenType> keywords = {
    {"program", TokenType::PROGRAM},
    {"club", TokenType::CLUB},
    {"event", TokenType::EVENT},
    {"happen", TokenType::HAPPEN},
    {"func", TokenType::FUNC},
    {"class", TokenType::CLASS},
    {"v", TokenType::V},
    {"val", TokenType::VAL},
    {"import", TokenType::IMPORT},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"for", TokenType::FOR},
    {"while", TokenType::WHILE},
    {"return", TokenType::RETURN},
    {"match", TokenType::MATCH},
    {"case", TokenType::CASE},
    {"and", TokenType::AND},
    {"or", TokenType::OR},
    {"not", TokenType::NOT},
    {"async", TokenType::ASYNC},
    {"timeout", TokenType::TIMEOUT},
    {"every", TokenType::EVERY},
    {"after", TokenType::AFTER},
    {"true", TokenType::BOOL},
    {"false", TokenType::BOOL}
    {"stream",TokenType::STREAM}
};

Lexer::Lexer(const std::string& source) : source(source), start(0), current(0), line(1), column(1) {}

std::vector<Token> Lexer::scanTokens() {
    std::vector<Token> tokens;
    
    while (!isAtEnd()) {
        start = current;
        scanToken(tokens);
    }
    
    tokens.push_back(createToken(TokenType::END_OF_FILE, ""));
    return tokens;
}

void Lexer::scanToken(std::vector<Token>& tokens) {
    char c = advance();
    
    switch (c) {
        case '(': addToken(TokenType::LPAREN); break;
        case ')': addToken(TokenType::RPAREN); break;
        case '{': addToken(TokenType::LBRACE); break;
        case '}': addToken(TokenType::RBRACE); break;
        case '[': addToken(TokenType::LBRACKET); break;
        case ']': addToken(TokenType::RBRACKET); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case ':': addToken(TokenType::COLON); break;
        case '?': addToken(TokenType::QUESTION); break;
