%{
#include "AST/ast.hpp"
#include "AST/program.hpp"
#include "AST/decl.hpp"
#include "AST/variable.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/function.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/print.hpp"
#include "AST/expression.hpp"
#include "AST/BinaryOperator.hpp"
#include "AST/UnaryOperator.hpp"
#include "AST/FunctionInvocation.hpp"
#include "AST/VariableReference.hpp"
#include "AST/assignment.hpp"
#include "AST/read.hpp"
#include "AST/if.hpp"
#include "AST/while.hpp"
#include "AST/for.hpp"
#include "AST/return.hpp"
#include "AST/AstDumper.hpp"
#include "sema/SemanticAnalyzer.hpp"
#include "codegen/CodeGenerator.hpp"

#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>

#define YYLTYPE yyltype

typedef struct YYLTYPE {
    uint32_t first_line;
    uint32_t first_column;
    uint32_t last_line;
    uint32_t last_column;
} yyltype;

/* Declared by scanner.l */
extern uint32_t line_num;
extern char buffer[512];
extern FILE *yyin;
extern char *yytext;
extern bool opt_dmp;
/* End */

static AstNode *root;
static const uint32_t MAX_FILE_PATH_SIZE = 512;
char *out_file_name;

bool has_error = false;
extern "C" int yylex(void);
static void yyerror(const char *msg);
extern int yylex_destroy(void);
%}

%code requires {
    #include "AST/PType.hpp"
    #include "AST/utils.hpp"

    #include <vector>
    #include <memory>

    class AstNode;
    class DeclNode;
    class FunctionNode;
    class ExpressionNode;
}

    /* For yylval */
%union {
    /* basic semantic value */
    char *identifier;
    int32_t integer;
    double real;
    char *string;
    bool boolean;

    AstNode *node;
    std::vector<IdInfo> *ids;
    std::vector<std::unique_ptr<AstNode>> *nodes;
    std::vector<std::unique_ptr<DeclNode>> *decls;
    std::vector<std::unique_ptr<FunctionNode>> *funcs;
    std::vector<std::unique_ptr<ExpressionNode>> *exprs;
    PType *type;
    std::vector<uint64_t> *dimensions;
};

%type <identifier> ProgramName FunctionName ID
%type <integer> NegOrNot INT_LITERAL
%type <real> REAL_LITERAL
%type <string> STRING_LITERAL
%type <boolean> TRUE FALSE
%type <node> Declaration LiteralConstant StringAndBoolean IntegerAndReal
             Function FunctionDeclaration FunctionDefinition FormalArg
             Statement
             CompoundStatement Simple Condition ElseOrNot While For Return FunctionCall
             VariableReference Expression FunctionInvocation
%type <ids> IdList
%type <nodes> StatementList Statements
%type <decls> DeclarationList Declarations FormalArgList FormalArgs
%type <funcs> FunctionList Functions
%type <exprs> ArrRef ExpressionList Expressions
%type <type> Type ScalarType ArrType ReturnType
%type <dimensions> ArrDecl

    /* Delimiter */
%token COMMA SEMICOLON COLON
%token L_PARENTHESIS R_PARENTHESIS
%token L_BRACKET R_BRACKET

    /* Operator */
%token ASSIGN
%left OR
%left AND
%right NOT
%left LESS LESS_OR_EQUAL EQUAL GREATER GREATER_OR_EQUAL NOT_EQUAL
%left PLUS MINUS
%left MULTIPLY DIVIDE MOD
%right UNARY_MINUS

    /* Keyword */
%token ARRAY BOOLEAN INTEGER REAL STRING
%token END BEGIN_ /* Use BEGIN_ since BEGIN is a keyword in lex */
%token DO ELSE FOR IF THEN WHILE
%token DEF OF TO RETURN VAR
%token FALSE TRUE
%token PRINT READ

    /* Identifier */
%token ID

    /* Literal */
%token INT_LITERAL
%token REAL_LITERAL
%token STRING_LITERAL

%%
    /*
       Program Units
                     */

Program:
    ProgramName SEMICOLON
    /* ProgramBody */
    DeclarationList FunctionList CompoundStatement
    /* End of ProgramBody */
    END {
        root = new ProgramNode(@1.first_line, @1.first_column,
                               new PType(PType::PrimitiveTypeEnum::kVoidType),
                               $1, $3, $4,
                               static_cast<CompoundStatementNode *>($5));

        free($1);
        delete $3;
        delete $4;
    }
;

ProgramName:
    ID
;

DeclarationList:
    Epsilon {
        $$ = new std::vector<std::unique_ptr<DeclNode>>();
    }
    |
    Declarations {
        $$ = $1;
    }
;

Declarations:
    Declaration {
        $$ = new std::vector<std::unique_ptr<DeclNode>>;
        $$->emplace_back(static_cast<DeclNode *>($1));
    }
    |
    Declarations Declaration {
        $1->emplace_back(static_cast<DeclNode *>($2));
        $$ = $1;
    }
;

FunctionList:
    Epsilon {
        $$ = new std::vector<std::unique_ptr<FunctionNode>>();
    }
    |
    Functions
;

Functions:
    Function {
        $$ = new std::vector<std::unique_ptr<FunctionNode>>;
        $$->emplace_back(static_cast<FunctionNode *>($1));
    }
    |
    Functions Function {
        $1->emplace_back(static_cast<FunctionNode *>($2));
        $$ = $1;
    }
;

Function:
    FunctionDeclaration
    |
    FunctionDefinition
;

FunctionDeclaration:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType SEMICOLON {
        $$ = new FunctionNode(@1.first_line, @1.first_column, $1, $3, $5, nullptr);
        free($1);
        delete $3;
    }
;

FunctionDefinition:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType
    CompoundStatement
    END {
        $$ = new FunctionNode(@1.first_line, @1.first_column, $1, $3, $5,
                              static_cast<CompoundStatementNode *>($6));
        free($1);
        delete $3;
    }
;

FunctionName:
    ID
;

FormalArgList:
    Epsilon {
        $$ = new std::vector<std::unique_ptr<DeclNode>>();
    }
    |
    FormalArgs {
        $$ = $1;
    }
;

FormalArgs:
    FormalArg {
        $$ = new std::vector<std::unique_ptr<DeclNode>>;
        $$->emplace_back(static_cast<DeclNode *>($1));
    }
    |
    FormalArgs SEMICOLON FormalArg {
        $1->emplace_back(static_cast<DeclNode *>($3));
        $$ = $1;
    }
;

FormalArg:
    IdList COLON Type {
        $$ = new DeclNode(@1.first_line, @1.first_column, $1, $3);
        delete $1;
    }
;

IdList:
    ID {
        auto ids = new std::vector<IdInfo>();
        ids->emplace_back(@1.first_line, @1.first_column, $1);
        free($1);
        $$ = ids;
    }
    |
    IdList COMMA ID {
        $1->emplace_back(@3.first_line, @3.first_column, $3);
        free($3);
        $$ = $1;
    }
;

ReturnType:
    COLON ScalarType {
        $$ = $2;
    }
    |
    Epsilon {
        $$ = new PType(PType::PrimitiveTypeEnum::kVoidType);
    }
;

    /*
       Data Types and Declarations
                                   */

Declaration:
    VAR IdList COLON Type SEMICOLON {
        $$ = new DeclNode(@1.first_line, @1.first_column, $2, $4);
        delete $2;
    }
    |
    VAR IdList COLON LiteralConstant SEMICOLON {
        $$ = new DeclNode(@1.first_line, @1.first_column, $2,
                          static_cast<ConstantValueNode *>($4));
        delete $2;
    }
;

Type:
    ScalarType
    |
    ArrType
;

    /* no need to release PType object, since it will be assigned to shared_ptr */
ScalarType:
    INTEGER { $$ = new PType(PType::PrimitiveTypeEnum::kIntegerType); }
    |
    REAL { $$ = new PType(PType::PrimitiveTypeEnum::kRealType); }
    |
    STRING { $$ = new PType(PType::PrimitiveTypeEnum::kStringType); }
    |
    BOOLEAN { $$ = new PType(PType::PrimitiveTypeEnum::kBoolType); }
;

ArrType:
    ArrDecl ScalarType {
        $2->setDimensions(*$1);
        delete $1;

        $$ = $2;
    }
;

ArrDecl:
    ARRAY INT_LITERAL OF {
        $$ = new std::vector<uint64_t>{static_cast<uint64_t>($2)};
    }
    |
    ArrDecl ARRAY INT_LITERAL OF {
        $1->emplace_back($3);
        $$ = $1;
    }
;

LiteralConstant:
    NegOrNot INT_LITERAL {
        Constant::ConstantValue value;
        value.integer = static_cast<int32_t>($1 * $2);
        auto *constant = new Constant(
            std::make_shared<const PType>(PType::PrimitiveTypeEnum::kIntegerType), value);
        auto *pos = ($1 == 1) ? &@2 : &@1;
        $$ = new ConstantValueNode(pos->first_line, pos->first_column, constant);
    }
    |
    NegOrNot REAL_LITERAL {
        Constant::ConstantValue value;
        value.real = static_cast<double>($1 * $2);
        auto *constant = new Constant(
            std::make_shared<const PType>(PType::PrimitiveTypeEnum::kRealType), value);
        auto *pos = ($1 == 1) ? &@2 : &@1;
        $$ = new ConstantValueNode(pos->first_line, pos->first_column, constant);
    }
    |
    StringAndBoolean
;

NegOrNot:
    Epsilon {
        $$ = 1;
    }
    |
    MINUS %prec UNARY_MINUS {
        $$ = -1;
    }
;

StringAndBoolean:
    STRING_LITERAL {
        Constant::ConstantValue value;
        value.string = $1;
        auto *constant = new Constant(
            std::make_shared<const PType>(PType::PrimitiveTypeEnum::kStringType), value);
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, constant);
    }
    |
    TRUE {
        Constant::ConstantValue value;
        value.boolean = $1;
        auto *constant = new Constant(
            std::make_shared<const PType>(PType::PrimitiveTypeEnum::kBoolType), value);
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, constant);
    }
    |
    FALSE {
        Constant::ConstantValue value;
        value.boolean = $1;
        auto *constant = new Constant(
            std::make_shared<const PType>(PType::PrimitiveTypeEnum::kBoolType), value);
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, constant);
    }
;

IntegerAndReal:
    INT_LITERAL {
        Constant::ConstantValue value;
        value.integer = static_cast<int32_t>($1);
        auto *constant = new Constant(
            std::make_shared<const PType>(PType::PrimitiveTypeEnum::kIntegerType), value);
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, constant);
    }
    |
    REAL_LITERAL {
        Constant::ConstantValue value;
        value.real = static_cast<double>($1);
        auto *constant = new Constant(
            std::make_shared<const PType>(PType::PrimitiveTypeEnum::kRealType), value);
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, constant);
    }
;

    /*
       Statements
                  */

Statement:
    CompoundStatement
    |
    Simple
    |
    Condition
    |
    While
    |
    For
    |
    Return
    |
    FunctionCall
;

CompoundStatement:
    BEGIN_
    DeclarationList
    StatementList
    END {
        $$ = new CompoundStatementNode(@1.first_line, @1.first_column, $2, $3);
        delete $2;
        delete $3;
    }
;

Simple:
    VariableReference ASSIGN Expression SEMICOLON {
        $$ = new AssignmentNode(@2.first_line, @2.first_column,
                                static_cast<VariableReferenceNode *>($1),
                                static_cast<ExpressionNode *>($3));
    }
    |
    PRINT Expression SEMICOLON {
        $$ = new PrintNode(@1.first_line, @1.first_column,
                           static_cast<ExpressionNode *>($2));
    }
    |
    READ VariableReference SEMICOLON {
        $$ = new ReadNode(@1.first_line, @1.first_column,
                          static_cast<VariableReferenceNode *>($2));
    }
;

VariableReference:
    ID {
        $$ = new VariableReferenceNode(@1.first_line, @1.first_column, $1);
        delete $1;
    }
    |
    ID ArrRef {
        $$ = new VariableReferenceNode(@1.first_line, @1.first_column, $1, $2);
        delete $1;
        delete $2;
    }
;

ArrRef:
    L_BRACKET Expression R_BRACKET {
        $$ = new std::vector<std::unique_ptr<ExpressionNode>>;
        $$->emplace_back(static_cast<ExpressionNode *>($2));
    }
    |
    ArrRef L_BRACKET Expression R_BRACKET {
        $1->emplace_back(static_cast<ExpressionNode *>($3));
        $$ = $1;
    }
;

Condition:
    IF Expression THEN
    CompoundStatement
    ElseOrNot
    END IF {
        $$ = new IfNode(@1.first_line, @1.first_column,
                        static_cast<ExpressionNode *>($2),
                        static_cast<CompoundStatementNode *>($4),
                        static_cast<CompoundStatementNode *>($5));
    }
;

ElseOrNot:
    ELSE
    CompoundStatement {
        $$ = $2;
    }
    |
    Epsilon {
        $$ = NULL;
    }
;

While:
    WHILE Expression DO
    CompoundStatement
    END DO {
        $$ = new WhileNode(@1.first_line, @1.first_column,
                           static_cast<ExpressionNode *>($2),
                           static_cast<CompoundStatementNode *>($4));
    }
;

For:
    FOR ID ASSIGN INT_LITERAL TO INT_LITERAL DO
    CompoundStatement
    END DO {
        Constant::ConstantValue value;
        Constant *constant;
        ConstantValueNode *constant_value_node;

        // DeclNode
        auto *ids = new std::vector<IdInfo>{IdInfo(@2.first_line, @2.first_column, $2)};
        auto *type = new PType(PType::PrimitiveTypeEnum::kIntegerType);
        auto *var_decl = new DeclNode(@2.first_line, @2.first_column, ids, type);

        // AssignmentNode
        auto *var_ref = new VariableReferenceNode(@2.first_line, @2.first_column, $2);
        value.integer = static_cast<int32_t>($4);
        constant = new Constant(
            std::make_shared<const PType>(PType::PrimitiveTypeEnum::kIntegerType), value);
        constant_value_node = new ConstantValueNode(@4.first_line, @4.first_column,
                                                          constant);
        auto *assignment = new AssignmentNode(@3.first_line, @3.first_column,
                                              var_ref, constant_value_node);

        // ExpressionNode
        value.integer = static_cast<int32_t>($6);
        constant = new Constant(
            std::make_shared<const PType>(PType::PrimitiveTypeEnum::kIntegerType), value);
        constant_value_node = new ConstantValueNode(@6.first_line, @6.first_column,
                                                    constant);

        $$ = new ForNode(@1.first_line, @1.first_column,
                         var_decl, assignment, constant_value_node,
                         static_cast<CompoundStatementNode *>($8));
        free($2);
        delete ids;
    }
;

Return:
    RETURN Expression SEMICOLON {
        $$ = new ReturnNode(@1.first_line, @1.first_column,
                            static_cast<ExpressionNode *>($2));
    }
;

FunctionCall:
    FunctionInvocation SEMICOLON
;

FunctionInvocation:
    ID L_PARENTHESIS ExpressionList R_PARENTHESIS {
        $$ = new FunctionInvocationNode(@1.first_line, @1.first_column, $1, $3);
        delete $1;
        delete $3;
    }
;

ExpressionList:
    Epsilon {
        $$ = new std::vector<std::unique_ptr<ExpressionNode>>();
    }
    |
    Expressions {
        $$ = $1;
    }
;

Expressions:
    Expression {
        $$ = new std::vector<std::unique_ptr<ExpressionNode>>;
        $$->emplace_back(static_cast<ExpressionNode *>($1));
    }
    |
    Expressions COMMA Expression {
        $1->emplace_back(static_cast<ExpressionNode *>($3));
        $$ = $1;
    }
;

StatementList:
    Epsilon {
        $$ = new std::vector<std::unique_ptr<AstNode>>();
    }
    |
    Statements {
        $$ = $1;
    }
;

Statements:
    Statement {
        $$ = new std::vector<std::unique_ptr<AstNode>>;
        $$->emplace_back($1);
    }
    |
    Statements Statement {
        $1->emplace_back($2);
        $$ = $1;
    }
;

Expression:
    L_PARENTHESIS Expression R_PARENTHESIS {
        $$ = $2;
    }
    |
    MINUS Expression %prec UNARY_MINUS {
        $$ = new UnaryOperatorNode(@1.first_line, @1.first_column,
                                   Operator::kNegOp, static_cast<ExpressionNode *>($2));
    }
    |
    Expression MULTIPLY Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    Operator::kMultiplyOp,
                                    static_cast<ExpressionNode *>($1),
                                    static_cast<ExpressionNode *>($3));
    }
    |
    Expression DIVIDE Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    Operator::kDivideOp,
                                    static_cast<ExpressionNode *>($1),
                                    static_cast<ExpressionNode *>($3));
    }
    |
    Expression MOD Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    Operator::kModOp,
                                    static_cast<ExpressionNode *>($1),
                                    static_cast<ExpressionNode *>($3));
    }
    |
    Expression PLUS Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    Operator::kPlusOp,
                                    static_cast<ExpressionNode *>($1),
                                    static_cast<ExpressionNode *>($3));
    }
    |
    Expression MINUS Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    Operator::kMinusOp,
                                    static_cast<ExpressionNode *>($1),
                                    static_cast<ExpressionNode *>($3));
    }
    |
    Expression LESS Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    Operator::kLessOp,
                                    static_cast<ExpressionNode *>($1),
                                    static_cast<ExpressionNode *>($3));
    }
    |
    Expression LESS_OR_EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    Operator::kLessOrEqualOp,
                                    static_cast<ExpressionNode *>($1),
                                    static_cast<ExpressionNode *>($3));
    }
    |
    Expression GREATER Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    Operator::kGreaterOp,
                                    static_cast<ExpressionNode *>($1),
                                    static_cast<ExpressionNode *>($3));
    }
    |
    Expression GREATER_OR_EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    Operator::kGreaterOrEqualOp,
                                    static_cast<ExpressionNode *>($1),
                                    static_cast<ExpressionNode *>($3));
    }
    |
    Expression EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    Operator::kEqualOp,
                                    static_cast<ExpressionNode *>($1),
                                    static_cast<ExpressionNode *>($3));
    }
    |
    Expression NOT_EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    Operator::kNotEqualOp,
                                    static_cast<ExpressionNode *>($1),
                                    static_cast<ExpressionNode *>($3));
    }
    |
    NOT Expression {
        $$ = new UnaryOperatorNode(@1.first_line, @1.first_column,
                                   Operator::kNotOp, static_cast<ExpressionNode *>($2));
    }
    |
    Expression AND Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    Operator::kAndOp,
                                    static_cast<ExpressionNode *>($1),
                                    static_cast<ExpressionNode *>($3));
    }
    |
    Expression OR Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    Operator::kOrOp,
                                    static_cast<ExpressionNode *>($1),
                                    static_cast<ExpressionNode *>($3));
    }
    |
    IntegerAndReal
    |
    StringAndBoolean
    |
    VariableReference
    |
    FunctionInvocation
;

    /*
       misc
            */
Epsilon:
;
%%

void yyerror(const char *msg) {
    fprintf(stderr,
            "\n"
            "|-----------------------------------------------------------------"
            "---------\n"
            "| Error found in Line #%d: %s\n"
            "|\n"
            "| Unmatched token: %s\n"
            "|-----------------------------------------------------------------"
            "---------\n",
            line_num, buffer, yytext);
    exit(-1);
}

char *parseOutputName(const char *file_path, const char *out_dir_name) {
    char *ret = new char[MAX_FILE_PATH_SIZE];
    char *tmp_file_path = new char[MAX_FILE_PATH_SIZE];
    char *file_name; 
    char *dot_pos; 
      
    snprintf(tmp_file_path, MAX_FILE_PATH_SIZE, "%s", file_path);
    file_name = strrchr(tmp_file_path, '/');
    if (file_name == nullptr)
        file_name = tmp_file_path;

    dot_pos = strrchr(file_name, '.');
    assert(dot_pos != nullptr && "file extension error.\n");
    *(dot_pos+1) = 'S';
    
    snprintf(ret, MAX_FILE_PATH_SIZE, "%s/%s", out_dir_name, file_name);

    delete[] tmp_file_path;

    return ret;
}

int main(int argc, const char *argv[]) {
    assert(argc >= 4 && "Usage: ./compiler <filename> --save-path [save path]\n");
    out_file_name = parseOutputName(argv[1], argv[3]);

    yyin = fopen(argv[1], "r");
    assert(yyin != NULL && "fopen() fails.\n");

    yyparse();

    CodeGenerator generator(argv[1], out_file_name);
    SemanticAnalyzer analyzer(opt_dmp, &generator);

    root->accept(analyzer);
    //root->accept(generator);

    if (!has_error) {
        printf("\n"
               "|---------------------------------------------------|\n"
               "|  There is no syntactic error and semantic error!  |\n"
               "|---------------------------------------------------|\n");
    }

    delete root;
    delete[] out_file_name;
    fclose(yyin);
    yylex_destroy();
    return 0;
}
