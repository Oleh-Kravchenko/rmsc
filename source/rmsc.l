%option yylineno noyywrap nodefault

%{
#include "rmsc.tab.h"
#include "flexbison.h"
%}

INTEGER			(-?[0-9]+)
IDENTIFIER		([_a-zA-Z]+[_a-zA-Z0-9]*)
HEXADECIMAL		(0[xX][0-9a-fA-F]+)

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
<RESTFUL>{IDENTIFIER}	{
							if (!(yylval.str = strdup(yytext))) {
								yyerror("strdup(): %m");
								YY_BREAK;
							}
							return (IDENTIFIER);
						}
<RESTFUL>{INTEGER}		{ return (INTEGER); }
<RESTFUL>{HEXADECIMAL}	{ return (HEXADECIMAL); }
<RESTFUL>.|\n			{ ; }

<RESTFUL>"/*"			{ BEGIN(COMMENT); }
<COMMENT>"*/"			{ BEGIN(RESTFUL); }
<COMMENT>.|\n			{ ; }
<COMMENT><<EOF>>		{ yyerror("unclosed comment"); }

.|\n					{ return (IGNORE); }

%%

void flex_reset()
{
	BEGIN(INITIAL);
}
