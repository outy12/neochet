#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

typedef struct {
    Lexer* lexer;
    Token current;
    Token peek;
    // 错误处理
    int had_error;
    const char* error_msg;
} Parser;

Parser* parser_create(Lexer* lexer);
void parser_destroy(Parser* parser);

// 解析入口点
Program* parse_program(Parser* parser);

// 各语法结构的解析函数
ClubDecl* parse_club_decl(Parser* parser);
EventDecl* parse_event_decl(Parser* parser);
FnDecl* parse_fn_decl(Parser* parser);
ClassDecl* parse_class_decl(Parser* parser);
StreamDecl* parse_stream_decl(Parser* parser);
VarDecl* parse_var_decl(Parser* parser);
Stmt* parse_stmt(Parser* parser);
Expr* parse_expr(Parser* parser);

#endif
