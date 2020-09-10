%{
#include <stdint.h>

#include "parser.h"
#include "error.h"

#define LIST                strCat(yytext)
#define TOKEN(t)            {LIST; if (OptTok) printf("<%s>\n", #t);}
#define TOKEN_CHAR(t)       {LIST; if (OptTok) printf("<%c>\n", (t));}
#define TOKEN_STRING(t, s)  {LIST; if (OptTok) printf("<%s: %s>\n", #t, (s));}
#define MAX_LINE_LENG       512

int32_t LineNum = 1;
char Buffer[MAX_LINE_LENG];

static int32_t OptSrc = 1;
static int32_t OptTok = 1;
static char StringLiteral[MAX_LINE_LENG];
static char *BufferPtr = Buffer;

static void strCat(const char *Text);

%}

integer 0|[1-9][0-9]*
float {integer}\.(0|[0-9]*[1-9])

%x CCOMMENT

%%
    /* Delimiter */
"," { TOKEN_CHAR(','); return COMMA;}
";" { TOKEN_CHAR(';'); return SEMICOLON;}
":" { TOKEN_CHAR(':'); return COLON;}
"(" { TOKEN_CHAR('('); return L_PARE_S;}
")" { TOKEN_CHAR(')'); return R_PARE_S;}
"[" { TOKEN_CHAR('['); return L_PARE_M;}
"]" { TOKEN_CHAR(']'); return R_PARE_M;}

    /* Operator */
"+"   { TOKEN_CHAR('+'); return PLUS;}
"-"   { TOKEN_CHAR('-'); return SUB;}
"*"   { TOKEN_CHAR('*'); return MULTI;}
"/"   { TOKEN_CHAR('/'); return DEVIDE;}
"mod" { TOKEN(mod); return MOD;}
":="  { TOKEN(:=); return ASSIGN;}
"<"   { TOKEN_CHAR('<'); return LT;}
"<="  { TOKEN(<=); return NGT;}
"<>"  { TOKEN(<>); return NEQ;}
">="  { TOKEN(>=); return NLT;}
">"   { TOKEN_CHAR('>'); return GT;}
"="   { TOKEN_CHAR('='); return EQ;}
"and" { TOKEN(and); return AND;}
"or"  { TOKEN(or); return OR;}
"not" { TOKEN(not); return NOT;}

    /* Keyword */
"array"   { TOKEN(KWarray); return KW_ARRAY;}
"begin"   { TOKEN(KWbegin); return KW_BEGIN;}
"boolean" { TOKEN(KWboolean); return KW_BOOL;}
"def"     { TOKEN(KWdef); return KW_DEF;}
"do"      { TOKEN(KWdo); return KW_DO;}
"else"    { TOKEN(KWelse); return KW_ELSE;}
"end"     { TOKEN(KWend); return KW_END;}
"false"   { TOKEN(KWfalse); return KW_FALSE;}
"for"     { TOKEN(KWfor); return KW_FOR;}
"integer" { TOKEN(KWinteger); return KW_INTEGER;}
"if"      { TOKEN(KWif); return KW_IF;}
"of"      { TOKEN(KWof); return KW_OF;}
"print"   { TOKEN(KWprint); return KW_PRINT;}
"read"    { TOKEN(KWread); return KW_READ;}
"real"    { TOKEN(KWreal); return KW_REAL;}
"string"  { TOKEN(KWstring); return KW_STRING;}
"then"    { TOKEN(KWthen); return KW_THEN;}
"to"      { TOKEN(KWto); return KW_TO;}
"true"    { TOKEN(KWtrue); return KW_TRUE;}
"return"  { TOKEN(KWreturn); return KW_RETURN;}
"var"     { TOKEN(KWvar); return KW_VAR;}
"while"   { TOKEN(KWwhile); return KW_WHILE;}

    /* Identifier */
[a-zA-Z][a-zA-Z0-9]* { TOKEN_STRING(id, yytext); return ID;}

    /* Integer (decimal/octal) */
{integer} { TOKEN_STRING(integer, yytext); return INT;}
0[0-7]+   { TOKEN_STRING(oct_integer, yytext); return OCT;}

    /* Floating-Point */
{float} { TOKEN_STRING(float, yytext); return FLOAT;}

    /* Scientific Notation [Ee][+-]?[0-9]+ */
({integer}|{float})[Ee][+-]?({integer}) { TOKEN_STRING(scientific, yytext); return SCIENCE;}

    /* String */
\"([^"\n]|\"\")*\" {
    char *YytextPtr = yytext;
    char *StrPtr = StringLiteral;

    /* Condition guard of the end of string literal
       Check only when *YytextPtr == '"' */
    while (*(YytextPtr + 1) != '\0') {
        ++YytextPtr;

        /* Handle the situation of double quotes */
        if (*YytextPtr == '"' && *(YytextPtr + 1) != '\0')
            *StrPtr++ = *YytextPtr++;

        while (*YytextPtr != '"')
            *StrPtr++ = *YytextPtr++;
    }
    *StrPtr = '\0';
    TOKEN_STRING(string, StringLiteral);
    return STRING;
}

    /* Whitespace */
[ \t]+ { LIST; }

    /* Pseudocomment */
"//&"[ST][+-].* {
    LIST;
    char option = yytext[3];
    switch (option) {
    case 'S':
        OptSrc = (yytext[4] == '+') ? 1 : 0;
        break;
    case 'T':
        OptTok = (yytext[4] == '+') ? 1 : 0;
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
    if (OptSrc)
        printf("%d: %s\n", LineNum, Buffer);
    ++LineNum;
    Buffer[0] = '\0';
    BufferPtr = Buffer;
}

    /* Catch the character which is not accepted by rules above */
. {
    printf("Error at line %d: bad character \"%s\"\n", LineNum, yytext);
    exit(-1);
}

%%

static void strCat(const char *Text) {
    while (*Text) {
        *BufferPtr++ = *Text++;
    }
    *BufferPtr = '\0';
}
