#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include "util.h"

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
