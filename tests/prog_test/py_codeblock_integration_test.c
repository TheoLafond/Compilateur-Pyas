#include <stdlib.h>
#include <stdio.h>

// #include <pyas/regexp.h>
// #include <pyas/chargroup.h>
// #include <pyas/lexem.h>
// #include <pyas/list.h>
// #include <pyas/parsing.h>
// #include <pyas/py_codeblock.h>
#include <pyas/pyasm.h>
#include <pyas/all.h>
int main ( int argc, char *argv[] ) {

  if ( argc < 3 ) {
    fprintf( stderr, "Usage :\n\t%s regexp text\n", argv[ 0 ] );
    exit( EXIT_FAILURE );
  }
  list_t lexem_list = lex(argv[1], argv[2]);
  list_t lexem_list_save = lexem_list;
  // list_print(lexem_list, lexem_print);
  printf("\n");

  py_cb_t codeblock;
  list_t mem=lexem_list;
  if(NULL==(codeblock=parse_pys(&lexem_list_save, &mem))){
  //list_print(lexem_list, lexem_print);
  printf("\n");
  list_delete(lexem_list, lexem_delete);
  exit(EXIT_FAILURE);
  }
  codeblock_fill(&mem,&codeblock,NULL);
  py_cb_print(codeblock);
  printf("\n");
  list_delete(lexem_list, lexem_delete);
  exit( EXIT_SUCCESS );
}
