%{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "error.h"

extern int32_t LineNum;   /* declared in scanner.l */
extern char Buffer[512];  /* declared in scanner.l */
extern FILE *yyin;        /* declared by lex */
extern char *yytext;      /* declared by lex */

extern int yylex(void);
static void yyerror(const char *msg);
%}



%token COMMA
%token SEMICOLON
%token COLON

%token ASSIGN
%token LT
%token NGT
%token NEQ
%token NLT
%token GT
%token EQ
%left GT LT

%left PLUS SUB
%left MULTI DEVIDE
%left MOD
%left NOT
%left AND
%left OR

%token L_PARE_S
%token R_PARE_S
%token L_PARE_M
%token R_PARE_M

%token KW_ARRAY
%token KW_BEGIN
%token KW_BOOL
%token KW_DEF
%token KW_DO
%token KW_ELSE
%token KW_END
%token KW_FALSE
%token KW_FOR
%token KW_INTEGER
%token KW_IF
%token KW_OF
%token KW_PRINT
%token KW_READ
%token KW_REAL
%token KW_STRING
%token KW_THEN
%token KW_TO
%token KW_TRUE
%token KW_RETURN
%token KW_VAR
%token KW_WHILE

%token ID
%token INT
%token OCT
%token FLOAT
%token SCIENCE
%token STRING

%start program

%%

program: ID SEMICOLON var_const_decl_list func_decl_defin_list compound_statement KW_END ;


/*declaration/definition*/
var_decl : KW_VAR identifier_list COLON scalar_type SEMICOLON ;

const_decl : KW_VAR identifier_list COLON literal_const SEMICOLON ;

var_const_decl_list : /*epsilon*/
                    | var_const_decl_seq
                    ;

var_const_decl_seq : var_decl
                   | const_decl
                   | var_const_decl_seq var_decl /*var_decl var_const_decl_seq??*/
                   | var_const_decl_seq const_decl
                   ;

array_decl : KW_ARRAY INT KW_OF scalar_type; /*TODO: INT should be non-negative*/

function_def : ID L_PARE_S identifier_list COLON scalar_type R_PARE_S COLON non_array_scalar_type compound_statement KW_END
             | ID L_PARE_S identifier_list COLON scalar_type R_PARE_S compound_statement KW_END
             | ID L_PARE_S R_PARE_S COLON non_array_scalar_type compound_statement KW_END
             | ID L_PARE_S R_PARE_S compound_statement KW_END
             ;

function_decl: ID L_PARE_S identifier_list COLON scalar_type R_PARE_S COLON non_array_scalar_type SEMICOLON
             | ID L_PARE_S identifier_list COLON scalar_type R_PARE_S SEMICOLON
             | ID L_PARE_S R_PARE_S COLON non_array_scalar_type SEMICOLON
             | ID L_PARE_S R_PARE_S SEMICOLON
             ;

func_decl_defin_list : /*epsilon*/
                     | func_decl_defin_seq
                     ;

func_decl_defin_seq : function_decl
                    | function_def
                    | func_decl_defin_seq function_decl /*function_decl func_decl_defin_seq??*/
                    | func_decl_defin_seq function_def
                    ;


/*statement*/
statements : /*epsilon*/
           | statement_seq
           ;

statement_seq : statement
              | statement_seq statement /*statement statement_seq??*/
              ;

statement : compound_statement
		  |	simple
		  | conditional
		  |	while
		  | for
		  | return
          | function_invocation SEMICOLON
          ;

compound_statement : KW_BEGIN var_const_decl_list statements KW_END ;

simple : variable_reference ASSIGN expressions SEMICOLON
       | KW_PRINT variable_reference SEMICOLON
       | KW_PRINT expressions SEMICOLON
       | KW_READ variable_reference SEMICOLON

conditional : KW_IF expressions KW_THEN compound_statement KW_END KW_IF
            | KW_IF expressions KW_THEN compound_statement KW_ELSE compound_statement KW_END KW_IF
            ;

while : KW_WHILE expressions KW_DO compound_statement KW_END KW_DO ;

for : KW_FOR ID ASSIGN INT KW_TO INT KW_DO compound_statement KW_END KW_DO ;

return : KW_RETURN expressions SEMICOLON;

/*base componenet - expression*/
expressions : arithmetic_expression
			| boolean_expression
            | STRING
			;

arithmetic_expression : SUB expressions %prec MULTI
		   | expressions PLUS expressions
		   | expressions SUB expressions
		   | expressions MULTI expressions
		   | expressions DEVIDE expressions
		   | expressions MOD expressions %prec MULTI
		   | L_PARE_S expressions R_PARE_S
		   | INT
           | FLOAT
           | OCT
           | SCIENCE
		   | variable_reference
		   | function_invocation
		   ;


boolean_expression :expressions GT expressions
			       |expressions LT expressions
			       |expressions NGT expressions %prec GT
                   |expressions NLT expressions %prec LT
                   |expressions EQ expressions
                   |expressions NEQ expressions %prec GT
                   |expressions AND expressions %prec AND
                   |expressions OR  expressions %prec OR
                   |NOT expressions %prec NOT
                   |KW_TRUE
                   |KW_FALSE
                   |variable_reference
                   |function_invocation
                   ;

/*base component*/


function_invocation : ID L_PARE_S args R_PARE_S;

args : /* epsilon */
     | argseq
     ;

argseq : arg
       | argseq COMMA arg
       ;

arg : ID
    | literal_const
    | expressions
    ;

variable_reference : array_varible
                   | ID
                   ;

/*TODO: assignments to constants are not allowed and constants can not be declared in terms of other named constants. Variables may not be initialized in declarations.*/

identifier_list : ID
                | identifier_list COMMA ID
                ;

array_varible : ID dimension

dimension : dimension L_PARE_M arithmetic_expression R_PARE_M
          | L_PARE_M arithmetic_expression R_PARE_M
          ;

scalar_type : KW_INTEGER
            | KW_REAL
            | KW_STRING
            | KW_BOOL
            | array_decl
            ;

non_array_scalar_type : KW_INTEGER
                      | KW_REAL
                      | KW_STRING
                      | KW_BOOL
                      ;

literal_const : INT
              | FLOAT
              | SCIENCE
              | STRING
              | OCT
              | KW_TRUE
              | KW_FALSE
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
            LineNum, Buffer, yytext);
    exit(-1);
}

int main(int argc, const char *argv[]) {
    CHECK(argc == 2, "Usage: ./parser <filename>\n");

    FILE *fp = fopen(argv[1], "r");

    CHECK(fp != NULL, "fopen() fails.\n");
    yyin = fp;
    yyparse();

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");
    return 0;
}
