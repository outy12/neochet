#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

typedef struct {
    FILE* output;
    int indent_level;
    int error_count;
} CodeGenerator;

CodeGenerator* codegen_create(FILE* output);
void codegen_destroy(CodeGenerator* gen);

void codegen_generate(CodeGenerator* gen, Node* program);

// 各节点的代码生成函数
void codegen_program(CodeGenerator* gen, Program* program);
void codegen_club_decl(CodeGenerator* gen, ClubDecl* club);
void codegen_event_decl(CodeGenerator* gen, EventDecl* event);
void codegen_fn_decl(CodeGenerator* gen, FnDecl* fn);
void codegen_class_decl(CodeGenerator* gen, ClassDecl* class);
void codegen_stream_decl(CodeGenerator* gen, StreamDecl* stream);
void codegen_var_decl(CodeGenerator* gen, VarDecl* var);
void codegen_stmt(CodeGenerator* gen, Node* stmt);
void codegen_expr(CodeGenerator* gen, Node* expr);

// 辅助函数
void codegen_indent(CodeGenerator* gen);
void codegen_write(CodeGenerator* gen, const char* format, ...);

#endif
