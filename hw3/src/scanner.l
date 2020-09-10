%{
#include <stdint.h>
#include <string.h>
#include <cstring>
#include <list>
#include <string>

#include "parser.h"

#define YY_USER_ACTION \
    yylloc.first_line = line_num; \
    yylloc.first_column = col_num; \
    col_num += yyleng;

#define LIST                strCat(yytext)
#define TOKEN(t)            { LIST; if (opt_tok) printf("<%s>\n", #t); }
#define TOKEN_CHAR(t)       { LIST; if (opt_tok) printf("<%c>\n", (t)); }
#define TOKEN_STRING(t, s)  { LIST; if (opt_tok) printf("<%s: %s>\n", #t, (s)); }
#define MAX_LINE_LENG       512
#define MAX_ID_LENG         32

// prevent undefined reference error in newer version of flex
extern "C" int yylex(void);

uint32_t line_num = 1;
uint32_t col_num = 1;
char buffer[MAX_LINE_LENG];

static int32_t opt_src = 1;
static int32_t opt_tok = 1;
static char string_literal[MAX_LINE_LENG];
static char *buffer_ptr = buffer;

static void strCat(const char *Text);

%}

integer 0|[1-9][0-9]*
float {integer}\.(0|[0-9]*[1-9])

%x CCOMMENT

%%
    /* Delimiter */
"," { TOKEN_CHAR(','); return COMMA; }
";" { TOKEN_CHAR(';'); return SEMICOLON; }
":" { TOKEN_CHAR(':'); return COLON; }
"(" { TOKEN_CHAR('('); return L_PARENTHESIS; }
")" { TOKEN_CHAR(')'); return R_PARENTHESIS; }
"[" { TOKEN_CHAR('['); return L_BRACKET; }
"]" { TOKEN_CHAR(']'); return R_BRACKET; }

    /* Operator */
"+"   { TOKEN_CHAR('+'); yylval.stringVal = "+"; return PLUS; }
"-"   { TOKEN_CHAR('-'); yylval.stringVal = "-"; return MINUS; }
"*"   { TOKEN_CHAR('*'); yylval.stringVal = "*"; return MULTIPLY; }
"/"   { TOKEN_CHAR('/'); yylval.stringVal = "/"; return DIVIDE; }
"mod" { TOKEN(mod);      yylval.stringVal = "mod"; return MOD; }
":="  { TOKEN(:=);       yylval.stringVal = ":="; return ASSIGN; }
"<"   { TOKEN_CHAR('<'); yylval.stringVal = "<"; return LESS; }
"<="  { TOKEN(<=);       yylval.stringVal = "<="; return LESS_OR_EQUAL; }
"<>"  { TOKEN(<>);       yylval.stringVal = "<>"; return NOT_EQUAL; }
">="  { TOKEN(>=);       yylval.stringVal = ">="; return GREATER_OR_EQUAL; }
">"   { TOKEN_CHAR('>'); yylval.stringVal = ">"; return GREATER; }
"="   { TOKEN_CHAR('='); yylval.stringVal = "="; return EQUAL; }
"and" { TOKEN(and);      yylval.stringVal = "and"; return AND; }
"or"  { TOKEN(or);       yylval.stringVal = "or"; return OR; }
"not" { TOKEN(not);      yylval.stringVal = "not"; return NOT; }

    /* Keyword */
"array"   { TOKEN(KWarray);   return ARRAY; }
"begin"   { TOKEN(KWbegin);   return BEGIN_; }
"boolean" { TOKEN(KWboolean); yylval.stringVal = "boolean"; return BOOLEAN; }
"def"     { TOKEN(KWdef);     return DEF; }
"do"      { TOKEN(KWdo);      return DO; }
"else"    { TOKEN(KWelse);    return ELSE; }
"end"     { TOKEN(KWend);     return END; }
"false"   { TOKEN(KWfalse);   return FALSE; }
"for"     { TOKEN(KWfor);     return FOR; }
"integer" { TOKEN(KWinteger); yylval.stringVal = "integer"; return INTEGER; }
"if"      { TOKEN(KWif);      return IF; }
"of"      { TOKEN(KWof);      yylval.stringVal = "of"; return OF; }
"print"   { TOKEN(KWprint);   return PRINT; }
"read"    { TOKEN(KWread);    return READ; }
"real"    { TOKEN(KWreal);    yylval.stringVal = "real"; return REAL; }
"string"  { TOKEN(KWstring);  yylval.stringVal = "string"; return STRING; }
"then"    { TOKEN(KWthen);    return THEN; }
"to"      { TOKEN(KWto);      return TO; }
"true"    { TOKEN(KWtrue);    return TRUE; }
"return"  { TOKEN(KWreturn);  return RETURN; }
"var"     { TOKEN(KWvar);     return VAR; }
"while"   { TOKEN(KWwhile);   return WHILE; }

    /* Identifier */
[a-zA-Z][a-zA-Z0-9]* {
    TOKEN_STRING(id, yytext);
    yylval.identifier = strndup(yytext, MAX_ID_LENG);
    return ID;
}

    /* Integer (decimal/octal) */
{integer} {
    TOKEN_STRING(integer, yytext);
    yylval.intVal=atoi(yytext);
    return INT_LITERAL;
}
0[0-7]+   {
    TOKEN_STRING(oct_integer, yytext);
    int temp = atoi(yytext);
    int scaler = 1;
    while(temp != 0){
        yylval.intVal += temp % 8 * scaler;
        temp /= 8;

        scaler *= 8;
    }
    return INT_LITERAL;
}

    /* Floating-Point */
{float} {
    TOKEN_STRING(float, yytext);
    yylval.floatVal=atof(yytext);
    return REAL_LITERAL;
}

    /* Scientific Notation [Ee][+-]?[0-9]+ */
({integer}|{float})[Ee][+-]?({integer}) {
    TOKEN_STRING(scientific, yytext);
    yylval.doubleVal=strtod(yytext,NULL);
    return REAL_LITERAL; //why you share the same token with float?
}

    /* String */
\"([^"\n]|\"\")*\" {
    char *yyt_ptr = yytext;
    char *str_ptr = string_literal;

    /* Condition guard of the end of string literal
       Check only when *yyt_ptr == '"' */
    while (*(yyt_ptr + 1) != '\0') {
        ++yyt_ptr;

        /* Handle the situation of double quotes */
        if (*yyt_ptr == '"' && *(yyt_ptr + 1) != '\0')
            *str_ptr++ = *yyt_ptr++;

        while (*yyt_ptr != '"')
            *str_ptr++ = *yyt_ptr++;
    }
    *str_ptr = '\0';
    TOKEN_STRING(string, string_literal);
    yylval.stringVal = strndup(string_literal, MAX_ID_LENG);
    return STRING_LITERAL;
}
    /* Whitespace */
[ \t]+ { LIST; }

    /* Pseudocomment */
"//&"[ST][+-].* {
    LIST;
    char option = yytext[3];
    switch (option) {
    case 'S':
        opt_src = (yytext[4] == '+') ? 1 : 0;
        break;
    case 'T':
        opt_tok = (yytext[4] == '+') ? 1 : 0;
        break;
    }
}

    /* C++ Style Comment */
"//".* { LIST; }

    /* C Style Comment */
"/*"           { LIST; BEGIN(CCOMMENT); }
<CCOMMENT>"*/" { LIST; BEGIN(INITIAL); }
<CCOMMENT>.    { LIST; }

    /* Newline */
<INITIAL,CCOMMENT>\n {
    if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num;
    col_num = 1;
    buffer[0] = '\0';
    buffer_ptr = buffer;
}

    /* Catch the character which is not accepted by rules above */
. {
    printf("Error at line %d: bad character \"%s\"\n", line_num, yytext);
    exit(-1);
}

%%

static void strCat(const char *Text) {
    while (*Text) {
        *buffer_ptr++ = *Text++;
    }
    *buffer_ptr = '\0';
}
