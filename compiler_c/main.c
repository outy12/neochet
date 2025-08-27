#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
// 解析类声明
ClassDecl* parse_class_decl(Parser* parser) {
    consume_token(parser, TOKEN_KEYWORD, "class");
    
    Token name_token = consume_token(parser, TOKEN_IDENTIFIER, NULL);
    char* class_name = strdup(name_token.value);
    
    char* base_class = NULL;
    if (match_token(parser, TOKEN_KEYWORD, "extends")) {
        consume_token(parser, TOKEN_KEYWORD, "extends");
        Token base_token = consume_token(parser, TOKEN_IDENTIFIER, NULL);
        base_class = strdup(base_token.value);
    }
    
    consume_token(parser, TOKEN_PUNCTUATION, "{");
    
    Node** members = NULL;
    int member_count = 0;
    
    while (!match_token(parser, TOKEN_PUNCTUATION, "}") && 
           !match_token(parser, TOKEN_EOF, NULL)) {
        // 解析字段、构造函数、方法等
        if (match_token(parser, TOKEN_KEYWORD, "var")) {
            FieldDecl* field = parse_field_decl(parser);
            members = realloc(members, sizeof(Node*) * (member_count + 1));
            members[member_count++] = create_field_decl_node(field);
        } else if (match_token(parser, TOKEN_KEYWORD, "fn")) {
            Token fn_name = peek_token(parser);
            if (strcmp(fn_name.value, "this") == 0) {
                ConstructorDecl* ctor = parse_constructor_decl(parser);
                members = realloc(members, sizeof(Node*) * (member_count + 1));
                members[member_count++] = create_constructor_node(ctor);
            } else if (strcmp(fn_name.value, "~this") == 0) {
                // 解析析构函数
                MethodDecl* dtor = parse_destructor_decl(parser);
                members = realloc(members, sizeof(Node*) * (member_count + 1));
                members[member_count++] = create_method_node(dtor);
            } else {
                MethodDecl* method = parse_method_decl(parser);
                members = realloc(members, sizeof(Node*) * (member_count + 1));
                members[member_count++] = create_method_node(method);
            }
        } else if (match_token(parser, TOKEN_KEYWORD, "static")) {
            // 解析静态成员
            consume_token(parser, TOKEN_KEYWORD, "static");
            if (match_token(parser, TOKEN_KEYWORD, "var")) {
                FieldDecl* field = parse_field_decl(parser);
                field->is_static = 1;
                members = realloc(members, sizeof(Node*) * (member_count + 1));
                members[member_count++] = create_field_decl_node(field);
            } else if (match_token(parser, TOKEN_KEYWORD, "fn")) {
                MethodDecl* method = parse_method_decl(parser);
                method->is_static = 1;
                members = realloc(members, sizeof(Node*) * (member_count + 1));
                members[member_count++] = create_method_node(method);
            }
        } else {
            // 错误处理
            parser_error(parser, "Unexpected token in class declaration");
            break;
        }
    }
    
    consume_token(parser, TOKEN_PUNCTUATION, "}");
    
    ClassDecl* class_decl = malloc(sizeof(ClassDecl));
    class_decl->name = class_name;
    class_decl->base_class = base_class;
    class_decl->members = members;
    class_decl->member_count = member_count;
    
    return class_decl;
}

// 解析构造函数
ConstructorDecl* parse_constructor_decl(Parser* parser) {
    consume_token(parser, TOKEN_KEYWORD, "fn");
    consume_token(parser, TOKEN_KEYWORD, "this");
    
    consume_token(parser, TOKEN_PUNCTUATION, "(");
    Node** params = parse_param_list(parser);
    consume_token(parser, TOKEN_PUNCTUATION, ")");
    
    Node* base_call = NULL;
    if (match_token(parser, TOKEN_PUNCTUATION, ":")) {
        consume_token(parser, TOKEN_PUNCTUATION, ":");
        consume_token(parser, TOKEN_KEYWORD, "base");
        consume_token(parser, TOKEN_PUNCTUATION, "(");
        Node** base_args = parse_arg_list(parser);
        consume_token(parser, TOKEN_PUNCTUATION, ")");
        
        base_call = create_super_call_node(base_args, count_args(base_args));
    }
    
    Node* body = parse_block(parser);
    
    ConstructorDecl* ctor = malloc(sizeof(ConstructorDecl));
    ctor->params = params;
    ctor->param_count = count_params(params);
    ctor->body = body;
    ctor->base_call = base_call;
    
    return ctor;
}

// 解析方法声明
MethodDecl* parse_method_decl(Parser* parser) {
    int is_static = 0;
    if (match_token(parser, TOKEN_KEYWORD, "static")) {
        consume_token(parser, TOKEN_KEYWORD, "static");
        is_static = 1;
    }
    
    consume_token(parser, TOKEN_KEYWORD, "fn");
    
    Token name_token = consume_token(parser, TOKEN_IDENTIFIER, NULL);
    char* method_name = strdup(name_token.value);
    
    consume_token(parser, TOKEN_PUNCTUATION, "(");
    Node** params = parse_param_list(parser);
    consume_token(parser, TOKEN_PUNCTUATION, ")");
    
    char* return_type = "void";
    if (match_token(parser, TOKEN_PUNCTUATION, ":")) {
        consume_token(parser, TOKEN_PUNCTUATION, ":");
        Token type_token = consume_token(parser, TOKEN_IDENTIFIER, NULL);
        return_type = strdup(type_token.value);
    }
    
    Node* body = parse_block(parser);
    
    MethodDecl* method = malloc(sizeof(MethodDecl));
    method->name = method_name;
    method->params = params;
    method->param_count = count_params(params);
    method->return_type = return_type;
    method->body = body;
    method->is_static = is_static;
    
    return method;
}

// 解析字段声明
FieldDecl* parse_field_decl(Parser* parser) {
    consume_token(parser, TOKEN_KEYWORD, "var");
    
    Token name_token = consume_token(parser, TOKEN_IDENTIFIER, NULL);
    char* field_name = strdup(name_token.value);
    
    char* field_type = NULL;
    Node* initial_value = NULL;
    
    if (match_token(parser, TOKEN_PUNCTUATION, ":")) {
        consume_token(parser, TOKEN_PUNCTUATION, ":");
        Token type_token = consume_token(parser, TOKEN_IDENTIFIER, NULL);
        field_type = strdup(type_token.value);
    }
    
    if (match_token(parser, TOKEN_OPERATOR, "=")) {
        consume_token(parser, TOKEN_OPERATOR, "=");
        initial_value = parse_expr(parser);
    }
    
    consume_token(parser, TOKEN_PUNCTUATION, ";");
    
    FieldDecl* field = malloc(sizeof(FieldDecl));
    field->name = field_name;
    field->type = field_type;
    field->initial_value = initial_value;
    field->is_static = 0;
    
    return field;
}

// 解析 new 表达式
NewExpr* parse_new_expr(Parser* parser) {
    consume_token(parser, TOKEN_KEYWORD, "new");
    
    Token class_token = consume_token(parser, TOKEN_IDENTIFIER, NULL);
    char* class_name = strdup(class_token.value);
    
    consume_token(parser, TOKEN_PUNCTUATION, "(");
    Node** args = parse_arg_list(parser);
    consume_token(parser, TOKEN_PUNCTUATION, ")");
    
    NewExpr* new_expr = malloc(sizeof(NewExpr));
    new_expr->class_name = class_name;
    new_expr->args = args;
    new_expr->arg_count = count_args(args);
    
    return new_expr;
}

// 解析成员访问表达式
MemberAccess* parse_member_access(Parser* parser, Node* object) {
    consume_token(parser, TOKEN_OPERATOR, ".");
    
    Token member_token = consume_token(parser, TOKEN_IDENTIFIER, NULL);
    char* member_name = strdup(member_token.value);
    
    MemberAccess* access = malloc(sizeof(MemberAccess));
    access->object = object;
    access->member_name = member_name;
    
    return access;
}

// 解析 this 表达式
ThisExpr* parse_this_expr(Parser* parser) {
    consume_token(parser, TOKEN_KEYWORD, "this");
    
    ThisExpr* this_expr = malloc(sizeof(ThisExpr));
    return this_expr;
}

int compile_file(const char* filename, const char* output_filename) {
    // 读取源文件
    char* source = read_file(filename);
    if (!source) {
        fprintf(stderr, "Error: Could not read file %s\n", filename);
        return 1;
    }
    
    // 词法分析
    Lexer* lexer = lexer_create(source);
    if (!lexer) {
        free(source);
        return 1;
    }
    
    // 语法分析
    Parser* parser = parser_create(lexer);
    Node* program = parse_program(parser);
    
    if (parser->had_error) {
        fprintf(stderr, "Parse errors encountered\n");
        free_node(program);
        parser_destroy(parser);
        lexer_destroy(lexer);
        free(source);
        return 1;
    }
    
    // 代码生成
    FILE* output = fopen(output_filename, "w");
    if (!output) {
        fprintf(stderr, "Error: Could not open output file %s\n", output_filename);
        free_node(program);
        parser_destroy(parser);
        lexer_destroy(lexer);
        free(source);
        return 1;
    }
    
    CodeGenerator* gen = codegen_create(output);
    codegen_generate(gen, program);
    
    // 清理
    fclose(output);
    codegen_destroy(gen);
    free_node(program);
    parser_destroy(parser);
    lexer_destroy(lexer);
    free(source);
    
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input.neochet> [output.c]\n", argv[0]);
        return 1;
    }
    
    const char* input_file = argv[1];
    const char* output_file = (argc > 2) ? argv[2] : "output.c";
    
    int result = compile_file(input_file, output_file);
    
    if (result == 0) {
        printf("Compilation successful. Generated %s\n", output_file);
        
        
        char command[256];
        snprintf(command, sizeof(command), "gcc -o %s %s", "output", output_file);
        printf("Compiling C code: %s\n", command);
        result = system(command);
        
        if (result == 0) {
            printf("Executable created: output\n");
        } else {
            fprintf(stderr, "C compilation failed\n");
        }
    }
    
    return result;
}
