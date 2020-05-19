#include "valoresAuxiliares.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lex.yy.c"
int main( int argc, const char* argv[] ){
  inicializarTablaSimbolos();
  iniciarLexico((char *)argv[1]);
  struct elemento * auxiliar=yylex();
  while(auxiliar->lexema!=NULL){
    printf("%d : < %s , %d >\n\n",devolverLineas(), auxiliar->lexema, auxiliar->componenteLexico);
    free(auxiliar->lexema);
    free(auxiliar);
    auxiliar=yylex();
  }
  imprimirTaboa();
  acabarLexico();
  return 1;
}
