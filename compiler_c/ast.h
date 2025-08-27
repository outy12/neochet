#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAM,
    NODE_CLUB_DECL,
    NODE_EVENT_DECL,
    NODE_FN_DECL,
    NODE_CLASS_DECL,
    NODE_STREAM_DECL,
    NODE_VAR_DECL,
    NODE_BLOCK,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_RETURN,
    NODE_HAPPEN,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_CALL,
    NODE_IDENTIFIER,
    NODE_LITERAL,
    NODE_ON_CLAUSE
} NodeType;

typedef struct Node Node;

typedef struct {
    Node** clubs;
    int club_count;
} Program;

typedef struct {
    char* name;
    Node** members;
    int member_count;
} ClubDecl;

typedef struct {
    char* name;
    Node** fields;
    int field_count;
} EventDecl;

typedef struct {
    char* name;
    Node** params;
    int param_count;
    char* return_type;
    Node* body;
    int is_async;
} FnDecl;

typedef struct {
    char* name;
    char* base_class;
    Node** members;
    int member_count;
} ClassDecl;

typedef struct {
    char* name;
    char* base_stream;
    Node** on_clauses;
    int on_clause_count;
} StreamDecl;

typedef struct {
    char* name;
    char* type;
    Node* value;
} VarDecl;

typedef struct {
    Node** statements;
    int statement_count;
} Block;

typedef struct {
    Node* condition;
    Node* then_branch;
    Node* else_branch;
} IfStmt;

typedef struct {
    Node* condition;
    Node* body;
} WhileStmt;

typedef struct {
    Node* init;
    Node* condition;
    Node* increment;
    Node* body;
} ForStmt;

typedef struct {
    Node* value;
} ReturnStmt;

typedef struct {
    char* event_name;
    Node** args;
    int arg_count;
} HappenStmt;

typedef struct {
    char* op;
    Node* left;
    Node* right;
} BinaryOp;

typedef struct {
    char* op;
    Node* operand;
} UnaryOp;

typedef struct {
    Node* callee;
    Node** args;
    int arg_count;
} CallExpr;

typedef struct {
    char* value;
} Identifier;

typedef struct {
    char* type;
    char* value;
} Literal;

typedef struct {
    char* event_name;
    Node** params;
    int param_count;
    Node* condition;
    Node* body;
} OnClause;

struct Node {
    NodeType type;
    int line;
    int column;
    union {
        Program program;
        ClubDecl club_decl;
        EventDecl event_decl;
        FnDecl fn_decl;
        ClassDecl class_decl;
        StreamDecl stream_decl;
        VarDecl var_decl;
        Block block;
        IfStmt if_stmt;
        WhileStmt while_stmt;
        ForStmt for_stmt;
        ReturnStmt return_stmt;
        HappenStmt happen_stmt;
        BinaryOp binary_op;
        UnaryOp unary_op;
        CallExpr call_expr;
        Identifier identifier;
        Literal literal;
        OnClause on_clause;
    } as;
};

// AST 节点创建和销毁函数
Node* create_program_node();
Node* create_club_decl_node(char* name);
// ... 其他节点创建函数

void free_node(Node* node);
typedef struct {
    char* name;
    char* base_class;
    Node** members;
    int member_count;
} ClassDecl;

typedef struct {
    Node** params;
    int param_count;
    Node* body;
    Node* base_call; // 基类构造函数调用
} ConstructorDecl;

typedef struct {
    char* name;
    Node** params;
    int param_count;
    char* return_type;
    Node* body;
    int is_static;
} MethodDecl;

typedef struct {
    char* name;
    char* type;
    Node* initial_value;
    int is_static;
} FieldDecl;

typedef struct {
    char* class_name;
    Node** args;
    int arg_count;
} NewExpr;

typedef struct {
    Node* object;
    char* member_name;
} MemberAccess;

typedef struct {
    Node** args;
    int arg_count;
} SuperCall;

typedef struct {
    // 无额外字段，仅表示 this 关键字
} ThisExpr;
#endif
