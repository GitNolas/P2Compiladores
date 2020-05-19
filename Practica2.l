CARACTER [a-zA-Z]
DIGITODECIMAL [0-9]
DIGITOBINARIO [0-1]
DIGITOHEXADECIMAL [0-9a-fB-F]
DIGITOOCTAL [0-7]
EXP [Ee][+-]?{DIGITODECIMAL}+
NUMEROHEXA 0x{DIGITOHEXADECIMAL}*
NUMEROENTEIRO {DIGITODECIMAL}+
NUMEROFLOTANTE {NUMEROENTEIRO}{EXP}
%x COMENTARIOBLOQUE
%{
#define YY_DECL struct elemento *yylex(void)
#include <stdio.h>
#include <stdlib.h>
#include "palabrasReservadas.h"
#include "valoresAuxiliares.h"
#include "tablaSimbolos.h"
int nLineas=1;
struct elemento *crearElemento(char *key, int value);
struct elemento *crearEOF();
%}
%%
\n  ++nLineas;//Deixamos pasar os caracteres que non nos interesan
\t
" "
"(" return crearElemento(yytext, PARENTESISI);
")" return crearElemento(yytext, PARENTESISD);
"[" return crearElemento(yytext,CORCHETEI);
"]" return crearElemento(yytext, CORCHETED);
"{" return crearElemento(yytext, LLAVEI);
"}" return crearElemento(yytext, LLAVED);
"." return crearElemento(yytext, PUNTO);
"-" return crearElemento(yytext, MENOS);
"+" return crearElemento(yytext,MAS);
"," return crearElemento(yytext,COMA);
";" return crearElemento(yytext,PUNTOCOMA);
":" return crearElemento(yytext,DOUSPUNTOS);
"/" return crearElemento(yytext,SLASH);
"*" return crearElemento(yytext,MULT);
"<-" return crearElemento(yytext,FLECHA);
"=" return crearElemento(yytext,IGUAL);
":=" return crearElemento(yytext,DOSPUNTOSIGUAL);
"+=" return crearElemento(yytext,MASIGUAL);
[\/][\/].*
"/*" BEGIN(COMENTARIOBLOQUE);
<COMENTARIOBLOQUE>[^*\n]*
<COMENTARIOBLOQUE>"*"+[^*/\n]*
<COMENTARIOBLOQUE>\n nLineas++;
<COMENTARIOBLOQUE>"*"+"/" BEGIN(INITIAL);
["]([^"\\\n]|\\.|\\\n)*["]
{CARACTER}+{DIGITODECIMAL}* return crearElemento(yytext,returnTabla(yytext));
"_" return crearElemento(yytext,returnTabla(yytext));
{NUMEROENTEIRO} return crearElemento(yytext,ENTERO);
{DIGITODECIMAL}*"."{DIGITODECIMAL}*{EXP}? return crearElemento(yytext,FLOTANTE);
{NUMEROHEXA} return crearElemento(yytext,HEXADECIMAL);
{NUMEROFLOTANTE} return crearElemento(yytext,FLOTANTE);
{NUMEROENTEIRO}".e+"{NUMEROENTEIRO}"i" return crearElemento(yytext,COMPLEJO);
{DIGITODECIMAL}"i" return crearElemento(yytext,COMPLEJO);
<<EOF>> return crearElemento(NULL,FINFICHERO);
%%
int devolverLineas(){
  return nLineas;
}

struct elemento *crearElemento(char *key, int value){
  struct elemento *e;
  e=malloc(sizeof(struct elemento));
  if(key==NULL){
    e->componenteLexico=FINFICHERO;
    e->lexema=NULL;
  }else{
    e->componenteLexico=value;
    e->lexema=malloc(sizeof(char)*strlen(key));
    e->lexema=strcpy(e->lexema,key);
  }
  return e;
}
void iniciarLexico(char* nomeFicheiro){
  yyin=fopen(nomeFicheiro,"r");
  if(yyin == NULL){
      printf("Non se pode abrir o arquivo\n");
      exit(1);
    }
}
void acabarLexico(){
    fclose(yyin);
    yylex_destroy();
}
