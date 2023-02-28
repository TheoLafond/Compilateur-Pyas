#include <stdlib.h>
#include <stdio.h>
#include <pyas/chargroup.h>
#include <pyas/regexp.h>
#include <pyas/list.h>
int main ( int argc, char *argv[] ) {
  if ( argc < 3 ) {
    fprintf( stderr, "Usage :\n\t%s regexp source\n", argv[ 0 ] );
    exit( EXIT_FAILURE );
  }
  list_t list =chargroup_parse( argv[ 1 ]);
  if ( list ) {
    char *end=argv[2];
    if(re_match_print(list,argv[2],&end)){
      printf( "The start of '%s' is %s, %s: '%s'.\n", argv[2], argv[ 1 ], *end ? "next" : "END", end );
    }
    else {
      printf( "The start of '%s' is *NOT* %s.\n", argv[2], argv[ 1 ] );
    }
    list_delete(list,delete_chargroup);
    exit( EXIT_SUCCESS );
  }
  printf( "Error parsing regexp.\n" );
  exit( EXIT_FAILURE);
}
