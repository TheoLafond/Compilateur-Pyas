#include <stdlib.h>
#include <stdio.h>

#include <pyas/regexp.h>
#include <pyas/chargroup.h>
#include <pyas/lexem.h>
#include <pyas/list.h>
#include <pyas/parsing.h>
#include <pyas/py_codeblock.h>
#include <pyas/pyasm.h>

int main ( int argc, char *argv[] ) {

  if ( argc < 3 ) {
    fprintf( stderr, "Usage :\n\t%s config/pyas.conf python/fichier.pys\n", argv[ 0 ] );
    exit( EXIT_FAILURE );
  }
  list_t lexem_list = lex(argv[1], argv[2]);
  list_t lexem_list_save = lexem_list;
  py_cb_t codeblock;
  list_t mem=lexem_list;
  if(NULL==(codeblock=parse_pys(&lexem_list_save, &mem))){
  list_print(lexem_list_save, lexem_print);
  printf("\n");
  list_delete(lexem_list, lexem_delete);
  exit(EXIT_FAILURE);
  }
  printf("Parsing du fichier réussi\n");
  py_cb_print(codeblock);
  py_cb_delete(codeblock);
  list_delete(lexem_list, lexem_delete);
  exit( EXIT_SUCCESS );
}
