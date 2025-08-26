
#pragma once
#include <string>
#include <variant>

enum class TokenType {
    // 关键字
    PROGRAM, CLUB, EVENT, HAPPEN, FUNC, CLASS, V, VAL, IMPORT,STREAM
    IF, ELSE, FOR, WHILE, RETURN, MATCH, CASE,
    AND, OR, NOT, ASYNC, TIMEOUT, EVERY, AFTER,
    
    // 标识符和字面量
    IDENTIFIER, INTEGER, FLOAT, STRING, BOOL,
    
    // 运算符
    PLUS, MINUS, STAR, SLASH, PERCENT,
    EQUAL, EQUAL_EQUAL, BANG_EQUAL,
    LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,
    ARROW, DOT, COLON, COMMA, QUESTION,
    
    // 分隔符
    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET,
    
    // 其他
    NEWLINE, INDENT, DEDENT, END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
    
    
    std::variant<int, float, bool, std::string> value;
    
    Token(TokenType type, const std::string& lexeme, int line, int column);
    Token(TokenType type, const std::string& lexeme, int line, int column, 
          const std::variant<int, float, bool, std::string>& value);
};
