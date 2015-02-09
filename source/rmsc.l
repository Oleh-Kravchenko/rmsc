%option yylineno noyywrap nodefault

%{
#include "rmsc.tab.h"
%}

HEX			(0[xX][0-9a-fA-F]+)
INTEGER		(-?[0-9]+)
IDENTIFIER	([_a-zA-Z]+[_a-zA-Z0-9]*)

%x RESTFUL COMMENT

%%

"__restful"				{ BEGIN(RESTFUL); }
<RESTFUL>";"			{ return yytext[0]; }
<RESTFUL>"{"			{ return yytext[0]; }
<RESTFUL>"}"			{ return yytext[0]; }
<RESTFUL>"["			{ return yytext[0]; }
<RESTFUL>"]"			{ return yytext[0]; }
<RESTFUL>"*"			{ return yytext[0]; }
<RESTFUL>"struct"		{ return (STRUCT); }
<RESTFUL>{IDENTIFIER}	{ yylval.str = strdup(yytext); return (IDENTIFIER); }
<RESTFUL>{INTEGER}		{ return (INTEGER); }
<RESTFUL>{HEX}			{ return (HEX); }
<RESTFUL>.|\n			{ ; }

<RESTFUL>"/*"			{ BEGIN(COMMENT) ; }
<COMMENT>"*/"			{ BEGIN(RESTFUL); }
<COMMENT>([^*]|\en)+|.

.|\n					{ return (IGNORE); }

%%

void reset()
{
	BEGIN(INITIAL);
}
