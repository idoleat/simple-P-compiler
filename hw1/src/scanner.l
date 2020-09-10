%{
#define LIST               strcat(buf, yytext)
#define token(t)           {LIST; if (Opt_T) printf("<%s>\n", #t);}
#define tokenChar(t)       {LIST; if (Opt_T) printf("<%c>\n", (t));}
#define tokenString(t, s)  {LIST; if (Opt_T) printf("<%s%s>\n", #t, (s));}
#define tokenInteger(i) {LIST; if (Opt_T) printf("<integer: %s>\n", (i));}
#define tokenOctInteger(i) {LIST; if (Opt_T) printf("<oct_integer: %s>\n", (i));}
#define tokenFloat(i) {LIST; if (Opt_T) printf("<float: %s>\n", (i));}
#define tokenScientific(i) {LIST; if (Opt_T) printf("<scientific: %s>\n", (i));}
#define tokenSString(i) {LIST; if (Opt_T) printf("<string: %s>\n", (i));}
#define tokenIdentifier(i) {LIST; if (Opt_T) printf("<id: %s>\n", (i));}
#include <string.h>
//todo: float, scientific , opt

#define MAX_LINE_LENG      256
#define MAX_STRING_LENG      256

int Opt_S = 1;
int Opt_T = 1;
int linenum = 1;
int Last_State = 0;
char buf[MAX_LINE_LENG];
char strContent[MAX_STRING_LENG];
char delims[3] = "&/";
char *pseduos;
%}

%x OTHER
%x STRING
%x SPECIAL
%x COMMENT

%%
<INITIAL>[ \t]+	{ LIST; } //I need +?
","     { tokenChar(','); }
";"     { tokenChar(';'); }
":"     { tokenChar(':'); }
"["     { tokenChar('['); }
"]"     { tokenChar(']'); }
")"     { tokenChar(')'); }
"("     { tokenChar('('); }
"+"     { tokenChar('+'); }
"-"     { tokenChar('-'); }
"*"     { tokenChar('*'); }
"/"     { tokenChar('/'); }
"mod"   { token(mod); }
":="     { token(:=); }
"<"     { tokenChar('<'); }
"<="     { token(<=); }
"<>"     { token(<>); }
">="     { token(>=); }
">"     { tokenChar('>'); }
"="     { tokenChar('='); }
"and"     { token(and); }
"or"     { token(or); }
"not"     { token(not); }
array|begin|boolean|def|do|else|end|false|for|integer|if|of|print|read|real|string|then|to|true|return|var|while {
    tokenString(KW, yytext);
}



[1-9][0-9]*\.[0-9]+|[0-9]\.[0-9]+	{ tokenFloat(yytext); }
0[0-9]+ { tokenOctInteger(yytext); }
0|[1-9][0-9]* { tokenInteger(yytext); }
([1-9]+[0-9]*[\.]?[0-9]+)([eE][-+]?[0-9]+)	{ tokenScientific(yytext); }




"\""	{ LIST; strContent[0] = '\0'; BEGIN STRING; }
<STRING>"\\"	{ LIST; BEGIN SPECIAL; }
<SPECIAL>\"	{ LIST; strncat( strContent, "\"", MAX_STRING_LENG); BEGIN STRING; }
<SPECIAL>n	{ LIST; strncat( strContent, "\\n", MAX_STRING_LENG); BEGIN STRING; }
<SPECIAL>t	{ LIST; strncat( strContent, "\\t", MAX_STRING_LENG); BEGIN STRING; }
<SPECIAL>\\	{ LIST; strncat( strContent, "\\", MAX_STRING_LENG); BEGIN STRING; }
<SPECIAL>\n  { fprintf(stderr, "Error at line %d: %s\n", linenum, yytext); exit(-1); }
<SPECIAL>[^\"nt\\]	{
		LIST;
		strncat( strContent, "\\", MAX_STRING_LENG);
		strncat( strContent, yytext, MAX_STRING_LENG);
		BEGIN STRING;
}
<STRING>"\n"	{ fprintf(stderr,"Error at line %d: %s\n",linenum,yytext); exit(-1); }
<STRING>\"\"    { LIST; strncat( strContent, "\"", MAX_STRING_LENG); }
<STRING>\"	{ tokenSString(strContent); BEGIN INITIAL; }
<STRING>[^\\\n\"]	{ LIST; strncat( strContent, yytext, MAX_STRING_LENG); }





<INITIAL>"\/\/&S+"   { LIST; Opt_S = 1; }
<INITIAL>"\/\/&S-"   { LIST; Opt_S = 0; }
<INITIAL>"\/\/&T+"   { LIST; Opt_T = 1; }
<INITIAL>"\/\/&T-"   { LIST; Opt_T = 0; }
\/\/&[ST][-+].*     { LIST;
    pseduos = strtok(yytext, delims);
    if(strcmp(pseduos, "S-") == 0)
        Opt_S = 0;
    else if(strcmp(pseduos, "S+") == 0)
        Opt_S = 1;
    else if(strcmp(pseduos, "T-") == 0)
        Opt_T = 0;
    else if(strcmp(pseduos, "T+") == 0)
        Opt_T = 1;
}
<OTHER>\/\/[^\n]*	{ LIST; }
\/\/.*	{ LIST; }
<OTHER>"/*"	{ LIST; Last_State = OTHER; BEGIN COMMENT; }
"/*"	{ LIST; Last_State = INITIAL; BEGIN COMMENT; }
<COMMENT>"\n"	{
		if(Last_State == OTHER)Last_State = INITIAL;
		if (Opt_S)
  	        printf("%d: %s\n", linenum, buf);
		linenum++;
                buf[0] = '\0';
}
<COMMENT>.	{ LIST; }
<COMMENT>"*/"	{ LIST; BEGIN Last_State; }



[a-zA-Z]|[a-zA-Z_][a-zA-Z0-9_]*[a-zA-Z0-9]      { tokenIdentifier(yytext); }
\n      {
          if (Opt_S)
            printf("%d: %s\n", linenum, buf);
          linenum++;
          buf[0] = '\0';
}
.       {
          /* error */
          printf("Error at line %d: bad character \"%s\"\n", linenum, yytext ); exit(-1);
}


%%
int main( int argc, char **argv )
{
  if( argc != 2 ) {
    fprintf( stderr, "Usage: ./scanner [filename]\n" );
    exit(0);
  }

  FILE *fp = fopen( argv[1], "r" );
  if( fp == NULL ) {
    fprintf( stderr, "Open file error\n" );
    exit(-1);
  }

  yyin = fp;

  yylex();

  exit(0);
}
