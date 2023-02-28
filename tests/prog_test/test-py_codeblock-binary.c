#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <pyas/list.h>
#include <pyas/pyobj.h>
#include <pyas/py_codeblock.h>
#include <pyas/lexem.h>
#include <pyas/parsing.h>
#include <pyas/pyasm.h>


int main( int argc, char *argv[] ){

  char* filename = "binary.pyc";
  FILE* binary_file = fopen(filename, "w+");
  if (!binary_file) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  if ( argc < 2 ) {
    fprintf( stderr, "Usage :\n\t%s python/fichier.pys\n", argv[ 0 ] );
    exit( EXIT_FAILURE );
  }
  list_t lexem_list = lex("definitionLexeme.txt", argv[1]);
  list_t lexem_list_save = lexem_list;

  py_cb_t codeblock;
  list_t mem=lexem_list;
  if(NULL==(codeblock=parse_pys(&lexem_list_save, &mem))){
  exit(EXIT_FAILURE);
  }
  codeblock_fill(&mem,&codeblock,NULL);

  py_cb_to_binary(binary_file, codeblock);

  list_delete(lexem_list, lexem_delete);
  fclose(binary_file);

  exit( EXIT_SUCCESS );
}
