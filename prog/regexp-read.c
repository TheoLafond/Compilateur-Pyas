#include <stdlib.h>
#include <stdio.h>
#include <pyas/chargroup.h>
int main ( int argc, char *argv[] ) {
  if ( argc < 2 ) {
    fprintf( stderr, "Usage :\n\t%s regexp\n", argv[ 0 ] );
    exit( EXIT_FAILURE );
  }
  list_t list =chargroup_parse( argv[ 1 ]);
  if ( list ) {
    list_print_regexp(list,print_chargroup);
    list_delete(list,delete_chargroup);
    printf("\n");
    exit( EXIT_SUCCESS );
  }
  printf( "Error parsing regexp.\n" );
  exit( EXIT_FAILURE);
}
