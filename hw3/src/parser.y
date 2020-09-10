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

#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <list>
#include <string>

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
/* End */

static AstNode *root;
uint32_t indent = 0; //bad way, very bad, bad as hell. Passing this into every constructor is very stupid.
bool decl_initialized = false;
bool idlist_initialized = false;
bool func_initialized = false;
bool FmArg_initialized = false;
bool Arr_initialized = false;
bool states_initialized = false;
bool exp_initialized = false;
bool ArrRefs_initialized = false;
// All of these booleans are horrible.

extern "C" int yylex(void);
static void yyerror(const char *msg);
extern int yylex_destroy(void);
%}

%code requires {
    class AstNode;
    class AssignmentNode;
    class BinaryOperatorNode;
    class CompoundStatementNode;
    class ConstantValueNode;
    class DeclNode;
    class ExpressionNode;
    class ForNode;
    class FunctionNode;
    class FunctionInvocationNode;
    class IfNode;
    class PrintNode;
    class ProgramNode;
    class ReadNode;
    class ReturnNode;
    class UnaryOperatorNode;
    class VariableNode;
    class VariableReferenceNode;
    class WhileNode;
}

    /* For yylval */
%union {
    /* basic semantic value */
    int intVal;
	float floatVal;
	double doubleVal;
	char *stringVal;
    char *identifier;
    bool neg;
    /* AST nodes */
    AstNode *node;
    std::list<AstNode> *nodes;
    std::list<AstNode *> *p_nodes;
    std::list<DeclNode> *declList; // please use std::list<DeclNode *>
    DeclNode *declNode;
    std::list<std::string> *idlist;
    ConstantValueNode *constantValue;
    ExpressionNode *expression;
    std::list<ExpressionNode *> *expressions;
    FunctionNode *function;
    std::list<FunctionNode> *functionList; // please use std::list<FunctionNode *>
    CompoundStatementNode *cp_statement;
    ReturnNode *returnNode;
    FunctionInvocationNode *func_invo;
    VariableReferenceNode *VarRef;
    IfNode *ifnode;
};

%type <identifier> ProgramName
%type <declList> DeclarationList
%type <declList> Declarations
%type <declNode> Declaration
%type <idlist> IdList
%type <neg> NegOrNot
%type <constantValue> LiteralConstant
%type <constantValue> IntegerAndReal
%type <constantValue> StringAndBoolean
%type <VarRef> VariableReference
%type <expression> Expression
%type <expressions> Expressions
%type <expressions> ExpressionList
%type <stringVal> Type
%type <function> FunctionDefinition
%type <function> FunctionDeclaration
%type <function> Function
%type <functionList> Functions
%type <functionList> FunctionList
%type <stringVal> FunctionName
%type <declNode> FormalArg
%type <declList> FormalArgs
%type <declList> FormalArgList
%type <stringVal> ScalarType
%type <stringVal> ReturnType
%type <stringVal> ArrDecl
%type <stringVal> ArrType
%type <cp_statement> CompoundStatement
%type <p_nodes> StatementList
%type <p_nodes> Statements
%type <node> Statement
%type <returnNode> Return
%type <node> Simple
%type <func_invo> FunctionInvocation
%type <expressions> ArrRefs
%type <expressions> ArrRefList
%type <func_invo> FunctionCall
%type <ifnode> Condition
%type <cp_statement> ElseOrNot

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
%token <identifier> ID

    /* Literal */
%token <intVal> INT_LITERAL
%token <floatVal> REAL_LITERAL
%token <stringVal> STRING_LITERAL

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
        root = new ProgramNode(@1.first_line, @1.first_column, $1, "void", $3, $4, $5, &indent);

        delete($1);
    }
;

ProgramName:
    ID
;

DeclarationList:
    Epsilon {
        $$ = NULL;
    }
    |
    Declarations{
        decl_initialized = false;
        $$ = $1;
    }
;

Declarations:
    Declaration {
        if(!decl_initialized){
            $$ = new std::list<DeclNode>;
            decl_initialized = true;
            //printf("Decl A initializing.....\n");
        }
        $$->insert( $$->begin(), *$1);
    }
    |
    Declarations Declaration {
        $1->insert( $1->begin(), *$2);
        //printf("Decl B list initializing.....\n");
        $$ = $1; //Do i need this?
    }
;

FunctionList:
    Epsilon {
        $$ = NULL;
    }
    |
    Functions {
        $$ = $1;
        func_initialized = false;
    }
;

Functions:
    Function {
        if(!func_initialized){
            $$ = new std::list<FunctionNode>;
            func_initialized = true;
            //printf("func A initializing.....\n");
        }
        $$->push_front(*$1);
    }
    |
    Functions Function {
        $1->push_front(*$2);
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
        $$ = new FunctionNode( @1.first_line, @1.first_column, $1, $3, $5, &indent );
    }
;

FunctionDefinition:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType
    CompoundStatement
    END {
        $$ = new FunctionNode( @1.first_line, @1.first_column, $1, $3, $5, $6, &indent );
    }
;

FunctionName:
    ID
;

FormalArgList:
    Epsilon {
        $$ = NULL;
    }
    |
    FormalArgs {
        FmArg_initialized = false;
        $$ = $1;
    }
;

FormalArgs:
    FormalArg {
        if(!FmArg_initialized) {
            $$ = new std::list<DeclNode>;
            FmArg_initialized = true;
        }
        $$->push_back(*$1);
    }
    |
    FormalArgs SEMICOLON FormalArg {
        $1->push_back(*$3);
        $$ = $1;
    }
;

FormalArg:
    IdList COLON Type {
        idlist_initialized = false;
        std::string last = "";
        uint32_t offset = @1.first_column;
        std::list<VariableNode> *variableNodes = new std::list<VariableNode>;
        while(!$1->empty()){
            offset += last.length() + 2; 
            // Fucking dirty way. Column position should be passed as soon as id is push in list.
            // I think I should use a struct to store both id(string) and position(uint32_t), and then extract them here.
            variableNodes->push_back(
            *(new VariableNode(@1.first_line, offset-2, $1->front().c_str(), $3, &indent))
            );
            last = $1->front();
            $1->pop_front();
        }
        $$ = new DeclNode(@1.first_line, @1.first_column, variableNodes, &indent);
    }
;

IdList:
    ID {
        if(!idlist_initialized){
            $$ = new std::list<std::string>;
            idlist_initialized = true;
            //printf("ID A list initializing.....\n");
        }
        $$->push_back( std::string($1) );
    }
    |
    IdList COMMA ID {
        $1->push_back( std::string($3) );
        $$ = $1;
        //printf("ID B list initializing.....\n");
    }
;

ReturnType:
    COLON ScalarType {
        $$ = $2;
    }
    |
    Epsilon {
        $$ = "void";
    }
;

    /*
       Data Types and Declarations
                                   */

Declaration:
    VAR IdList COLON Type SEMICOLON {
        idlist_initialized = false;
        std::string last = "";
        uint32_t offset = @2.first_column;
        std::list<VariableNode> *variableNodes = new std::list<VariableNode>;
        while(!$2->empty()){
            offset += last.length() + 2;
            variableNodes->push_back(
                *(new VariableNode(@2.first_line, offset-2, $2->front().c_str(), $4, &indent))
            );
            last = $2->front();
            $2->pop_front();
        }
        $$ = new DeclNode(@1.first_line, @1.first_column, variableNodes, &indent);
    }
    |
    VAR IdList COLON LiteralConstant SEMICOLON {
        idlist_initialized = false;
        std::string last = "";
        uint32_t offset = @2.first_column;
        std::list<VariableNode> *variableNodes = new std::list<VariableNode>;
        while(!$2->empty()){
            offset += last.length() + 2;
            variableNodes->push_back(
                *(new VariableNode(@2.first_line, offset-2, $2->front().c_str(), $4, &indent))
            );
            last = $2->front();
            $2->pop_front();
        }
        $$ = new DeclNode(@1.first_line, @1.first_column, variableNodes, &indent);
    }
;

Type:
    ScalarType
    |
    ArrType
;

ScalarType:
    INTEGER
    |
    REAL
    |
    STRING
    |
    BOOLEAN

ArrType:
    ArrDecl ScalarType {
        char *ch = new char[256];
        strcat(ch, $2);
        strcat(ch, " ");
        strcat(ch, $1);
        $$ = ch;
        Arr_initialized = false;
    }
;

ArrDecl:
    ARRAY INT_LITERAL OF {
        if(!Arr_initialized){
            $$ = new char[256];
            Arr_initialized = true;
        }
        strcat($$, "[");
        strcat($$, std::to_string($2).c_str());
        strcat($$, "]");
    }
    |
    ArrDecl ARRAY INT_LITERAL OF {
        strcat($$, "[");
        strcat($$, std::to_string($3).c_str());
        strcat($$, "]");
    }
;

LiteralConstant:
    NegOrNot INT_LITERAL {
        if($1) $$ = new ConstantValueNode(@2.first_line, @2.first_column-1, -$2, &indent);
        else $$ = new ConstantValueNode(@2.first_line, @2.first_column, $2, &indent);
    }
    |
    NegOrNot REAL_LITERAL {
        if($1) $$ = new ConstantValueNode(@2.first_line, @2.first_column-1, -$2, &indent);
        else $$ = new ConstantValueNode(@2.first_line, @2.first_column, $2, &indent);
    }
    |
    StringAndBoolean
;

NegOrNot:
    Epsilon {
        $$ = false;
    }
    |
    MINUS %prec UNARY_MINUS {
        $$ = true;
    }
;

StringAndBoolean:
    STRING_LITERAL {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1, &indent);
    }
    |
    TRUE {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, true, &indent);
    }
    |
    FALSE {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, false, &indent);
    }
;

IntegerAndReal:
    INT_LITERAL {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1, &indent);
    }
    |
    REAL_LITERAL {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1, &indent);
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
        $$ = new CompoundStatementNode(@1.first_line, @1.first_column, $2, $3, &indent);
    }
;

Simple:
    VariableReference ASSIGN Expression SEMICOLON {
        $$ = new AssignmentNode(@2.first_line, @2.first_column, $1, $3, &indent);
    }
    |
    PRINT Expression SEMICOLON {
        $$ = new PrintNode(@1.first_line, @1.first_column, $2, &indent);
    }
    |
    READ VariableReference SEMICOLON {
        $$ = new ReadNode(@1.first_line, @1.first_column, $2, &indent);
    }
;

VariableReference:
    ID ArrRefList {
        $$ = new VariableReferenceNode(@1.first_line, @1.first_column, $1, $2, &indent);
    }
;

ArrRefList:
    Epsilon {
        $$ = NULL;
    }
    |
    ArrRefs {
        ArrRefs_initialized = false;
        $$ = $1;
    }
;

ArrRefs:
    L_BRACKET Expression R_BRACKET {
        if(!ArrRefs_initialized){
            $$ = new std::list<ExpressionNode *>;
            ArrRefs_initialized = true;
        }
        $$->push_back($2);
    }
    |
    ArrRefs L_BRACKET Expression R_BRACKET {
        $1->push_back($3);
        $$ = $1;
    }
;

Condition:
    IF Expression THEN
    CompoundStatement
    ElseOrNot
    END IF {
        $$ = new IfNode(@1.first_line, @1.first_column, $2, $4, $5, &indent);
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
    END DO
;

For:
    FOR ID ASSIGN INT_LITERAL TO INT_LITERAL DO
    CompoundStatement
    END DO
;

Return:
    RETURN Expression SEMICOLON {
        $$ = new ReturnNode(@1.first_line, @1.first_column, $2, &indent);
    }
;

FunctionCall:
    FunctionInvocation SEMICOLON
;

FunctionInvocation:
    ID L_PARENTHESIS ExpressionList R_PARENTHESIS {
        $$ = new FunctionInvocationNode(@1.first_line, @1.first_column, $1, $3, &indent);
    }
;

ExpressionList:
    Epsilon {
        $$ = NULL;
    }
    |
    Expressions {
        exp_initialized = false;
        $$ = $1;
    }
;

Expressions:
    Expression {
        if(!exp_initialized){
            $$ = new std::list<ExpressionNode *>;
            exp_initialized = true;
        }
        $$->push_back($1);
    }
    |
    Expressions COMMA Expression {
        $1->push_back($3);
        $$ = $1;
    }
;

StatementList:
    Epsilon {
        $$ = NULL;
    }
    |
    Statements {
        states_initialized = false;
        $$ = $1;
    }
;

Statements:
    Statement {
        if(!states_initialized){
            $$ = new std::list<AstNode *>;
            states_initialized = true;
        }
        $$->push_back($1);
    }
    |
    Statements Statement {
        $1->push_back($2);
        $$ = $1;
    }
;

Expression:
    L_PARENTHESIS Expression R_PARENTHESIS {
        $$ = $2;
    }
    |
    MINUS Expression %prec UNARY_MINUS {
        $$ = new UnaryOperatorNode(@1.first_line, @1.first_column, "-", $2, &indent);
    }
    |
    Expression MULTIPLY Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "*", $1, $3, &indent);
    }
    |
    Expression DIVIDE Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "/", $1, $3, &indent);
    }
    |
    Expression MOD Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "mod", $1, $3, &indent);
    }
    |
    Expression PLUS Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "+", $1, $3, &indent);
    }
    |
    Expression MINUS Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "-", $1, $3, &indent);
    }
    |
    Expression LESS Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "<", $1, $3, &indent);
    }
    |
    Expression LESS_OR_EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "<=", $1, $3, &indent);
    }
    |
    Expression GREATER Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, ">", $1, $3, &indent);
    }
    |
    Expression GREATER_OR_EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, ">=", $1, $3, &indent);
    }
    |
    Expression EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "=", $1, $3, &indent);
    }
    |
    Expression NOT_EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "<>", $1, $3, &indent);
    }
    |
    NOT Expression {
        $$ = new UnaryOperatorNode(@1.first_line, @1.first_column, "not", $2, &indent);
    }
    |
    Expression AND Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "and", $1, $3, &indent);
    }
    |
    Expression OR Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column, "or", $1, $3, &indent);
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

int main(int argc, const char *argv[]) {
    assert(argc >= 2 && "Usage: ./parser <filename> [--dump-ast]\n");

    yyin = fopen(argv[1], "r");
    assert(yyin != NULL && "fopen() fails.\n");

    yyparse();

    if (argc > 2 && strcmp(argv[2], "--dump-ast") == 0) {
        root->print();
    }

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");

    delete root;
    fclose(yyin);
    yylex_destroy();
    return 0;
}
