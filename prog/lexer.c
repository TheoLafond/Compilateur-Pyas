#include <stdlib.h>
#include <stdio.h>

#include <pyas/regexp.h>
#include <pyas/chargroup.h>
#include <pyas/lexem.h>
#include <pyas/list.h>

int main ( int argc, char *argv[] ) {

  if ( argc < 3 ) {
    fprintf( stderr, "Usage :\n\t%s config/pyas.conf  python/fichier.pys\n", argv[ 0 ] );
    exit( EXIT_FAILURE );
  }
  list_t lexem_list = lex(argv[1], argv[2]);
  list_print_soutenance(lexem_list, lexem_print);
  printf("\n");
  list_delete(lexem_list, lexem_delete);
  exit( EXIT_SUCCESS );
}
